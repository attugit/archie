.PHONY: configure all release debug clean distclean

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

