#include "gui/board_render.cpp"
#include "arena/arena.cpp"


int main(int argc, char **argv){
    Arena arena = Arena(15, 15, 5);

    RandomSnake rsnake = RandomSnake{};
    Strategy* strat = &rsnake;
    arena.addStrategy(strat);
    arena.addStrategy(strat);
    arena.addStrategy(strat);
    arena.addStrategy(strat);

    SDL_Handle handle = initSDL(arena);

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
            renderArena(handle, arena);
        }

        while(SDL_PollEvent(&e) != 0){
          if(e.type == SDL_QUIT){
            quit = true;
          }
        }
    }

    cleanupSDL(handle);
}
   