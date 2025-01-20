/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "utils/math.h"
#include "utils/axis.h"
#include "game/game_common.h"

typedef struct PlayerInput
{
    Vec2 axis;

    float triggerL;
    float triggerR;

    bool shootDown;
    bool shootPressed;

    bool validatePressed;
    bool cancelPressed;
    bool pausePressed;

    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;

    AxisData axisLeftData;
} PlayerInput;

void PlayerInput_setTriggerL(PlayerInput *playerInput, Sint16 value);
void PlayerInput_setTriggerR(PlayerInput *playerInput, Sint16 value);

/// @brief Structure représentant le gestionnaire des entrées utilisateur.
typedef struct Input
{
    /// @brief Booléen indiquant si le bouton "quitter" vient d'être pressé.
    bool quitPressed;

    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;

    bool validatePressed;
    bool cancelPressed;
    bool pausePressed;

    PlayerInput players[MAX_PLAYER_COUNT];
} Input;

/// @brief Crée un nouveau gestionnaire des entrées utilisateur.
/// @return Le gestionnaire créé.
Input *Input_create();

/// @brief Détruit le gestionnaire des entrées utilisateur.
/// @param self le gestionnaire.
void Input_destroy(Input *self);

/// @brief Met à jour le gestionnaire des entrées utilisateur.
/// Cette fonction effectue la boucle des événements SDL.
/// @param self le gestionnaire.
void Input_update(Input *self);

void Input_updateControllerButtonDown(Input *self, PlayerInput *playerInput, int button);
void Input_updateControllerButtonUp(Input *self, PlayerInput *playerInput, int button);
void Input_updateControllerAxisMotion(Input *self, PlayerInput *playerInput, int axis, Sint16 value);
void Input_updateKeyDown(Input *self, PlayerInput *playerInput, bool repeat, int scancode);
void Input_updateKeyUp(Input *self, PlayerInput *playerInput, bool repeat, int scancode);
