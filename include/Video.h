#pragma once

#include <array>
#include <vector>

#include "AudioStream.h"
#include "CodecContext.h"
#include "FormatContext.h"
#include "Frame.h"
#include "Packet.h"
#include "SWScaleContext.h"
#include "VideoStream.h"

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

  VideoStreamVariant BestVideoStream() const noexcept {
    return m_VideoStreamLadder.back();
  }

  VideoStreamVariant WorstVideoStream() const noexcept {
    return m_VideoStreamLadder.front();
  }

  VideoStreamVariant DefaultVideoStream() const noexcept {
    return m_VideoStreamLadder.front();
  }

  AudioStreamVariant BestAudioStream() const noexcept;
  AudioStreamVariant WorstAudioStream() const noexcept;
  AudioStreamVariant DefaultAudioStream() const noexcept;

private:
  std::vector<VideoStreamVariant> MakeVideoStreamLadder();
  std::vector<AudioStreamVariant> MakeAudioStreamLadder();

  FormatContext                   m_FmtCtx;
  std::unique_ptr<CodecContext>   m_CodecCtx;
  std::unique_ptr<Packet>         m_Pkt;
  std::unique_ptr<Frame>          m_Frame;
  std::unique_ptr<SWScaleContext> m_SwsCtx;
  int                             m_CurrentAudioStreamIndex;

  std::unique_ptr<VideoStreamVariant> m_CurrentStream;

  std::vector<VideoStreamVariant> m_VideoStreamLadder;
  std::vector<AudioStreamVariant> m_AudioStreamLadder{};

  std::array<uint8_t*, 4> m_RgbData{};
  std::array<int, 4>      m_RgbStride{};
  int                     m_BufSize{0};
  std::pair<int, int>     m_DisplayDimensions;
};
