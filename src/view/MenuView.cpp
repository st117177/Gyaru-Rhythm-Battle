#include "view/MenuView.h"

#include "interfaces/IController.h"
#include "interfaces/IModel.h"
#include "view/TextureUtils.h"

#include <algorithm>
#include <array>
#include <string>

namespace gyaru {

namespace {

void drawText(sf::RenderWindow& window, const sf::Font& font, const sf::String& text, unsigned int size, float x, float y, sf::Color color) {
    sf::Text label(text, font, size);
    label.setFillColor(color);
    label.setPosition(x, y);
    window.draw(label);
}

void drawCenteredText(sf::RenderWindow& window, const sf::Font& font, const sf::String& text, unsigned int size, float centerX, float y, sf::Color color) {
    sf::Text label(text, font, size);
    label.setFillColor(color);
    const auto bounds = label.getLocalBounds();
    label.setPosition(centerX - bounds.width * 0.5f - bounds.left, y);
    window.draw(label);
}

} // namespace

MenuView::MenuView(const std::string& assetRoot) {
    hasLogo_ = loadTextureWithTransparentWhite(logoTexture_, assetRoot + "/charts/logo/gyaru_logo.png");
    if (hasLogo_) {
        logoTexture_.setSmooth(false);
    }

    hasDj_ = loadTextureWithTransparentWhite(djTexture_, assetRoot + "/charts/logo/djthing.png");
    if (hasDj_) {
        djTexture_.setSmooth(false);
    }

    hasMusicLogo_ = loadTextureWithTransparentWhite(musicLogoTexture_, assetRoot + "/charts/logo/musiclogo.png");
    if (hasMusicLogo_) {
        musicLogoTexture_.setSmooth(false);
    }

    hasPalm_ = loadTextureWithTransparentWhite(palmTexture_, assetRoot + "/charts/logo/palm.png");
    if (hasPalm_) {
        palmTexture_.setSmooth(false);
    }

    hasStar_ = loadTextureWithTransparentWhite(starTexture_, assetRoot + "/charts/logo/star.png");
    if (hasStar_) {
        starTexture_.setSmooth(false);
    }
}

void MenuView::handleEvent(const sf::Event& event, IController& controller) const {
    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        controller.requestClose();
    } else if (event.key.code == sf::Keyboard::Enter) {
        controller.startDialogue();
    }
}

void MenuView::render(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    const auto& state = model.state();

    window.clear(sf::Color(255, 236, 77));
    drawFrame(window);
    drawStars(window);
    drawDecorations(window);
    drawHighScore(window, font, state.highScore);
    drawStartPanel(window, font);
}

void MenuView::drawFrame(sf::RenderWindow& window) const {
    sf::RectangleShape topBar({960.f, 118.f});
    topBar.setFillColor(sf::Color(255, 88, 166));
    topBar.setOutlineThickness(5.f);
    topBar.setOutlineColor(sf::Color::Black);
    window.draw(topBar);

    sf::RectangleShape titlePanel({720.f, 320.f});
    titlePanel.setPosition(120.f, 120.f);
    titlePanel.setFillColor(sf::Color(76, 230, 214, 235));
    titlePanel.setOutlineThickness(6.f);
    titlePanel.setOutlineColor(sf::Color::Black);
    window.draw(titlePanel);
}

void MenuView::drawStars(sf::RenderWindow& window) const {
    const std::array<sf::Vector2f, 22> starPositions = {
        sf::Vector2f{28.f, 132.f}, {76.f, 72.f}, {166.f, 116.f}, {254.f, 54.f}, {354.f, 118.f},
        {454.f, 66.f}, {594.f, 106.f}, {700.f, 54.f}, {828.f, 94.f}, {908.f, 166.f},
        {42.f, 364.f}, {118.f, 432.f}, {224.f, 372.f}, {342.f, 458.f}, {518.f, 384.f},
        {650.f, 430.f}, {754.f, 356.f}, {862.f, 452.f}, {902.f, 540.f}, {544.f, 526.f},
        {420.f, 548.f}, {720.f, 526.f}
    };

    for (std::size_t i = 0; i < starPositions.size(); ++i) {
        if (hasStar_) {
            sf::Sprite star(starTexture_);
            const auto bounds = star.getLocalBounds();
            const float target = (i % 3 == 0) ? 46.f : ((i % 3 == 1) ? 34.f : 26.f);
            const float scale = target / std::max(bounds.width, bounds.height);
            star.setScale(scale, scale);
            star.setPosition(starPositions[i]);
            window.draw(star);
        } else {
            sf::CircleShape star(9.f);
            star.setFillColor(sf::Color::White);
            star.setPosition(starPositions[i]);
            window.draw(star);
        }
    }
}

void MenuView::drawDecorations(sf::RenderWindow& window) const {
    if (hasPalm_) {
        sf::Sprite palm(palmTexture_);
        const auto bounds = palm.getLocalBounds();
        const float scale = 330.f / std::max(bounds.width, bounds.height);
        palm.setScale(scale, scale);
        palm.setPosition(676.f, 138.f);
        window.draw(palm);
    }

    if (hasMusicLogo_) {
        sf::Sprite music(musicLogoTexture_);
        const auto bounds = music.getLocalBounds();
        const float scale = 128.f / std::max(bounds.width, bounds.height);
        music.setScale(scale, scale);
        music.setPosition(696.f, 24.f);
        window.draw(music);
    }

    if (hasLogo_) {
        sf::Sprite logo(logoTexture_);
        const auto bounds = logo.getLocalBounds();
        const float scale = 360.f / std::max(bounds.width, bounds.height);
        logo.setScale(scale, scale);
        logo.setPosition(
            480.f - bounds.width * scale * 0.5f,
            265.f - bounds.height * scale * 0.5f);
        window.draw(logo);
    }

    if (hasDj_) {
        sf::Sprite dj(djTexture_);
        const auto bounds = dj.getLocalBounds();
        const float scale = 690.f / bounds.width;
        dj.setScale(scale, scale);
        dj.setPosition(138.f, 382.f);
        window.draw(dj);
    }
}

void MenuView::drawHighScore(sf::RenderWindow& window, const sf::Font& font, int highScore) const {
    sf::RectangleShape scorePanel({190.f, 86.f});
    scorePanel.setPosition(722.f, 404.f);
    scorePanel.setFillColor(sf::Color(255, 255, 255, 225));
    scorePanel.setOutlineThickness(5.f);
    scorePanel.setOutlineColor(sf::Color(255, 88, 166));
    window.draw(scorePanel);

    drawCenteredText(window, font, "HIGH SCORE", 20, 817.f, 416.f, sf::Color::Black);
    drawCenteredText(window, font, std::to_string(highScore), 30, 817.f, 444.f, sf::Color(138, 0, 94));
}

void MenuView::drawStartPanel(sf::RenderWindow& window, const sf::Font& font) const {
    sf::RectangleShape startPanel({240.f, 104.f});
    startPanel.setPosition(42.f, 408.f);
    startPanel.setFillColor(sf::Color(255, 255, 255, 225));
    startPanel.setOutlineThickness(5.f);
    startPanel.setOutlineColor(sf::Color(255, 88, 166));
    window.draw(startPanel);

    drawText(window, font, "ENTER", 30, 70, 424, sf::Color(255, 88, 166));
    drawText(window, font, "start battle", 22, 70, 462, sf::Color(138, 0, 94));
    drawText(window, font, "ESC  quit", 18, 70, 492, sf::Color(255, 88, 166));
}

} // namespace gyaru
