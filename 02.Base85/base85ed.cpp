#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "base85ed.h"

static const char ALPHABET[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "!#$%&()*+-;<=>?@^_`{|}~";

// TODO: implement this in C++
std::vector<uint8_t> base85::encode(std::vector<uint8_t> const &bytes)
{
    static const uint32_t pow85[5] =
    {
        52200625u,
        614125u,
        7225u,
        85u,
        1u
    };

    std::vector<uint8_t> out;
    size_t i = 0;
    const size_t len = bytes.size();

    while (i + 4 <= len)
    {
        uint32_t n = (uint32_t(bytes[i])     << 24)
                     | (uint32_t(bytes[i + 1]) << 16)
                     | (uint32_t(bytes[i + 2]) <<  8)
                     |  uint32_t(bytes[i + 3]);

        for (int p = 0; p < 5; ++p)
        {
            int digit = static_cast<int>(n / pow85[p]);
            n = n % pow85[p];
            out.push_back(static_cast<uint8_t>(ALPHABET[digit]));
        }
        i += 4;
    }

    if (i < len)
    {
        size_t r = len - i;
        uint8_t b0 = bytes[i];
        uint8_t b1 = (r >= 2) ? bytes[i + 1] : 0;
        uint8_t b2 = (r >= 3) ? bytes[i + 2] : 0;
        uint8_t b3 = 0;
        uint32_t n = (uint32_t(b0) << 24)
                     | (uint32_t(b1) << 16)
                     | (uint32_t(b2) <<  8)
                     |  uint32_t(b3);
        int k = static_cast<int>(r + 1);

        for (int p = 0; p < k; ++p)
        {
            int digit = static_cast<int>(n / pow85[p]);
            n = n % pow85[p];
            out.push_back(static_cast<uint8_t>(ALPHABET[digit]));
        }
    }
    return out;

}

// TODO: implement this in C++
std::vector<uint8_t> base85::decode(std::vector<uint8_t> const &b85str)
{
    static int FR[256];
    static bool ready = false;

    if (!ready)
    {
        for (int i = 0; i < 256; ++i)
            FR[i] = -1;
        for (int i = 0; i < 85; ++i)
            FR[static_cast<unsigned char>(ALPHABET[i])] = i;
        ready = true;
    }

    std::vector<uint8_t> s = b85str;
    size_t pad = (5 - s.size() % 5) % 5;

    for (size_t k = 0; k < pad; ++k)
        s.push_back(static_cast<uint8_t>(ALPHABET[84]));

    std::vector<uint8_t> out;

    for (size_t i = 0; i < s.size(); i += 5)
    {
        uint32_t n = 0;
        for (int j = 0; j < 5; ++j)
        {
            int d = FR[static_cast<unsigned char>(s[i + j])];
            if (d < 0)
                return out;
            n = n * 85 + static_cast<uint32_t>(d);
        }

        out.push_back(static_cast<uint8_t>((n >> 24) & 0xFF));
        out.push_back(static_cast<uint8_t>((n >> 16) & 0xFF));
        out.push_back(static_cast<uint8_t>((n >>  8) & 0xFF));
        out.push_back(static_cast<uint8_t>( n        & 0xFF));
    }

    if (pad > 0)
        out.resize(out.size() - pad);

    return out;

}