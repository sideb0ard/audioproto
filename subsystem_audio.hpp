#ifndef SUBSYSTEM_AUDIO_H
#define SUBSYSTEM_AUDIO_H

#include <thread>
#include <array>
#include <portaudio.h>

#include "defjams.hpp"

using std::thread;
using std::array;

class subsystem_audio
{
public:
    subsystem_audio();
    ~subsystem_audio();
    void start_audio_capture();
    void stop_audio_capture();
    void add_client();
    void remove_client();

private:
    thread m_executor;
    array<int, BUFFER_LENGTH_IN_SECONDS * NUM_CHANNELS * SAMPLE_RATE> m_circ_buffer;
private:
    void run();
    void audio_init();
    static int stream_capture_callback(const void *inputBuffer,
                                void *outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData);
};

#endif // SUBSYSTEM_AUDIO_H
