#include "FormatContext.h"

FormatContext::FormatContext(const std::string& hls_package_uri)
    : m_HLSPackageURI(hls_package_uri) {
  AVFormatContext* fmt_ctx = nullptr;

  int status =
      avformat_open_input(&fmt_ctx, hls_package_uri.c_str(), nullptr, nullptr);
  if (status < 0) {
    char errbuf[256];
    av_strerror(status, errbuf, sizeof(errbuf));
    fprintf(stderr, "failed to open input: %s\n", errbuf);
    exit(1);
  }

  if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
    fprintf(stderr, "failed to find stream info\n");
    avformat_close_input(&fmt_ctx);
    exit(1);
  }

  m_Inner = std::unique_ptr<AVFormatContext, Deleter>(fmt_ctx);
}

AVFormatContext* FormatContext::Raw() { return m_Inner.get(); }
