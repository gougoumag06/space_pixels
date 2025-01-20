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
#include "game/input.h"

#define PLAYER_MAX_HP 100

typedef struct LevelScene LevelScene;

typedef enum PlayerState
{
    PLAYER_STATE_FLYING,
    PLAYER_STATE_DYING,
    PLAYER_STATE_DEAD,
} PlayerState;

typedef struct Player
{
    /// @brief Pointeur vers la scène du niveau.
    LevelScene *m_scene;

    /// @brief Position dans le référentiel monde.
    Vec2 m_position;

    /// @brief Vitesse dans le référentiel monde.
    Vec2 m_velocity;

    /// @brief Rayon du cercle de collision dans le référentiel monde.
    float m_radius;

    /// @brief Etat du joueur.
    /// Les valeurs possibles sont données dans PlayerState.
    int m_state;

    /// @brief Points de vie du joueur.
    int m_hp;

    /// @brief Identifiant du joueur (entre 0 et playerCount - 1).
    int m_playerID;

    /// @brief Score du joueur.
    int m_score;

    /// @brief Animation des réacteurs.
    //SpriteAnim *m_animEngine;

    /// @brief Accumulateur pour les tirs par défaut.
    float m_accuBullet;
} Player;

Player *Player_create(LevelScene *levelScene, int playerID);
void Player_destroy(Player *self);

void Player_update(Player *self);
void Player_render(Player *self);
void Player_drawGizmos(Player *self, Gizmos *gizmos);

void Player_damage(Player *self, int damage);

INLINE void Player_addPoints(Player *self, int points)
{
    self->m_score += points;
    if (self->m_score < 0) self->m_score = 0;
}

INLINE int player_getScore(Player *self)
{
    return self->m_score;
}

PlayerInput Player_getInput(Player *self);
