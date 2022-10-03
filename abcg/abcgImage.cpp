/**
 * @file abcgImage.cpp
 * @brief Definition of texture loading helper functions.
 *
 * This file is part of ABCg (https://github.com/hbatagelo/abcg).
 *
 * @copyright (c) 2021--2022 Harlen Batagelo. All rights reserved.
 * This project is released under the MIT License.
 */

#include "abcgImage.hpp"

#include <cmath>
#include <cppitertools/itertools.hpp>
#include <gsl/gsl>

#include <iterator>
#include <span>
#include <vector>

/**
 * @brief Flips an image horizontally.
 *
 * Reverses each row of the image, in place.
 *
 * @param surface Pointer to the SDL surface of a RGB or RGBA image.
 */
void abcg::flipHorizontally(gsl::not_null<SDL_Surface *> const surface) {
  auto const bytesPerPixel{gsl::narrow<int>(surface->format->BytesPerPixel)};
  auto const widthInBytes{gsl::narrow<std::size_t>(surface->w * bytesPerPixel)};
  auto const height{gsl::narrow<std::size_t>(surface->h)};
  std::span const pixels{static_cast<std::byte *>(surface->pixels),
                         widthInBytes * height};

  // Row of pixels for the swap
  std::vector<std::byte> pixelRow(gsl::narrow<std::size_t>(widthInBytes),
                                  std::byte{});

  SDL_LockSurface(surface);

  // For each row
  for (auto const rowIndex : iter::range(height)) {
    auto const rowStart{widthInBytes * rowIndex};
    auto const rowEnd{rowStart + widthInBytes};
    // For each pixel (RGB/RGA) of current row
    auto srcBegin{pixels.begin() + gsl::narrow<long>(rowEnd)};
    auto dstBegin{pixelRow.begin()};
    for ([[maybe_unused]] auto const pixelIndex : iter::range(surface->w)) {
      srcBegin -= bytesPerPixel;
      std::copy(srcBegin, srcBegin + bytesPerPixel, dstBegin);
      dstBegin += bytesPerPixel;
    }
    std::copy(pixelRow.begin(),
              pixelRow.begin() + gsl::narrow<long>(widthInBytes),
              pixels.subspan(rowStart).begin());
  }

  SDL_UnlockSurface(surface);
}

/**
 * @brief Flips an image vertically.
 *
 * Reverses each column of the image, in place.
 *
 * @param surface Pointer to the SDL surface of a RGB or RGBA image.
 */
void abcg::flipVertically(gsl::not_null<SDL_Surface *> const surface) {
  auto const bytesPerPixel{gsl::narrow<int>(surface->format->BytesPerPixel)};
  auto const widthInBytes{gsl::narrow<std::size_t>(surface->w * bytesPerPixel)};
  auto const height{gsl::narrow<std::size_t>(surface->h)};
  std::span const pixels{static_cast<std::byte *>(surface->pixels),
                         gsl::narrow<std::size_t>(widthInBytes * height)};

  // Row of pixels for the swap
  std::vector<std::byte> pixelRow(widthInBytes, std::byte{});

  SDL_LockSurface(surface);

  // If height is odd, it doesn't need to swap the middle row
  for (auto const halfHeight{height / 2};
       auto const rowIndex : iter::range(halfHeight)) {
    auto const rowStartFromTop{widthInBytes * rowIndex};
    auto const rowStartFromBottom{widthInBytes * (height - rowIndex - 1)};

    auto const &topSpan{pixels.subspan(rowStartFromTop)};
    auto const &bottomSpan{pixels.subspan(rowStartFromBottom)};

    std::copy(topSpan.begin(),
              topSpan.begin() + gsl::narrow<long>(widthInBytes),
              pixelRow.begin());
    std::copy(bottomSpan.begin(),
              bottomSpan.begin() + gsl::narrow<long>(widthInBytes),
              topSpan.begin());
    std::copy(pixelRow.begin(),
              pixelRow.begin() + gsl::narrow<long>(widthInBytes),
              bottomSpan.begin());
  }

  SDL_UnlockSurface(surface);
}