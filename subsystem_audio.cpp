#include <iostream>
#include <cstdio>
#include <thread>
#include <chrono>

#include <portaudio.h>

#include "defjams.hpp"

#include "subsystem_audio.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using std::ios;
using std::thread;
using std::array;

std::chrono::milliseconds sleeptime(1000);

subsystem_audio::subsystem_audio() {
    cout << "Audio subsystem starting up\n";
    m_executor = thread(&subsystem_audio::run, this);
    audio_init();
}
subsystem_audio::~subsystem_audio() {
    m_executor.join();
}

void subsystem_audio::run() {
    for (;;) {
        cout << "Wurking\n";
        std::this_thread::sleep_for(sleeptime);
    }
}

void subsystem_audio::audio_init() {
    cout << "Initializing PortAudio\n";
    PaError err = Pa_Initialize();
    if ( err != paNoError ) {
        cout << "Couldn't initialize audio subsystem - bailing out..\n";
        exit(-1);
    }
}

int subsystem_audio::stream_capture_callback(const void *inputBuffer,
						    void *outputBuffer,
							unsigned long framesPerBuffer,
							const PaStreamCallbackTimeInfo* timeInfo,
							PaStreamCallbackFlags statusFlags,
							void *userData)
{
    paData *data = (paData*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;

    if ( framesLeft < framesPerBuffer ) {
        framesToCalc = framesLeft;
        finished = paComplete;
    } else {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }
    cout << "CB CALLED - " << framesToCalc << " still to be done\n";

    int samp_index = data->frameIndex;
    for ( i = 0; i < framesToCalc; i++ ) {
        data->recordedSamples[samp_index++] = *rptr++;
        if  (NUM_CHANNELS == 2) data->recordedSamples[samp_index++] = *rptr++; 
    }
    data->frameIndex += framesToCalc;
    return finished;
}

void subsystem_audio::start_audio_capture() {

    PaStreamParameters  inputParameters;
    PaStream            *stream;
    PaError             err = paNoError;

    paData data{.frameIndex = 0, 
                .maxFrameIndex = BUFFER_LENGTH_IN_SECONDS * SAMPLE_RATE,
                .recordedSamples = m_circ_buffer};

    cout << "Starting audio capture\n";

    inputParameters.device = 4;
    if (inputParameters.device == paNoDevice) goto error;

    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream,
                        &inputParameters,
                        NULL,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        paClipOff,
                        subsystem_audio::stream_capture_callback,
                        &data);
    if ( err != paNoError) goto error;

    err = Pa_StartStream(stream);
    if ( err != paNoError) goto error;

    while ( (err = Pa_IsStreamActive(stream)) == 1 ) {
        Pa_Sleep(1000);
        cout << "Index: " << data.frameIndex << endl;
    }

    err = Pa_CloseStream(stream);
    if ( err != paNoError) goto error;

    Pa_Terminate();
    cout << "Finished recording test\n";

error:
    Pa_Terminate();

    //for ( const auto v : data.recordedSamples ) {
    //    cout << "DVal " << v << "\n";
    //}
    //for ( const auto v : m_circ_buffer) {
    //    cout << "MVal " << v << "\n";
    //}
    if ( std::FILE* f = std::fopen("audiotest.raw", "wb")) {
        std::fwrite(m_circ_buffer.data(), sizeof m_circ_buffer[0], m_circ_buffer.size(), f);
        std::fclose(f);
    }

    cerr << "Errblah: " << Pa_GetErrorText(err) << endl;
    exit(err);
}
