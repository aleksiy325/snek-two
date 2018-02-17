#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include "../common/board.cpp"
#include "../common/defs.cpp"
#undef main

int BOX_PIXELS = 30;
int BOX_SPACE = 8;

struct SdlHandle{
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
};

SdlHandle initSDL(Board& board){
    int xSize = board.getWidth();
    int ySize = board.getHeight();

    SdlHandle sdl_handle;

    int screenWidth = (xSize * BOX_PIXELS) + ((xSize + 1) * BOX_SPACE);
    int screenHeight = (ySize * BOX_PIXELS) + ((ySize + 1) * BOX_SPACE);
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
    }
    else{
    sdl_handle.window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

    sdl_handle.renderer = SDL_CreateRenderer(sdl_handle.window, -1, SDL_RENDERER_ACCELERATED);
    }
    return sdl_handle;
}


map<CellType, SDL_Color> CELL_COLOR_MAP {
    {CellType::empty, { 180,180,180,0 }},
    {CellType::wall, {255,255,255,0}},
    {CellType::food, {233,10,10,0}}
};

int renderBoard(SDL_Renderer* renderer, Board& board){
    SDL_Color bg_color = { 220,220,220,0 };

    SDL_Color snake_colors[8];
    int index = 0;
    for(int i = 1; i <= 2; i++){
        for(int j = 1; j <= 2; j++){
            for(int k = 1; k <= 2; k++){
                snake_colors[index] = {i*50, j*50, k*50, 0};
                index++;
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderClear(renderer);
    cout << board.getPoints().size() << endl;

    for( auto point: board.getPoints()){ 
        SDL_Color cur_color;

        if(board.cellNumOccupants(point) > 0){
          cur_color = snake_colors[*board.getCellOccupants(point).begin()];

        }else{
             cur_color = CELL_COLOR_MAP[board.getCellType(point)];
        }

        SDL_SetRenderDrawColor(renderer, cur_color.r, cur_color.g, cur_color.b, cur_color.a);
            SDL_Rect rect = {
            BOX_SPACE + (point.x * (BOX_SPACE+BOX_PIXELS)),
            BOX_SPACE + (point.y * (BOX_SPACE+BOX_PIXELS)),
            BOX_PIXELS,
            BOX_PIXELS
        };
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
    return 0;
}

void cleanupSDL(SdlHandle handle){
    SDL_DestroyWindow(handle.window);
    SDL_Quit();
}