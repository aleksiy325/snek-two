#include "gui/board_render.cpp"
#include "arena/arena.cpp"


int main(int argc, char **argv){
    Arena arena = Arena(10, 10, 0);

    RandomSnake rsnake = RandomSnake{};
    Strategy* strat = &rsnake;
    arena.addStrategy(strat);

    SdlHandle sdl_handle = initSDL(arena);

    bool quit = false;
    SDL_Event e;

    Uint32 movesPerSecond = 5;
    Uint32 timePerFrame = 1000 / movesPerSecond;
    Uint32 prevTime = SDL_GetTicks();

    while(!quit){
        Uint32 curTime = SDL_GetTicks();
        Uint32 deltaTime = curTime - prevTime;
        if ((deltaTime) > timePerFrame){
            prevTime = curTime;
            arena.executeTick();
            renderArena(sdl_handle.renderer, arena);
        }

        while(SDL_PollEvent(&e) != 0){
          if(e.type == SDL_QUIT){
            quit = true;
          }
        }
    }

    cleanupSDL(sdl_handle);
}
   