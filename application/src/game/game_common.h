/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "utils/asset_manager.h"

#define MAX_PLAYER_COUNT 2
#define PIX_TO_WORLD (1.0f / 48.0f)

#define DAMAGE_SMALL 5
#define DAMAGE_MED 10
#define DAMAGE_BIG 20

#ifdef _WIN32
#define ASSETS_PATH "../../assets/"
#else
#define ASSETS_PATH "../assets/"
#endif

typedef enum LevelID
{
    LEVEL_1,
    LEVEL_2,
    //
    LEVEL_COUNT,
} LevelID;

typedef enum GameScene
{
    GAME_SCENE_TITLE,
    GAME_SCENE_LEVEL,
    GAME_SCENE_QUIT
} GameScene;

typedef struct GameConfig
{
    int playerCount;
    int nextScene;
    int levelID;
} GameConfig;

typedef enum SceneState
{
    SCENE_STATE_RUNNING,
    SCENE_STATE_FADING_IN,
    SCENE_STATE_FADING_OUT,
    SCENE_STATE_FINISHED
} SceneState;

typedef struct GameColors
{
    SDL_Color black;
    SDL_Color white;
    SDL_Color red;
    SDL_Color blue;
    SDL_Color green;
    SDL_Color yellow;
    SDL_Color magenta;
    SDL_Color cyan;
    //
    SDL_Color gold;
    SDL_Color whiteSemi;
} GameColors;

extern GameColors g_colors;

typedef enum SpriteID
{
    SPRITE_PLAYER,
    SPRITE_PLAYER_ENGINE,
    SPRITE_PLAYER_POWERING,
    //
    SPRITE_BULLET_PLAYER_DEFAULT,
    SPRITE_BULLET_FIGHTER,
    //
    SPRITE_FIGHTER_FIRING,
    SPRITE_FIGHTER_DYING,
    //
    SPRITE_BACKGROUND_BLUE_NEBULA,
    SPRITE_BACKGROUND_PURPLE_NEBULA,
    //
    SPRITE_COUNT,
} SpriteID;

typedef enum FontID
{
    FONT_NORMAL,
    FONT_BIG,
    FONT_MAIN_TITLE,
    //
    FONT_COUNT,
} FontID;

typedef enum SoundID
{
    SOUND_PLAYER_FIRE,
    //
    SOUND_ENEMY_DIYNG,
    SOUND_ENEMY_FIRE,
    //
    SOUND_UI_SELECT,
    SOUND_UI_VALIDATE,
    SOUND_UI_CANCEL,
    SOUND_UI_PAUSE,
    //
    SOUND_COUNT,
} SoundID;

typedef enum MusicID
{
    MUSIC_LUMINARES,
    MUSIC_POWER,
    //
    MUSIC_COUNT,
} MusicID;

void Game_addAssets(AssetManager *assets);
