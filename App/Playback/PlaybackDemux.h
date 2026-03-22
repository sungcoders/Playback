#ifndef PLAYBACKDEMUX_H
#define PLAYBACKDEMUX_H

#include "PlaybackPacket.h"
extern "C" {
#include <libavformat/avformat.h>
// #include <libavcodec/avcodec.h>
}

class PlaybackDemux
{
public:
    PlaybackDemux();
    ~PlaybackDemux();

    void Start();
    void Init(const std::string& filename);
    void Demux();

private:
    AVFormatContext* fmtCtx = nullptr;
    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    int videoStream = -1;
    AVCodecContext* codecCtx = nullptr;
    AVCodecParameters* codecPar = nullptr;
    const AVCodec* codec = nullptr;
    std::thread demuxThread;
    int width = -1;
    int height = -1;
};

#endif // PLAYBACKDEMUX_H
