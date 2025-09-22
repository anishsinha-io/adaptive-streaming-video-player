#include "VideoPlayer.h"

#include <raylib.h>

#include <iostream>

void VideoPlayer::SetTargetDisplayDimensions(int x, int y) noexcept {
  m_TargetDisplayDimensions.first  = x;
  m_TargetDisplayDimensions.second = y;
}

void VideoPlayer::Update() {
  if (!IsWindowReady()) {
    std::cout << "initialize the window before calling Video::Play" << "\n";
    exit(1);
  }

  using clock = std::chrono::steady_clock;
  using sec   = std::chrono::duration<double>;

  double now_wall =
      std::chrono::duration_cast<sec>(clock::now().time_since_epoch()).count();
  m_Clock = (now_wall - m_StartTimeWall) + m_PauseOffset;

  if (m_Clock >= m_FrameTimer) {
    m_Src->Update();
    UpdateTexture(*m_OutputTexture, m_Src->RgbData()[0]);
  }

  Frame*     current_frame = m_Src->CurrentFrame();
  auto       frame_time    = current_frame->Raw()->pts;
  AVRational time_base     = m_Src->CurrentStreamTimeBase();
  double     seconds =
      frame_time * (static_cast<float>(time_base.num) / time_base.den);
  m_FrameTimer = seconds;
}

void VideoPlayer::Play() {
  using clock = std::chrono::steady_clock;
  using sec   = std::chrono::duration<double>;

  if (!m_PlaybackStarted) {
    auto tp = clock::now();

    m_StartTimeWall =
        std::chrono::duration_cast<sec>(tp.time_since_epoch()).count();

    m_PlaybackStarted = true;
  }

  double now_wall =
      std::chrono::duration_cast<sec>(clock::now().time_since_epoch()).count();
  m_Clock = (now_wall - m_StartTimeWall) + m_PauseOffset;

  DrawTexturePro(*m_OutputTexture,
                 {0, 0, static_cast<float>(m_Src->DisplayDimensions().first),
                  static_cast<float>(m_Src->DisplayDimensions().second)},
                 {0, 0, static_cast<float>(m_TargetDisplayDimensions.first),
                  static_cast<float>(m_TargetDisplayDimensions.second)},
                 {0, 0}, 0.0F, WHITE);
}

VideoPlayer::VideoPlayer(const std::string&  hls_package_uri,
                         std::pair<int, int> target_display_dimensions)
    : m_Src(std::make_unique<Video>(hls_package_uri)),
      m_TargetDisplayDimensions(std::move(target_display_dimensions)),
      m_OutputTexture(std::make_unique<Texture2D>()) {
  m_Src->Update();

  Image* img   = new Image();
  img->data    = m_Src->RgbData().data()[0];
  img->width   = m_Src->DisplayDimensions().first;
  img->height  = m_Src->DisplayDimensions().second;
  img->mipmaps = 1;
  img->format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

  *m_OutputTexture = LoadTextureFromImage(*img);
}
