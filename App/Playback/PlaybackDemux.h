#ifndef PLAYBACKDEMUX_H
#define PLAYBACKDEMUX_H

extern "C" {
#include <libavformat/avformat.h>
}

#include "PlaybackPacket.h"
#include "PlaybackDecodeVideo.h"
#include <unistd.h>

class PlaybackDemux
{
public:
    PlaybackDemux();
    ~PlaybackDemux();

    void Start();
    void Init(const std::string& filename);
    void Demux();

private:
    AVFormatContext* fmtCtx;
    AVPacket* packet;
    int videoStream;
    AVCodecContext* codecCtx;
    AVCodecParameters* codecPar;
    const AVCodec* codec;
    std::shared_ptr<PlaybackPacket> m_pCpacketVideo;
    std::shared_ptr<PlaybackPacket> m_pCpacketAudio;
    std::unique_ptr<PlaybackDecodeVideo> m_pCdecode;
    std::thread demuxThread;
};

#endif // PLAYBACKDEMUX_H
