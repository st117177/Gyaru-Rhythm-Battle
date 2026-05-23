#pragma once

namespace gyaru {

/**
 * @brief Interface for game actions requested by views.
 */
class IController {
public:
    /**
     * @brief Destroys the controller interface.
     */
    virtual ~IController() = default;

    /** @brief Requests closing the game window. */
    virtual void requestClose() = 0;

    /** @brief Starts the pre-battle dialogue scene. */
    virtual void startDialogue() = 0;

    /** @brief Moves dialogue selection one option up. */
    virtual void selectPreviousDialogueChoice() = 0;

    /** @brief Moves dialogue selection one option down. */
    virtual void selectNextDialogueChoice() = 0;

    /** @brief Confirms the currently selected dialogue choice. */
    virtual void confirmDialogueChoice() = 0;

    /**
     * @brief Attempts to hit a note in the selected rhythm lane.
     * @param[in] lane Zero-based lane index.
     */
    virtual void pressBattleLane(int lane) = 0;

    /** @brief Resets the game flow back to the main menu. */
    virtual void resetToMenu() = 0;

    /** @brief Updates game logic for the current frame. */
    virtual void update() = 0;

    /** @brief Returns true when the controller has requested closing the game. */
    virtual bool wantsClose() const = 0;

    /** @brief Clears the close request flag. */
    virtual void clearCloseRequest() = 0;
};

} // namespace gyaru
