
#include "fixed-point.hpp"

#include <iostream>
#include <cstdio>
#include <ranges>

#define ERR(...) { \
    cerr << "ERROR: "; \
    cerr << "file `" << __FILE__ << "` "; \
    cerr << "line " << __LINE__ << ": "; \
    cerr << __VA_ARGS__; \
    cerr << endl; \
    exit(1); \
}

#define UNREACHABLE() { \
    ERR("Unreachable code reached") \
}

#define ASSERT(condition) { \
    if(!(condition)){ \
        ERR("Assertion failed"); \
    } \
}

////// uint64_t uint32_t bitwise operations

bool ui32_check_first_bit(uint32_t num){

    uint32_t zeroes = 0;
    uint32_t ones = ~ zeroes;
    uint32_t zero_followed_by_1s = ones >> 1;
    uint32_t one_followed_by_0s = ~ zero_followed_by_1s;

    if(num & one_followed_by_0s){
        return true;
    }else{
        return false;
    }

}

char ui32_check_first_byte(uint32_t num){

    uint32_t zeroes = 0;
    uint32_t ones = ~ zeroes;
    uint32_t eight_0s_followed_by_1s = ones >> 8;
    uint32_t eight_1s_followed_by_0s = ~ eight_0s_followed_by_1s;
    uint32_t first_8bits_followed_by_0s = num & eight_1s_followed_by_0s;
    uint32_t zeroes_followed_by_first_8bits = first_8bits_followed_by_0s >> ( (sizeof(num) - sizeof(char)) * 8);
    char first_8bits = static_cast<char>(zeroes_followed_by_first_8bits);

    return first_8bits;

}

////// uint64_t uint32_t mathematics

pair<uint32_t, uint32_t> ui64_split(uint64_t num){
    uint32_t overflow = static_cast<uint32_t>( (num & 0xffffffff00000000) >> 32 );
    uint32_t value    =                         num & 0x00000000ffffffff;
    return {overflow, value};
}

pair<uint32_t, uint32_t> ui32_add(uint32_t num0, uint32_t num1){
    uint64_t result = num0;
    result += num1;
    return ui64_split(result);
}

pair<uint32_t, uint32_t> ui32_sub(uint32_t num0, uint32_t num1){
    if(num1 > num0){
        return {1, num0 - num1};
    }else{
        return {0, num0 - num1};
    }
}

pair<uint32_t, uint32_t> ui32_mul(uint32_t num0, uint32_t num1){
    // we can store the result in a uint64_t because:
    // (2**32-1) * (2**32-1) < (2**64-1)
    uint64_t result = num0;
    result *= num1;
    return ui64_split(result);
}

pair<uint32_t, uint32_t> ui32_div(uint32_t num0, uint32_t num1){
    uint64_t result = num0;
    result <<= 32;
    result /= num1;
    return ui64_split(result);
}

////// fixed point

fp_t fp_create_from_double(double double_value){

    if(double_value < 0.0){
        ERR("Value must be positive");
    }

    if(double_value > 1.0){
        ERR("Value must not be greater than 1.0: " << double_value);
    }

    fp_t num;
    size_t bit_idx = 0;

    while(true){

        bool fail = [&double_value, &num, &bit_idx]{
            if(double_value < 0.5){
                double_value *= 2;
                return fp_set_bit(num, bit_idx, false);
            }else{
                double_value -= 0.5;
                double_value *= 2;
                return fp_set_bit(num, bit_idx, true);
            }
        }();

        if(fail){
            break;
        }

        bit_idx += 1;

    }

    return num;
}

void fp_print(const fp_t & num){
    cout << "[" << num.value.size() * sizeof(num.value[0]) << "B]0x";
    for(uint32_t piece : num.value){
        printf("[%08x]", piece);
    }
}

bool fp_set_bit(fp_t & num, size_t idx, bool value){

    size_t part_idx = 0;

    while(idx >= 8 * sizeof(num.value[0])){
        part_idx += 1;
        idx -= 8 * sizeof(num.value[0]);
    }

    if(part_idx >= num.value.size()){
        return true;
    }

    if(value){
        uint32_t zeroes = 0;
        uint32_t ones = ~zeroes;
        uint32_t leading_0_then_1s = ones >> 1;
        uint32_t leading_1_then_0s = ~ leading_0_then_1s;
        num.value.at(part_idx) |= leading_1_then_0s >> idx;
    }else{
        uint32_t one = 1;
        num.value.at(part_idx) &= ~ ( one << ( sizeof(num.value[0])*8 -1 - idx ) );
    }

    return false;

}

bool fp_check_first_bit(const fp_t & num){
    return ui32_check_first_bit(num.value.at(0));
}

char fp_check_first_byte(const fp_t & num){
    return ui32_check_first_byte(num.value.at(0));
}

