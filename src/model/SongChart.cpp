#include "model/SongChart.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <utility>

namespace gyaru {

namespace {

bool parseNoteLine(std::string line, Note& note) {
    const auto commentStart = line.find('#');
    if (commentStart != std::string::npos) {
        line = line.substr(0, commentStart);
    }

    std::istringstream stream(line);
    int timeMs = 0;
    int lane = 0;
    if (!(stream >> timeMs >> lane) || timeMs < 0 || lane < 0 || lane >= 4) {
        return false;
    }

    note = {timeMs, lane, NoteState::Pending};
    return true;
}

void sortByTime(std::vector<Note>& notes) {
    std::sort(notes.begin(), notes.end(), [](const Note& a, const Note& b) {
        return a.timeMs < b.timeMs;
    });
}

} // namespace

bool SongChart::loadFromFile(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        return false;
    }

    std::vector<Note> loaded;
    std::string line;
    while (std::getline(input, line)) {
        Note note;
        if (parseNoteLine(line, note)) {
            loaded.push_back(note);
        }
    }

    if (loaded.empty()) {
        return false;
    }

    sortByTime(loaded);
    notes_ = std::move(loaded);
    return true;
}

const std::vector<Note>& SongChart::notes() const {
    return notes_;
}

std::vector<Note>& SongChart::notes() {
    return notes_;
}

int SongChart::lastNoteTimeMs() const {
    if (notes_.empty()) {
        return 0;
    }
    return notes_.back().timeMs;
}

int SongChart::maxScore() const {
    return static_cast<int>(notes_.size()) * 1000;
}

bool SongChart::empty() const {
    return notes_.empty();
}

SongChart SongChart::fallback() {
    SongChart chart;
    chart.notes_ = {
        {1000, 0}, {1300, 1}, {1600, 2}, {1900, 3},
        {2400, 3}, {2700, 2}, {3000, 1}, {3300, 0},
        {3900, 0}, {4200, 2}, {4500, 1}, {4800, 3},
        {5400, 0}, {5700, 1}, {6000, 2}, {6300, 3}
    };
    return chart;
}

} // namespace gyaru
