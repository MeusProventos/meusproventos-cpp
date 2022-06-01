all: meusproventos

meusproventos: src/main.cpp src/MeusProventos.hpp
	g++ --std=c++17 -Ithirdparty/ -Isrc/ -fsanitize=address src/main.cpp -o meusproventos -lpthread

exemplo: meusproventos
	./meusproventos --entrada exemplos/exemplo0.csv

test:
	cd tests && make test