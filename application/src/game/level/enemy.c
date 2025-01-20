/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/level/enemy.h"
#include "game/level/level_scene.h"

Enemy *Enemy_create(LevelScene *scene, int type, Vec2 position)
{
    Enemy *self = (Enemy *)calloc(1, sizeof(Enemy));
    AssertNew(self);

    self->m_scene = scene;
    self->m_type = type;
    self->m_state = ENEMY_STATE_FIRING;
    self->m_position = position;

    AssetManager *assets = LevelScene_getAssetManager(self->m_scene);
    switch (type)
    {
    default:
    case ENEMY_TYPE_FIGHTER:
        self->m_hp = 10;
        self->m_extent = Vec2_set(64 * PIX_TO_WORLD, 64 * PIX_TO_WORLD);
        self->m_radius = 1.25f;

        /* TODO : Affichage d'un ennemi
        self->m_firingSpriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_FIGHTER_FIRING);
        self->m_firingAnim = SpriteAnim_create(self->m_firingSpriteSheet->rectCount, 1.5f, -1);
        self->m_dyingSpriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_FIGHTER_DYING);
        self->m_dyingAnim = SpriteAnim_create(self->m_dyingSpriteSheet->rectCount, 0.8f, 1);
        //*/
        break;
    }

    return self;
}

void Enemy_destroy(Enemy *self)
{
    if (!self) return;

    /* TODO : Affichage d'un ennemi
    SpriteAnim_destroy(self->m_firingAnim);
    SpriteAnim_destroy(self->m_dyingAnim);
    //*/

    free(self);
}

void Enemy_update(Enemy *self)
{
    LevelScene *scene = self->m_scene;
    float shootTime = 2.0f;
    float delta = Timer_getDelta(g_time);

    /* TODO : Affichage d'un ennemi
    if (self->m_state == ENEMY_STATE_FIRING)
    {
        SpriteAnim_update(self->m_firingAnim, delta);
    }
    else if (self->m_state == ENEMY_STATE_DYING)
    {
        SpriteAnim_update(self->m_dyingAnim, delta);
        //if (SpriteAnim_isFinished(self->m_dyingAnim))
        //{
        //    self->m_state = ENEMY_STATE_DEAD;
        //    return;
        //}
    }
    //*/

    switch (self->m_type)
    {
    case ENEMY_TYPE_FIGHTER:
        /* TODO : Tir d'un ennemi
        Enemy_updateFigther(self);
        //*/
        break;
    default: break;
    }
}

void Enemy_updateFigther(Enemy *self)
{
    if (self->m_state != ENEMY_STATE_FIRING)
        return;

    LevelScene *scene = self->m_scene;
    AssetManager *assets = LevelScene_getAssetManager(scene);

    /* TODO : Tir d'un ennemi
    if (SpriteAnim_frameChanged(self->m_firingAnim) == false) return;

    int idx = SpriteAnim_getFrameIndex(self->m_firingAnim);
    if (idx == 1)
    {
        Vec2 velocity = Vec2_set(-3.5f, 0.0f);
        Vec2 position = Vec2_add(self->m_position, Vec2_set(-6 * PIX_TO_WORLD, +6 * PIX_TO_WORLD));
        Bullet *bullet = Bullet_create(scene, position, velocity, BULLET_FIGHTER, -90.0f, DAMAGE_SMALL, -1);
        LevelScene_addBullet(scene, bullet);
        Game_playSoundFX(assets, SOUND_ENEMY_FIRE);
    }
    else if (idx == 3)
    {
        Vec2 velocity = Vec2_set(-3.5f, 0.0f);
        Vec2 position = Vec2_add(self->m_position, Vec2_set(-6 * PIX_TO_WORLD, -6 * PIX_TO_WORLD));
        Bullet *bullet = Bullet_create(scene, position, velocity, BULLET_FIGHTER, -90.0f, DAMAGE_SMALL, -1);
        LevelScene_addBullet(scene, bullet);
        Game_playSoundFX(assets, SOUND_ENEMY_FIRE);
    }
    //*/
}

void Enemy_render(Enemy *self)
{
    LevelScene *scene = self->m_scene;
    AssetManager *assets = LevelScene_getAssetManager(scene);
    Camera *camera = LevelScene_getCamera(scene);
    SDL_Rect *src = NULL;
    SDL_Texture *texture = NULL;
    SpriteSheet *spriteSheet = NULL;
    int index = 0;

    float scale = Camera_getWorldToViewScale(camera);
    SDL_FRect dst = { 0 };
    dst.h = self->m_extent.y * scale;
    dst.w = self->m_extent.x * scale;
    Camera_worldToView(camera, self->m_position, &dst.x, &dst.y);
    dst.x -= 0.50f * dst.w;
    dst.y -= 0.50f * dst.h;

    /* TODO : Affichage d'un ennemi
    if ((self->m_state == ENEMY_STATE_FIRING) ||
        (self->m_state == ENEMY_STATE_SHOWING))
    {
        spriteSheet = self->m_firingSpriteSheet;
        index = SpriteAnim_getFrameIndex(self->m_firingAnim);
        src = &(spriteSheet->rects[index]);
        texture = spriteSheet->texture;
        SDL_RenderCopyExF(g_renderer, texture, src, &dst, -90.0, NULL, 0);
    }
    else if (self->m_state == ENEMY_STATE_DYING)
    {
        spriteSheet = self->m_dyingSpriteSheet;
        index = SpriteAnim_getFrameIndex(self->m_dyingAnim);
        src = &(spriteSheet->rects[index]);
        texture = spriteSheet->texture;
        SDL_RenderCopyExF(g_renderer, texture, src, &dst, -90.0, NULL, 0);
    }
    //*/
}
int Enemy_damage(Enemy *self, int damage)
{
    if (self->m_state == ENEMY_STATE_DYING ||
        self->m_state == ENEMY_STATE_DEAD)
    {
        return 0;
    }

    /* TODO : Dommages à un ennemi
    self->m_hp -= damage;

    LevelScene *scene = self->m_scene;
    AssetManager *assets = LevelScene_getAssetManager(scene);

    int score = damage;
    if (self->m_hp <= 0)
    {
        self->m_state = ENEMY_STATE_DYING;

        SpriteAnim_restart(self->m_dyingAnim);
        Game_playSoundFX(assets, SOUND_ENEMY_DIYNG);
    }

    return score;
    //*/
    return 0;
}

void Enemy_drawGizmos(Enemy *self, Gizmos *gizmos)
{
    Gizmos_drawCircle(gizmos, self->m_position, self->m_radius);
}
