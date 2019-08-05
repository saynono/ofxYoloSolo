#!/bin/bash
set -e
BASEDIR=$(dirname "$0")
echo "$1"
FILE=$1
echo "BUILD NUMBER $(cat $1)";
awk -F, '{print("#define BUILD_VERSION " $1)}' OFS=, $FILE > $BASEDIR/../src/common/BuildVersion.h
awk -F, '{$1=$1+1;print}' OFS=, $FILE > $BASEDIR/../tmp.txt
#awk -F, '{$1=$1+1;print}' OFS=, $BASEDIR
mv $BASEDIR/../tmp.txt $FILE
