#!/bin/bash

export DPLibInclude=~/lib/
export DPLibLib=~/lib/lib.a

make -f Make $(1)
exit
