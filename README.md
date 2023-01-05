# Jubilant Rifle

Jubilant Rifle is a 2D platform shooter game in which the player moves through platforms and battles enemies with a gun. The game becomes increasingly difficult as time passes, and the goal is to stay alive as long as possible.

## Screenshot

![game](https://user-images.githubusercontent.com/37275728/193692533-f2302e53-ff44-45ea-9d15-3800c0e1ea5c.png)

## How to play

To move the player, use the arrow keys. Press the space bar to shoot at enemies. The enemies are also armed with guns and will shoot at the player. As the game progresses, the enemies will become more numerous and more difficult to defeat. The goal is to stay alive as long as possible.

## System requirements

Jubilant Rifle requires C and SDL2 to be installed, as well as CMake 3.9 or higher. 

On Debian based systems, you can install SDL2 with the following command:

```bash
apt install libsdl2-dev libsdl2-image-dev  libsdl2-mixer-dev  libsdl2-ttf-dev  
```

## Building and running the game

To build and run the game, follow these steps:

1. Clone the repository: `git clone https://github.com/djeada/Jubilant-Rifle.git`
1. Navigate to the project directory: `cd Jubilant-Rifle`
1. Create a build directory: `mkdir build`
1. Navigate to the build directory: `cd build`
1. Run CMake: `cmake ..`
1. Build the game: `make`
1. Run the game: `./jubilant-rifle`
    
## TODO

- [x] Display the background image.
- [x] Create a player and place it in front of the background image.
- [x] Use an infinite loop to detect if the user pressed any keys. If this is the case, move the player.
- [x] Limit possible movements.
- [x] Add n enemies. 
- [x] Allow the player to shoot at the opponents.
- [x] Enable the enemies to shoot the player.
- [x] Make a map configuration parser. A map configuration file should specify the platforms and enemies on a given map.
- [ ] Show the intro and outro screens.
- [ ] Support for multiple maps/levels should be provided. 

## Contributing
We welcome contributions to Jubilant Rifle! If you are interested in making a contribution, please open an issue or submit a pull request with your proposed changes. Contributions could include bug fixes, new features, or improvements to existing code.

## License
Jubilant Rifle is licensed under the MIT license. This means that you can use and modify the code for any purpose, as long as you include the MIT license and attribution to the original authors.

[MIT](https://choosealicense.com/licenses/mit/)
