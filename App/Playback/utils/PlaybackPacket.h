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
    PlaybackPacket() = default;
    ~PlaybackPacket() = default;

    void push(AVPacket* pkt);
    void pop(AVPacket* pkt);

    int size();

private:
    std::queue<AVPacket> queue;
    std::mutex mtx;
    std::condition_variable cv;

};

#endif // PLAYBACKPACKET_H