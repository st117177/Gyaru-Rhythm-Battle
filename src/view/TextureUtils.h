#pragma once

#include <SFML/Graphics.hpp>

#include <cstddef>
#include <string>

namespace gyaru {

/**
 * @brief Loads a texture and makes white background regions transparent.
 * @param[out] texture Texture object that receives the loaded image.
 * @param[in] path Path to the source image file.
 * @param[in] largeInternalWhiteRegion Minimum size of internal white regions to erase.
 * @return true if the image was loaded and converted successfully.
 */
bool loadTextureWithTransparentWhite(
    sf::Texture& texture,
    const std::string& path,
    std::size_t largeInternalWhiteRegion = 0
);

} // namespace gyaru
