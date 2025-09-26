FROM saschpe/android-ndk:35-jdk22.0.2_9-ndk27.0.12077973-cmake3.22.1

RUN apt-get update && apt-get install make -y

VOLUME [ "/repo" ]
WORKDIR /repo/bridge

ENTRYPOINT [ "make", "-f", "Makefile.android" ]
