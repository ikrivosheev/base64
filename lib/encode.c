#include "b64_stream.h"
#include "table.h"

void b64_stream_encode_init(struct b64_state *state)
{
    state->phase = 0;
    memset(state->encoded, 0, sizeof(state->encoded));
    memset(state->decoded, 0, sizeof(state->decoded))
}

void b64_stream_encode(
    struct b64_state* state,
    const char* src,
    size_t src_len,
    char* out,
    size_t *out_len)
{

}

void b64_stream_encode_final(
    struct b64_state *state,
    char* out,
    size_t* out_len)
{

}
