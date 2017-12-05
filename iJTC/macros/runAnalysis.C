
#include "workflow_lib.h"

void runAnalysis(){
		/*
		// pull the signal for MC
		input_raw2D::get2DInput_GenGen(); input_raw2D::pullSig("gen_gen");
		input_raw2D::get2DInput_GenRec(); input_raw2D::pullSig("gen_rec");
		input_raw2D::get2DInput_RecGen(); input_raw2D::pullSig("rec_gen");
		input_raw2D::get2DInput_RecRec(); input_raw2D::pullSig("rec_rec");
		*/

		//getting the 4-commuted tabels for jet and track reco validation check 
		signal2D::loadFile();
		signal2D::drawTable("gen_gen", "gen_rec", signal2D::gengen_pb_f, signal2D::genrec_pb_f);
		signal2D::drawTable("rec_gen", "rec_rec", signal2D::recgen_pb_f, signal2D::recrec_pb_f);
		signal2D::drawTable("gen_gen", "rec_gen", signal2D::gengen_pb_f, signal2D::recgen_pb_f);
		signal2D::drawTable("gen_rec", "rec_rec", signal2D::genrec_pb_f, signal2D::recrec_pb_f);
}
