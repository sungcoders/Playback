#include "PlaybackFrame.h"

PlaybackFrame::PlaybackFrame()
: queue()
, m_mutex()
, cv()
{
}

void PlaybackFrame::push(FrameInfo& sFrame)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    FrameInfo sFrameInfo{};
    sFrameInfo.frame = av_frame_alloc();
    
    sFrameInfo.timestamp = sFrame.timestamp;
    sFrameInfo.duration  = sFrame.duration;
    av_frame_move_ref(sFrameInfo.frame, sFrame.frame);

    LOGW("Push frame: {} size {}", sFrameInfo.frame->pts, queue.size());
    queue.push(sFrameInfo);

    av_frame_free(&sFrame.frame);

    cv.notify_one();
}

void PlaybackFrame::pop(FrameInfo& sFrame)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    cv.wait(lock, [this]() {
        return !queue.empty();
    });

    FrameInfo sFrameInfo = queue.front();
    queue.pop();

    sFrame.frame = sFrameInfo.frame;
    sFrame.timestamp = sFrameInfo.timestamp;
    sFrame.duration  = sFrameInfo.duration;

    sFrameInfo.frame = nullptr;
}


int PlaybackFrame::size()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return queue.size();
}