void fp_left_shift_by_1(fp_t & num){

    uint32_t prev_bit = 0;

    for(uint32_t & part : ranges::reverse_view(num.value)){

        bool first_bit = ui32_check_first_bit(part);

        part <<= 1;

        part |= prev_bit;

        prev_bit = first_bit;

    }

}

void fp_left_shift_by_8(fp_t & num){

    uint32_t prev_byte = 0;

    for(uint32_t & part : ranges::reverse_view(num.value)){

        char first_byte = ui32_check_first_byte(part);

        part <<= 8;

        part |= static_cast<unsigned char>(prev_byte);
        // if it's not for the unsigned cast, shit gets really bad

        prev_byte = first_byte;

    }

}

fp_t fp_add_fp(const fp_t & num0, const fp_t & num1, fp_saturation_action_t saturation_action){

    fp_t result = num0;
    const fp_t & to_add = num1;

    uint32_t overflow = 0;

    for(ssize_t i=FP_VALUE_LEN-1 ; i>=0 ; --i){

        uint32_t part0 = result.value.at(i);
        uint32_t part1 = to_add.value.at(i);

        auto [ovf0, val_step0] = ui32_add(part0, part1);
        auto [ovf1, val_step1] = ui32_add(val_step0, overflow);

        result.value.at(i) = val_step1;

        auto [zero, overflow_new] = ui32_add(ovf0, ovf1);
        ASSERT(zero == 0);

        overflow = overflow_new;
    }

    if(overflow != 0){
        switch(saturation_action){
            case FP_SATURATION_CRASH:
                ERR("saturation");
                break;
            case FP_SATURATION_NO_CRASH:
                return fp_create_from_double(1.0); // TODO this could be sped up if we create such an object beforehand
                break;
            default:
                UNREACHABLE();
        }
    }

    return result;
}

fp_t fp_sub_fp(const fp_t & num0, const fp_t & num1){
    
    fp_t result = num0;

    uint32_t underflow = 0;

    for(ssize_t i=FP_VALUE_LEN-1; i>=0; --i){

        uint32_t part0 = num0.value.at(i);
        uint32_t part1 = num1.value.at(i);

        auto [und0, val_step0] = ui32_sub(part0, part1);
        auto [und1, val_step1] = ui32_sub(val_step0, underflow);

        result.value.at(i) = val_step1;

        auto [zero, underflow_new] = ui32_add(und0, und1);
        ASSERT(zero == 0);

        underflow = underflow_new;
    }

    if(underflow != 0){
        ERR("result of subtraction is < 0");
    }

    return result;
}

fp_t fp_mul_fp(const fp_t & num0, const fp_t & num1){

    fp_t result;

    for(ssize_t part0_idx=FP_VALUE_LEN-1; part0_idx>=0; --part0_idx){
        uint32_t part0 = num0.value.at(part0_idx);

        for(ssize_t part1_idx=FP_VALUE_LEN-1; part1_idx>=0; --part1_idx){
            uint32_t part1 = num1.value.at(part1_idx);

            ssize_t part_offset = part0_idx + part1_idx + 1;
            // TODO molq se tova da e pravilno
            // za6toto me murzi da go razigraq na list hartiq

            // if(part_offset - 1 >= FP_VALUE_LEN){
            //     // no point, we don't support this accuracy
            //     continue;
            // }

            if(part_offset - 1 < FP_VALUE_LEN){

                auto [ovf, val] = ui32_mul(part0, part1);

                fp_t adder;

                if(ovf != 0){

                    if(part_offset - 1 < 0){
                        ERR("Value >= 1.0");
                    }

                    adder.value.at(part_offset-1) = ovf;

                }

                if(part_offset < FP_VALUE_LEN){

                    adder.value.at(part_offset) = val;

                }

                result = fp_add_fp(result, adder);

            }

        }

    }

    return result;
}

bool fp_lt_fp(const fp_t & num0, const fp_t & num1){

    for(size_t idx=0; idx<FP_VALUE_LEN; ++idx){
        uint32_t part0 = num0.value.at(idx);
        uint32_t part1 = num1.value.at(idx);
        if(part0 < part1){
            return true;
        }else if(part0 > part1){
            return false;
        }
    }

    return false; // equal
}

bool fp_le_fp(const fp_t & num0, const fp_t & num1){

    for(size_t idx=0; idx<FP_VALUE_LEN; ++idx){
        uint32_t part0 = num0.value.at(idx);
        uint32_t part1 = num1.value.at(idx);
        if(part0 < part1){
            return true;
        }else if(part0 > part1){
            return false;
        }
    }

    return true; // equal
}

bool fp_eq_fp(const fp_t & num0, const fp_t & num1){

    for(size_t idx=0; idx<FP_VALUE_LEN; ++idx){
        uint32_t part0 = num0.value.at(idx);
        uint32_t part1 = num1.value.at(idx);
        if(part0 != part1){
            return false;
        }
    }

    return true;

}
