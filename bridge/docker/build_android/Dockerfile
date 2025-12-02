FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# basic tools
RUN apt-get update && apt-get install -y \
    wget \
    unzip \
    openjdk-11-jdk \
    make \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Android NDK r25b (same version as Defold's build server)
ENV ANDROID_NDK_HOME=/opt/android-ndk-r25b
RUN cd /opt && \
    wget -q https://dl.google.com/android/repository/android-ndk-r25b-linux.zip && \
    unzip -q android-ndk-r25b-linux.zip && \
    rm android-ndk-r25b-linux.zip

ENV PATH="${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/bin:${PATH}"

VOLUME [ "/repo" ]
WORKDIR /repo/bridge

ENTRYPOINT [ "make", "-f", "Makefile.android" ]
