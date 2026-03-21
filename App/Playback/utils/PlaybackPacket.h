#ifndef PLAYBACKPACKET_H
#define PLAYBACKPACKET_H

#include <queue>
#include <mutex>
#include <condition_variable>

extern "C" {
#include <libavcodec/avcodec.h>
}

class PlaybackPacket
{
public:
    PlaybackPacket();
    ~PlaybackPacket();

    void push(AVPacket* pkt);
    bool pop(AVPacket* pkt);

private:
    std::queue<AVPacket> queue;
    std::mutex mtx;
    std::condition_variable cv;

};

#endif // PLAYBACKPACKET_H