/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "utils/common.h"
#include "utils/camera.h"
#include "utils/gizmos.h"

#include "game/game_common.h"
#include "game/input.h"
#include "game/level/player.h"
#include "game/level/bullet.h"
#include "game/level/enemy.h"
#include "game/level/item.h"
#include "game/level/level_ui.h"
#include "game/level/level.h"

#define ENEMY_CAPACITY 32
#define ITEM_CAPACITY 8
#define BULLET_CAPACITY 256

/// @brief Structure représentant la scène d'un niveau du jeu.
typedef struct LevelScene
{
    GameConfig *m_gameConfig;
    AssetManager *m_assets;
    Camera *m_camera;
    Input *m_input;
    Gizmos *m_gizmos;
    LevelUI *m_ui;

    Level *m_level;

    Player *m_players[MAX_PLAYER_COUNT];
    int m_playerCount;

    Enemy *m_enemies[ENEMY_CAPACITY];
    int m_enemyCount;

    Bullet *m_bullets[BULLET_CAPACITY];
    int m_bulletCount;

    Item *m_items[ITEM_CAPACITY];
    int m_itemCount;

    bool m_isLocked;
    int m_state;
    float m_accu;
    float m_fadingTime;
} LevelScene;

/// @brief Crée la scène représentant un niveau du jeu.
/// @param gameConfig la configuration globale du jeu.
/// @return La scène créée.
LevelScene *LevelScene_create(GameConfig *gameConfig);

/// @brief Détruit la scène représentant un niveau du jeu.
/// @param self la scène.
void LevelScene_destroy(LevelScene *self);

/// @brief Boucle principale de la scène.
/// @param self la scène.
/// @param drawGizmos booléen indiquant s'il faut dessiner les gizmos.
void LevelScene_mainLoop(LevelScene *self, bool drawGizmos);

/// @brief Met à jour la scène.
/// Cette fonction est appelée à chaque tour de la boucle de rendu.
/// @param self la scène.
void LevelScene_update(LevelScene *self);

/// @brief Active l'animation de fin de scène.
/// La boucle principale s'arrête une fois l'animation terminée.
/// @param self la scène.
void LevelScene_quit(LevelScene *self);

/// @brief Dessine la scène dans le moteur de rendu.
/// @param self la scène.
void LevelScene_render(LevelScene *self);

/// @brief Dessine les gizmos de la scène dans le moteur de rendu.
/// @param self 
void LevelScene_drawGizmos(LevelScene *self);

/// @brief Ajoute un projectile à la scène.
/// @param self la scène.
/// @param bullet le projectile à ajouter.
void LevelScene_addBullet(LevelScene *self, Bullet *bullet);

/// @brief Ajoute un ennemi à la scène.
/// @param self la scène.
/// @param enemy l'ennemi à ajouter.
void LevelScene_addEnemy(LevelScene *self, Enemy *enemy);

/// @brief Ajoute un objet à la scène.
/// @param self la scène.
/// @param item l'objet à ajouter.
void LevelScene_addItem(LevelScene *self, Item *item);

/// @brief Renvoie la configuration globale du jeu.
/// @param self la scène.
/// @return La configuration globale du jeu.
INLINE GameConfig *LevelScene_getGameConfig(LevelScene *self)
{
    assert(self && "The LevelScene must be created");
    return self->m_gameConfig;
}

/// @brief Renvoie le gestionnaire des assets de la scène.
/// @param self la scène.
/// @return Le gestionnaire des assets de la scène.
INLINE AssetManager *LevelScene_getAssetManager(LevelScene *self)
{
    assert(self && "The LevelScene must be created");
    return self->m_assets;
}

/// @brief Renvoie la caméra de la scène.
/// @param self la scène.
/// @return La caméra de la scène.
INLINE Camera *LevelScene_getCamera(LevelScene *self)
{
    assert(self && "The LevelScene must be created");
    return self->m_camera;
}

/// @brief Renvoie le gestionnaire des entrées utilisateur de la scène.
/// @param self la scène.
/// @return Le gestionnaire des entrées utilisateur de la scène.
INLINE Input *LevelScene_getInput(LevelScene *self)
{
    assert(self && "The LevelScene must be created");
    return self->m_input;
}

/// @brief Renvoie le nombre de joueurs de la scène.
/// @param self la scène.
/// @return Le nombre de joueurs.
INLINE int LevelScene_getPlayerCount(LevelScene *self)
{
    assert(self && "The LevelScene must be created");
    return self->m_playerCount;
}

/// @brief Renvoie un des joueurs de la scène.
/// @param self la scène.
/// @param index l'indice du joueur (entre 0 et le nombre de joueurs - 1).
/// @return Le joueur.
INLINE Player *LevelScene_getPlayer(LevelScene *self, int index)
{
    assert(self && "The LevelScene must be created");
    assert(0 <= index && index < self->m_playerCount);
    return self->m_players[index];
}

/// @brief Renvoie le nombre d'ennemis de la scène.
/// @param self la scène.
/// @return Le nombre d'ennemis de la scène.
INLINE int LevelScene_getEnemyCount(LevelScene *self)
{
    assert(self && "The LevelScene must be created");
    return self->m_enemyCount;
}

/// @brief Renvoie un des ennemis de la scène.
/// @param self la scène
/// @param index l'indice de l'ennemi (entre 0 et le nombre d'ennemis - 1).
/// @return L'ennemi.
INLINE Enemy *LevelScene_getEnemy(LevelScene *self, int index)
{
    assert(self && "The LevelScene must be created");
    assert(0 <= index && index < self->m_enemyCount);
    return self->m_enemies[index];
}

/// @brief Renvoie le nombre d'objets de la scène.
/// @param self la scène.
/// @return Le nombre d'objets de la scène.
INLINE int LevelScene_getItemCount(LevelScene *self)
{
    assert(self && "The LevelScene must be created");
    return self->m_itemCount;
}

/// @brief Renvoie un des objets de la scène.
/// @param self la scène
/// @param index l'indice de l'objet (entre 0 et le nombre d'objets - 1).
/// @return L'objet.
INLINE Item *LevelScene_getItem(LevelScene *self, int index)
{
    assert(self && "The LevelScene must be created");
    assert(0 <= index && index < self->m_itemCount);
    return self->m_items[index];
}

/// @brief Renvoie le niveau associé à la scène.
/// @param self la scène.
/// @return Le niveau associé à la scène.
INLINE Level *LevelScene_getLevel(LevelScene *self)
{
    assert(self && "The LevelScene must be created");
    return self->m_level;
}
