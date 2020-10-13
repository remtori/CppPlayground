#!/bin/sh

find . \( -name Build -o -name Out \) -prune -o \( -name '*.cpp' -or -name '*.h' \) -print > CppPlayground.files
