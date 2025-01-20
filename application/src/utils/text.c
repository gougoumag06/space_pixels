/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "utils/text.h"
#include "utils/common.h"

void Text_refreshTexture(Text *self);

Text *Text_create(SDL_Renderer *renderer, TTF_Font *font, const char *str, SDL_Color color)
{
    assert(renderer && "The SDL_Renderer must be created");
    assert(font && "The TTF_Font must be created");
    assert(str && "The string must be valid");

    Text *self = (Text *)calloc(1, sizeof(Text));
    AssertNew(self);

    self->m_renderer = renderer;
    self->m_font = font;
    self->m_str = Strdup(str);
    self->m_color = color;
    self->m_texture = NULL;

    Text_refreshTexture(self);

    return self;
}

void Text_destroy(Text *self)
{
    if (!self) return;

    if (self->m_texture)
    {
        SDL_DestroyTexture(self->m_texture);
    }
    free(self->m_str);
    free(self);
}

void Text_setString(Text *self, const char *str)
{
    assert(self && "The Text must be created");
    assert(str && "The string must be valid");
    if (strcmp(self->m_str, str) == 0)
    {
        return;
    }

    free(self->m_str);
    self->m_str = Strdup(str);

    Text_refreshTexture(self);
}

void Text_setColor(Text *self, SDL_Color color)
{
    assert(self && "The Text must be created");
    if (memcmp(&(self->m_color), &color, sizeof(SDL_Color)) == 0)
    {
        return;
    }

    self->m_color = color;
    Text_refreshTexture(self);
}

void Text_refreshTexture(Text *self)
{
    assert(self && "The Text must be created");

    if (self->m_texture)
    {
        SDL_DestroyTexture(self->m_texture);
        self->m_texture = NULL;
    }

    SDL_Surface *surface = TTF_RenderUTF8_Blended(self->m_font, self->m_str, self->m_color);
    AssertNew(surface);
    self->m_texture = SDL_CreateTextureFromSurface(self->m_renderer, surface);
    AssertNew(self->m_texture);

    SDL_FreeSurface(surface);
}

