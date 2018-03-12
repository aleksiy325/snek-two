
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
If you wish to be able to run the tests you will need GTEST.
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

