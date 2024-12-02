#include "core/game.h"
#include "core/grid.h"
#include "random.h"
#include "reasing.h"
#include <raylib.h>
#include <stdlib.h>

static const Color COLORS[] = {
    BEIGE,  GREEN,  SKYBLUE, PURPLE,    RED,      GOLD,       LIME,      BLUE,
    VIOLET, MAROON, ORANGE,  DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
};

#define MAX_COLORS_COUNT (sizeof(COLORS) / sizeof(COLORS[0]))

Color GetTileColor(uint64_t number) {
  if (number == 0) {
    return LIGHTGRAY;
  }
  int i = -1;
  while (number >>= 1) {
    ++i;
  }
  return COLORS[i % MAX_COLORS_COUNT];
}

const float gap = 10.0f;
const float offsetY = 120.0f;
const float margin = 20.0f;

static inline Vector2 GetTilePosition(float tileSize, int index,
                                      uint8_t gridSize) {
  Vector2 pos = {0};
  pos.x = margin + tileSize * (index % gridSize) + gap * (index % gridSize);
  pos.y = offsetY + tileSize * (index / gridSize) + gap * (index / gridSize);
  return pos;
}

int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 640;
  const int screenHeight = 960;
  int framesCounter = 0;

  InitWindow(screenWidth, screenHeight, "r2048");

  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor())); // Set our game to run at monitor refresh rate
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
  bool gameOver = false;
  const uint8_t gridSize = game->grid->size;
  const uint16_t gridLength = game->grid->length;
  Rectangle *tiles = (Rectangle *)calloc(gridLength, sizeof(Rectangle));

  const float tileSize =
      (((float)GetScreenWidth() - (margin * 2) - (gap * (gridSize - 1))) / gridSize);
  for (int i = 0; i < gridLength; ++i) {
    Vector2 pos = GetTilePosition(tileSize, i, gridSize);
    tiles[i].x = pos.x;
    tiles[i].y = pos.y;
    tiles[i].width = tileSize;
    tiles[i].height = tileSize;
  }
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    ++framesCounter;
    if (gameOver) {
        if (IsKeyPressed(KEY_ENTER)) {
            GameFree(&game);
            GameInit(&game, 4);
            gameOver = false;
        }
    }
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
      gameOver = true;
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
    DrawFPS(GetScreenWidth() - 100, 20);

    for (int i = 0; i < gridLength; ++i) // Draw all rectangles
    {
      Rectangle bgTile = tiles[i];
      DrawRectangleRec(bgTile, LIGHTGRAY); // Draw background tile
      DrawRectangleRec(tiles[i], GetTileColor(game->grid->cells[i]));
      if (game->grid->cells[i] == 0) {
        continue;
      }
      const char *txtTile = TextFormat(txtTileFormat, game->grid->cells[i]);
      while (MeasureText(txtTile, txtTileSize) > (int)tiles[i].width - 4) {
        --txtTileSize;
      }
      txtTileWidth = MeasureText(txtTile, txtTileSize);
      int txtTileX =
          (int)(tiles[i].x + (tiles[i].width - (float)txtTileWidth) / 2);
      int txtTileY =
          (int)(tiles[i].y + (tiles[i].height - (float)txtTileSize) / 2);
      DrawText(txtTile, txtTileX, txtTileY, txtTileSize, RAYWHITE);
    }
    if (gameOver) {
      const char *txtGameOver = "Game Over!";
      const int txtGameOverSize = 54;
      const int txtGameOverWidth = MeasureText(txtGameOver, txtGameOverSize);
      const char *txtTryAgain = "Press [Enter] to try again";
      const int txtTryAgainSize = 20;
      const int txtTryAgainWidth = MeasureText(txtTryAgain, txtTryAgainSize);

      const Vector2 firstTilePos = GetTilePosition(tileSize, 0, gridSize);
      Vector2 lastTilePos = GetTilePosition(tileSize, gridLength - 1, gridSize);
      lastTilePos.x += tileSize + gap;
      lastTilePos.y += tileSize + gap;
      const Vector2 txtGameOverPos = {
          (GetScreenWidth() - txtGameOverWidth) / 2.0f,
          firstTilePos.y + ((lastTilePos.y - firstTilePos.y) / 2.0f) -
              ((txtGameOverSize / 2.0f) + (txtTryAgainSize / 2.0f) + 5),
      };
      const Vector2 txtTryAgainPos = {
          (GetScreenWidth() - txtTryAgainWidth) / 2.0f,
          txtGameOverPos.y + txtGameOverSize + 10,
      };

      Rectangle bg = {txtGameOverPos.x - 20, txtGameOverPos.y - 20,
                      txtGameOverWidth + 40, txtGameOverSize + 10 + txtTryAgainSize + 40};
      DrawRectangleRec(
          (Rectangle){
              bg.x + 6,
              bg.y + 6,
              bg.width,
              bg.height,
          },
          BLACK);
      DrawRectangleRec(bg, RAYWHITE);
      DrawRectangleRec(bg, Fade(RED, 0.3f));
      DrawRectangleLinesEx(bg, 4, RED);
      DrawText(txtGameOver, txtGameOverPos.x, txtGameOverPos.y, txtGameOverSize, RED);
      DrawText(txtTryAgain, txtTryAgainPos.x, txtTryAgainPos.y, txtTryAgainSize, GRAY);
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
