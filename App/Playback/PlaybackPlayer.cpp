#include "PlaybackPlayer.h"
#include <iostream>
#include <windows.h>

PlaybackPlayer::PlaybackPlayer() : running(false), keyPressed(0) {}

PlaybackPlayer::~PlaybackPlayer() {
    stop();
}

void PlaybackPlayer::start() {
    running = true;
    inputThread = std::thread(&PlaybackPlayer::inputLoop, this);
    mainThread = std::thread(&PlaybackPlayer::HandlePlayback, this);
    std::cout << "PlaybackPlayer started. Press 'Q' to quit.\n";
}

void PlaybackPlayer::stop() {
    running = false;
    if (inputThread.joinable()) {
        inputThread.join();
    }
    if (mainThread.joinable()) {
        mainThread.join();
    }
}

void PlaybackPlayer::inputLoop() {
    while (running) {
        for (int i = 0; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                keyPressed = (char)i;
                Sleep(100);
            }
        }
    }
}

void PlaybackPlayer::update() {
    if (keyPressed != 0) {
        char k = keyPressed;
        keyPressed = 0;

        std::cout << "Nhan phim: " << k << std::endl;

        if (k == 'q' || k == 'Q') {
            std::cout << "Thoat...\n";
            stop();
        }
    }
}

void PlaybackPlayer::HandlePlayback() {

    const char* filename = "video.mp4";

    // Init FFmpeg
    avformat_network_init();

    AVFormatContext* fmtCtx = nullptr;

    if (avformat_open_input(&fmtCtx, filename, nullptr, nullptr) != 0) {
        std::cout << "Không mở được file\n";
        return;
    }

    avformat_find_stream_info(fmtCtx, nullptr);

    // Tìm video stream
    int videoStream = -1;
    for (unsigned int i = 0; i < fmtCtx->nb_streams; i++) {
        if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }

    if (videoStream == -1) {
        std::cout << "Không tìm thấy video stream\n";
        return;
    }

    AVCodecParameters* codecPar = fmtCtx->streams[videoStream]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecPar->codec_id);

    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, codecPar);

    avcodec_open2(codecCtx, codec, nullptr);

    int width = codecCtx->width;
    int height = codecCtx->height;

    // Init SDL
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "FFmpeg Player",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_IYUV,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();

    SDL_Event event;

    // Main loop
    while (av_read_frame(fmtCtx, packet) >= 0) {

        if (packet->stream_index == videoStream) {

            avcodec_send_packet(codecCtx, packet);

            while (avcodec_receive_frame(codecCtx, frame) == 0) {

                // Update texture từ YUV frame
                SDL_UpdateYUVTexture(
                    texture,
                    NULL,
                    frame->data[0], frame->linesize[0],
                    frame->data[1], frame->linesize[1],
                    frame->data[2], frame->linesize[2]
                );

                // Render
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);

                // Handle event (tránh treo)
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        goto end;
                    }
                }

                SDL_Delay(33); // ~30 FPS (demo)
            }
        }

        av_packet_unref(packet);
    }

end:
    // Cleanup
    av_frame_free(&frame);
    av_packet_free(&packet);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    avcodec_free_context(&codecCtx);
    avformat_close_input(&fmtCtx);
}

void PlaybackPlayer::SetPlayInfo() {
    // Code để thiết lập thông tin phát video
}

void PlaybackPlayer::SetConfig() {
    // Code để thiết lập cấu hình phát video
}

void PlaybackPlayer::PlayStart() {
    // Code để bắt đầu phát video
}

void PlaybackPlayer::Pause() {
    // Code để tạm dừng phát video
}

void PlaybackPlayer::PlayStop() {
    // Code để dừng phát video
}

