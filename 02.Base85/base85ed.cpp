#include <cstdint>
#include "base85ed.h"

std::vector<uint8_t> base85::encode(std::vector<uint8_t> const &bytes)
{
    return std::vector<uint8_t>(bytes);
}

std::vector<uint8_t> base85::decode(std::vector<uint8_t> const &b85str)
{
    return std::vector<uint8_t>(b85str);
}
