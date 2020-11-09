#pragma once
#include <array>

const size_t FRAGMENT_SIZE = 5;
const size_t CHUNK_SIZE = 10;

using Fragment = std::array<int, FRAGMENT_SIZE>;
using Chunk = std::array<Fragment, CHUNK_SIZE>;