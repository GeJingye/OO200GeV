#include "iostream.h"

class     StChain;
class StRefMultCorr;
StChain  *chain=0;

Int_t iEvt=0,istat=0,nEvents=0;
//ProtoTypes

// ------------------ Here is the actual method -----------------------------------------
void doEvent(const Int_t nEvents = 100000, 
		const char *fMcFile = "/star/data105/embedding/production_OO_200GeV_2021/Electron_100_20250302/P23ic.SL23c/2021/131/22131020/st_physics_adc_22131020_raw_6000013.MuDst.root",
		const char *outDir="./test")
{
	// First load some shared libraries we need
	if (gClassTable->GetID("TTable") < 0)
	{
		gSystem->Load("libStar");
		gSystem->Load("libPhysics");
	}  
	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();

	// cout << "lzh" << endl;

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

	// cout << "lzh" << endl;
	gSystem->Load("StRefMultCorr");
	gSystem->Load("StBTofUtil");
	gSystem->Load("StVpdCalibMaker");
	// gSystem->Load("RTS");

	// cout << "lzh" << endl;
	gSystem->Load("StMuDSTMaker");

	// cout << "lzh" << endl;
	// gSystem->Load("StMuEvent");

	// cout << "lzh" << endl;
	// gSystem->Load("StMuTrack");

	// cout << "lzh" << endl;
	gSystem->Load("StEvent");
	gSystem->Load("StEventMaker");
	gSystem->Load("StElectronMcMaker");

	// cout << "lzh" << endl;

	//chain  = new StChain("StChain");
	chain  = new StChain();

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

	// StMcEventMaker *mcEventReader = new StMcEventMaker(); // Make an instance..
	//mcEventReader->doPrintEventInfo = false;
	//mcEventReader->doPrintMemoryInfo = false;

	// StEmcSimulatorMaker *emcSim = new StEmcSimulatorMaker();
	// emcSim->setCheckStatus(kBarrelEmcTowerId,false);
	// emcSim->setMakeFullDetector(kBarrelEmcTowerId,true);
	// emcSim->setDoZeroSuppression(kBarrelEmcTowerId,false);
	// StEmcADCtoEMaker *adc1 = new StEmcADCtoEMaker("EReadEmbed");
	// adc1->setEmbeddingMode(kTRUE);
	// adc1->saveAllStEvent(kTRUE);
	// adc1->setPrint(kFALSE);
	// cout<<"lzh"<<endl;

	// StPreEclMaker *pre = new StPreEclMaker();
	// pre->setPrint(kFALSE);
	// cout<<"lzh"<<endl;

	// StEpcMaker *epc = new StEpcMaker();
	// epc->setPrint(kFALSE);

	StAssociationMaker *associator = new StAssociationMaker();
	cout << "created new StAssociationMaker" << endl;
	//associator->useIdAssoc();
	associator->useInTracker();

	StMcParameterDB* parameterDB = StMcParameterDB::instance();
	parameterDB->setXCutTpc(.6);//6mm
	parameterDB->setYCutTpc(.6);
	parameterDB->setZCutTpc(.6);
	parameterDB->setReqCommonHitsTpc(10); // Require 10 hits in common for tracks

	// StEmcAssociationMaker *emcAssociation = new StEmcAssociationMaker();
	// emcAssociation->setPrint(kFALSE);

	// new StMuDstMaker("muDstMaker");
	// // cout<<"lzh"<<endl;

	// cout << "lzh" << endl;

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

	// StTriggerSimuMaker *trigSimuMaker = new StTriggerSimuMaker();
	// trigSimuMaker->useOfflineDB();
	// trigSimuMaker->setMC(2);
	// trigSimuMaker->useBemc();
	// trigSimuMaker->useEemc(0);
	// trigSimuMaker->bemc->setConfig(2);        

	StElectronMcMaker *electronMcMaker = new StElectronMcMaker("electron",outputname);	

	// cout << "lzh" << endl;

	// Initialize chain
	Int_t iInit = chain->Init();
	if (iInit) chain->Fatal(iInit,"on init");
	chain->PrintInfo();

	// cout << "lzh" << endl;

	// Event loop
	int istat = 0, i = 1;
	EventLoop: if(i <= nEvents && istat != 2)
	{

		cout << endl << "============================ Event " << i << " start ============================" << endl;

		chain->Clear();
		// cout<<"ustc"<<endl;
		istat = chain->Make(i);
		// cout<<"ustc"<<endl;   
		if (istat == 2) 
		{
			cout << "Last  event processed. Status = " << istat << endl;
		}
		if (istat == 3) 
		{
			cout << "Error event processed. Status = " << istat << endl;
		}

		//gObjectTable->Print();
		i++;
		goto EventLoop;
	}

	i--;
	cout << endl << "============================ Event " << i << " finish ============================" << endl;


	cout << "lzh" << endl;
	// Chain Finish
	chain->Finish();
	delete chain;
}