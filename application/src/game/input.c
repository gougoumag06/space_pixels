/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/input.h"
#include "utils/common.h"
#include "utils/math.h"

#define TRIGGER_MAX_VALUE 32767

void PlayerInput_setTriggerL(PlayerInput *playerInput, Sint16 value)
{
    float trigger = Float_clamp((float)value / 32767.f, -1.f, 1.f);
    if (trigger < 0.3f) trigger = 0.f;
    playerInput->triggerL = trigger;
}

void PlayerInput_setTriggerR(PlayerInput *playerInput, Sint16 value)
{
    float trigger = Float_clamp((float)value / 32767.f, -1.f, 1.f);
    if (trigger < 0.3f) trigger = 0.f;
    playerInput->triggerR = trigger;
}

Input *Input_create()
{
    Input *self = (Input *)calloc(1, sizeof(Input));
    AssertNew(self);

    int playerID = 0;
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (SDL_IsGameController(i))
        {
            SDL_GameController *controller = SDL_GameControllerOpen(i);
            SDL_GameControllerSetPlayerIndex(controller, playerID);
            playerID += (playerID < MAX_PLAYER_COUNT - 1) ? 1 : 0;
        }
    }

    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        AxisData_init(&(self->players[i].axisLeftData), 0.3f);
    }

    return self;
}

void Input_destroy(Input *self)
{
    if (!self) return;
    free(self);
}

void Input_update(Input *self)
{
    assert(self);
    PlayerInput *playerInput = &(self->players[0]);
    AxisData *axisLeftData = &(playerInput->axisLeftData);
    SDL_GameController *controller = NULL;
    int playerID = 0;

    self->quitPressed = false;
    self->cancelPressed = false;
    self->validatePressed = false;
    self->pausePressed = false;

    self->upPressed = false;
    self->downPressed = false;
    self->leftPressed = false;
    self->rightPressed = false;

    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        playerInput = &(self->players[i]);
        AxisData_resetPressed(&(playerInput->axisLeftData));

        playerInput->shootPressed = false;

        playerInput->validatePressed = false;
        playerInput->cancelPressed = false;
        playerInput->pausePressed = false;

        playerInput->upPressed = false;
        playerInput->downPressed = false;
        playerInput->leftPressed = false;
        playerInput->rightPressed = false;
    }

    SDL_Event evt = { 0 };
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            self->quitPressed = true;
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            controller = SDL_GameControllerFromInstanceID(evt.cbutton.which);
            playerID = SDL_GameControllerGetPlayerIndex(controller);
            assert(0 <= playerID && playerID < MAX_PLAYER_COUNT);
            playerInput = &(self->players[playerID]);
            Input_updateControllerButtonDown(self, playerInput, evt.cbutton.button);
            break;

        case SDL_CONTROLLERBUTTONUP:
            controller = SDL_GameControllerFromInstanceID(evt.cbutton.which);
            playerID = SDL_GameControllerGetPlayerIndex(controller);
            assert(0 <= playerID && playerID < MAX_PLAYER_COUNT);
            playerInput = &(self->players[playerID]);
            Input_updateControllerButtonUp(self, playerInput, evt.cbutton.button);
            break;

        case SDL_CONTROLLERAXISMOTION:
            controller = SDL_GameControllerFromInstanceID(evt.caxis.which);
            playerID = SDL_GameControllerGetPlayerIndex(controller);
            assert(0 <= playerID && playerID < MAX_PLAYER_COUNT);
            playerInput = &(self->players[playerID]);
            Input_updateControllerAxisMotion(self, playerInput, evt.caxis.axis, evt.caxis.value);
            break;

        case SDL_KEYDOWN:
            playerInput = &(self->players[0]);
            Input_updateKeyDown(self, playerInput, evt.key.repeat, evt.key.keysym.scancode);
            break;

        case SDL_KEYUP:
            playerInput = &(self->players[0]);
            axisLeftData = &(playerInput->axisLeftData);
            Input_updateKeyUp(self, playerInput, evt.key.repeat, evt.key.keysym.scancode);
            break;
        }
    }

    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        playerInput = &(self->players[i]);
        AxisData *axisData = &(playerInput->axisLeftData);

        playerInput->axis = AxisData_getAxis(axisData);
        playerInput->upPressed = AxisData_isUpPressed(axisData);
        playerInput->downPressed = AxisData_isDownPressed(axisData);
        playerInput->rightPressed = AxisData_isRightPressed(axisData);
        playerInput->leftPressed = AxisData_isLeftPressed(axisData);

        if (playerInput->validatePressed) self->validatePressed = true;
        if (playerInput->cancelPressed) self->cancelPressed = true;
        if (playerInput->pausePressed) self->pausePressed = true;
        if (playerInput->upPressed) self->upPressed = true;
        if (playerInput->downPressed) self->downPressed = true;
        if (playerInput->leftPressed) self->leftPressed = true;
        if (playerInput->rightPressed) self->rightPressed = true;
    }
}

