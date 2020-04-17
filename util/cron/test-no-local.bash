#!/usr/bin/env bash
#
# Test --no-local configuration on full suite.

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="no-local"

nightly_args="${nightly_args} -no-local"
$CWD/nightly -cron ${nightly_args}
