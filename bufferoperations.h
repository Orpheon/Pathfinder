#include <stdint.h>

int read_uint(unsigned char *buffer, int offset)
{
    return (((uint32_t)buffer[offset+3]) | (((uint32_t)buffer[offset+2])<<8) | (((uint32_t)buffer[offset+1])<<16) | (((uint32_t)buffer[offset+0])<<24));
}
