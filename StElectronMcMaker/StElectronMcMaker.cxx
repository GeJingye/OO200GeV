#include "StElectronMcMaker.h"

#include "StChain.h"
#include "TF1.h"
#include "TRandom.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>

#include "PhysicalConstants.h"
#include "SystemOfUnits.h"
#include "StThreeVector.hh"
#include "StThreeVectorF.hh"
#include "StThreeVectorD.hh"
#include "StLorentzVectorD.hh"

#include "StTriggerIdCollection.h"
#include "StTriggerId.h"

#include "StMcEvent/StMcTpcHit.hh"

#include "StEvent.h"
#include "StEvent/StTrack.h"
#include "StEvent/StBTofCollection.h"
#include "StEvent/StBTofHit.h"
#include "StEvent/StBTofPidTraits.h"

#include "StMcEvent/StMcEvent.hh"
#include "StMcEvent/StMcVertex.hh"
#include "StMcEvent/StMcTrack.hh"
#include "StEventUtilities/StuRefMult.hh"

#include "StRoot/StRefMultCorr/StRefMultCorr.h"
#include "StRoot/StRefMultCorr/CentralityMaker.h"

#include <math.h>
#include "TRandom.h"
#include "Random.h"
#include "RanluxEngine.h"
#include "RandGauss.h"

static RanluxEngine engine;
static RandGauss ranGauss(engine);

//_________________________________________________
StGlobalTrack*  partnerTrack(mcTrackMapType* map, StMcTrack* mT, int *nCommon) 
{
	mcTrackMapIter i = map->find(mT);
	StGlobalTrack* rT = 0;
	*nCommon = 0;
	if (i != map->end()) 
	{
		if( (int) ((*i).second->commonTpcHits()) > (*nCommon) ) 
		{
			rT = const_cast<StGlobalTrack*>((*i).second->partnerTrack());
			*nCommon = (*i).second->commonTpcHits();
		}
	}
	return rT; 
} 
//_________________________________________________
StMcTrack*  partnerMcTrack(rcTrackMapType* map, StGlobalTrack* rT, int *nCommon) 
{
	rcTrackMapIter i = map->find(rT);
	StMcTrack* mT = 0;
	*nCommon = 0;
	if (i != map->end()) 
	{
		if( (int) ((*i).second->commonTpcHits()) > (*nCommon) ) 
		{
			mT = const_cast<StMcTrack*>((*i).second->partnerMcTrack());
			*nCommon = (*i).second->commonTpcHits();
		}
	}
	return mT;
} 

ClassImp(StElectronMcMaker)

	//_________________________________________________
