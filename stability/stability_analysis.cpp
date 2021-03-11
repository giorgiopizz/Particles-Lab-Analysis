/*
c++ -o stability_analysis stability_analysis.cpp `root-config --cflags --glibs`  ../lib/CfgParser.cc
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


int main(int argc, char** argv)
{
   // TApplication* myApp = new TApplication("myApp", NULL, NULL);
   // TCanvas *c2 = new TCanvas("c2","c2",600,400);
   // //TGraph2D * g = new TGraph2D("stability.txt");
   // //g->Draw("TRI1");

   char config_file[] = "../fit.cfg";
   CfgParser * gConfigParser = new CfgParser (config_file);
   string name = gConfigParser->readStringOpt("general::name");


   ifstream tau,chi;
   tau.open(("files/tau_"+name+".txt").c_str());
   chi.open(("files/chi_"+name+".txt").c_str());

   int x_bins = 100, y_bins = 230;
   double x_min = 6, x_max=10.75, y_min = 10, y_max = 240;
   TH2F *hsurf = new TH2F("hsurf","Tau",x_bins,x_min,x_max,y_bins,y_min,y_max);
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
   // double x_increment = (double)(4.5-0)/400;
   // double bins_increment = (double)(100-5)/400;

   // int i=0;
   while(!(tau >> i >> j >> z)==0){

       hsurf->SetBinContent(i,j,z);
       // cout<< x << " " << y << " " << z <<endl;
       // dt->SetPoint(i, x , y, z);
      // i++;
   }
   // Float_t px, py;
   // for (Int_t i = 0; i < 25000; i++) {
   //    gRandom->Rannor(px,py);
   //    hsurf->Fill(px-1,5*py);
   //    hsurf->Fill(2+0.5*px,2*py-10.,0.1);
   // }
   TCanvas *c2 = new TCanvas("c2","c2",1200,800);
   c2->cd();
   gStyle->SetPalette(1);
   hsurf->Draw("surf1");
   c2->Modified();
   c2->Update();
   c2->SaveAs(("images/tau_3d"+name+".png").c_str());

   TCanvas *c3 = new TCanvas("c3","c3",1200,800);
   c3->cd();
   hsurf->Draw("colz");
   c3->Modified();
   c3->Update();
   c3->SaveAs(("images/tau_2d"+name+".png").c_str());
   // stability_chi_square

   TH2F *hsurf2 = new TH2F("hsurf2","Chi2/Ndf",x_bins,x_min,x_max,y_bins,y_min,y_max);
   // int i=0;
   while(!(chi >> i >> j >> z)==0){

       hsurf2->SetBinContent(i,j,z);
       // cout<< x << " " << y << " " << z <<endl;
       // dt->SetPoint(i, x , y, z);
      // i++;
   }
   // Float_t px, py;
   // for (Int_t i = 0; i < 25000; i++) {
   //    gRandom->Rannor(px,py);
   //    hsurf->Fill(px-1,5*py);
   //    hsurf->Fill(2+0.5*px,2*py-10.,0.1);
   // }
   TCanvas *c4 = new TCanvas("c4","c4",1200,800);
   c4->cd();
   gStyle->SetPalette(1);
   hsurf2->Draw("surf1");
   c4->Modified();
   c4->Update();
   c4->SaveAs(("images/chi_3d"+name+".png").c_str());
   TCanvas *c5 = new TCanvas("c5","c5",1200,800);
   c5->cd();
   gStyle->SetPalette(1);
   hsurf2->Draw("colz");
   c5->Modified();
   c5->Update();
   c5->SaveAs(("images/chi_2d"+name+".png").c_str());

   myApp->Run();
}
