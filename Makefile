.PHONY: configure all release debug clean distclean run

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

