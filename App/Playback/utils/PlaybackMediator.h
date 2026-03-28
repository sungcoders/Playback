#include <iostream>
#include <string>
#include "PlaybackFrame.h"

class PlaybackMediator {
public:
    virtual void fwdFrameOut(std::shared_ptr<PlaybackFrame> MediaFrame) = 0;
    virtual ~PlaybackMediator() = default;
};
