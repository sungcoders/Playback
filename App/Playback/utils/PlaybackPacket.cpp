#include "PlaybackPacket.h"

void PlaybackPacket::push(AVPacket* pkt)
{
    std::unique_lock<std::mutex> lock(mtx);

    AVPacket packet;
    av_packet_move_ref(&packet, pkt);

    queue.push(packet);
    UtilsLog::debug("[{}] push packet: {} size {}", packet.stream_index, packet.pts, queue.size());
    cv.notify_one();
}

void PlaybackPacket::pop(AVPacket* pkt)
{
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [this]()
    {
        UtilsLog::debug("Waiting for packet, current queue size: {}", queue.size());
        return !queue.empty(); 
    });

    AVPacket packet = queue.front();
    queue.pop();

    av_packet_move_ref(pkt, &packet);

    return;
}

int PlaybackPacket::size()
{
    std::unique_lock<std::mutex> lock(mtx);
    return queue.size();
}
