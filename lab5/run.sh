#!/bin/bash
python3 gentest.py > test.txt
./a.out < test.txt > MyRes.txt
python3 checker.py < test.txt > PythonRes.txt