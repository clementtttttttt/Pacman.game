emcc main.cpp notes.cpp gsdl.cpp -s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 -o pacman.html -DEMCXX -O3  --preload-file "./font.ttf" --shell-file=./template.html




