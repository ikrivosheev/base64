[![Build Status](https://travis-ci.com/ikrivosheev/base64.svg?branch=master)](https://travis-ci.com/ikrivosheev/base64)

# Base64 stream encoder/decoder

## API

### Encoding

#### b64_stream_encode_init

```c
struct b64_encode_state 
{
    int phase;
    size_t out_len;
    char buffer[3];
};

void b64_stream_encode_init(struct b64_encode_state *state);
```

Call this function for init state and befor calling `b64_stream_decode()`


#### b64_stream_encode

```c
int b64_stream_encode(
    struct b64_encode_state *state,
    const char* str,
    size_t src_len,
    char* out
);
```

#### b64_stream_encode_final

```c
int b64_stream_encode_final(struct b64_encode_state *state, char* out);
```

### Decoding

#### b64_stream_decode_init

```c
struct b64_decode_state 
{
    int phase;
    size_t out_len;
    char buffer[4];
};

void b64_stream_decode_init(struct b64_decode_state *state);
```

#### b64_stream_decode

```c
int b64_stream_decode(
    struct b64_decode_state *state, 
    const char* src, 
    size_t src_len, 
    char* out
);
```

#### b64_stream_decode_final

```c
int b64_stream_decode_final(struct b64_decode_state *state);
```
