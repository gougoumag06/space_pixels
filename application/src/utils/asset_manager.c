/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "utils/asset_manager.h"
#include "utils/common.h"

static void AssetManager_createRWops(const char *fileName, SDL_RWops **rwops, void **buffer);
static void AssetManager_destroyRWops(SDL_RWops *rwops, void *buffer);

static void SpriteSheetData_load(SpriteSheetData *self);
static void SpriteSheetData_clear(SpriteSheetData *self);

static void FontData_load(FontData *self);
static void FontData_clear(FontData *self);

static void SoundData_load(SoundData *self);
static void SoundData_clear(SoundData *self);

static void MusicData_load(MusicData *self);
static void MusicData_clear(MusicData *self);

static char *AssetManager_makePath(const char *assetsPath, const char *fileName);
SDL_Texture *AssetManager_loadTexture(SDL_Renderer *renderer, const char *fileName);

void SpriteSheet_renderCopyF(
    SpriteSheet *self, int index,
    SDL_Renderer *renderer, const SDL_FRect *dstRect,
    const double angle, const SDL_FPoint *center, const SDL_RendererFlip flip)
{
        assert(self && renderer);
        assert(index >= 0);
        index = index % self->rectCount;
        SDL_RenderCopyExF(renderer, self->texture, self->rects + index, dstRect, angle, center, flip);
}

void SpriteSheet_setOpacity(SpriteSheet *self, Uint8 alpha)
{
    int exitStatus;
    exitStatus = SDL_SetTextureBlendMode(self->texture, SDL_BLENDMODE_BLEND);
    assert(exitStatus == 0);
    exitStatus = SDL_SetTextureAlphaMod(self->texture, alpha);
    assert(exitStatus == 0);
}

AssetManager *AssetManager_create(
    int spriteCapacity, int fontCapacity,
    int soundCapacity, int musicCapacity)
{
    AssetManager *self = (AssetManager *)calloc(1, sizeof(AssetManager));
    AssertNew(self);

    self->m_fontCapacity = fontCapacity;
    self->m_spriteCapacity = spriteCapacity;
    self->m_soundCapacity = soundCapacity;
    self->m_musicCapacity = musicCapacity;

    self->m_spriteData = (SpriteSheetData *)calloc(spriteCapacity, sizeof(SpriteSheetData));
    AssertNew(self->m_spriteData);

    self->m_fontData = (FontData *)calloc(fontCapacity, sizeof(FontData));
    AssertNew(self->m_fontData);

    self->m_soundData = (SoundData *)calloc(soundCapacity, sizeof(SoundData));
    AssertNew(self->m_soundData);

    self->m_musicData = (MusicData *)calloc(musicCapacity, sizeof(MusicData));
    AssertNew(self->m_musicData);

    return self;
}

void AssetManager_destroy(AssetManager *self)
{
    if (!self) return;

    if (self->m_spriteData)
    {
        // Libère les spriteSheet
        for (int i = 0; i < self->m_spriteCapacity; i++)
        {
            SpriteSheetData_clear(self->m_spriteData + i);
        }
        free(self->m_spriteData);
    }

    if (self->m_musicData)
    {
        // Libère les musiques
        for (int i = 0; i < self->m_musicCapacity; i++)
        {
            MusicData_clear(self->m_musicData + i);
        }
        free(self->m_musicData);
    }

    if (self->m_soundData)
    {
        // Libère les sons
        for (int i = 0; i < self->m_soundCapacity; i++)
        {
            SoundData_clear(self->m_soundData + i);
        }
        free(self->m_soundData);
    }

    if (self->m_fontData)
    {
        // Libère les fonts
        for (int i = 0; i < self->m_fontCapacity; i++)
        {
            FontData_clear(self->m_fontData + i);
        }
        free(self->m_fontData);
    }

    free(self);
}

static char *AssetManager_makePath(const char *assetsPath, const char *fileName)
{
    assert(assetsPath && "The assets path is not valid");
    assert(fileName && "The file name is not valid");

    size_t assetPathLen = strlen(assetsPath);
    size_t fileNameLen = strlen(fileName);
    size_t pathSize = assetPathLen + fileNameLen + 1;
    char *fullPath = (char *)calloc(pathSize + 1, sizeof(char));
    AssertNew(fullPath);
    Memcpy(fullPath, pathSize, assetsPath, assetPathLen);
    Memcpy(fullPath + assetPathLen, fileNameLen, fileName, fileNameLen);
    return fullPath;
}

