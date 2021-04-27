#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <emscripten/emscripten.h>
#include <emscripten/emscripten.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <fstream> 

using std::chrono::milliseconds;
using std::chrono::system_clock;
#ifdef __cplusplus
extern "C" {
#endif

static const unsigned BUFFER_SIZE = 200;
static const short BIT_DEPTH = 16;
static const ALenum SAMPLE_FORMAT = AL_FORMAT_MONO16 ;
unsigned systemSampleRate = 44100;
ALCdevice *device;
ALCcontext *context;
short bufferData [BUFFER_SIZE]; // array of samples we can write to over and over again
ALuint source;

unsigned query_sample_rate_of_audiocontexts() {
    return EM_ASM_INT({
        var AudioContext = window.AudioContext || window.webkitAudioContext;
        var ctx = new AudioContext();
        var sr = ctx.sampleRate;
        ctx.close();
        return sr;
    });
}

bool print_al_errors(int number)
{
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        switch(error)
        {
        case AL_INVALID_NAME:
            std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
            break;
        case AL_INVALID_ENUM:
            std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
            break;
        case AL_INVALID_VALUE:
            std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case AL_INVALID_OPERATION:
            std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
            break;
        case AL_OUT_OF_MEMORY:
            std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            break;
        default:
            std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << " number: " << number << std::endl;
        return false;
    }
    return true;
}

void fillBuffer() {
    auto currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    srand (currentTime);
    // create random samples for white noise
    for (int index = 0; index < BUFFER_SIZE; index++) {
        short value = (short) rand() % 0xFFFF; // 16 bit number
        bufferData[index] = value;
    }
}

void printBuffer() {
    for(int index = 0; index < BUFFER_SIZE; index++) std::cout << bufferData[index] << std::endl;
}

// this gets called once every (BUFFER_SIZE/systemSampleRate) seconds. It fills a buffer with BUFFER_SIZE samples and sends that buffer to the DAC.
void audioLoop() {
    fillBuffer();
    ALuint buffer;
    alBufferData(buffer, SAMPLE_FORMAT, bufferData, BUFFER_SIZE, systemSampleRate);
    print_al_errors(0);
    alSourcei(source, AL_BUFFER, buffer);
    print_al_errors(1);
    alSourcePlay(source);
    print_al_errors(2);
    alDeleteBuffers(1, &buffer);
}

int main() {
    systemSampleRate = query_sample_rate_of_audiocontexts();
    device = alcOpenDevice(NULL);
    if (!device) {
        // throw some error
        std::cout << "no device, idiot!" << std::endl;
        return 1;
    }
    context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context)) {
        // throw some error
        std::cout << "couldn't use context!" << std::endl;
        return 1;
    }
    alGenSources((ALuint)1, &source);
    emscripten_set_main_loop(audioLoop, systemSampleRate / BUFFER_SIZE, 1);
    return 0;
}

#ifdef __cplusplus
}
#endif
