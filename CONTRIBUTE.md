# Contribution guide

If you have some improvements to bring the project, feel free to open a PR.

Please stick to the indentation style used throughout the project (K&R-like).

## External dependencies

| Tool | Platform | Description |
|------|----------|-------|
| `gcc` | All | C preprocessor for parsing FMOD headers |
| `codesign` | macOS only | Signs extracted `.dylib` and `.a` |
| `hdiutil` | macOS only | Mounts `.dmg` archives for macOS/iOS libraries |
| `7z` or `unar` | All | Extracts Windows `.exe` installers (auto-detected) |
| `ar` | All | Combines split WASM `.a` archives for HTML5 |
| `patchelf` | Linux only | Workaround: strips versioned SONAME from `.so` files (e.g. `libfmod.so.14` → `libfmod.so`) so Defold's `dynamicLibs` can resolve them |

Python packages (install with `pip install -r cicd/requirements.txt`):

## CI/CD tool

All build, update, and test tasks are managed through `cicd/build.py`:

```bash
python cicd/build.py --help
```

### Checking your environment

Run the health check to verify all required tools are installed:

```bash
python cicd/build.py health
```

This checks for every external dependency listed above and reports any that are
missing, along with install hints.

### Generating bindings

The C/Lua bindings are auto-generated from FMOD headers. To regenerate:

```bash
pip install -r cicd/requirements.txt
python cicd/build.py generate
```

This produces `fmod/src/fmod_generated.c` and `fmod/api/fmod.script_api`.

### Running tests

```bash
python cicd/build.py test
```

### Linting code

```
./cicd/build.py lint
or
python cicd/build.py lint
```

### Format code

```
./cicd/build.py format
or
python cicd/build.py format
```

## Updating FMOD to a newer version

Download all the platform distributions from https://fmod.com/download into
a single directory, then point the update command at it:

```bash
python cicd/build.py update ~/Downloads/fmod20309/
```

The script discovers archives by filename pattern. Platforms
without a matching archive are skipped.

On non-macOS systems (where `hdiutil` is unavailable), skip Apple platforms
with `--ignore-apple`:

```bash
python cicd/build.py update --ignore-apple ~/Downloads/fmod20309/
```

After updating, regenerate the bindings:

```bash
python cicd/build.py generate
```

## Testing HTML5/WASM pthread builds locally

WASM pthread builds require specific CORS headers to enable `SharedArrayBuffer`.
Regular `python3 -m http.server` will **not** work so please send required headers.

**Required headers:**
- `Cross-Origin-Opener-Policy: same-origin`
- `Cross-Origin-Embedder-Policy: require-corp`

So for python serving, you can add the request CORS handler like
```python3
class CORSRequestHandler(SimpleHTTPRequestHandler):
    (...)
    def end_headers(self):
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
```


**Build with both architectures** (loader needs both for fallback):

```bash
java -jar bob.jar build bundle --platform js-web \
  --architectures wasm-web,wasm_pthread-web \
  --bundle-format html5
  (...)
```

Validate in the console

```javascript
console.log("Pthread:", Module.isWASMPthreadSupported);  // Should be: true
```
