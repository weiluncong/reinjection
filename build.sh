#! /bin/bash
root_path=$(pwd)
build_path=$root_path/mybuild

killall -9 reinjection_
rm $root_path/reinjection_

# if [ -e build ]
# then
#     cd build
#     rm -r ./*
#     echo "build exist! " 
# else
#     mkdir build
#     cd build
# fi

if [ -e mybuild ]
then
    if [ "$1" = "clean" ]
    then
        rm -r ./mybuild/*
        echo "mybuild is cleaned! "
    else
        echo "mybuild exist! " 
    fi
    # rm -r ./*
else
    mkdir mybuild
fi

cd $build_path

cmake ..
make -j9
# make package
cp reinjection_ ..
cp -r ../dbc .