void AssetManager_addSpriteSheet(
    AssetManager *self, int sheetID,
    const char *assetsPath, const char *fileName,
    int rectCount, int rectWidth, int rectHeight)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= sheetID && sheetID < self->m_spriteCapacity && "The sheetID is not valid");

    SpriteSheetData *spriteData = &(self->m_spriteData[sheetID]);
    if (spriteData->m_fileName)
    {
        printf("ERROR - Add sprite sheet %s%s\n", assetsPath, fileName);
        printf("      - The sprite sheet with ID %d already exists\n", sheetID);
        assert(false && "The sheetID is already used");
        return;
    }
    spriteData->m_fileName = AssetManager_makePath(assetsPath, fileName);
    spriteData->m_rectCount = rectCount;
    spriteData->m_rectWidth = rectWidth;
    spriteData->m_rectHeight = rectHeight;
}

void AssetManager_addFont(
    AssetManager *self, int fontID,
    const char *assetsPath, const char *fileName, int size)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= fontID && fontID < self->m_fontCapacity && "The fontID is not valid");

    FontData *fontData = &(self->m_fontData[fontID]);
    if (fontData->m_fileName)
    {
        printf("ERROR - Add font %s%s\n", assetsPath, fileName);
        printf("      - The font with ID %d already exists\n", fontID);
        assert(false && "The fontID is already used");
        return;
    }
    fontData->m_fileName = AssetManager_makePath(assetsPath, fileName);
    fontData->m_size = size;
}

void AssetManager_addSound(
    AssetManager *self, int soundID,
    const char *assetsPath, const char *fileName)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= soundID && soundID < self->m_soundCapacity && "The soundID is not valid");

    SoundData *soundData = &(self->m_soundData[soundID]);
    if (soundData->m_fileName)
    {
        printf("ERROR - Add sound %s%s\n", assetsPath, fileName);
        printf("      - The sound with ID %d already exists\n", soundID);
        assert(false && "The soundID is already used");
        return;
    }
    soundData->m_fileName = AssetManager_makePath(assetsPath, fileName);
}

void AssetManager_addMusic(
    AssetManager *self, int musicID,
    const char *assetsPath, const char *fileName)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= musicID && musicID < self->m_musicCapacity && "The musicID is not valid");

    MusicData *musidData = &(self->m_musicData[musicID]);
    if (musidData->m_fileName)
    {
        printf("ERROR - Add music %s%s\n", assetsPath, fileName);
        printf("      - The music with ID %d already exists\n", musicID);
        assert(false && "The musicID is already used");
        return;
    }
    musidData->m_fileName = AssetManager_makePath(assetsPath, fileName);
}

SpriteSheet *AssetManager_getSpriteSheet(AssetManager *self, int sheetID)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= sheetID && sheetID < self->m_spriteCapacity && "The sheetID is not valid");

    SpriteSheetData *spriteData = &(self->m_spriteData[sheetID]);
    if (spriteData->m_spriteSheet) return spriteData->m_spriteSheet;

    if (spriteData->m_fileName == NULL)
    {
        printf("ERROR - No sprite sheet with ID %d\n", sheetID);
        assert(false && "No sprite sheet with this sheetID");
        return NULL;
    }

    SpriteSheetData_load(spriteData);
    return spriteData->m_spriteSheet;
}

TTF_Font *AssetManager_getFont(AssetManager *self, int fontID)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= fontID && fontID < self->m_fontCapacity && "The fontID is not valid");

    FontData *fontData = &(self->m_fontData[fontID]);
    if (fontData->m_font) return fontData->m_font;

    if (fontData->m_fileName == NULL)
    {
        printf("ERROR - No font with ID %d\n", fontID);
        assert(false && "No font with this fontID");
        return NULL;
    }

    FontData_load(fontData);
    return fontData->m_font;
}

Mix_Chunk *AssetManager_getSound(AssetManager *self, int soundID)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= soundID && soundID < self->m_soundCapacity && "The soundID is not valid");

    SoundData *soundData = &(self->m_soundData[soundID]);
    if (soundData->m_chunk) return soundData->m_chunk;

    if (soundData->m_fileName == NULL)
    {
        printf("ERROR - No sound with ID %d\n", soundID);
        assert(false && "No sound with this soundID");
        return NULL;
    }

    SoundData_load(soundData);
    return soundData->m_chunk;
}

