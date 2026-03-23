#ifndef PLAYBACKDECODEVIDEO_H
#define PLAYBACKDECODEVIDEO_H

#include "PlaybackDecode.h"
#include "PlaybackWindow.h"
#include "PlaybackOutputVideo.h"

class PlaybackDecodeVideo : public PlaybackDecode
{
public:
    PlaybackDecodeVideo(std::shared_ptr<PlaybackPacket>& p) : PlaybackDecode(p) {};
    ~PlaybackDecodeVideo() = default;

    void Init(AVCodecContext* codecCtx, AVFormatContext* fmtCtx);
    void Decode();

private:
    std::shared_ptr<PlaybackFrame> m_pCFrame = std::make_shared<PlaybackFrame>();
    AVFormatContext* fmtCtx = nullptr;
    AVCodecContext* codecCtx = nullptr;
    PlaybackWindow win;
    std::thread decodeThread;
    std::unique_ptr<PlaybackOutputVideo> outputVideo = std::make_unique<PlaybackOutputVideo>(m_pCFrame);
};

#endif // PLAYBACKDECODEVIDEO_H
