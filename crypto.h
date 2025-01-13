#ifndef CRYPTO_H
#define CRYPTO_H

#include <gmp.h>

/* Function prototypes */
char* caesar(int n, const char* s);
char* affine(int a, int b, const char* s);
char* big_prime(int bits);
void rsa(const char* prime_str1, const char* prime_str2, const char* message);
void one_time_pad(const char *m, const char *k, const char *c, size_t m_len, size_t k_len, size_t c_len);

#endif /* CRYPTO_H */
