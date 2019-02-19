#include <string.h>
#include "parser.h"


void b64decode_block(struct base64_state *state) {
    state->decoded[0] = state->encoded[0] << 2 | state->encoded[1] >> 4;
    state->decoded[1] = state->encoded[1] << 4 | state->encoded[2] >> 2;
    state->decoded[2] = state->encoded[2] << 6 | state->encoded[3];
}


inline void b64decode_stream_init(struct base64_state *state) {
    state->phase = 0;
    memset(state->encoded, 0, sizeof(state->encoded));
    memset(state->decoded, 0, sizeof(state->decoded));
}


void b64decode_stream(struct base64_state *state, const char* src, char* out) {
    unsigned char this_ch;

    for( ; *src > 0; src++) {
        this_ch = *src;
        if ( (this_ch > 0x7f) || (this_ch == '\r') || (this_ch == '\n') || (this_ch == ' ') ) {
            continue;
        }
        if ( this_ch == BASE64_PAD ) {
            b64decode_block(state);
            switch (state->phase) {
                case 1:
                    *(out++) = state->decoded[0];
                    break;
                case 2:
                    *(out++) = state->decoded[0];
                    *(out++) = state->decoded[1];
                    break;
                case 3:
                    *(out++) = state->decoded[0];
                    *(out++) = state->decoded[1];
                    *(out++) = state->decoded[3];
                    break;
            }
            state->phase = 0;
            break;
        }
        state->encoded[state->phase] = (unsigned char) BASE64_TABLE[this_ch];
        state->phase = (state->phase + 1) % 4;
        if (state->phase == 0) {
            b64decode_block(state);
            *(out++) = state->decoded[0];
            *(out++) = state->decoded[1];
            *(out++) = state->decoded[2];
            b64decode_stream_init(state);
        }
    }
    *(out++) = '\0';
}

int b64decode_stream_final(struct base64_state *state) {
    return state->phase > 0 ? 0: 1;
}

int main() {
    return 0;
}