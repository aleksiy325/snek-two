#pragma once
#include "../../json/single_include/nlohmann/json.hpp"
#include "../common/game_state.cpp"

using JSON = nlohmann::json;

string SnakeInfo() {
    JSON info;
    info["color"] = "#000F00";
    info["secondary_color"] = "#000F00";
    info["head_url"] = "http://pets.wilco.org/Portals/7/Containers/Pets2011/images/star.png";
    info["taunt"] = "C++ is a superior language";
    info["name"] = "leks";
    info["head_type"] = "pixel";
    info["tail_type"] = "pixel";
    return info.dump();
}

string encodeMove(Direction move) {
    JSON jmove;
    jmove["move"] = DIR_STR_MAP[move];
    return jmove.dump();
}

Point parsePoint(JSON point_json) {
    assert(point_json["object"] == "point");
    int x = point_json["x"];
    int y = point_json["y"];
    return Point(x + 1, y + 1);
}


Snake parseSnake(JSON snake_json, int turn) {
    assert(snake_json["object"] == "snake");
    int free_moves = FREE_MOVES - turn;
    if (free_moves < 0) {
        free_moves = 0;
    }
    int health = snake_json["health"];
    string id = snake_json["id"];
    Snake snake = Snake(health, turn, free_moves, id);

    for (auto point : snake_json["body"]["data"]) {
        Point p = parsePoint(point);
        // if (!snake.inSnake(p)) {
            snake.addPoint(p);
        // }
    }

    return snake;
}

pair<GameState, snake_index> parseGameState(string body) {
    JSON j = JSON::parse(body);
    assert(j["object"] == "world");

    int height = j["height"];
    int width = j["width"];
    int turn = j["turn"];
    GameState gs = GameState(width, height);

    for (auto food_json : j["food"]["data"]) {
        Point p = parsePoint(food_json);
        gs.addFood(p);
    }
    cout << j["you"]["id"];
    string id = j["you"]["id"];
    cout << id << endl;
    snake_index me = -1;
    for (auto snake_json : j["snakes"]["data"]) {
        Snake snake = parseSnake(snake_json, turn);
        snake_index cur = gs.addSnake(snake);
        if (snake.getID() == id) {
            me = cur;
        }
    }

    assert(me != -1);

    return make_pair(gs, me);
}

