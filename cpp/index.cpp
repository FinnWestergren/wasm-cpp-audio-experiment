#include <stdio.h>
#include <emscripten/emscripten.h>
#ifdef __cplusplus
extern "C" {
#endif

static int c = 0;

EMSCRIPTEN_KEEPALIVE
void setC(int a, int b) {
}

EMSCRIPTEN_KEEPALIVE 
int getC() {
    return c;
}

// unsigned query_sample_rate_of_audio {
//     return EM_ASM_INT({
//         var AudioContext = window.AudioContext || window.webkitAudioContext;
//         var ctx = new AudioContext();
//         var sr = ctx.sampleRate;
//         ctx.close();
//         return sr;
//     });
// }

void run() {
    c += 1;
}

int main(){
    emscripten_set_main_loop(run, 1, 1);
}

#ifdef __cplusplus
}
#endif
