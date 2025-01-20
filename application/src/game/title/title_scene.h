/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "utils/common.h"
#include "utils/camera.h"
#include "utils/gizmos.h"
#include "game/game_common.h"
#include "game/input.h"
#include "game/title/title_ui.h"

/// @brief Structure représentant la scène du menu principal du jeu.
typedef struct TitleScene
{
    GameConfig *m_gameConfig;
    AssetManager *m_assets;
    Camera *m_camera;
    Input *m_input;
    TitleUI *m_ui;
    Gizmos *m_gizmos;

    int m_state;
    float m_accu;
    float m_fadingTime;
} TitleScene;

/// @brief Crée la scène représentant le menu principal du jeu.
/// @param gameConfig la configuration globale du jeu.
/// @return La scène créée.
TitleScene *TitleScene_create(GameConfig *gameConfig);

/// @brief Détruit la scène représentant le menu principal du jeu.
/// @param self la scène.
void TitleScene_destroy(TitleScene *self);

/// @brief Boucle principale de la scène.
/// @param self la scène.
/// @param drawGizmos booléen indiquant s'il faut dessiner les gizmos.
void TitleScene_mainLoop(TitleScene *self, bool drawGizmos);

/// @brief Met à jour la scène.
/// Cette fonction est appelée à chaque tour de la boucle de rendu.
/// @param self la scène.
void TitleScene_update(TitleScene *self);

/// @brief Active l'animation de fin de scène.
/// La boucle principale s'arrête une fois l'animation terminée.
/// @param self la scène.
void TitleScene_quit(TitleScene *self);

/// @brief Dessine la scène dans le moteur de rendu.
/// @param self la scène.
void TitleScene_render(TitleScene *self);

/// @brief Dessine les gizmos de la scène dans le moteur de rendu.
/// @param self 
void TitleScene_drawGizmos(TitleScene *self);

/// @brief Renvoie la configuration globale du jeu.
/// @param self la scène.
/// @return La configuration globale du jeu.
INLINE GameConfig *TitleScene_getGameConfig(TitleScene *self)
{
    assert(self && "The TitleScene must be created");
    return self->m_gameConfig;
}

/// @brief Renvoie le gestionnaire des assets de la scène.
/// @param self la scène.
/// @return Le gestionnaire des assets de la scène.
INLINE AssetManager *TitleScene_getAssetManager(TitleScene *self)
{
    assert(self && "The TitleScene must be created");
    return self->m_assets;
}

/// @brief Renvoie la caméra de la scène.
/// @param self la scène.
/// @return La caméra de la scène.
INLINE Camera *TitleScene_getCamera(TitleScene *self)
{
    assert(self && "The TitleScene must be created");
    return self->m_camera;
}

/// @brief Renvoie le gestionnaire des entrées utilisateur de la scène.
/// @param self la scène.
/// @return Le gestionnaire des entrées utilisateur de la scène.
INLINE Input *TitleScene_getInput(TitleScene *self)
{
    assert(self && "The TitleScene must be created");
    return self->m_input;
}
