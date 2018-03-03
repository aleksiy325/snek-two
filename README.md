#Gtest Install 

sudo apt-get install libgtest-dev

cd /usr/src/gtest

sudo cmake .

sudo make

sudo mv libg* /usr/lib/

#SDL2
sudo apt-get install libsdl2-ttf-dev

#boost
sudo apt-get install libboost-all-dev