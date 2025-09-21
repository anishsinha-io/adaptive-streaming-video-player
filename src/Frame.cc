#include "Frame.h"

Frame::Frame() : m_Inner(std::unique_ptr<AVFrame, Deleter>(av_frame_alloc())) {}

void Frame::Unref() { av_frame_unref(m_Inner.get()); }

AVFrame* Frame::Raw() { return m_Inner.get(); }
