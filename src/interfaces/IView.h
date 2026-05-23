#pragma once

#include <SFML/Graphics.hpp>

namespace gyaru {

class IController;
class IModel;

/**
 * @brief Interface for a drawable and input-aware game screen.
 */
class IView {
public:
    /**
     * @brief Destroys the view interface.
     */
    virtual ~IView() = default;

    /**
     * @brief Handles one SFML event and sends commands to the controller.
     * @param[in] event Event received from the SFML window.
     * @param[in,out] controller Controller interface used for game actions.
     */
    virtual void handleEvent(const sf::Event& event, IController& controller) const = 0;

    /**
     * @brief Draws the screen using data from the model.
     * @param[in,out] window Render target window.
     * @param[in] font Shared UI font.
     * @param[in] model Read-only model interface.
     */
    virtual void render(sf::RenderWindow& window, const sf::Font& font, const IModel& model) const = 0;
};

} // namespace gyaru
