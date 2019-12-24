#include <string.h>

#include "b64_stream.h"
#include "table.h"


static inline void _b64_decode_block(struct b64_state *state) {
    state->decoded[0] = state->encoded[0] << 2 | state->encoded[1] >> 4;
    state->decoded[1] = state->encoded[1] << 4 | state->encoded[2] >> 2;
    state->decoded[2] = state->encoded[2] << 6 | state->encoded[3];
}


inline void b64_stream_decode_init(struct b64_state *state) 
{
    state->phase = 0;
    memset(state->encoded, 0, sizeof(state->encoded));
    memset(state->decoded, 0, sizeof(state->decoded));
}


void b64_stream_decode(
    struct b64_state *state, 
    const char* src, 
    size_t src_len, 
    char* out,
    size_t* out_len) 
{
    size_t i;
    *out_len = 0;
    unsigned char this_ch;

    for(i = 0; i < src_len; ++i) {
        this_ch = src[i];
        if ( (this_ch > 0x7f) || 
             (this_ch == '\r') || 
             (this_ch == '\n') || 
             (this_ch == ' ') ) {
            continue;
        }
        if ( this_ch == BASE64_PAD ) {
            _b64_decode_block(state);
            switch (state->phase) {
                case 1:
                    break;
                case 2:
                    *(out++) = state->decoded[0];
                    *out_len += 1;
                    break;
                case 3:
                    *(out++) = state->decoded[0];
                    *(out++) = state->decoded[1];
                    *out_len += 2;
                    break;
            }
            state->phase = 0;
            break;
        }
        state->encoded[state->phase] = (unsigned char) BASE64_TABLE[this_ch];
        state->phase = (state->phase + 1) % 4;
        if (state->phase == 0) {
            _b64_decode_block(state);
            *(out++) = state->decoded[0];
            *(out++) = state->decoded[1];
            *(out++) = state->decoded[2];
            b64_stream_decode_init(state);
            *out_len += 3;
        }
    }
}

inline int b64_stream_decode_final(struct b64_state *state) 
{
    return state->phase == 0 ? 0: -1;
}

