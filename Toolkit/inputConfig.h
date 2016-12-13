
#ifndef INPUTCONFIG_H
#define INPUTCONFIG_H

#ifndef INPUTTREE_H
#include "inputTree.h"
#endif

class inputConfig{

	public: 
		inputConfig(TTree* t);

		int trkQualityCuts(int jtrk);
		int evtSelection(Long64_t jentry);
};
