#!/usr/bin/env sh
# Copyright (c) 2026 Agile Robots SE
# SPDX-License-Identifier: MIT
set -eu

usage() {
    echo "Usage: $0 {18.04|20.04|22.04|all}" >&2
    exit 2
}

version="${1:-}"
case "${version}" in
    all)
        for item in 18.04 20.04 22.04; do
            "$0" "${item}"
        done
        exit 0
        ;;
    18.04|20.04|22.04)
        ;;
    *)
        usage
        ;;
esac

if [ ! -f third_party/diana_sdk_cpp/include/DianaAPI.h ] || [ ! -f third_party/diana_sdk_cpp/lib/libDianaApi.so ]; then
    echo "DianaApi submodule is not initialized." >&2
    echo "Run: git submodule update --init --recursive" >&2
    exit 3
fi

image="diana-api-examples:ubuntu-${version}"
apt_mirror="${APT_MIRROR:-http://mirrors.aliyun.com/ubuntu/}"
echo "== Build test image for Ubuntu ${version} =="
docker build \
    --build-arg "UBUNTU_VERSION=${version}" \
    --build-arg "APT_MIRROR=${apt_mirror}" \
    --tag "${image}" \
    --file docker/Dockerfile .

echo "== Run platform test for Ubuntu ${version} =="
docker run --rm "${image}"