StElectronMcMaker::StElectronMcMaker(const char *name, const char *file):StMaker(name)
{
	//book tree and auto-save to TFile
	mOutputFile = new TFile(file,"RECREATE");
	mOutputFile->SetCompressionLevel(1);
	int BufSize=(int)pow(2.,16.);
	mTree = new TTree("mcT","mcT",BufSize);
	mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
	// 顶点信息
	mTree->Branch("runId",            &mElectron.runId,         "runId/I"    );
	mTree->Branch("triggerId",        &mElectron.triggerId,     "triggerId/I");
	mTree->Branch("zdcX",             &mElectron.zdcX,          "zdcX/F"     );
	mTree->Branch("mcEvtId",          &mElectron.mcEvtId,       "mcEvtId/I"  );
	mTree->Branch("mcVertexX",        &mElectron.mcVertexX,     "mcVertexX/F");
	mTree->Branch("mcVertexY",        &mElectron.mcVertexY,     "mcVertexY/F");
	mTree->Branch("mcVertexZ",        &mElectron.mcVertexZ,     "mcVertexZ/F");

	mTree->Branch("rcEvtId",          &mElectron.rcEvtId,       "rcEvtId/I"  );
	mTree->Branch("Ranking",          &mElectron.Ranking,       "Ranking/F"  );
	mTree->Branch("rcVertexX",        &mElectron.rcVertexX,     "rcVertexX/F");
	mTree->Branch("rcVertexY",        &mElectron.rcVertexY,     "rcVertexY/F");
	mTree->Branch("rcVertexZ",        &mElectron.rcVertexZ,     "rcVertexZ/F");
	mTree->Branch("rcVpdVz",          &mElectron.rcVpdVz,       "rcVpdVz/F"  );
	mTree->Branch("rcRefMult",        &mElectron.rcRefMult,     "rcRefMult/I");
	mTree->Branch("rcRefMultCorr",    &mElectron.rcRefMultCorr, "rcRefMultCorr/F");
	mTree->Branch("reWeight",         &mElectron.reWeight,      "gWeigth/F");
	mTree->Branch("cent9",            &mElectron.cent9,         "cent9/I");
	mTree->Branch("cent16",           &mElectron.cent16,        "cent16/I");
	// 径迹信息
	mTree->Branch("nMcTrks",          &mElectron.nMcTrks,       "nMcTrks/I");
	mTree->Branch("iMcE",             &mElectron.iMcE,          "iMcE/I");
	mTree->Branch("geantId",          mElectron.geantId,        "geantId[iMcE]/I");
	mTree->Branch("mcTrkKey",         mElectron.mcTrkKey,       "mcTrkKey[iMcE]/I");
	mTree->Branch("parentGeantId",    mElectron.parentGeantId,  "parentGeantId[iMcE]/I");
	mTree->Branch("mcParentTrkKey",   mElectron.mcParentTrkKey, "mcParentTrkKey[iMcE]/I");
	mTree->Branch("mcPt",             mElectron.mcPt,           "mcPt[iMcE]/F");
	mTree->Branch("mcEta",            mElectron.mcEta,          "mcEta[iMcE]/F");
	mTree->Branch("mcPhi",            mElectron.mcPhi,          "mcPhi[iMcE]/F");
	mTree->Branch("mcPtFirst",        mElectron.mcPtFirst,      "mcPtFirst[iMcE]/F");
	mTree->Branch("mcEtaFirst",       mElectron.mcEtaFirst,     "mcEtaFirst[iMcE]/F");
	mTree->Branch("mcPhiFirst",       mElectron.mcPhiFirst,     "mcPhiFirst[iMcE]/F");
	mTree->Branch("mcPtLast",         mElectron.mcPtLast,       "mcPtLast[iMcE]/F");
	mTree->Branch("mcEtaLast",        mElectron.mcEtaLast,      "mcEtaLast[iMcE]/F");
	mTree->Branch("mcPhiLast",        mElectron.mcPhiLast,      "mcPhiLast[iMcE]/F");
 
	mTree->Branch("rcTrkKey",         mElectron.rcTrkKey,       "rcTrkKey[iMcE]/I");
	mTree->Branch("rcPtFirst",        mElectron.rcPtFirst,      "rcPtFirst[iMcE]/F");
	mTree->Branch("rcEtaFirst",       mElectron.rcEtaFirst,     "rcEtaFirst[iMcE]/F");
	mTree->Branch("rcPhiFirst",       mElectron.rcPhiFirst,     "rcPhiFirst[iMcE]/F");
	mTree->Branch("rcPtLast",         mElectron.rcPtLast,       "rcPtLast[iMcE]/F");
	mTree->Branch("rcEtaLast",        mElectron.rcEtaLast,      "rcEtaLast[iMcE]/F");
	mTree->Branch("rcPhiLast",        mElectron.rcPhiLast,      "rcPhiLast[iMcE]/F");
	mTree->Branch("rcNHitsFit",       mElectron.rcNHitsFit,     "rcNHitsFit[iMcE]/I");
	mTree->Branch("rcNHitsPoss",      mElectron.rcNHitsPoss,    "rcNHitsPoss[iMcE]/I");
	mTree->Branch("rcNHitsDedx",      mElectron.rcNHitsDedx,    "rcNHitsDedx[iMcE]/I");
	mTree->Branch("rcNHitsCommon",    mElectron.rcNHitsCommon,  "rcNHitsCommon[iMcE]/I");
	mTree->Branch("rcDedx",           mElectron.rcDedx,         "rcDedx[iMcE]/F");
	mTree->Branch("rcNSigmaE",        mElectron.rcNSigmaE,      "rcNSigmaE[iMcE]/F");
	mTree->Branch("rcNSigmaPi",       mElectron.rcNSigmaPi,     "rcNSigmaPi[iMcE]/F");
	mTree->Branch("rcNSigmaK",        mElectron.rcNSigmaK,      "rcNSigmaK[iMcE]/F");
	mTree->Branch("rcNSigmaP",        mElectron.rcNSigmaP,      "rcNSigmaP[iMcE]/F");
	mTree->Branch("rcDca",            mElectron.rcDca,          "rcDca[iMcE]/F");
	mTree->Branch("rcDca1",           mElectron.rcDca1,         "rcDca1[iMcE]/F");
	mTree->Branch("rcDca2",           mElectron.rcDca2,         "rcDca2[iMcE]/F");
	mTree->Branch("rcDca3",           mElectron.rcDca3,         "rcDca3[iMcE]/F");
	mTree->Branch("rcDca4",           mElectron.rcDca4,         "rcDca4[iMcE]/F");
	mTree->Branch("rcDca5",           mElectron.rcDca5,         "rcDca5[iMcE]/F");
	mTree->Branch("rcDca6",           mElectron.rcDca6,         "rcDca6[iMcE]/F");

	// - zero all pointers defined in the header file
	mRcEvent=0;
	mMcEvent=0;
	mMcVertex.setX(0.); mMcVertex.setY(0.); mMcVertex.setZ(0.);
	mAssocMaker=0;
	mRcTrackMap=0;
	mMcTrackMap=0;
}
//_________________________________________________
StElectronMcMaker::~StElectronMcMaker(){}
//_________________________________________________
void StElectronMcMaker::Clear(const char*){StMaker::Clear();}
//_________________________________________________
Int_t StElectronMcMaker::Finish()
{
	mOutputFile->Write();
	mOutputFile->Close();
	return StMaker::Finish();
}
//_________________________________________________
Int_t StElectronMcMaker::Init()
{
	Clear("C");
	//wangzhen change it ,no centrality definition
	//intialize the StRefMultCorr
	//mRefMultCorr = new StRefMultCorr();
	refmultCorrUtil = CentralityMaker::instance()->getRefMultCorr() ;
	return kStOK;
}
//_________________________________________________
Int_t StElectronMcMaker::Make()
{
	Clear("C");
	memset(&mElectron,0,sizeof(mElectron)); //initialize the mElectron struct

	mMcEvent = (StMcEvent*)GetDataSet("StMcEvent");
	if(!mMcEvent) return kStErr;

	mRcEvent=(StEvent *)GetInputDS("StEvent");
	if(!mRcEvent){
		LOG_WARN << "No StEvent!" << endm;
		return kStOK;
	}

	mAssocMaker = (StAssociationMaker*)GetMaker("StAssociationMaker");
	if(mAssocMaker){
		mRcTrackMap = mAssocMaker->rcTrackMap();
		mMcTrackMap = mAssocMaker->mcTrackMap();
	}

	StTriggerIdCollection *trgIdColl = mRcEvent->triggerIdCollection();
	if(
			trgIdColl && 
			trgIdColl->nominal() && //zaochen's trigger
			(
			 trgIdColl->nominal()->isTrigger(610001) ||
			 trgIdColl->nominal()->isTrigger(610011) ||
			 trgIdColl->nominal()->isTrigger(610021) ||
			 trgIdColl->nominal()->isTrigger(610031) ||
			 trgIdColl->nominal()->isTrigger(610041) ||
			 trgIdColl->nominal()->isTrigger(610051) 
			)
	  )
	{}
	else
	{
		cout << "Not a minbias trigger event!" << endl;
		return kStOK;
	}
	if(trgIdColl->nominal()->isTrigger(610001)) mElectron.triggerId =610001;
	if(trgIdColl->nominal()->isTrigger(610011)) mElectron.triggerId =610011;
	if(trgIdColl->nominal()->isTrigger(610021)) mElectron.triggerId =610021;
	if(trgIdColl->nominal()->isTrigger(610031)) mElectron.triggerId =610031;
	if(trgIdColl->nominal()->isTrigger(610041)) mElectron.triggerId =610041;
	if(trgIdColl->nominal()->isTrigger(610051)) mElectron.triggerId =610051;

	mElectron.runId   = mRcEvent->info()->runId();
	mElectron.rcEvtId = mRcEvent->info()->id();

	StPrimaryVertex  *pVertex = mRcEvent->primaryVertex();
	StThreeVectorF rcVertex(-999.,-999.,-999.);//为什么要用-999初始化
	if(pVertex)
	{
		mElectron.Ranking = pVertex->ranking();
		rcVertex = pVertex->position();

		if(fabs(rcVertex.x())<1.e-5 &&
		   fabs(rcVertex.y())<1.e-5	&&
		   fabs(rcVertex.z())<1.e-5)
		return kStOK;//reject event with bad vertex
		else
		{
			mElectron.rcVertexX = rcVertex.x();
			mElectron.rcVertexY = rcVertex.y();
			mElectron.rcVertexZ = rcVertex.z();
		}
	}
	else return kStOK;

	mElectron.rcVpdVz = -999.;
	if(mRcEvent->btofCollection())
	{
		StBTofHeader *btofHeader = mRcEvent->btofCollection()->tofHeader();
		if(btofHeader)
		{
			mElectron.rcVpdVz = btofHeader->vpdVz();
		}
	}

	mElectron.mcEvtId = mMcEvent->eventNumber();

	if(mMcEvent->primaryVertex())
	{
		mMcVertex = mMcEvent->primaryVertex()->position();
		mElectron.mcVertexX = mMcVertex.x();
		mElectron.mcVertexY = mMcVertex.y();
		mElectron.mcVertexZ = mMcVertex.z();
	}
	else return kStOK;

	mElectron.rcRefMult = uncorrectedNumberOfPositivePrimaries(*mRcEvent) + uncorrectedNumberOfNegativePrimaries(*mRcEvent);
	mElectron.zdcX      = mRcEvent->runInfo()->zdcCoincidenceRate();

	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------
	//zaochen add
	refmultCorrUtil ->init( mElectron.runId );
	refmultCorrUtil ->initEvent( mElectron.rcRefMult,mElectron.mcVertexZ,mElectron.zdcX ) ;

	mElectron.rcRefMultCorr = refmultCorrUtil->getRefMultCorr();
	mElectron.reWeight      = refmultCorrUtil->getWeight();
	mElectron.cent9         = refmultCorrUtil->getCentralityBin16();
	mElectron.cent16        = refmultCorrUtil->getCentralityBin9();

	//wangzhen change it for no RefMultCorr
	//beacuse there is no centrality definition for 54GeV
	/*
	   if(fabs(mElectron.mcVertexZ)<30.){ //refMultCorr requirment
	   mRefMultCorr->init(mElectron.runId);
	   mRefMultCorr->initEvent(mElectron.rcRefMult,mElectron.mcVertexZ,mElectron.zdcX);//use mc vertexz to do the refMult correction
	   mElectron.rcRefMultCorr = mRefMultCorr->getRefMultCorr() ;
	//mElectron.reWeight = mRefMultCorr->getWeight() ;
	mElectron.reWeight = 1; //now, the correction parameters are unvaliable.
	}
	//cout<<"refMult:"<<mElectron.rcRefMult<<"  refMultCorr:"<<mElectron.rcRefMultCorr<<"  reWeight:"<<mElectron.reWeight<<endl;
	*/

	//smear the vertex
	StThreeVectorF rcVertexSmear[6];
	const float sigma[6] = {0.02, 0.05, 0.10, 0.15, 0.20, 0.25};
	for(int i=0;i<6;i++) 
	{
		//ranGauss.shoot(): 返回一个服从 均值 = 0、标准差 = 1 的 double 随机数
		rcVertexSmear[i].set(rcVertex.x()+ranGauss.shoot()*sigma[i], rcVertex.y()+ranGauss.shoot()*sigma[i], rcVertex.z());
	}

	//looking for electron
	StSPtrVecMcTrack mcTracks = mMcEvent->tracks();
	mElectron.nMcTrks         = (int)mcTracks.size();
	//cout<<"# of mcTracks:"<<mcTracks.size()<<endl;

	int iMcE = 0; //当前电子索引
	for(int i=0; i<(int)mcTracks.size(); i++)
	{
		StMcTrack *mcTrack = dynamic_cast<StMcTrack *>(mcTracks[i]);
		const StMcTrack *mcParentTrack = mcTrack->parent();

		if(!mcTrack ||
		//(mcTrack->key()==0 && mcTrack->geantId()==0) ||				//mcTrack->key()：MC 轨道在事件数组里的序号，用来当唯一索引；geantId()=0 表示“这是由事件生成器直接产生的粒子
		(mcTrack->geantId() != 2 && mmcTrack->geantId() != 3) ||	//1：gamma;2:e+;3:e-;5:\mu+;6:\mu-;8:\pi+; 9:\pi-;11:K+;12:K-;13:n;14:p;15:p_bar;
		mcParentTrack->geantId() != 0								//只记录geantId = 0的母粒子
		) continue;

		mElectron.geantId[iMcE] = mcTrack->geantId();
		mElectron.mcTrkKey[iMcE]= mcTrack->key(); 
		mElectron.parentGeantId[iMcE]  =  mcParentTrack->geantId();
		mElectron.mcParentTrkKey[iMcE] = mcParentTrack->key();
		
		// 只保留来自primary vertex的粒子
		//if(!mcTrack->startVertex() ||
		//		(mcTrack->startVertex()->position()-mMcVertex).mag()>1.e-5 ) continue;

		StThreeVectorF mcMom = mcTrack->momentum();
		//if(fabs(mcMom.pseudoRapidity())>1.5) continue;

		mElectron.mcPt[iMcE]  = mcMom.perp();
		mElectron.mcEta[iMcE] = mcMom.pseudoRapidity();
		mElectron.mcPhi[iMcE] = mcMom.phi();

		StPtrVecMcTpcHit& mcTpcHit = mcTrack->tpcHits();
		if(mcTpcHit.size()>0)
		{
			mElectron.mcPtFirst[iMcE]  = mcTpcHit[0]->localMomentum().perp();
			mElectron.mcEtaFirst[iMcE] = mcTpcHit[0]->localMomentum().pseudoRapidity();
			mElectron.mcPhiFirst[iMcE] = mcTpcHit[0]->localMomentum().phi();
			mElectron.mcPtLast[iMcE]   = mcTpcHit[mcTpcHit.size()-1]->localMomentum().perp();
			mElectron.mcEtaLast[iMcE]  = mcTpcHit[mcTpcHit.size()-1]->localMomentum().pseudoRapidity();
			mElectron.mcPhiLast[iMcE]  = mcTpcHit[mcTpcHit.size()-1]->localMomentum().phi();
		}

		StGlobalTrack *rcGTrack  = 0;
		StPrimaryTrack *rcPTrack = 0;
		// find the associated track with mc track
		if(mAssocMaker)
		{
			if(mMcTrackMap)
			{
				StTrackPairInfo* mTrackPairInfo = findBestMatchedGlobal(mcTrack);
				//cout<<"mTrackPairInfo Pointer:"<<mTrackPairInfo<<endl;
				if(mTrackPairInfo)
				{
					rcGTrack              = const_cast<StGlobalTrack*>(mTrackPairInfo->partnerTrack());
					if(rcGTrack) rcPTrack = dynamic_cast<StPrimaryTrack *>(rcGTrack->node()->track(primary));

					mElectron.rcNHitsCommon[iMcE] = mTrackPairInfo->commonTpcHits();
				}
			}
		}
		//cout << "nCommonHits = " <<mElectron.rcNHitsCommon[iMcE]<< endl;
		//if(mElectron.rcNHitsCommon[iMcE]>10) cout<<"gTrkPointer:"<<rcGTrack<<"  pTrkPointer:"<<rcPTrack<<endl;

		if(rcPTrack)
		{
			mElectron.rcTrkKey[iMcE]    = rcPTrack->key();
			mElectron.rcPtFirst[iMcE]   = rcPTrack->geometry()->momentum().perp();
			mElectron.rcEtaFirst[iMcE]  = rcPTrack->geometry()->momentum().pseudoRapidity();
			mElectron.rcPhiFirst[iMcE]  = rcPTrack->geometry()->momentum().phi();
			mElectron.rcPtLast[iMcE]    = rcPTrack->outerGeometry()->momentum().perp();
			mElectron.rcEtaLast[iMcE]   = rcPTrack->outerGeometry()->momentum().pseudoRapidity();
			mElectron.rcPhiLast[iMcE]   = rcPTrack->outerGeometry()->momentum().phi();
			mElectron.rcNHitsFit[iMcE]  = rcPTrack->fitTraits().numberOfFitPoints(kTpcId);
			mElectron.rcNHitsPoss[iMcE] = rcPTrack->numberOfPossiblePoints(kTpcId);

			static StTpcDedxPidAlgorithm pidAlgorithm;
			static StPionPlus* Pion        = StPionPlus::instance();
			static StKaonPlus* Kaon        = StKaonPlus::instance();
			static StProton* Proton        = StProton::instance();
			static StElectron* Electron    = StElectron::instance();
			const StParticleDefinition* p1 = rcPTrack->pidTraits(pidAlgorithm);

			if(p1 && pidAlgorithm.traits())
			{
				mElectron.rcNHitsDedx[iMcE] = pidAlgorithm.traits()->numberOfPoints();
				mElectron.rcDedx[iMcE]      = pidAlgorithm.traits()->mean()*1.e6;
				mElectron.rcNSigmaE[iMcE]   = pidAlgorithm.numberOfSigma(Electron);
				mElectron.rcNSigmaPi[iMcE]  = pidAlgorithm.numberOfSigma(Pion);
				mElectron.rcNSigmaK[iMcE]   = pidAlgorithm.numberOfSigma(Kaon);
				mElectron.rcNSigmaP[iMcE]   = pidAlgorithm.numberOfSigma(Proton);
			}

			if(rcGTrack)
			{
				mElectron.rcDca[iMcE]  = rcGTrack->geometry()->helix().distance(rcVertex);
				mElectron.rcDca1[iMcE] = rcGTrack->geometry()->helix().distance(rcVertexSmear[0]);
				mElectron.rcDca2[iMcE] = rcGTrack->geometry()->helix().distance(rcVertexSmear[1]);
				mElectron.rcDca3[iMcE] = rcGTrack->geometry()->helix().distance(rcVertexSmear[2]);
				mElectron.rcDca4[iMcE] = rcGTrack->geometry()->helix().distance(rcVertexSmear[3]);
				mElectron.rcDca5[iMcE] = rcGTrack->geometry()->helix().distance(rcVertexSmear[4]);
				mElectron.rcDca6[iMcE] = rcGTrack->geometry()->helix().distance(rcVertexSmear[5]);
			}
		}
		iMcE++;
	}//loop tracks
	mElectron.nMcE = iMcE;

	mTree->Fill();

	return kStOK;
}
//_________________________________________________
StTrackPairInfo* StElectronMcMaker::findBestMatchedGlobal(StMcTrack *mcTrack)
{
	pair<mcTrackMapIter,mcTrackMapIter> mcBounds = mMcTrackMap->equal_range(mcTrack);
	StTrackPairInfo* candTrackPair = 0;  //used for finding the best matched track
	StGlobalTrack* candTrack       = 0;
	for(mcTrackMapIter mcMapIter   = mcBounds.first ; mcMapIter != mcBounds.second; ++mcMapIter)
	{
		StTrackPairInfo* assocPair = (*mcMapIter).second;
		StGlobalTrack* globTrack =(StGlobalTrack*)assocPair->partnerTrack();
		if(!globTrack || globTrack->flag()<=0) continue;
		if(globTrack->fitTraits().numberOfFitPoints(kTpcId)>=10)
		{
			if(!candTrackPair)
			{
				candTrackPair = assocPair;
				candTrack     = globTrack;
			}
			else if(globTrack->fitTraits().numberOfFitPoints(kTpcId) > candTrack->fitTraits().numberOfFitPoints(kTpcId))
			{
				candTrackPair = assocPair;
				candTrack     = globTrack;
			}
		} //nHitsFit requirement
	} //bounds loop
	return candTrackPair; //Note that candTrack might be zero, for example if only one track is matched and has 9 tpc fit pts.
}
