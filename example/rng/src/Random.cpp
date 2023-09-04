#include "rng/Random.h"

namespace rng
{

Random::Random()
    : m_generator(std::random_device{}())
{}

int Random::RandInt(int begin, int end)
{
    return std::uniform_int_distribution(begin, end-1)(m_generator);
}

} // namespace rng
