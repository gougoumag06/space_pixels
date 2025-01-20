/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
    test 3
    
*/

#include "settings.h"
#include "utils/common.h"
#include "game/game_common.h"
#include "game/input.h"
#include "game/level/level_scene.h"
#include "game/title/title_scene.h"

//#define FULLSCREEN
//#define WINDOW_FHD

#ifdef WINDOW_FHD
#define WINDOW_WIDTH   FHD_WIDTH
#define WINDOW_HEIGHT  FHD_HEIGHT
#else
#define WINDOW_WIDTH   HD_WIDTH
#define WINDOW_HEIGHT  HD_HEIGHT
#endif
#define LOGICAL_WIDTH  HD_WIDTH
#define LOGICAL_HEIGHT HD_HEIGHT

int main(int argc, char *argv[])
{
    //--------------------------------------------------------------------------
    // Initialisation
    
    // Initialisation de la SDL
    const Uint32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER;
    const Uint32 imgFlags = IMG_INIT_PNG;
    const Uint32 mixFlags = MIX_INIT_MP3;
    Game_init(sdlFlags, imgFlags, mixFlags);

    Game_setMusicVolume(0.5f);
    Game_setFXChannelsVolume(0.5f);

    // Crée la fenêtre et le moteur de rendu
    Uint32 windowFlags = 0;
#ifdef FULLSCREEN
    windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif
    Game_createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);
    Game_createRenderer(LOGICAL_WIDTH, LOGICAL_HEIGHT);

    //--------------------------------------------------------------------------
    // Boucle de jeu

    GameConfig gameConfig = { 0 };
    LevelScene *levelScene = NULL;
    TitleScene *titleScene = NULL;

    // Paramètres
    gameConfig.nextScene = GAME_SCENE_LEVEL;
    gameConfig.levelID = LEVEL_1;
    gameConfig.playerCount = 2;
    bool drawGizmos = true;

    bool quitGame = false;
    while (quitGame == false)
    {
        Int_clamp(gameConfig.playerCount, 1, MAX_PLAYER_COUNT); 
        switch (gameConfig.nextScene)
        {
        case GAME_SCENE_TITLE:
            titleScene = TitleScene_create(&gameConfig);
            TitleScene_mainLoop(titleScene, drawGizmos);

            TitleScene_destroy(titleScene);
            titleScene = NULL;
            break;

        case GAME_SCENE_LEVEL:
            levelScene = LevelScene_create(&gameConfig);
            LevelScene_mainLoop(levelScene, drawGizmos);

            LevelScene_destroy(levelScene);
            levelScene = NULL;
            break;

        case GAME_SCENE_QUIT:
        default:
            quitGame = true;
            break;
        }
    }

    //--------------------------------------------------------------------------
    // Libération de la mémoire

    TitleScene_destroy(titleScene);
    titleScene = NULL;
    LevelScene_destroy(levelScene);
    levelScene = NULL;

    Game_destroyRenderer();
    Game_destroyWindow();
    Game_quit();

    return EXIT_SUCCESS;
}
