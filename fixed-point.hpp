
#include <cstdint>
#include <vector>
#include <cstdio>
#include <iostream>
#include <array>

using namespace std;

#define FP_VALUE_LEN 2

typedef struct{
    array<uint32_t, FP_VALUE_LEN> value = {0};
} fp_t;

fp_t fp_create();

fp_t fp_create_from_double(double double_value);

void fp_print(const fp_t & num);

bool fp_set_bit(fp_t & num, size_t idx, bool value);

fp_t fp_add_fp(const fp_t & num0, const fp_t & num1);

fp_t fp_sub_fp(const fp_t & num0, const fp_t & num1);
