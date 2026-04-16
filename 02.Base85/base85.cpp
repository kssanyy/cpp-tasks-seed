#include <iostream>
#include <vector>
#include <cstdint>

#include "base85ed.h"

std::vector<uint8_t> read_stdin_to_vector_iostream() {
    constexpr std::streamsize BUF_SIZE = 64 * 1024;
    std::vector<uint8_t> out;
    out.reserve(1024);
    std::vector<char> buf(BUF_SIZE);

    // Ensure std::cin is in binary mode where applicable (no-op on POSIX).
    std::ios::sync_with_stdio(false);

    while (std::cin) {
        std::cin.read(buf.data(), BUF_SIZE);
        std::streamsize n = std::cin.gcount();
        if (n > 0) {
            out.insert(out.end(), reinterpret_cast<uint8_t*>(buf.data()),
                       reinterpret_cast<uint8_t*>(buf.data() + n));
        }
        if (n < BUF_SIZE) {
            // either EOF or error
            break;
        }
    }
    return out;
}

void write_vector_to_stdout(const std::vector<uint8_t>& data) {
    // Ensure no tied flushing and faster IO (optional)
    std::ios::sync_with_stdio(false);
    std::cout.setf(std::ios::fmtflags(0), std::ios::basefield); // no formatting changes

    if (!data.empty()) {
        std::cout.write(reinterpret_cast<const char*>(data.data()), data.size());
    }
    // flush to ensure data is written out
    std::cout.flush();
}


int main(int argc, const char *argv[])
{
    auto data = read_stdin_to_vector_iostream();
    auto encoded = base85::encode(data);
    write_vector_to_stdout(encoded);
    return 0;
}
