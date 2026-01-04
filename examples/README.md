# Map Generator Examples

This directory contains example programs demonstrating how to use the Jubilant Rifle map generation system.

## Map Generator Example

The `map_gen_example.c` program shows how to procedurally generate game maps with different styles and parameters.

### Building

To build the example (without the full game build system):

```bash
cd examples
gcc -o map_gen_example map_gen_example.c \
    ../src/map/map.c \
    ../src/map/map_generator.c \
    ../src/map/ladder.c \
    ../src/map/trap.c \
    ../src/map/flag.c \
    -I../include \
    -lm
```

Note: This requires SDL2 development headers to be installed.

### Usage

Generate a map with default settings (mixed style):
```bash
./map_gen_example
```

Generate a map with a specific style:
```bash
./map_gen_example horizontal resources/maps/my_horizontal_map.cfg
./map_gen_example vertical resources/maps/my_vertical_map.cfg
./map_gen_example spiral resources/maps/my_spiral_map.cfg
./map_gen_example random resources/maps/my_random_map.cfg
```

### Generation Styles

- **horizontal**: Platforms arranged in horizontal rows, good for traditional platformer feel
- **vertical**: Platforms arranged in vertical columns, emphasizes vertical climbing
- **spiral**: Platforms arranged in a spiral pattern, creates circular progression
- **mixed**: Combination of horizontal and vertical arrangements (default)
- **random**: Completely random platform placement, most chaotic

### Generated Map Files

The generated maps are saved as JSON configuration files compatible with the game's map system. Each generated map includes:

- **Platforms**: The main structures players stand on
- **Ladders**: Automatically placed to connect platforms vertically
- **Traps**: Environmental hazards (fuel barrels) placed strategically on platforms
- **Flags**: Objective markers distributed evenly across the map

### Customizing Generation

You can modify the generation parameters in the code:

```c
mapGenParamsInit(&params, 6400, 3600);  // Width x Height
params.platformCount = 50;               // Number of platforms
params.ladderCount = 25;                 // Number of ladders
params.trapCount = 20;                   // Number of traps
params.flagCount = 5;                    // Number of flags
params.minPlatformWidth = 200;           // Minimum platform width
params.maxPlatformWidth = 500;           // Maximum platform width
params.verticalSpacing = 300;            // Vertical space between levels
params.horizontalSpacing = 400;          // Horizontal space between platforms
```

### Using Generated Maps in Game

After generating a map, add it to the game by editing `include/utils/consts.h`:

```c
#define MAPS \
  { \
    "resources/maps/mountains.cfg", \
    "resources/maps/urban.cfg", \
    "resources/maps/desert.cfg", \
    "resources/maps/industrial.cfg", \
    "resources/maps/my_generated_map.cfg"  /* Add your map here */ \
  }
```

Then rebuild the game and your map will be included in the map rotation.

## Tips for Good Maps

1. **Balance vertical and horizontal space**: Too much vertical climbing can be tedious, too much horizontal running can be boring
2. **Connect platforms with ladders**: Make sure players can reach most areas
3. **Strategic trap placement**: Place traps near choke points or objectives
4. **Flag distribution**: Spread flags evenly to encourage exploration
5. **Test and iterate**: Generate multiple maps and test them to find good layouts

## Map Format

Generated maps use JSON format:

```json
{
  "background_image": "resources/textures/background.png",
  "width": 6400,
  "height": 3600,
  "platforms": [
    { "x": 100, "y": 3400, "width": 300, "height": 30 },
    ...
  ],
  "ladders": [
    { "x": 200, "y": 3100, "width": 30, "height": 300 },
    ...
  ],
  "traps": [
    { "x": 250, "y": 3350, "width": 40, "height": 50, "type": 0 },
    ...
  ],
  "flags": [
    { "x": 300, "y": 3300, "width": 80, "height": 100 },
    ...
  ]
}
```

All coordinates are in pixels, with origin (0,0) at top-left.
