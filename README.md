# 🧙 LONE

<p align="center">
  <strong>A Metroidvania × Souls-like built from scratch.</strong><br>
  <em>C++ · SFML · No game engine · Solo development</em>
</p>

LONE is an experimental 2D action game about a mage exploring a hostile medieval
world. The project is developed from scratch in C++ with SFML, without relying
on a game engine.

## Features

- Room loading and platform collisions through Tiled maps.
- Player movement, attacks, projectiles and enemy encounters.
- Death, respawn and transition effects.

## Requirements

- CMake 3.15 or newer.
- A C++17-compatible compiler.
- SFML 3.

SFML is kept in `extern/SFML` so the repository can be built reproducibly.

## Build

```bash
cmake -S . -B build
cmake --build build --config Release
```

On Windows, the executable is generated in `build/Release/` for multi-config
generators. Runtime assets are copied next to the executable automatically.

## Project layout

```text
assets/        Game textures, maps and other runtime resources
extern/        Vendored third-party dependencies
include/lone/  Public C++ headers
src/           C++ implementations and application entry point
```

## Development

LONE is a work in progress. Bug reports, ideas and contributions are welcome
through the repository's Issues and Discussions.

## License

The project is currently under development. License information will be added
before the first release.
