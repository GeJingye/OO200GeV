#! /bin/tcsh
# dependent document: 
if($#argv != 3) then
   echo "Please inpput Three arguments!"
   echo "argv1 -- the particle species. For me, it should be 'Electron' or 'Positron' "
   echo "argv2 -- the production number ID. For me, it should be '100', or '200' "
   echo "argv3 -- the request ID "
   exit 0
endif

set myPath = /gpfs01/star/pwg/jge/OO200GeV2021/corrEff/readEmbedding
set infileDir = /star/data105/embedding/production_OO_200GeV_2021/$1_$2_$3/P23ic.SL23c/2021

set outfileDir = production/production_$1_$2
set scriptDir = script/script_$1_$2
set logDir = log/log_$1_$2
set errDir = err/err_$1_$2
set outDir = out/out_$1_$2
set jobDir = job_a9/

rm -rf $outfileDir $scriptDir $logDir $errDir $outDir $jobDir/runAll_$1_$2_a9.jdl
mkdir -p $myPath/$outfileDir $myPath/$scriptDir $myPath/$logDir $myPath/$errDir $myPath/$outDir $myPath/$jobDir
cp -p run_Header.jdl $jobDir/runAll_$1_$2_a9.jdl

@ nfile=0
foreach file (`find $infileDir/*/* -name 'st_physics_adc*.MuDst.root'`)
  #echo " *** $nfile ***"
  set baseName = `basename $file`

  # #!/bin/bash
  # date
  # echo $SHELL
  # starver SL19b
  # echo $STAR
  # root4star -b -l -q <<EOF
  # .O2
  # .x doEvent.C(1e9,"$file","$outfileDir")
  # .q
  # EOF 
  cp -p run_Header.csh run_tmp.csh
  echo "root4star -b <<EOF">>run_tmp.csh
  echo ".O2">>run_tmp.csh
  echo -n '.x doEvent.C(1e9,"'>>run_tmp.csh
  echo -n $file>>run_tmp.csh
  echo -n '","'>>run_tmp.csh
  echo -n $outfileDir>>run_tmp.csh
  echo '")'>>run_tmp.csh
  echo ".q">>run_tmp.csh
  echo "EOF">>run_tmp.csh
  mv run_tmp.csh $scriptDir/$baseName.csh

  cat >> $jobDir/runAll_$1_$2_a9.jdl << EOF
Executable       = /usr/bin/singularity
Arguments        = "exec -e -B /direct -B /star -B /afs -B /gpfs -B /sdcc/lustre02 /cvmfs/star.sdcc.bnl.gov/containers/rhic_sl7.sif $myPath/$scriptDir/$baseName.csh"
Output           = $outDir/$baseName.out
Error            = $errDir/$baseName.err
Log              = $logDir/$baseName.log
Queue
            
EOF

  #condor_submit $jobDir/runAll_$1_$2_$nfile.jdl

  @ nfile++

end
echo "*** Finish $1_$2 ***"
#condor_submit $jobDir/runAll_$1_$2_a9.jdl