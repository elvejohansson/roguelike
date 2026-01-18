#!/bin/sh

set -e

cmake --build build --preset debug

./build/debug/src/Game
