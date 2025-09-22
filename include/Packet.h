#pragma once

#include <memory>

#include "FormatContext.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

class Packet {
public:
  Packet();

  void      Unref();
  AVPacket* Raw();
  int       StreamIndex();

  int ReadFrame(FormatContext* fmt_ctx);

private:
  struct Deleter {
    void operator()(AVPacket* pkt) {
      if (pkt) {
        av_packet_free(&pkt);
      }
    }
  };

  std::unique_ptr<AVPacket, Deleter> m_Inner;
};