Mix_Music *AssetManager_getMusic(AssetManager *self, int musicID)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= musicID && musicID < self->m_musicCapacity && "The musicID is not valid");

    MusicData *musicData = &(self->m_musicData[musicID]);
    if (musicData->m_music) return musicData->m_music;

    if (musicData->m_fileName == NULL)
    {
        printf("ERROR - No music with ID %d\n", musicID);
        assert(false && "No music with this musicID");
        return NULL;
    }

    MusicData_load(musicData);
    return musicData->m_music;
}

void AssetManager_loadSpriteSheet(AssetManager *self, int sheetID)
{
    SpriteSheet *spriteSheet = AssetManager_getSpriteSheet(self, sheetID);
    assert(spriteSheet);
}

void AssetManager_loadFont(AssetManager *self, int fontID)
{
    TTF_Font *font = AssetManager_getFont(self, fontID);
    assert(font);
}

void AssetManager_loadSound(AssetManager *self, int soundID)
{
    Mix_Chunk *chunk = AssetManager_getSound(self, soundID);
    assert(chunk);
}

void AssetManager_loadMusic(AssetManager *self, int musicID)
{
    Mix_Music *music = AssetManager_getMusic(self, musicID);
    assert(music);
}

static void SpriteSheetData_load(SpriteSheetData *self)
{
    SpriteSheet *spriteSheet = (SpriteSheet *)calloc(1, sizeof(SpriteSheet));
    AssertNew(spriteSheet);

    self->m_spriteSheet = spriteSheet;

    void *buffer = NULL;
    SDL_RWops *rwops = NULL;
    AssetManager_createRWops(self->m_fileName, &rwops, &buffer);

    spriteSheet->texture = IMG_LoadTexture_RW(g_renderer, rwops, 0);
    if (spriteSheet->texture == NULL)
    {
        printf("ERROR - Loading m_spriteSheet %s\n", self->m_fileName);
        printf("      - %s\n", IMG_GetError());
        assert(false);
        abort();
    }

    AssetManager_destroyRWops(rwops, buffer);
    rwops = NULL; buffer = NULL;

    int rectCount = self->m_rectCount;
    spriteSheet->rectCount = rectCount;
    spriteSheet->rects = (SDL_Rect *)calloc(rectCount, sizeof(SDL_Rect));
    AssertNew(spriteSheet->rects);

    int w, h;
    SDL_QueryTexture(spriteSheet->texture, NULL, NULL, &w, &h);

    int x = 0, y = 0;
    for (int i = 0; i < rectCount; i++)
    {
        spriteSheet->rects[i].w = self->m_rectWidth;
        spriteSheet->rects[i].h = self->m_rectHeight;
        spriteSheet->rects[i].x = x;
        spriteSheet->rects[i].y = y;

        x += self->m_rectWidth;
        if (x > w - self->m_rectWidth)
        {
            x = 0;
            y += self->m_rectHeight;
        }
    }
}

static void SpriteSheetData_clear(SpriteSheetData *self)
{
    if (self->m_spriteSheet)
    {
        free(self->m_spriteSheet->rects);
        if (self->m_spriteSheet->texture)
            SDL_DestroyTexture(self->m_spriteSheet->texture);
    }
    free(self->m_fileName);
    memset(self, 0, sizeof(SpriteSheetData));
}

static void SoundData_load(SoundData *self)
{
    void *buffer = NULL;
    SDL_RWops *rwops = NULL;
    AssetManager_createRWops(self->m_fileName, &rwops, &buffer);

    self->m_chunk = Mix_LoadWAV_RW(rwops, 0);
    if (self->m_chunk == NULL)
    {
        printf("ERROR - Loading audio %s\n", self->m_fileName);
        printf("      - %s\n", Mix_GetError());
        assert(false);
        abort();
    }

    AssetManager_destroyRWops(rwops, buffer);
    rwops = NULL; buffer = NULL;
}

static void SoundData_clear(SoundData *self)
{
    if (self->m_chunk)
    {
        Mix_FreeChunk(self->m_chunk);
    }
    free(self->m_fileName);
    memset(self, 0, sizeof(SoundData));
}

