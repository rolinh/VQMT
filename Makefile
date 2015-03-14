all:
	test -d build || mkdir build
	cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make

debug:
	test -d build || mkdir build
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make

clean:
	rm -rf build

.PHONY: all debug clean
