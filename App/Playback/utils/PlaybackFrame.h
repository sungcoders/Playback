#ifndef PLAYBACKFRAME_H
#define PLAYBACKFRAME_H

#include <queue>
#include <mutex>
#include <condition_variable>

extern "C" {
#include <libavcodec/avcodec.h>
}

#include "UtilsLog.h"

class PlaybackFrame
{
public:
    PlaybackFrame();
    ~PlaybackFrame() = default;

    void push(AVFrame* pkt);
    void pop(AVFrame* pkt);

    int size();

private:
    std::queue<AVFrame*> queue;
    std::mutex m_mutex;
    std::condition_variable cv;

};

#endif // PLAYBACKFRAME_H