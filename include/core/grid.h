#pragma once

#ifndef R2048_CORE_GRID_H
#define R2048_CORE_GRID_H

#include <stdint.h>
#include <stdbool.h>

struct Grid {
    uint8_t size;
    uint64_t * cells;
    uint16_t length;
};

typedef struct Grid * Grid;

/**
 * Initialize a grid with the given width and height
 *
 * @param[out] grid pointer to the grid to be initialized
 * @param size size of the grid (e.g: 4 for 4x4 grid)
 **/
void GridInit(Grid * grid, uint8_t size);

/**
 * Check if the cell at the given index is available
 *
 * @param[in] grid grid to check
 * @param index index of the cell to check
 * @return true if the cell is available, false otherwise
 **/
bool GridCellAvailable(Grid grid, uint64_t index);

/**
 * Check if any cell in the grid is available
 *
 * @param[in] grid grid to check
 * @return true if any cell is available, false otherwise
 **/
bool GridAnyCellAvailable(Grid grid);

/**
 * Get an array that contains the index of all available cells
 *
 * @param[in] grid grid to get the available cells from
 * @param array pointer to the array to store the available cells
 * @note
 * The array must be large enough to store all available cells or else
 * it will miss some cells.
 * @param size size of the array to store the available cells
 * @return number of available cells
 *
 **/
uint64_t GridGetAvailableCells(Grid grid, uint16_t * array, uint16_t size);

/**
 * Free the memory allocated for the grid
 *
 * @param[out] grid grid to free
 **/
void GridFree(Grid * grid);


#endif
