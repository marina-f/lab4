/*
    ===========================================================================
    Random number generating functions for BigInt
    ===========================================================================
*/

#ifndef BIG_INT_RANDOM_FUNCTIONS_HPP
#define BIG_INT_RANDOM_FUNCTIONS_HPP

#include <random>
#include <climits>
#include <string>

#include "BigInt.hpp"

// when the number of digits are not specified, a random value is used for it
// which is kept below the following:
const size_t MAX_RANDOM_LENGTH = 1000;


/*
    big_random (num_digits)
    -----------------------
    Returns a random BigInt with a specific number of digits.
*/

BigInt big_random(size_t num_digits = 0) {
	std::random_device rand_generator;      // true random number generator

    if (num_digits == 0)    // the number of digits were not specified
        // use a random number for it:
        num_digits = 1 + rand_generator() % MAX_RANDOM_LENGTH;

	std::string buf= "";
    // ensure that the first digit is non-zero
	buf += std::to_string(1 + rand_generator() % 9);

    while (buf.size() < num_digits)
		buf += std::to_string(rand_generator());
    if (buf.size() != num_digits)
		buf.erase(num_digits);   // erase extra digits



	BigInt big_rand(buf);
    return big_rand;
}


#endif  // BIG_INT_RANDOM_FUNCTIONS_HPP