void Input_updateControllerButtonDown(Input *self, PlayerInput *playerInput, int button)
{
    AxisData *axisLeftData = &(playerInput->axisLeftData);
    switch (button)
    {
    case SDL_CONTROLLER_BUTTON_A:
        playerInput->shootPressed = true;
        playerInput->shootDown = true;
        playerInput->validatePressed = true;
        break;
    case SDL_CONTROLLER_BUTTON_B:
        playerInput->cancelPressed = true;
        break;
    case SDL_CONTROLLER_BUTTON_START:
        playerInput->pausePressed = true;
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        AxisData_setDirectionUp(axisLeftData, true);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        AxisData_setDirectionDown(axisLeftData, true);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        AxisData_setDirectionLeft(axisLeftData, true);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        AxisData_setDirectionRight(axisLeftData, true);
        break;
    default: break;
    }
}

void Input_updateControllerButtonUp(Input *self, PlayerInput *playerInput, int button)
{
    AxisData *axisLeftData = &(playerInput->axisLeftData);
    switch (button)
    {
    case SDL_CONTROLLER_BUTTON_A:
        playerInput->shootDown = false;
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        AxisData_setDirectionUp(axisLeftData, false);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        AxisData_setDirectionDown(axisLeftData, false);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        AxisData_setDirectionLeft(axisLeftData, false);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        AxisData_setDirectionRight(axisLeftData, false);
        break;
    default: break;
    }
}

void Input_updateControllerAxisMotion(Input *self, PlayerInput *playerInput, int axis, Sint16 value)
{
    AxisData *axisLeftData = &(playerInput->axisLeftData);
    switch (axis)
    {
    case SDL_CONTROLLER_AXIS_LEFTX:
        AxisData_setValueX(axisLeftData, value);
        break;

    case SDL_CONTROLLER_AXIS_LEFTY:
        AxisData_setValueY(axisLeftData, value);
        break;

    case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
        PlayerInput_setTriggerL(playerInput, value);
        break;

    case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
        PlayerInput_setTriggerR(playerInput, value);
        break;

    default: break;
    }
}

void Input_updateKeyDown(Input *self, PlayerInput *playerInput, bool repeat, int scancode)
{
    if (repeat) return;

    AxisData *axisLeftData = &(playerInput->axisLeftData);
    switch (scancode)
    {
    case SDL_SCANCODE_ESCAPE:
        self->quitPressed = true;
        break;
    case SDL_SCANCODE_UP:
        AxisData_setDirectionUp(axisLeftData, true);
        break;
    case SDL_SCANCODE_DOWN:
        AxisData_setDirectionDown(axisLeftData, true);
        break;
    case SDL_SCANCODE_LEFT:
        AxisData_setDirectionLeft(axisLeftData, true);
        break;
    case SDL_SCANCODE_RIGHT:
        AxisData_setDirectionRight(axisLeftData, true);
        break;
    case SDL_SCANCODE_LCTRL:
        PlayerInput_setTriggerL(playerInput, TRIGGER_MAX_VALUE);
        break;
    case SDL_SCANCODE_LALT:
        PlayerInput_setTriggerR(playerInput, TRIGGER_MAX_VALUE);
        break;
    case SDL_SCANCODE_SPACE:
        PlayerInput_setTriggerR(playerInput, TRIGGER_MAX_VALUE);
        playerInput->validatePressed = true;
        playerInput->shootPressed = true;
        break;
    case SDL_SCANCODE_RETURN:
        playerInput->pausePressed = true;
        break;
    case SDL_SCANCODE_BACKSPACE:
        playerInput->cancelPressed = true;
        break;
    default: break;
    }
}

void Input_updateKeyUp(Input *self, PlayerInput *playerInput, bool repeat, int scancode)
{
    if (repeat) return;

    AxisData *axisLeftData = &(playerInput->axisLeftData);
    switch (scancode)
    {
    case SDL_SCANCODE_UP:
        AxisData_setDirectionUp(axisLeftData, false);
        break;
    case SDL_SCANCODE_DOWN:
        AxisData_setDirectionDown(axisLeftData, false);
        break;
    case SDL_SCANCODE_LEFT:
        AxisData_setDirectionLeft(axisLeftData, false);
        break;
    case SDL_SCANCODE_RIGHT:
        AxisData_setDirectionRight(axisLeftData, false);
        break;
    case SDL_SCANCODE_LCTRL:
        PlayerInput_setTriggerL(playerInput, 0);
        break;
    case SDL_SCANCODE_LALT:
        PlayerInput_setTriggerR(playerInput, 0);
        break;
    case SDL_SCANCODE_SPACE:
        PlayerInput_setTriggerR(playerInput, 0);
        break;
    default: break;
    }
}
