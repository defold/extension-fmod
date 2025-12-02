FROM emscripten/emsdk:4.0.16

VOLUME [ "/repo" ]
WORKDIR /repo/bridge

ENTRYPOINT [ "make", "-f", "Makefile.emscripten" ]
