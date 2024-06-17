
// TODO
//
// add tests for all functions

#include "fixed-point.hpp"

#include <cstdint>
#include <iostream>
#include <bitset>
#include <sstream>

int main(){

    // {
    //     uint32_t a = 4294967295;
    //     uint32_t b = 4294967295;
    //     auto [c_ovf, c_val] = ui32_add(a, b);
    //     cout << "c_ovf:" << c_ovf << " c_val:" << c_val << endl;
    // }

    // {
    //     uint32_t a = 1;
    //     uint32_t b = 2;
    //     auto [c_udf, c_val] = ui32_sub(a, b);
    //     cout << "c_udf:" << c_udf << " c_val:" << c_val << endl;
    // }

    // {
    //     uint32_t a = 4294967295;
    //     uint32_t b = 4294967295;
    //     auto [c_ovf, c_val] = ui32_mul(a, b);
    //     cout << "c_ovf:" << c_ovf << " c_val:" << c_val << endl;
    // }

    // {
    //     fp_t a = fp_create(1);

    //     cout << "a: ";
    //     fp_print(a);
    //     cout << endl;

    //     fp_t b = fp_create(25);

    //     cout << "b: ";
    //     fp_print(b);
    //     cout << endl;

    //     fp_t c = fp_add_fp(a, b);

    //     cout << "c: ";
    //     fp_print(c);
    //     cout << endl;
    // }

    // {
    //     fp_t a = fp_create(4294967295);

    //     cout << "a: ";
    //     fp_print(a);
    //     cout << endl;

    //     fp_t b = fp_create(4294967295);

    //     cout << "b: ";
    //     fp_print(b);
    //     cout << endl;

    //     fp_t c = fp_add_fp(a, b);

    //     cout << "c: ";
    //     fp_print(c);
    //     cout << endl;
    // }

    // {
    //     cout << endl;
    //     uint32_t a = 5;
    //     uint32_t b = 3;
    //     auto [c_big, c_smo] = ui32_div(a, b);
    //     cout << "c_big:" << c_big << " c_smo:" << c_smo << endl;
    // }

    {
        cout << endl;
        fp_t a = fp_create_from_double(0.5);
        cout << "a:";
        fp_print(a);
        cout << endl;
    }

    {
        cout << endl;
        fp_t a = fp_create_from_double(0.0);
        cout << "a:";
        fp_print(a);
        cout << endl;
    }

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.5);
        cout << "a:";
        fp_print(a);
        cout << endl;

        fp_t b = fp_create_from_double(0.25);
        cout << "b:";
        fp_print(b);
        cout << endl;

        fp_t c = fp_sub_fp(a, b);
        cout << "c = a - b = ";
        fp_print(c);
        cout << endl;
    }

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.5);
        cout << "a:";
        fp_print(a);
        cout << endl;

        fp_t b = fp_create_from_double(0.5);
        cout << "b:";
        fp_print(b);
        cout << endl;

        fp_t c = fp_mul_fp(a, b);
        cout << "c = a * b = ";
        fp_print(c);
        cout << endl;

        for(int i=0; i<3; ++i){
            c = fp_mul_fp(c, b);
        }
        cout << "... ";
        fp_print(c);
        cout << endl;
    }

    {
        cout << endl;

        fp_t a;
        cout << "a:";
        fp_print(a);
        cout << endl;
    }

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.85);
        cout << "a:";
        fp_print(a);
        cout << endl;

        fp_t b = fp_create_from_double(0.7214);
        cout << "b:";
        fp_print(b);
        cout << endl;

        cout << "a < b: " << fp_lt_fp(a, b) << endl;
    }

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.4);
        cout << "a:";
        fp_print(a);
        cout << endl;
        cout << "first bit: " << fp_check_first_bit(a) << endl;
    }

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.4);
        cout << "a <<= 0 ; ";
        fp_print(a);
        cout << endl;
        
        fp_left_shift_by_1(a);
        cout << "a <<= 1 ; ";
        fp_print(a);
        cout << endl;

        fp_left_shift_by_1(a);
        cout << "a <<= 1 ; ";
        fp_print(a);
        cout << endl;

    }

    {
        cout << endl;

        fp_t a = fp_create_from_double(1.0);
        cout << "a:";
        fp_print(a);
        cout << endl;

        for(uint32_t part : a.value){
            if( (~part) != 0){
                return 1;
            }
        }
    }

    // test: fp_check_first_byte

    {
        fp_t a = fp_create_from_double(1.0);
        if(fp_check_first_byte(a) != static_cast<char>(0xffff)){
            return 1;
        }
    }

    {
        fp_t a = fp_create_from_double(0.0);
        if(fp_check_first_byte(a) != static_cast<char>(0x0000)){
            return 1;
        }
    }

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.5);
        cout << "a:";
        fp_print(a);
        cout << endl;

        char first_byte = fp_check_first_byte(a);

        bitset<8> binary(first_byte);
        cout << "first byte: " << binary << endl;

        if(first_byte != static_cast<char>(0x80)){
            return 1;
        }
    }

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.75);
        cout << "a:";
        fp_print(a);
        cout << endl;

        char first_byte = fp_check_first_byte(a);

        bitset<8> binary(first_byte);
        cout << "first byte: " << binary << endl;

        if(first_byte != static_cast<char>(0xc0)){
            return 1;
        }
    }

    // test: fp_left_shift_by_8

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.0001);
        cout << "a <<= 0 : ";
        fp_print(a);
        cout << endl;

        fp_left_shift_by_8(a);
        cout << "a <<= 8 : ";
        fp_print(a);
        cout << endl;

        // left shifting by 8 is equal to multiplying by 2**8
        fp_t b = fp_create_from_double(0.0256);
        cout << "b  =    : ";
        fp_print(b);
        cout << endl;

        if(!fp_eq_fp(a, b)){
            return 1;
        }
    }

    // test: fp_write_significant_to_file

    {
        {
            ofstream file;
            file.open("/tmp/fp-test", ios::binary);
            if(!file.is_open()){
                return 1;
            }

            fp_t a = fp_create_from_double(0.5);
            fp_write_significant_to_file(a, file);
        }

        {
            ifstream file;
            file.open("/tmp/fp-test", ios::binary);
            if(!file.is_open()){
                return 1;
            }

            stringstream buffer;
            buffer << file.rdbuf();

            string data = buffer.str();

            char expected[] = {static_cast<char>(0x80), 0};
            if(data != expected){
                return 1;
            }
        }
    }

    {
        {
            ofstream file;
            file.open("/tmp/fp-test", ios::binary);
            if(!file.is_open()){
                return 1;
            }

            fp_t a = fp_create_from_double(0.25);
            fp_write_significant_to_file(a, file);
        }

        {
            ifstream file;
            file.open("/tmp/fp-test", ios::binary);
            if(!file.is_open()){
                return 1;
            }

            stringstream buffer;
            buffer << file.rdbuf();

            string data = buffer.str();

            char expected[] = {static_cast<char>(0x40), 0};
            if(data != expected){
                return 1;
            }
        }
    }

    // testing: fp_set_8_least_significant_bits

    {
        cout << endl;

        fp_t a;
        cout << "a: ";
        fp_print(a);
        cout << endl;

        fp_set_8_least_significant_bits(a, 0x69);
        cout << "a: ";
        fp_print(a);
        cout << endl;

        if(a.value.at(FP_VALUE_LEN-1) != 0x69){
            return 1;
        }
    }

    // testing: fp_left_shift_by_up_to_31

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.000432432);
        cout << "a: ";
        fp_print(a);
        cout << endl;

        fp_t b = a;
        fp_left_shift_by_up_to_31(b, 3);
        cout << "b: ";
        fp_print(b);
        cout << endl;

        fp_t c = fp_create_from_double(0.000432432 * 2 * 2 * 2);
        cout << "c: ";
        fp_print(c);
        cout << endl;

        if(!fp_eq_fp(b, c)){
            return 1;
        }
    }

    // testing: fp_gobble_as_much_as_possible_from_file

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.454325346354);
        fp_t b;

        {
            ofstream file;
            file.open("/tmp/fp-test", ios::binary);
            if(!file.is_open()){
                return 1;
            }
            
            cout << "a: ";
            fp_print(a);
            cout << endl;

            fp_write_significant_to_file(a, file);
        }

        {
            ifstream file;
            file.open("/tmp/fp-test", ios::binary);
            if(!file.is_open()){
                return 1;
            }

            fp_gobble_as_much_as_possible_from_file(b, file);

            cout << "b: ";
            fp_print(b);
            cout << endl;
        }

        if(!fp_eq_fp(a, b)){
            return 1;
        }
    }

    // testing: fp_raw_lt_ui32

    {
        cout << endl;

        fp_t a;
        a.value.at(FP_VALUE_LEN-1) = 12345;

        if(!fp_raw_lt_ui32(a, 54321)){
            return 1;
        }

        a.value.at(FP_VALUE_LEN-2) = 1;

        cout << "a:";
        fp_print(a);
        cout << endl;

        if(fp_raw_lt_ui32(a, 54321)){
            return 1;
        }
    }

    return 0;
}
