#include "model/DialogueScene.h"
#include "model/GameState.h"
#include "model/RhythmBattle.h"
#include "model/SongChart.h"

#include <cassert>
#include <iostream>

using namespace gyaru;

namespace {

SongChart tinyChart() {
    SongChart chart = SongChart::fallback();
    chart.notes().resize(2);
    chart.notes()[0] = {1000, 0, NoteState::Pending};
    chart.notes()[1] = {1400, 1, NoteState::Pending};
    return chart;
}

void testPerfectHitScoresAndCombos() {
    RhythmBattle battle;
    battle.start(tinyChart());

    const auto result = battle.tryHit(0, 1000);

    assert(result.rating == HitRating::Perfect);
    assert(battle.score() > 0);
    assert(battle.combo() == 1);
    assert(battle.bestCombo() == 1);
    assert(battle.hits() == 1);
    assert(battle.misses() == 0);
}

void testLateNoteBecomesMiss() {
    RhythmBattle battle;
    battle.start(tinyChart());

    battle.update(1300);

    assert(battle.combo() == 0);
    assert(battle.misses() == 1);
}

void testNextPendingDeltaForLane() {
    RhythmBattle battle;
    battle.start(tinyChart());

    assert(battle.nextPendingDeltaForLane(0, 800) == 200);
}

void testDialogueChoiceChangesSocialStats() {
    GameState state;
    const auto scene = DialogueScene::openingScene();

    scene.applyChoice(0, state);

    assert(state.rival.relationship == 2);
    assert(state.reputation == 51);
    assert(state.rival.dramaRisk == 0);
}

void testBattleOutcomeChangesSocialStats() {
    GameState state;
    const auto outcome = makeBattleOutcome(10000, 10000, 0);

    applyBattleOutcome(state, outcome);

    assert(outcome.grade == "S");
    assert(state.rival.relationship > 0);
    assert(state.reputation > 50);
}

} // namespace

int main() {
    testPerfectHitScoresAndCombos();
    testLateNoteBecomesMiss();
    testNextPendingDeltaForLane();
    testDialogueChoiceChangesSocialStats();
    testBattleOutcomeChangesSocialStats();

    std::cout << "All model tests passed.\n";
    return 0;
}
