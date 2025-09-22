#pragma once

#include <utility>

extern "C" {
#include <libswscale/swscale.h>
}

class SWScaleContext {
public:
  template <typename... Args>
  SWScaleContext(Args&&... args)
      : m_Inner(sws_getContext(std::forward<Args>(args)...)) {}

  struct SwsContext* Raw();

  template <typename... Args>
  int Scale(Args&&... args) {
    return sws_scale(m_Inner, std::forward<Args>(args)...);
  }

private:
  struct Deleter {
    void operator()(SwsContext* ctx) const noexcept {
      if (ctx) {
        sws_freeContext(ctx);
      }
    }
  };

  struct SwsContext* m_Inner;
};
