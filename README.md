# snek-two

Winner of [Battlesnake 2018](https://www.battlesnake.io/) Advanced Division. This is a complete rewrite based on things learned from a first attempt of writing a [C++ snake](https://github.com/rdbrck/battlesnake-2017-aleksiys-snake) for Battlesnake 2017. This time the focus was on building a good base of data structures to keep the state of the game. In addition we wanted to be able to simulate games locally for testing and training. Finally, by providing a good base of data structures with well defined interfaces writing various algorithms becomes a much easier process, something that was much harder in the first version.

## General Overview
The snake uses the [minimax](https://en.wikipedia.org/wiki/Minimax) with [alpha-beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) to make all of its decisions. A heuristic function is used to evaluate every game state. The heuristic function takes multiple weights to balance the terms of the heuristic function which are tuned by a genetic algorithm and then were hand tuned based on perceived flaws.

### Heuristic Function
The heuristic function is a points system used to score the current state of the board for a snake. Its range is the the maximum and minimum of the double type in C++. Right away we can return the minimum if we are dead in this state and the maximum if we are a winner. Otherwise, we used two main heuristics to evaluate a game state: how close we are to food in relation with our health and the number of squares we own.

#### Food
Food is found by using [Breadth first search](https://en.wikipedia.org/wiki/Breadth-first_search). From there we can use the size of the path to get distance to this food. It is obvious that food and health is related. In principle we probably do not want to eat food when we have high health and do want to eat food when we are low health. We decided to simply minus the length to the closest food from our health and then use a arctan function with two weights to get a score. The final equation was *score = a \* arctan((h - l) / b)* where *h* is the current health, *l* is the path to nearest food and *a* and *b* are constants chosen by the genetic algorithm. The final weights for the constants was *a = 68.60914* and *b=8.51774*. 

![Arc Tan](/arc_tan.png?raw=true)


### Voronoi
[Voronoi](https://en.wikipedia.org/wiki/Voronoi_diagram) is a concept that we read in [a1k0n's post mortem](https://www.a1k0n.net/2010/03/04/google-ai-postmortem.html) for the Google AI lightbike challenge. Voronoi allows us to introduce a heuristic for maximizing the amount of points we can reach before anyone. It follows that we also minimize our opponents space which is extremely useful in lategame one on one, as well as surviving in multiopponent situations. A visualization from aik0n's blog post shows how it works.

![Voronoi](https://www.a1k0n.net/img/voronoi.gif)

Voronoi can easily implemented by doing BFS from the head of every snake one depth at a time. We score this heuristic by giving a point for every owned space and then multiplying by a constant as found by the genetic algorithm. In our case this constant was *7.60983*. 

## Training
By building a test arena in C++ we were able to test different heuristics for our snake. We had four hyper parameters in our heuristic, and to converge to a solution faster we chose a [genetic algorithm](https://en.wikipedia.org/wiki/Genetic_algorithm) approach. We found a promising library in the form of [GALGO](https://github.com/olmallet81/GALGO-2.0). Unfortunately, GALGO performed a fitness function on each candidate independently. We had to modify the library to perform perform the fitness function evaluation on the population as a whole. Each candidate solution in the population was initialized into a snake game of other candidates as well as some basic snakes. These basic snakes were snakes that moved randomly and snakes that always ate the nearest food. The candidates where initialized as snakes without alpha-beta look ahead, moving only based on their heuristic. Each population ran the game 10 times.

### Fitness Function



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

