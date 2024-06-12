
#include <cstdint>

#include "fixed-point.hpp"

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
        fp_t a = fp_create_from_double(1.0);
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

        fp_t a = fp_create_arbitrary(0, 5);
        cout << "a:";
        fp_print(a);
        cout << endl;
    }

    {
        cout << endl;

        fp_t a = fp_create_from_double(0.4);
        cout << "a:";
        fp_print(a);
        cout << endl;
        cout << "first bit: " << fp_check_first_bit(a) << endl;
    }

    return 0;
}
