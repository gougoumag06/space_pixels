/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "game/game_common.h"

typedef struct LevelScene LevelScene;

typedef enum LevelState
{
    LEVEL_STATE_STARTING,
    LEVEL_STATE_PLAYING,
    LEVEL_STATE_COMPLETED,
    LEVEL_STATE_FAILED
} LevelState;

typedef struct Level
{
    /// @brief Pointeur vers la scène du niveau.
    LevelScene *m_scene;

    /// @brief Etat du niveau.
    /// Les valeurs possibles sont données dans LevelState.
    int m_state;

    /// @brief Identifiant du niveau.
    /// Les valeurs possibles sont données dans LevelID.
    int m_levelID;

    /// @brief Indice de la vague d'ennemis.
    int m_waveIdx;

    /// @brief Identifiant de l'image de fond du niveau.
    int m_backgroundID;
} Level;

Level *Level_create(LevelScene *scene, int levelID);
void Level_destroy(Level *self);
void Level_update(Level *self);
void Level_renderBackground(Level *self);

INLINE int Level_getState(Level *self)
{
    return self->m_state;
}
