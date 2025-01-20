/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "utils/math.h"
#include "utils/sprite_anim.h"
#include "utils/gizmos.h"
#include "game/game_common.h"

typedef struct LevelScene LevelScene;
typedef struct Player Player;

typedef enum ItemType
{
    ITEM_AUTO,
    ITEM_ROCKET,
    ITEM_CURE,
} ItemType;

typedef enum ItemState
{
    ITEM_STATE_ACTIVE,
    ITEM_STATE_PICKED_UP,
} ItemState;

typedef struct Item
{
    /// @brief Pointeur vers la scène du niveau.
    LevelScene *m_scene;

    /// @brief Position dans le référentiel monde.
    Vec2 m_position;

    /// @brief Dimensions du sprite dans le réferentiel monde.
    Vec2 m_extent;

    /// @brief Rayon du cercle de collision dans le référentiel monde.
    float m_radius;

    /// @brief Type de l'objet.
    /// Les valeurs possibles sont données dans ItemType.
    int m_type;

    /// @brief Etat de l'objet.
    /// Les valeurs possibles sont données dans ItemState.
    int m_state;

    /// @brief Sprite sheet associée à l'objet
    SpriteSheet *m_spriteSheet;

    /// @brief Animation associée à la sprite sheet.
    SpriteAnim *m_anim;
} Item;

Item *Item_create(LevelScene *scene, int type, Vec2 position);
void Item_destroy(Item *self);

void Item_update(Item *self);
void Item_render(Item *self);
void Item_pickUp(Item *self, Player *player);
void Item_drawGizmos(Item *self, Gizmos *gizmos);

INLINE bool Item_shouldBeDestroyed(Item *self)
{
    return self->m_state == ITEM_STATE_PICKED_UP;
}

