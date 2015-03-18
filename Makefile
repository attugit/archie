.PHONY: configure all release debug clean distclean run format

all:
	make --directory=build -f Makefile

configure:
	./premake4

debug:
	make config=debug --directory=build -f Makefile

release:
	make config=release --directory=build -f Makefile

clean:
	make clean --directory=build -f Makefile

distclean:
	rm -rf build

format:
	clang-format -i `find test/ -name *.cpp` `find include/ -name *.h`
