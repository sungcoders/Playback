#include "PlaybackWindow.h"
#include <iostream>

PlaybackWindow::PlaybackWindow()
{
}

PlaybackWindow::~PlaybackWindow()
{
    std::cout << "Destroying PlaybackWindow...\n";
    destroyWindow();
}

void PlaybackWindow::createWindow(int width, int height)
{
    // Code để tạo cửa sổ phát video
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "FFmpeg Player",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        0
    );

    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_IYUV,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );
}

void PlaybackWindow::renderFrame(AVFrame* frame)
{
    SDL_UpdateYUVTexture(
        texture,
        NULL,
        frame->data[0], frame->linesize[0],
        frame->data[1], frame->linesize[1],
        frame->data[2], frame->linesize[2]
    );
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    WindowEvent();
}

void PlaybackWindow::delay(int ms)
{
    SDL_Delay(ms);
}


void PlaybackWindow::destroyWindow()
{
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void PlaybackWindow::WindowEvent()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            std::cout << "Quit event received, exiting...\n";
            destroyWindow();
            return;
        }
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_q)
            {
                std::cout << "Q key pressed, exiting...\n";
                return;
            }
        }
    }
}