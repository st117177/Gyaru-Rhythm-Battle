#include "view/RhythmView.h"

#include "interfaces/IController.h"
#include "interfaces/IModel.h"
#include "view/TextureUtils.h"

#include <array>
#include <cstddef>
#include <sstream>

namespace gyaru {

namespace {

constexpr float receptorY = 470.f;
// Larger values make arrows travel more slowly before they reach the target row.
constexpr float spawnLeadMs = 2400.f;
constexpr float spawnY = -560.f;
constexpr float laneWidth = 142.f;
constexpr float receptorArrowSize = 116.f;
constexpr float fallingArrowSize = 104.f;

void drawText(sf::RenderWindow& window, const sf::Font& font, const sf::String& text, unsigned int size, float x, float y, sf::Color color) {
    sf::Text label(text, font, size);
    label.setFillColor(color);
    label.setPosition(x, y);
    window.draw(label);
}

bool lanePressed(int lane) {
    if (lane == 0) {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    }
    if (lane == 1) {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    }
    if (lane == 2) {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    }
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

void drawLogoTexture(sf::RenderWindow& window, const sf::Texture& texture) {
    sf::Sprite sprite(texture);
    const auto bounds = sprite.getLocalBounds();
    const float scale = 210.f / std::max(bounds.width, bounds.height);
    sprite.setScale(scale, scale);
    sprite.setPosition(707.f, 132.f);
    window.draw(sprite);
}

} // namespace

RhythmView::RhythmView(const std::string& assetRoot) {
    const std::array<std::string, 4> files = {
        assetRoot + "/charts/Arrows/left.png",
        assetRoot + "/charts/Arrows/down.png",
        assetRoot + "/charts/Arrows/up.png",
        assetRoot + "/charts/Arrows/right.png"
    };

    for (std::size_t i = 0; i < files.size(); ++i) {
        hasArrowTexture_[i] = loadTextureWithTransparentWhite(arrowTextures_[i], files[i]);
        if (hasArrowTexture_[i]) {
            arrowTextures_[i].setSmooth(false);
        }
    }

    hasPerfectTexture_ = loadTextureWithTransparentWhite(perfectTexture_, assetRoot + "/charts/Arrows/perfect.png");
    if (hasPerfectTexture_) {
        perfectTexture_.setSmooth(false);
    }

    hasGoodTexture_ = loadTextureWithTransparentWhite(goodTexture_, assetRoot + "/charts/Arrows/good.png");
    if (hasGoodTexture_) {
        goodTexture_.setSmooth(false);
    }

    hasTooEarlyTexture_ = loadTextureWithTransparentWhite(tooEarlyTexture_, assetRoot + "/charts/Arrows/too_early.png");
    if (hasTooEarlyTexture_) {
        tooEarlyTexture_.setSmooth(false);
    }
}

void RhythmView::handleEvent(const sf::Event& event, IController& controller) const {
    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        controller.requestClose();
    } else if (event.key.code == sf::Keyboard::Left) {
        controller.pressBattleLane(0);
    } else if (event.key.code == sf::Keyboard::Down) {
        controller.pressBattleLane(1);
    } else if (event.key.code == sf::Keyboard::Up) {
        controller.pressBattleLane(2);
    } else if (event.key.code == sf::Keyboard::Right) {
        controller.pressBattleLane(3);
    }
}

void RhythmView::render(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    window.clear(sf::Color(34, 34, 44));

    const std::array<float, 4> laneX = {118.f, 292.f, 466.f, 640.f};
    const std::array<sf::Color, 4> laneColors = {
        sf::Color(255, 88, 166),
        sf::Color(76, 230, 214),
        sf::Color(255, 236, 77),
        sf::Color(175, 126, 255)
    };

    drawHud(window, font, model);
    drawReceptors(window, laneX, laneColors);
    drawFallingNotes(window, model, laneX, laneColors);
    drawHitFeedback(window, font, model.lastHitText());
    drawText(window, font, "Arrow keys only: \xE2\x86\x90  \xE2\x86\x93  \xE2\x86\x91  \xE2\x86\x92", 19, 618, 495, sf::Color::White);
}

void RhythmView::drawHud(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    const auto& state = model.state();

    sf::RectangleShape hud({960.f, 86.f});
    hud.setFillColor(sf::Color(255, 88, 166));
    hud.setOutlineThickness(4.f);
    hud.setOutlineColor(sf::Color::Black);
    window.draw(hud);

    std::ostringstream hudText;
    hudText << "Score " << state.score << "   Combo " << state.combo << "   Best " << state.bestCombo
            << "   Misses " << state.misses;
    drawText(window, font, hudText.str(), 22, 32, 24, sf::Color::Black);
    drawText(window, font, "Relationship " + std::to_string(state.rival.relationship) + "   Reputation " + std::to_string(state.reputation), 18, 640, 28, sf::Color::Black);
}

void RhythmView::drawReceptors(sf::RenderWindow& window, const std::array<float, 4>& laneX, const std::array<sf::Color, 4>& laneColors) const {
    for (int lane = 0; lane < 4; ++lane) {
        const bool pressed = lanePressed(lane);
        sf::RectangleShape laneBack({laneWidth, 410.f});
        laneBack.setPosition(laneX[lane], 112.f);
        laneBack.setFillColor(sf::Color(53, 53, 70, 130));
        laneBack.setOutlineThickness(4.f);
        laneBack.setOutlineColor(sf::Color::Black);
        window.draw(laneBack);

        sf::RectangleShape targetLine({laneWidth - 8.f, 5.f});
        targetLine.setPosition(laneX[lane] + 4.f, receptorY + 72.f);
        targetLine.setFillColor(pressed ? sf::Color(135, 135, 135) : laneColors[lane]);
        window.draw(targetLine);

        drawArrow(
            window,
            lane,
            laneX[lane] + (laneWidth - receptorArrowSize) * 0.5f,
            receptorY - 2.f,
            receptorArrowSize,
            laneColors[lane],
            pressed ? sf::Color(135, 135, 135) : sf::Color::White);
    }
}

void RhythmView::drawFallingNotes(sf::RenderWindow& window, const IModel& model, const std::array<float, 4>& laneX, const std::array<sf::Color, 4>& laneColors) const {
    const auto& battle = model.battle();
    const int elapsedMs = model.battleElapsedMs();

    for (const auto& note : battle.chart().notes()) {
        if (note.state != NoteState::Pending) {
            continue;
        }

        const float untilHit = static_cast<float>(note.timeMs - elapsedMs);
        if (untilHit > spawnLeadMs || untilHit < -220.f) {
            continue;
        }

        const float progress = 1.f - (untilHit / spawnLeadMs);
        const float y = spawnY + progress * (receptorY - spawnY);
        drawArrow(window, note.lane, laneX[note.lane] + (laneWidth - fallingArrowSize) * 0.5f, y, fallingArrowSize, laneColors[note.lane]);
    }
}

void RhythmView::drawHitFeedback(sf::RenderWindow& window, const sf::Font& font, const std::string& lastHitText) const {
    if (!lastHitText.empty()) {
        if (lastHitText == "PERFECT" && hasPerfectTexture_) {
            drawLogoTexture(window, perfectTexture_);
        } else if (lastHitText == "GOOD" && hasGoodTexture_) {
            drawLogoTexture(window, goodTexture_);
        } else if (lastHitText == "TOO EARLY" && hasTooEarlyTexture_) {
            drawLogoTexture(window, tooEarlyTexture_);
        } else {
            drawText(window, font, lastHitText, 42, 708, 180, sf::Color(255, 236, 77));
        }
    }
}

void RhythmView::drawArrow(sf::RenderWindow& window, int lane, float x, float y, float size, sf::Color fallbackColor, sf::Color tint) const {
    if (lane >= 0 && lane < 4 && hasArrowTexture_[static_cast<std::size_t>(lane)]) {
        sf::Sprite sprite(arrowTextures_[static_cast<std::size_t>(lane)]);
        const auto bounds = sprite.getLocalBounds();
        const float scale = size / std::max(bounds.width, bounds.height);
        sprite.setScale(scale, scale);
        sprite.setColor(tint);
        sprite.setPosition(x + (size - bounds.width * scale) * 0.5f, y + (size - bounds.height * scale) * 0.5f);
        window.draw(sprite);
        return;
    }

    sf::ConvexShape arrow;
    arrow.setPointCount(3);
    arrow.setFillColor(fallbackColor);
    arrow.setOutlineThickness(2.f);
    arrow.setOutlineColor(sf::Color::Black);

    if (lane == 0) {
        arrow.setPoint(0, {x, y + size * 0.5f});
        arrow.setPoint(1, {x + size, y});
        arrow.setPoint(2, {x + size, y + size});
    } else if (lane == 1) {
        arrow.setPoint(0, {x, y});
        arrow.setPoint(1, {x + size, y});
        arrow.setPoint(2, {x + size * 0.5f, y + size});
    } else if (lane == 2) {
        arrow.setPoint(0, {x + size * 0.5f, y});
        arrow.setPoint(1, {x, y + size});
        arrow.setPoint(2, {x + size, y + size});
    } else {
        arrow.setPoint(0, {x, y});
        arrow.setPoint(1, {x + size, y + size * 0.5f});
        arrow.setPoint(2, {x, y + size});
    }
    window.draw(arrow);
}

} // namespace gyaru
