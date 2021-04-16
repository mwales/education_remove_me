#!/bin/bash

SECRET_SCRIPT=`echo ZWNobyB5b3VfbXVzdF9oYXZlX3J1bl9leDMgPiB3dGYudHh0Cg== | base64 -d`

echo "Oh hey, this script really doesn't do anything, sorry"

eval ${SECRET_SCRIPT}
