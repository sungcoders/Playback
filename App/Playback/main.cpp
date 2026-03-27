#include <iostream>
#include "PlaybackPlayer.h"
#include "PlaybackOutputVideo.h"

// void OutputPlayback();

PlaybackPlayer player;
// PlaybackOutputVideo videoOut;

int main(int argc, char* argv[]) {
    
    player.start();
    // videoOut.Init();
    // OutputPlayback();
    while(1);
    return 0;
}

// void OutputPlayback()
// {
//     while (true)
//     {
//         // videoOut.Output();
//     }
// }
