programa: Secante.cpp
	c++ Secante.cpp -o programa -I ./exprtk -std=c++11 -O2 -Wa,-mbig-obj

run: programa
	./programa

clean:
	rm -f programa