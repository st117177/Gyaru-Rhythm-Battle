#pragma once

#include "interfaces/IView.h"
#include "model/DialogueScene.h"

#include <SFML/Graphics.hpp>

#include <map>
#include <string>

namespace gyaru {

/**
 * @brief View for the dialogue screen.
 */
class DialogueView final : public IView {
public:
    /**
     * @brief Creates the dialogue view and loads portrait/background textures.
     * @param[in] assetRoot Root directory that contains game assets.
     */
    explicit DialogueView(const std::string& assetRoot);

    /**
     * @brief Handles dialogue input and sends commands to the controller.
     * @param[in] event SFML input event.
     * @param[in,out] controller Controller interface.
     */
    void handleEvent(const sf::Event& event, IController& controller) const override;

    /**
     * @brief Draws the current dialogue scene.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void render(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const override;

private:
    /**
     * @brief Draws portrait panel and character mood caption.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void drawCharacterPanel(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const;

    /**
     * @brief Draws speaker name and dialogue text.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void drawDialogueBox(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const;

    /**
     * @brief Draws selectable dialogue choices.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void drawChoices(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const;

    /**
     * @brief Draws relationship, reputation, and drama values.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void drawSocialStats(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const;

    /**
     * @brief Draws the dialogue background.
     * @param[in,out] window Render target window.
     */
    void drawBackground(sf::RenderWindow& window) const;

    /**
     * @brief Draws the current character portrait.
     * @param[in,out] window Render target window.
     * @param[in] scene Dialogue scene that selects the portrait key.
     */
    void drawPortrait(sf::RenderWindow& window, const DialogueScene& scene) const;

    /**
     * @brief Draws a placeholder portrait when assets are missing.
     * @param[in,out] window Render target window.
     */
    void drawFallbackPortrait(sf::RenderWindow& window) const;

    std::map<std::string, sf::Texture> portraits_; //!< Portrait textures by portrait key.
    sf::Texture background_;                       //!< Dialogue background texture.
    bool hasBackground_ = false;                   //!< Whether the background loaded.
};

} // namespace gyaru
