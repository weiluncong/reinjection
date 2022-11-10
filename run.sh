#!/bin/bash
readonly curPath=$PWD
export LD_LIBRARY_PATH=$curPath:$curPath/third_party/:$curPath/third_party/python3.7/lib:$curPath/third_party/Qt/lib:$curPath/third_party/yaml:$curPath/third_party/docker/GL:$curPath/mybuild

export PYTHONHOME=./third_party/python3.7m/dependent/python3.7
export PYTHONPATH=./third_party/python3.7m/dependent/python3.7

./reinjection_
