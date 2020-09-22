#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// polyfill of non-standard drand48()
#ifdef _MSC_VER

#include <random>
static std::mt19937_64 std_gen64;
static std::uniform_real_distribution<double> std_dist64(0., 1.);
//extern "C" void srand48(int64_t seed) { std_gen64.seed(seed); }

//static std::uniform_int_distribution<int64_t> std_disti64(-(1 << 31), 1 << 31);
//extern "C" int64_t mrand48() { return std_disti64(std_gen64); }

#endif // _MSC_VER

#ifdef __cplusplus
extern "C" {
#endif

void write(const uint8_t* line, size_t size) {
    fwrite(line, size, 1, stdout);
}

void print(const uint8_t* s) {
    printf("%s", s);
}

void println(const uint8_t* s) {
    printf("%s\n", s);
}

void print_i32(int32_t i) {
    printf("%" PRIi32 "\n", i);
}

void print_u8(uint8_t c) {
    printf("%c\n", c);
}

void print_f64(double d) {
    printf("%.9f\n", d);
}

void print_piece_mask(const uint64_t* a) {
    for (int i = 0; i < 12; ++i)
        printf("%" PRIu64 " ", a[i]);
    printf("\n");
}

void print_piece_def(const uint8_t* a) {
    for (int i = 0; i < 40; ++i)
        printf("%d ", a[i]);
    printf("\n");
}

void print_meteor_scnt(int32_t cnt) {
    printf("%" PRIi32 " solutions found\n\n", cnt);
}

void print_meteor_lines(const uint8_t* a) {
    printf("%c %c %c %c %c \n %c %c %c %c %c \n",
        a[0] + '0', a[1] + '0', a[2] + '0', a[3] + '0', a[4] + '0',
        a[5] + '0', a[6] + '0', a[7] + '0', a[8] + '0', a[9] + '0');
}

int32_t crt_atoi(uint8_t* str) { return atoi(reinterpret_cast<const char*>(str)); }

double crt_drand() {
#ifdef _MSC_VER
	return std_dist64(std_gen64);
#else
	return drand48();
#endif // _MSC_VER
}

double crt_sqrt(double x) { return sqrt(x); }
double crt_fabs(double x) { return fabs(x); }
double crt_cos(double x) { return cos(x); }
double crt_sin(double x) { return sin(x); }

double* alloc_img(int32_t w, int32_t h) {
    auto ptr = calloc(sizeof(double), w * h * 3);
    return static_cast<double*>(ptr);
}

static inline uint8_t convert_col(double col) {
    col *= 255.0;
    if (col < 0) col = 0;
    if (col > 255) col = 255;
    return (uint8_t)col;
}

void save_img(int32_t w, int32_t h, const double* img) {
    printf("P6\n");
    printf("%d %d\n", w, h);
    printf("255\n");
    auto ptr = malloc(sizeof(uint8_t) * 3 * w);
    uint8_t* out_row = static_cast<uint8_t*>(ptr);
    for (int32_t y = 0; y < h; y++) {
        const double* in_row = &img[y * (w * 3)];
        for (int32_t x = 0; x < w; x++) {
            out_row[x * 3 + 0] = convert_col(in_row[x * 3 + 0]);
            out_row[x * 3 + 1] = convert_col(in_row[x * 3 + 1]);
            out_row[x * 3 + 2] = convert_col(in_row[x * 3 + 2]);
        }
        fwrite(out_row, w * 3, sizeof(uint8_t), stdout);
    }
    free(out_row);
}

#ifdef __cplusplus
} // extern "C"
#endif
