#pragma once

#include <iostream>
#include <memory>
#include <string>

extern "C" {
#include <libavformat/avformat.h>
}

class FormatContext {
public:
  FormatContext(const std::string& hls_package_uri);
  FormatContext(FormatContext&& other) noexcept
      : m_HLSPackageURI(std::move(other.m_HLSPackageURI)),
        m_Inner(std::move(other.m_Inner)) {}

  const AVCodecParameters* CodecParams(int stream_index) const noexcept {
    if (stream_index >= m_Inner->nb_streams) {
      return nullptr;
    }

    return m_Inner->streams[stream_index]->codecpar;
  }

  AVFormatContext* Raw();
  int              BestVideoStream() {
    return av_find_best_stream(m_Inner.get(), AVMEDIA_TYPE_VIDEO, -1, -1, NULL,
                                            0);
  }

private:
  struct Deleter {
    void operator()(AVFormatContext* ctx) const {
      if (ctx) {
        avformat_close_input(&ctx);
      }
      std::cout << "destruct formatcontext" << "\n";
    }
  };

  const std::string                         m_HLSPackageURI;
  std::unique_ptr<AVFormatContext, Deleter> m_Inner;
};
