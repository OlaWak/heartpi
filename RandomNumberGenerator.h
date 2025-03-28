#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

/**
 * @file RandomNumberGenerator.h
 * @brief Declaration of the RandomNumberGenerator class.
 *
 * This header file declares the RandomNumberGenerator class, which provides functionality to generate
 * random numbers within a specified range. It is useful for simulating sensor readings when actual sensor data
 * is unavailable.
 */

#include <random>

/**
 * @class RandomNumberGenerator
 * @brief Generates random numbers within a specified range.
 *
 * This class uses the Mersenne Twister random number engine (std::mt19937) along with a uniform real distribution
 * to generate random numbers. It is particularly useful for simulating sensor readings.
 *
 * @note The random engine is seeded using std::random_device to ensure a different sequence of random numbers
 * each time the program runs.
 * 
 * @author Sena Debian and Yosra Alim
 */
class RandomNumberGenerator {
private:
    std::mt19937 rng; //mersenne twister random nb generator engine :has  a wide range, basically we wont see the same numbers for a long time
    std::uniform_real_distribution<double> dist; //mimics sensor readings(uniform distribution)


/**
     * @brief Constructs a new RandomNumberGenerator object with a specified range.
     *
     * Initializes the random number generator engine with a random seed and sets up the uniform real distribution
     * with the provided minimum and maximum values.
     *
     * @param min The minimum value for the random number generation.
     * @param max The maximum value for the random number generation.
     */

public:
    RandomNumberGenerator(double min, double max); //constructor with range
    
    /**
     * @brief Generates a random number within the specified range.
     *
     * Uses the configured random engine and uniform distribution to produce a random number.
     *
     * @return double A random number within the range [min, max].
     */
    double generate(); //generates the nbwithin a range
};

#endif
