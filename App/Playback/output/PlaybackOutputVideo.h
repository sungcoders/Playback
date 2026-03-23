#ifndef PLAYBACKOUTPUTVIDEO_H
#define PLAYBACKOUTPUTVIDEO_H

#include "PlaybackOutput.h"

class PlaybackOutputVideo : public PlaybackOutput
{
public:
    PlaybackOutputVideo(std::shared_ptr<PlaybackFrame>& f) : PlaybackOutput(f) {};
    ~PlaybackOutputVideo() = default;

    void Init();
    void Output();

private:
    std::thread outputThread;

};

#endif // PLAYBACKOUTPUTVIDEO_H
