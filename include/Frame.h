#pragma once

#include <iostream>
#include <memory>

extern "C" {
#include <libavformat/avformat.h>
}

class Frame {
public:
  Frame();
  void     Unref();
  AVFrame* Raw();
  int      Width() { return m_Inner->width; };
  int      Height() { return m_Inner->height; };

private:
  struct Deleter {
    void operator()(AVFrame* frame) const noexcept {
      if (frame) {
        av_frame_free(&frame);
      }
      std::cout << "destruct frame" << "\n";
    }
  };

  std::unique_ptr<AVFrame, Deleter> m_Inner;
};
