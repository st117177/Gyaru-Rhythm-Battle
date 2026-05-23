#pragma once

#include "interfaces/IView.h"

#include <SFML/Graphics.hpp>

#include <map>
#include <string>

namespace gyaru {

/**
 * @brief View for the final battle result screen.
 */
class ResultView final : public IView {
public:
    /**
     * @brief Creates the result view and loads grade/background textures.
     * @param[in] assetRoot Root directory that contains game assets.
     */
    explicit ResultView(const std::string& assetRoot);

    /**
     * @brief Handles result-screen input.
     * @param[in] event SFML input event.
     * @param[in,out] controller Controller interface.
     */
    void handleEvent(const sf::Event& event, IController& controller) const override;

    /**
     * @brief Draws the battle result screen.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void render(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const override;

private:
    /**
     * @brief Draws grade-dependent background art.
     * @param[in,out] window Render target window.
     * @param[in] model Read-only model interface.
     */
    void drawBackground(sf::RenderWindow& window, const IModel& model) const;

    /**
     * @brief Draws the central result panel.
     * @param[in,out] window Render target window.
     */
    void drawPanel(sf::RenderWindow& window) const;

    /**
     * @brief Draws the final grade badge.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void drawGrade(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const;

    /**
     * @brief Draws score, hits, misses, and high score.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void drawBattleStats(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const;

    /**
     * @brief Draws relationship and reputation changes.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void drawSocialStats(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const;

    /**
     * @brief Draws final outcome dialogue text.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    void drawOutcomeMessage(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const;

    /** @brief Textures for grade badges (S, A, B, C, D, F). */
    std::map<std::string, sf::Texture> gradeTextures_; //!< Grade badge textures by grade.
    /** @brief Textures for grade-dependent background art. */
    std::map<std::string, sf::Texture> backgroundTextures_; //!< Result backgrounds by grade.
};

} // namespace gyaru
