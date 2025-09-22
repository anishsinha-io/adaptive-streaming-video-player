#include "CodecContext.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

CodecContext::CodecContext(const AVCodecParameters* params)
    : CodecContext(params, nullptr) {}

CodecContext::CodecContext(const AVCodecParameters* params,
                           AVDictionary**           options) {
  AVCodec* codec = const_cast<AVCodec*>(avcodec_find_decoder(params->codec_id));
  AVCodecContext* ctx = avcodec_alloc_context3(codec);
  avcodec_parameters_to_context(ctx, params);
  avcodec_open2(ctx, codec, options);
  m_Inner = ctx;
}

AVCodecContext* CodecContext::Raw() { return m_Inner; }
int             CodecContext::SendPacket(Packet* pkt) {
  return avcodec_send_packet(m_Inner, pkt->Raw());
}
int CodecContext::ReceiveFrame(Frame* frame) {
  return avcodec_receive_frame(m_Inner, frame->Raw());
}
