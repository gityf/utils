/*
 * $Id$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"

static void base64_init(void);

static int base64_initialized = 0;
#define BASE64_VALUE_SZ 256
int base64_value[BASE64_VALUE_SZ];
const char base64_code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void
base64_init(void)
{
    int i;

    for (i = 0; i < BASE64_VALUE_SZ; i++)
        base64_value[i] = -1;

    for (i = 0; i < 64; i++)
        base64_value[(int) base64_code[i]] = i;
    base64_value['='] = 0;

    base64_initialized = 1;
}

char *
base64_decode(const char *in, char *out)
{
    char *result = out;
    int j;
    int c;
    long val;
    const char *p = in;
    if (!p || !out)
        return NULL;
    if (!base64_initialized)
        base64_init();
    val = c = 0;
    for (j = 0; *p && j + 4 < 0xffff; p++) {
        unsigned int k = ((unsigned char) *p) % BASE64_VALUE_SZ;
        if (base64_value[k] < 0)
            continue;
        val <<= 6;
        val += base64_value[k];
        if (++c < 4)
            continue;
        /* One quantum of four encoding characters/24 bit */
        result[j++] = (val >> 16) & 0xff;   /* High 8 bits */
        result[j++] = (val >> 8) & 0xff;    /* Mid 8 bits */
        result[j++] = val & 0xff;   /* Low 8 bits */
        val = c = 0;
    }
    result[j] = 0;
    return result;
}

/* adopted from http://ftp.sunet.se/pub2/gnu/vm/base64-encode.c with adjustments */
const char *
base64_encode(const char *in, char *out)
{
    char *result = out;
    int bits = 0;
    int char_count = 0;
    int out_cnt = 0;
    int c;

    if ( !in )
    {
        return in;
    }
    if ( !out )
    {
        return out;
    }


    if (!base64_initialized)
        base64_init();

    while ((c = (unsigned char) *in++) && out_cnt < sizeof(result) - 5) {
        bits += c;
        char_count++;
        if (char_count == 3) {
            result[out_cnt++] = base64_code[bits >> 18];
            result[out_cnt++] = base64_code[(bits >> 12) & 0x3f];
            result[out_cnt++] = base64_code[(bits >> 6) & 0x3f];
            result[out_cnt++] = base64_code[bits & 0x3f];
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 8;
        }
    }
    if (char_count != 0) {
        bits <<= 16 - (8 * char_count);
        result[out_cnt++] = base64_code[bits >> 18];
        result[out_cnt++] = base64_code[(bits >> 12) & 0x3f];
        if (char_count == 1) {
            result[out_cnt++] = '=';
            result[out_cnt++] = '=';
        } else {
            result[out_cnt++] = base64_code[(bits >> 6) & 0x3f];
            result[out_cnt++] = '=';
        }
    }
    result[out_cnt] = '\0'; /* terminate */
    return result;
}

/* adopted from http://ftp.sunet.se/pub2/gnu/vm/base64-encode.c with adjustments */
const char *
base64_encode_bin(const char *data, int len, char *out)
{
    char *result = out;
    int bits = 0;
    int char_count = 0;
    int out_cnt = 0;

    if (!data || !out)
        return NULL;

    if (!base64_initialized)
        base64_init();

    while (len-- && out_cnt < sizeof(result) - 5) {
        int c = (unsigned char) *data++;
        bits += c;
        char_count++;
        if (char_count == 3) {
            result[out_cnt++] = base64_code[bits >> 18];
            result[out_cnt++] = base64_code[(bits >> 12) & 0x3f];
            result[out_cnt++] = base64_code[(bits >> 6) & 0x3f];
            result[out_cnt++] = base64_code[bits & 0x3f];
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 8;
        }
    }
    if (char_count != 0) {
        bits <<= 16 - (8 * char_count);
        result[out_cnt++] = base64_code[bits >> 18];
        result[out_cnt++] = base64_code[(bits >> 12) & 0x3f];
        if (char_count == 1) {
            result[out_cnt++] = '=';
            result[out_cnt++] = '=';
        } else {
            result[out_cnt++] = base64_code[(bits >> 6) & 0x3f];
            result[out_cnt++] = '=';
        }
    }
    result[out_cnt] = '\0'; /* terminate */
    return result;
}
void base64_encode1(char *dbuf, const char *data, int data_len)
{
    const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    int prepare;
    int temp = 0;
    char *f = NULL;
    int tmp = 0;
    char changed[4];
    int i = 0;
    temp = data_len % 3;
    f = dbuf;
    while (tmp < data_len)
    {
        temp = 0;
        prepare = 0;
        memset(changed, '\0', 4);
        while (temp < 3)
        {
            //printf("tmp = %d\n", tmp);
            if (tmp >= data_len)
            {
                break;
            }
            prepare = ((prepare << 8) | (data[tmp] & 0xFF));
            tmp++;
            temp++;
        }
        prepare = (prepare<<((3-temp)*8));
        //printf("before for : temp = %d, prepare = %d\n", temp, prepare);
        for (i = 0; i < 4 ;i++ )
        {
            if (temp < i)
            {
                changed[i] = 0x40;
            }
            else
            {
                changed[i] = (prepare>>((3-i)*6)) & 0x3F;
            }
            *f = base[changed[i]];
            //printf("%.2X", changed[i]);
            f++;
        }
    }
    *f = '\0';
}