# Jubilant Rifle

**Jubilant Rifle** is an action-packed 2D platform shooter game that offers a retro arcade gaming experience. Players navigate through challenging platforms, engaging in fierce gun battles with various enemies. As the game progresses, the difficulty ramps up with smarter and tougher adversaries. The ultimate objective is to survive the onslaught and rack up the highest score possible.

## Screenshot

![game](https://user-images.githubusercontent.com/37275728/193692533-f2302e53-ff44-45ea-9d15-3800c0e1ea5c.png)

## Features

- **Smooth Controls**: Responsive movement and shooting mechanics
- **Score Tracking**: Track your performance with combo multipliers and accuracy stats
- **High Scores**: Local high score persistence
- **Customizable Settings**: Audio, video, and gameplay configuration
- **Custom Key Bindings**: Remap controls to your preference
- **Extensible Map System**: Dynamic map loading with JSON configuration
- **Modular Architecture**: Clean, maintainable code with clear separation of concerns
- **Level Timer**: Track your completion time for each level
- **Ladders**: Climb between platforms for vertical movement
- **Grenades**: Explosive combat for both players and enemies
- **Melee Combat**: Close-quarters fighting for alternative combat style
- **Environmental Traps**: Strategic hazards like fuel barrels that explode
- **Flag Capture**: Capture flags as level objectives
- **Parachuting Enemies**: Waves of enemies descend from above

## Gameplay Mechanics

- **Movement**: Use **arrow keys** or **WASD** to move the character across platforms.
- **Shooting**: Press **Space** or **X** to shoot and eliminate enemies.
- **Melee Attack**: Press **Z** or **Left Ctrl** for close-quarters combat.
- **Throw Grenade**: Press **G** to throw a grenade (limited supply).
- **Climb Ladders**: Press **Up/W** while on a ladder to climb.
- **Pause**: Press **Escape** to return to menu.
- **Enemy Encounters**: Enemies are armed and will return fire, plus throw grenades.
- **Parachute Waves**: Enemy reinforcements drop from the sky periodically.
- **Combo System**: Chain kills together for score multipliers.
- **Trap Strategy**: Shoot fuel barrels near enemies for explosive damage.
- **Flag Objectives**: Capture all flags to complete level objectives.
- **Difficulty Scaling**: Enemies increase in numbers and develop tougher strategies as you advance.
- **Objective**: Survive as long as possible and maximize your score.

## System Requirements

For an optimal gaming experience, Jubilant Rifle requires:

- A C99-compatible compiler (GCC, Clang, or MSVC)
- **SDL2** framework and related libraries
- **CMake** version 3.10 or higher for building the game

### Dependencies Installation

#### Debian/Ubuntu

```bash
sudo apt-get install build-essential cmake libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

#### Fedora

```bash
sudo dnf install gcc cmake SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel
```

#### macOS (using Homebrew)

```bash
brew install cmake sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

## Building and Running

To build and run the game, follow these steps:

```bash
# Clone the repository
git clone https://github.com/djeada/Jubilant-Rifle.git
cd Jubilant-Rifle

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run the game
./jubilant-rifle
```

## Project Structure

```
Jubilant-Rifle/
├── include/           # Header files
│   ├── entities/      # Entity system (player, enemy, bullet)
│   ├── game/          # Game logic and state management
│   ├── map/           # Map loading and management
│   ├── rendering/     # Rendering and texture management
│   └── utils/         # Utilities (logging, config, math)
├── src/               # Source files (mirrors include structure)
├── resources/         # Game assets
│   ├── textures/      # Sprite images
│   ├── fonts/         # Game fonts
│   ├── maps/          # Level configurations
│   └── audio/         # Sound effects and music
├── tests/             # Unit tests (Google Test)
└── scripts/           # CMake find modules
```

## Configuration

### Game Settings

Create a `config.cfg` file in the game directory to customize settings:

```ini
# Audio Settings
master_volume = 1.0
music_volume = 0.7
sfx_volume = 0.8

# Video Settings
window_width = 1280
window_height = 720
fullscreen = false
vsync = true

# Gameplay Settings
player_speed = 1.0
difficulty_scale = 1.0
show_fps = false
```

### Custom Key Bindings

Controls can be customized by creating an `input.cfg` file:

```ini
Move Left = 1073741904, 97      # Left Arrow, A
Move Right = 1073741903, 100    # Right Arrow, D
Shoot = 120, 1073742048         # X, Left Ctrl
```

## Roadmap

### Completed

- [x] Basic game mechanics (movement, shooting, enemy AI)
- [x] Graphics and animation system
- [x] Input handling with customizable key bindings
- [x] Level configuration parser with dynamic platform loading
- [x] Score tracking with combo system
- [x] Configuration system for settings
- [x] Logging and error handling utilities

### In Progress

- [ ] Soundtrack and sound effects
- [ ] Pause menu functionality
- [ ] Game over screen with stats

### Planned

- [ ] Multi-level support
- [ ] Boss battles
- [ ] Power-ups and special abilities
- [ ] Local multiplayer
- [ ] Achievements system
- [ ] Global leaderboards

## Contributing

We welcome contributions to Jubilant Rifle! To contribute:

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/my-feature`
3. Make your changes and add tests if applicable
4. Run the test suite: `cd build && ctest`
5. Commit your changes: `git commit -m "Add my feature"`
6. Push to the branch: `git push origin feature/my-feature`
7. Open a Pull Request

### Code Style

- Follow the existing code style (enforced by `.clang-format`)
- Add documentation comments to public APIs
- Include unit tests for new functionality

## License

Jubilant Rifle is licensed under the MIT license. See [LICENSE](LICENSE) for details.

[MIT](https://choosealicense.com/licenses/mit/)
