#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iomanip>
#include <numeric>

constexpr auto WIDTH = 25u, HEIGHT = 6u;

auto part1(std::vector<std::vector<int>> const& image) -> void
{
  auto layer = std::min_element(
    std::cbegin(image), std::cend(image), 
    [](auto const& lhs, auto const& rhs)
  {
    return std::count(std::cbegin(lhs), std::cend(lhs), 0) 
      < std::count(std::cbegin(rhs), std::cend(rhs), 0);
  });

  auto const countOf1 = std::count(std::cbegin(*layer), std::cend(*layer), 1);
  auto const countOf2 = std::count(std::cbegin(*layer), std::cend(*layer), 2);
  std::cout << "Part1: " << countOf1 * countOf2 << '\n';
}

auto part2(std::vector<std::vector<int>> const& image) -> void
{
  auto const finalImage = std::accumulate(
    std::next(std::crbegin(image)), std::crend(image), image.back(), 
    [](auto &&prev, auto const& curr)
    {
      std::transform(
        std::cbegin(curr), std::cend(curr), std::cbegin(prev), std::begin(prev),
        [](auto pix, auto prevpix)
        {
          return pix == 2/* transparent */ ? prevpix : pix;
        });

      return std::move(prev);
    });

  for (auto y = 0u; y < HEIGHT; ++y) {
    for (auto x = 0u; x < WIDTH; ++x)
      std::cout << (finalImage[y * WIDTH + x] == 1 ? u8"██" : "  ");
    std::cout << '\n';
  }
}

auto main() -> int
{
  std::ifstream input("input.txt");

  auto beg = std::istream_iterator<char>(input);
  auto end = std::istream_iterator<char>();
  auto const groupSize = WIDTH * HEIGHT;

  auto groupCount = 0u;
  std::vector<std::vector<int>> image;
  
  for (auto x = beg; x != end; ++x) {
    if (groupCount++ % groupSize == 0)
      image.emplace_back();
    image.back().emplace_back(*x - '0');
  }
  
  part1(image);
  part2(image);
}
