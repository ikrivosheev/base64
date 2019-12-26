#ifndef _PARSER_H_
#define _PARSER_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct b64_state {
    int phase;
    unsigned char encoded[4];
    unsigned char decoded[4];
};

// Decode functions
void b64_stream_decode_init(struct b64_state *state);
void b64_stream_decode(
    struct b64_state *state, 
    const char* src, 
    size_t src_len, 
    char* out,
    size_t* out_len
);
int b64_stream_decode_final(struct b64_state *state);

// Encode functions
void b64_stream_encode_init(struct b64_state *state);
void b64_stream_encode(
    struct b64_state *state,
    const char* str,
    size_t src_len,
    char* out,
    size_t* out_len
);
void b64_stream_encode_final(
    struct b64_state *state,
    char* out,
    size_t* out_len
);

#ifdef __cplusplus
}
#endif

#endif // _PARSER_H_
