#pragma once

#include <memory>

#include "Frame.h"
#include "Packet.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixfmt.h>
}

class CodecContext {
public:
  CodecContext(const AVCodecParameters* params);
  CodecContext(const AVCodecParameters*, AVDictionary** options);

  int             Width() { return m_Inner->width; }
  int             Height() { return m_Inner->height; }
  AVPixelFormat   PixFmt() { return m_Inner->pix_fmt; }
  AVCodecContext* Raw();
  int             SendPacket(Packet* pkt);
  int             ReceiveFrame(Frame* frame);

private:
  struct Deleter {
    void operator()(AVCodecContext* ctx) {
      if (ctx) {
        avcodec_free_context(&ctx);
      }
    }
  };

  std::unique_ptr<AVCodecContext, Deleter> m_Inner;
};
