#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

// Binär → Grey (en uint32_t i taget)
uint32_t bin_to_gray(uint32_t bin) {
    return bin ^ (bin >> 1);
}

// Grey → Binär (snabb konstanttid-version)
uint32_t gray_to_bin(uint32_t gray) {
    gray ^= gray >> 16;
    gray ^= gray >>  8;
    gray ^= gray >>  4;
    gray ^= gray >>  2;
    gray ^= gray >>  1;
    return gray;
}

// ──────────────────────────────────────────────────
// Grey-koda data från stdin → stdout (binär → Grey)
void gray_encode_stream() {
    uint32_t buffer[8192];
    size_t n;

    while ((n = fread(buffer, sizeof(uint32_t), 8192, stdin)) > 0) {
        for (size_t i = 0; i < n; i++) {
            buffer[i] = bin_to_gray(buffer[i]);
        }
        fwrite(buffer, sizeof(uint32_t), n, stdout);
    }
}

// ──────────────────────────────────────────────────
// Avkoda Grey-data från stdin → stdout (Grey → binär)
void gray_decode_stream() {
    uint32_t buffer[8192];
    size_t n;

    while ((n = fread(buffer, sizeof(uint32_t), 8192, stdin)) > 0) {
        for (size_t i = 0; i < n; i++) {
            buffer[i] = gray_to_bin(buffer[i]);
        }
        fwrite(buffer, sizeof(uint32_t), n, stdout);
    }
}

// ──────────────────────────────────────────────────
// Huvudprogram med kommandoradsflaggor
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Användning:\n");
        fprintf(stderr, "  %s -e   | --encode   : Binär → Greykod till stdout\n", argv[0]);
        fprintf(stderr, "  %s -d   | --decode   : Greykod → Binär till stdout\n", argv[0]);
        fprintf(stderr, "  %s -t   | --test     : Kör internt test (som tidigare)\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "--encode") == 0) {
        gray_encode_stream();
    }
    else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--decode") == 0) {
        gray_decode_stream();
    }
    else if (strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--test") == 0) {
        // Samma test som i tidigare versionen
        printf("Test av Grey-kodning (0–31):\n");
        for (uint32_t i = 0; i < 32; i++) {
            uint32_t g = bin_to_gray(i);
            uint32_t b = gray_to_bin(g);
            printf("%2u:  %08x → %08x → %08x %s\n", i, i, g, b, (b == i) ? "OK" : "FEL");
        }
        printf("Större test: 123456789 → %u → %u %s\n",
               bin_to_gray(123456789),
               gray_to_bin(bin_to_gray(123456789)),
               (gray_to_bin(bin_to_gray(123456789)) == 123456789) ? "OK" : "FEL");
    }
    else {
        fprintf(stderr, "Okänt kommando: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
