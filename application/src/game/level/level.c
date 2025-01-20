/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/level/level.h"
#include "game/level/level_scene.h"

static void Level_updateLevel1(Level *self)
{
    Enemy *enemy = NULL;
    LevelScene *scene = self->m_scene;
    switch (self->m_waveIdx)
    {
    case 0:
        enemy = Enemy_create(scene, ENEMY_TYPE_FIGHTER, Vec2_set(9.0f, 4.5f));
        LevelScene_addEnemy(scene, enemy);
        break;

        /* TODO : Ajouter une vague d'ennemis
    case 1:
        enemy = Enemy_create(scene, ENEMY_TYPE_FIGHTER, Vec2_set(10.0f, 3.0f));
        LevelScene_addEnemy(scene, enemy);
        enemy = Enemy_create(scene, ENEMY_TYPE_FIGHTER, Vec2_set(10.0f, 6.0f));
        LevelScene_addEnemy(scene, enemy);
        //*/
        break;

    default:
        self->m_state = LEVEL_STATE_COMPLETED;
        return;
    }

    self->m_waveIdx++;
}


static void Level_updateLevel2(Level *self)
{
    Enemy *enemy = NULL;
    LevelScene *scene = self->m_scene;
    switch (self->m_waveIdx)
    {
    case 0:
        enemy = Enemy_create(scene, ENEMY_TYPE_FIGHTER, Vec2_set(10.0f, 3.0f));
        LevelScene_addEnemy(scene, enemy);
        enemy = Enemy_create(scene, ENEMY_TYPE_FIGHTER, Vec2_set(10.0f, 6.0f));
        LevelScene_addEnemy(scene, enemy);
        break;

    case 1:
        enemy = Enemy_create(scene, ENEMY_TYPE_FIGHTER, Vec2_set(11.0f, 2.0f));
        LevelScene_addEnemy(scene, enemy);
        enemy = Enemy_create(scene, ENEMY_TYPE_FIGHTER, Vec2_set(9.0f, 4.5f));
        LevelScene_addEnemy(scene, enemy);
        enemy = Enemy_create(scene, ENEMY_TYPE_FIGHTER, Vec2_set(11.0f, 7.0f));
        LevelScene_addEnemy(scene, enemy);
        break;

    default:
        self->m_state = LEVEL_STATE_COMPLETED;
        return;
    }

    self->m_waveIdx++;
}

Level *Level_create(LevelScene *scene, int levelID)
{
    Level *self = (Level *)calloc(1, sizeof(Level));
    AssertNew(self);

    self->m_scene = scene;
    self->m_levelID = levelID;
    self->m_waveIdx = 0;
    self->m_state = LEVEL_STATE_PLAYING;

    AssetManager *assets = LevelScene_getAssetManager(scene);
    switch (levelID)
    {
    default:
    case LEVEL_1:
        self->m_backgroundID = SPRITE_BACKGROUND_BLUE_NEBULA;
        break;
        // TODO LEVEL 2
    }

    Game_playMusic(assets, MUSIC_LUMINARES);

    return self;
}

void Level_destroy(Level *self)
{
    if (!self) return;
    free(self);
}

void Level_update(Level *self)
{
    LevelScene *scene = self->m_scene;

    if ((self->m_state == LEVEL_STATE_COMPLETED) ||
        (self->m_state == LEVEL_STATE_FAILED))
        return;

    if (self->m_state == LEVEL_STATE_PLAYING)
    {
        bool gameOver = true;
        for (int i = 0; i < LevelScene_getPlayerCount(scene); i++)
        {
            Player *player = LevelScene_getPlayer(scene, i);
            if (player->m_state != PLAYER_STATE_DEAD) gameOver = false;
        }

        if (gameOver)
        {
            self->m_state = LEVEL_STATE_FAILED;
            return;
        }
        
        // On ne change pas de vague tant qu'il y a des enemies
        if (LevelScene_getEnemyCount(scene) > 0) return;

        switch (self->m_levelID)
        {
        default:
        case LEVEL_1: Level_updateLevel1(self); break;
        case LEVEL_2: Level_updateLevel2(self); break;
        }
    }
}

void Level_renderBackground(Level *self)
{
    assert(self);
    LevelScene *scene = self->m_scene;
    AssetManager *assets = LevelScene_getAssetManager(scene);
    Camera *camera = LevelScene_getCamera(scene);
    SpriteSheet *spriteSheet = NULL;
    float scale = Camera_getWorldToViewScale(camera);

    SDL_FRect dstFRect = { 0 };
    dstFRect.w = 1024 * PIX_TO_WORLD * scale;
    dstFRect.h = 1024 * PIX_TO_WORLD * scale;

    dstFRect.x = 0;
    spriteSheet = AssetManager_getSpriteSheet(assets, self->m_backgroundID);
    SpriteSheet_renderCopyF(spriteSheet, 0, g_renderer, &dstFRect, 0.0, NULL, 0);

    dstFRect.x += dstFRect.w;
    SpriteSheet_renderCopyF(spriteSheet, 0, g_renderer, &dstFRect, 0.0, NULL, 0);
}

