#include "VideoStream.h"

#include <algorithm>

VideoStreamLadder::VideoStreamLadder(const AVStream** streams,
                                     const int        nb_streams) {
  m_Streams.reserve(nb_streams);

  for (int i = 0; i < nb_streams; i++) {
    const AVStream*    st  = streams[i];
    AVCodecParameters* par = st->codecpar;

    if (par->codec_type == AVMEDIA_TYPE_VIDEO) {
      m_Streams.push_back(
          {par->width, par->height,
           static_cast<float>(par->framerate.num) / par->framerate.den, i});
    }
  }

  std::sort(m_Streams.begin(), m_Streams.end());
}

const VideoStreamVariant VideoStreamLadder::BestVideoStream() {
  return m_Streams.back();
}

const VideoStreamVariant VideoStreamLadder::WorstVideoStream() {
  return m_Streams.front();
}

const VideoStreamVariant VideoStreamLadder::DefaultVideoStream() {
  return m_Streams.front();
}
