#!/bin/bash
lcov --rc lcov_branch_coverage=1 -c -i -d ./build -o ./build/coverage/.coverage.base
lcov --rc lcov_branch_coverage=1 -c -d ./build -o ./build/coverage/.coverage.run
lcov --rc lcov_branch_coverage=1 -d ./build -a ./build/coverage/.coverage.base -a ./build/coverage/.coverage.run -o ./build/coverage/.coverage.total
#lcov --rc lcov_branch_coverage=1 -e ./build/coverage/.coverage.total */include/archie* -o ./build/coverage/.coverage.final 
lcov --rc lcov_branch_coverage=1 -e ./build/coverage/.coverage.total */utils* -o ./build/coverage/.coverage.total 
genhtml --rc lcov_branch_coverage=1 -o ./build/coverage ./build/coverage/.coverage.total
rm ./build/coverage/.coverage.base ./build/coverage/.coverage.run ./build/coverage/.coverage.total
