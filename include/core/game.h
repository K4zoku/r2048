#pragma once
#ifndef R2048_CORE_GAME_MANAGER_H
#define R2048_CORE_GAME_MANAGER_H

#include <stdbool.h>
#include <stdint.h>

#include "grid.h"
#include "random.h"

typedef enum { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 } Direction;

typedef struct HistoryState {
  uint64_t score;
  uint64_t *cells;
} HistoryState;

typedef struct Game {
  Grid grid;
  random_engine_t * re;
  uint64_t score;
  uint32_t moves;
  // [TODO] Add history
} *Game;

/**
 * Initialize a game with the given width and height
 *
 * @param[out] game pointer to the game to be initialized
 * @param size size of the grid
 **/
void GameInit(Game *game, uint8_t size);

/**
 * Move the tiles in the given direction
 *
 * @param[in] game game to move the tiles in
 * @param dir direction to move the tiles
 * @return true if the tiles were moved, false otherwise
 */
bool GameMove(Game game, Direction direction, uint16_t * diff);

/**
 * Add a random tile to the game
 *
 * @param[in] game game to add the tile to
 * @return index of inserted tile, -1 if no cell available
 */
int GameAddRandomTile(Game game);

/**
 * Add a given number of random tiles to the game
 *
 * @param[in] game game to add the tiles to
 * @param n_tiles number of tiles to add
 * @return true if the tiles were added, false otherwise
 */
bool GameAddRandomTiles(Game game, uint16_t n_tiles);

/**
 * Check if there are any tile matches available
 *
 * @param[in] game game to check
 * @return true if there are matches available, false otherwise
 */
bool GameTileMatchesAvailable(Game game);

/**
 * Free the memory allocated for the game
 *
 * @param[out] game pointer to the game to be freed
 **/
void GameFree(Game *game);

#endif
