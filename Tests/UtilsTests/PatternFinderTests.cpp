#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <string_view>

#include <emmintrin.h>

#include <gtest/gtest.h>

#include <MemorySearch/BytePatternLiteral.h>
#include <MemorySearch/PatternFinder.h>

namespace
{

template <std::size_t N>
[[nodiscard]] constexpr auto createByteArray(const unsigned char(&bytes)[N])
{
    std::array<std::byte, N> arr;
    std::ranges::transform(bytes, arr.begin(), [](unsigned char c) { return std::byte{ c }; });
    return arr;
}

template <std::size_t N>
[[nodiscard]] constexpr auto createByteArray()
{
    std::array<std::byte, N> arr;
    arr.fill(std::byte{ 0xCC });
    return arr;
}

TEST(Utils_PatternFinderTest, ShortPatternCanBeFound) {
    constexpr auto bytes = createByteArray({ 0xA0, 0xA1, 0xA2 });
    EXPECT_EQ(PatternFinder{ bytes }("A1"_pat), &bytes[1]);
}

TEST(Utils_PatternFinderTest, ReturnsNullptrWhenByteBufferIsEmpty) {
    EXPECT_EQ(PatternFinder{ {} }("01 02 03 04"_pat), nullptr);
}

TEST(Utils_PatternFinderTest, ReturnsNullptrWhenPatternWasNotFound) {
    constexpr auto bytes = createByteArray({ 0xA0 });
    EXPECT_EQ(PatternFinder{ bytes }("AB CD EF"_pat), nullptr);
}

TEST(Utils_PatternFinderTest, PatternCanBeFoundWithSIMD) {
    auto bytes = createByteArray<100>();
    bytes[30] = std::byte{ 0xDE };
    bytes[41] = std::byte{ 0xDE };
    bytes[42] = std::byte{ 0xAD };
    bytes[43] = std::byte{ 0x00 };
    bytes[44] = std::byte{ 0xC0 };
    bytes[45] = std::byte{ 0xDE };
    
    EXPECT_EQ(PatternFinder{ bytes }("DE AD ? C0 DE"_pat), &bytes[41]);
}

}
