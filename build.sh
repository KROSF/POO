#!/usr/bin/env bash
cd $TRAVIS_BUILD_DIR/P0/
make test-auto && make test-P0-consola
