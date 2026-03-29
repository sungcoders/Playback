#include "PlaybackPlayer.h"
#include <iostream>
#include <windows.h>

PlaybackPlayer::PlaybackPlayer()
: m_bIsExit(false)
, m_pCdemux(nullptr)
, m_pCdecode(nullptr)
, m_pCFrame(nullptr)
, m_pCClock(nullptr)
, m_ePlaybackState(PlaybackState::INIT_E)
{
}

PlaybackPlayer::~PlaybackPlayer()
{
    outPutThread.join();
}

void PlaybackPlayer::start()
{
    m_pCFrame = std::make_shared<PlaybackFrame>();
    m_pCClock = std::make_shared<PlaybackClock>();
    m_pCdecode = std::make_shared<PlaybackDecodeVideo>(m_pCFrame, m_pCClock);
    m_pCdemux = std::make_unique<PlaybackDemux>(m_pCdecode, m_pCClock);

    if(m_pCdemux == nullptr || m_pCdecode == nullptr || m_pCFrame == nullptr)
    {
        LOGE("Failed to start MediaPlayer");
        return;
    }
    PlayStart("D:/SLV/WorkSpace/App/Playback/Media/mp4/video.mp4");
}

void PlaybackPlayer::outPutView()
{
    win.createWindow(1920, 1044);
    while(!m_bIsExit.load())
    {
        handleEvent();
        if(m_pCClock->isPaused())
        {
            win.renderFrame(m_pCFrame->getLastFrame()->frame);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        FrameInfo sFrame = {};
        m_pCFrame->pop(sFrame);
        // LOGE("Display frame video: {:.3f}s", sFrame.timestamp);
        win.renderFrame(sFrame.frame);
        win.delay(33);
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
        return;
    case SDL_MOUSEBUTTONDOWN:
        if(win.isPaused()) { Pause();  }
        else               { Resume(); }
        return;
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
    m_ePlaybackState.store(PlaybackState::PLAY_E);
    m_pCdemux->Init(filename);
    m_pCdemux->Start();
    outPutThread = std::thread(&PlaybackPlayer::outPutView, this);
    LOGD("PlaybackPlayer started");
}

void PlaybackPlayer::Pause()
{
    m_ePlaybackState.store(PlaybackState::PAUSE_E);
    m_pCClock->setPause();
    LOGD("PlaybackPlayer paused");
}

void PlaybackPlayer::Resume()
{
    m_ePlaybackState.store(PlaybackState::PLAY_E);
    m_pCClock->setPlay();
    LOGD("PlaybackPlayer resumed");
}

void PlaybackPlayer::PlayStop()
{
    m_ePlaybackState.store(PlaybackState::STOP_E);
    m_pCdemux->Stop();
    win.destroyWindow();
    m_bIsExit.store(true);
}

