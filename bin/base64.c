#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/b64_stream.h"

#define B64_READ_BUFFER_SIZE 1024
#define B64_ENCODE_BUFFER_SIZE (1024 + 2) * 4 / 3
#define B64_DECODE_BUFFER_SIZE 1024 / 4 * 3


int encode(FILE* file) 
{
    int read_len;
    char read_buffer[B64_READ_BUFFER_SIZE];
    char buffer[B64_ENCODE_BUFFER_SIZE];

    struct b64_encode_state state; 
    b64_stream_encode_init(&state);

    while((read_len = fread(read_buffer, sizeof(char), sizeof(read_buffer), file)) > 0)
    {
        int buffer_len = b64_stream_encode(&state,
                                           read_buffer, 
                                           read_len,
                                           buffer);
        fwrite(buffer, buffer_len, 1, stdout);
        if (feof(file)) 
            break;
    }
    
    int buffer_len = b64_stream_encode_final(&state, buffer);
    fwrite(buffer, buffer_len, 1, stdout);

    return 0;

}

int decode(FILE* file)
{
    int read_len;
    char read_buffer[B64_READ_BUFFER_SIZE];
    char buffer[B64_ENCODE_BUFFER_SIZE];

    struct b64_decode_state state;
    b64_stream_decode_init(&state);
    while ((read_len = fread(read_buffer, sizeof(char), sizeof(read_buffer), file)) > 0)
    {
        int buffer_len = b64_stream_decode(&state, 
                                           read_buffer, 
                                           read_len, 
                                           buffer);
        fwrite(buffer, buffer_len, 1, stdout);
        if (feof(file))
            break;
    }

    return b64_stream_decode_final(&state);
}

int main() 
{
    FILE* file = stdin;

    int result = encode(file);

    fwrite("\n", 1, 1, stdout);
    fflush(stdout);
    fclose(file);
    return result;
}
