#include <stdlib.h>
#include <stdio.h>

#define FIELD_GENERATOR 3

#define LOG_TABLE_SIZE 256

#define ANTILOG_TABLE_SIZE 255

unsigned char gf_mul(const unsigned char a, const unsigned char b, const unsigned short ipoly) {
    unsigned char p = 0, c, aa = a, bb = b;

#define gf_mul_iter(p, c, a, b, pl) {\
    if ((b) & 0x1) {\
        (p) ^= (a);\
    }\
    (b) >>= 1;\
    (c) = (a) >> 7;\
    (a) <<= 1;\
    if ((c)) {\
        (a) ^= (pl);\
    }\
}
    gf_mul_iter(p, c, aa, bb, ipoly);
    gf_mul_iter(p, c, aa, bb, ipoly);
    gf_mul_iter(p, c, aa, bb, ipoly);
    gf_mul_iter(p, c, aa, bb, ipoly);
    gf_mul_iter(p, c, aa, bb, ipoly);
    gf_mul_iter(p, c, aa, bb, ipoly);
    gf_mul_iter(p, c, aa, bb, ipoly);
    gf_mul_iter(p, c, aa, bb, ipoly);

#undef gf_mul_iter

    return p;
}

void eval_log_antilog_tables(unsigned char *log_table,
                             unsigned char *antilog_table,
                             const size_t antilog_table_size,
                             unsigned char field_generator, unsigned short ipoly) {
    size_t i = 0;
    unsigned char x = 1;
    log_table[0] = 0;
    while (i < antilog_table_size) {
        log_table[x] = i;
        antilog_table[i] = x;
        x = gf_mul(x, field_generator, ipoly);
        i++;
    }
}

void dump_table(unsigned char *table, const size_t table_size, const size_t entries_per_line, const char *table_name) {
    size_t l;
    printf("unsigned char %s[] = {", table_name);
    for (l = 0; l < table_size; l++) {
        if ((l % entries_per_line) == 0) {
            printf("\n   ");
        }
        printf(" 0x%.2X", table[l]);
        if ((l + 1) != table_size) {
            printf(",");
        } else {
            printf("\n};\n\n");
        }
    }
}

unsigned char gf_mul_lgt(unsigned char a, unsigned char b, unsigned char *log_table, unsigned char *antilog_table) {
    if (a == 0 || b == 0) {
        return 0;
    }
    return antilog_table[(log_table[a] + log_table[b]) % 255];
}

unsigned char gf_inv_mul_lgt(unsigned char p, unsigned char *log_table, unsigned char *antilog_table) {
    if (p == 0) {
        return 0;
    }
    return antilog_table[255 - log_table[p]];
}

int main(int argc, char **argv) {
    unsigned char log_table[LOG_TABLE_SIZE];
    unsigned char antilog_table[ANTILOG_TABLE_SIZE];
    eval_log_antilog_tables(log_table, antilog_table, ANTILOG_TABLE_SIZE, FIELD_GENERATOR, 0x11B);
    dump_table(log_table, sizeof(log_table) / sizeof(log_table[0]), 16, "log_table");
    dump_table(antilog_table, sizeof(antilog_table) / sizeof(antilog_table[0]), 16, "antilog_table");
    return 0;
}
