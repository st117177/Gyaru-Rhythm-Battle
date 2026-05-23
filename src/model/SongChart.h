#pragma once

#include <string>
#include <vector>

namespace gyaru {

/**
 * @brief Runtime state of one chart note.
 */
enum class NoteState {
    Pending, //!< Note has not been judged yet.
    Hit,     //!< Note was hit by the player.
    Missed   //!< Note was missed.
};

/**
 * @brief One timed note in a rhythm chart.
 */
struct Note {
    int timeMs = 0;                          //!< Hit time in milliseconds.
    int lane = 0;                            //!< Zero-based lane index.
    NoteState state = NoteState::Pending;    //!< Current note state.
};

/**
 * @brief Song chart loaded from a text file.
 */
class SongChart {
public:
    /**
     * @brief Loads notes from a chart file.
     * @param[in] path Path to the chart file.
     * @return true if at least one valid note was loaded.
     */
    bool loadFromFile(const std::string& path);

    /** @brief Returns read-only notes. */
    const std::vector<Note>& notes() const;

    /** @brief Returns mutable notes for battle logic. */
    std::vector<Note>& notes();

    /** @brief Returns the hit time of the last note. */
    int lastNoteTimeMs() const;

    /** @brief Returns maximum possible score for this chart. */
    int maxScore() const;

    /** @brief Returns true when the chart has no notes. */
    bool empty() const;

    /** @brief Creates a built-in fallback chart. */
    static SongChart fallback();

private:
    std::vector<Note> notes_; //!< Notes sorted by hit time.
};

} // namespace gyaru
