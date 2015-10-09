#ifndef _BASE64UTILS_H_
#define _BASE64UTILS_H_

#ifdef  __cplusplus
extern "C" {
#endif /* __cplusplus */
    char *base64_decode(const char *in, char *out);
    const char *base64_encode(const char *in, char *out);
    void base64_encode1(char *dbuf, const char *data, int data_len);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif // #define _BASE64UTILS_H_
