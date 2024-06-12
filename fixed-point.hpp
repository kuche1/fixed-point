
#include <cstdint>
#include <vector>
#include <array>

using namespace std;

#define FP_VALUE_LEN 4

typedef struct{
    array<uint32_t, FP_VALUE_LEN> value = {0};
} fp_t;

fp_t fp_create_from_double(double double_value);

void fp_print(const fp_t & num);

bool fp_set_bit(fp_t & num, size_t idx, bool value);

bool fp_check_first_bit(const fp_t & num);

void fp_left_shift_by_1(fp_t & num);

fp_t fp_add_fp(const fp_t & num0, const fp_t & num1);

fp_t fp_sub_fp(const fp_t & num0, const fp_t & num1);

fp_t fp_mul_fp(const fp_t & num0, const fp_t & num1);

bool fp_lt_fp(const fp_t & num0, const fp_t & num1);

bool fp_le_fp(const fp_t & num0, const fp_t & num1);
