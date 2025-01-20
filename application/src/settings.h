/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#  include "SDL.h"
#  include "SDL_image.h"
#  include "SDL_ttf.h"
#  include "SDL_mixer.h"
#else
#  include <SDL2/SDL.h>
#  include <SDL2/SDL_image.h>
#  include <SDL2/SDL_ttf.h>
#  include <SDL2/SDL_mixer.h>
#endif

#ifdef _MSC_VER
#  define INLINE inline
#else
#  define INLINE static inline
#endif

#define AssertNew(ptr) { if (ptr == NULL) { assert(false); abort(); } }

#define HD_WIDTH 1280
#define HD_HEIGHT 720
#define FHD_WIDTH 1920
#define FHD_HEIGHT 1080
