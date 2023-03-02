#! /bin/bash

FILE="$1"

if [ -z "$FILE" ] ; then

    root -l tpot.C\(0\)
else

    root -l tpot.C\(\"$FILE\"\)
fi
