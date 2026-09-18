/*
类					一句话职责
StMuDstMaker		把.MuDst.root 里的树分支读成 StMuEvent / StMuTrack 内存对象，供后续分析链使用。
StIOMaker			低层“文件打开器”，负责把任何 StTree / StEvent / geantBranch 按名挂载到 StIO 数据流；通常与 StMuDstMaker 成对出现。
St_db_Maker			离线数据库总入口，根据 run 号自动把磁强场、增益、几何、时间校准等表读到内存，供后面任何 Maker 查询。
StAssociationMaker	把 重建轨道 与 Geant 蒙特卡洛轨道 做最近邻匹配，生成 StTrackPairInfo，供效率 / 分辨率研究。
StMcParameterDB		存放匹配算法的可调参数（TPC 空间截断、最小公共 hit 数等），全局单例，链里任意 Maker 都能取。
StElectronMcMaker	用户自定义分析 Maker，本例里读取匹配结果，把电子 / 正电子的 MC 与 reco 信息填进 TTree 输出。

层级顺序（按数据流向）
StIOMaker → StMuDstMaker → St_db_Maker → StAssociationMaker（依赖 StMcParameterDB）→ 用户 Maker（StElectronMcMaker）
前三个负责“把事件读进来并带上数据库”；
StAssociationMaker 给 MC - Reco 配对；
最后的 StElectronMcMaker 只关心“拿到配对后，我要写什么变量到 ntuple”。
*/

#include "iostream.h"

class     StChain;
class StRefMultCorr;

// ------------------ Here is the actual method -----------------------------------------
void doEvent(const Int_t nEvents = 100000, 
		const char *fMcFile = "/star/data105/embedding/production_OO_200GeV_2021/Electron_100_20250302/P23ic.SL23c/2021/131/22131020/st_physics_adc_22131020_raw_6000013.MuDst.root",
		const char *outDir  = "./test")
{
	// First load some shared libraries we need
	loadLabraries();

	StChain* chain = new StChain();
	StMuDstMaker* muDstMaker = new StMuDstMaker(0,1,"",fMcFile,"",100000,"MuDst");
	muDstMaker->SetStatus("*",0);
	muDstMaker->SetStatus("MuEvent",1);
	muDstMaker->SetStatus("PrimaryVertices",1);
	muDstMaker->SetStatus("PrimaryTracks",1);
	muDstMaker->SetStatus("GlobalTracks",1);
	muDstMaker->SetStatus("CovGlobTrack",1);
	muDstMaker->SetStatus("MCAll",1);
	muDstMaker->SetStatus("BTofHeader",1);
	muDstMaker->SetStatus("EpdHit", 1);
	muDstMaker->SetDebug(0);

	StIOMaker* ioMaker = new StIOMaker("IO");
	ioMaker->SetFile(fMcFile);
	ioMaker->SetIOMode("r");
	ioMaker->SetBranch("*",0,"0");             //deactivate all branches
	ioMaker->SetBranch("geantBranch",0,"r");   //activate geant Branch
	ioMaker->SetBranch("eventBranch",0,"r");   //activate event Branch

	St_db_Maker *db1 = new St_db_Maker("StarDb","MySQL:StarDb","MySQL:StarDb","$STAR/StarDb");

	StAssociationMaker *associator = new StAssociationMaker();
	cout << "created new StAssociationMaker" << endl;
	associator->useInTracker();

	StMcParameterDB* parameterDB = StMcParameterDB::instance();
	parameterDB->setXCutTpc(.6);//6mm
	parameterDB->setYCutTpc(.6);
	parameterDB->setZCutTpc(.6);
	parameterDB->setReqCommonHitsTpc(10); // Require 10 hits in common for tracks

	TString outputname = fMcFile;
	TString embedrun = fMcFile;
	int embedRunIndex = embedrun.Index("production_OO_200GeV_2021",0);
	embedrun.Remove(0,embedRunIndex);
	embedRunIndex = embedrun.Index("P23ic",0);
	embedrun.Remove(embedRunIndex);
	cout << "embedrun:   " << embedrun << endl;
	int fileBeginIndex = outputname.Index("st_physics_adc",0);
	outputname.Remove(0,fileBeginIndex+15);
	cout << "output: " << outputname << endl;
	outputname.Prepend("_");
	outputname.Prepend(embedrun);
	outputname.Prepend("emb");
	outputname.ReplaceAll("/","x");
	outputname.ReplaceAll("event","myminimc");
	outputname.Prepend("/");
	outputname.Prepend(outDir);
	cout << "Output: " << outputname << endl;       
	StElectronMcMaker *electronMcMaker = new StElectronMcMaker("electron",outputname);

	// Initialize chain
	Int_t iInit = chain->Init();
	if (iInit) chain->Fatal(iInit,"on init");
	chain->PrintInfo();

	int istat = 0, i = 1;
	for (int iEvent = 1; iEvent <= nEvents; iEvent++)
	{
		if (iEvent && iEvent % 1000 == 0) cout << "... finished processing " << iEvent << " events." << endl;
		chain->Clear();
		int istat = chain->Make(i);
		// 判断状态
		if (istat == 2) {cout << "Last event processed. Status = " << istat << endl;break;}
		if (istat == 3) {cout << "Error event processed. Status = " << istat << endl;continue;}
	}
	cout << endl << "============================ Event " << iEvent << " finish ============================" << endl;

	// Chain Finish
	chain->Finish();
	delete chain;
}

void loadLabraries()
{
	if (gClassTable->GetID("TTable") < 0)
	{
		gSystem->Load("libStar");
		gSystem->Load("libPhysics");
	}
	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();
	gROOT->Macro("loadMuDst.C");
	gSystem->Load("StarMagField");
	gSystem->Load("StMagF");
	gSystem->Load("StTpcDb");
	gSystem->Load("StDetectorDbMaker");
	gSystem->Load("StDbUtilities");
	// gSystem->Load("StEEmcUtil");
	// gSystem->Load("StEmcUtil");
	// gSystem->Load("StEEmcDbMaker");
	gSystem->Load("StMcEvent");
	gSystem->Load("StMcEventMaker");
	gSystem->Load("StDaqLib");
	gSystem->Load("St_g2t.so");
	gSystem->Load("St_geant_Maker.so");
	gSystem->Load("StAssociationMaker");
	gSystem->Load("StMcAnalysisMaker");
	gSystem->Load("StDbLib");
	gSystem->Load("StDbBroker");
	gSystem->Load("St_db_Maker");
	gSystem->Load("libgeometry_Tables");
	gSystem->Load("libgen_Tables");
	gSystem->Load("libsim_Tables");
	gSystem->Load("libglobal_Tables");
	// gSystem->Load("StEmcRawMaker");
	// gSystem->Load("StEmcADCtoEMaker");
	// gSystem->Load("StPreEclMaker");
	// gSystem->Load("StEpcMaker");
	// gSystem->Load("StEmcSimulatorMaker");
	// gSystem->Load("StTriggerUtilities");
	gSystem->Load("StRefMultCorr");
	gSystem->Load("StBTofUtil");
	gSystem->Load("StVpdCalibMaker");
	// gSystem->Load("RTS");
	gSystem->Load("StMuDSTMaker");
	// gSystem->Load("StMuEvent");
	// gSystem->Load("StMuTrack");
	gSystem->Load("StEvent");
	gSystem->Load("StEventMaker");
	gSystem->Load("StElectronMcMaker");
}