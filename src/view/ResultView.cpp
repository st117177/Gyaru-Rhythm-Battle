#include "view/ResultView.h"

#include "interfaces/IController.h"
#include "interfaces/IModel.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace gyaru {

namespace {

sf::String utf8(const std::string& text) {
    return sf::String::fromUtf8(text.begin(), text.end());
}

void drawText(sf::RenderWindow& window, const sf::Font& font, const std::string& text, unsigned int size, float x, float y, sf::Color color) {
    sf::Text label(utf8(text), font, size);
    label.setFillColor(color);
    label.setPosition(x, y);
    window.draw(label);
}

std::vector<std::string> wrapWords(const std::string& text, std::size_t maxChars) {
    std::istringstream words(text);
    std::vector<std::string> lines;
    std::string word;
    std::string line;

    while (words >> word) {
        if (!line.empty() && line.size() + 1 + word.size() > maxChars) {
            lines.push_back(line);
            line.clear();
        }
        if (!line.empty()) {
            line += ' ';
        }
        line += word;
    }

    if (!line.empty()) {
        lines.push_back(line);
    }
    return lines;
}

void drawWrappedText(sf::RenderWindow& window, const sf::Font& font, const std::string& text, unsigned int size, float x, float y, sf::Color color, std::size_t maxChars) {
    const auto lines = wrapWords(text, maxChars);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        drawText(window, font, lines[i], size, x, y + static_cast<float>(i) * (size + 8.f), color);
    }
}

} // namespace

ResultView::ResultView(const std::string& assetRoot) {
    const std::map<std::string, std::string> files = {
        {"S", assetRoot + "/charts/result/s.png"},
        {"A", assetRoot + "/charts/result/a.png"},
        {"B", assetRoot + "/charts/result/b.png"},
        {"C", assetRoot + "/charts/result/c.png"},
        {"D", assetRoot + "/charts/result/d.png"}
    };

    for (const auto& entry : files) {
        sf::Texture texture;
        if (texture.loadFromFile(entry.second)) {
            texture.setSmooth(false);
            gradeTextures_.emplace(entry.first, std::move(texture));
        }
    }

    const std::map<std::string, std::string> backgroundFiles = {
        {"S", assetRoot + "/charts/backgrounds/S.jpg"},
        {"A", assetRoot + "/charts/backgrounds/S.jpg"},
        {"B", assetRoot + "/charts/backgrounds/tie.jpg"},
        {"C", assetRoot + "/charts/backgrounds/C.jpg"},
        {"D", assetRoot + "/charts/backgrounds/lose.jpg"}
    };

    for (const auto& entry : backgroundFiles) {
        sf::Texture texture;
        if (texture.loadFromFile(entry.second)) {
            texture.setSmooth(false);
            backgroundTextures_.emplace(entry.first, std::move(texture));
        }
    }
}

void ResultView::handleEvent(const sf::Event& event, IController& controller) const {
    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        controller.requestClose();
    } else if (event.key.code == sf::Keyboard::Enter) {
        controller.resetToMenu();
    }
}

void ResultView::render(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    drawBackground(window, model);
    drawPanel(window);
    drawText(window, font, "BATTLE RESULT", 42, 292, 105, sf::Color(138, 0, 94));
    drawGrade(window, font, model);
    drawBattleStats(window, font, model);
    drawSocialStats(window, font, model);
    drawOutcomeMessage(window, font, model);
}

void ResultView::drawBackground(sf::RenderWindow& window, const IModel& model) const {
    const auto& state = model.state();
    const auto backgroundTexture = backgroundTextures_.find(state.lastOutcome.grade);
    if (backgroundTexture == backgroundTextures_.end()) {
        window.clear(sf::Color(76, 230, 214));
        return;
    }

    window.clear(sf::Color::Black);
    sf::Sprite background(backgroundTexture->second);
    const auto bounds = background.getLocalBounds();
    const float scale = std::max(static_cast<float>(window.getSize().x) / bounds.width, static_cast<float>(window.getSize().y) / bounds.height);
    background.setScale(scale, scale);
    background.setPosition((static_cast<float>(window.getSize().x) - bounds.width * scale) * 0.5f, (static_cast<float>(window.getSize().y) - bounds.height * scale) * 0.5f);
    window.draw(background);

    sf::RectangleShape colorWash({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
    colorWash.setFillColor(sf::Color(76, 230, 214, 45));
    window.draw(colorWash);
}

void ResultView::drawPanel(sf::RenderWindow& window) const {
    sf::RectangleShape panel({780.f, 430.f});
    panel.setPosition(90.f, 80.f);
    panel.setFillColor(sf::Color(255, 255, 255, 225));
    panel.setOutlineThickness(6.f);
    panel.setOutlineColor(sf::Color::Black);
    window.draw(panel);
}

void ResultView::drawGrade(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    const auto& state = model.state();
    const auto gradeTexture = gradeTextures_.find(state.lastOutcome.grade);
    if (gradeTexture == gradeTextures_.end()) {
        drawText(window, font, "Grade " + state.lastOutcome.grade, 38, 392, 175, sf::Color::Black);
        return;
    }

    sf::Sprite grade(gradeTexture->second);
    const auto bounds = grade.getLocalBounds();
    const float scale = 120.f / std::max(bounds.width, bounds.height);
    grade.setScale(scale, scale);
    grade.setPosition(420.f, 160.f);
    window.draw(grade);
}

void ResultView::drawBattleStats(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    const auto& state = model.state();
    std::ostringstream stats;
    stats << "Score " << state.score << "   Hits " << state.hits << "   Misses " << state.misses << "   Best combo " << state.bestCombo;
    drawText(window, font, stats.str(), 22, 145, 246, sf::Color::Black);
    drawText(window, font, "High score " + std::to_string(state.highScore), 22, 145, 272, sf::Color(138, 0, 94));
}

void ResultView::drawSocialStats(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    const auto& state = model.state();
    std::ostringstream social;
    social << "Relationship " << state.rival.relationship << " ("
           << (state.lastOutcome.relationshipDelta >= 0 ? "+" : "") << state.lastOutcome.relationshipDelta
           << ")   Reputation " << state.reputation << " ("
           << (state.lastOutcome.reputationDelta >= 0 ? "+" : "") << state.lastOutcome.reputationDelta << ")";
    drawText(window, font, social.str(), 22, 145, 310, sf::Color::Black);
}

void ResultView::drawOutcomeMessage(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    const auto& state = model.state();
    drawText(window, font, "Yumi:", 26, 145, 356, sf::Color(138, 0, 94));
    drawWrappedText(window, font, state.lastOutcome.message, 20, 145, 402, sf::Color::Black, 62);
    drawText(window, font, "ENTER  return to title", 22, 342, 550, sf::Color::Black);
}

} // namespace gyaru
