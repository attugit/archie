#!/bin/bash

./waf distclean configure build install --testcmd="valgrind --error-exitcode=1 %s" --prefix=/usr
