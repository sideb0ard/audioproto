#include <iostream>
#include "subsystem_audio.hpp"

using std::cout;

int main()
{
    cout << "Yarly, start me up, yo!\n";
    subsystem_audio audio;
    audio.start_audio_capture();

}
