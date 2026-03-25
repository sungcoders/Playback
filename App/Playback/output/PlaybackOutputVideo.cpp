#include "PlaybackOutputVideo.h"

void PlaybackOutputVideo::Init()
{
    win.createWindow(1920, 1044);
    LOGE("Video output thread initialized");
}

void PlaybackOutputVideo::Start()
{
    outputThread = std::thread(&PlaybackOutputVideo::Output, this);
    LOGE("Video output thread started");
}

void PlaybackOutputVideo::Output()
{
    while (true)
    {
        win.WindowEvent();
        FrameInfo sFrame = {};
        m_pCFrame->pop(sFrame);

        LOGE("Display frame video: {:.3f}", sFrame.timestamp);
        win.renderFrame(sFrame.frame);
        win.delay(33);

        av_frame_free(&sFrame.frame);
        // LOGE("Frame displayed, waiting for next frame...");
    }
    LOGE("Video output thread finished");
}
