/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/level/level_scene.h"

/// @brief Met à jour le moteur du niveau de la scène.
/// @param self la scène.
void LevelScene_updateEngine(LevelScene *self);

LevelScene *LevelScene_create(GameConfig *gameConfig)
{
    LevelScene *self = (LevelScene *)calloc(1, sizeof(LevelScene));
    AssertNew(self);

    self->m_assets = AssetManager_create(
        SPRITE_COUNT, FONT_COUNT, SOUND_COUNT, MUSIC_COUNT);
    Game_addAssets(self->m_assets);

    self->m_input = Input_create();
    self->m_gameConfig = gameConfig;
    self->m_camera = Camera_create(Game_getWidth(), Game_getHeight());
    self->m_gizmos = Gizmos_create(self->m_camera);

    self->m_playerCount = gameConfig->playerCount;
    for (int i = 0; i < self->m_playerCount; i++)
    {
        self->m_players[i] = Player_create(self, i);
    }

    self->m_ui = LevelUI_create(self);
    self->m_level = Level_create(self, gameConfig->levelID);
    self->m_state = SCENE_STATE_FADING_IN;
    self->m_fadingTime = 0.5f;
    self->m_isLocked = false;

    self->m_gameConfig->nextScene = GAME_SCENE_TITLE;
    LevelUI_update(self->m_ui);

    return self;
}

void LevelScene_destroy(LevelScene *self)
{
    if (!self) return;

    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        Player_destroy(self->m_players[i]);
    }
    for (int i = 0; i < ENEMY_CAPACITY; i++)
    {
        Enemy_destroy(self->m_enemies[i]);
    }
    for (int i = 0; i < BULLET_CAPACITY; i++)
    {
        Bullet_destroy(self->m_bullets[i]);
    }
    for (int i = 0; i < ITEM_CAPACITY; i++)
    {
        Item_destroy(self->m_items[i]);
    }
    AssetManager_destroy(self->m_assets);
    Camera_destroy(self->m_camera);
    Gizmos_destroy(self->m_gizmos);
    Input_destroy(self->m_input);
    Level_destroy(self->m_level);
    LevelUI_destroy(self->m_ui);

    free(self);
}

void LevelScene_mainLoop(LevelScene *self, bool drawGizmos)
{
    assert(self && "The LevelScene must be created");
    while (true)
    {
        // Met à jour la scène
        Timer_update(g_time);
        LevelScene_update(self);

        Input *input = LevelScene_getInput(self);
        if (input->quitPressed)
        {
            self->m_gameConfig->nextScene = GAME_SCENE_QUIT;
            return;
        }

        if (self->m_state == SCENE_STATE_FINISHED)
            return;

        // Rend la scène
        LevelScene_render(self);

        if (drawGizmos) LevelScene_drawGizmos(self);

        // Affiche le nouveau rendu
        SDL_RenderPresent(g_renderer);
    }
}

void LevelScene_update(LevelScene *self)
{
    assert(self && "The LevelScene must be created");

    Input_update(self->m_input);

    if (self->m_state == SCENE_STATE_RUNNING)
    {
        LevelScene_updateEngine(self);
        Level_update(self->m_level);
    }
    if (self->m_state == SCENE_STATE_FADING_IN)
    {
        self->m_accu += Timer_getUnscaledDelta(g_time);
        if (self->m_accu >= self->m_fadingTime)
        {
            self->m_state = SCENE_STATE_RUNNING;
        }
    }
    if (self->m_state == SCENE_STATE_FADING_OUT)
    {
        self->m_accu += Timer_getUnscaledDelta(g_time);
        if (self->m_accu >= self->m_fadingTime)
        {
            self->m_state = SCENE_STATE_FINISHED;
        }
    }

    LevelUI_update(self->m_ui);
}

void LevelScene_quit(LevelScene *self)
{
    assert(self && "The LevelScene must be created");

    if ((self->m_state == SCENE_STATE_FADING_OUT) ||
        (self->m_state == SCENE_STATE_FINISHED))
        return;

    self->m_state = SCENE_STATE_FADING_OUT;
    self->m_accu = 0.f;
}

