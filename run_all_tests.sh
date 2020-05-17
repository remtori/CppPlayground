#!/bin/sh
find ./bin -maxdepth 2 -type f -name "Test*" -exec {} \;
