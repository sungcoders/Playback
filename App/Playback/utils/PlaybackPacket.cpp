#include "PlaybackPacket.h"

PlaybackPacket::PlaybackPacket()
: queue()
, m_mutex()
, cv()
{
}

void PlaybackPacket::push(AVPacket* pkt)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    AVPacket* packet = av_packet_alloc();
    
    if(queue.size() > 10)
    {
        usleep(150000);
    }
    
    if (!packet) {
        LOGE("Failed to allocate AVPacket");
        return;
    }

    av_packet_move_ref(packet, pkt);
    {
        queue.push(packet);
    }

    cv.notify_one();
}

void PlaybackPacket::pop(AVPacket* pkt)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    cv.wait(lock, [this]()
    {
        // LOGD("Waiting for packet, current queue size: {}", queue.size());
        return !queue.empty();
    });

    AVPacket* packet = queue.front();
    queue.pop();

    av_packet_move_ref(pkt, packet);
    av_packet_free(&packet);
}

int PlaybackPacket::size()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return queue.size();
}

bool PlaybackPacket::waitPacket()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    cv.wait(lock, [this]() {
        return queue.size() < 10;
    });
    return true;
}

