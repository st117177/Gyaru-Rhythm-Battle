#include "controller/GameController.h"

namespace gyaru {

GameController::GameController(IModel& model) : model_(model) {}

void GameController::requestClose() {
    closeRequested_ = true;
}

void GameController::startDialogue() {
    model_.startDialogue();
}

void GameController::selectPreviousDialogueChoice() {
    model_.selectPreviousDialogueChoice();
}

void GameController::selectNextDialogueChoice() {
    model_.selectNextDialogueChoice();
}

void GameController::confirmDialogueChoice() {
    model_.confirmDialogueChoice();
}

void GameController::pressBattleLane(int lane) {
    model_.pressBattleLane(lane);
}

void GameController::resetToMenu() {
    model_.resetToMenu();
}

void GameController::update() {
    model_.update();
}

bool GameController::wantsClose() const {
    return closeRequested_;
}

void GameController::clearCloseRequest() {
    closeRequested_ = false;
}

} // namespace gyaru
