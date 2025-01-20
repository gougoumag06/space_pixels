/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "utils/text.h"

/// @brief Structure représentant un atlas de textures.
typedef struct SpriteSheet
{
    SDL_Texture *texture;
    SDL_Rect *rects;
    int rectCount;
} SpriteSheet;

/// @brief Copie un sprite d'une sprite sheet vers la cible du moteur de rendu.
/// @param self la sprite sheet.
/// @param index indice du sprite à copier.
/// @param renderer le moteur de rendu.
/// @param dstRect le rectangle de destination sur le rendu.
/// @param angle l'angle de rotation, en degrés.
/// @param center le centre de rotation, ou NULL pour le centre de l'image.
/// @param flip flag indiquant quels retrounements (horizontal, vertical) sont appliqués à la copie.
void SpriteSheet_renderCopyF(
    SpriteSheet *self, int index,
    SDL_Renderer *renderer, const SDL_FRect *dstRect,
    const double angle, const SDL_FPoint *center, const SDL_RendererFlip flip);

/// @brief Modifie l'opacité d'une sprite sheet.
/// Cette fonction doit être appliquée avant une opération de copie sur le rendu
/// pour être prise en compte.
/// @param self la sprite sheet.
/// @param alpha l'opacité (0 pour transparant, 255 pour opaque).
void SpriteSheet_setOpacity(SpriteSheet *self, Uint8 alpha);

typedef struct SpriteSheetData SpriteSheetData;
typedef struct FontData FontData;
typedef struct SoundData SoundData;
typedef struct MusicData MusicData;

/// @brief Structure contenant l'ensemble des assets du jeu.
/// Cela correspond aux ressources utilisées (textures, fontes, musiques, son...)
typedef struct AssetManager
{
    int m_spriteCapacity;
    SpriteSheetData *m_spriteData;

    int m_fontCapacity;
    FontData *m_fontData;

    int m_musicCapacity;
    MusicData *m_musicData;

    int m_soundCapacity;
    SoundData *m_soundData;
} AssetManager;

/// @brief Crée le gestionnaire des assets du jeu.
/// @param spriteCapacity le nombre maximum de sprite sheets pouvant être ajoutées.
/// @param fontCapacity le nombre maximum de polices pouvant être ajoutées.
/// @param soundCapacity le nombre maximum de sons pouvant être ajoutés.
/// @param musicCapacity le nombre maximum de musiques pouvant être ajoutées.
/// @return Le gestionnaire des assets du jeu.
AssetManager *AssetManager_create(
    int spriteCapacity, int fontCapacity,
    int soundCapacity, int musicCapacity);

/// @brief Détruit le gestionnaire des assets du jeu.
/// @param self les assets.
void AssetManager_destroy(AssetManager *self);

/// @brief Ajoute une sprite sheet au gestionnaire des assets.
/// @param self le gestionnaire d'assets.
/// @param sheetID l'identifiant de la sprite sheet.
/// @param assetsPath le chemin vers le dossier principal contenant les assets.
/// @param fileName le nom du fichier.
/// @param rectCount le nombre de sprites de la sprite sheet.
/// @param rectWidth la largeur d'un sprite.
/// @param rectHeight la hauteur d'un sprite.
void AssetManager_addSpriteSheet(
    AssetManager *self, int sheetID,
    const char *assetsPath, const char *fileName,
    int rectCount, int rectWidth, int rectHeight);

/// @brief Ajoute une police au gestionnaire des assets.
/// @param self le gestionnaire d'assets.
/// @param fontID l'identifiant de la police.
/// @param assetsPath le chemin vers le dossier principal contenant les assets.
/// @param fileName le nom du fichier.
/// @param size la taille de la police.
void AssetManager_addFont(
    AssetManager *self, int fontID,
    const char *assetsPath, const char *fileName, int size);

/// @brief Ajoute un son au gestionnaire des assets.
/// @param self le gestionnaire d'assets.
/// @param soundID l'identifiant du son.
/// @param assetsPath le chemin vers le dossier principal contenant les assets.
/// @param fileName le nom du fichier.
void AssetManager_addSound(
    AssetManager *self, int soundID,
    const char *assetsPath, const char *fileName);

/// @brief Ajoute une musique au gestionnaire des assets.
/// @param self le gestionnaire d'assets.
/// @param musicID l'identifiant de la musique.
/// @param assetsPath le chemin vers le dossier principal contenant les assets.
/// @param fileName le nom du fichier.
void AssetManager_addMusic(
    AssetManager *self, int musicID,
    const char *assetsPath, const char *fileName);

/// @brief Renvoie une sprite sheet répertoriée dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param sheetID l'identifiant de la sprite sheet.
/// @return La sprite sheet associée à l'identifiant sheetID;
SpriteSheet *AssetManager_getSpriteSheet(AssetManager *self, int sheetID);

/// @brief Renvoie une police répertoriée dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param fontID l'identifiant de la police.
/// @return Le police associée à l'identifiant fontID.
TTF_Font *AssetManager_getFont(AssetManager *self, int fontID);

/// @brief Renvoie un son répertorié dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param soundID l'identifiant du son.
/// @return Le son associé à l'identifiant soundID.
Mix_Chunk *AssetManager_getSound(AssetManager *self, int soundID);

/// @brief Renvoie une musique répertoriée dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param musicID l'identifiant de la musique.
/// @return La musique associée à l'identifiant musicID.
Mix_Music *AssetManager_getMusic(AssetManager *self, int musicID);

/// @brief Charge une sprite sheet répertoriée dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param sheetID l'identifiant de la sprite sheet.
void AssetManager_loadSpriteSheet(AssetManager *self, int sheetID);

/// @brief Charge une police répertoriée dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param fontID l'identifiant de la police.
void AssetManager_loadFont(AssetManager *self, int fontID);

/// @brief Charge un son répertorié dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param soundID l'identifiant du son.
void AssetManager_loadSound(AssetManager *self, int soundID);

/// @brief Charge une musique répertoriée dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param musicID l'identifiant de la musique.
void AssetManager_loadMusic(AssetManager *self, int musicID);

struct SpriteSheetData
{
    SpriteSheet *m_spriteSheet;
    char *m_fileName;
    int m_rectCount;
    int m_rectWidth;
    int m_rectHeight;
};

struct FontData
{
    TTF_Font *m_font;
    char *m_fileName;
    void *m_buffer;
    SDL_RWops *m_rwops;
    int m_size;
};

struct MusicData
{
    Mix_Music *m_music;
    char *m_fileName;
    void *m_buffer;
    SDL_RWops *m_rwops;
};

struct SoundData
{
    Mix_Chunk *m_chunk;
    char *m_fileName;
};
