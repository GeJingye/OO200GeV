#! /bin/tcsh

if($#argv != 2) then
   echo "Please infile TWO arguments!"
   echo "argv0 -- the particle species. For me, it should be 'Electron' or 'Positron' "
   echo "argv1 -- the production number ID. For me, it should be '27', or '200' "
   exit 0
endif

set pwgPath = /star/data01/pwg/zaochen/analysis/Run18/Run18_27GeV/out_embd
set infileDir = /star/embed/embedding/27GeV_production_2018/$1_$2_20193201/P19ib.SL19b/2018
set outfileDir = out_$1_$2
set logDir     = log_$1_$2
set scriptDir  = script/script_$1_$2

mkdir -p $pwgPath/$outfileDir
mkdir -p $pwgPath/$logDir
mkdir -p $pwgPath/$scriptDir
mkdir -p $pwgPath/job

ln -s $pwgPath/$outfileDir ./
ln -s $pwgPath/$logDir ./
ln -s $pwgPath/$scriptDir ./
ln -s $pwgPath/job ./

rm -rf $scriptDir/*
rm -rf $logDir/*
rm -rf $outfileDir/*.root
rm -rf job/*

#初始化计数器，后面每成功提交一个 job 就 nfile++，最后可看到总任务数
@ nfile=0

#foreach file (`ls $infileDir/*/*/st_physics_adc*.geant.root`)
#遍历所有符合条件的root文件
foreach file (`find $infileDir/*/* -name 'st_physics_*.geant.root'`)
  echo " *** $nfile ***"

  cp run.con job/runAll$1_$2_$nfile.job

  set baseName = `basename $file`	#输出st_physics_adc_22131020_raw_6000013.geant.root
  set log = `basename $file`
  echo $baseName

  cp run.csh run_tmp.csh
  echo "root4star -b -l -q <<EOF">>run_tmp.csh
  echo ".O2">>run_tmp.csh
  echo -n '.x doEvent.C(1e9,"'>>run_tmp.csh
  echo -n $file>>run_tmp.csh
  echo -n '","'>>run_tmp.csh
  echo -n $outfileDir>>run_tmp.csh
  echo '")'>>run_tmp.csh
  echo ".q">>run_tmp.csh
  echo "EOF">>run_tmp.csh
  mv run_tmp.csh script_$1_$2/$baseName.csh

  echo "Executable        = script_$1_$2/$baseName.csh">>job/runAll$1_$2_$nfile.job
  echo "Output             = $logDir/$baseName.out"	    >>job/runAll$1_$2_$nfile.job
  echo "Error                = $logDir/$baseName.err"	    >>job/runAll$1_$2_$nfile.job
  echo "Log                  = $logDir/$baseName.log"	    >>job/runAll$1_$2_$nfile.job
  echo  "Queue" >>job/runAll$1_$2_$nfile.job
  echo  "     " >>job/runAll$1_$2_$nfile.job

  condor_submit job/runAll$1_$2_$nfile.job
  @ nfile++
end


