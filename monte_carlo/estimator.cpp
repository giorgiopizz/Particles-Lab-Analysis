/*
c++ -o estimator estimator.cpp `root-config --cflags --glibs`

mu:2.10751
std:0.0109906
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

double time_offset = 0.00;


void histo_populate(TH1F * h, int n_tot, double tau){
    TRandom3 * r = new TRandom3();
    // double width = (max-min)/bin;
    for(int i=0;i<n_tot;i++){
        // nu = n_tot * func->Integrate(min+i*width,min+(i+1)*width);
        // h->Fill(r->Poisson(nu));
        h->Fill(r->Exp(tau)+time_offset);
    }
}

void histo_populate_background(TH1F * h, int n_tot){
    TRandom3 * r = new TRandom3();
    // double width = (max-min)/bin;
    for(int i=0;i<n_tot;i++){
        // nu = n_tot * func->Integrate(min+i*width,min+(i+1)*width);
        // h->Fill(r->Poisson(nu));
        h->Fill(r->Uniform(0,11)+time_offset);
    }
}

double mean(vector<double> x){
    double s = 0;
    for (auto i: x) s+=i;
    return s/x.size();
}

double err(vector<double> x, double mu){
    double m =-1;
    for (auto i: x){
        if (abs(i-mu)>m)    m = abs(i-mu);
    }
    return m;
}

int main(int argc, char** argv)
{

    std::vector<double> tau;
    std::vector<double> chi_square;
    int experiments = 10000;
    int counts = 100000;
    gStyle->SetOptStat(11);
    gStyle->SetOptFit(1111);
    TRandom3 * r = new TRandom3();
    int n = r->Poisson(counts);

    TF1 * func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1])",0.0+time_offset, 11);
    func->SetParameter(0,n/2);
    func->SetParameter(1,2.100);
    // func->SetParameter(2,2.2);
    // func->SetParLimits(2,0.0,100);

    TApplication* myApp = new TApplication("myApp", NULL, NULL);

    // double f_mu_pos = r->Gaus(0.56075, 0.00087);
    double f_mu_pos = 0.56075;
    // double f_mu_pos = 0.8;
    double f_mu_neg = 1 - f_mu_pos;

    for(int i=0;i<experiments;i++){
        // TCanvas* cnv1 = new TCanvas(("myC1" + to_string(i)).c_str(),"myC1",10,10,1200,800);
        // generate each pseudoexperiment
        // in a single experiment we populate an histogram with the histo_populate
        // method, we then fit this histogram in order to estimate tau,
        double min=0, max=11;
        int bin = 100;
        TH1F *h = new TH1F("h", "example histogram",bin,min,max);


        // generate number of mu+ and mu-


        int mu_pos = (int) r->Binomial(n, f_mu_pos);
        int mu_neg = (int) r->Binomial(n, f_mu_neg);
        // int mu_neg = n - mu_pos;

        // cout << mu_pos << " " << mu_neg << endl;


        histo_populate(h, mu_pos, 2.197);
        histo_populate(h, mu_neg, 2.026);


        // histo_populate_background(h, 0.2*n);


        // cnv1->cd();
        // h->Draw();

        TFitResultPtr result = h->Fit(func, "SQR");
        if(result->Status()!=0){
            cout << "Error!" << endl;
            continue;
        }


        tau.push_back(result->Parameter(1));
        chi_square.push_back(result->Chi2());

        delete h;
        // cnv1->Modified();
        // cnv1->Update();
    }

    double mu = mean(tau);
    double std = err(tau, mu);

    cout << "Tau statistics:\nmu:" << mu << "\nstd:" << std << endl;


    TCanvas* cnv = new TCanvas("myC1","myC1",10,10,1200,800);
    gStyle->SetOptFit(1111);
    double min = *min_element(tau.begin(), tau.end());
    double max = *max_element(tau.begin(), tau.end());
    TH1F *h = new TH1F("h", "tau_MC",50, min, max);

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
    TH1F *h1 = new TH1F("h1", "chi_square",60, min1, max1);

    for(double t: chi_square)   h1->Fill(t);

    h1-> Draw();
    h1->Fit(func2);
    cnv1->Modified();
    cnv1->Update();
    cnv1->SaveAs("images/chi_MC.png");
    myApp->Run();
}
