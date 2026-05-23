#pragma once

#include "model/SongChart.h"

#include <string>

namespace gyaru {

/**
 * @brief Hit quality for one rhythm input.
 */
enum class HitRating {
    None,    //!< No valid hit.
    Good,    //!< Good timing hit.
    Perfect  //!< Perfect timing hit.
};

/**
 * @brief Result of one rhythm-lane input.
 */
struct HitResult {
    HitRating rating = HitRating::None; //!< Hit quality.
    int scoreDelta = 0;                 //!< Score gained from this input.
    int comboAfter = 0;                 //!< Combo value after this input.
    int timingDeltaMs = 0;              //!< Timing error in milliseconds.
};

/**
 * @brief Rhythm battle model with note judging and score tracking.
 */
class RhythmBattle {
public:
    /**
     * @brief Starts a battle with the provided chart.
     * @param[in] chart Chart to play.
     */
    void start(SongChart chart);

    /**
     * @brief Updates missed notes using elapsed battle time.
     * @param[in] elapsedMs Battle time in milliseconds.
     */
    void update(int elapsedMs);

    /**
     * @brief Attempts to hit the closest pending note in a lane.
     * @param[in] lane Zero-based lane index.
     * @param[in] elapsedMs Battle time in milliseconds.
     * @return Hit result.
     */
    HitResult tryHit(int lane, int elapsedMs);

    /**
     * @brief Returns time until the next pending note in a lane.
     * @param[in] lane Zero-based lane index.
     * @param[in] elapsedMs Battle time in milliseconds.
     * @return Positive delta in milliseconds or a large value when no note exists.
     */
    int nextPendingDeltaForLane(int lane, int elapsedMs) const;

    /**
     * @brief Returns true when the chart has ended.
     * @param[in] elapsedMs Battle time in milliseconds.
     */
    bool isFinished(int elapsedMs) const;

    /** @brief Returns the active chart. */
    const SongChart& chart() const;

    /** @brief Returns current score. */
    int score() const;

    /** @brief Returns current combo. */
    int combo() const;

    /** @brief Returns best combo reached in this battle. */
    int bestCombo() const;

    /** @brief Returns number of hit notes. */
    int hits() const;

    /** @brief Returns number of missed notes. */
    int misses() const;

    /** @brief Returns maximum possible score for this battle. */
    int maxScore() const;

    /** @brief Returns current grade estimate. */
    std::string grade() const;

private:
    /**
     * @brief Finds the closest hittable pending note in a lane.
     * @param[in] lane Zero-based lane index.
     * @param[in] elapsedMs Battle time in milliseconds.
     * @param[out] closestAbsDelta Absolute timing delta for the closest note.
     * @param[out] signedDelta Signed timing delta for the closest note.
     */
    Note* findClosestPendingNote(int lane, int elapsedMs, int& closestAbsDelta, int& signedDelta);

    /**
     * @brief Marks a note as hit and applies score/combo changes.
     * @param[in,out] note Note to mark as hit.
     * @param[in] closestAbsDelta Absolute timing delta for the hit note.
     * @param[in] signedDelta Signed timing delta for the hit note.
     */
    HitResult registerHit(Note& note, int closestAbsDelta, int signedDelta);

    static constexpr int perfectWindowMs_ = 70;   //!< Max timing error for Perfect.
    static constexpr int goodWindowMs_ = 140;     //!< Max timing error for Good.
    static constexpr int missWindowMs_ = 180;     //!< Delay after which pending notes miss.
    static constexpr int finishPaddingMs_ = 1200; //!< Extra time before battle finishes.

    SongChart chart_;  //!< Active rhythm chart.
    int score_ = 0;    //!< Current score.
    int combo_ = 0;    //!< Current combo.
    int bestCombo_ = 0; //!< Best combo reached.
    int hits_ = 0;     //!< Successful hit count.
    int misses_ = 0;   //!< Missed note count.
};

} // namespace gyaru
