#ifndef PLAYBACKDEMUX_H
#define PLAYBACKDEMUX_H

#include "PlaybackPacket.h"

class PlaybackDemux
{
public:
    PlaybackDemux();
    ~PlaybackDemux();

    void Init();
    void Demux();

private:
    /* data */

};

#endif // PLAYBACKDEMUX_H
