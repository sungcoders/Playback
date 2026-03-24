#ifndef PLAYBACKDECODE_H
#define PLAYBACKDECODE_H

#include "PlaybackFrame.h"
#include "PlaybackPacket.h"
#include <unistd.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

class PlaybackDecode
{
public:
    PlaybackDecode(std::shared_ptr<PlaybackPacket>& packet) : m_pCPacket(packet) {};
    ~PlaybackDecode() = default;

    void Start();
    virtual void Decode() = 0;

protected:
    std::shared_ptr<PlaybackPacket> m_pCPacket = std::make_shared<PlaybackPacket>();
    std::thread decodeThread;
};

#endif // PLAYBACKDECODE_H
