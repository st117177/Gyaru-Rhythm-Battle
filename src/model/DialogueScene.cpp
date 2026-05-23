#include "model/DialogueScene.h"

#include <utility>

namespace gyaru {

DialogueScene::DialogueScene(std::string speaker, std::string line, std::string portrait, std::vector<DialogueChoice> choices)
    : speaker_(std::move(speaker)),
      line_(std::move(line)),
      portrait_(std::move(portrait)),
      choices_(std::move(choices)) {}

const std::string& DialogueScene::speaker() const {
    return speaker_;
}

const std::string& DialogueScene::line() const {
    return line_;
}

const std::string& DialogueScene::portrait() const {
    return portrait_;
}

const std::vector<DialogueChoice>& DialogueScene::choices() const {
    return choices_;
}

void DialogueScene::applyChoice(std::size_t index, GameState& state) const {
    if (index >= choices_.size()) {
        return;
    }

    const auto& choice = choices_[index];

    state.lastChoiceText = choice.label;
    state.rival.relationship += choice.relationshipDelta;
    state.reputation += choice.reputationDelta;
    state.rival.dramaRisk += choice.dramaRiskDelta;
    state.rival.mood = choice.response;

    if (!choice.nextPortrait.empty()) {
        state.currentPortrait = choice.nextPortrait;
    }

    clampSocialStats(state);
}

DialogueScene DialogueScene::openingScene() {
    return DialogueScene{
        "Юми",
        "О, новая «богиня гяру» решила занять музыкалку? У нас тут очередь: я, и мои девочки, которые пришли смотреть, как ты промахнешься.",
        "pointing",
        {
            {"«Ногти красивые. Посмотрим, кнопки ими нажимать умеешь?»", 2, 1, 0, "Пытается не улыбнуться", "angry_happy"},
            {"«Очередь? Я думала, это фан-встреча»", -2, 1, 2, "Возмущена, но слушает", "angry"}
        }
    };
}

DialogueScene DialogueScene::secondScene(const GameState& state) {
    if (state.rival.dramaRisk >= 2) {
        return DialogueScene{
            "Юми",
            "Фан-встреча? Смело. Если проиграешь, я запишу твой позор на рингтон.",
            "angry",
            {
                {"«Окей, но обложку я выбираю сама»", 1, 1, -1, "Оценила стиль", "angry_happy"},
                {"«Он и то станет хитом быстрее, чем твоя демка из подвала»", -3, -1, 2, "Злится сильнее", "angry"},
            }
        };
    }

    return DialogueScene{
        "Юми",
        "Комплимент засчитан. Но не расслабляйся: я видела, как ты нажимаешь кнопки. Это было похоже на СМС вслепую под партой.",
        "angry_happy",
        {
            {"«Значит, представь, что будет, когда я посмотрю на экран»", 2, 2, -1, "пон", "normal"},
            {"«Зато попала по смыслу. Теперь попаду по биту»", 1, 1, 0, "Кивает с интересом", "angry_happy"},
        }
    };
}

} // namespace gyaru
