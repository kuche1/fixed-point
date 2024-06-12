
#include "fixed-point.hpp"

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

////// uint64_t uint32_t

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

fp_t fp_create(){
    return {};
}

fp_t fp_create_from_double(double double_value){

    if(double_value < 0.0){
        ERR("Value must be positive");
    }

    if(double_value > 1.0){
        ERR("Value must not be greater than 1.0");
    }

    fp_t num = fp_create();
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
        printf("%04x", piece);
    }
}

bool fp_set_bit(fp_t & num, size_t idx, bool value){

    size_t part_idx = 0;

    while(idx >= 8 * sizeof(num.value[0])){
        part_idx += 1;
        idx -= 8;
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

fp_t fp_add_fp(const fp_t & num0, const fp_t & num1){

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
        ERR("result of addition is >= 1.0");
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
