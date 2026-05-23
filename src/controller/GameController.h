#pragma once

#include "interfaces/IController.h"
#include "interfaces/IModel.h"

namespace gyaru {

/**
 * @brief Concrete controller that converts view commands into model updates.
 */
class GameController final : public IController {
public:
    /**
     * @brief Creates a controller for the provided model interface.
     * @param[in,out] model Model interface that receives game actions.
     */
    explicit GameController(IModel& model);

    /** @brief Requests closing the game window. */
    void requestClose() override;

    /** @brief Starts the dialogue scene. */
    void startDialogue() override;

    /** @brief Moves dialogue selection upward. */
    void selectPreviousDialogueChoice() override;

    /** @brief Moves dialogue selection downward. */
    void selectNextDialogueChoice() override;

    /** @brief Confirms the selected dialogue choice. */
    void confirmDialogueChoice() override;

    /**
     * @brief Attempts a rhythm-lane hit.
     * @param[in] lane Zero-based rhythm lane index.
     */
    void pressBattleLane(int lane) override;

    /** @brief Resets the game to the menu. */
    void resetToMenu() override;

    /** @brief Updates model state for one frame. */
    void update() override;

    /** @brief Returns true when the game should close. */
    bool wantsClose() const override;

    /** @brief Clears the close request flag. */
    void clearCloseRequest() override;

private:
    /** @brief Ссылка на модель, с которой работает контроллер. */
    IModel& model_; //!< Model receiving controller actions.

    /** @brief Флаг запроса на закрытие окна. */
    bool closeRequested_ = false; //!< Whether the game should close.
};

} // namespace gyaru
