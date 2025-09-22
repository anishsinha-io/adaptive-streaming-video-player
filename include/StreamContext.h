#pragma once

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

class StreamContext {
public:
private:
  FormatContext                       m_FmtCtx;
  std::unique_ptr<CodecContext>       m_CodecCtx;
  std::unique_ptr<Packet>             m_Pkt;
  std::unique_ptr<Frame>              m_Frame;
  std::unique_ptr<SWScaleContext>     m_SwsCtx;
  std::unique_ptr<VideoStreamVariant> m_CurrentStream;
  std::vector<VideoStreamVariant>     m_VideoStreamLadder;
  std::array<uint8_t*, 4>             m_RgbData{};
  std::array<int, 4>                  m_RgbStride{};
  int                                 m_BufSize{0};
  std::pair<int, int>                 m_DisplayDimensions;
};
