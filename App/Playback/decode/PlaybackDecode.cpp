#include "PlaybackDecode.h"

// PlaybackDecode::PlaybackDecode() {
// }

// PlaybackDecode::~PlaybackDecode() {
// }

void PlaybackDecode::Start()
{
    LOGW("Starting decode thread...");
    decodeThread = std::thread(&PlaybackDecode::Decode, this);
}
