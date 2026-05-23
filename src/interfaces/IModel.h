#pragma once

#include "model/DialogueScene.h"
#include "model/GameState.h"
#include "model/RhythmBattle.h"

#include <string>

namespace gyaru {

/**
 * @brief Interface for game state and game rules.
 */
class IModel {
public:
    /**
     * @brief Destroys the model interface.
     */
    virtual ~IModel() = default;

    /** @brief Returns the current global game state. */
    virtual const GameState& state() const = 0;

    /** @brief Returns the current dialogue scene. */
    virtual const DialogueScene& dialogue() const = 0;

    /** @brief Returns the current rhythm battle state. */
    virtual const RhythmBattle& battle() const = 0;

    /** @brief Returns elapsed battle time in milliseconds. */
    virtual int battleElapsedMs() const = 0;

    /** @brief Returns the last hit feedback text. */
    virtual const std::string& lastHitText() const = 0;

    /** @brief Switches from menu to dialogue. */
    virtual void startDialogue() = 0;

    /** @brief Moves dialogue selection upward. */
    virtual void selectPreviousDialogueChoice() = 0;

    /** @brief Moves dialogue selection downward. */
    virtual void selectNextDialogueChoice() = 0;

    /** @brief Applies the selected dialogue choice or starts the battle. */
    virtual void confirmDialogueChoice() = 0;

    /**
     * @brief Attempts a rhythm hit in the selected lane.
     * @param[in] lane Zero-based rhythm lane index.
     */
    virtual void pressBattleLane(int lane) = 0;

    /** @brief Updates model state for the current frame. */
    virtual void update() = 0;

    /** @brief Resets scene flow back to the main menu while keeping high score. */
    virtual void resetToMenu() = 0;
};

} // namespace gyaru
