#pragma once

extern "C" {
#include <libavformat/avformat.h>
}

class Frame {
public:
  Frame();
  ~Frame() { av_frame_free(&m_Inner); }

  void     Unref();
  AVFrame* Raw();
  int      Width() { return m_Inner->width; };
  int      Height() { return m_Inner->height; };

private:
  AVFrame* m_Inner;
};
