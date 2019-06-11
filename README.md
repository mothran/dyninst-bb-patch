# Simple patcher repro

## Building / setup

```bash
export DYNINST_ROOT=/path/to/dyninst
export LD_LIBRARY_PATH=${DYNINST_ROOT}/lib
export DYNINSTAPI_RT_LIB=${LD_LIBRARY_PATH}/libdyninstAPI_RT.so
make
```

## Patching

```bash
./patcher /path/to/target /path/to/output_bin
```

## Running

```bash
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:. /path/to/output_bin
```

## Test requirements

### yaml

Ubuntu 18.04 package:

* libyaml-dev
