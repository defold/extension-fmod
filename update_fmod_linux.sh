#!/bin/bash

echo "Updating FMOD, linux-able platforms"

set -e

PREFIX="${1}"
REPO="$PWD"
TMPDIR=$(mktemp -d)

YELLOW='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m'

echo -e "Temporary directory: ${YELLOW}$TMPDIR${NC}"
echo -e "Unzipping to ${YELLOW}$TMPDIR${NC}"

echo -e "${YELLOW}Updating Linux FMOD...${NC}"
mkdir -p "$TMPDIR/linux"
tar xzf "${PREFIX}linux${FMOD_PATCH_POSTFIX}.tar.gz" -C "$TMPDIR/linux" --strip-components 1
cp -L "$TMPDIR/linux/api/core/lib/x86_64/libfmod.so" "$REPO/fmod/res/x86_64-linux/libfmod.so"
cp -L "$TMPDIR/linux/api/studio/lib/x86_64/libfmodstudio.so" "$REPO/fmod/res/x86_64-linux/libfmodstudio.so"

echo -e "${YELLOW}Updating Windows FMOD...${NC}"
mkdir -p "$TMPDIR/win"
7z x "${PREFIX}win${FMOD_PATCH_POSTFIX}-installer.exe" -o"$TMPDIR/win"
find "$TMPDIR/win" -type f -path "*/api/core/lib/x64/fmod.dll" -exec cp {} "$REPO/fmod/res/x86_64-win32/fmod-x86_64/fmod.dll" \;
find "$TMPDIR/win" -type f -path "*/api/studio/lib/x64/fmodstudio.dll" -exec cp {} "$REPO/fmod/res/x86_64-win32/fmod-x86_64/fmodstudio.dll" \;
find "$TMPDIR/win" -type f -path "*/api/core/lib/x86/fmod.dll" -exec cp {} "$REPO/fmod/res/x86-win32/fmod-x86/fmod.dll" \;
find "$TMPDIR/win" -type f -path "*/api/studio/lib/x86/fmodstudio.dll" -exec cp {} "$REPO/fmod/res/x86-win32/fmod-x86/fmodstudio.dll" \;

echo -e "${YELLOW}Updating HTML5 FMOD...${NC}"
mkdir -p "$TMPDIR/html5"
unzip "${PREFIX}html5${FMOD_PATCH_POSTFIX}.zip" -d "$TMPDIR/html5"

# Regular WASM libraries (non-pthread)
# FMOD w32 libs are split - we need both _wasm.a and _bindings.a combined
echo -e "${YELLOW}Extracting regular WASM libraries...${NC}"
find "$TMPDIR/html5" -type f -path "*/api/studio/lib/w32/fmodstudio_wasm.a" -exec cp {} "$REPO/fmod/lib/web/libfmodstudio_wasm.a" \;
find "$TMPDIR/html5" -type f -path "*/api/studio/lib/w32/fmodstudio_bindings.a" -exec cp {} "$REPO/fmod/lib/web/libfmodstudio_bindings.a" \;

# Combine split libraries into single archive
cd "$REPO/fmod/lib/web"
ar -M <<EOF
CREATE libfmodstudio.a
ADDLIB libfmodstudio_wasm.a
ADDLIB libfmodstudio_bindings.a
SAVE
END
EOF
rm libfmodstudio_wasm.a libfmodstudio_bindings.a

# Pthread WASM libraries
echo -e "${YELLOW}Extracting pthread WASM libraries...${NC}"
mkdir -p "$REPO/fmod/lib/wasm_pthread-web"
find "$TMPDIR/html5" -type f -path "*/api/studio/lib/w32/fmodstudioP_wasm.a" -exec cp {} "$REPO/fmod/lib/wasm_pthread-web/libfmodstudio_wasm.a" \;
find "$TMPDIR/html5" -type f -path "*/api/studio/lib/w32/fmodstudioP_bindings.a" -exec cp {} "$REPO/fmod/lib/wasm_pthread-web/libfmodstudio_bindings.a" \;

# Combine pthread split libraries into single archive
cd "$REPO/fmod/lib/wasm_pthread-web"
ar -M <<EOF
CREATE libfmodstudio.a
ADDLIB libfmodstudio_wasm.a
ADDLIB libfmodstudio_bindings.a
SAVE
END
EOF
rm libfmodstudio_wasm.a libfmodstudio_bindings.a
cd "$REPO"


echo -e "${YELLOW}Updating Android FMOD...${NC}"
mkdir -p "$TMPDIR/android"
tar xzf "${PREFIX}android${FMOD_PATCH_POSTFIX}.tar.gz" -C "$TMPDIR/android" --strip-components 1
cp -L "$TMPDIR/android/api/core/lib/arm64-v8a/libfmod.so" "$REPO/fmod/res/android/lib/arm64-v8a/libfmod.so"
cp -L "$TMPDIR/android/api/studio/lib/arm64-v8a/libfmodstudio.so" "$REPO/fmod/res/android/lib/arm64-v8a/libfmodstudio.so"
cp -L "$TMPDIR/android/api/core/lib/armeabi-v7a/libfmod.so" "$REPO/fmod/res/android/lib/armeabi-v7a/libfmod.so"
cp -L "$TMPDIR/android/api/studio/lib/armeabi-v7a/libfmodstudio.so" "$REPO/fmod/res/android/lib/armeabi-v7a/libfmodstudio.so"
cp -L "$TMPDIR/android/api/core/lib/fmod.jar" "$REPO/fmod/lib/android/fmod.jar"

echo -e "${YELLOW}Copying FMOD headers...${NC}"
rm -f "$REPO/bridge/include/fmod"*
cp "$TMPDIR/linux/api/core/inc/"*.{h,hpp} "$REPO/bridge/include/"
cp "$TMPDIR/linux/api/studio/inc/"*.{h,hpp} "$REPO/bridge/include/"

rm -rf "$TMPDIR"
echo -e "${GREEN}Done!${NC}"
