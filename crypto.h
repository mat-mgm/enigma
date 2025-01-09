#ifndef CRYPTO_H
#define CRYPTO_H

#include <gmp.h>

/* Function prototypes */
static int is_prime(mpz_t num);
static void calculate_rsa_params(mpz_t p, mpz_t q, mpz_t n, mpz_t phi_n, mpz_t e, mpz_t d);
static void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n);
static void decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n);
static void rsa(const char* prime_str1, const char* prime_str2, const char* message);
char* caesar(int n, const char* s);
char* affine(int a, int b, const char* s);
char* big_prime(int bits);

#endif /* CRYPTO_H */
