#include <string.h>

#include "include/b64_stream.h"
#include "table.h"


void b64_stream_encode_init(struct b64_encode_state *state)
{
    state->phase = 0;
    state->out_len = 0;
    memset(state->buffer, 0, sizeof(state->buffer));
}

int b64_stream_encode(
    struct b64_encode_state* state,
    const char* src,
    size_t src_len,
    char* out)
{
    size_t i;
    int encoded_len = 0;
    char this_ch;
    
    for(i = 0; i < src_len; ++i) 
    {
       state->buffer[state->phase] = src[i];
       state->phase = (state->phase + 1) % 3;

       if (state->phase == 0) 
       {
            *(out++) = BASIS_64[state->buffer[0] >> 2 & 0x3F];
            *(out++) = BASIS_64[((state->buffer[0] & 0x3) << 4) | ((int) (state->buffer[1] & 0xF0) >> 4)];
            *(out++) = BASIS_64[((state->buffer[1] & 0xF) << 2) | ((int) (state->buffer[2] & 0xC0) >> 6)];
            *(out++) = BASIS_64[state->buffer[2] & 0x3F];
            state->out_len += 4;
            encoded_len += 4;
            memset(state->buffer, 0, sizeof(state->buffer));       
       }
    }
    return encoded_len;
}

int b64_stream_encode_final(struct b64_encode_state *state, char* out)
{
    switch (state->phase)
    {
        case 0:
            return 0;
        case 1:
            *(out++) = BASIS_64[state->buffer[0] >> 2 & 0x3F];
            *(out++) = BASIS_64[(state->buffer[0] & 0x3) << 4];
            *(out++) = BASE64_PAD;
            *(out++) = BASE64_PAD;

            state->phase = 0;
            state->out_len += 4;
            break;
        case 2:
            *(out++) = BASIS_64[state->buffer[0] >> 2 & 0x3F];
            *(out++) = BASIS_64[((state->buffer[0] & 0x3) << 4) | ((int) (state->buffer[1] & 0xF0) >> 4)];
            *(out++) = BASIS_64[(state->buffer[1] & 0xF) << 2];
            *(out++) = BASE64_PAD;

            state->phase = 0;
            state->out_len += 4;
            break;
        default:
            return -1;
    }
    return 4;
}
