#include "PlaybackPlayer.h"
#include <iostream>
#include <windows.h>

PlaybackPlayer::PlaybackPlayer()
: m_bIsExit(false)
, m_pCdemux(nullptr)
, m_pCdecode(nullptr)
, m_pCFrame(nullptr)
{
}

PlaybackPlayer::~PlaybackPlayer()
{
    outPutThread.join();
}

void PlaybackPlayer::start()
{
    m_pCFrame = std::make_shared<PlaybackFrame>();
    m_pCdecode = std::make_shared<PlaybackDecodeVideo>(m_pCFrame);
    m_pCdemux = std::make_unique<PlaybackDemux>(m_pCdecode);

    if(m_pCdemux == nullptr || m_pCdecode == nullptr || m_pCFrame == nullptr)
    {
        LOGE("Failed to start MediaPlayer");
        return;
    }
    PlayStart("D:/SLV/WorkSpace/App/Playback/Media/video.mp4");
}

void PlaybackPlayer::outPutView()
{
    win.createWindow(1920, 1044);
    while(!m_bIsExit.load())
    {
        handleEvent();
        FrameInfo sFrame = {};
        m_pCFrame->pop(sFrame);
        LOGE("Display frame video: {:.3f}s", sFrame.timestamp);
        win.renderFrame(sFrame.frame);
        win.delay(30);

        av_frame_free(&sFrame.frame);
    }
    LOGE("Outputing process finished");
}

void PlaybackPlayer::handleEvent()
{
    SDL_Event event;
    win.WindowEvent(event);
    switch (event.type)
    {
    case SDL_QUIT:
        LOGI("Quit event received, exiting...");
        PlayStop();
        return;
    case SDL_KEYDOWN:
        LOGI("Key down event: {}", event.key.keysym.sym);
    }
}

void PlaybackPlayer::SetPlayInfo()
{
    // Code để thiết lập thông tin phát video
}

void PlaybackPlayer::SetConfig()
{
    // Code để thiết lập cấu hình phát video
}

void PlaybackPlayer::PlayStart(std::string filename)
{
    m_pCdemux->Init(filename);
    m_pCdemux->Start();
    outPutThread = std::thread(&PlaybackPlayer::outPutView, this);
    LOGD("PlaybackPlayer started");
}

void PlaybackPlayer::Pause()
{
    // Code để tạm dừng phát video
}

void PlaybackPlayer::PlayStop()
{
    m_pCdemux->Stop();
    win.destroyWindow();
    m_bIsExit.store(true);
}

