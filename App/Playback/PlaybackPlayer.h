#ifndef PLAYBACKPLAYER_H
#define PLAYBACKPLAYER_H

#include <thread>
#include <atomic>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
}

#include "PlaybackDemux.h"
#include "PlaybackClock.h"
#include "PlaybackDecodeVideo.h"
#include "PlaybackOutputVideo.h"
#include "PlaybackWindow.h"

class PlaybackPlayer
{
public:
    PlaybackPlayer();
    ~PlaybackPlayer();

    void start();
    void SetConfig();
    void SetPlayInfo();
    void PlayStart(std::string filename);
    void Seek(int64_t position);
    void Pause();
    void Resume();
    void PlayStop();

    void outPutView();
    void handleEvent();

private:
    enum class PlaybackState
    {
        INIT_E,
        PLAY_E,
        PAUSE_E,
        STOP_E,
        COMPLETED_E
    };

    std::atomic<bool> m_bIsExit;
    std::unique_ptr<PlaybackDemux> m_pCdemux;
    std::shared_ptr<PlaybackDecodeVideo> m_pCdecode;
    std::shared_ptr<PlaybackFrame> m_pCFrame;
    std::shared_ptr<PlaybackClock> m_pCClock;
    std::atomic<PlaybackState> m_ePlaybackState;
    std::thread inputThread;
    std::thread outPutThread;
    PlaybackWindow win;

};


#endif