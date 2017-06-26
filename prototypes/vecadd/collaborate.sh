#! /bin/bash

make
rm -f foo

./vecadd > cpp.out &
python vecadd.py > py.out &

wait