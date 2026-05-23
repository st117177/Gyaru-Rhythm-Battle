#include "model/GameState.h"

#include <algorithm>

namespace gyaru {

BattleOutcome makeBattleOutcome(int score, int maxScore, int misses) {
    const double ratio = maxScore > 0 ? static_cast<double>(score) / maxScore : 0.0;

    if (ratio >= 0.82 && misses <= 4) {
        return {"S", 6, 5, "Юми делает вид, что ей все равно, но просит не выкладывать ее поражение в школьный чат."};
    }
    if (ratio >= 0.62) {
        return {"A", 4, 3, "Юми фыркает: «Ладно, ритм у тебя есть. Но корону я еще не сдала»."};
    }
    if (ratio >= 0.40) {
        return {"B", 1, 0, "Ничья по вайбу. Все делают вид, что так и было задумано."};
    }
    if (ratio >= 0.22) {
        return {"C", -2, -3, "Юми улыбается слишком вежливо. Это худший вид улыбки в общаге."};
    }
    return {"D", -5, -7, "Ритм развалился, а Юми уже репетирует речь для коридорного суда."};
}

void applyBattleOutcome(GameState& state, const BattleOutcome& outcome) {
    state.lastOutcome = outcome;
    state.rival.relationship += outcome.relationshipDelta;
    state.reputation += outcome.reputationDelta;

    if (outcome.relationshipDelta < 0) {
        state.rival.dramaRisk += 2;
        state.rival.mood = "Готова устроить драму";
    } else if (outcome.relationshipDelta > 3) {
        state.rival.dramaRisk = std::max(0, state.rival.dramaRisk - 1);
        state.rival.mood = "Бесится, но уважает";
    } else {
        state.rival.mood = "Запоминает счет";
    }

    clampSocialStats(state);
}

void clampSocialStats(GameState& state) {
    state.reputation = std::clamp(state.reputation, 0, 100);
    state.rival.relationship = std::clamp(state.rival.relationship, -20, 20);
    state.rival.dramaRisk = std::clamp(state.rival.dramaRisk, 0, 10);
}

} // namespace gyaru
