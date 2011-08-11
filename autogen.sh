#!/bin/bash
set -e
srcdir=$(dirname $0)
here=$(pwd)
cd $srcdir
mkdir -p config.aux
aclocal
autoconf
autoheader
automake -a || true		# for INSTALL
automake --foreign -a
