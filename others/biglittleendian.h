#ifndef _UTILS_BIG_LITTLE_ENDIAN_H
#define _UTILS_BIG_LITTLE_ENDIAN_H
#ifdef __cplusplus
extern "C" {
#endif
typedef unsigned short int uint16;
typedef unsigned long int uint32;

/*
* big little swap for unsigned short 16 bits.
*/
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | \
                             (((uint16)(A) & 0x00ff) << 8))

/*
* big little swap for unsigned int 32 bits.
*/
#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
                             (((uint32)(A) & 0x00ff0000) >> 8 ) | \
                             (((uint32)(A) & 0x0000ff00) << 8 ) | \
                             (((uint32)(A) & 0x000000ff) << 24))

/*
* return 1 for big, 0 for little.
*/
int checkCPUendian()
{
    union{
        unsigned long int i;
        unsigned char s[4];
    }c;

    c.i = 0x12345678;
    return (0x12 == c.s[0]);
}

/*
* like htonl, local host bits to network bits.
*/
unsigned long int t_htonl(unsigned long int h)
{
    /*
     * only return when local host and network bits are big endian.
     * bits is swapped to big endian when local host bits is little endian.
     */
    return checkCPUendian() ? h : BigLittleSwap32(h);
}

/*
* like ntohl, network bits to local host bits.
*/
unsigned long int t_ntohl(unsigned long int n)
{
    /*
     * only return when local host and network bits are big endian.
     * bits is swapped network big endian to local host little endian
     * when local host bits is little endian.
     */
    return checkCPUendian() ? n : BigLittleSwap32(n);
}

/*
* like htons, local host bits to network bits.
*/
unsigned short int t_htons(unsigned short int h)
{
    return checkCPUendian() ? h : BigLittleSwap16(h);
}

/*
* like ntohs, network bits to local host bits.
*/
unsigned short int t_ntohs(unsigned short int n)
{
    return checkCPUendian() ? n : BigLittleSwap16(n);
}
#ifdef __cplusplus
}
#endif
#endif /* _UTILS_BIG_LITTLE_ENDIAN_H */