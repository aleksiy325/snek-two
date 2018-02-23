#include "gui/board_render.cpp"
#include "strategies/random_snake.cpp"
#include "strategies/eat_snake.cpp"
#include "strategies/heuristic_snake.cpp"
#include "arena/game.cpp"


int main(int argc, char **argv){
    Game game = Game(15, 15, 5);

    RandomSnake rsnake = RandomSnake();
    EatSnake esnake = EatSnake();
    HeuristicSnake hsnake = HeuristicSnake();

    game.addStrategy(&rsnake);
    game.addStrategy(&rsnake);
    game.addStrategy(&esnake);
    game.addStrategy(&esnake);
    game.addStrategy(&hsnake);
    game.addStrategy(&hsnake);
    game.addStrategy(&hsnake);
    game.addStrategy(&hsnake);
    game.addStrategy(&hsnake);
    game.addStrategy(&hsnake);

    SDL_Handle handle = initSDL(game);

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
   