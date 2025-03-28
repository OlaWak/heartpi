/**
 * @file RandomNumberGenerator.cpp
 * @brief Implements the RandomNumberGenerator class for generating random numbers.
 *
 * This file defines the constructor and member function for the RandomNumberGenerator class,
 * which generates random numbers within a specified range using a uniform distribution.
 * @author Sena Debian and Yosra Alim
 */
#include "RandomNumberGenerator.h"

/**
 * @brief Constructs a new RandomNumberGenerator object.
 *
 * Initializes the random number engine with a random seed and sets up the distribution
 * with the provided minimum and maximum values.
 *
 * @param min The minimum value for the random number generation.
 * @param max The maximum value for the random number generation.
 *
 */
RandomNumberGenerator::RandomNumberGenerator(double min, double max)
// Constructor: Initializes the random engine with a random seed and sets up the distribution.
    : rng(std::random_device{}()), dist(min, max) {}// rng:random nb engine

/**
 * @brief Generates a random number within the specified range.
 *
 * Uses the preconfigured distribution and random number engine to produce and return
 * a random number.
 *
 * @return double A randomly generated number.
 */
double RandomNumberGenerator::generate() {
    return dist(rng); //produces and returns a random number
}
