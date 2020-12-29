#include "prime_field.h"
#include "reed_solomon.h"
#include "reed_solomon_test.h"

int reed_solomon_test()
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
