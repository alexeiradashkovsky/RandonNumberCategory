#include "RandomNumberClassification.h"
#include <sstream>
struct NumberState {
    bool starts_with_one = false;
    bool contains_three_digits = false;
    uint32_t digits_multiplication = 1;
};

NumberState analyze_number(uint32_t number)
{
    NumberState res;
    uint32_t carry = 0;
    uint32_t number_of_digit = 0;
    while (number)
    {
        carry = number % 10;
        res.digits_multiplication = carry * res.digits_multiplication;
        ++number_of_digit;
        number = number / 10;
    }

    res.starts_with_one = carry == 1;
    res.contains_three_digits = number_of_digit == 3;

    return res;
}

std::vector<uint32_t> generate_random_numbers(uint32_t n_elements)
{
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<uint32_t> distrib(1, 999);

    std::vector<uint32_t> res;
    res.reserve(n_elements);


    for (uint32_t i = 0; i < n_elements; ++i)
        res.emplace_back(distrib(gen));

    return res;
}

std::string print_random_number_classification(uint32_t number)
{
    std::ostringstream  res;
    if (number % 2)
    {
        res << number << ": this is an odd number\n";
        return res.str();
    }
    
    NumberState number_state = analyze_number(number);
    if (number_state.starts_with_one)
    {
        std::vector<uint32_t> new_rand_nums = generate_random_numbers(number);
        res << number << ": starts with 1 [origin list:";
        for (auto const& new_num : new_rand_nums)
            res << " " << new_num;

        std::sort(new_rand_nums.begin(), new_rand_nums.end());
        res << " Ordered list:";
        for (auto const& new_num : new_rand_nums)
            res << " " << new_num;

        res << "]\n";
    }
    else if (number_state.contains_three_digits)
    {
        res << number << ": the digit's multiplications are equal to "
        << number_state.digits_multiplication << "\n";
    }
    else
    {
        res << number << ": this number belongs to the others\n";
    }
    
    return res.str();
}