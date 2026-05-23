# Scope Statement

## In Version 1

- C++17 project using SFML for graphics, audio, input, and timing.
- MVC folder split: model, controller, view.
- One character: Yumi.
- One pre-battle dialogue scene with three choices.
- One four-lane rhythm battle using arrow keys in `← ↓ ↑ →` order.
- One chart loaded from `assets/charts/intro_battle.chart`.
- Score, combo, best combo, hits, and misses.
- Relationship, reputation, and drama-risk variables.
- Result screen with grade and social outcome.
- Model-level tests for timing, scoring, and social state changes.

## Out Of Version 1

- map exploration
- dorm simulator systems
- dress-up mechanics
- more characters
- procedural drama
- natural language dialogue
- AI personalities
- full reputation network
- chart editor
- multiple songs
- fighting mechanics
- generation/life simulation

## Acceptance Criteria

- The player can launch the game, choose dialogue, play a rhythm chart, and see an ending result.
- Different battle performance produces different relationship/reputation outcomes.
- The model classes can be tested without SFML.
- The project can be explained through MVC diagrams.
