#include <gtest/gtest.h>

#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string.h>

#include "base85ed.h"

const std::vector<std::pair<const char *, const char * >> short_cases =
{
    { "",     ""     },
    { "F#",   "1"    },
    { "F){",  "12"   },
    { "F)}j", "123"  },
    { "F)}kW","1234" }
};

static std::vector<uint8_t> cstr2v(const char *s)
{
    return std::vector<uint8_t>(
               s,
               s + std::string(s).size()
           );
}

// Тесты encode

TEST(Base85Encode, Empty)
{
    EXPECT_EQ(base85::encode(cstr2v("")), cstr2v(""));
}

TEST(Base85Encode, ShortCasesFromTable)
{
    for (const auto &p : short_cases)
    {
        EXPECT_EQ(base85::encode(cstr2v(p.second)), cstr2v(p.first));
    }
}

TEST(Base85Encode, OneByte)
{
    EXPECT_EQ(base85::encode(cstr2v("1")), cstr2v("F#"));
}

TEST(Base85Encode, TwoBytes)
{
    EXPECT_EQ(base85::encode(cstr2v("12")), cstr2v("F){"));
}

TEST(Base85Encode, ThreeBytes)
{
    EXPECT_EQ(base85::encode(cstr2v("123")), cstr2v("F)}j"));
}

TEST(Base85Encode, FourBytesFullBlock)
{
    EXPECT_EQ(base85::encode(cstr2v("1234")), cstr2v("F)}kW"));
}

TEST(Base85Encode, TwoBlocks)
{
    auto enc = base85::encode(cstr2v("12345678"));
    EXPECT_EQ(enc.size(), 10u);
}

TEST(Base85Encode, StringWithZeros)
{
    std::vector<uint8_t> raw = { 'a', 0, 0, 'z' };
    auto enc = base85::encode(raw);
    EXPECT_FALSE(enc.empty());
}

// Тесты decode

TEST(Base85Decode, Empty)
{
    EXPECT_EQ(base85::decode(cstr2v("")), cstr2v(""));
}

TEST(Base85ShortsDecode, TrivialShortDecodes)
{
    for (const auto &p : short_cases)
    {
        EXPECT_EQ(base85::decode(cstr2v(p.first)), cstr2v(p.second));
    }
}

TEST(Base85Decode, OneCharTail)
{
    EXPECT_EQ(base85::decode(cstr2v("F#")), cstr2v("1"));
}

TEST(Base85Decode, TwoCharsTail)
{
    EXPECT_EQ(base85::decode(cstr2v("F){")), cstr2v("12"));
}

TEST(Base85Decode, ThreeCharsTail)
{
    EXPECT_EQ(base85::decode(cstr2v("F)}j")), cstr2v("123"));
}

TEST(Base85Decode, FiveCharsFullBlock)
{
    EXPECT_EQ(base85::decode(cstr2v("F)}kW")), cstr2v("1234"));
}

TEST(Base85Decode, TwoBlocks)
{
    auto enc = base85::encode(cstr2v("12345678"));
    EXPECT_EQ(base85::decode(enc), cstr2v("12345678"));
}

TEST(Base85Decode, StringWithZeros)
{
    std::vector<uint8_t> raw = { 'a', 0, 0, 'z' };
    auto enc = base85::encode(raw);
    EXPECT_EQ(base85::decode(enc), raw);
}
