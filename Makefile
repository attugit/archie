.PHONY: configure all release debug clean distclean run format

all: debug

configure:
	./premake4 gmake

debug:
	make config=debug --directory=build/

release:
	make config=release --directory=build/

clean:
	make --directory=build/ clean

distclean:
	rm -rf build/

run: debug
	./build/debug/bin/unittest

format:
	clang-format -i `find test/ -name *.cpp` `find include/ -name *.h`
