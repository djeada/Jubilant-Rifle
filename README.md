# Jubilant Rifle

A platform shooter game inspired by classic arcade run and gun games.

## Screenshot

![game](https://user-images.githubusercontent.com/37275728/193692533-f2302e53-ff44-45ea-9d15-3800c0e1ea5c.png)

## Game description

* The player moves through platforms on a 2D map.
* The player is armed with a gun and may fire at enemies.
* The enemies also have guns and can shoot the player.
* The game becomes increasingly difficult as time passes.
* The goal is to stay alive as long as possible.

## Dependencies

In order to play the game, you need to install the following dependencies:

* C++14
* SDL2
* CMake 3.9 or higher

To install SDL2 on Debian based systems, use:

```bash
apt install libsdl2-dev libsdl2-image-dev  libsdl2-mixer-dev  libsdl2-ttf-dev  
```

## Installation
 
To compile the game using CMake, run the following commands:

    git clone https://github.com/djeada/Jubilant-Rifle.git
    cd Jubilant-Rifle
    mkdir build && cd build
    cmake ..
    make

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
