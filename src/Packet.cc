#include "Packet.h"

Packet::Packet()
    : m_Inner(std::unique_ptr<AVPacket, Deleter>(av_packet_alloc())) {}

void Packet::Unref() { av_packet_unref(m_Inner.get()); }

AVPacket* Packet::Raw() { return m_Inner.get(); }
int       Packet::StreamIndex() { return m_Inner->stream_index; }

int Packet::ReadFrame(FormatContext* fmt_ctx) {
  return av_read_frame(fmt_ctx->Raw(), m_Inner.get());
}
