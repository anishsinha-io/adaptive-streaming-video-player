#pragma once

#include <compare>
#include <vector>

extern "C" {
#include <libavformat/avformat.h>
}

class VideoStreamVariant {
public:
  VideoStreamVariant(int width, int height, float frame_rate, int stream_index)
      : m_Width(width),
        m_Height(height),
        m_FrameRate(frame_rate),
        m_StreamIndex(stream_index) {}

  std::strong_ordering operator<=>(const VideoStreamVariant& rhs) const {
    return RankingScore() <=> rhs.RankingScore();
  }

  int  FrameRate() { return m_FrameRate; }
  long RankingScore() const {
    return m_Width * m_Height + frame_rate_multiplier * m_FrameRate;
  }

  int StreamIndex() const noexcept { return m_StreamIndex; }

private:
  static const int frame_rate_multiplier = 5;

  int   m_Width;
  int   m_Height;
  float m_FrameRate;
  int   m_StreamIndex;
};

class VideoStreamLadder {
public:
  VideoStreamLadder(const AVStream** streams, const int nb_streams);

  const VideoStreamVariant BestVideoStream();
  const VideoStreamVariant WorstVideoStream();
  const VideoStreamVariant DefaultVideoStream();

private:
  std::vector<VideoStreamVariant> m_Streams;
};
