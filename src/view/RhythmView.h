#pragma once

#include "interfaces/IView.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <string>

namespace gyaru {

/**
 * @brief View for the rhythm battle screen.
 */
class RhythmView final : public IView {
public:
    /**
     * @brief Creates the rhythm view and loads arrow/feedback textures.
     * @param[in] assetRoot Root directory that contains game assets.
     */
    explicit RhythmView(const std::string& assetRoot);

    /**
     * @brief Handles rhythm input and sends lane-hit commands to the controller.
     * @param[in] event SFML input event.
     * @param[in,out] controller Controller interface.
     */
    void handleEvent(const sf::Event& event, IController& controller) const override;

    /**
     * @brief Draws the current rhythm battle.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void render(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const override;

private:
    /**
     * @brief Draws score, combo, and social HUD text.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void drawHud(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const;

    /**
     * @brief Draws stationary receptor arrows and lane backgrounds.
     * @param[in,out] window Render target window.
     * @param[in] laneX Horizontal positions for each rhythm lane.
     * @param[in] laneColors Display colors for each rhythm lane.
     */
    void drawReceptors(sf::RenderWindow& window, const std::array<float, 4>& laneX, const std::array<sf::Color, 4>& laneColors) const;

    /**
     * @brief Draws pending falling notes from the active chart.
     * @param[in,out] window Render target window.
     * @param[in] model Read-only model interface.
     * @param[in] laneX Horizontal positions for each rhythm lane.
     * @param[in] laneColors Display colors for each rhythm lane.
     */
    void drawFallingNotes(sf::RenderWindow& window, const IModel& model, const std::array<float, 4>& laneX, const std::array<sf::Color, 4>& laneColors) const;

    /**
     * @brief Draws Perfect, Good, Too Early, or Miss feedback.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] lastHitText Last hit feedback label from the model.
     */
    void drawHitFeedback(sf::RenderWindow& window, const sf::Font& font, const std::string& lastHitText) const;

    /**
     * @brief Draws one arrow texture or fallback triangle.
     * @param[in,out] window Render target window.
     * @param[in] lane Zero-based rhythm lane index.
     * @param[in] x Left position of the arrow.
     * @param[in] y Top position of the arrow.
     * @param[in] size Width and height of the arrow area.
     * @param[in] fallbackColor Color used for the fallback triangle.
     * @param[in] tint Color tint applied to the arrow texture.
     */
    void drawArrow(sf::RenderWindow& window, int lane, float x, float y, float size, sf::Color fallbackColor, sf::Color tint = sf::Color::White) const;

    /** @brief Textures for each lane. */
    std::array<sf::Texture, 4> arrowTextures_; //!< Arrow textures for each lane.

    /** @brief Flags indicating whether each arrow texture is loaded. */
    std::array<bool, 4> hasArrowTexture_ = {false, false, false, false}; //!< Loaded flags for lane arrows.

    /** @brief Texture displayed for Perfect hits. */
    sf::Texture perfectTexture_; //!< Perfect feedback texture.

    /** @brief Texture displayed for Good hits. */
    sf::Texture goodTexture_; //!< Good feedback texture.

    /** @brief Texture displayed for Too Early hits. */
    sf::Texture tooEarlyTexture_; //!< Too Early feedback texture.

    /** @brief Whether the Perfect hit texture was successfully loaded. */
    bool hasPerfectTexture_ = false; //!< Whether Perfect texture loaded.

    /** @brief Whether the Good hit texture was successfully loaded. */
    bool hasGoodTexture_ = false; //!< Whether Good texture loaded.

    /** @brief Whether the Too Early hit texture was successfully loaded. */
    bool hasTooEarlyTexture_ = false; //!< Whether Too Early texture loaded.
};

} // namespace gyaru
