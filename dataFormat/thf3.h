
#ifndef xthf3_H
#define xthf3_H
#include "xAlgo.h"

class xthf3{
	public:
		xthf3(TString name, int nxbin, float * xbin, int nybin, float *ybin, int nzbin, float *zbin);
		void Fill(float x, float y, float z, float w = 1);
		void Rebin(int nx=2, int ny=1, int nz=1);
		void RebinZ(int nbin, float * bins);
		void RebinZ(int n, float *bins);
		void Write();
		TH2F* ProjectionZ(TString name="", int ibin=0, int ebin=-1);
	public: 
		std::vector<TH2F*> thf3;
		TString hname;
		std::vector<float> zbin;
}; 
xthf3::xthf3(TString name, int nxbin, float * xbin, int nybin, float *ybin, int nzbin, float *zbinConfig){
	thf3.reserve(nzbin+1);
	hname = name;
	for(int jz=0; jz<nzbin; ++jz){
		thf3.push_back(new TH2F(name+Form("_%d",jz), "", nxbin, xbin, nybin,ybin));
		thf3.back()->Sumw2();
	}
	thf3.push_back(new TH2F(name+Form("_%d",nzbin), "overflow", nxbin, xbin, nybin,ybin));
	for(int j=0; j<nzbin+1;++j) zbin.push_back(zbinConfig[j]);
}

void xthf3::Fill(float x, float y, float z, float w){
	int jz = xAlgo::binarySearch(z, zbin, zbin.size()-1, 0); // size-1 is the overflow bin
	if(jz <0) thf3.at(zbin.size()-1)->Fill(x,y,w); //filling overflow
	else thf3.at(jz)->Fill(x,y,w);
}

void xthf3::Rebin(int nx=2, int ny=1, int nz=1){
	for(auto &it : thf3) it->Rebin2D(nx, ny);
	if(nz==1) return;
	float ft = float(thf3.size()-1)/nz;
	if(floor(ft)!=ceil(ft)){
		std::cout<<"can't rebin with un-proper size for z!"<<std::endl;
		return;
	}
	else {
		for(int j=0; j<ft; ++j){
			if(j!=0) thf3.at(j) = thf3.at(j*nz);
			for(int k=1; k<nz; ++k) {
				thf3.at(j)->Add(thf3.at(j*nz+k));
				delete thf3.at(j*nz+k);
			}
		}
	}
	thf3.at(ft)=thf3.at(thf3.size()-1);
	thf3.resize(ft+1);
	return;
}

TH2F* xthf3::ProjectionZ(TString name="", int ibin=0,int ebin=-1){
	if(ibin > thf3.size()-1) {
		std::cout<<"bin number isn't valide!"<<std::endl;
		return NULL;
	}
	int end;
	TH2F* proj = (TH2F*) thf3.at(ibin)->Clone(name+"_pz");
	if (ebin == -1 ) end = thf3.size()-2;
	else if(ebin > thf3.size()-1) end = thf3.size()-1; 
	else end = ebin;

	for(int j=ibin+1; j<end+1; j++){
		proj->Add(thf3.at(j));
	}
	return proj;
}

void xthf3::RebinZ(int n, float *bins){
	std::vector<int > binIndx;
	for(int j=0; j<n+1; ++j){
		for(int i=j; i<zbin.size(); ++i){
			if(zbin[i]==bins[j]){
				binIndx.push_back(i);
				break;
			}
		}
	}
	if(binIndx.size()<n+1){
		std::cout<<"error:binning can't be match exactly!"<<std::endl;
		return;
	}
	for(int j=0; j<binIndx.size()-1; ++j){
		thf3[j]=thf3[binIndx[j]];
		for(int k=binIndx[j]+1; k<binIndx[j+1]; ++k){
			thf3[j]->Add(thf3[k]);
			delete thf3[k];
		}
	}
	cout<<binIndx.size()<<endl;
	thf3[binIndx.size()-1] = thf3[thf3.size()-1]; //overflow
	thf3.resize(binIndx.size());
}
void xthf3::Write(){
	for(auto &it: thf3){
		it->Write();
	}
}
#endif
/*
void th3(){
	float bin[] = {0,1,2,3,4,5};
	float zbin[] = {0,2,4,5, 6};
	auto a = new xthf3("a",5, bin, 5, bin , 4, zbin);
	a->Fill(1,3,5.4);
	a->Fill(1,3,4.4);
	a->Fill(1,3,7.4);
//	a->Rebin(1,1,2);
	cout<<a->thf3.size()-1<<endl;
	//a->thf3.at(2)->Draw("colz");
	a->ProjectionZ("", 3,4)->Draw("colz");
}
*/
