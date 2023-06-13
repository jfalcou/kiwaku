#!/bin/bash

path=`pwd`

cd $path/kwk_def/

bash bench.sh

cd $path/standalone/

bash bench.sh

cd $path

bash plot.sh
