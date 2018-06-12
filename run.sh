#!/bin/bash
cp -r /app/_build/. /build
./app/_build/player/player "$@"