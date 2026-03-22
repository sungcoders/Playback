#include "PlaybackDecodeVideo.h"
#include "PlaybackFrame.h"

void PlaybackDecodeVideo::Init(AVCodecContext* codecCtx, AVFormatContext* fmtCtx)
{
    this->codecCtx = codecCtx;
    this->fmtCtx = fmtCtx;
    decodeThread = std::thread(&PlaybackDecodeVideo::Decode, this);
    UtilsLog::warning("Video decode thread started");
}

void PlaybackDecodeVideo::Decode()
{
    AVPacket* packetV = new AVPacket();
    while (true)
    {
        sleep(5);
        m_pCPacket->pop(packetV);
    //     if (packetV->stream_index == -1) {
    //         UtilsLog::debug("End of video stream");
    //         break;
    //     }
    //     // if (packetV->stream_index == videoStream)
    //     // {
            while (avcodec_send_packet(codecCtx, packetV) == 0)
            {
                sleep(1);
                AVFrame* frame = av_frame_alloc();
                if (avcodec_receive_frame(codecCtx, frame) == 0)
                {
                    UtilsLog::warning("[{}] decode frame: {}", packetV->stream_index, frame->pts * av_q2d(fmtCtx->streams[packetV->stream_index]->time_base));
                    m_pCFrame->push(frame);
                }
                av_frame_free(&frame);
            }
        // }
        av_packet_unref(packetV);
    }
    // av_packet_free(&packetV);
    // av_frame_free(&frame);
    // av_packet_free(&packet); 
    // avcodec_free_context(&codecCtx);
    // avformat_close_input(&fmtCtx);
}