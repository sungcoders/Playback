#include "PlaybackOutputVideo.h"

void PlaybackOutputVideo::Init()
{
    win.createWindow(1920, 1044);
    outputThread = std::thread(&PlaybackOutputVideo::Output, this);
    UtilsLog::error("Video output thread started");
}

void PlaybackOutputVideo::Output()
{
    AVFrame* frameV = av_frame_alloc();
    while (true)
    {
        m_pCFrame->pop(frameV);
        UtilsLog::error("Display frame: {}", frameV->pts);
        win.renderFrame(frameV);
        win.delay(33);
        av_frame_unref(frameV);
        UtilsLog::error("Frame displayed, waiting for next frame...");
    }
    av_frame_free(&frameV);
    UtilsLog::error("Video output thread finished");
}
