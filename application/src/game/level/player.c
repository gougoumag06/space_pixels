/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/level/player.h"
#include "game/level/level_scene.h"

Player *Player_create(LevelScene *levelScene, int playerID)
{
    AssetManager *assets = LevelScene_getAssetManager(levelScene);

    Player *self = (Player *)calloc(1, sizeof(Player));
    AssertNew(self);

    self->m_scene = levelScene;
    self->m_position = Vec2_set(4.f, 4.5f + playerID);
    self->m_radius = 0.15f;
    self->m_playerID = playerID;
    self->m_state = PLAYER_STATE_FLYING;
    self->m_hp = PLAYER_MAX_HP;

    /* TODO : Affichage du joueur
    SpriteSheet *spriteSheet = NULL;
    spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_PLAYER_POWERING);
    self->m_animEngine = SpriteAnim_create(spriteSheet->rectCount, 0.3f, -1);
    //*/

    return self;
}

void Player_destroy(Player *self)
{
    if (!self) return;

    /* TODO : Affichage du joueur
    SpriteAnim_destroy(self->m_animEngine);
    //*/

    free(self);
}

void Player_update(Player *self)
{
    LevelScene *scene = self->m_scene;
    Level *level = LevelScene_getLevel(scene);
    PlayerInput playerInput = Player_getInput(self);
    float delta = Timer_getDelta(g_time);

    /* TODO : Déplacement du joueur
    // Mise à jour de la vitesse en fonction de l'état des touches
    Vec2 velocity = Vec2_scale(playerInput.axis, 3.f);

    // Mise à jour de la position
    // Nouvelle pos. = ancienne pos. + (vitesse * temps écoulé) 
    self->m_position = Vec2_add(
        self->m_position,
        Vec2_scale(velocity, delta)
    );
    //*/

    // Bullet par défaut
    /* TODO : Tir du joueur V1
    if (playerInput.shootPressed)
    {
        Vec2 velocity = Vec2_set(8.0f, 0.0f);
        Bullet *bullet = Bullet_create(
            self->m_scene, self->m_position, velocity,
            BULLET_PLAYER_DEFAULT, 90.0f, DAMAGE_SMALL, self->m_playerID);
        LevelScene_addBullet(self->m_scene, bullet);
    }
    //*/
    /* TODO : Tir du joueur V2
    if (playerInput.triggerR > 0.f || playerInput.shootDown)
    {
        const float bulletTime = 0.2f;
        self->m_accuBullet += delta;
        if (self->m_accuBullet > bulletTime)
        {
            self->m_accuBullet = fmodf(self->m_accuBullet, bulletTime);

            Vec2 velocity = Vec2_set(8.0f, 0.0f);
            Bullet *bullet = Bullet_create(
                self->m_scene, self->m_position, velocity,
                BULLET_PLAYER_DEFAULT, 90.0f, DAMAGE_SMALL, self->m_playerID);
            LevelScene_addBullet(self->m_scene, bullet);

            AssetManager *assets = LevelScene_getAssetManager(scene);
            Game_playSoundFX(assets, SOUND_PLAYER_FIRE);
        }
    }
    //*/

    /* TODO : Affichage du joueur
    SpriteAnim_update(self->m_animEngine, delta);
    //*/
}

void Player_render(Player *self)
{
    // On récupère des infos essentielles
    LevelScene *scene = self->m_scene;
    AssetManager *assets = LevelScene_getAssetManager(scene);
    Camera *camera = LevelScene_getCamera(scene);
    double angle = 90.0;
    SpriteSheet *spriteSheet = NULL;
    int index = 0;

    // On calcule la position en pixels en fonction de la position 
    // en tuiles, la taille de la fenêtre et la taille des textures.
    float scale = Camera_getWorldToViewScale(camera);
    SDL_FRect dst = { 0 };
    dst.h = 48 * PIX_TO_WORLD * scale;
    dst.w = 48 * PIX_TO_WORLD * scale;
    Camera_worldToView(camera, self->m_position, &dst.x, &dst.y);
    // Le point de référence est le centre de l'objet
    dst.x -= 0.50f * dst.w;
    dst.y -= 0.50f * dst.h;

    /* TODO : Affichage du joueur
    // Vaisseau
    spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_PLAYER);
    index = 0;
    SpriteSheet_renderCopyF(spriteSheet, index, g_renderer, &dst, angle, NULL, 0);

    // Propulsion - flammes
    spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_PLAYER_POWERING);
    index = 0;
    SpriteSheet_renderCopyF(spriteSheet, index, g_renderer, &dst, angle, NULL, 0);

    // Réacteurs
    spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_PLAYER_ENGINE);
    index = 0;
    SpriteSheet_renderCopyF(spriteSheet, index, g_renderer, &dst, angle, NULL, 0);
    //*/
}

void Player_drawGizmos(Player *self, Gizmos *gizmos)
{
    Gizmos_drawCircle(gizmos, self->m_position, self->m_radius);
}

void Player_damage(Player *self, int damage)
{
    if (self->m_state != PLAYER_STATE_FLYING) return;
    self->m_hp -= damage;
}

PlayerInput Player_getInput(Player *self)
{
    Input *input = LevelScene_getInput(self->m_scene);
    return input->players[self->m_playerID];
}