void LevelScene_updateEngine(LevelScene *self)
{
    assert(self && "The LevelScene must be created");

    // Met à jour les projectiles
    for (int i = 0; i < BULLET_CAPACITY; i++)
    {
        Bullet *bullet = self->m_bullets[i];
        if (bullet == NULL) continue;

        Bullet_update(bullet);

        bool outOfBounds =
            (bullet->m_position.x < -1.0f) ||
            (bullet->m_position.x > 17.0f) ||
            (bullet->m_position.y < -1.0f) ||
            (bullet->m_position.y > 10.0f);
        if (outOfBounds)
        {
            Bullet_setState(bullet, BULLET_STATE_OUT_OF_BOUNDS);
        }

        int playerID = bullet->m_playerID;
        if (playerID >= 0)
        {
            for (int i = 0; i < ENEMY_CAPACITY; i++)
            {
                Enemy *enemy = self->m_enemies[i];
                if (enemy == NULL) continue;

                float dist = Vec2_distanceSquared(bullet->m_position, enemy->m_position);
                float radius = bullet->m_radius + enemy->m_radius;
                if (dist < radius * radius)
                {
                    int score = Enemy_damage(enemy, bullet->m_damage);
                    Player_addPoints(self->m_players[playerID], score);
                    Bullet_setState(bullet, BULLET_STATE_HIT_TARGET);
                    break;
                }
            }
        }
        else
        {
            for (int i = 0; i < self->m_playerCount; i++)
            {
                Player *player = self->m_players[i];
                if (player->m_state == PLAYER_STATE_DEAD) continue;

                float dist = Vec2_distanceSquared(bullet->m_position, player->m_position);
                float radius = bullet->m_radius + player->m_radius;
                if (dist < radius * radius)
                {
                    Player_damage(player, bullet->m_damage);
                    Bullet_setState(bullet, BULLET_STATE_HIT_TARGET);
                    break;
                }
            }
        }
    }

    // Met à jour les ennemis
    for (int i = 0; i < ENEMY_CAPACITY; i++)
    {
        Enemy *enemy = self->m_enemies[i];
        if (enemy == NULL) continue;

        Enemy_update(enemy);
    }

    // Met à jour les objets
    for (int i = 0; i < ITEM_CAPACITY; i++)
    {
        Item *item = self->m_items[i];
        if (item == NULL) continue;

        Item_update(item);

        for (int i = 0; i < self->m_playerCount; i++)
        {
            Player *player = self->m_players[i];
            if (player->m_state == PLAYER_STATE_DEAD) continue;

            float dist = Vec2_distanceSquared(item->m_position, player->m_position);
            float radius = item->m_radius + player->m_radius;
            if (dist < radius * radius)
            {
                Item_pickUp(item, player);
                break;
            }
        }
    }

    // Met à jour les joueurs
    for (int i = 0; i < self->m_playerCount; i++)
    {
        Player_update(self->m_players[i]);
    }

    self->m_isLocked = true;

    // Détruit les projectiles invalides et réaligne le tableau
    int bulletCount = self->m_bulletCount;
    for (int i = 0; i < bulletCount; i++)
    {
        Bullet *bullet = self->m_bullets[i];
        if (bullet != NULL && Bullet_shouldBeDestroyed(bullet))
        {
            Bullet_destroy(bullet);
            self->m_bullets[i] = NULL;
        }
    }
    bulletCount = 0;
    for (int i = 0; i < BULLET_CAPACITY; i++)
    {
        Bullet *bullet = self->m_bullets[i];
        if (bullet)
        {
            self->m_bullets[i] = NULL;
            self->m_bullets[bulletCount++] = bullet;
        }
    }
    self->m_bulletCount = bulletCount;

    // Détruit les ennemis invalides et réaligne le tableau
    int enemyCount = self->m_enemyCount;
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy *enemy = self->m_enemies[i];
        if (enemy != NULL && Enemy_shouldBeDestroyed(enemy))
        {
            Enemy_destroy(enemy);
            self->m_enemies[i] = NULL;
        }
    }
    enemyCount = 0;
    for (int i = 0; i < ENEMY_CAPACITY; i++)
    {
        Enemy *enemy = self->m_enemies[i];
        if (enemy)
        {
            self->m_enemies[i] = NULL;
            self->m_enemies[enemyCount++] = enemy;
        }
    }
    self->m_enemyCount = enemyCount;

    // Détruit les objets invalides et réaligne le tableau
    int itemCount = self->m_itemCount;
    for (int i = 0; i < ITEM_CAPACITY; i++)
    {
        Item *item = self->m_items[i];
        if (item != NULL && Item_shouldBeDestroyed(item))
        {
            Item_destroy(item);
            self->m_items[i] = NULL;
        }
    }
    itemCount = 0;
    for (int i = 0; i < ITEM_CAPACITY; i++)
    {
        Item *item = self->m_items[i];
        if (item)
        {
            self->m_items[i] = NULL;
            self->m_items[itemCount++] = item;
        }
    }
    self->m_itemCount = itemCount;

    self->m_isLocked = false;
}

