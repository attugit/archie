#!/bin/bash

./waf distclean configure build_${BUILDTYPE} install_${BUILDTYPE} --testcmd="valgrind --error-exitcode=1 %s" --prefix=/usr
