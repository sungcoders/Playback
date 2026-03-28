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
    PlaybackDemux();
    ~PlaybackDemux();

    void Start();
    void Init(const std::string& filename);
    void Demux();

private:
    std::atomic<bool> m_bExit;
    AVFormatContext* m_fmtCtx;
    int m_idxvideoStream;
    int m_idxaudioStream;
    double m_dVideoTimeBase;
    double m_dAudioTimeBase;
    AVCodecContext* m_codecCtx;
    AVCodecParameters* m_codecPar;
    std::shared_ptr<PlaybackPacket> m_pCpacketVideo;
    std::shared_ptr<PlaybackPacket> m_pCpacketAudio;
    std::unique_ptr<PlaybackDecodeVideo> m_pCdecode;
    std::thread demuxThread;

    void pushPacketAV(AVPacket* avpacket);
};

#endif // PLAYBACKDEMUX_H
