#include "core/game.h"
#include "core/grid.h"
#include <raylib.h>
#include <stdlib.h>

// static const Color COLORS[] = {
//     DARKGRAY,  MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
//     GRAY,      RED,    GOLD,   LIME,      BLUE,     VIOLET,     BROWN,
//     LIGHTGRAY, PINK,   YELLOW, GREEN,     SKYBLUE,  PURPLE,     BEIGE};
static const Color COLORS[] = {
    LIGHTGRAY,
    BEIGE,  GREEN,  SKYBLUE, PURPLE,    RED,      GOLD,       LIME,      BLUE,
    VIOLET, MAROON, ORANGE,  DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
};

#define MAX_COLORS_COUNT (sizeof(COLORS) / sizeof(COLORS[0]))

Color GetTileColor(uint64_t number) {
  if (number == 0) {
    return LIGHTGRAY;
  }
  int i = -1;
  while (number) {
    ++i;
    number >>= 1;
  }
  return COLORS[i % MAX_COLORS_COUNT];
}

int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 640;
  const int screenHeight = 960;

  InitWindow(screenWidth, screenHeight, "r2048");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  const char txtMoves[] = "Moves";
  const int txtMovesSize = 16;
  const int txtMovesWidth = MeasureText(txtMoves, txtMovesSize);
  const char txtMovesCountFormat[] = "%u";
  const int txtMovesCountSize = 24;
  int txtMovesCountWidth = 0;

  const char txtScoreFormat[] = "%u";
  const int txtScoreSize = 40;
  int txtScoreWidth = 0;
  const char txtTileFormat[] = "%lu";
  int txtTileSize = 24;
  int txtTileWidth = 0;

  Game game;
  GameInit(&game, 4);
  const uint8_t gridSize = game->grid->size;
  const uint16_t gridLength = game->grid->length;
  Rectangle *tiles = (Rectangle *)calloc(gridLength, sizeof(Rectangle));
  const float gap = 10.0f;
  const float offsetY = 120.0f;
  const float margin = 20.0f;
  const float tileSize =
      ((float)GetScreenWidth() - (margin * 2)) / gridSize - (gap / 2);
  for (int i = 0; i < gridLength; ++i) {
    tiles[i].x = margin + tileSize * (i % gridSize) + gap * (i % gridSize);
    tiles[i].y = offsetY + tileSize * (i / gridSize) + gap * (i / gridSize);
    tiles[i].width = tileSize;
    tiles[i].height = tileSize;
  }
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    if (GridAnyCellAvailable(game->grid) || GameTileMatchesAvailable(game)) {
      int direction = -1;
      if (IsKeyPressed(KEY_LEFT)) {
        direction = LEFT;
      } else if (IsKeyPressed(KEY_UP)) {
        direction = UP;
      } else if (IsKeyPressed(KEY_RIGHT)) {
        direction = RIGHT;
      } else if (IsKeyPressed(KEY_DOWN)) {
        direction = DOWN;
      }
      if (direction != -1) {
        bool moved = GameMove(game, direction);
        if (moved) {
          GameAddRandomTile(game);
          ++game->moves;
        }
      }
    } else {
      // Game over
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);
    const char *txtScore = TextFormat(txtScoreFormat, game->score);
    txtScoreWidth = MeasureText(txtScore, txtScoreSize);
    DrawText(txtScore, (GetScreenWidth() - txtScoreWidth) / 2, 20, txtScoreSize,
             GRAY);
    const char *txtElapsed = "00:00:00";
    const int txtElapsedSize = 20;
    const int txtElapsedWidth = MeasureText(txtElapsed, txtElapsedSize);
    DrawText(txtElapsed, (GetScreenWidth() - txtElapsedWidth) / 2, 70,
             txtElapsedSize, GRAY);
    const char *txtMovesCount = TextFormat(txtMovesCountFormat, game->moves);
    txtMovesCountWidth = MeasureText(txtMovesCount, txtMovesCountSize);
    DrawText(txtMoves, 20, 20, txtMovesSize, GRAY);
    DrawText(txtMovesCount, 20 + (txtMovesWidth - txtMovesCountWidth) / 2,
             20 + txtMovesSize, txtMovesCountSize, GRAY);

    for (int i = 0; i < gridLength; ++i) // Draw all rectangles
    {
      DrawRectangleRec(tiles[i], GetTileColor(game->grid->cells[i]));
      if (game->grid->cells[i] == 0) {
        continue;
      }
      char txtTile[24];
      sprintf(txtTile, txtTileFormat, game->grid->cells[i]);
      while (MeasureText(txtTile, txtTileSize) > (int)tiles[i].width) {
        --txtTileSize;
      }
      txtTileWidth = MeasureText(txtTile, txtTileSize);
      int txtTileX =
          (int)(tiles[i].x + (tiles[i].width - (float)txtTileWidth) / 2);
      int txtTileY =
          (int)(tiles[i].y + (tiles[i].height - (float)txtTileSize) / 2);
      DrawText(txtTile, txtTileX, txtTileY, txtTileSize, RAYWHITE);
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  free(tiles);
  GameFree(&game);
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
