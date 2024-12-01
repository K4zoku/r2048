#include <assert.h>
#include <stddef.h>

#include "core/grid.h"

int main(void) {
    Grid grid;
    // TEST INITIALIZATION
    GridInit(&grid, 4);
    assert(grid->size == 4);
    assert(grid->length == 16);
    assert(grid->cells);
    for (uint64_t i = 0; i < grid->size; ++i) {
        assert(GridCellAvailable(grid, i));
    }
    // END TEST INITIALIZATION

    // TEST GridAnyCellAvailable
    uint16_t array[16];
    uint64_t count = GridGetAvailableCells(grid, array, 16);

    assert(count == 16);
    for (uint64_t i = 0; i < count; ++i) {
        assert(array[i] == i);
    }
    // END TEST GridAnyCellAvailable

    // TEST GridAnyCellAvailable with array of size 8
    uint16_t array2[8];
    count = GridGetAvailableCells(grid, array2, 8);
    assert(count == 8); // assert that function should not buffer overflow
    // END TEST GridAnyCellAvailable with array of size 8

    // TEST filling cells and checking if they are available
    // fill cells with 2
    for (uint16_t i = 0; i < grid->length; ++i) {
        grid->cells[i] = 2;
    }
    // check if cells are available
    // all cells should be unavailable
    for (uint16_t i = 0; i < grid->length; ++i) {
        assert(!GridCellAvailable(grid, i));
    }
    // END TEST filling cells and checking if they are available

    // TEST GridFree
    GridFree(&grid);
    assert(grid == NULL);
    // END TEST GridFree
}
