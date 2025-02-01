# Jubilant Rifle

**Jubilant Rifle** is an action-packed 2D platform shooter game that offers a retro arcade gaming experience. Players navigate through challenging platforms, engaging in fierce gun battles with various enemies. As the game progresses, the difficulty ramps up with smarter and tougher adversaries. The ultimate objective is to survive the onslaught and rack up the highest score possible.

## Screenshot

![game](https://user-images.githubusercontent.com/37275728/193692533-f2302e53-ff44-45ea-9d15-3800c0e1ea5c.png)

## Gameplay Mechanics

- **Movement**: Utilize the **arrow keys** to move the character across platforms.
- **Shooting**: Hit the **space bar** to shoot and eliminate enemies.
- **Enemy Encounters**: Beware! Enemies are equipped with firearms and will return fire.
- **Difficulty Scaling**: Enemies increase in numbers and develop tougher strategies as you advance.
- **Objective**: Keep your character alive and maximize your score.

## System Requirements

For an optimal gaming experience, Jubilant Rifle requires:

- A system running **C** programming environment and **SDL2** framework.
- **CMake** version 3.9 or higher for building the game.
-
### Dependencies Installation on Debian-Based Systems

Execute the following command in the terminal to install SDL2:

```bash
apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

TODO: to install cmake

TODO: to verify your c compiler

## Building and running the game

To build and run the game, follow these steps:

1. Clone the repository: `git clone https://github.com/djeada/Jubilant-Rifle.git`
1. Navigate to the project directory: `cd Jubilant-Rifle`
1. Create a build directory: `mkdir build`
1. Navigate to the build directory: `cd build`
1. Run CMake: `cmake ..`
1. Build the game: `make`
1. Run the game: `./jubilant-rifle`
    
## Roadmap

The development of **Jubilant Rifle** is an ongoing process. We aim to continuously improve the game and add new features that will enhance the overall gameplay experience. Below is the roadmap outlining our planned enhancements and new features to look forward to:

- [x] **Initial Release**: Basic game mechanics, including player movement, shooting, and simple enemy AI.
- [x] **Graphic Enhancements**: Upgraded background and character graphics for a more visually engaging experience.
- [x] **Input Optimization**: Enhanced input detection for smoother player controls.
- [x] **Boundary Mechanics**: Implemented movement boundaries to define the playable area within the game world.
- [x] **Enemy Variety**: Introduction of diverse enemy types with varying behaviors.
- [x] **Shooting Mechanics**: Refined the shooting experience for the player, making it more responsive and satisfying.
- [x] **Enemy AI**: Improved enemy AI, making them more challenging and strategic in combat.
- [x] **Level Configuration Parser**: Developed a system to read and apply custom map configurations, paving the way for user-created levels.
- [ ] **Soundtrack and Sound Effects**: Compose a full original soundtrack and detailed sound effects for an immersive audio experience.
- [ ] **Multi-Level Support**: Implement multiple levels with increasing difficulty and varied environments.
- [ ] **Boss Battles**: Introduce challenging boss fights that require strategy and skill to overcome.
- [ ] **Power-Ups and Special Abilities**: Add power-ups that temporarily enhance the player's abilities or provide new ways to play.
- [ ] **Cutscene Integration**: Create introductory and concluding cutscenes to give context and depth to the game narrative.
- [ ] **Local Multiplayer**: Implement a local co-op or versus mode to play with friends.
- [ ] **Customizable Controls**: Allow players to customize their control schemes.
- [ ] **Achievements and Trophies**: Implement a system for in-game achievements to provide players with additional challenges and rewards.
- [ ] **Cloud Saving**: Enable players to save their game progress to the cloud for cross-platform play.
- [ ] **Global Leaderboards**: Include a global leaderboard system to encourage competitive play.

*Please note that this roadmap is subject to change based on player feedback and our internal prioritization. We're dedicated to creating the best experience possible and welcome community input on the direction of the game's development.*

If you have ideas or suggestions for features you'd like to see in **Jubilant Rifle**, don't hesitate to let us know by opening an issue or starting a discussion on our community forums.

## Contributing
We welcome contributions to Jubilant Rifle! If you are interested in making a contribution, please open an issue or submit a pull request with your proposed changes. Contributions could include bug fixes, new features, or improvements to existing code.

## License
Jubilant Rifle is licensed under the MIT license. This means that you can use and modify the code for any purpose, as long as you include the MIT license and attribution to the original authors.

[MIT](https://choosealicense.com/licenses/mit/)
