#include "PlaybackPacket.h"

void push(AVPacket* pkt) {
    std::unique_lock<std::mutex> lock(mtx);

    AVPacket packet;
    av_packet_move_ref(&packet, pkt);

    queue.push(packet);

    cv.notify_one();
}

bool pop(AVPacket* pkt) {
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [this]() { return !queue.empty(); });

    AVPacket packet = queue.front();
    queue.pop();

    av_packet_move_ref(pkt, &packet);

    return true;
}