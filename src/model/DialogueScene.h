#pragma once

#include "model/GameState.h"

#include <string>
#include <vector>

namespace gyaru {

/**
 * @brief One selectable dialogue answer and its social effects.
 */
struct DialogueChoice {
    std::string label;                   //!< Text shown for this choice.
    int relationshipDelta = 0;           //!< Relationship change after selecting it.
    int reputationDelta = 0;             //!< Reputation change after selecting it.
    int dramaRiskDelta = 0;              //!< Drama-risk change after selecting it.
    std::string response;                //!< Rival response text after selection.
    std::string nextPortrait = "normal"; //!< Portrait key used after selection.
};

/**
 * @brief Dialogue scene with speaker text, portrait key, and player choices.
 */
class DialogueScene {
public:
    /** @brief Creates an empty dialogue scene. */
    DialogueScene() = default;

    /**
     * @brief Creates a dialogue scene with text and choices.
     * @param[in] speaker Speaker name.
     * @param[in] line Dialogue line.
     * @param[in] portrait Portrait key.
     * @param[in] choices Available player choices.
     */
    DialogueScene(std::string speaker, std::string line, std::string portrait, std::vector<DialogueChoice> choices);

    /** @brief Returns speaker name. */
    const std::string& speaker() const;

    /** @brief Returns dialogue text. */
    const std::string& line() const;

    /** @brief Returns portrait key. */
    const std::string& portrait() const;

    /** @brief Returns available choices. */
    const std::vector<DialogueChoice>& choices() const;

    /**
     * @brief Applies selected choice effects to game state.
     * @param[in] index Selected choice index.
     * @param[in,out] state Game state to modify.
     */
    void applyChoice(std::size_t index, GameState& state) const;

    /** @brief Creates the opening dialogue scene. */
    static DialogueScene openingScene();

    /**
     * @brief Creates the second dialogue scene based on current social state.
     * @param[in] state Current game state.
     * @return Follow-up dialogue scene.
     */
    static DialogueScene secondScene(const GameState& state);

private:
    std::string speaker_;                 //!< Speaker name for this scene.
    std::string line_;                    //!< Dialogue line shown to the player.
    std::string portrait_;                //!< Portrait key for the current speaker image.
    std::vector<DialogueChoice> choices_; //!< Available player choices.
};

} // namespace gyaru
