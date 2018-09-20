#!/bin/sh

compiler='g++'
sources='main.cpp'
headers='network/*.hpp'
flags='-std=c++14'
output='bin/httpTest'

$compiler $sources $headers $flags -o $output