ARG UBUNTU_VERSION=22.04
ARG APT_MIRROR=http://mirrors.aliyun.com/ubuntu/
FROM ubuntu:${UBUNTU_VERSION}

ARG UBUNTU_VERSION
ARG APT_MIRROR
ENV DEBIAN_FRONTEND=noninteractive

# Use a mainland mirror before updating packages. Override APT_MIRROR with
# https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ after the base image has its
# certificate bundle available.
RUN sed -i \
        -e "s|http://archive.ubuntu.com/ubuntu/|${APT_MIRROR}|g" \
        -e "s|http://security.ubuntu.com/ubuntu/|${APT_MIRROR}|g" \
        /etc/apt/sources.list \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        cmake \
        g++ \
        make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . /workspace
COPY docker/entrypoint.sh /usr/local/bin/diana-platform-test
RUN chmod +x /usr/local/bin/diana-platform-test

ENTRYPOINT ["/usr/local/bin/diana-platform-test"]
