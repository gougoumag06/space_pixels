/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/title/title_scene.h"

TitleScene *TitleScene_create(GameConfig *gameConfig)
{
    TitleScene *self = (TitleScene *)calloc(1, sizeof(TitleScene));
    AssertNew(self);

    self->m_assets = AssetManager_create(
        SPRITE_COUNT, FONT_COUNT, SOUND_COUNT, MUSIC_COUNT);
    Game_addAssets(self->m_assets);

    self->m_input = Input_create();

    self->m_camera = Camera_create(Game_getWidth(), Game_getHeight());
    self->m_gizmos = Gizmos_create(self->m_camera);
    self->m_state = SCENE_STATE_FADING_IN;
    self->m_fadingTime = 0.5f;
    self->m_ui = TitleUI_create(self);
    self->m_gameConfig = gameConfig;

    self->m_gameConfig->nextScene = GAME_SCENE_LEVEL;

    Game_playMusic(self->m_assets, MUSIC_POWER);
    TitleUI_update(self->m_ui);

    return self;
}

void TitleScene_destroy(TitleScene *self)
{
    if (!self) return;

    AssetManager_destroy(self->m_assets);
    Input_destroy(self->m_input);
    Camera_destroy(self->m_camera);
    Gizmos_destroy(self->m_gizmos);
    TitleUI_destroy(self->m_ui);

    free(self);
}

void TitleScene_mainLoop(TitleScene *self, bool drawGizmos)
{
    assert(self && "The TitleScene must be created");
    while (true)
    {
        // Met à jour la scène
        Timer_update(g_time);
        TitleScene_update(self);

        Input *input = TitleScene_getInput(self);
        if (input->quitPressed)
        {
            self->m_gameConfig->nextScene = GAME_SCENE_QUIT;
            return;
        }

        if (self->m_state == SCENE_STATE_FINISHED)
            return;

        // Rend la scène
        TitleScene_render(self);

        if (drawGizmos) TitleScene_drawGizmos(self);

        // Affiche le nouveau rendu
        SDL_RenderPresent(g_renderer);
    }
}

void TitleScene_update(TitleScene *self)
{
    assert(self && "The TitleScene must be created");
    Input_update(self->m_input);

    if (self->m_state == SCENE_STATE_FADING_IN)
    {
        self->m_accu += Timer_getUnscaledDelta(g_time);
        if (self->m_accu >= self->m_fadingTime)
        {
            self->m_state = SCENE_STATE_RUNNING;
        }
    }
    if (self->m_state == SCENE_STATE_FADING_OUT)
    {
        self->m_accu += Timer_getUnscaledDelta(g_time);
        if (self->m_accu >= self->m_fadingTime)
        {
            self->m_state = SCENE_STATE_FINISHED;
        }
    }

    TitleUI_update(self->m_ui);
}

void TitleScene_quit(TitleScene *self)
{
    assert(self && "The TitleScene must be created");
    if ((self->m_state == SCENE_STATE_FADING_OUT) ||
        (self->m_state == SCENE_STATE_FINISHED))
        return;

    self->m_state = SCENE_STATE_FADING_OUT;
    self->m_accu = 0.f;
}

void TitleScene_render(TitleScene *self)
{
    assert(self && "The TitleScene must be created");
    AssetManager *assets = TitleScene_getAssetManager(self);

    // Efface le rendu précédent
    SDL_SetRenderDrawColor(g_renderer, 37, 37, 37, 255);
    SDL_RenderClear(g_renderer);

    SDL_SetHintWithPriority(SDL_HINT_RENDER_SCALE_QUALITY, "0", SDL_HINT_OVERRIDE);

    // Affiche l'interface utilisateur
    TitleUI_render(self->m_ui);

    // Fading
    if ((self->m_state == SCENE_STATE_FADING_IN) ||
        (self->m_state == SCENE_STATE_FADING_OUT))
    {
        int opacity = (int)(255.f * self->m_accu / self->m_fadingTime);
        if (self->m_state == SCENE_STATE_FADING_IN)
        {
            opacity = 255 - opacity;
        }
        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, opacity);
        SDL_RenderFillRect(g_renderer, NULL);
    }
}

void TitleScene_drawGizmos(TitleScene *self)
{
    assert(self && "The TitleScene must be created");
    Gizmos *gizmos = self->m_gizmos;
    TitleUI_drawGizmos(self->m_ui, gizmos);
}
