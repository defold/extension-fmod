FROM emscripten/emsdk:3.1.67-upstream

VOLUME [ "/repo" ]
WORKDIR /repo/bridge

ENTRYPOINT [ "make", "-f", "Makefile.emscripten" ]
