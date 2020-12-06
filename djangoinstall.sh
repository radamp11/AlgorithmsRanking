#!/bin/bash
#instaluje lokalne wirtualne srodowisko oraz lokalny framework django
sudo apt-get install python3-venv
python3 -m venv env
source env/bin/activate
pip3 install django
django-admin.py startproject ranking .
python3 manage.py migrate
#python3 manage.py runserver