void LevelScene_render(LevelScene *self)
{
    assert(self && "The LevelScene must be created");

    // Efface le rendu précédent
    SDL_SetRenderDrawColor(g_renderer, 37, 37, 37, 255);
    SDL_RenderClear(g_renderer);

    SDL_SetHintWithPriority(SDL_HINT_RENDER_SCALE_QUALITY, "0", SDL_HINT_OVERRIDE);

    // Affiche le fond
    Level_renderBackground(self->m_level);

    // Affiche les projectiles
    for (int i = 0; i < self->m_bulletCount; i++)
    {
        assert(self->m_bullets[i]);
        Bullet_render(self->m_bullets[i]);
    }
    // Affiche les objets
    for (int i = 0; i < self->m_itemCount; i++)
    {
        assert(self->m_items[i]);
        Item_render(self->m_items[i]);
    }
    // Affiche les ennemis
    for (int i = 0; i < self->m_enemyCount; i++)
    {
        assert(self->m_enemies[i]);
        Enemy_render(self->m_enemies[i]);
    }
    // Affiche les joueurs
    for (int i = 0; i < self->m_playerCount; i++)
    {
        assert(self->m_players[i]);
        Player_render(self->m_players[i]);
    }
    // Affiche l'interface utilisateur
    LevelUI_render(self->m_ui);

    // Fading
    if ((self->m_state == SCENE_STATE_FADING_IN) ||
        (self->m_state == SCENE_STATE_FADING_OUT))
    {
        int opacity = (int)(255.f * self->m_accu / self->m_fadingTime);
        if (self->m_state == SCENE_STATE_FADING_IN)
        {
            opacity = 255 - opacity;
        }
        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, opacity);
        SDL_RenderFillRect(g_renderer, NULL);
    }
}

void LevelScene_drawGizmos(LevelScene *self)
{
    assert(self && "The scene LevelScene be created");
    Gizmos *gizmos = self->m_gizmos;

    // Projectiles
    Gizmos_setColor(gizmos, g_colors.yellow);
    for (int i = 0; i < self->m_bulletCount; i++)
    {
        assert(self->m_bullets[i]);
        Bullet_drawGizmos(self->m_bullets[i], gizmos);
    }

    // Objets
    Gizmos_setColor(gizmos, g_colors.green);
    for (int i = 0; i < self->m_itemCount; i++)
    {
        assert(self->m_items[i]);
        Item_drawGizmos(self->m_items[i], gizmos);
    }

    // Ennemis
    Gizmos_setColor(gizmos, g_colors.magenta);
    for (int i = 0; i < self->m_enemyCount; i++)
    {
        assert(self->m_enemies[i]);
        Enemy_drawGizmos(self->m_enemies[i], gizmos);
    }

    // Joueurs
    Gizmos_setColor(gizmos, g_colors.cyan);
    for (int i = 0; i < self->m_playerCount; i++)
    {
        assert(self->m_players[i]);
        Player_drawGizmos(self->m_players[i], gizmos);
    }

    // Interface utilisateur
    LevelUI_drawGizmos(self->m_ui, gizmos);
}

void LevelScene_addBullet(LevelScene *self, Bullet *bullet)
{
    assert(self && "The LevelScene must be created");
    assert(bullet && "The bullet Bullet be created");
    assert(self->m_isLocked == false);
    if (self->m_bulletCount < BULLET_CAPACITY)
    {
        self->m_bullets[self->m_bulletCount++] = bullet;
        return;
    }
    assert(false && "BULLET_CAPACITY exceeded");
    Bullet_destroy(self->m_bullets[0]);
    self->m_bullets[0] = bullet;
}

void LevelScene_addEnemy(LevelScene *self, Enemy *enemy)
{
    assert(self && "The LevelScene must be created");
    assert(enemy && "The Enemy must be created");
    assert(self->m_isLocked == false);
    if (self->m_enemyCount < ENEMY_CAPACITY)
    {
        self->m_enemies[self->m_enemyCount++] = enemy;
        return;
    }
    assert(false && "ENEMY_CAPACITY exceeded");
    Enemy_destroy(self->m_enemies[0]);
    self->m_enemies[0] = enemy;
}

void LevelScene_addItem(LevelScene *self, Item *item)
{
    assert(self && "The LevelScene must be created");
    assert(item && "The Item must be created");
    assert(self->m_isLocked == false);
    if (self->m_itemCount < ITEM_CAPACITY)
    {
        self->m_items[self->m_itemCount++] = item;
        return;
    }
    assert(false && "ITEM_CAPACITY exceeded");
    Item_destroy(self->m_items[0]);
    self->m_items[0] = item;
}
