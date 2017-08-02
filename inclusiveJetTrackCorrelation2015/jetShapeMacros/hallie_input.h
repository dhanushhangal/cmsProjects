
TH1D* jetShape_hallie[4];
const double xdrbins[16] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,1., 1.2};

TFile* hf = TFile::Open("/Users/tabris/Research/HIN_5.02TeV/analyzer/JetTrack2016/Jet_Shapes_pTweighted.root");
//TFile* hf = TFile::Open("/Users/tabris/Research/HIN_5.02TeV/analyzer/JetTrack2016/Jet_Shapes_NoCorrection-5.root");

TString htag[] = {"0", "10", "30", "50", "100"};

void getInput_Hallie (){
	TH1D* htemp[4];
	for(int i=0;i<4;++i){
			TString name = "JetShape2_Yield_BkgSub_pTweightedInclusive_Cent"+htag[i]+"_Cent"+htag[i+1]+"_TrkPt300_";
			cout<<name<<endl;
			htemp[i]= (TH1D*) hf->Get(name);
			//htemp[i]->Scale(1.0/htemp[i]->Integral("width"));
			jetShape_hallie[i]=new TH1D(Form("jtshape_approval_%d",i),"",15,xdrbins);
			for(int j=1; j<16; ++j){
				jetShape_hallie[i]->SetBinContent(j,htemp[i]->GetBinContent(j));
				jetShape_hallie[i]->SetBinError(j, htemp[i]->GetBinError(j));
			}
	}
}
