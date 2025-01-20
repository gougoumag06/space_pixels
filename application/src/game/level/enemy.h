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

typedef enum EnemyState
{
    ENEMY_STATE_SHOWING,
    ENEMY_STATE_FIRING,
    ENEMY_STATE_DYING,
    ENEMY_STATE_DEAD,
} EnemyState;

typedef enum EnemyType
{
    ENEMY_TYPE_FIGHTER,
    //ENEMY_TYPE_DREADNOUGHT,
    //ENEMY_TYPE_BATTLECRUISER,
    //ENEMY_SCOUT,
} EnemyType;

typedef struct Enemy
{
    /// @brief Pointeur vers la scène du niveau.
    LevelScene *m_scene;

    /// @brief Position dans le référentiel monde.
    Vec2 m_position;

    /// @brief Dimensions du sprite dans le réferentiel monde.
    Vec2 m_extent;

    /// @brief Rayon du cercle de collision dans le référentiel monde.
    float m_radius;

    /// @brief Type de l'ennmi.
    /// Les valeurs possibles sont données dans EnemyType.
    int m_type;

    /// @brief Etat de l'ennmi.
    /// Les valeurs possibles sont données dans EnemyState.
    int m_state;

    /// @brief Points de vie de l'ennemi.
    int m_hp;

    /// @brief Sprite sheet associée à l'attaque.
    //SpriteSheet *m_firingSpriteSheet;

    /// @brief Sprite sheet associée à l'explosion de l'ennemi quand il est vaincu.
    //SpriteSheet *m_dyingSpriteSheet;

    /// @brief Animation associée à l'attaque.
    //SpriteAnim *m_firingAnim;

    /// @brief Animation associée à l'explosion de l'ennemi quand il est vaincu.
    //SpriteAnim *m_dyingAnim;
} Enemy;

Enemy *Enemy_create(LevelScene *scene, int type, Vec2 position);
void Enemy_destroy(Enemy *self);

void Enemy_update(Enemy *self);
void Enemy_render(Enemy *self);
int Enemy_damage(Enemy *self, int damage);
void Enemy_drawGizmos(Enemy *self, Gizmos *gizmos);

void Enemy_updateFigther(Enemy *self);

INLINE bool Enemy_shouldBeDestroyed(Enemy *self)
{
    return (self->m_state == ENEMY_STATE_DEAD);
}
