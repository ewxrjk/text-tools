#!/bin/bash
set -e
srcdir=$(dirname $0)
here=$(pwd)
cd $srcdir
exec autoreconf -si "$@"
