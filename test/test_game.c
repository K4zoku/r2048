#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "core/game.h"
#include "core/grid.h"

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

    uint64_t gridCells[16] = {
        2, 0, 0, 2,
        0, 2, 2, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    const uint64_t expectedGridCells[4][16] = {
        {
            4, 0, 0, 0,
            4, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        {
            2, 2, 2, 2,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 0, 0, 4,
            0, 0, 0, 4,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            2, 2, 2, 2
        }
    };
    Direction direction = LEFT;
    memcpy(game->grid->cells, gridCells, 16 * sizeof(uint64_t));
    assert(memcmp(game->grid->cells, gridCells, 16 * sizeof(uint64_t)) == 0);
    GameMove(game, direction);
    assert(memcmp(game->grid->cells, expectedGridCells[direction], 16 * sizeof(uint64_t)) == 0);

    direction = UP;
    memcpy(game->grid->cells, gridCells, 16 * sizeof(uint64_t));
    GameMove(game, direction);
    assert(memcmp(game->grid->cells, expectedGridCells[direction], 16 * sizeof(uint64_t)) == 0);

    direction = RIGHT;
    memcpy(game->grid->cells, gridCells, 16 * sizeof(uint64_t));
    GameMove(game, direction);
    assert(memcmp(game->grid->cells, expectedGridCells[direction], 16 * sizeof(uint64_t)) == 0);

    direction = DOWN;
    memcpy(game->grid->cells, gridCells, 16 * sizeof(uint64_t));
    GameMove(game, direction);
    assert(memcmp(game->grid->cells, expectedGridCells[direction], 16 * sizeof(uint64_t)) == 0);

    // TEST Free
    GameFree(&game);
    assert(game == NULL);
    // END TEST Free
}
