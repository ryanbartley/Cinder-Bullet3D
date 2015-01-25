#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
rm -r $DIR/lib/

# using Bullet3 binaries from commit log 46b9f7fbee79d04048312fc1e8f364c51d5b2a04 on bullet3
curl -J -L -o lib.zip https://googledrive.com/host/0B_9cvCpvdnyHa2lJWVBvNHVILVE && \
unzip lib.zip && \
rm lib.zip

