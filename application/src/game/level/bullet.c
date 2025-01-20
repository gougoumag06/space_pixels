/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/level/bullet.h"
#include "game/level/level_scene.h"
#include "utils/common.h"

Bullet *Bullet_create(LevelScene *scene, Vec2 position, Vec2 velocity, int type, float angle, int damage, int playerID)
{
    Bullet *self = (Bullet *)calloc(1, sizeof(Bullet));
    AssertNew(self);

    self->m_position = position;
    self->m_velocity = velocity;
    self->m_type = type;
    self->m_scene = scene;
    self->m_angle = angle;
    self->m_damage = damage;
    self->m_playerID = playerID;
    self->m_state = BULLET_STATE_ACTIVE;

    AssetManager *assets = LevelScene_getAssetManager(scene);
    switch (type)
    {
    default:
    case BULLET_PLAYER_DEFAULT:
        /* TODO : Tir du joueur
        self->m_spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_BULLET_PLAYER_DEFAULT);
        //*/
        self->m_extent = Vec2_set(8 * PIX_TO_WORLD, 16 * PIX_TO_WORLD);
        self->m_radius = 0.05f;
        // self->m_anim = SpriteAnim_create(self->m_spriteSheet->rectCount, 0.4f, -1);
        break;
    }

    return self;
}

void Bullet_destroy(Bullet *self)
{
    if (!self) return;

    // SpriteAnim_destroy(self->m_anim);
    free(self);
}

void Bullet_update(Bullet *self)
{
    float delta = Timer_getDelta(g_time);

    // Met à jour la position
    /* TODO : Tir du joueur
    self->m_position = Vec2_add(
        self->m_position,
        Vec2_scale(self->m_velocity, delta)
    );
    //*/

    // Met à jour l'animation
    // if (self->m_anim) SpriteAnim_update(self->m_anim, delta);
}

void Bullet_render(Bullet *self)
{
    assert(self);
    LevelScene *scene = self->m_scene;
    AssetManager *assets = LevelScene_getAssetManager(scene);
    Camera *camera = LevelScene_getCamera(scene);

    float scale = Camera_getWorldToViewScale(camera);
    SDL_FRect dst = { 0 };
    dst.h = self->m_extent.y * scale;
    dst.w = self->m_extent.x * scale;
    Camera_worldToView(camera, self->m_position, &dst.x, &dst.y);
    dst.x -= 0.50f * dst.w;
    dst.y -= 0.50f * dst.h;

    SDL_Rect *src = NULL;
    SDL_Texture *texture = NULL;
    SpriteSheet *spriteSheet = NULL;
    int index = 0;

    /* TODO : Tir du joueur
    spriteSheet = self->m_spriteSheet;
    index = 0;//SpriteAnim_getFrameIndex(self->m_anim);
    src = &(spriteSheet->rects[index]);
    texture = spriteSheet->texture;
    SDL_RenderCopyExF(g_renderer, texture, src, &dst, self->m_angle, NULL, 0);
    //*/
}

void Bullet_drawGizmos(Bullet *self, Gizmos *gizmos)
{
    Gizmos_drawCircle(gizmos, self->m_position, self->m_radius);
}
