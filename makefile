whatif : whatif.cpp deps.cpp
	g++ -std=c++11 $^ -o $@
clean: whatif
	rm -f $^
