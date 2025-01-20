/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "utils/math.h"

const Vec2 Vec2_up    = {  0.0f,  1.0f };
const Vec2 Vec2_down  = {  0.0f, -1.0f };
const Vec2 Vec2_left  = { -1.0f,  0.0f };
const Vec2 Vec2_right = {  1.0f,  0.0f };
const Vec2 Vec2_zero  = {  0.0f,  0.0f };
const Vec2 Vec2_one   = {  1.0f,  1.0f };

//==============================================================================
// TODO
// Codez les fonctions suivantes

Vec2 Vec2_set(float x, float y)
{
    return Vec2_zero;
}

Vec2 Vec2_add(Vec2 v1, Vec2 v2)
{
    return Vec2_zero;
}

Vec2 Vec2_sub(Vec2 v1, Vec2 v2)
{
    return Vec2_zero;
}

Vec2 Vec2_scale(Vec2 v, float s)
{
    return Vec2_zero;
}

float Vec2_lengthSquared(Vec2 v)
{
    return 0.f;
}

float Vec2_length(Vec2 v)
{
    return 0.f;
}

Vec2 Vec2_normalize(Vec2 v)
{
    return Vec2_up;
}

float Vec2_distanceSquared(Vec2 v1, Vec2 v2)
{
    return 0.f;
}

float Vec2_distance(Vec2 v1, Vec2 v2)
{
    return 0.f;
}

Vec2 Vec2_perp(Vec2 v)
{
    return Vec2_zero;
}

float Vec2_det(Vec2 v1, Vec2 v2)
{
    return 0.f;
}

float Vec2_dot(Vec2 v1, Vec2 v2)
{
    return 0.f;
}

// END TODO
//==============================================================================

INLINE float Vec2_signedAngleAtan2(Vec2 from, Vec2 to)
{
    // Le calcule de l'angle est plus précis avec atan2 plutôt qu'avec acos
    // Il n'y a pas non plus besoin de normaliser les vecteurs
    float s = Vec2_det(from, to); // |from|.|to|.sin(angle)
    float c = Vec2_dot(from, to); // |from|.|to|.cos(angle)
    return atan2f(s, c);
}

float Vec2_signedAngleDeg(Vec2 from, Vec2 to)
{
    return Vec2_signedAngleAtan2(from, to) * RAD_TO_DEG;
}

float Vec2_signedAngleRad(Vec2 from, Vec2 to)
{
    return Vec2_signedAngleAtan2(from, to);
}

Vec2 Vec2_smoothDamp(
    Vec2 current, Vec2 target, Vec2 *currentVelocity,
    float smoothTime, float maxSpeed, float deltaTime)
{
    Vec2 res = { 0 };

    res.x = Float_smoothDamp(
        current.x, target.x, &(currentVelocity->x),
        smoothTime, maxSpeed, deltaTime);

    res.y = Float_smoothDamp(
        current.y, target.y, &(currentVelocity->y),
        smoothTime, maxSpeed, deltaTime);

    return res;
}

float Float_smoothDamp(
    float current, float target, float *currentVelocity,
    float smoothTime, float maxSpeed, float deltaTime)
{
    // Implémentation de Unity
    // Voir Game Programming Gems 4 chapitre 1.10 page 99
    smoothTime = fmaxf(0.0001f, smoothTime);
    float omega = 2.0f / smoothTime;
    float x = omega * deltaTime;
    float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
    float change = current - target;
    float targetCopy = target;
    float maxChange = maxSpeed * smoothTime;
    change = Float_clamp(change, -maxChange, maxChange);
    target = current - change;
    float temp = (*currentVelocity + omega * change) * deltaTime;
    *currentVelocity = (*currentVelocity - omega * temp) * exp;
    float res = target + (change + temp) * exp;
    if (targetCopy - current > 0.0f == res > targetCopy)
    {
        res = targetCopy;
        *currentVelocity = (res - targetCopy) / deltaTime;
    }
    return res;
}