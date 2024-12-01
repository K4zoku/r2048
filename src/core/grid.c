#include "core/grid.h"
#include <stdlib.h>

void GridInit(Grid * grid, uint8_t size) {
    *grid = malloc(sizeof(Grid));
    (*grid)->size = size;
    (*grid)->length = size * size;
    (*grid)->cells = calloc((*grid)->length, sizeof(uint64_t));
}

bool GridCellAvailable(Grid grid, uint64_t index) {
    return grid->cells[index] == 0;
}

bool GridAnyCellAvailable(Grid grid) {
    uint64_t index;
    for (index = 0; index < grid->length; ++index) {
        if (GridCellAvailable(grid, index)) {
            return true;
        }
    }
    return false;
}

uint64_t GridGetAvailableCells(Grid grid, uint16_t * array, uint16_t size) {
    uint64_t count = 0;
    uint16_t index;
    for (index = 0; index < grid->length; ++index) {
        if (GridCellAvailable(grid, index)) {
            array[count++] = index;
            if (count == size) {
                break; // prevent buffer overflow
            }
        }
    }
    return count;
}

void GridFree(Grid * grid) {
    free((*grid)->cells);
    free(*grid);
    *grid = NULL;
}
