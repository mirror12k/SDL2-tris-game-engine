# SDL2-tris-game-engine
a minimalistic game engine built on SDL2

## about
requires SDL2, SDL2_image, and SDL2_ttf

several examples exist in main.cpp, simply uncomment them to view

## engine description
- single threaded engine
- uses a single pool of entities, with support for service entities
- entities do not get to draw themselves (unlike the Diesel engine),
instead they have to pass a tris::box reference to the graphics_context on creation.
this box is rendered once into a texture, and that texture is copied onto the final canvas.
tris::box can be extended to provide custom texture renderinga

## screenshots
![alt text](https://raw.githubusercontent.com/mirror12k/SDL2-tris-game-engine/master/screenshots/screen1.png)
![alt text](https://raw.githubusercontent.com/mirror12k/SDL2-tris-game-engine/master/screenshots/screen2.png)
![alt text](https://raw.githubusercontent.com/mirror12k/SDL2-tris-game-engine/master/screenshots/screen3.png)
![alt text](https://raw.githubusercontent.com/mirror12k/SDL2-tris-game-engine/master/screenshots/screen4.png)
![alt text](https://raw.githubusercontent.com/mirror12k/SDL2-tris-game-engine/master/screenshots/screen5.png)
