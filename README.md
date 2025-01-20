# Space Pixels

Space Pixels is a Shoot'Em Up game developed in C. The code available in this directory corresponds to the basecode of the mid-year project for the first year at ESIEA in 2024-2025.

## Building

### Building for Visual Studio (Windows)

This project was developed for Windows under Visual Studio.
To compile this project, make sure you have correctly installed the following tools:
- [Visual Studio](https://visualstudio.microsoft.com/) with "Desktop Development in C++" option
- [CMake](https://cmake.org/download/)

Rename the ``build_vs.txt`` file to ``build_vs.bat`` and double-click on this file to run the script.
The Visual Studio solution is created in the ``_build_vs`` folder.

If you wish to compile the project manually, you can run the following commands:
```
mkdir _build_vs
cd _build_vs
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Building for linux

Use the following commands to install project dependencies:
```
sudo apt install cmake
sudo apt install libsdl2-dev
sudo apt install libsdl2-image-dev
sudo apt install libsdl2-ttf-dev
sudo apt install libsdl2-mixer-dev
```
Then run the following commands:
```
mkdir _build_linux
cd _build_linux
cmake ..
cmake --build .
../_bin/application
```

## License

All code produced for this project is licensed under the [MIT license](/LICENSE.md). It uses the [SDL2](https://github.com/libsdl-org/SDL), [SDL2_image](https://github.com/libsdl-org/SDL_image), [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer) and [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf) libraries.

Licenses for game assets are available in the assets folder.
