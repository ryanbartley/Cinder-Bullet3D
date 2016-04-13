#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
rm -r $DIR/lib/

# using Bullet3 binaries from commit log 46b9f7fbee79d04048312fc1e8f364c51d5b2a04 on bullet3
curl -J -L -o lib.zip "http://b2.ge.tt/gett/6jmoecZ2/lib.zip?index=0&user=user-r8gtUrq763pID4m0auC…nVGvqSV9u0-&referrer=user-r8gtUrq763pID4m0auC0YV1GL6KnVGvqSV9u0-&download=" && \
unzip lib.zip && \
rm lib.zip && \
rm -rf __MACOSX

