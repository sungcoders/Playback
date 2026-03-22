#include "PlaybackFrame.h"

void PlaybackFrame::push(AVFrame* pkt)
{
    std::unique_lock<std::mutex> lock(mtx);

    AVFrame frame;
    av_frame_move_ref(&frame, pkt);

    queue.push(frame);
    UtilsLog::warning("Push frame: {} size {}", frame.pts, queue.size());
    cv.notify_one();
}

void PlaybackFrame::pop(AVFrame* pkt)
{
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [this]()
    {
        UtilsLog::warning("Waiting for frame, current queue size: {}", queue.size());
        return !queue.empty();
    });

    AVFrame frame = queue.front();
    queue.pop();

    av_frame_move_ref(pkt, &frame);

    return;
}


int PlaybackFrame::size()
{
    std::unique_lock<std::mutex> lock(mtx);
    return queue.size();
}
