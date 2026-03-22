#ifndef PLAYBACKDECODEVIDEO_H
#define PLAYBACKDECODEVIDEO_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include "PlaybackDecode.h"
#include "PlaybackWindow.h"

class PlaybackDecodeVideo : public PlaybackDecode
{
public:
    PlaybackDecodeVideo(const std::shared_ptr<PlaybackPacket>& p) : PlaybackDecode(p) {};
    ~PlaybackDecodeVideo() = default;

    void Init(AVCodecContext* codecCtx, AVFormatContext* fmtCtx);
    void Decode();

private:
    std::shared_ptr<PlaybackFrame> m_pCFrame = std::make_shared<PlaybackFrame>();
    AVFormatContext* fmtCtx = nullptr;
    AVCodecContext* codecCtx = nullptr;
    PlaybackWindow win;
    std::thread decodeThread;
};

#endif // PLAYBACKDECODEVIDEO_H
