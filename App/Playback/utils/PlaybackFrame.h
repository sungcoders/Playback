#ifndef PLAYBACKFRAME_H
#define PLAYBACKFRAME_H

#include <queue>
#include <mutex>
#include <condition_variable>

extern "C" {
#include <libavcodec/avcodec.h>
}

class PlaybackFrame
{
public:
    PlaybackFrame();
    ~PlaybackFrame();

    void push(AVFrame* pkt);
    bool pop(AVFrame* pkt);

private:
    std::queue<AVFrame> queue;
    std::mutex mtx;
    std::condition_variable cv;

};

#endif // PLAYBACKFRAME_H