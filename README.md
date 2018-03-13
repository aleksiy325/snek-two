# snek-two

Winner of [Battlesnake 2018](https://www.battlesnake.io/) Advanced Division. This is a complete rewrite based on things learned from a first attempt of writing a [C++ snake](https://github.com/rdbrck/battlesnake-2017-aleksiys-snake) for Battlesnake 2017. This time the focus was on building a good base of data structures to keep the state of the game. In addition we wanted to be able to simulate games locally for testing and training. Finally, by providing a good base of data structures with well defined interfaces writing various algorithms becomes a much easier process, something we struggled with in the first version.

## General Overview
The snake uses the [minimax](https://en.wikipedia.org/wiki/Minimax) with [alpha-beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) to make all of its decisions. A heuristic function is used to evaluate every game state. The heuristic function takes multiple weights to balance the terms of the heuristic function which are tuned by a genetic algorithm and then were hand tuned based perceived flaws.


## Building
After pulling the repository the submodules need to be pulled. This includes the Crow C++ Micro webserver, JSON for Modern C++, and GALGO genetic algorithm library.
```
git submodule init
git submodule update
```
Next Crow header file needs to be created.
```
cd crow/amalgamate 
python merge_all.py
```

### Main Dependencies
This project uses the C++ Boost libraries and SDL2 for GUI.
```
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libboost-all-dev
```

### Test Dependencies
If you wish to be able to run the tests you will need [googletest](https://github.com/google/googletest).
```
sudo apt-get install libgtest-dev
cd /usr/src/gtest
sudo cmake .
sudo make
sudo mv libg* /usr/lib/
```

## Running 
There are three main ways to run the project. 

Run a game with multiple snakes as defined in `src/gui_main.cpp`.
```
make gui
```

Run the genetic algorithm training as defined in `src/training.cpp`
```
make train
```

Finally run the server as defined in in `src/server.cpp`
```
make server
```

