#include "view/DialogueView.h"

#include "interfaces/IController.h"
#include "interfaces/IModel.h"
#include "view/TextureUtils.h"

#include <algorithm>
#include <sstream>
#include <string>
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

std::vector<std::string> wrapWords(const sf::Font& font, const std::string& text, unsigned int size, float maxWidth) {
    std::istringstream words(text);
    std::vector<std::string> lines;
    std::string word;
    std::string line;

    while (words >> word) {
        const std::string candidate = line.empty() ? word : line + " " + word;
        sf::Text probe(utf8(candidate), font, size);
        if (!line.empty() && probe.getLocalBounds().width > maxWidth) {
            lines.push_back(line);
            line = word;
        } else {
            line = candidate;
        }
    }

    if (!line.empty()) {
        lines.push_back(line);
    }
    return lines;
}

void drawWrappedText(sf::RenderWindow& window, const sf::Font& font, const std::string& text, unsigned int size, float x, float y, sf::Color color, float maxWidth, float lineGap = 5.f) {
    const auto lines = wrapWords(font, text, size, maxWidth);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        drawText(window, font, lines[i], size, x, y + static_cast<float>(i) * (size + lineGap), color);
    }
}

} // namespace

DialogueView::DialogueView(const std::string& assetRoot) {
    const std::map<std::string, std::string> files = {
        {"normal", assetRoot + "/charts/yumi/yuminormal.png"},
        {"angry", assetRoot + "/charts/yumi/yumi_angry.png"},
        {"angry_happy", assetRoot + "/charts/yumi/yumi_angry_happy.png"},
        {"pointing", assetRoot + "/charts/yumi/yumi_pointingfinger.png"}
    };

    for (const auto& entry : files) {
        sf::Texture texture;
        if (loadTextureWithTransparentWhite(texture, entry.second, 2400)) {
            texture.setSmooth(false);
            portraits_.emplace(entry.first, std::move(texture));
        }
    }

    hasBackground_ = background_.loadFromFile(assetRoot + "/charts/backgrounds/dorm_1.png");
    if (hasBackground_) {
        background_.setSmooth(false);
    }
}

void DialogueView::handleEvent(const sf::Event& event, IController& controller) const {
    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        controller.requestClose();
    } else if (event.key.code == sf::Keyboard::Up) {
        controller.selectPreviousDialogueChoice();
    } else if (event.key.code == sf::Keyboard::Down) {
        controller.selectNextDialogueChoice();
    } else if (event.key.code == sf::Keyboard::Enter) {
        controller.confirmDialogueChoice();
    }
}

void DialogueView::render(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    drawBackground(window);
    drawCharacterPanel(window, font, model);
    drawDialogueBox(window, font, model);
    drawChoices(window, font, model);
    drawSocialStats(window, font, model);
}

void DialogueView::drawCharacterPanel(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    const auto& state = model.state();
    const auto& scene = model.dialogue();

    sf::RectangleShape portrait({270.f, 420.f});
    portrait.setPosition(35.f, 70.f);
    portrait.setFillColor(sf::Color(255, 134, 205, 235));
    portrait.setOutlineThickness(6.f);
    portrait.setOutlineColor(sf::Color::Black);
    window.draw(portrait);

    drawPortrait(window, scene);

    sf::RectangleShape caption({236.f, 76.f});
    caption.setPosition(52.f, 392.f);
    caption.setFillColor(sf::Color(255, 255, 255, 220));
    caption.setOutlineThickness(3.f);
    caption.setOutlineColor(sf::Color::Black);
    window.draw(caption);

    drawText(window, font, "ЮМИ", 26, 62, 397, sf::Color::Black);
    drawWrappedText(window, font, state.rival.mood, 14, 62, 430, sf::Color::Black, 212.f, 3.f);
}

void DialogueView::drawDialogueBox(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    const auto& scene = model.dialogue();

    sf::RectangleShape box({600.f, 238.f});
    box.setPosition(330.f, 50.f);
    box.setFillColor(sf::Color(76, 230, 214, 240));
    box.setOutlineThickness(6.f);
    box.setOutlineColor(sf::Color::Black);
    window.draw(box);

    drawText(window, font, scene.speaker() + ":", 26, 356, 75, sf::Color(138, 0, 94));
    drawWrappedText(window, font, scene.line(), 18, 356, 120, sf::Color::Black, 540.f, 5.f);
}

