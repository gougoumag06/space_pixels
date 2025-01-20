/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "utils/text.h"
#include "utils/gizmos.h"
#include "game/game_common.h"

typedef struct TitleScene TitleScene;

typedef struct TitleUI
{
    TitleScene *m_scene;
    Text *m_textTitleMain;
    Text *m_textQuit;
    Text *m_textSelectLevel;
    Text *m_textLevels[32];

    int m_pageID;
    int m_selection;

    int m_hTitleMain;
    int m_hTitle;
    int m_hTexts;
} TitleUI;

TitleUI *TitleUI_create(TitleScene *scene);
void TitleUI_destroy(TitleUI *self);

void TitleUI_render(TitleUI *self);
void TitleUI_update(TitleUI *self);
void TitleUI_drawGizmos(TitleUI *self, Gizmos *gizmos);

void TitleUI_updateMainPage(TitleUI *self);
void TitleUI_updateLevelPage(TitleUI *self);

void TitleUI_renderBackground(TitleUI *self);
void TitleUI_renderMainPage(TitleUI *self);
void TitleUI_renderLevelPage(TitleUI *self);
