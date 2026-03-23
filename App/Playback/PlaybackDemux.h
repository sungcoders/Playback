#ifndef PLAYBACKDEMUX_H
#define PLAYBACKDEMUX_H

extern "C" {
#include <libavformat/avformat.h>
// #include <libavcodec/avcodec.h>
}

#include "PlaybackPacket.h"
#include "PlaybackDecodeVideo.h"
#include <unistd.h>

class PlaybackDemux
{
public:
    PlaybackDemux() = default;
    ~PlaybackDemux() = default;

    void Start();
    void Init(const std::string& filename);
    void Demux();

private:
    AVFormatContext* fmtCtx = nullptr;
    AVPacket* packet = av_packet_alloc();
    int videoStream = -1;
    AVCodecContext* codecCtx = nullptr;
    AVCodecParameters* codecPar = nullptr;
    const AVCodec* codec = nullptr;
    std::thread demuxThread;
    int width = -1;
    int height = -1;
    std::shared_ptr<PlaybackPacket> packetVideoQueue = std::make_shared<PlaybackPacket>();
    std::shared_ptr<PlaybackPacket> packetAudioQueue = std::make_shared<PlaybackPacket>();
    std::unique_ptr<PlaybackDecodeVideo> decodeVideo = std::make_unique<PlaybackDecodeVideo>(packetVideoQueue);
};

#endif // PLAYBACKDEMUX_H