void DialogueView::drawChoices(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    const auto& state = model.state();
    const auto& scene = model.dialogue();

    sf::RectangleShape choicePanel({600.f, 240.f});
    choicePanel.setPosition(330.f, 315.f);
    choicePanel.setFillColor(sf::Color(255, 255, 255, 240));
    choicePanel.setOutlineThickness(6.f);
    choicePanel.setOutlineColor(sf::Color::Black);
    window.draw(choicePanel);

    const auto& choices = scene.choices();
    for (std::size_t i = 0; i < choices.size(); ++i) {
        const bool selected = static_cast<int>(i) == state.dialogueChoiceIndex;
        sf::RectangleShape row({540.f, 82.f});
        row.setPosition(360.f, 340.f + static_cast<float>(i) * 98.f);
        row.setFillColor(selected ? sf::Color(255, 88, 166) : sf::Color(245, 245, 245));
        row.setOutlineThickness(3.f);
        row.setOutlineColor(sf::Color::Black);
        window.draw(row);

        drawWrappedText(window, font, choices[i].label, 17, 380, 353.f + static_cast<float>(i) * 98.f, sf::Color::Black, 500.f, 4.f);
    }
}

void DialogueView::drawSocialStats(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const {
    const auto& state = model.state();


    drawText(window, font, "Отношения " + std::to_string(state.rival.relationship) + "   Репутация " + std::to_string(state.reputation)
        + "   Риск драмы " + std::to_string(state.rival.dramaRisk), 17, 52, 530, sf::Color::Black);
}

void DialogueView::drawBackground(sf::RenderWindow& window) const {
    if (!hasBackground_) {
        window.clear(sf::Color(255, 236, 77));
        return;
    }

    window.clear(sf::Color::Black);
    sf::Sprite sprite(background_);
    const auto bounds = sprite.getLocalBounds();
    const float scale = std::max(
        static_cast<float>(window.getSize().x) / bounds.width,
        static_cast<float>(window.getSize().y) / bounds.height);
    sprite.setScale(scale, scale);
    sprite.setPosition(
        (static_cast<float>(window.getSize().x) - bounds.width * scale) * 0.5f,
        (static_cast<float>(window.getSize().y) - bounds.height * scale) * 0.5f);
    window.draw(sprite);

    sf::RectangleShape wash({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
    wash.setFillColor(sf::Color(255, 236, 77, 95));
    window.draw(wash);
}

void DialogueView::drawPortrait(sf::RenderWindow& window, const DialogueScene& scene) const {
    auto texture = portraits_.find(scene.portrait());
    if (texture == portraits_.end()) {
        texture = portraits_.find("normal");
    }

    if (texture == portraits_.end()) {
        drawFallbackPortrait(window);
        return;
    }

    sf::Sprite sprite(texture->second);
    const auto bounds = sprite.getLocalBounds();
    if (bounds.width <= 0.f || bounds.height <= 0.f) {
        drawFallbackPortrait(window);
        return;
    }

    const float scale = std::min(246.f / bounds.width, 306.f / bounds.height);
    sprite.setScale(scale, scale);
    sprite.setPosition(47.f + (246.f - bounds.width * scale) * 0.5f, 82.f + (306.f - bounds.height * scale) * 0.5f);
    window.draw(sprite);
}

void DialogueView::drawFallbackPortrait(sf::RenderWindow& window) const {
    sf::CircleShape face(63.f);
    face.setPosition(120.f, 150.f);
    face.setFillColor(sf::Color(255, 226, 194));
    face.setOutlineThickness(4.f);
    face.setOutlineColor(sf::Color::Black);
    window.draw(face);

    sf::RectangleShape hair({155.f, 70.f});
    hair.setPosition(103.f, 120.f);
    hair.setFillColor(sf::Color(251, 205, 51));
    hair.setOutlineThickness(4.f);
    hair.setOutlineColor(sf::Color::Black);
    window.draw(hair);
}

} // namespace gyaru
