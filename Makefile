default:
	mkdir -p dist
	g++ -std=c++11 src/snek.cpp -o dist/snek

run:
	make
	dist/snek

test: 
	mkdir -p dist
	g++ -std=c++11 src/tests/tests.cpp -o dist/tests_snek /usr/lib/libgtest.a /usr/lib/libgtest_main.a -lpthread
	dist/tests_snek

gui:
	mkdir -p dist
	g++ -std=c++11 src/gui_main.cpp -o dist/snek -lSDL2main -lSDL2 -lSDL2_ttf
	dist/snek

clean:
	rm -rf dist

galgo: 
	mkdir -p dist
	g++ -std=c++11 GALGO-2.0/src/example.cpp -o dist/galgo
	dist/galgo

train: 
	mkdir -p dist
	g++ -std=c++11 src/training.cpp -o dist/train -O3 
	dist/train
