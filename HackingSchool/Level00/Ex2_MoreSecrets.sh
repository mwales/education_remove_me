#!/bin/bash

echo "Usage of this script: $0 password"

secret_message=$(tail -n 1 $0 | base64 -d)
secret_password=$(tail -n 2 $0 | head -n 1 | base64 -d)

# Test to see if the first argument to the script matches the secret password
if [ $secret_password == $1 ]; then
	echo $secret_message
else
	echo "Wrong password!"
fi

# Exit script
exit 0

# Hmmmmm, I wonder what this stuff is down here?
SGFja1RoZUdpYnNvbgo=
VGhpcyByZWFsbHkgaXNuJ3QgYSBzZWNyZXQgaXRzIGp1c3QgYmFzZTY0IGVuY29kZWQgbWVzc2FnZS4gIENvdWxkIHlvdSBleHRyYWN0IHRoZSBtZXNzYWdlIHdpdGhvdXQga25vd2luZyB0aGUgcGFzc3dvcmQ/Cg==
