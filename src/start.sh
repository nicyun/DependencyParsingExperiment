#!/bin/bash

#python train.py ../data/english_train.conll ../result/model
python test.py ../result/model ../data/english_test.conll  ../result/out.txt
