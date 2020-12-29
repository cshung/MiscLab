#include "prime_field.h"
#include "galois_field.h"
#include "reed_solomon.h"
#include "reed_solomon_test.h"

int prime_field_test()
{
    reed_solomon_code<prime_field<7>> code(1);

    std::vector<int> input(4);

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            for (int k = 0; k < 7; k++)
            {
                for (int l = 0; l < 7; l++)
                {
                    input[0] = i;
                    input[1] = j;
                    input[2] = k;
                    input[3] = l;

                    auto encoded = code.encode(input);

                    for (int p1 = 0; p1 < 6; p1++)
                    {
                        for (int e1 = 0; e1 < 7; e1++)
                        {
                            int o1 = encoded[p1];
                            encoded[p1] = e1;
                            auto decoded = code.decode(encoded);
                            assert(input == decoded);
                            encoded[p1] = o1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


int galois_field_test()
{
    reed_solomon_code<galois_field<8, 285>> code(5);
    std::vector<int> input(245, 0);
    for (int iteration = 0; iteration < 50; iteration++)
    {
        for (int i = 0; i < 245; i++)
        {
            input[i] = rand() % 256;
        }
        auto encoded = code.encode(input);
        encoded[10] = 12;
        encoded[13] = 7;
        auto decoded = code.decode(encoded);
        assert(input == decoded);
    }
    return 0;
}

int reed_solomon_test()
{
    prime_field_test();
    galois_field_test();
    return 0;
}
