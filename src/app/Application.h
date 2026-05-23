#pragma once

#include "interfaces/IController.h"
#include "interfaces/IModel.h"
#include "interfaces/IView.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>

namespace gyaru {

/**
 * @brief Main application object that owns the SFML loop and wires MVC objects together.
 */
class Application {
public:
    /**
     * @brief Creates the application and concrete MVC implementations.
     * @param[in] assetRoot Root directory that contains game assets.
     */
    explicit Application(std::string assetRoot);

    /**
     * @brief Starts the game loop.
     * @return Process exit code.
     */
    int run();

private:
    /**
     * @brief Polls SFML events and sends input to the active view.
     */
    void processEvents();

    /**
     * @brief Runs one frame of game-state updates through the controller.
     */
    void update();

    /**
     * @brief Draws the active view and displays the frame.
     */
    void render();

    /**
     * @brief Loads the shared UI font.
     * @return true when a usable font was loaded.
     */
    bool loadFont();

    /**
     * @brief Selects the view that matches the current scene in the model.
     * @return Active screen view.
     */
    const IView& activeView() const;

    std::string assetRoot_;                   //!< Root directory for game assets.
    sf::RenderWindow window_;                 //!< Main SFML game window.
    sf::Font font_;                           //!< Shared UI font.
    std::unique_ptr<IModel> model_;           //!< Game model.
    std::unique_ptr<IController> controller_; //!< Game controller.
    std::unique_ptr<IView> menuView_;         //!< Main menu view.
    std::unique_ptr<IView> dialogueView_;     //!< Dialogue view.
    std::unique_ptr<IView> rhythmView_;       //!< Rhythm battle view.
    std::unique_ptr<IView> resultView_;       //!< Result screen view.
};

} // namespace gyaru
