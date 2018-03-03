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

train: 
	mkdir -p dist
	g++ -std=c++11 src/training.cpp -o dist/train -O3
	dist/train

server:
	mkdir -p dist
	g++ -std=c++11 src/server.cpp -o dist/server -lpthread -lboost_system
	dist/server

crow:
	cd crow/amalgamate 
	python crow/amalgamate/merge_all.py
	
	
clean:
	rm -rf dist
