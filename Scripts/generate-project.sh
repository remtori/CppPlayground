#!/bin/sh

rm -rf Build

mkdir Build

cd Build

cmake .. -G Ninja
