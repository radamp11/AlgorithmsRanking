#!/bin/bash
#ZIPLIB
wget -O ziplib https://bitbucket.org/wbenny/ziplib/get/176e4b6d51fc.zip
unzip ziplib -d FileManagement/lib
rm ziplib
mv FileManagement/lib/wbenny-ziplib-176e4b6d51fc FileManagement/lib/ziplib

sed -i 's/#CC       = gcc/CC       = gcc/' FileManagement/lib/ziplib/Makefile
sed -i 's/#CXX      = g++-4.8/CXX      = g++/' FileManagement/lib/ziplib/Makefile
sed -i '8,9d' FileManagement/lib/ziplib/Makefile

make -C FileManagement/lib/ziplib


#instalacja django

#sudo apt-get install python3-venv
#python3 -m venv env
#source env/bin/activate
#sudo pip3 install django
#sudo pip3 install matplotlib
#sudo pip3 install mpld3
#python3 manage.py makemigrations algorithms
#python3 manage.py migrate
