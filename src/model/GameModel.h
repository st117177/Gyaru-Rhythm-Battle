#pragma once

#include "interfaces/IModel.h"

#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>

#include <string>

namespace gyaru {

/**
 * @brief Concrete model that owns all game state and game rules.
 */
class GameModel final : public IModel {
public:
    /**
     * @brief Creates the model and starts menu background music.
     * @param[in] assetRoot Root directory that contains game assets.
     */
    explicit GameModel(std::string assetRoot);

    /** @brief Returns the current global game state. */
    const GameState& state() const override;

    /** @brief Returns the current dialogue scene. */
    const DialogueScene& dialogue() const override;

    /** @brief Returns the active rhythm battle. */
    const RhythmBattle& battle() const override;

    /** @brief Returns elapsed battle time in milliseconds. */
    int battleElapsedMs() const override;

    /** @brief Returns last hit feedback text. */
    const std::string& lastHitText() const override;

    /** @brief Starts the pre-battle dialogue. */
    void startDialogue() override;

    /** @brief Moves dialogue selection one option up. */
    void selectPreviousDialogueChoice() override;

    /** @brief Moves dialogue selection one option down. */
    void selectNextDialogueChoice() override;

    /** @brief Applies selected dialogue choice or starts the battle. */
    void confirmDialogueChoice() override;

    /**
     * @brief Attempts to hit a rhythm note in a lane.
     * @param[in] lane Zero-based lane index.
     */
    void pressBattleLane(int lane) override;

    /** @brief Updates current model state. */
    void update() override;

    /** @brief Resets flow back to menu while preserving high score. */
    void resetToMenu() override;

private:
    /** @brief Loads chart/audio and switches to the battle scene. */
    void startBattle();

    /** @brief Calculates battle outcome and switches to the result scene. */
    void finishBattle();

    /** @brief Starts menu/result background music if available. */
    void startBackgroundMusic();

    /** @brief Stops menu/result background music. */
    void stopBackgroundMusic();

    std::string assetRoot_;              //!< Root directory for game assets.
    GameState state_;                    //!< Current shared game state.
    DialogueScene dialogue_;             //!< Active dialogue scene.
    RhythmBattle battle_;                //!< Active rhythm battle.
    sf::Clock battleClock_;              //!< Clock used to time battle notes.
    sf::Music music_;                    //!< Battle music stream.
    sf::Music backgroundMusic_;          //!< Menu/result background music stream.
    bool musicLoaded_ = false;           //!< Whether battle music was loaded.
    bool backgroundMusicLoaded_ = false; //!< Whether background music was loaded.
    std::string lastHitText_;            //!< Last rhythm feedback text.
};

} // namespace gyaru
