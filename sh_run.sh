#!/bin/sh
clear
echo ---------------------------
make clean
make
cd bin/ && ./server.bin &
cd bin/ && ./client.bin &

