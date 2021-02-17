/*
   c++ -o dead_time dead_time.cpp `root-config --cflags --glibs` CfgParser.cc -lm
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>

#include "TStyle.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TMath.h"
#include "CfgParser.h"

using namespace std;

//background shoul be 2*r_start*r_stop*(10e-6)/bins*T_measure

int main(int argc, char** argv)
{

        TApplication* myApp = new TApplication("myApp", NULL, NULL);



        TCanvas* cnv = new TCanvas("myC1","myC1",10,10,1200,800);
        gStyle->SetOptFit(1111);
        //TF1 * func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1])",0.5, 10);
        TF1 * func = new TF1("fun", "[0]*TMath::Exp(-[0]*x[0])",0,300);
        // double neg_muon = 5164*0.44;
        // double pos_muon = 5164*0.56;
        // double neg_tau = 2.005;
        // double pos_tau = 2.2;
        // func->SetParameter(0, neg_muon);
        // func->SetParLimits(0, neg_muon-0.1*neg_muon,neg_muon+0.1*neg_muon);
        // func->SetParameter(1, neg_tau);
        // func->SetParLimits(1, neg_tau-neg_tau*0.05,neg_tau+neg_tau*0.05);
        // func->SetParameter(2, pos_muon);
        // func->SetParLimits(2, pos_muon-0.1*pos_muon,pos_muon+0.1*pos_muon);
        // func->SetParameter(3, pos_tau);
        // func->SetParLimits(3, pos_tau-pos_tau*0.05,pos_tau+pos_tau*0.05);
        // func->SetParameter(4, 386);

        // for (int i=0; i<parameters.size(); i++) {
        //
        //         if(i!=search) {func->FixParameter(i, parameters[i]);}
        //         else{func->SetParameter(i, parameters[i]);}
        // }

        TH1F *h = new TH1F("h", "example histogram",100,0,300);
        ifstream inp;
        double x;
        inp.open(argv[1]);
        while(!(inp >> x)==0) {h->Fill(x);}
        cnv->cd();
        h->Draw();

        TFitResultPtr r = h->Fit("fun","SR");

        // double tot = func->GetParameter(0)+func->GetParameter(2);
        // double prop = func->GetParameter(0)/tot*100;
        // cout << "proprorzioni:\nneg: " << prop << "\npos: " << 100-prop <<endl;
        //
        // cout << "Vita media: " << func->GetParameter(1) << " microsec ± " <<  r->ParError(1) <<endl;
        // cout << "errore percentuale: " << r->ParError(1)/func->GetParameter(1) * 100 << endl;
        // double tau = 2.1969811;
        // double t = abs(tau-func->GetParameter(1))/r->ParError(1);
        // cout << "t: " << t <<endl;
        //
        // stringstream s;
        // s << "Vita media: " << round(func->GetParameter(1)*1000)/1000 << " +/- " <<  round(r->ParError(1)*1000)/1000 << " microsec. t: " << round(t*1000)/1000;
        // h->SetTitle(s.str().c_str());
        cnv->Modified();
        cnv->Update();
        cnv->Print("fit_vita_media.png", "png");
        myApp->Run();
        return 0;
}
