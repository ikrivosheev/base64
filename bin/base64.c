#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/b64_stream.h"

#define B64_READ_BUFFER_SIZE 1024
#define B64_ENCODE_BUFFER_SIZE (1024 + 2) * 4 / 3
#define B64_DECODE_BUFFER_SIZE 1024 / 4 * 3

typedef int (*handler_t)(FILE*);


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
    fwrite("\n", 1, 1, stdout);
    return 0;

}

int decode(FILE* file)
{
    int result;
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
    
    result = b64_stream_decode_final(&state);
    if (!result)
    {
        printf("base64: invalid input\n");
    }
    return result;
}

void help() 
{
    printf("Usage: base64 [OPTION]... [FILE]\n");
    printf("Base64 encode or decode file or standart input to standart output\n"); 
    printf("\n");
    printf("Mandatory arguments to long options are mandatory for short options too.\n");
    printf("  -d, --decode  decode data\n");
    printf("  -h, --help    display this help and exit\n");
}

int main(int argc, char* argv[]) 
{
    int i = 1;
    int decode_flag = 0;
    FILE* fd = stdin;
    handler_t handler = &encode;
    
    while (1)
    {
        if (i == argc)
            break;
        if ((strcmp(argv[i], "-d") == 0) || (strcmp(argv[i], "--decode") == 0))
        {
            i++;
            handler = &decode;
            continue;
        }
        else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
        {
            help();
            return 0;
        }
        else
        {
            fd = fopen(argv[i], "rb");
            if (fd == NULL)
            {
                printf("base64: %s: No such file or directory\n", argv[i]);
                return 1;
            }
            i++;
        }
    }

    int result = handler(fd);

    fflush(stdout);
    fclose(fd);
    return result;
}
