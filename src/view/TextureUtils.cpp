#include "view/TextureUtils.h"

#include <vector>

namespace gyaru {

namespace {

bool isWhite(const sf::Image& image, unsigned int x, unsigned int y) {
    const sf::Color pixel = image.getPixel(x, y);
    return pixel.r >= 245 && pixel.g >= 245 && pixel.b >= 245;
}

void erasePixels(sf::Image& image, const std::vector<sf::Vector2u>& region) {
    for (const auto& point : region) {
        sf::Color pixel = image.getPixel(point.x, point.y);
        pixel.a = 0;
        image.setPixel(point.x, point.y, pixel);
    }
}

void tryPushWhitePixel(
    const sf::Image& image,
    std::vector<unsigned char>& visited,
    std::vector<sf::Vector2u>& stack,
    unsigned int x,
    unsigned int y
) {
    const auto size = image.getSize();
    const std::size_t index = static_cast<std::size_t>(y) * size.x + x;
    if (!visited[index] && isWhite(image, x, y)) {
        visited[index] = 1;
        stack.push_back({x, y});
    }
}

std::vector<sf::Vector2u> collectWhiteRegion(
    const sf::Image& image,
    std::vector<unsigned char>& visited,
    unsigned int startX,
    unsigned int startY,
    bool& touchesBorder
) {
    const auto size = image.getSize();
    std::vector<sf::Vector2u> region;
    std::vector<sf::Vector2u> stack;
    tryPushWhitePixel(image, visited, stack, startX, startY);

    while (!stack.empty()) {
        const auto point = stack.back();
        stack.pop_back();
        region.push_back(point);
        touchesBorder = touchesBorder || point.x == 0 || point.y == 0 || point.x + 1 == size.x || point.y + 1 == size.y;

        if (point.x > 0) tryPushWhitePixel(image, visited, stack, point.x - 1, point.y);
        if (point.x + 1 < size.x) tryPushWhitePixel(image, visited, stack, point.x + 1, point.y);
        if (point.y > 0) tryPushWhitePixel(image, visited, stack, point.x, point.y - 1);
        if (point.y + 1 < size.y) tryPushWhitePixel(image, visited, stack, point.x, point.y + 1);
    }

    return region;
}

void eraseWhiteRegions(sf::Image& image, std::size_t largeInternalWhiteRegion) {
    const auto size = image.getSize();
    std::vector<unsigned char> visited(static_cast<std::size_t>(size.x) * size.y, 0);

    for (unsigned int y = 0; y < size.y; ++y) {
        for (unsigned int x = 0; x < size.x; ++x) {
            const std::size_t index = static_cast<std::size_t>(y) * size.x + x;
            if (visited[index] || !isWhite(image, x, y)) {
                continue;
            }

            bool touchesBorder = false;
            const auto region = collectWhiteRegion(image, visited, x, y, touchesBorder);
            if (touchesBorder || (largeInternalWhiteRegion > 0 && region.size() >= largeInternalWhiteRegion)) {
                erasePixels(image, region);
            }
        }
    }
}

} // namespace

bool loadTextureWithTransparentWhite(sf::Texture& texture, const std::string& path, std::size_t largeInternalWhiteRegion) {
    sf::Image image;
    if (!image.loadFromFile(path)) {
        return false;
    }

    eraseWhiteRegions(image, largeInternalWhiteRegion);
    return texture.loadFromImage(image);
}

} // namespace gyaru
