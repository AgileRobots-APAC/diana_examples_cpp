#!/usr/bin/env sh
# Copyright (c) 2026 Agile Robots SE
# SPDX-License-Identifier: MIT
set -eu

sdk_dir="${DIANA_SDK_DIR:-/workspace/third_party/diana_sdk_cpp}"
build_dir="/tmp/diana-api-examples-build"

if [ ! -f "${sdk_dir}/include/DianaAPI.h" ] || [ ! -f "${sdk_dir}/lib/libDianaApi.so" ]; then
    echo "SDK is missing. Initialize the submodule before building the image:" >&2
    echo "  git submodule update --init --recursive" >&2
    exit 2
fi

export LD_LIBRARY_PATH="${sdk_dir}/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"

echo "== Configure =="
mkdir -p "${build_dir}"
(
    cd "${build_dir}"
    cmake /workspace -DCMAKE_BUILD_TYPE=Release -DDIANA_SDK_DIR="${sdk_dir}"
)

echo "== Build all examples =="
jobs="${CMAKE_BUILD_PARALLEL_LEVEL:-$(getconf _NPROCESSORS_ONLN)}"
cmake --build "${build_dir}" -- -j"${jobs}"

echo "== Check SDK dynamic dependencies =="
dependency_output="$(ldd "${sdk_dir}/lib/libDianaApi.so" 2>&1 || true)"
printf '%s\n' "${dependency_output}"
if printf '%s\n' "${dependency_output}" | grep -q 'not found'; then
    echo "SDK runtime dependencies are missing on this Ubuntu version." >&2
    exit 3
fi

echo "== SDK load smoke test =="
"${build_dir}/bin/01_connection" --list-models

echo "Platform test passed. No robot connection or motion command was sent."
