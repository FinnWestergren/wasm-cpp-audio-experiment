#include <stdio.h>
#include <emscripten/emscripten.h>
#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE int myCustomAddMethod(int a, int b) {
    return a + b;
}

#ifdef __cplusplus
}
#endif