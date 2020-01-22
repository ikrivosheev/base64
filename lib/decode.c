#include <string.h>

#include "include/b64_stream.h"
#include "table.h"


inline void b64_stream_decode_init(struct b64_decode_state *state) 
{
    state->phase = 0;
    state->out_len = 0;
    memset(state->buffer, 0, sizeof(state->buffer));
}


int b64_stream_decode(
    struct b64_decode_state *state, 
    const char* src, 
    size_t src_len, 
    char* out) 
{
    int decoded_len = 0;
    size_t i;
    char this_ch;

    for(i = 0; i < src_len; ++i) 
    {
        this_ch = src[i];
        if ( (this_ch > 0x7f) || 
             (this_ch == '\r') || 
             (this_ch == '\n') || 
             (this_ch == ' ') ) 
        {
            continue;
        }
        if ( this_ch == BASE64_PAD ) 
        {
            switch (state->phase) 
            {
                case 1:
                    break;
                case 2:
                    *(out++) = state->buffer[0] << 2 | state->buffer[1] >> 4;
                    state->out_len += 1;
                    decoded_len++;
                    break;
                case 3:
                    *(out++) = state->buffer[0] << 2 | state->buffer[1] >> 4;
                    *(out++) = state->buffer[1] << 4 | state->buffer[2] >> 2;
                    state->out_len += 2;
                    decoded_len += 2;
                    break;
            }
            state->phase = 0;
            break;
        }
        state->buffer[state->phase] = (unsigned char) BASE64_TABLE[this_ch];
        state->phase = (state->phase + 1) % 4;
        if (state->phase == 0) 
        {
            *(out++) = state->buffer[0] << 2 | state->buffer[1] >> 4;
            *(out++) = state->buffer[1] << 4 | state->buffer[2] >> 2;
            *(out++) = state->buffer[2] << 6 | state->buffer[3];
            state->out_len += 3;
            decoded_len += 3;
            memset(state->buffer, 0, sizeof(state->buffer));
        }
    }
    return decoded_len;
}

inline int b64_stream_decode_final(struct b64_decode_state *state) 
{
    return state->phase == 0;
}

