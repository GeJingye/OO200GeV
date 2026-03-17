#!/bin/bash
date

echo $SHELL
starver SL19b
echo $STAR
root4star -b -l -q <<EOF
.O2
.x doEvent.C(1e9,"$file","$outfileDir")
.q
EOF
