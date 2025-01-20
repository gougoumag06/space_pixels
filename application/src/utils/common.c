/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "utils/common.h"
#include "utils/asset_manager.h"

Timer *g_time = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_Window *g_window = NULL;

static int g_rendererW = 0;
static int g_rendererH = 0;

void Game_init(int sdlFlags, int imgFlags, int mixFlags)
{
    // Initialise la SDL2
    if (SDL_Init(sdlFlags) < 0)
    {
        printf("ERROR - SDL_Init %s\n", SDL_GetError());
        assert(false);
        abort();
    }

    // Initialise la SDL2 image
    if (IMG_Init(imgFlags) != imgFlags)
    {
        printf("ERROR - IMG_Init %s\n", IMG_GetError());
        assert(false);
        abort();
    }

    // Initialise la SDL2 TTF
    if (TTF_Init() == -1)
    {
        printf("ERROR - TTF_Init %s\n", TTF_GetError());
        assert(false);
        abort();
    }

    // Initialise la SDL2 Mixer
    if (Mix_Init(mixFlags) != mixFlags)
    {
        printf("ERROR - IMG_Init %s\n", Mix_GetError());
        assert(false);
        abort();
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        printf("ERROR - Mix_OpenAudio %s\n", Mix_GetError());
        assert(false);
        abort();
    }
    Mix_AllocateChannels(MIX_CHANNEL_COUNT);

    // Crée le temps global du jeu
    g_time = Timer_create();
    AssertNew(g_time);
}

void Game_createWindow(int width, int height, Uint32 flags)
{
    assert(g_window == NULL && "The window is already created");
    g_window = SDL_CreateWindow(
        u8"Space Pixels", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, flags
    );
    if (!g_window)
    {
        printf("ERROR - Create window %s\n", SDL_GetError());
        assert(false); abort();
    }
}

void Game_createRenderer(int width, int height)
{
    assert(g_renderer == NULL && "The renderer is already created");
    assert(g_window);
    g_renderer = SDL_CreateRenderer(
        g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!g_renderer)
    {
        printf("ERROR - Create renderer %s\n", SDL_GetError());
        assert(false); abort();
    }
    g_rendererW = width;
    g_rendererH = height;
    SDL_RenderSetLogicalSize(g_renderer, g_rendererW, g_rendererH);
    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
}

void Game_destroyRenderer()
{
    if (!g_renderer) return;
    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;
}

void Game_destroyWindow()
{
    if (!g_window) return;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
}

void Game_quit()
{
    Timer_destroy(g_time);
    g_time = NULL;

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game_setMusicVolume(float volume)
{
    int v = (int)(fminf(fmaxf(volume, 0.f), 1.f) * MIX_MAX_VOLUME);
    Mix_VolumeMusic(v);
}

void Game_setSoundVolume(AssetManager *assets, int soundID, float volume)
{
    assert(assets && "The asset manager must be created");
    Mix_Chunk *chunk = AssetManager_getSound(assets, soundID);
    if (chunk == NULL)
    {
        assert(false);
        return;
    }
    int v = (int)(fminf(fmaxf(volume, 0.f), 1.f) * MIX_MAX_VOLUME);
    Mix_VolumeChunk(chunk, v);
}

void Game_setFXChannelsVolume(float volume)
{
    int v = (int)(fminf(fmaxf(volume, 0.f), 1.f) * MIX_MAX_VOLUME);
    for (int channel = 0; channel < MIX_CHANNEL_COUNT; channel++)
    {
        Mix_Volume(channel, v);
    }
}

void Game_playSoundFX(AssetManager *assets, int soundID)
{
    assert(assets && "The asset manager must be created");
    Mix_Chunk *chunk = AssetManager_getSound(assets, soundID);
    if (chunk == NULL)
    {
        assert(false);
        return;
    }
    int loops = 0;
    int exitStatus = Mix_PlayChannel(-1, chunk, loops);
}

void Game_playMusic(AssetManager *assets, int musicID)
{
    assert(assets && "The asset manager must be created");
    Mix_Music *music = AssetManager_getMusic(assets, musicID);
    if (music == NULL)
    {
        assert(false);
        return;
    }
    int loops = -1;
    int exitStatus = Mix_PlayMusic(music, loops);
}

void Game_setRenderDrawColor(SDL_Color color, Uint8 alpha)
{
    assert(g_renderer && "The renderer must be created");
    SDL_SetRenderDrawColor(g_renderer, color.r, color.g, color.b, alpha);
}

void Memcpy(void *const dst, size_t dstSize, const void *src, size_t srcSize)
{
#ifdef _MSC_VER
    memcpy_s(dst, dstSize, src, srcSize);
#else
    memcpy(dst, src, (srcSize < dstSize) ? srcSize : dstSize);
#endif
}

char *Strdup(const char *src)
{
    size_t size = strlen(src);
    char *dst = (char *)calloc(size + 1, sizeof(char));
    AssertNew(dst);
    Memcpy(dst, size, src, size);
    return dst;
}

int Game_getWidth()
{
    return g_rendererW;
}

int Game_getHeight()
{
    return g_rendererH;
}
