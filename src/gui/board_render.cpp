#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "../common/board.cpp"
#include "../common/defs.cpp"
#include "../arena/arena.cpp"
#undef main

const int BOX_PIXELS = 30;
const int BOX_SPACE = 8;
const int SCORE_WIDTH = 200;

struct SDL_Handle{
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int board_width;
    TTF_Font* font = NULL;
};

map<CellType, SDL_Color> CELL_COLOR_MAP {
    {CellType::empty, { 180,180,180,0 }},
    {CellType::wall, {255,255,255,0}},
    {CellType::food, {233,10,10,0}}
};

vector<SDL_Color> initSnakeColors(){
    vector<SDL_Color> snake_colors;
    for(int i = 1; i <= 2; i++){
        for(int j = 1; j <= 2; j++){
            for(int k = 1; k <= 2; k++){
                snake_colors.push_back({i*50, j*50, k*50, 0});
            }
        }
    }
    return snake_colors;
}

vector<SDL_Color> SNAKE_COLORS = initSnakeColors();

SDL_Handle initSDL(Arena& arena){
    int xSize = arena.getWidth();
    int ySize = arena.getHeight();
    SDL_Handle handle;
    handle.board_width = (xSize * BOX_PIXELS) + ((xSize + 1) * BOX_SPACE);

    int screenWidth = handle.board_width + SCORE_WIDTH;
    int screenHeight = (ySize * BOX_PIXELS) + ((ySize + 1) * BOX_SPACE);
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
    }
    else{
        handle.window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
        handle.renderer = SDL_CreateRenderer(handle.window, -1, SDL_RENDERER_ACCELERATED);
    }

    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
    }

    handle.font = TTF_OpenFont("font/OpenSans-Regular.ttf", 16);
    if (handle.font == NULL){
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }

    return handle;
}


void renderBoard(SDL_Handle handle, Board& board){
    for(auto point: board.getPoints()){ 
        SDL_Color cur_color;

        if(board.cellNumOccupants(point) > 0){
            cur_color = SNAKE_COLORS[*board.getCellOccupants(point).begin()];

        }else{
            cur_color = CELL_COLOR_MAP[board.getCellType(point)];
        }

        SDL_SetRenderDrawColor(handle.renderer, cur_color.r, cur_color.g, cur_color.b, cur_color.a);
        SDL_Rect rect = {
            BOX_SPACE + (point.x * (BOX_SPACE+BOX_PIXELS)),
            BOX_SPACE + (point.y * (BOX_SPACE+BOX_PIXELS)),
            BOX_PIXELS,
            BOX_PIXELS
        };
        SDL_RenderFillRect(handle.renderer, &rect);
    }
}

void renderScoreboard(SDL_Handle handle, vector<Snake> snakes){
    SDL_Color text_color = {0, 0, 0, 0};

    int index = 0;
    for(auto snake: snakes){
        SDL_Color cur_color = SNAKE_COLORS[index];
        SDL_SetRenderDrawColor(handle.renderer, cur_color.r, cur_color.g, cur_color.b, cur_color.a);
        SDL_Rect rect = {
            handle.board_width + BOX_SPACE,
            BOX_SPACE + (index * (BOX_SPACE + BOX_PIXELS)),
            BOX_PIXELS,
            BOX_PIXELS
        };
        SDL_RenderFillRect(handle.renderer, &rect);

        string str = "";
        str += to_string(snake.getHealth());
        str += " ";

        if(snake.isAlive()){
            str += "Alive";
        }else{
            str += "Dead";
        }

        SDL_Surface* surface = TTF_RenderText_Solid(handle.font, str.c_str(), text_color);
        SDL_Texture* message = SDL_CreateTextureFromSurface(handle.renderer, surface);
        SDL_Rect message_rect = {
            handle.board_width + BOX_SPACE + BOX_PIXELS + BOX_SPACE,
            BOX_SPACE + (index * (BOX_SPACE + BOX_PIXELS)),
            surface->w,
            surface->h,
        };

        SDL_RenderCopy(handle.renderer, message, NULL, &message_rect);
        SDL_FreeSurface(surface);
        index++;
    }

}

void renderArena(SDL_Handle handle, Arena& arena){
    Board board = arena.getBoard();
    GameState gs = arena.getGameState();
    vector<Snake> snakes = gs.getSnakes();
    SDL_Color bg_color = { 220, 220, 220, 0 };
    SDL_SetRenderDrawColor(handle.renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderClear(handle.renderer);
    renderBoard(handle, board);
    renderScoreboard(handle, snakes);
    SDL_RenderPresent(handle.renderer);
}

void cleanupSDL(SDL_Handle handle){
    SDL_DestroyWindow(handle.window);
    SDL_Quit();
}