static void FontData_load(FontData *self)
{
    AssetManager_createRWops(self->m_fileName, &self->m_rwops, &self->m_buffer);

    self->m_font = TTF_OpenFontRW(self->m_rwops, 0, self->m_size);
    if (self->m_font == NULL)
    {
        printf("ERROR - Loading font %s\n", self->m_fileName);
        printf("      - %s\n", TTF_GetError());
        assert(false);
        abort();
    }
}

static void FontData_clear(FontData *self)
{
    if (self->m_font)
    {
        TTF_CloseFont(self->m_font);
        AssetManager_destroyRWops(self->m_rwops, self->m_buffer);
    }
    free(self->m_fileName);
    memset(self, 0, sizeof(FontData));
}

static void MusicData_load(MusicData *self)
{
    AssetManager_createRWops(self->m_fileName, &self->m_rwops, &self->m_buffer);

    self->m_music = Mix_LoadMUS_RW(self->m_rwops, 0);
    if (self->m_music == NULL)
    {
        printf("ERROR - Loading music %s\n", self->m_fileName);
        printf("      - %s\n", Mix_GetError());
        assert(false);
        abort();
    }
}

static void MusicData_clear(MusicData *self)
{
    if (self->m_music)
    {
        Mix_FreeMusic(self->m_music);
        AssetManager_destroyRWops(self->m_rwops, self->m_buffer);
    }
    free(self->m_fileName);
    memset(self, 0, sizeof(MusicData));
}

static void AssetManager_obfuscateMem(void *memory, size_t size)
{
    char *buffer = (char *)memory;
    buffer[0] ^= 0x73;
    buffer[0] = 0xBB * buffer[0] + 0xC9;
    for (size_t i = 1; i < size; i++)
    {
        buffer[i] ^= buffer[i - 1];
        buffer[i] = 0xBB * buffer[i] + 0xC9;
    }
}

static void AssetManager_retriveMem(void *memory, size_t size)
{
    char *buffer = (char *)memory;
    for (size_t i = size - 1; i > 0; i--)
    {
        buffer[i] = 0x73 * (buffer[i] + 0x37);
        buffer[i] ^= buffer[i - 1];
    }
    buffer[0] = 0x73 * (buffer[0] + 0x37);
    buffer[0] ^= 0x73;
}

void AssetManager_createRWops(const char *fileName, SDL_RWops **rwops, void **buffer)
{
    FILE *file = fopen(fileName, "rb");
    if (file == NULL)
    {
        printf("ERROR - The file %s cannot be opened\n", fileName);
        assert(false);
        abort();
    }

    fseek(file, 0, SEEK_END);
    size_t size = (size_t)ftell(file);
    rewind(file);
    uint8_t *mem = (unsigned char *)calloc(size, 1);
    AssertNew(mem);

    *buffer = (void *)mem;

    size_t freadCount = fread(mem, 1, size, file);
    assert(freadCount == size);
    fclose(file); file = NULL;

    if (size > 2 && mem[0] == (uint8_t)0x0B && mem[1] == (uint8_t)0xF7)
    {
        // Utilisation du magic number 0x0BF7 pour les fichiers obsfusqués
        mem += 2;
        size -= 2;
        AssetManager_retriveMem((void *)mem, size);
    }

    *rwops = SDL_RWFromConstMem(mem, (int)size);

    if (*rwops == NULL)
    {
        printf("ERROR - Loading RWops %s\n", fileName);
        printf("      - %s\n", SDL_GetError());
        assert(false);
        abort();
    }
}

void AssetManager_destroyRWops(SDL_RWops *rwops, void *buffer)
{
    if (rwops) SDL_RWclose(rwops);
    free(buffer);
}

SDL_Texture *AssetManager_loadTexture(SDL_Renderer *renderer, const char *fileName)
{
    void *buffer = NULL;
    SDL_RWops *rwops = NULL;
    AssetManager_createRWops(fileName, &rwops, &buffer);

    SDL_Texture *texPtr = IMG_LoadTexture_RW(renderer, rwops, 0);
    if (texPtr == NULL)
    {
        printf("ERROR - Loading texture %s\n", fileName);
        printf("      - %s\n", IMG_GetError());
        assert(false);
        abort();
    }

    AssetManager_destroyRWops(rwops, buffer);

    return texPtr;
}
