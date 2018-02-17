#include "game_state.cpp"
#include "gui/board_render.cpp"

int main(int argc, char **argv){
    GameState gs = GameState(10, 10, 0);
    gs.addSnake(Point(1, 1));
    gs.addSnake(Point(6, 5));
    gs.board.print();
    gs.makeMove(Direction::North, 0);
    gs.cleanup();
    gs.board.print();
    gs.makeMove(Direction::East, 0);
    gs.cleanup();
    gs.board.print();

    SdlHandle sdl_handle = initSDL(gs.board);

    bool quit = false;
    SDL_Event e;

    while(!quit){
        renderBoard(sdl_handle.renderer, gs.board);

        while(SDL_PollEvent(&e) != 0){
          if(e.type == SDL_QUIT){
            quit = true;
          }
        }
    }

    cleanupSDL(sdl_handle);
}
   