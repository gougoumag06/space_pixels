﻿/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "utils/sprite_anim.h"

SpriteAnim *SpriteAnim_create(int rectCount, float cycleTime, int cycleCount)
{
    assert(rectCount > 0 && cycleTime > 0.f);

    SpriteAnim *self = (SpriteAnim *)calloc(1, sizeof(SpriteAnim));
    AssertNew(self);

    self->m_frameCount = rectCount;
    self->m_cycleTime = cycleTime;
    self->m_cycleCount = cycleCount;

    return self;
}

void SpriteAnim_destroy(SpriteAnim *self)
{
    if (!self) return;
    free(self);
}

void SpriteAnim_update(SpriteAnim *self, float dt)
{
    assert(self && "The SpriteAnim must be created");
    int rectCount = self->m_frameCount;
    int cycleCount = self->m_cycleCount;
    float cycleTime = self->m_cycleTime;

    self->m_prevFrameIdx = self->m_currFrameIdx;

    if ((cycleCount > 0) && (self->m_cycleIdx >= cycleCount))
    {
        self->m_accu = cycleTime;
        self->m_currFrameIdx = rectCount - 1;
        return;
    }

    self->m_accu += dt;

    while (self->m_accu >= cycleTime)
    {
        self->m_accu -= cycleTime;
        self->m_cycleIdx++;

        if ((cycleCount > 0) && (self->m_cycleIdx >= cycleCount))
        {
            self->m_accu = cycleTime;
            self->m_currFrameIdx = rectCount - 1;
            return;
        }
    }

    float p = self->m_accu / self->m_cycleTime;

    int rectIdx = (int)(p * rectCount);
    if (rectIdx >= rectCount)
        rectIdx = rectCount - 1;

    self->m_currFrameIdx = rectIdx;
}

int SpriteAnim_getFrameIndex(SpriteAnim *self)
{
    assert(self && "The SpriteAnim must be created");
    return self->m_currFrameIdx;
}

bool SpriteAnim_frameChanged(SpriteAnim *self)
{
    assert(self && "The SpriteAnim must be created");
    return (self->m_currFrameIdx != self->m_prevFrameIdx);
}

void SpriteAnim_set(SpriteAnim *self, int rectCount, float cycleTime, int cycleCount)
{
    assert(self && "The SpriteAnim must be created");
    self->m_accu = 0.0f;
    self->m_cycleIdx = 0;
    self->m_currFrameIdx = 0;
    self->m_prevFrameIdx = 0;
    self->m_frameCount = rectCount;
    self->m_cycleTime = cycleTime;
    self->m_cycleCount = cycleCount;
}

void SpriteAnim_restart(SpriteAnim *self)
{
    assert(self && "The SpriteAnim must be created");
    self->m_accu = 0.0f;
    self->m_cycleIdx = 0;
    self->m_currFrameIdx = 0;
    self->m_prevFrameIdx = 0;
}

bool SpriteAnim_isFinished(SpriteAnim *self)
{
    assert(self && "The SpriteAnim must be created");
    return ((self->m_cycleCount > 0) && (self->m_cycleIdx >= self->m_cycleCount));
}
