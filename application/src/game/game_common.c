/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/game_common.h"
#include "utils/common.h"

GameColors g_colors = {
    .black = { 0, 0, 0, 255 },
    .white = { 255, 255, 255, 255 },
    .red = { 255, 0, 0, 255 },
    .green = { 0, 255, 0, 255 },
    .blue = { 0, 0, 255, 255 },
    .yellow = { 255, 255, 0, 255 },
    .cyan = { 0, 255, 255, 255 },
    .magenta = { 255, 0, 255, 255 },
    .gold = { 225, 185, 143, 255 },
    .whiteSemi = { 255, 255, 255, 127 },
};

void Game_addAssets(AssetManager *assets)
{
    // -------------------------------------------------------------------------
    // Sprite sheets

    // Player

    /* TODO : Affichage du joueur
    AssetManager_addSpriteSheet(
        assets, SPRITE_PLAYER,
        ASSETS_PATH, "player/player.png", 1, 48, 48
    );
    AssetManager_addSpriteSheet(
        assets, SPRITE_PLAYER_ENGINE,
        ASSETS_PATH, "player/engine.png", 1, 48, 48
    );
    AssetManager_addSpriteSheet(
        assets, SPRITE_PLAYER_POWERING,
        ASSETS_PATH, "player/engine_powering.png", 4, 48, 48
    );
    //*/

    // Bullet
    /* TODO : Tir du joueur
    AssetManager_addSpriteSheet(
        assets, SPRITE_BULLET_PLAYER_DEFAULT,
        ASSETS_PATH, "player/bullet_default.png", 4, 8, 16
    );
    //*/
    /* TODO : Tir d'un ennemi
    AssetManager_addSpriteSheet(
        assets, SPRITE_BULLET_FIGHTER,
        ASSETS_PATH, "enemy/fighter_bullet.png", 4, 4, 16
    );
    //*/

    // Fighter
    /* TODO : Affichage d'un ennemi
    AssetManager_addSpriteSheet(
        assets, SPRITE_FIGHTER_FIRING,
        ASSETS_PATH, "enemy/fighter_firing.png", 6, 64, 64
    );
    AssetManager_addSpriteSheet(
        assets, SPRITE_FIGHTER_DYING,
        ASSETS_PATH, "enemy/fighter_dying.png", 9, 64, 64
    );
    //*/

    // Background
    AssetManager_addSpriteSheet(
        assets, SPRITE_BACKGROUND_BLUE_NEBULA,
        ASSETS_PATH, "background/blue_nebula.png", 1, 1024, 1024
    );
    AssetManager_addSpriteSheet(
        assets, SPRITE_BACKGROUND_PURPLE_NEBULA,
        ASSETS_PATH, "background/purple_nebula.png", 1, 1024, 1024
    );

    // -------------------------------------------------------------------------
    // Sounds

    // Player
    AssetManager_addSound(
        assets, SOUND_PLAYER_FIRE,
        ASSETS_PATH, "audio/sound_fx/LASRGun_Classic Blaster A Fire_03.wav"
    );

    // Enemies
    AssetManager_addSound(
        assets, SOUND_ENEMY_DIYNG,
        ASSETS_PATH, "audio/sound_fx/EXPLDsgn_Explosion Impact_14.wav"
    );
    AssetManager_addSound(
        assets, SOUND_ENEMY_FIRE,
        ASSETS_PATH, "audio/sound_fx/LASRGun_Classic Blaster A Fire_03.wav"
    );

    // UI
    AssetManager_addSound(
        assets, SOUND_UI_SELECT,
        ASSETS_PATH, "audio/sound_fx/FUI Button Beep Clean.wav"
    );
    AssetManager_addSound(
        assets, SOUND_UI_VALIDATE,
        ASSETS_PATH, "audio/sound_fx/FUI Holographic Button Press-1.wav"
    );
    AssetManager_addSound(
        assets, SOUND_UI_CANCEL,
        ASSETS_PATH, "audio/sound_fx/FUI Holographic Interaction Radiate.wav"
    );
    AssetManager_addSound(
        assets, SOUND_UI_PAUSE,
        ASSETS_PATH, "audio/sound_fx/FUI Ping Triplet Echo.wav"
    );

    // Ajustement du volume
    Game_setSoundVolume(assets, SOUND_PLAYER_FIRE, 0.5f);
    Game_setSoundVolume(assets, SOUND_ENEMY_FIRE, 0.3f);

    // -------------------------------------------------------------------------
    // Musics

    AssetManager_addMusic(
        assets, MUSIC_LUMINARES,
        ASSETS_PATH, "audio/musics/heatley_bros_luminare.mp3"
    );
    AssetManager_addMusic(
        assets, MUSIC_POWER,
        ASSETS_PATH, "audio/musics/heatley_bros_8_bit_power.mp3"
    );

    // -------------------------------------------------------------------------
    // Fonts

    AssetManager_addFont(
        assets, FONT_NORMAL, ASSETS_PATH, "font/m5x7.ttf", 48
    );
    AssetManager_addFont(
        assets, FONT_BIG, ASSETS_PATH, "font/futile_pro.ttf", 48
    );
    AssetManager_addFont(
        assets, FONT_MAIN_TITLE, ASSETS_PATH, "font/futile_pro.ttf", 96
    );
}
