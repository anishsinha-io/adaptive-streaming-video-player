#include "Packet.h"

Packet::Packet() : m_Inner(av_packet_alloc()) {}

void Packet::Unref() { av_packet_unref(m_Inner); }

AVPacket* Packet::Raw() { return m_Inner; }
int       Packet::StreamIndex() { return m_Inner->stream_index; }

int Packet::ReadFrame(FormatContext* fmt_ctx) {
  return av_read_frame(fmt_ctx->Raw(), m_Inner);
}
