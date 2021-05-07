/*
c++ -o stability2 stability2.cpp `root-config --cflags --glibs` ../lib/CfgParser.cc
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
#include "../lib/CfgParser.h"


using namespace std;


int main(int argc, char** argv)
{


  //TApplication* myApp = new TApplication("myApp", NULL, NULL);


  // ########################################
  // # Choice of the minimization algorithm #
  // ########################################
  // Use numerical minimization algorithm: MIGRAD


  // TGraphErrors constructor from file
  /*myGraph->GetXaxis()->SetTitle("lambda (nm)");
  myGraph->GetYaxis()->SetTitle("Refractive index");
  myGraph->SetMarkerStyle(20);
  myGraph->SetMarkerSize(0.4);
  myC0->cd();
  myGraph->Draw("AP");
  TFitResultPtr r = myGraph->Fit("myFun","S");
  myC0->Modified();
  myC0->Update();*/
/*
  cout << "\nReduced Chi-2: " << myFun->GetChisquare()/myFun->GetNDF() << endl;
  cout << "p-value: " << myFun->GetProb() << endl;
  double Chi2, NDF, pvalue;
  computeChi2(myGraph,myFun,Chi2,NDF,pvalue);
  cout << "\nMy Chi-2: " << Chi2 << endl;
  cout << "My NDF: " << NDF << endl;
  cout << "My reduced Chi-2: " << Chi2/NDF << endl;
  cout << "My p-value: " << pvalue << endl;
  r->Print("V");
*/

  //TCanvas* cnv = new TCanvas("myC1","myC1",10,10,1200,800);


  char config_file[] = "../fit.cfg";
  CfgParser * gConfigParser = new CfgParser (config_file);
  string name = gConfigParser->readStringOpt("general::name");

  string filename = gConfigParser->readStringOpt(name+"::filename");



  ofstream tau,chi;
  tau.open(("files/tau_"+name+".txt").c_str());
  chi.open(("files/chi_"+name+".txt").c_str());
  //gStyle->SetOptFit(1111);
  //TF1 * func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1])",0.5, 10);
  double x;
  int bins;
  double x_stop = 11;

  vector<float> range_x =gConfigParser->readFloatListOpt(name + "::stability_range_x");
  int x_divisions = gConfigParser->readIntOpt(name + "::stability_x_divisions");
  vector<int> range_bins =gConfigParser->readIntListOpt(name + "::stability_range_bins");
  int bins_divisions = gConfigParser->readIntOpt(name + "::stability_bins_divisions");

  double x_start = range_x[0], x_max = range_x[1];

  int bins_start = range_bins[0], bins_max = range_bins[1];


  double x_step = (x_max-x_start)/x_divisions;
  double bins_step = (bins_max-bins_start)/bins_divisions;

  int k = 0;
  // we let the starting point of the fit range vary from 0.01 to 5


  //double bins_increment = 1;
  for(int i=1; i< x_divisions+1;i++){
      x = x_start + i*x_step;
      TF1 * func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1])+[2]",x, x_stop);
      func->SetParameter(0,5000);
      func->SetParameter(1,2.1);
      func->SetParameter(2,100);
      for(int j=1;j<bins_divisions+1;j++){
          bins = bins_start + j*bins_step;
          //cout << "xmin: " << xmin << "\nnbins: " << nbins<<endl;


          TH1F *h = new TH1F("h", "example histogram",bins,0,11);
          ifstream inp;
          double point;
          inp.open(filename);
          while(!(inp >> point)==0){h->Fill(point);}
          inp.close();
          //cnv->cd();
          //h->Draw();
          //h->Fit(func);

          /*
          TH1*  histo= new TH1("histo", argv[1]);
          graph->SetTitle("Calibrazione con Pulser");

          graph->Draw("AP");*/
          /*graph->GetXaxis()->SetTitle("Channels[mV]");
          graph->GetYaxis()->SetTitle("Energies[KeV]");*/
          TFitResultPtr r = h->Fit("fun","SRQ");
          //cout << "Fit status: "  << r->Status() << endl;
          if(r->Status()==0 && r->Parameter(1)<4.0 && r->Parameter(1)>1.0){
              tau << i <<"\t"<< j <<"\t"<<r->Parameter(1)<<endl;
              chi << i <<"\t"<< j <<"\t"<<r->Chi2()/r->Ndf()<<endl;
          }
          else{
              cout << "Error" << k << " " << bins << " " << x << endl;
              k++;
          }
          //cout << "Vita media: " << func->GetParameter(1) << " microsec ± " <<  r->ParError(1) <<endl;
          // cout << "errore percentuale: " << r->ParError(1)/func->GetParameter(1) * 100 << endl;
          // double tau = 2.1969811;
          // double t = abs(tau-func->GetParameter(1))/r->ParError(1);
          // cout << "t: " << t <<endl;
          //r->Parameter(1)<<"\t"<<
         /*
          float x[4]={3.96971e+03,4.00621e+03,4.03594e+03,4.06157e+03};
          float y_real[4]={5388,5443,5486,5545};
          for(int i=0;i<4;i++){
              cout<< x[i]<<" :\t"<<func->Eval(x[i])<<"\t"<<y_real[i]-func->Eval(x[i])<<endl;
          }
        */


          delete h;

      }
      delete func;
  }




  tau.close();
  chi.close();


  // stringstream s;
  // s << "Vita media: " << round(func->GetParameter(1)*1000)/1000 << " +/- " <<  round(r->ParError(1)*1000)/1000 << " microsec. t: " << round(t*1000)/1000 ;
  // // h->SetTitle(s.str().c_str());
  // cnv->Modified();
  // cnv->Update();
  // cnv->Print("fit_vita_media.png", "png");
  //myApp->Run();
  return 0;
}
