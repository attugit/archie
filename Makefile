.PHONY: configure all release debug clean distclean run format

all: debug

configure:
	./premake4

debug:
	make --directory=build config=debug

release:
	make --directory=build config=release

clean:
	make --directory=build clean

distclean:
	rm -rf build

format:
	clang-format -i `find test/ -name *.cpp` `find include/ -name *.h`
