/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "utils/gizmos.h"

Gizmos *Gizmos_create(Camera *camera)
{
    assert(camera);

    Gizmos *self = (Gizmos *)calloc(1, sizeof(Gizmos));
    AssertNew(self);

    self->m_camera = camera;
    self->m_color.r = (Uint8)255;
    self->m_color.g = (Uint8)0;
    self->m_color.b = (Uint8)0;
    self->m_color.a = (Uint8)255;

    return self;
}

void Gizmos_destroy(Gizmos *self)
{
    if (!self) return;
    free(self);
}

void Gizmos_setColorRGB(Gizmos *self, Uint8 r, Uint8 g, Uint8 b)
{
    assert(self && "The Gizmos must be created");
    self->m_color.r = r;
    self->m_color.g = g;
    self->m_color.b = b;
}

void Gizmos_setColor(Gizmos *self, SDL_Color color)
{
    assert(self && "The Gizmos must be created");
    self->m_color = color;
}

#define CIRCLE_SEG_COUNT 16
void Gizmos_drawCircle(Gizmos *self, Vec2 center, float radius)
{
    assert(self && "The Gizmos must be created");
    float inc = (float)(2.f * M_PI / CIRCLE_SEG_COUNT);
    Vec2 vertices[CIRCLE_SEG_COUNT + 1] = { 0 };

    float theta = 0.f;
    for (int i = 0; i < CIRCLE_SEG_COUNT; i++, theta += inc)
    {
        vertices[i] = Vec2_add(
            center,
            Vec2_scale(Vec2_set(cosf(theta), sinf(theta)), radius)
        );
    }
    vertices[CIRCLE_SEG_COUNT] = vertices[0];

    SDL_SetRenderDrawColor(
        g_renderer,
        self->m_color.r,
        self->m_color.g,
        self->m_color.b,
        self->m_color.a);

    float x, y;
    SDL_Point points[2] = { 0 };

    Camera_worldToView(self->m_camera, vertices[0], &x, &y);
    points[0].x = (int)x;
    points[0].y = (int)y;

    for (int i = 1; i < CIRCLE_SEG_COUNT + 1; ++i)
    {
        Camera_worldToView(self->m_camera, vertices[i], &x, &y);
        points[1].x = (int)x;
        points[1].y = (int)y;

        SDL_RenderDrawLines(g_renderer, points, 2);
        points[0] = points[1];
    }
}
