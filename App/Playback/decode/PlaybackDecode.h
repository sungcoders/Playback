#ifndef PLAYBACKDECODE_H
#define PLAYBACKDECODE_H

#include "PlaybackFrame.h"
#include "PlaybackPacket.h"
#include <unistd.h>

class PlaybackDecode
{
public:
    PlaybackDecode(const std::shared_ptr<PlaybackPacket>& p) : m_pCPacket(p) {}
    ~PlaybackDecode() = default;

protected:
    /* data */
    std::shared_ptr<PlaybackPacket> m_pCPacket;
    std::shared_ptr<PlaybackFrame> m_pCFrame = std::make_shared<PlaybackFrame>();
};

#endif // PLAYBACKDECODE_H
