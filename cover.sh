#!/bin/bash
lcov --rc lcov_branch_coverage=1 -c -i -d ./build -o ./build/.coverage.base
lcov --rc lcov_branch_coverage=1 -c -d ./build -o ./build/.coverage.run
lcov --rc lcov_branch_coverage=1 -d ./build -a ./build/.coverage.base -a ./build/.coverage.run -o ./build/.coverage.total
lcov --rc lcov_branch_coverage=1 -e ./build/.coverage.total */include/archie* -o ./build/.coverage.total 
genhtml --rc lcov_branch_coverage=1 -o ./build/cover ./build/.coverage.total
rm ./build/.coverage.base ./build/.coverage.run ./build/.coverage.total
