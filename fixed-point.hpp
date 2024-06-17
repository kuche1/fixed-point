
#include <cstdint>
#include <vector>
#include <array>
#include <fstream>

using namespace std;

#define FP_VALUE_LEN 4

typedef enum{
    FP_SATURATION_CRASH,
    FP_SATURATION_NO_CRASH,
} fp_saturation_action_t;

typedef struct{
    array<uint32_t, FP_VALUE_LEN> value = {0};
} fp_t;

void fp_print(const fp_t & num);

//  creation

fp_t fp_create_from_double(double double_value);

// bitwise operations

bool fp_check_first_bit(const fp_t & num);

char fp_check_first_byte(const fp_t & num);

bool fp_set_bit(fp_t & num, size_t idx, bool value);

void fp_set_8_least_significant_bits(fp_t & num, unsigned char value);

void fp_left_shift_by_1(fp_t & num);

void fp_left_shift_by_8(fp_t & num);

void fp_left_shift_by_up_to_31(fp_t & num, size_t value);

void fp_left_shift_by(fp_t & num, size_t value);

// arithmeric operations fp-raw

void fp_raw_inc(fp_t & num);

// arithmeric operations fp-raw ui32

void fp_raw_dec_ui32(fp_t & num0, uint32_t num1);

// arithmetic operations fp fp

fp_t fp_add_fp(const fp_t & num0, const fp_t & num1, fp_saturation_action_t saturation_action = FP_SATURATION_CRASH);

fp_t fp_sub_fp(const fp_t & num0, const fp_t & num1);

fp_t fp_mul_fp(const fp_t & num0, const fp_t & num1);

// fp_t fp_div_ui32(fp_t num0, const uint32_t num1);
// this is so slow, I can't even test it

// comparisons fp-raw ui32

bool fp_raw_lt_ui32(const fp_t & num0, const uint32_t num1);

// comparisons fp fp

bool fp_lt_fp(const fp_t & num0, const fp_t & num1);

bool fp_le_fp(const fp_t & num0, const fp_t & num1);

bool fp_eq_fp(const fp_t & num0, const fp_t & num1);

bool fp_ge_fp(const fp_t & num0, const fp_t & num1);

// file dump

void fp_write_significant_to_file(fp_t num, ofstream & file);
// writes data as long as it's not 0

void fp_gobble_as_much_as_possible_from_file(fp_t & num, ifstream & file);

bool fp_gobble_next_byte_from_file(fp_t & num, ifstream & file);
