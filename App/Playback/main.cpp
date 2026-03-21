#include <iostream>
#include "PlaybackPlayer.h"

int main(int argc, char* argv[]) {
    
    PlaybackPlayer player;
    player.start();

     while (true) {
        player.update();
        // Sleep(50);
    }

    return 0;
}
