#pragma once

#include "interfaces/IView.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace gyaru {

/**
 * @brief View for the main menu screen.
 */
class MenuView final : public IView {
public:
    /**
     * @brief Creates the menu view and loads menu textures.
     * @param[in] assetRoot Root directory that contains game assets.
     */
    explicit MenuView(const std::string& assetRoot);

    /**
     * @brief Handles menu input and sends commands to the controller.
     * @param[in] event SFML input event.
     * @param[in,out] controller Controller interface.
     */
    void handleEvent(const sf::Event& event, IController& controller) const override;

    /**
     * @brief Draws the main menu.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void render(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const override;

private:
    /**
     * @brief Draws menu background panels.
     * @param[in,out] window Render target window.
     */
    void drawFrame(sf::RenderWindow& window) const;

    /**
     * @brief Draws decorative stars.
     * @param[in,out] window Render target window.
     */
    void drawStars(sf::RenderWindow& window) const;

    /**
     * @brief Draws logo and decorative image assets.
     * @param[in,out] window Render target window.
     */
    void drawDecorations(sf::RenderWindow& window) const;

    /**
     * @brief Draws the high-score panel.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] highScore Current saved high score.
     */
    void drawHighScore(sf::RenderWindow& window, const sf::Font& font, int highScore) const;

    /**
     * @brief Draws start and quit instructions.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     */
    void drawStartPanel(sf::RenderWindow& window, const sf::Font& font) const;

    sf::Texture logoTexture_;      //!< Main logo texture.
    sf::Texture djTexture_;        //!< Decorative DJ texture.
    sf::Texture musicLogoTexture_; //!< Decorative music-logo texture.
    sf::Texture palmTexture_;      //!< Decorative palm texture.
    sf::Texture starTexture_;      //!< Decorative star texture.
    bool hasLogo_ = false;         //!< Whether the logo texture loaded.
    bool hasDj_ = false;           //!< Whether the DJ texture loaded.
    bool hasMusicLogo_ = false;    //!< Whether the music-logo texture loaded.
    bool hasPalm_ = false;         //!< Whether the palm texture loaded.
    bool hasStar_ = false;         //!< Whether the star texture loaded.
};

} // namespace gyaru
