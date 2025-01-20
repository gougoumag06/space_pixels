/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "utils/camera.h"

Camera *Camera_create(int width, int height)
{
    Camera *self = (Camera *)calloc(1, sizeof(Camera));
    AssertNew(self);

    float worldW = 16.0f;
    float worldH = 9.0f;

    self->m_rasterWidth = width;
    self->m_rasterHeight = height;
    self->m_worldView = AABB_set(0.0f, 0.0f, worldW, worldH);

    return self;
}

void Camera_destroy(Camera *self)
{
    if (!self) return;
    free(self);
}

void Camera_worldToView(Camera *self, Vec2 position, float *x, float *y)
{
    assert(self && "The Camera must be created");
    assert(x && y);
    float w = self->m_worldView.upper.x - self->m_worldView.lower.x;
    float h = self->m_worldView.upper.y - self->m_worldView.lower.y;
    float xScale = (float)self->m_rasterWidth  / w;
    float yScale = (float)self->m_rasterHeight / h;
    *x = (position.x - self->m_worldView.lower.x) * xScale;
    *y = (position.y - self->m_worldView.lower.y) * yScale;
    *y = (float)self->m_rasterHeight - *y;
}

void Camera_viewToWorld(Camera *self, float x, float y, Vec2 *position)
{
    assert(self && "The Camera must be created");
    y = self->m_rasterHeight - y;
    float ratioX = (float)x / (float)self->m_rasterWidth;
    float ratioY = (float)y / (float)self->m_rasterHeight;
    float w = self->m_worldView.upper.x - self->m_worldView.lower.x;
    float h = self->m_worldView.upper.y - self->m_worldView.lower.y;
    position->x = self->m_worldView.lower.x + ratioX * w;
    position->y = self->m_worldView.lower.y + ratioY * h;
}

void Camera_translateWorldView(Camera *self, Vec2 displacement)
{
    assert(self && "The Camera must be created");
    AABB_translate(&self->m_worldView, displacement);
}

int Camera_getWidth(Camera *self)
{
    assert(self && "The Camera must be created");
    return self->m_rasterWidth;
}

int Camera_getHeight(Camera *self)
{
    assert(self && "The Camera must be created");
    return self->m_rasterHeight;
}

float Camera_getWorldToViewScale(Camera *self)
{
    assert(self && "The Camera must be created");
    float w = self->m_worldView.upper.x - self->m_worldView.lower.x;
    return (float)self->m_rasterWidth / w;
}