#pragma once

#include <array>

#include "CodecContext.h"
#include "FormatContext.h"
#include "Frame.h"
#include "Packet.h"
#include "SWScaleContext.h"

extern "C" {
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

class Video {
public:
  Video(const std::string& hls_package_uri);
  ~Video();

  void Update();

  std::array<uint8_t*, 4> RgbData();
  std::array<int, 4>      RgbStride();
  std::pair<int, int>     DisplayDimensions();
  Frame*                  CurrentFrame() const noexcept;
  AVRational              CurrentStreamTimeBase() noexcept;

private:
  FormatContext                   m_FmtCtx;
  std::unique_ptr<CodecContext>   m_CodecCtx;
  std::unique_ptr<Packet>         m_Pkt;
  std::unique_ptr<Frame>          m_Frame;
  std::unique_ptr<SWScaleContext> m_SwsCtx;
  int                             m_CurrentVideoStreamIndex;
  std::array<uint8_t*, 4>         m_RgbData{};
  std::array<int, 4>              m_RgbStride{};
  int                             m_BufSize{0};
  std::pair<int, int>             m_DisplayDimensions;
};
