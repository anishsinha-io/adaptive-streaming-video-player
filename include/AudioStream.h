#pragma once

class AudioStreamVariant {
public:
  int operator<=>(const AudioStreamVariant& rhs) {
    return bitrate - rhs.bitrate;
  }

private:
  int nb_channels;
  int sample_rate;
  int bitrate;
};
