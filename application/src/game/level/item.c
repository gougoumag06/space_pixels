/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/level/item.h"
#include "game/level/level_scene.h"

Item *Item_create(LevelScene *scene, int type, Vec2 position)
{
    Item *self = (Item *)calloc(1, sizeof(Item));
    AssertNew(self);

    self->m_scene = scene;
    self->m_position = position;
    self->m_type = type;
    self->m_state = ITEM_STATE_ACTIVE;

    self->m_extent = Vec2_set(16 * PIX_TO_WORLD, 16 * PIX_TO_WORLD);
    self->m_radius = 1.f;

    //AssetManager *assets = LevelScene_getAssetManager(self->m_scene);
    //self->m_spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_ID_TO_DEFINE);
    //self->m_anim = SpriteAnim_create(self->m_spriteSheet->rectCount, 1.0f, -1);

    return self;
}

void Item_destroy(Item *self)
{
    if (!self) return;

    SpriteAnim_destroy(self->m_anim);
    free(self);
}

void Item_update(Item *self)
{
    LevelScene *scene = self->m_scene;
    float delta = Timer_getDelta(g_time);
    SpriteAnim_update(self->m_anim, delta);
}

void Item_render(Item *self)
{
}

void Item_pickUp(Item *self, Player *player)
{
}

void Item_drawGizmos(Item *self, Gizmos *gizmos)
{
    Gizmos_drawCircle(gizmos, self->m_position, self->m_radius);
}
