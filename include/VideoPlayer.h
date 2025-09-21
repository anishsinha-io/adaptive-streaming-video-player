#pragma once

#include <raylib.h>

#include <memory>

#include "Video.h"

class VideoPlayer {
public:
  VideoPlayer(const std::string&  hls_package_uri,
              std::pair<int, int> display_dimensions);

  void SetTargetDisplayDimensions(int x, int y) noexcept;
  void Update();
  void Play();

private:
  std::unique_ptr<Video>     m_Src;
  std::pair<int, int>        m_TargetDisplayDimensions;
  std::unique_ptr<Texture2D> m_OutputTexture;
  bool                       m_PlaybackStarted{false};
  double                     m_StartTimeWall;
  double                     m_PauseOffset{0.0F};
  double                     m_Clock{0.0F};
  double                     m_FrameTimer{0.0F};
};
