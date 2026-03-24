#include "PlaybackDecodeVideo.h"
#include "PlaybackFrame.h"

void PlaybackDecodeVideo::Init(AVCodecContext* codecCtx, AVFormatContext* fmtCtx)
{
    this->codecCtx = codecCtx;
    this->fmtCtx = fmtCtx;
    // decodeThread = std::thread(&PlaybackDecodeVideo::Decode, this);
    LOGW("Video decode thread started");
}

void PlaybackDecodeVideo::Decode()
{
    LOGW("Starting video decode loop...");
    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();

    outputVideo->Init();

    while (true)
    {
        m_pCPacket->pop(packet);
        int ret = avcodec_send_packet(codecCtx, packet);
        if (ret < 0)
        {
            av_packet_unref(packet);
            continue;
        }

        while (ret >= 0)
        {
            ret = avcodec_receive_frame(codecCtx, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF || ret < 0)
            {
                break;
            }

            LOGW("[{}] decode frame: {:.3f} size {}", packet->stream_index, frame->pts * av_q2d(fmtCtx->streams[packet->stream_index]->time_base), m_pCFrame->size());
            m_pCFrame->push(frame);
            frame = av_frame_alloc();
        }
        av_packet_unref(packet);
    }

    av_packet_free(&packet);
    av_frame_free(&frame);
}
