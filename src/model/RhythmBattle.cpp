#include "model/RhythmBattle.h"

#include "model/GameState.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace gyaru {

void RhythmBattle::start(SongChart chart) {
    chart_ = std::move(chart);

    for (auto& note : chart_.notes()) {
        note.state = NoteState::Pending;
    }

    score_ = 0;
    combo_ = 0;
    bestCombo_ = 0;
    hits_ = 0;
    misses_ = 0;
}

void RhythmBattle::update(int elapsedMs) {
    for (auto& note : chart_.notes()) {
        if (note.state == NoteState::Pending && elapsedMs - note.timeMs > missWindowMs_) {
            note.state = NoteState::Missed;
            combo_ = 0;
            ++misses_;
        }
    }
}

HitResult RhythmBattle::tryHit(int lane, int elapsedMs) {
    int closestAbsDelta = std::numeric_limits<int>::max();
    int signedDelta = 0;
    Note* closest = findClosestPendingNote(lane, elapsedMs, closestAbsDelta, signedDelta);

    if (closest == nullptr) {
        return {};
    }

    return registerHit(*closest, closestAbsDelta, signedDelta);
}

Note* RhythmBattle::findClosestPendingNote(int lane, int elapsedMs, int& closestAbsDelta, int& signedDelta) {
    Note* closest = nullptr;
    for (auto& note : chart_.notes()) {
        if (note.state != NoteState::Pending || note.lane != lane) {
            continue;
        }

        const int delta = elapsedMs - note.timeMs;
        const int absDelta = std::abs(delta);

        if (absDelta <= goodWindowMs_ && absDelta < closestAbsDelta) {
            closest = &note;
            closestAbsDelta = absDelta;
            signedDelta = delta;
        }
    }
    return closest;
}

HitResult RhythmBattle::registerHit(Note& note, int closestAbsDelta, int signedDelta) {
    note.state = NoteState::Hit;
    ++hits_;
    ++combo_;
    bestCombo_ = std::max(bestCombo_, combo_);

    const bool perfect = closestAbsDelta <= perfectWindowMs_;
    const int baseScore = perfect ? 1000 : 600;
    const int comboBonus = std::min(combo_ * 10, 250);
    const int scoreDelta = baseScore + comboBonus;
    score_ += scoreDelta;

    return {
        perfect ? HitRating::Perfect : HitRating::Good,
        scoreDelta,
        combo_,
        signedDelta
    };
}

int RhythmBattle::nextPendingDeltaForLane(int lane, int elapsedMs) const {
    int bestDelta = std::numeric_limits<int>::max();

    for (const auto& note : chart_.notes()) {
        if (note.state != NoteState::Pending || note.lane != lane) {
            continue;
        }

        const int delta = note.timeMs - elapsedMs;

        if (delta >= 0 && delta < bestDelta) {
            bestDelta = delta;
        }
    }

    return bestDelta;
}

bool RhythmBattle::isFinished(int elapsedMs) const {
    return !chart_.empty() && elapsedMs > chart_.lastNoteTimeMs() + finishPaddingMs_;
}

const SongChart& RhythmBattle::chart() const {
    return chart_;
}

int RhythmBattle::score() const {
    return score_;
}

int RhythmBattle::combo() const {
    return combo_;
}

int RhythmBattle::bestCombo() const {
    return bestCombo_;
}

int RhythmBattle::hits() const {
    return hits_;
}

int RhythmBattle::misses() const {
    return misses_;
}

int RhythmBattle::maxScore() const {
    return chart_.maxScore();
}

std::string RhythmBattle::grade() const {
    return makeBattleOutcome(score_, maxScore(), misses_).grade;
}

} // namespace gyaru
