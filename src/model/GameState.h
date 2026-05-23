#pragma once

#include <string>

namespace gyaru {

/**
 * @brief Game screens used by the application.
 */
enum class SceneType {
    Menu,        //!< Main menu screen.
    PreDialogue, //!< Dialogue screen before the rhythm battle.
    Battle,      //!< Rhythm battle screen.
    Result       //!< Final result screen.
};

/**
 * @brief Social state for the rival character.
 */
struct CharacterProfile {
    std::string name = "Yumi";              //!< Character display name.
    std::string mood = "Watching closely"; //!< Short text describing current mood.
    int relationship = 0;                  //!< Relationship score with the player.
    int dramaRisk = 0;                     //!< Risk level for negative drama outcomes.
};

/**
 * @brief Result produced after a rhythm battle.
 */
struct BattleOutcome {
    std::string grade = "F";      //!< Final rhythm grade.
    int relationshipDelta = 0;    //!< Relationship change from the battle.
    int reputationDelta = 0;      //!< Reputation change from the battle.
    std::string message;          //!< Result message shown on the final screen.
};

/**
 * @brief Mutable state shared by model, controller, and views.
 */
struct GameState {
    SceneType scene = SceneType::Menu;             //!< Current screen.
    CharacterProfile rival;                        //!< Social state for the rival character.
    int reputation = 50;                           //!< Player reputation score.
    int dialogueChoiceIndex = 0;                   //!< Currently selected dialogue choice.
    int dialogueStep = 0;                          //!< Current dialogue stage.
    std::string currentPortrait = "normal";        //!< Portrait key currently selected.
    int score = 0;                                 //!< Current battle score.
    int highScore = 0;                             //!< Best saved battle score.
    int combo = 0;                                 //!< Current hit combo.
    int bestCombo = 0;                             //!< Best combo reached in battle.
    int hits = 0;                                  //!< Number of successful hits.
    int misses = 0;                                //!< Number of missed notes.
    std::string lastChoiceText;                    //!< Text of the last selected dialogue choice.
    BattleOutcome lastOutcome;                     //!< Final outcome from the last battle.
};

/**
 * @brief Calculates final battle outcome from score and misses.
 * @param[in] score Player score.
 * @param[in] maxScore Maximum possible chart score.
 * @param[in] misses Number of missed notes.
 * @return Battle outcome with grade and social deltas.
 */
BattleOutcome makeBattleOutcome(int score, int maxScore, int misses);

/**
 * @brief Applies battle outcome to social state.
 * @param[in,out] state Game state to modify.
 * @param[in] outcome Outcome to apply.
 */
void applyBattleOutcome(GameState& state, const BattleOutcome& outcome);

/**
 * @brief Clamps relationship, reputation, and drama values to valid ranges.
 * @param[in,out] state Game state to clamp.
 */
void clampSocialStats(GameState& state);

} // namespace gyaru
