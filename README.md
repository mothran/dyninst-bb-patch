# Simple patcher repro

## Building

```bash
DYNINST_ROOT=/path/to/dyninst make
```

## Patching

```bash
export LD_LIBRARY_PATH=/path/to/dyninstroot/
export DYNINSTAPI_RT_LIB=${LD_LIBRARY_PATH}/libdyninstAPI_RT.so
./patcher /path/to/target /path/to/output_bin
```

## Running

```bash
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:. /path/to/output_bin
```