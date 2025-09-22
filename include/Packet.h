#pragma once

#include "FormatContext.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

class Packet {
public:
  Packet();
  ~Packet() { av_packet_free(&m_Inner); }

  void      Unref();
  AVPacket* Raw();
  int       StreamIndex();

  int ReadFrame(FormatContext* fmt_ctx);

private:
  AVPacket* m_Inner;
};
