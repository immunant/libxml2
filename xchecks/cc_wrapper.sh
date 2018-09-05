#!/bin/sh
#
# Compiler wrapper script for cross-checks
# Pass to autogen.sh when running it:
# $ ./autogen.sh CC=./xchecks/cc_wrapper.sh [other autogen arguments...]

SCRIPT_DIR=`dirname $0`
C2RUST_DIR=`readlink -f $SCRIPT_DIR/../../../..`
MACHINE=`uname -n`
CC_WRAPPER="$C2RUST_DIR/cross-checks/c-checks/clang-plugin/cc_wrapper.sh"
CC="$C2RUST_DIR/dependencies/llvm-6.0.1/build.$MACHINE/bin/clang"
XCHECK_PLUGIN="$C2RUST_DIR/dependencies/clang-xcheck-plugin.$MACHINE/plugin/CrossChecks.so"
XCHECK_RUNTIME="$C2RUST_DIR/dependencies/clang-xcheck-plugin.$MACHINE/runtime/libruntime.a"
#FAKECHECKS_PATH="$C2RUST_DIR/cross-checks/libfakechecks"
FAKECHECKS_PATH="$C2RUST_DIR/cross-checks/rust-checks/target/release"

exec "$CC_WRAPPER" "$CC" "$XCHECK_PLUGIN" "$XCHECK_RUNTIME" \
    -ffunction-sections \
    "$@" \
    -L$FAKECHECKS_PATH -lfakechecks_zstd -lpthread -Wl,-rpath,$FAKECHECKS_PATH \
    -Xclang -plugin-arg-crosschecks -Xclang -C$SCRIPT_DIR/libxml2_c.yaml
