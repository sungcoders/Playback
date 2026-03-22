#include "PlaybackFrame.h"

void push(AVFrame* pkt)
{
    std::unique_lock<std::mutex> lock(mtx);

    AVFrame frame;
    av_frame_move_ref(&frame, pkt);

    queue.push(frame);

    cv.notify_one();
}

bool pop(AVFrame* pkt)
{
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [this]() { return !queue.empty(); });

    AVFrame frame = queue.front();
    queue.pop();

    av_frame_move_ref(pkt, &frame);

    return true;
}