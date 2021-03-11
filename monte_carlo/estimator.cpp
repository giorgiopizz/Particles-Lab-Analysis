/*
c++ -o estimator estimator.cpp `root-config --cflags --glibs`
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
}

int main(int argc, char** argv)
{






    std::vector<double> tau;
    std::vector<double> chi_square;
    int experiments = 10000;
    int counts = 20000;

    TRandom3 * r = new TRandom3();
    int n = r->Poisson(counts);

    TF1 * func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1])",0, 11);
    func->SetParameter(0,counts/2);
    func->SetParameter(1,2.1);


    for(int i=0;i<experiments;i++){
        // generate each pseudoexperiment
        // in a single experiment we populate an histogram with the histo_populate
        // method, we then fit this histogram in order to estimate tau,
        double min=0, max=11;
        int bin = 70;
        TH1F *h = new TH1F("h", "example histogram",bin,min,max);


        // generate number of mu+ and mu-

        double f_mu_pos = r->Gaus(0.56075, 0.00087);
        int mu_pos = (int) r->Binomial(n, f_mu_pos);

        double f_mu_neg = r->Gaus(0.43925, 0.00087);
        int mu_neg = (int) r->Binomial(n, f_mu_neg);
        //cout << mu_pos << " " << mu_neg << endl;
        histo_populate(h, mu_pos, 2.197);
        histo_populate(h, mu_neg, 2.026);

        //h->Draw();
        TFitResultPtr result = h->Fit(func, "SQ");
        if(result->Status()!=0){
            cout << "Error!" << endl;
            continue;
        }


        tau.push_back(result->Parameter(1));
        chi_square.push_back(result->Chi2());

        delete h;

    }

    TApplication* myApp = new TApplication("myApp", NULL, NULL);
    TCanvas* cnv = new TCanvas("myC1","myC1",10,10,1200,800);
    gStyle->SetOptFit(1111);
    double min = *min_element(tau.begin(), tau.end());
    double max = *max_element(tau.begin(), tau.end());
    TH1F *h = new TH1F("h", "tau_MC",25, min, max);

    for(double t: tau)   h->Fill(t);

    h-> Draw();
    TF1 * func2 = new TF1("fun2", "gaus",min, max);
    h->Fit(func2);
    cnv->Modified();
    cnv->Update();
    cnv->SaveAs("images/tau_MC.png");

    TCanvas* cnv1 = new TCanvas("myC2","myC2",10,10,1200,800);
    gStyle->SetOptFit(1111);
    double min1 = *min_element(chi_square.begin(), chi_square.end());
    double max1 = *max_element(chi_square.begin(), chi_square.end());
    TH1F *h1 = new TH1F("h1", "chi_square",30, min1, max1);

    for(double t: chi_square)   h1->Fill(t);

    h1-> Draw();
    h1->Fit(func2);
    cnv1->Modified();
    cnv1->Update();
    cnv1->SaveAs("images/chi_MC.png");
    myApp->Run();
}
