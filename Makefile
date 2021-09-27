

test: test.o
	g++ -shared -fPIC open_close_wrapper.cpp -o libopen_close_wrapper.so -ldl
	g++ $< -L ./ -lopen_close_wrapper -g -o $@

test.o: test.cpp
	g++ -c -g $< -o $@

clean:
	rm libopen_close_wrapper.so test test.o

