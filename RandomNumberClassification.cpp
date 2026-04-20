#include "RandomNumberClassification.h"
#include <sstream>
struct NumberState {
    bool is_first_digit_one = false;
    bool is_containe_only_three_digit = false;
    uint32_t multi_result = 1;
};

NumberState analyze_number(uint32_t number)
{
    NumberState res;
    uint32_t carry = 0;
    uint32_t number_of_digit = 0;
    while (number)
    {
        carry = number % 10;
        res.multi_result = carry * res.multi_result;
        ++number_of_digit;
        number = number / 10;
    }

    res.is_first_digit_one = carry == 1;
    res.is_containe_only_three_digit = number_of_digit == 3;

    return res;
}

std::vector<uint32_t> generate_random_numbers(uint32_t n_elements)
{
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<uint32_t> distrib(1, 999);

    std::vector<uint32_t> res;
    res.reserve(n_elements);


    for (uint32_t i = 0; i < n_elements; ++i)
        res.emplace_back(distrib(gen));

    return res;
}

std::string print_random_number_classificatrion(uint32_t number)
{
    std::ostringstream  res;
    if (number % 2)
    {
        res << number << ":this is an odd number" << std::endl;
        return res.str();
    }
    
    NumberState number_state = analyze_number(number);
    if (number_state.is_first_digit_one)
    {
        std::vector<uint32_t> new_rand_nums = generate_random_numbers(number);
        res << number << ":start with 1 [original list:";
        for (auto const& new_num : new_rand_nums)
            res << " " << new_num;

        std::sort(new_rand_nums.begin(), new_rand_nums.end(), std::greater<uint32_t>());
        res << " Ordered list:";
        for (auto const& new_num : new_rand_nums)
            res << " " << new_num;

        res << std::endl;
    }
    else if (number_state.is_containe_only_three_digit)
    {
        res << number << ":the digit's multiplcation are equal to "
        << number_state.multi_result << std::endl;
    }
    else
    {
        res << number << ":this is number belongs to te others" << std::endl;
    }
    
    return res.str();
}