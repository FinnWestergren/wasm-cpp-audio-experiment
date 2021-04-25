#include <stdio.h>
#include <emscripten/emscripten.h>
#ifdef __cplusplus
extern "C" {
#endif

static unsigned SYSTEM_SAMPLE_RATE = 44100;
static const unsigned BUFFER_SIZE = 200;
static const unsigned BIT_DEPTH = 16;

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
    unsigned samplesPerBuffer = SYSTEM_SAMPLE_RATE / BUFFER_SIZE;
    emscripten_set_main_loop(audioLoop, samplesPerBuffer, 1);
}

#ifdef __cplusplus
}
#endif
