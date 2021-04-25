#include <stdio.h>
#include <iostream>
#include <emscripten/emscripten.h>
#include <emscripten/emscripten.h>
#include <AL/al.h>
#include <AL/alc.h>

#ifdef __cplusplus
extern "C" {
#endif

static unsigned SYSTEM_SAMPLE_RATE = 44100;
static const unsigned BUFFER_SIZE = 200;
static const unsigned BIT_DEPTH = 16;
static void* test = NULL;


unsigned query_sample_rate_of_audiocontexts() {
    return EM_ASM_INT({
        var AudioContext = window.AudioContext || window.webkitAudioContext;
        var ctx = new AudioContext();
        var sr = ctx.sampleRate;
        ctx.close();
        return sr;
    });
}

void audioLoop() {
    
}

int main() {
    SYSTEM_SAMPLE_RATE = query_sample_rate_of_audiocontexts();
    test = alcOpenDevice(NULL);
    unsigned buffersPerSecond = SYSTEM_SAMPLE_RATE / BUFFER_SIZE;
    std::cout << "buffers per second: " << buffersPerSecond << std::endl;
    emscripten_set_main_loop(audioLoop, buffersPerSecond, 1);
    return 0;
}

#ifdef __cplusplus
}
#endif
