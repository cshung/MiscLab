#pragma once
#include <vector>

template<class TField>
class reed_solomon_code
{
public:
    reed_solomon_code(int number_of_errors);
    std::vector<int> encode(std::vector<int> input);
    std::vector<int> decode(std::vector<int> input);
private:
    std::vector<int> compute_syndrome(std::vector<int> input);
    std::vector<int> find_error_locator_polynomial(std::vector<int> syndrome);
    std::vector<int> solve_error_in_frequency(std::vector<int> error_locator_polynomial, std::vector<int> syndrome);
    std::vector<int> solve_error_in_time(std::vector<int> error_in_frequency);
    std::vector<int> decode_with_error(std::vector<int> input, std::vector<int> error_in_time);
    void verify_circular_convolution(std::vector<int> error_locator_polynomial, std::vector<int> error_in_frequency);
    std::vector<int> generator;
    int number_of_errors;
};

#include "reed_solomon.inl"
