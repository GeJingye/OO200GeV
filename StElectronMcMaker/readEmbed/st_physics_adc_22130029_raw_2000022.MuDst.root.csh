#!/bin/csh 
date
echo $SHELL
starver SL24a
echo $STAR

root4star -b <<EOF
.O2
.x doEvent.C(1e9,"/star/data105/embedding/production_OO_200GeV_2021/Electron_100_20250302/P23ic.SL23c/2021/130/22130029/st_physics_adc_22130029_raw_2000022.MuDst.root","production/production_Electron_100")
.q
EOF
