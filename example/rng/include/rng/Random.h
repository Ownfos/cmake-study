#ifndef RNG_RANDOM_H
#define RNG_RANDOM_H

#include <random>

namespace rng
{

class Random
{
public:
    Random();

    int RandInt(int begin, int end);

private:
    std::mt19937 m_generator;
};

} // namespace rng

#endif
