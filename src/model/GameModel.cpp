#include "model/GameModel.h"

#include "model/SongChart.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <utility>

namespace gyaru {

namespace {

// 1.0 = original speed. Lower values slow both the music and chart clock.
constexpr float battleSpeed = 0.92f;
constexpr int tooEarlyFeedbackWindowMs = 520;

std::string firstExistingMusicPath(const std::string& assetRoot) {
    const std::array<std::string, 3> candidates = {
        assetRoot + "/audio/intro_battle.ogg",
        assetRoot + "/audio/intro_battle.mp3",
        assetRoot + "/audio/intro_battle.wav"
    };

    for (const auto& path : candidates) {
        std::ifstream input(path, std::ios::binary);
        if (input.good()) {
            return path;
        }
    }

    return {};
}

std::string firstExistingBackgroundMusicPath(const std::string& assetRoot) {
    const std::array<std::string, 4> candidates = {
        assetRoot + "/audio/background_music.ogg",
        assetRoot + "/audio/background_music.mp3",
        assetRoot + "/audio/background_music.wav",
        assetRoot + "/audio/forbackroundmusic.mp4"
    };

    for (const auto& path : candidates) {
        std::ifstream input(path, std::ios::binary);
        if (input.good()) {
            return path;
        }
    }

    return {};
}

} // namespace

GameModel::GameModel(std::string assetRoot)
    : assetRoot_(std::move(assetRoot)), dialogue_(DialogueScene::openingScene()) {
    startBackgroundMusic();
}

const GameState& GameModel::state() const {
    return state_;
}

const DialogueScene& GameModel::dialogue() const {
    return dialogue_;
}

const RhythmBattle& GameModel::battle() const {
    return battle_;
}

int GameModel::battleElapsedMs() const {
    return static_cast<int>(battleClock_.getElapsedTime().asMilliseconds() * battleSpeed);
}

const std::string& GameModel::lastHitText() const {
    return lastHitText_;
}

void GameModel::startDialogue() {
    startBackgroundMusic();
    state_.scene = SceneType::PreDialogue;
    state_.dialogueChoiceIndex = 0;
    state_.dialogueStep = 0;
    dialogue_ = DialogueScene::openingScene();
}

void GameModel::selectPreviousDialogueChoice() {
    if (state_.dialogueChoiceIndex > 0) {
        --state_.dialogueChoiceIndex;
    }
}

void GameModel::selectNextDialogueChoice() {
    const int lastIndex = static_cast<int>(dialogue_.choices().size()) - 1;
    state_.dialogueChoiceIndex = std::min(state_.dialogueChoiceIndex + 1, lastIndex);
}

void GameModel::confirmDialogueChoice() {
    dialogue_.applyChoice(static_cast<std::size_t>(state_.dialogueChoiceIndex), state_);
    if (state_.dialogueStep == 0) {
        state_.dialogueStep = 1;
        state_.dialogueChoiceIndex = 0;
        dialogue_ = DialogueScene::secondScene(state_);
    } else {
        startBattle();
    }
}

void GameModel::pressBattleLane(int lane) {
    const auto result = battle_.tryHit(lane, battleElapsedMs());
    if (result.rating == HitRating::Perfect) {
        lastHitText_ = "PERFECT";
    } else if (result.rating == HitRating::Good) {
        lastHitText_ = "GOOD";
    } else {
        const int nextDelta = battle_.nextPendingDeltaForLane(lane, battleElapsedMs());
        lastHitText_ = nextDelta <= tooEarlyFeedbackWindowMs ? "TOO EARLY" : "MISS";
    }
}

void GameModel::update() {
    if (state_.scene != SceneType::Battle) {
        return;
    }

    const int elapsedMs = battleElapsedMs();
    battle_.update(elapsedMs);

    state_.score = battle_.score();
    state_.highScore = std::max(state_.highScore, state_.score);
    state_.combo = battle_.combo();
    state_.bestCombo = battle_.bestCombo();
    state_.hits = battle_.hits();
    state_.misses = battle_.misses();

    if (battle_.isFinished(elapsedMs)) {
        finishBattle();
    }
}

void GameModel::resetToMenu() {
    const int highScore = state_.highScore;
    state_ = GameState{};
    state_.highScore = highScore;
    dialogue_ = DialogueScene::openingScene();
    lastHitText_.clear();
}

void GameModel::startBattle() {
    stopBackgroundMusic();

    SongChart chart;
    const std::string chartPath = assetRoot_ + "/charts/intro_battle.chart";
    if (!chart.loadFromFile(chartPath)) {
        chart = SongChart::fallback();
    }

    battle_.start(chart);
    battleClock_.restart();
    lastHitText_.clear();
    state_.scene = SceneType::Battle;

    const std::string musicPath = firstExistingMusicPath(assetRoot_);
    musicLoaded_ = !musicPath.empty() && music_.openFromFile(musicPath);
    if (musicLoaded_) {
        music_.setLoop(false);
        music_.setPitch(battleSpeed);
        music_.setVolume(55.f);
        music_.play();
    }
}

void GameModel::finishBattle() {
    if (musicLoaded_) {
        music_.stop();
    }

    state_.score = battle_.score();
    state_.combo = battle_.combo();
    state_.bestCombo = battle_.bestCombo();
    state_.hits = battle_.hits();
    state_.misses = battle_.misses();

    const auto outcome = makeBattleOutcome(battle_.score(), battle_.maxScore(), battle_.misses());
    applyBattleOutcome(state_, outcome);
    state_.scene = SceneType::Result;
    startBackgroundMusic();
}

void GameModel::startBackgroundMusic() {
    if (backgroundMusic_.getStatus() == sf::Music::Playing) {
        return;
    }

    if (!backgroundMusicLoaded_) {
        const std::string path = firstExistingBackgroundMusicPath(assetRoot_);
        backgroundMusicLoaded_ = !path.empty() && backgroundMusic_.openFromFile(path);
        if (backgroundMusicLoaded_) {
            backgroundMusic_.setLoop(true);
            backgroundMusic_.setVolume(35.f);
        }
    }

    if (backgroundMusicLoaded_) {
        backgroundMusic_.play();
    }
}

void GameModel::stopBackgroundMusic() {
    if (backgroundMusic_.getStatus() != sf::Music::Stopped) {
        backgroundMusic_.stop();
    }
}

} // namespace gyaru
