#!/bin/bash
wget -O ziplib https://bitbucket.org/wbenny/ziplib/get/176e4b6d51fc.zip
unzip ziplib -d lib
rm ziplib
mv lib/wbenny-ziplib-176e4b6d51fc lib/ziplib

sed -i 's/#CC       = gcc/CC       = gcc/' lib/ziplib/Makefile
sed -i 's/#CXX      = g++-4.8/CXX      = g++/' lib/ziplib/Makefile
sed -i '8,9d' lib/ziplib/Makefile

make -C lib/ziplib

#g++ -std=c++11 main.cpp ../lib/ziplib/Bin/libzip.so -pthread      kompilacja fileManagement
#gcc -c sqlite.c -lpthread -ldl     			 	   kompilacja sqlite
