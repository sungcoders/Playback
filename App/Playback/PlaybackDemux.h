#ifndef PLAYBACKDEMUX_H
#define PLAYBACKDEMUX_H

extern "C" {
#include <libavformat/avformat.h>
}

#include "PlaybackPacket.h"
#include "PlaybackDecodeVideo.h"

class PlaybackDemux
{
public:
    PlaybackDemux(std::shared_ptr<PlaybackDecodeVideo> decode);
    ~PlaybackDemux();

    void Init(const std::string& filename);
    void Start(void);
    void Stop(void);

private:
    std::atomic<bool> m_bExit;
    AVFormatContext* m_fmtCtx;
    AVCodecContext* m_codecCtx;
    int m_idxvideoStream;
    int m_idxaudioStream;
    double m_dVideoTimeBase;
    double m_dAudioTimeBase;
    std::shared_ptr<PlaybackPacket> m_pCpacketVideo;
    std::shared_ptr<PlaybackPacket> m_pCpacketAudio;
    std::shared_ptr<PlaybackDecodeVideo> m_pCdecode;
    std::thread demuxThread;

    void Demux(void);
    void pushPacketAV(AVPacket* avpacket);
    void handleEnoughPacket(AVPacket* avpacket);

};

#endif // PLAYBACKDEMUX_H
