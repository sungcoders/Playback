#ifndef PLAYBACKOUTPUT_H
#define PLAYBACKOUTPUT_H

#include "PlaybackFrame.h"
#include "PlaybackPacket.h"
#include <unistd.h>
#include "PlaybackWindow.h"

class PlaybackOutput
{
public:
    PlaybackOutput(std::shared_ptr<PlaybackFrame>& f) : m_pCFrame(f) {};
    ~PlaybackOutput() = default;

protected:
    PlaybackWindow win;
    std::shared_ptr<PlaybackFrame> m_pCFrame = std::make_shared<PlaybackFrame>();

};

#endif // PLAYBACKOUTPUT_H
