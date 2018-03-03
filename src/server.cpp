#include "../crow/amalgamate/crow_all.h"
#include "strategies/minimax_snake.cpp"
#include "utils/server_utils.cpp"
#include "utils/profile.cpp"


using namespace crow;


int main(int argc, char **argv)
{
    int port = 7000;
    if (argc == 2) {
        port = atoi(argv[1]);
    }
    SimpleApp app;

    //INFO
    CROW_ROUTE(app, "/")([]() {
        return "test";
    });


    //START
    CROW_ROUTE(app, "/start")
    .methods("POST"_method)
    ([](const crow::request & req) {
        return SnakeInfo();
    });

    //MOVE
    CROW_ROUTE(app, "/move")
    .methods("POST"_method)
    ([](const crow::request & req) {
        profile prof(__FUNCTION__, __LINE__);
        MinimaxSnake minimax_snake = MinimaxSnake();
        pair<GameState, snake_index> info = parseGameState(req.body);
        GameState gs = info.first;
        assert(gs.isGivenValid());
        snake_index idx = info.second;
        Direction move = minimax_snake.decideMove(gs, idx);
        string resp = encodeMove(move);
        cout << resp << endl;
        return resp;
    });

    CROW_ROUTE(app, "/end")
    .methods("POST"_method)
    ([](const crow::request & req) {
        return "ok";
    });

    app.port(port).multithreaded().run();
}