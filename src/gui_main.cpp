#include "gui/board_render.cpp"
#include "strategies/random_snake.cpp"
#include "strategies/eat_snake.cpp"
#include "strategies/heuristic_snake.cpp"
#include "strategies/minimax_snake.cpp"
#include "arena/game.cpp"

const int num_hsnake = 1;

int main(int argc, char **argv){
    Game game = Game(20, 20, 15);

    // RandomSnake rsnake = RandomSnake();
    EatSnake esnake = EatSnake();
    MinimaxSnake hsnake = MinimaxSnake();

    // game.addStrategy(&rsnake);
    // game.addStrategy(&rsnake);
    // game.addStrategy(&esnake);
    game.addStrategy(&esnake);

    for(int i = 0; i < num_hsnake; i++){
        game.addStrategy(&hsnake);
    }

    SDL_Handle handle = initSDL(game);

    bool quit = false;
    SDL_Event e;

    Uint32 movesPerSecond = 100;
    Uint32 timePerFrame = 1000 / movesPerSecond;
    Uint32 prevTime = SDL_GetTicks();

    while(!quit){
        Uint32 curTime = SDL_GetTicks();
        Uint32 deltaTime = curTime - prevTime;
        if ((deltaTime) > timePerFrame){
            prevTime = curTime;
            game.executeTick();
            renderGame(handle, game);
        }

        while(SDL_PollEvent(&e) != 0){
          if(e.type == SDL_QUIT){
            quit = true;
          }
        }
    }

    cleanupSDL(handle);
}
