#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

char* caesar(int n, const char* s) {
    /* Determine the length of the input string */
    size_t len = strlen(s);
    /* Allocate memory for the result string */
    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    /* Iterate through each character in the input string */
    for (size_t i = 0; i < len; i++) {
        /* Check if the character is an uppercase letter */
        if ('A' <= s[i] && s[i] <= 'Z') {
            /* Apply the Caesar cipher shift for uppercase letters */
            result[i] = 'A' + (s[i] - 'A' + n) % 26;
        }
        /* Check if the character is a lowercase letter */
        else if ('a' <= s[i] && s[i] <= 'z') {
            /* Apply the Caesar cipher shift for lowercase letters */
            result[i] = 'a' + (s[i] - 'a' + n) % 26;
        }
        /* Keep non-letter characters unchanged */
        else {
            result[i] = s[i];
        }
    }
    /* Null-terminate the result string */
    result[len] = '\0';
    return result;
}

/* Affine cipher encryption function */
char* affine(int a, int b, const char* s) {
    /* Determine the length of the input string */
    size_t len = strlen(s);
    /* Allocate memory for the result string */
    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    /* Iterate through each character in the input string */
    for (size_t i = 0; i < len; i++) {
        /* Check if the character is an uppercase letter */
        if ('A' <= s[i] && s[i] <= 'Z') {
            /* Apply the Affine cipher transformation for uppercase letters */
            result[i] = 'A' + (a * (s[i] - 'A') + b) % 26;
        }
        /* Check if the character is a lowercase letter */
        else if ('a' <= s[i] && s[i] <= 'z') {
            /* Apply the Affine cipher transformation for lowercase letters */
            result[i] = 'a' + (a * (s[i] - 'a') + b) % 26;
        }
        /* Keep non-letter characters unchanged */
        else {
            result[i] = s[i];
        }
    }
    /* Null-terminate the result string */
    result[len] = '\0';
    return result;
}

int is_prime(mpz_t num) {
    return mpz_probab_prime_p(num, 25);
}

void calculate_rsa_params(mpz_t p, mpz_t q, mpz_t n, mpz_t phi_n, mpz_t e, mpz_t d) {
    /* Calculate n = p * q */
    mpz_mul(n, p, q);
    /* Calculate phi_n = (p-1) * (q-1) */
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi_n, p, q);
    /* Choose e such that 1 < e < phi_n and gcd(e, phi_n) = 1 */
    mpz_set_ui(e, 3);
    while (1) {
        mpz_t gcd;
        mpz_init(gcd);
        mpz_gcd(gcd, e, phi_n);
        if (mpz_cmp_ui(gcd, 1) == 0) {
            mpz_clear(gcd);
            break;
        }
        mpz_add_ui(e, e, 2);
        mpz_clear(gcd);
    }
    /* Calculate d = e^-1 mod phi_n */
    if (mpz_invert(d, e, phi_n) == 0) {
        mpz_clear(phi_n);
        gmp_printf("Modular inverse does not exist\n");
        return;
    }
}

void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    /* Calculate c = m^e mod n */
    mpz_powm(c, m, e, n);
}

void decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    /* Calculate m = c^d mod n */
    mpz_powm(m, c, d, n);
}

void rsa(const char* prime_str1, const char* prime_str2, const char* message) {
    mpz_t p, q, n, phi_n, e, d, m, c;

    mpz_inits(p, q, n, phi_n, e, d, m, c, NULL);

    /* Convert prime numbers from strings to mpz_t */
    mpz_set_str(p, prime_str1, 10);
    if (!is_prime(p)) {
        printf("p is not a prime number.\n");
        return;
    }

    mpz_set_str(q, prime_str2, 10);
    if (!is_prime(q)) {
        printf("q is not a prime number.\n");
        return;
    }

    /* Calculate RSA parameters */
    calculate_rsa_params(p, q, n, phi_n, e, d);

    /* Encrypt and decrypt each character */
    printf("Original message: %s\n", message);
    printf("Encrypted message: ");
    for (int i = 0; i < strlen(message); i++) {
        mpz_set_ui(m, message[i]);
        encrypt(c, m, e, n);
        gmp_printf("%Zd ", c);
    }
    printf("\n");
    gmp_printf("Prime number p: %Zd\n", p);
    gmp_printf("Prime number q: %Zd\n", q);
    gmp_printf("RSA parameter n: %Zd\n", n);
    gmp_printf("RSA parameter phi_n: %Zd\n", phi_n);
    gmp_printf("RSA parameter d: %Zd\n", d);
    printf("\nDecrypted message: ");
    for (int i = 0; i < strlen(message); i++) {
        mpz_set_ui(m, message[i]);
        encrypt(c, m, e, n);
        decrypt(m, c, d, n);
        printf("%c", (char)mpz_get_ui(m));
    }
    printf("\n");

    mpz_clears(p, q, n, phi_n, e, d, m, c, NULL);
}

/* generate a big random prime of a certain bit length */
char* big_prime(int bits) {
    mpz_t prime;
    gmp_randstate_t randstate;
    char* prime_str;
    /* Initialize variables */
    mpz_init(prime);
    gmp_randinit_default(randstate);
    /* Seed the random number generator with the current system time */
    unsigned long seed = (unsigned long)time(NULL);
    gmp_randseed_ui(randstate, seed);
    /* Generate a random number with the specified number of bits */
    mpz_urandomb(prime, randstate, bits);
    /* Set the most significant bit and the least significant bit to ensure the number has the desired bit length */
    mpz_setbit(prime, bits - 1);
    mpz_setbit(prime, 0);
    /* Find the next prime greater than or equal to the generated number */
    mpz_nextprime(prime, prime);
    /* Allocate memory for the string representation of the prime */
    prime_str = (char*)malloc((size_t)mpz_sizeinbase(prime, 10) + 2);
    /* Convert the prime to a string */
    mpz_get_str(prime_str, 10, prime);
    /* Clean up resources */
    mpz_clear(prime);
    gmp_randclear(randstate);
    return prime_str;
}
