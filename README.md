#Gtest Install 

sudo apt-get install libgtest-dev

cd /usr/src/gtest
sudo cmake .
sudo make
sudo mv libg* /usr/lib/