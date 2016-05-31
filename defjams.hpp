#ifndef DEF_JAMS_H_
#define DEF_JAMS_H_

#include <array>

using std::array;

#define NUM_SECONDS                 5
#define SAMPLE_RATE                 (44100)
#define FRAMES_PER_BUFFER           (512)
#define NUM_CHANNELS                (1)
#define PA_SAMPLE_TYPE              paFloat32
#define BUFFER_LENGTH_IN_SECONDS    2
#define BUFFER_SIZE_IN_SAMPLES      BUFFER_LENGTH_IN_SECONDS * NUM_CHANNELS * SAMPLE_RATE

typedef float SAMPLE;

typedef struct
{
    int     frameIndex;
    int     maxFrameIndex;
    array<SAMPLE, BUFFER_SIZE_IN_SAMPLES>& recordedSamples;
} paData;

#endif  // DEF_JAMZ_H_
