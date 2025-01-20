/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "utils/common.h"
#include "utils/camera.h"
#include "utils/math.h"

typedef struct Gizmos
{
    Camera *m_camera;
    SDL_Color m_color;
} Gizmos;

Gizmos *Gizmos_create(Camera *camera);
void Gizmos_destroy(Gizmos *self);

void Gizmos_setColor(Gizmos *self, SDL_Color color);
void Gizmos_setColorRGB(Gizmos *self, Uint8 r, Uint8 g, Uint8 b);
void Gizmos_drawCircle(Gizmos *self, Vec2 center, float radius);
