#include "PlaybackWindow.h"

PlaybackWindow::PlaybackWindow()
: window(nullptr)
, renderer(nullptr)
, texture(nullptr)
// , isPaused(false)
// , winWidth(0)
// , winHeight(0)
{
}

PlaybackWindow::~PlaybackWindow()
{
    LOGI("Destroying PlaybackWindow...\n");
    destroyWindow();
}

void PlaybackWindow::createWindow(int width, int height)
{
    LOGI("Creating window with resolution: {}x{}", width, height);
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "Playback (SungLV author)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_IYUV,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

    // winWidth = width;
    // winHeight = height;

    // // pauseBtn = {x, y, w, h};
    // pauseBtn = {20, height - 70, 120, 50};
}

void PlaybackWindow::resizeWindow(int width, int height)
{
    if (window)
    {
        SDL_SetWindowSize(window, width, height);
    }
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

    // //  render button overlay
    // if (isPaused) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // {render, r, g, b, a} đỏ
    // else          SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); // {render, r, g, b, a} xanh

    // SDL_RenderFillRect(renderer, &pauseBtn);
    SDL_RenderPresent(renderer);
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

void PlaybackWindow::WindowEvent(SDL_Event& eventType)
{
    while (SDL_PollEvent(&event))
    {
        LOGI("-------------------- Received event: {} --------------------", event.type);
        eventType = event;
        // if(event.type == SDL_MOUSEBUTTONDOWN)
        // {
        //     int x = event.button.x;
        //     int y = event.button.y;

        //     if (x >= pauseBtn.x && x <= pauseBtn.x + pauseBtn.w &&
        //         y >= pauseBtn.y && y <= pauseBtn.y + pauseBtn.h)
        //     {
        //         isPaused = !isPaused;
        //         LOGI("Pause toggled: {}", isPaused);
        //     }
        // }
        // if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        // {
        //     winWidth = event.window.data1;
        //     winHeight = event.window.data2;

        //     pauseBtn.y = winHeight - 70;
        //     LOGI("Window resized: {}x{}", winWidth, winHeight);
        // }
    }
}
