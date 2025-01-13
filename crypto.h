#ifndef CRYPTO_H
#define CRYPTO_H

#include <gmp.h>

/* Function prototypes */
void rsa(const char* prime_str1, const char* prime_str2, const char* message);
char* caesar(int n, const char* s);
char* affine(int a, int b, const char* s);
char* big_prime(int bits);

#endif /* CRYPTO_H */
