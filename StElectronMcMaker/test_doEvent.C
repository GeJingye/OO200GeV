#include "iostream.h"

class     StChain;
class StRefMultCorr;
StChain  *chain=0;

Int_t iEvt=0,istat=0,nEvents=0;

int test_doEvent()
{
    if (gClassTable->GetID("TTable") < 0)
	{
		gSystem->Load("libStar");
		gSystem->Load("libPhysics");
	}  
	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();

	cout << "lzh" << endl;

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

	cout << "lzh" << endl;
	gSystem->Load("StRefMultCorr");
	gSystem->Load("StBTofUtil");
	gSystem->Load("StVpdCalibMaker");
	// gSystem->Load("RTS");

	cout << "lzh" << endl;
	gSystem->Load("StMuDSTMaker");

	cout << "lzh" << endl;
	gSystem->Load("StMuEvent");

	cout << "lzh" << endl;
	gSystem->Load("StMuTrack");

	cout << "lzh" << endl;
	gSystem->Load("StEvent");
	gSystem->Load("StEventMaker");
	gSystem->Load("StElectronMcMaker");

	cout << "lzh" << endl;

	return 0;
}