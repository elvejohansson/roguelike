#!/bin/sh

set -e

cmake --build --preset debug
ctest --test-dir build/debug --output-on-failure
