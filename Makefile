.PHONY: configure all release debug clean distclean run format

all: debug

configure:
	./waf configure

debug:
	./waf debug

release:
	./waf release

clean:
	./waf clean

distclean:
	./waf distclean

run: debug
	./waf --alltests

format:
	clang-format -i `find test/ -name *.cpp` `find include/ -name *.h`
