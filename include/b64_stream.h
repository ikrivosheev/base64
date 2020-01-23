#ifndef _PARSER_H_
#define _PARSER_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


struct b64_decode_state 
{
    int phase;
    size_t out_len;
    char buffer[4];
};

struct b64_encode_state 
{
    int phase;
    size_t out_len;
    char buffer[3];
};

// Decode functions
void b64_stream_decode_init(struct b64_decode_state *state);
int b64_stream_decode(
    struct b64_decode_state *state, 
    const char* src, 
    size_t src_len, 
    char* out
);
int b64_stream_decode_final(struct b64_decode_state *state);

// Encode functions
void b64_stream_encode_init(struct b64_encode_state *state);
int b64_stream_encode(
    struct b64_encode_state *state,
    const char* src,
    size_t src_len,
    char* out
);
int b64_stream_encode_final(struct b64_encode_state *state, char* out);

#ifdef __cplusplus
}
#endif

#endif // _PARSER_H_
