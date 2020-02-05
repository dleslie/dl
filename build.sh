#!/bin/bash

if [ ! -d build ];
then
	meson build
fi

pushd build && \
	ninja && \
	ninja -t compdb c_COMPILER cxx_COMPILER > ../compile_commands.json
popd
