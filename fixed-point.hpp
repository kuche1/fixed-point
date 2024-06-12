
#include <cstdint>
#include <vector>
#include <array>

using namespace std;

#define FP_VALUE_LEN 4

typedef enum{
    FP_SATURATION_CRASH,
    FP_SATURATION_NO_CRASH,
} fp_saturation_action_t;

typedef struct{
    array<uint32_t, FP_VALUE_LEN> value = {0};
} fp_t;

fp_t fp_create_from_double(double double_value);

void fp_print(const fp_t & num);

// bitwise operations

bool fp_set_bit(fp_t & num, size_t idx, bool value);

bool fp_check_first_bit(const fp_t & num);

char fp_check_first_byte(const fp_t & num);

void fp_left_shift_by_1(fp_t & num);

void fp_left_shift_by_8(fp_t & num);

// arithmetic operations

fp_t fp_add_fp(const fp_t & num0, const fp_t & num1, fp_saturation_action_t saturation_action = FP_SATURATION_CRASH);

fp_t fp_sub_fp(const fp_t & num0, const fp_t & num1);

fp_t fp_mul_fp(const fp_t & num0, const fp_t & num1);

// comparisons

bool fp_lt_fp(const fp_t & num0, const fp_t & num1);

bool fp_le_fp(const fp_t & num0, const fp_t & num1);

bool fp_eq_fp(const fp_t & num0, const fp_t & num1);
