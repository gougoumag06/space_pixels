/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/level/level_ui.h"
#include "game/level/level_scene.h"

LevelUI *LevelUI_create(LevelScene *scene)
{
    LevelUI *self = (LevelUI *)calloc(1, sizeof(LevelUI));
    AssertNew(self);

    AssetManager *assets = LevelScene_getAssetManager(scene);
    self->m_scene = scene;

    TTF_Font *font = NULL;
    char buffer[1024] = { 0 };
    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        font = AssetManager_getFont(assets, FONT_BIG);
        self->m_healths[i] = Text_create(g_renderer, font, "100%", g_colors.green);
    }

    font = AssetManager_getFont(assets, FONT_MAIN_TITLE);
    self->m_textPause = Text_create(g_renderer, font, "PAUSE", g_colors.yellow);

    int h, w;
    font = AssetManager_getFont(assets, FONT_BIG);
    TTF_SizeText(font, "100%", &w, &h);
    self->m_hHealth = h;
    self->m_wHealth = w;

    return self;
}

void LevelUI_destroy(LevelUI *self)
{
    if (!self) return;

    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        Text_destroy(self->m_healths[i]);
    }
    Text_destroy(self->m_textPause);

    free(self);
}

void LevelUI_render(LevelUI *self)
{
    LevelScene *scene = self->m_scene;
    AssetManager *assets = LevelScene_getAssetManager(scene);
    const int playerCount = LevelScene_getPlayerCount(scene);

    int w, h;
    SDL_Rect dst = { 0 };
    SDL_Texture *texture = NULL;
    const int xMargin = 20;
    const int yMargin = 20;
    const int xPadding = 20;
    const int yPadding = 10;
    const int wUI = self->m_wHealth + 2 * xPadding;
    const int hUI = 2 * self->m_hHealth + 2 * yPadding;

    int x = xMargin;
    int y = yMargin;

    Game_setRenderDrawColor(g_colors.magenta, 127);
    dst.x = x;
    dst.y = y;
    dst.w = wUI;
    dst.h = hUI;
    SDL_RenderFillRect(g_renderer, &dst);

    x += xPadding;
    y += yPadding;

    for (int i = 0; i < playerCount; i++)
    {
        // Health (value)
        texture = Text_getTexture(self->m_healths[i]);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        dst.x = x;
        dst.y = y;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(g_renderer, texture, NULL, &dst);

        y += self->m_hHealth;
    }

    if (self->m_paused)
    {
        Game_setRenderDrawColor(g_colors.blue, 127);
        SDL_RenderFillRect(g_renderer, NULL);

        texture = Text_getTexture(self->m_textPause);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        dst.x = Game_getWidth() / 2 - w / 2;
        dst.y = Game_getHeight() / 2 - h / 2;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(g_renderer, texture, NULL, &dst);
    }
}

void LevelUI_update(LevelUI *self)
{
    LevelScene *scene = self->m_scene;
    AssetManager *assets = LevelScene_getAssetManager(scene);
    Input *input = LevelScene_getInput(scene);

    int playerCount = LevelScene_getPlayerCount(scene);
    for (int i = 0; i < playerCount; i++)
    {
        Player *player = LevelScene_getPlayer(scene, i);
        char buffer[128] = { 0 };
        sprintf(buffer, u8"%d%%", player->m_hp);
        Text_setString(self->m_healths[i], buffer);
    }


    if ((scene->m_level->m_state == LEVEL_STATE_COMPLETED) ||
        (scene->m_level->m_state == LEVEL_STATE_FAILED))
    {
        LevelScene_quit(scene);
    }
    if (scene->m_level->m_state == LEVEL_STATE_PLAYING)
    {
        if (input->pausePressed)
        {
            self->m_paused = !self->m_paused;
            float scale = self->m_paused ? 0.f : 1.f;
            Timer_setTimeScale(g_time, scale);
            Game_playSoundFX(assets, SOUND_UI_PAUSE);
        }
    }
}

void LevelUI_drawGizmos(LevelUI *self, Gizmos *gizmos)
{
}
