#ifndef PLAYBACKDECODEVIDEO_H
#define PLAYBACKDECODEVIDEO_H

#include "PlaybackDecode.h"
#include "PlaybackWindow.h"
#include "PlaybackOutputVideo.h"

class PlaybackDecodeVideo : public PlaybackDecode
{
public:
    PlaybackDecodeVideo();
    ~PlaybackDecodeVideo() = default;

    void Init(AVCodecContext* codecCtx, AVFormatContext* fmtCtx, std::shared_ptr<PlaybackPacket> packet);
    void Decode() override;

private:
    std::shared_ptr<PlaybackFrame> m_pCFrame;
    std::shared_ptr<PlaybackPacket> m_pCPacket;
    AVFormatContext* fmtCtx;
    AVCodecContext* codecCtx;
    std::unique_ptr<PlaybackOutputVideo> outputVideo;
    std::thread decodeThread;
};

#endif // PLAYBACKDECODEVIDEO_H
