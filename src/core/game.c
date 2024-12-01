#include "core/game.h"
#include "random.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


uint16_t GetDirectedIndex(Grid grid, Direction direction, uint8_t x, uint8_t y) {
    int edge, area, a, b;
    edge = grid->size;
    area = edge * edge - 1;
    a = !!(direction & 2);
    b = direction & 1;
    return (1 - 2 * a) * (x * (edge * b - b + 1) + b * y + edge * y * (1 - b)) + a * area;
}

void GameInit(Game *game, uint8_t size) {
  *game = (Game)malloc(sizeof(struct Game));
  Grid grid;
  GridInit(&grid, size);
  (*game)->grid = grid;
  (*game)->score = 0;
  (*game)->moves = 0;
  (*game)->re = Xoshiro256ssEngine.ctor();
  GameAddRandomTiles(*game, 2);
}

bool GameMove(Game game, Direction direction) {
  bool moved = false;
  uint8_t x, y, xx;
  uint16_t index;
  Grid grid = game->grid;

  for (y = 0; y < grid->size; ++y) {
    uint16_t previous = GetDirectedIndex(grid, direction, 0, y);
    for (x = 1; x < grid->size; ++x) {
      index = GetDirectedIndex(grid, direction, x, y);
      if (grid->cells[index]) {
        if (grid->cells[index] == grid->cells[previous]) {
          grid->cells[previous] <<= 1;
          grid->cells[index] = 0;

          game->score += grid->cells[previous];
          moved = true;
        }
        previous = index;
      }
    }
    for (x = 0; x < grid->size; ++x) {
      index = GetDirectedIndex(grid, direction, x, y);
      if (grid->cells[index] == 0) {
        for (xx = x + 1; xx < grid->size; ++xx) {
          uint16_t next = GetDirectedIndex(grid, direction, xx, y);
          if (grid->cells[next]) {
            grid->cells[index] = grid->cells[next];
            grid->cells[next] = 0;
            moved = true;
            break;
          }
        }
      }
    }
  }
  return moved;
}

int GameAddRandomTile(Game game) {
  random_engine_t *re = game->re;
  Grid grid = game->grid;
  uint8_t len = grid->length;

  uint16_t available[len];
  uint16_t n_available = GridGetAvailableCells(game->grid, available, len);
  if (n_available == 0) {
    return -1;
  }

  uint16_t index = available[uniform_int_distribution(re, 0, n_available - 1)];
  grid->cells[index] = bernoulli_distribution(re, 0.9)
                           ? 2
                           : 4; // 90% chance of 2, 10% chance of 4
  return index;
}

static inline void ArrayErase(uint16_t *array, uint8_t len, uint8_t index) {
  for (; index < len - 1; ++index) {
    array[index] = array[index + 1];
  }
}

bool GameAddRandomTiles(Game game, uint16_t n_tiles) {
  bool added = true;
  random_engine_t *re = game->re;
  Grid grid = game->grid;
  uint8_t len = grid->length;

  uint16_t available[len];
  uint16_t n_available = GridGetAvailableCells(game->grid, available, len);
  if (n_available == 0) {
    return false;
  }

  while (n_tiles--) {
    if (n_available == 0) {
      added = false;
      break;
    }
    uint8_t choosen = uniform_int_distribution(re, 0, n_available - 1);
    uint16_t index = available[choosen];
    ArrayErase(available, n_available,
               choosen); // n_available instead of len cuz it's smaller
    --n_available;

    grid->cells[index] = bernoulli_distribution(re, 0.9) ? 2 : 4;
  }

  return added;
}

bool GameTileMatchesAvailable(Game game) {
  Direction direction;
  Grid grid = game->grid;

  for (direction = 0; direction < 2; ++direction) {
    uint16_t x, y;
    for (y = 0; y < grid->size; ++y) {
      for (x = 1; x < grid->size; ++x) {
        uint16_t index = GetDirectedIndex(grid, direction, x, y);
        if (grid->cells[index]) {
          uint16_t neighbor = GetDirectedIndex(grid, direction, x - 1, y);
          if (grid->cells[index] == grid->cells[neighbor]) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

void GameFree(Game *game) {
  // Free the memory allocated for the game
  GridFree(&(*game)->grid);
  random_engine_dtor((*game)->re);
  free(*game);
  *game = NULL;
}
