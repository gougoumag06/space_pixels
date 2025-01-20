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

typedef enum BulletType
{
    BULLET_FIGHTER,
    BULLET_PLAYER_DEFAULT,
} BulletType;

typedef enum BulletState
{
    BULLET_STATE_ACTIVE,
    BULLET_STATE_OUT_OF_BOUNDS,
    BULLET_STATE_HIT_TARGET
} BulletState;

typedef struct Bullet
{
    /// @brief Pointeur vers la scène du niveau.
    LevelScene *m_scene;

    /// @brief Position dans le référentiel monde.
    Vec2 m_position;

    /// @brief Vitesse dans le référentiel monde.
    Vec2 m_velocity;

    /// @brief Dimensions du sprite dans le réferentiel monde.
    Vec2 m_extent;

    /// @brief Rayon du cercle de collision dans le référentiel monde.
    float m_radius;

    /// @brief Indice du joueur qui a tiré ou -1 s'il s'agit d'un ennemi.
    /// Permet d'attribuer un score à chaque joueur.
    int m_playerID;

    /// @brief Type du projectile.
    /// Les valeurs possibles sont données dans BulletType.
    int m_type;

    /// @brief Etat du projectile.
    /// Les valeurs possibles sont données dans BulletState.
    int m_state;

    /// @brief Dommages infligés par le projectile.
    int m_damage;

    /// @brief Angle de rendu du sprite.
    float m_angle;

    /// @brief Sprite sheet associée au projectile
    SpriteSheet *m_spriteSheet;

    /// @brief Animation associée à la sprite sheet.
    // SpriteAnim *m_anim;
} Bullet;

Bullet *Bullet_create(
    LevelScene *scene, Vec2 position, Vec2 velocity,
    int type, float angle, int damage, int playerID);
void Bullet_destroy(Bullet *self);
void Bullet_update(Bullet *self);

void Bullet_render(Bullet *self);
void Bullet_drawGizmos(Bullet *self, Gizmos *gizmos);

INLINE void Bullet_setState(Bullet *self, int state)
{
    self->m_state = state;
}

INLINE bool Bullet_shouldBeDestroyed(Bullet *self)
{
    return (self->m_state == BULLET_STATE_OUT_OF_BOUNDS)
        || (self->m_state == BULLET_STATE_HIT_TARGET);
}

