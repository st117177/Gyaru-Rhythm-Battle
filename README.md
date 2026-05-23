# Gyaru Dating Battle: Toxic Love 2007

**GitHub description:** C++/SFML rhythm dating battle with MVC architecture, SOLID-based class design, and Doxygen documentation.

Gyaru Dating Battle is a small C++ game project built with SFML. The game combines a short dialogue scene with a four-lane rhythm battle and a final result screen where the player's score changes relationship and reputation values.

The project is intentionally scoped as one complete gameplay loop:

1. Main menu.
2. Pre-battle dialogue with choices.
3. Four-lane rhythm battle.
4. Score, combo, hits, and misses calculation.
5. Result screen with grade and social outcome.

## Features

- Menu, dialogue, rhythm battle, and result screens.
- Four-lane rhythm gameplay controlled by arrow keys.
- Dialogue choices that change relationship, reputation, and drama risk.
- Chart loading from a text file with a built-in fallback chart.
- Texture loading helpers for transparent white backgrounds.
- Model-level tests for core gameplay logic.

## Build

Requirements:

- CMake 3.20+
- C++17 compiler
- Git, so CMake can fetch SFML 2.6.1

Build in Release mode:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Run tests:

```powershell
ctest --test-dir build -C Release --output-on-failure
```

Run the game:

```powershell
.\build\Release\gyaru_dating_battle.exe
```

For Debug builds, replace `Release` with `Debug`.

## Controls

- `Enter`: start / confirm / return to menu
- `Up` / `Down`: select dialogue choice
- `Left` / `Down` / `Up` / `Right`: rhythm lanes
- `Escape`: quit

## Project Structure

```text
src/app          Application loop and view switching
src/controller   Controller that converts UI input into model actions
src/interfaces   Interfaces for model, view, and controller
src/model        Game state, dialogue logic, chart loading, rhythm judging
src/view         SFML rendering for menu, dialogue, rhythm, and result screens
assets           Images, charts, music, and fonts
tests            Model-level tests
docs             Coursework documentation
```

## Architecture

The project uses an MVC-style architecture:

- **Model** stores game state and rules.
- **View** draws the current screen and reads user input.
- **Controller** connects view commands with model actions.

The main interfaces are:

- `IModel`
- `IView`
- `IController`

This keeps rendering, input handling, and game rules separated.

## SOLID Usage

The project follows SOLID principles in a practical coursework-friendly way:

- **Single Responsibility:** model classes handle game logic, view classes handle drawing, and controller classes handle commands.
- **Open/Closed:** new screens or models can be added through interfaces without rewriting the whole application loop.
- **Liskov Substitution:** concrete views implement `IView` and can be used through the same interface.
- **Interface Segregation:** model, view, and controller contracts are split into separate interfaces.
- **Dependency Inversion:** `Application` and views work through interfaces instead of depending directly on concrete model/controller classes.

## Doxygen Documentation

Header files include Doxygen comments for public classes, functions, parameters, return values, enum values, and fields.

Examples used in the code:

```cpp
/**
 * @brief Loads notes from a chart file.
 * @param[in] path Path to the chart file.
 * @return true if at least one valid note was loaded.
 */
bool loadFromFile(const std::string& path);
```

```cpp
int score = 0; //!< Current battle score.
```

`@param` is used for function parameters, while `//!<` documents fields and enum values on the same line.

## Timing Tweaks

- Audio/chart speed: change `battleSpeed` in `src/model/GameModel.cpp`.
- Arrow travel speed: change `spawnLeadMs` in `src/view/RhythmView.cpp`.

`1.0` is normal speed. Values below `1.0` make the battle slightly slower.

## Assets

The battle music is loaded from:

- `assets/audio/intro_battle.ogg`
- `assets/audio/intro_battle.mp3`
- `assets/audio/intro_battle.wav`

If battle music is missing, the rhythm battle still works using the internal timer.

Background music is loaded from:

- `assets/audio/background_music.ogg`
- `assets/audio/background_music.mp3`
- `assets/audio/background_music.wav`
- `assets/audio/forbackroundmusic.mp4`

## Status

This is a finished vertical slice for a coursework project. The focus is a working loop, clear architecture, documented headers, and testable model logic.
