#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "core/game.h"
#include "core/grid.h"

void printDiff(const uint16_t * expected, const uint16_t * actual, uint8_t size) {
    puts("-----------------------------");
    printf("Expected: \n");
    for (int y = 0; y < size; ++y) {
      for (int x = 0; x < size; ++x) {
        uint16_t index = y * size + x;
        if (expected[index] == index) {
          printf("%*s ", 2, "x");
        } else {
          printf("%02d ", expected[index]);
        }
      }
      printf("\n");
    }
    printf("Actual: \n");
    for (int y = 0; y < size; ++y) {
      for (int x = 0; x < size; ++x) {
        uint16_t index = y * size + x;
        if (actual[index] == index) {
          printf("%*s ", 2, "x");
        } else {
          printf("%02d ", actual[index]);
        }
      }
      printf("\n");
    }
    puts("-----------------------------");
}

int main(void) {
  Game game;
  // TEST INITIALIZATION
  GameInit(&game, 4);
  assert(game);
  assert(game->grid);
  assert(game->grid->size == 4);
  assert(game->grid->length == 16);
  assert(game->grid->cells);
  assert(game->re);
  assert(game->score == 0);
  assert(game->moves == 0);
  uint16_t available[16];
  uint16_t n_available = GridGetAvailableCells(game->grid, available, 16);
  assert(n_available == 14); // 2 tiles randomly placed
  // END TEST INITIALIZATION

  uint64_t gridCells[16] = {2, 0, 0, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const uint64_t expectedGridCells[4][16] = {
      {4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2}};
  const uint16_t expectedDiff[4][16] = {
      {0, 1, 2, 0, 4, 4, 4, 7, 8, 9, 10, 11, 12, 13, 14, 15},
      {0, 1, 2, 3, 4, 1, 2, 7, 8, 9, 10, 11, 12, 13, 14, 15},
      {3, 1, 2, 3, 4, 7, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15},
      {12, 1, 2, 15, 4, 13, 14, 7, 8, 9, 10, 11, 12, 13, 14, 15}};
  Direction direction = LEFT;
  uint16_t diff[16];
  memcpy(game->grid->cells, gridCells, 16 * sizeof(uint64_t));
  assert(memcmp(game->grid->cells, gridCells, 16 * sizeof(uint64_t)) == 0);
  GameMove(game, direction, diff);
  assert(memcmp(game->grid->cells, expectedGridCells[direction],
                16 * sizeof(uint64_t)) == 0);
  printDiff(expectedDiff[direction], diff, 4);
  assert(memcmp(diff, expectedDiff[direction], 16 * sizeof(uint16_t)) == 0);

  direction = UP;
  memcpy(game->grid->cells, gridCells, 16 * sizeof(uint64_t));
  GameMove(game, direction, diff);
  assert(memcmp(game->grid->cells, expectedGridCells[direction],
                16 * sizeof(uint64_t)) == 0);
  printDiff(expectedDiff[direction], diff, 4);
  assert(memcmp(diff, expectedDiff[direction], 16 * sizeof(uint16_t)) == 0);

  direction = RIGHT;
  memcpy(game->grid->cells, gridCells, 16 * sizeof(uint64_t));
  GameMove(game, direction, diff);
  assert(memcmp(game->grid->cells, expectedGridCells[direction],
                16 * sizeof(uint64_t)) == 0);
  printDiff(expectedDiff[direction], diff, 4);
  assert(memcmp(diff, expectedDiff[direction], 16 * sizeof(uint16_t)) == 0);

  direction = DOWN;
  memcpy(game->grid->cells, gridCells, 16 * sizeof(uint64_t));
  GameMove(game, direction, diff);
  assert(memcmp(game->grid->cells, expectedGridCells[direction],
                16 * sizeof(uint64_t)) == 0);
  printDiff(expectedDiff[direction], diff, 4);
  assert(memcmp(diff, expectedDiff[direction], 16 * sizeof(uint16_t)) == 0);

  // TEST Free
  GameFree(&game);
  assert(game == NULL);
  // END TEST Free
}
