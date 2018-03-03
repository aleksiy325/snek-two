#include "gui/board_render.cpp"
#include "strategies/random_snake.cpp"
#include "strategies/eat_snake.cpp"
#include "strategies/heuristic_snake.cpp"
#include "strategies/minimax_snake.cpp"
#include "arena/game.cpp"

const int num_hsnake = 4;
const int board_width = 20;
const int board_height = 20;
const int max_food = 20;

int main(int argc, char **argv){
    Game game = Game(board_width, board_height, max_food);

    // RandomSnake rsnake = RandomSnake();
    EatSnake esnake = EatSnake();
    MinimaxSnake hsnake = MinimaxSnake();

    // game.addStrategy(&rsnake);
    // game.addStrategy(&rsnake);
    // game.addStrategy(&esnake);
    // game.addStrategy(&esnake);

    for(int i = 0; i < num_hsnake; i++){
        game.addStrategy(&hsnake);
    }

    SDL_Handle handle = initSDL(game);

    bool quit = false;
    SDL_Event e;

    Uint32 movesPerSecond = 100;
    Uint32 timePerFrame = 1000 / movesPerSecond;
    Uint32 prevTime = SDL_GetTicks();

    int nextGameDelayFrames = 0;
    while(!quit){
        Uint32 curTime = SDL_GetTicks();
        Uint32 deltaTime = curTime - prevTime;
        if ((deltaTime) > timePerFrame){
            prevTime = curTime;
            if(nextGameDelayFrames <= 0){
              game.executeTick();
              if(game.winnerExists()){
                nextGameDelayFrames = 10;
              }
            }
            else if(nextGameDelayFrames == 1){
              game.resetGame(board_width, board_height, max_food, time(NULL));
              nextGameDelayFrames--;
            }
            else{
              nextGameDelayFrames--;
            }
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
