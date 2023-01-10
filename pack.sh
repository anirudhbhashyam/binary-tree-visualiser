#!/bin/bash 

set -xe -o pipefail

CC=gcc
filename=${1%.*}
CFLAGS="-Wall -Wextra -std=c11"

${CC} ${CFLAGS} -o ${filename} ${1}