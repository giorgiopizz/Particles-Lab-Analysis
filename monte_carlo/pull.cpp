/*
c++ -o pull pull.cpp `root-config --cflags --glibs` ../lib/CfgParser.cc
*/


#include <iostream>
#include <fstream>
#include <cmath>
//#include <math.h>
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
#include "TRandom3.h"
#include "../lib/CfgParser.h"
#include <bits/stdc++.h>
using namespace std;




void histo_populate(TH1F * h, int n_tot, double tau){
    TRandom3 * r = new TRandom3();
    // double width = (max-min)/bin;
    for(int i=0;i<n_tot;i++){
        // nu = n_tot * func->Integrate(min+i*width,min+(i+1)*width);
        // h->Fill(r->Poisson(nu));
        h->Fill(r->Exp(tau));
    }
    //cout << "J: " << j << endl;
}

int main(int argc, char** argv)
{
    char config_file[] = "../fit.cfg";
    CfgParser * gConfigParser = new CfgParser (config_file);
    string name = gConfigParser->readStringOpt("general::name");

    int function_type = gConfigParser->readIntOpt (name+"::f_type");


    string filename = gConfigParser->readStringOpt(name+"::filename");

    string title = gConfigParser->readStringOpt(name+"::title");

    int save = gConfigParser->readIntOpt(name+"::save");

    vector<float> parameters = gConfigParser->readFloatListOpt (name+"::parameters");

    vector<float> range = gConfigParser->readFloatListOpt (name+"::range");
    float bin = gConfigParser->readFloatOpt (name+"::bin");


    std::vector<double> pulls;


    TApplication* myApp = new TApplication("myApp", NULL, NULL);

    TCanvas* cnv1 = new TCanvas("myC2","myC2",10,10,1200,800);
    gStyle->SetOptFit(1111);

    //TF1 * func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1])",0.5, 10);
    TF1 * func;
    if(function_type==1){
        func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1]) + [2]",range[0], range[1]);

    }
    else if(function_type==2){
        func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1]) +  [2]*TMath::Exp(-x[0]/[3])+ [4]",range[0], range[1]);
    }


    for (int i=0; i<parameters.size(); i++) {

            func->SetParameter(i, parameters[i]);
    }

    TH1F *h = new TH1F("h", "example histogram",bin,0,11);
    ifstream inp;
    double x;
    inp.open(filename);
    while(!(inp >> x)==0) h->Fill(x);
    cnv1->cd();
    h->Draw();
    cnv1->Modified();
    cnv1->Update();
    TFitResultPtr r = h->Fit("fun","SR");
    double N;
    TAxis * xaxis = h->GetXaxis();
    for(int i=1;i<bin+1;i++){
        if(xaxis->GetBinCenter(i)>range[0] && xaxis->GetBinCenter(i)<range[1]){
            N = h->GetBinContent(i);
            // cout << N << "\t" << xaxis->GetBinCenter(i) << "\t" << func->Eval(xaxis->GetBinCenter(i)) <<  endl;
            x = (N-func->Eval(xaxis->GetBinCenter(i)))/sqrt(N);
            pulls.push_back(x);
            if(x<-2.0){
                cout << i << endl;
            }
        }

    }


    // delete h, func;


    TCanvas* cnv = new TCanvas("myC1","myC1",10,10,1200,800);
    gStyle->SetOptFit(1111);


    double min = *min_element(pulls.begin(), pulls.end());
    double max = *max_element(pulls.begin(), pulls.end());

    cout << min << "\t" << max << endl;
    TH1F * h1 = new TH1F("h1", "Pulls",30,min, max);

    for(double x: pulls) h1->Fill(x);

    TF1 * func1 = new TF1("fun1", "gaus");

    h1->Fit(func1);
    h1->Draw();

    cnv->Modified();
    cnv->Update();

    myApp->Run();

}
