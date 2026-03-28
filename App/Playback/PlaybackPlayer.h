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
#include "PlaybackWindow.h"

class PlaybackPlayer : public PlaybackMediator
{
public:
    PlaybackPlayer();
    ~PlaybackPlayer();

    void start();   // start thread
    void stop();    // stop thread

    void SetConfig();
    void SetPlayInfo();
    void PlayStart();
    void Pause();
    void PlayStop();

    void inputLoop();
    void outPutView();

    void fwdFrameOut(std::shared_ptr<PlaybackFrame> MediaFrame) override;

private:    
    std::atomic<bool> m_bIsExit;
    std::unique_ptr<PlaybackDemux> m_pCdemux;
    std::shared_ptr<PlaybackFrame> m_pCFrame;
    std::thread inputThread;
    std::thread outPutThread;
    PlaybackWindow win;

};


#endif