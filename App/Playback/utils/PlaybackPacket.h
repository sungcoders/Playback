#ifndef PLAYBACKPACKET_H
#define PLAYBACKPACKET_H

#include <queue>
#include <mutex>
#include <condition_variable>

extern "C" {
#include <libavcodec/avcodec.h>
}

#include "UtilsLog.h"

class PlaybackPacket
{
public:
    PlaybackPacket();
    ~PlaybackPacket() = default;

    void push(AVPacket* pkt);
    void pop(AVPacket* pkt);

    int size();

private:
    std::queue<AVPacket*> queue;
    std::mutex m_mutex;
    std::condition_variable cv;

};

#endif // PLAYBACKPACKET_H