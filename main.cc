#include <raylib.h>

#include "VideoPlayer.h"

const int   SCREEN_HEIGHT = 800;
const int   SCREEN_WIDTH  = 1200;
const char* SCREEN_TITLE  = "Video Streaming/Playback";

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
  SetTargetFPS(60);

  VideoPlayer player("http://localhost:8859/video/manifest.m3u8", {900, 506});

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    player.Play();

    EndDrawing();

    player.Update();
  }

  return 0;
}
