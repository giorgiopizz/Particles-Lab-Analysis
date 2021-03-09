/*
c++ -o stability_analysis stability_analysis.cpp `root-config --cflags --glibs`
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


using namespace std;


int main(int argc, char** argv)
{
   TApplication* myApp = new TApplication("myApp", NULL, NULL);
   TCanvas *c2 = new TCanvas("c2","c2",600,400);
   //TGraph2D * g = new TGraph2D("stability.txt");
   //g->Draw("TRI1");
   TH2F *hsurf = new TH2F("hsurf","Option SURF example ",400,0,4.5,400,5,100);
   ifstream inp;
   double x,y,z;
   double x_increment = (double)(4.5-0)/400;
   double bins_increment = (double)(100-5)/400;
   inp.open("stability.txt");
   while(!(inp >> x >> y >> z)==0){hsurf->SetBinContent(x/x_increment,y/bins_increment,z);}
   inp.close();
   // Float_t px, py;
   // for (Int_t i = 0; i < 25000; i++) {
   //    gRandom->Rannor(px,py);
   //    hsurf->Fill(px-1,5*py);
   //    hsurf->Fill(2+0.5*px,2*py-10.,0.1);
   // }
   c2->cd();
   hsurf->Draw("SURF1");
   c2->Modified();
   c2->Update();
    myApp->Run();
}
