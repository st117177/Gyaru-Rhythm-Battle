#include "app/Application.h"

#include "controller/GameController.h"
#include "model/GameModel.h"
#include "view/DialogueView.h"
#include "view/MenuView.h"
#include "view/ResultView.h"
#include "view/RhythmView.h"

#include <array>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace gyaru {

namespace {

constexpr int windowWidth = 960;
constexpr int windowHeight = 600;
constexpr int frameLimit = 60;

} // namespace

Application::Application(std::string assetRoot)
    : assetRoot_(std::move(assetRoot)),
      window_(sf::VideoMode(windowWidth, windowHeight), "Gyaru Rhythm Battle Game"),
      model_(std::make_unique<GameModel>(assetRoot_)),
      controller_(std::make_unique<GameController>(*model_)),
      menuView_(std::make_unique<MenuView>(assetRoot_)),
      dialogueView_(std::make_unique<DialogueView>(assetRoot_)),
      rhythmView_(std::make_unique<RhythmView>(assetRoot_)),
      resultView_(std::make_unique<ResultView>(assetRoot_)) {
    window_.setFramerateLimit(frameLimit);
}

int Application::run() {
    if (!loadFont()) {
        std::cerr << "Could not load a UI font. Add assets/fonts/ui.ttf or install a system font.\n";
        return 1;
    }

    while (window_.isOpen()) {
        processEvents();
        update();
        render();
    }

    return 0;
}

void Application::processEvents() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        } else {
            activeView().handleEvent(event, *controller_);
        }
    }

    if (controller_->wantsClose()) {
        controller_->clearCloseRequest();
        window_.close();
    }
}

void Application::update() {
    controller_->update();
}

void Application::render() {
    activeView().render(window_, font_, *model_);
    window_.display();
}

bool Application::loadFont() {
    const std::array<std::string, 5> candidates = {
        "assets/fonts/ui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/tahoma.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
    };
    for (const auto& path : candidates) {
        if (font_.loadFromFile(path)) {
            return true;
        }
    }
    return false;
}

const IView& Application::activeView() const {
    switch (model_->state().scene) {
    case SceneType::Menu:
        return *menuView_;
    case SceneType::PreDialogue:
        return *dialogueView_;
    case SceneType::Battle:
        return *rhythmView_;
    case SceneType::Result:
        return *resultView_;
    }

    throw std::logic_error("Unknown scene type");
}

} // namespace gyaru
