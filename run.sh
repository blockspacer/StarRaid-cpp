#!/bin/sh
clear
echo ---------------------------
make
#make clean
make run server
make run client
