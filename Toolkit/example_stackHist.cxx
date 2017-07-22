
#include "stackHist.h"

void example_stackHist(){

	stackHist* st = new stackHist("st"); // set the name of stack
	TH1D* h[5];
	TH1D* hdiff[5];
	float weight[5]={0.5, 0.9, 1.2,1.1,0.9};

	for(int i=0;i<5;i++){
		h[i]= new TH1D(Form("h_%d",i),"", 20, -10, 10);
		h[i]->FillRandom("gaus",50);
	}

	/*add the hist to the stack trunk, it has to be the TH1**, the pointer to the TH1* array, and followed by the size of
	* the array. The weight is optional, it has to be a pointer to the array and at least as long as the size added to
	* the stack. It will scale the hist as h[i]->Scale(w[i]);
	*/
	st->addHist((TH1**)h,5);

	/* there's a default color scheme for at most 10 hists in the stack for plotting, and you can change it by create
	 * a new color vector like:
	 * vector<Color_t> *v = ...;
	 * st->color_v = v;
	 * and push the color you want to the vector, the order for applying to the hist will be the same as the vector of
	 * the hist trunk. 
	 */
	TCanvas *c =new TCanvas("c", "", 800,600);

	/* the default range is 0,1 for both x and y axis.
	 * and the drawStack( opt, addopt);
	 * where opt ="" for default order,
	 *       opt ="r"for reverse order of plotting.
	 * the addopt is the option related to the option in THStack
	 * THStack::Add(TH1*, addopt);
	 * the line between differen stack can be set the width by 
	 * st->lineW = 0 for no line appearing.
	 * for configuring the plottin, you can modify the section of stackConfig() in the package.
	 */
	c->cd();
	st->setRange(-10, 5,"x");
	st->setRange(0, 100,"y");
	st->drawStack("r");

	/* there's a legend in the stackHist and can be access by 
	 * st->tl_stack
	 * and the legend can be easily add by calling the function
	 * makeLegend(TString * tl_txt, float x1, float y1, float x2, float y2);
	 * where tl_txt is the pointer to the TString array.
	 */

	TString tl_txt [5] = {"gaus 1", "gaus 2", "gaus 3", "gaus 4", "gaus 5"};
	TLegend* ll = st->makeLegend(tl_txt, 0.15, 0.56, 0.7, 0.86);
	ll->Draw();

	/* this is an example to show how to get the difference stack, which is similar to the previous one
	 * but using addDiff and drawDiff.
	 */

	for(int i=0;i<4;i++){
		hdiff[i] = (TH1D*)h[i]->Clone(Form("hdiff_%d",i));
		hdiff[i]->Add(h[4], -1);
	}
	TCanvas* c2 = new TCanvas("c2","",800,600);
	c2->cd();
	gPad->SetLogy();

	st->addDiff((TH1**)hdiff,4, weight);
	st->setRange(-10, 5,"xd");
	st->setRange(-10, 10,"yd");
	st->drawDiff();

	/* we can still use the makeLegend to generate the legend for the difference plot. the second last bool variable specifies
	 * whether this legend used to mark the difference group or not. and the last size_t variable refers to how
	 * many hists will be added into the legend. The default is all of the hists in the trunk.
	 */
	TString tl_txt2 [5] = {"gaus 1-5", "gaus 2-5", "gaus 3-5", "gaus 4-5", "gaus 5"};
	TLegend* ll2 = st->makeLegend(tl_txt2, 0.15, 0.56, 0.7, 0.86, true);
	ll2->Draw();

}
