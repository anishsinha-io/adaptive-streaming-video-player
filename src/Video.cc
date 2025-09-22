#include "Video.h"

#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>

#include <algorithm>
#include <print>

Video::~Video() {
  if (m_RgbData[0]) {
    av_freep(&m_RgbData[0]);
  }
}

Video::Video(const std::string& hls_package_uri)
    : m_FmtCtx(FormatContext(hls_package_uri)),
      m_VideoStreamLadder(MakeVideoStreamLadder()) {
  m_CurrentStream = std::make_unique<VideoStreamVariant>(DefaultVideoStream());

  int current_stream_index = m_CurrentStream->StreamIndex();

  const AVCodecParameters* codecpar =
      m_FmtCtx.CodecParams(current_stream_index);
  m_CodecCtx = std::make_unique<CodecContext>(codecpar);
  m_Pkt      = std::make_unique<Packet>();
  m_Frame    = std::make_unique<Frame>();

  m_SwsCtx = std::make_unique<SWScaleContext>(
      m_CodecCtx->Width(), m_CodecCtx->Height(), m_CodecCtx->PixFmt(),
      m_CodecCtx->Width(), m_CodecCtx->Height(), AV_PIX_FMT_RGBA, SWS_BILINEAR,
      nullptr, nullptr, nullptr);

  m_BufSize =
      av_image_alloc(m_RgbData.data(), m_RgbStride.data(), m_CodecCtx->Width(),
                     m_CodecCtx->Height(), AV_PIX_FMT_RGBA, 32);

  m_DisplayDimensions = {m_Frame->Width(), m_Frame->Height()};
}

void Video::Update() {
  bool got_frame = false;

  while (!got_frame && m_Pkt->ReadFrame(&m_FmtCtx) >= 0) {
    if (m_Pkt->StreamIndex() == m_CurrentStream->StreamIndex()) {
      m_CodecCtx->SendPacket(m_Pkt.get());
      if (m_CodecCtx->ReceiveFrame(m_Frame.get()) == 0) {
        sws_scale(m_SwsCtx->Raw(), m_Frame->Raw()->data,
                  m_Frame->Raw()->linesize, 0, m_Frame->Raw()->height,
                  m_RgbData.data(), m_RgbStride.data());

        m_DisplayDimensions = {m_Frame->Width(), m_Frame->Height()};
        got_frame           = true;
      }
      m_Pkt->Unref();
    }
  }
}

std::array<uint8_t*, 4> Video::RgbData() { return m_RgbData; };
std::array<int, 4>      Video::RgbStride() { return m_RgbStride; };
std::pair<int, int> Video::DisplayDimensions() { return m_DisplayDimensions; }
Frame* Video::CurrentFrame() const noexcept { return m_Frame.get(); }

AVRational Video::CurrentStreamTimeBase() noexcept {
  int index = m_CurrentStream->StreamIndex();
  return m_FmtCtx.Raw()->streams[index]->time_base;
}

std::vector<VideoStreamVariant> Video::MakeVideoStreamLadder() {
  std::vector<VideoStreamVariant> ladder;

  AVFormatContext* raw        = m_FmtCtx.Raw();
  int              nb_streams = raw->nb_streams;
  ladder.reserve(nb_streams);

  for (int i = 0; i < nb_streams; i++) {
    AVStream*          st  = raw->streams[i];
    AVCodecParameters* par = st->codecpar;

    if (par->codec_type == AVMEDIA_TYPE_VIDEO) {
      ladder.push_back(
          {par->width, par->height,
           static_cast<float>(par->framerate.num) / par->framerate.den, i});
    }
  }

  std::sort(ladder.begin(), ladder.end());

  return ladder;
}
