#include "Frame.h"

Frame::Frame() : m_Inner(av_frame_alloc()) {}

void Frame::Unref() { av_frame_unref(m_Inner); }

AVFrame* Frame::Raw() { return m_Inner; }
