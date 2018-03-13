# snek-two

Winner of [Battlesnake 2018](https://www.battlesnake.io/) Advanced Division. This is a complete rewrite based on things learned from a first attempt of writing a [C++ snake](https://github.com/rdbrck/battlesnake-2017-aleksiys-snake) for Battlesnake 2017. This time the focus was on building a good base of data structures to keep the state of the game. In addition we wanted to be able to simulate games locally for testing and training. Finally, by providing a good base of data structures with well defined interfaces writing various algorithms becomes a much easier process, something we struggled with in the first version.

## General Overview
The snake uses the [minimax](https://en.wikipedia.org/wiki/Minimax) with [alpha-beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) to make all of its decisions. A heuristic function is used to evaluate every game state. The heuristic function takes multiple weights to balance the terms of the heuristic function which are tuned by a genetic algorithm and then were hand tuned based on perceived flaws.

### Heuristic Function
The heuristic function is a points system used to score the current state of the board for a snake. Its range is the the maximum and minimum of the double type in C++. Right away we can return the minimum if we are dead in this state and the maximum if we are a winner. Otherwise, we use three main heuristics to evaluate a game state: how close we are to food in relation with our health, the number of squares we own, how long are we. 

#### Food
Food is found by using [Breadth first search](https://en.wikipedia.org/wiki/Breadth-first_search). From there we can use the size of the path to get distance to this food. It is obvious that food and health is related. In principle we probably do not want to eat food when we have high health and do want to eat food when we are low health. We decided to simply minus the length to the closest food from our health and then use a arctan function with two weights to get a score. 

![Arc Tan](/arc_tan.png?raw=true)




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

