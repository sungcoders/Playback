#include "PlaybackPacket.h"

PlaybackPacket::PlaybackPacket()
: queue()
, m_mutex()
, cv()
{
}

void PlaybackPacket::push(AVPacket* pkt)
{
    AVPacket* packet = av_packet_alloc();
    if (!packet) {
        LOGE("Failed to allocate AVPacket");
        return;
    }

    av_packet_move_ref(packet, pkt);
    {
        std::lock_guard<std::mutex> lock(m_mutex);
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
