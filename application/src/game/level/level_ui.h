/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "utils/text.h"
#include "utils/gizmos.h"
#include "game/game_common.h"

typedef struct LevelScene LevelScene;

typedef struct LevelUI
{
    /// @brief Pointeur vers la scène du niveau.
    LevelScene *m_scene;

    /// @brief Tableau de textes indiquant la santé de chaque joueur.
    Text *m_healths[MAX_PLAYER_COUNT];

    /// @brief Texte "Pause".
    Text *m_textPause;

    /// @brief Hauteur du texte indiquant la santé (en pixels).
    int m_hHealth;

    /// @brief Largeur du texte indiquant la santé (en pixels).
    int m_wHealth;

    /// @brief Booléen indiquant si le niveau est en pause.
    bool m_paused;
} LevelUI;

LevelUI *LevelUI_create(LevelScene *scene);
void LevelUI_destroy(LevelUI *self);

void LevelUI_render(LevelUI *self);
void LevelUI_update(LevelUI *self);
void LevelUI_drawGizmos(LevelUI *self, Gizmos *gizmos);
