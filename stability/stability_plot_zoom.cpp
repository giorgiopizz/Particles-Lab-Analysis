/*
c++ -o stability_plot_zoom stability_plot_zoom.cpp `root-config --cflags --glibs`  ../lib/CfgParser.cc
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
#include "TH2D.h"
#include "TGraph2D.h"
#include "../lib/CfgParser.h"

using namespace std;

double mean(vector<double> v)
{      double sum=0;
       for(int i=0;i<v.size();i++)
               sum+=v[i];
       return sum/v.size();
}
//DEVIATION
double standard_deviation(vector<double> v, double mean)
{
       double E=0;
       for(int i=0;i<v.size();i++)
               E+=(v[i] - mean)*(v[i] - mean);
       return sqrt(E/v.size());
}

int main(int argc, char** argv)
{
   // TApplication* myApp = new TApplication("myApp", NULL, NULL);
   // TCanvas *c2 = new TCanvas("c2","c2",600,400);
   // //TGraph2D * g = new TGraph2D("stability.txt");
   // //g->Draw("TRI1");

   char config_file[] = "../fit.cfg";
   CfgParser * gConfigParser = new CfgParser (config_file);
   string name = gConfigParser->readStringOpt("general::name");
   vector<float> cuts_x =gConfigParser->readFloatListOpt(name + "::stability_cuts_x");
   vector<int> cuts_bins =gConfigParser->readIntListOpt(name + "::stability_cuts_bins");
   float cuts_x_min = cuts_x[0], cuts_x_max = cuts_x[1];
   int cuts_bins_min= cuts_bins[0], cuts_bins_max = cuts_bins[1];

   ifstream tau,chi;
   tau.open(("files/tau_"+name+".txt").c_str());





   // chi.open(("files/chi_"+name+".txt").c_str());


   //
   // double cuts_x_min =1.0, cuts_x_max=1.7;
   // int cuts_bins_min=40, cuts_bins_max=180;
   // cout<< argv[2] << argv[5] <<endl;
   // cuts_x_min = stod(argv[2]);
   // cuts_x_max = stod(argv[3]);
   // cuts_bins_min = stoi(argv[4]);
   // cuts_bins_max = stoi(argv[5]);

   // double x;
   // int bins;
   double x_stop = 11;

   vector<float> range_x =gConfigParser->readFloatListOpt(name + "::stability_range_x");
   int x_divisions = gConfigParser->readIntOpt(name + "::stability_x_divisions");
   vector<int> range_bins =gConfigParser->readIntListOpt(name + "::stability_range_bins");
   int bins_divisions = gConfigParser->readIntOpt(name + "::stability_bins_divisions");

   double x_start = range_x[0], x_max = range_x[1];

   int bins_start = range_bins[0], bins_max = range_bins[1];


   double x_step = (x_max-x_start)/x_divisions;
   double bins_step = (bins_max-bins_start)/bins_divisions;


   // int x_bins = 100, y_bins = 230;
   // double x_start = 0.01, x_max=5, bins_start = 10, bins_max = 240;

   TH2F *hsurf = new TH2F("hsurf","Tau", x_divisions, x_start, x_max, bins_divisions, bins_start, bins_max);

   // ifstream inp;
   // // double x,y,z;
   // double x_increment =(2.375)/100;
   // double bins_increment = (double)(100-5);
   // inp.open("stability.txt");
   // while(!(inp >> x >> y >> z)==0){hsurf->SetBinContent(x,y,z);}
   // inp.close();
   // // Float_t px, py;
   // // for (Int_t i = 0; i < 25000; i++) {
   // //    gRandom->Rannor(px,py);
   // //    hsurf->Fill(px-1,5*py);
   // //    hsurf->Fill(2+0.5*px,2*py-10.,0.1);
   // // }
   // c2->cd();
   // hsurf->Draw("SURF1");
   // c2->Modified();
   // c2->Update();
   //  myApp->Run();

   TApplication* myApp = new TApplication("myApp", NULL, NULL);

   //TGraph2D * g = new TGraph2D("stability.txt");
   //g->Draw("TRI1");

   // TGraph2D *dt = new TGraph2D();
   // dt->SetTitle("Graph title; X axis title; Y axis title; Z axis title");
   int i,j;
   double z;
   // Float_t x;
   // Int_t bins;
   // TTree * tree = new TTree("T", "tau");
   // tree->Branch("x",&x,"x/F");
   // tree->Branch("bins",&bins,"bins/I");
   // double x_increment = (double)(4.5-0)/400;
   // double bins_increment = (double)(100-5)/400;

   // int i=0;
   // double tau_max =-10, tau_min = 1000;

   double x, bins;
   std::vector<double> taus;
   while(!(tau >> i >> j >> z)==0){
       // cout << x << endl;
       // x = x_start + i*x_step;
       // bins = bins_start + j*bins_step;
       x = hsurf->GetXaxis()->GetBinCenter(i);
       bins = hsurf->GetYaxis()->GetBinCenter(j);
       // cout <<  x << " " << bins << endl;
       if ((x>=cuts_x_min && x<=cuts_x_max) && (bins>=cuts_bins_min && bins<=cuts_bins_max)){
           // cout << "inside" << endl;
           // if (z > tau_max) tau_max = z;
           // if (z < tau_min) tau_min = z;
           taus.push_back(z);
           hsurf->SetBinContent(i,j,z);
       }
   }
   // cout << "lenght of taus" << taus.size() << endl;
   cout << "Stability analysis for " << name << " with cuts: " <<endl;
   cout << "cuts on x: " << cuts_x_min << ", "  << cuts_x_max << endl;
   cout << "cuts on bins: " << cuts_bins_min << ", " << cuts_bins_max << endl;
   cout << "Min tau: " << *max_element(taus.begin(), taus.end()) << "\nMax tau: " << *min_element(taus.begin(), taus.end()) <<endl;
   cout << "Mean: " << mean(taus) << "\nStd: " << standard_deviation(taus, mean(taus)) << endl;

   hsurf->GetXaxis()->SetRangeUser(cuts_x_min+x_step,cuts_x_max);
   hsurf->GetYaxis()->SetRangeUser(cuts_bins_min,cuts_bins_max);
   hsurf->GetXaxis()->SetTitle("Starting fit point [#mu s]");
   hsurf->GetYaxis()->SetTitle("Number of bins");

   // Float_t px, py;
   // for (Int_t i = 0; i < 25000; i++) {
   //    gRandom->Rannor(px,py);
   //    hsurf->Fill(px-1,5*py);
   //    hsurf->Fill(2+0.5*px,2*py-10.,0.1);
   // }
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);

   TCanvas *c3 = new TCanvas("c3","c3",1200,800);
   c3->cd();
   gStyle->SetPalette(1);
   hsurf->Draw("colz");
   c3->Modified();
   c3->Update();
   if (name == "cerbero_sopra_new_CORR_down_two_exp"){
       int search = gConfigParser->readIntOpt(name + "::search");
       if (search==1) name += "_muminus";
       else name += "_muplus";
   }
   c3->SaveAs(("images/tau_2d_zoomed_"+name+".png").c_str());
   // stability_chi_square
   //
   // TH2F *hsurf2 = new TH2F("hsurf2","Chi2/Ndf",x_bins,x_min,x_max,y_bins,y_min,y_max);
   // // int i=0;
   // while(!(chi >> i >> j >> z)==0){
   //
   //     hsurf2->SetBinContent(i,j,z);
   //     // cout<< x << " " << y << " " << z <<endl;
   //     // dt->SetPoint(i, x , y, z);
   //    // i++;
   // }
   // // Float_t px, py;
   // // for (Int_t i = 0; i < 25000; i++) {
   // //    gRandom->Rannor(px,py);
   // //    hsurf->Fill(px-1,5*py);
   // //    hsurf->Fill(2+0.5*px,2*py-10.,0.1);
   // // }
   // TCanvas *c5 = new TCanvas("c5","c5",1200,800);
   // c5->cd();
   // gStyle->SetPalette(1);
   // hsurf2->Draw("colz");
   // c5->Modified();
   // c5->Update();
   // c5->SaveAs(("images/chi_2d"+name+".png").c_str());

   myApp->Run();
}
