#!/bin/bash

./waf distclean configure build install --prefix=/usr;
if [[ "$MODE" == "coverage" ]]; then
  virtualenv --python=python3 gcovr;
  source gcovr/bin/activate;
  pip install gcovr;
  ln -s `which gcov-6` ./gcovr/bin/gcov;
  gcovr --exclude=test -sr .;
  gcovr --exclude=test -sbr .;
  deactivate;
fi
