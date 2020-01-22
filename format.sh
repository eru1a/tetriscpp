#!/bin/sh
ls *.h *.cpp | xargs clang-format -i
