#include "WindowAudio.h"

constexpr int BYTE_PER_SECOND = 44100 * 4; // 1 second of audio at 44100 Hz, 16-bit stereo

WindowAudio::WindowAudio()
: m_swrCtx(nullptr)
, m_codecCtx(nullptr)
, m_deviceID(-1)
{
}

WindowAudio::~WindowAudio()
{
    destroyAudio();
}

void WindowAudio::getDeviceIDAudio()
{
    SDL_AudioSpec spec = {};
    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.samples = 1024;
    spec.callback = nullptr;

    m_deviceID = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
    if (m_deviceID == 0)
    {
        LOGE("SDL_OpenAudioDevice failed: {}", SDL_GetError());
        return;
    }
    SDL_PauseAudioDevice(m_deviceID, 0);
}

void WindowAudio::audioInit(AVCodecContext* codecCtx)
{
    SDL_Init(SDL_INIT_AUDIO);
    m_codecCtx = codecCtx;
    getDeviceIDAudio();
    LOGE("Audio device initialized with ID: {}", m_deviceID);
}

void WindowAudio::resampleAudio()
{
    // LOGE("Initializing audio resampling...");
    // Tạo layout output (stereo)
    AVChannelLayout out_layout;
    av_channel_layout_default(&out_layout, 2);
    // LOGE("Output channel layout: {} channels", out_layout.nb_channels);
    if(m_codecCtx == nullptr)
    {
        LOGE("Codec context is null, cannot initialize resampling");
        return;
    }
    // Input layout từ codec
    const AVChannelLayout* in_layout = &m_codecCtx->ch_layout;

    // phải truyền pointer của m_swrCtx
    int ret = swr_alloc_set_opts2(
        &m_swrCtx,              // SwrContext**
        &out_layout,            // output layout
        AV_SAMPLE_FMT_S16,
        44100,
        in_layout,              // input layout
        m_codecCtx->sample_fmt,
        m_codecCtx->sample_rate,
        0,
        nullptr
    );

    // LOGE("swr_alloc_set_opts2 returned: {}", ret);
    if (ret < 0 || !m_swrCtx)
    {
        LOGE("swr_alloc_set_opts2 failed\n");
        return;
    }

    if (swr_init(m_swrCtx) < 0)
    {
        LOGE("swr_init failed\n");
        return;
    }

    // free layout nếu cần
    av_channel_layout_uninit(&out_layout);
    // LOGE("Audio resampling initialized successfully");
}

void WindowAudio::sendFrameOutput(AVFrame* frame)
{
    resampleAudio();
    int out_channels = 2;
    int out_sample_rate = 44100;
    AVSampleFormat out_fmt = AV_SAMPLE_FMT_S16;

    int out_nb_samples = av_rescale_rnd(
        swr_get_delay(m_swrCtx, frame->sample_rate) + frame->nb_samples,
        out_sample_rate,
        frame->sample_rate,
        AV_ROUND_UP);

    uint8_t* out_buffer = nullptr;
    int out_linesize = 0;

    av_samples_alloc(&out_buffer,
                        &out_linesize,
                        out_channels,
                        out_nb_samples,
                        out_fmt,
                        0);

    int samples = swr_convert(
        m_swrCtx,
        &out_buffer,
        out_nb_samples,
        (const uint8_t**)frame->data,
        frame->nb_samples);

    int data_size = av_samples_get_buffer_size(
        &out_linesize,
        out_channels,
        samples,
        out_fmt,
        1);

    //  sample rate = 44100 samples / giây
    // format = S16 (16-bit = 2 bytes)
    // channels = 2 (stereo)
    // => 1 sample frame = 2 bytes * 2 channels = 4 bytes
    // 1s = 44100 sample frames * 4 bytes = 44100 * 4 bytes
    while (SDL_GetQueuedAudioSize(m_deviceID) > BYTE_PER_SECOND)
    {
        SDL_Delay(10);
    }

    //write audio data to the device
    // LOGE("Queueing {} bytes of audio data", data_size);
    SDL_QueueAudio(m_deviceID, out_buffer, data_size);

    av_free(out_buffer);
    // av_frame_free(&frame);
}

void WindowAudio::destroyAudio()
{
    if (m_deviceID != -1)
    {
        SDL_CloseAudioDevice(m_deviceID);
        m_deviceID = -1;
    }

    if (m_swrCtx)
    {
        swr_free(&m_swrCtx);
    }

    SDL_Quit();
}
