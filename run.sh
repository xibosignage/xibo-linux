#!/bin/bash
cp -r /app/_build/. /build
./app/_build/player/player "$@"
# ./app/_build/player/tests/unit_tests