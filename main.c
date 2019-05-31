#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *byte_to_bitstring(char c) {
    char *bitstring = malloc(9 * sizeof(char));
    for (int i = 7; i >= 0; --i) {
        bitstring[i] = (c & 0b1) + '0';
        c >>= 1;
    }
    bitstring[8] = 0;
    return bitstring;
}

char bitstring_to_char(char *bitstring) {
    unsigned char c = 0;
    for ( int i = 0; i < 8; ++i) {
        c <<= 1;
        c += (bitstring[i] != '0');
    }
    return c;
}

void LZ_print(FILE* fp, int relevant_bytes) {
    char *dest = malloc(relevant_bytes * sizeof(char) * 8);
    for (int i = 0; i < relevant_bytes; ++i) {
        char* bits = byte_to_bitstring(fgetc(fp));
        strncpy(dest + i * 8, bits, 8);
    }

    for (int i = 0; i < relevant_bytes * 8;) {
        if (dest[i] == '0') {
            printf("%.9s ", dest + i);
            char c = bitstring_to_char(dest+i+1);
            if ( c >= ' ')
                printf("(%c) ", c);
            i += 9;
        } else {
            printf("%.17s ", dest + i );
            i += 17;
        }
    }

    printf("\n");
}

int main( int argc, char *argv[] ) {
    int relevant_bytes = 32;

    if (argc < 2) {
        printf("Use: bitreader <path_to_file> [-lz]\n");
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Unable to open file!\n");
        return EXIT_FAILURE;
    }

    if (argc >= 3)
        sscanf(argv[2], "%d", &relevant_bytes);

    if (argc >= 4) {
        if (strcmp(argv[3], "-lz") == 0) {
            LZ_print(fp, relevant_bytes);
            return EXIT_SUCCESS;
        }
    }

    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    if ( sz < relevant_bytes )
        relevant_bytes = sz;
    fseek(fp, 0L, SEEK_SET);

    char ch;
    for (int i = 0; i < relevant_bytes && (ch =fgetc(fp)) != EOF; ++i) {
        printf("%s ", byte_to_bitstring(ch));
    }
    printf("\n");

    return EXIT_SUCCESS;
}
