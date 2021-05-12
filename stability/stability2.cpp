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

  int function_type = gConfigParser->readIntOpt (name+"::f_type");
  vector<float> parameters = gConfigParser->readFloatListOpt (name+"::parameters");

  int fix = gConfigParser->readIntOpt(name+"::fix");
  int search = gConfigParser->readIntOpt(name+"::search");
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
  for(int i=1; i < x_divisions+1; i++){

      x = x_start + i*x_step;
      // TF1 * func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1])+[2]",x, x_stop);

      TF1 * func;
      if(function_type==1){
          func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1]) + [2]",x, x_stop);

      }
      else if(function_type==2){
          func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1]) +  [2]*TMath::Exp(-x[0]/[3])+ [4]",x, x_stop);
      }

      vector<string>  names;
      string n1[] = {"N", "\\tau_{\\mu}", "Bkg"};
      string n2[] = {"N_{-}", "\\tau_{\\mu^-}","N_{+}", "\\tau_{\\mu^+}", "Bkg"};
      if (function_type==1){
          for (int l=0;l<3;l++)   names.push_back(n1[l]);
      }
      else{
          for (int l=0;l<5;l++)   names.push_back(n2[l]);
      }

      for (int l=0; l<parameters.size(); l++) {
          if (name != "cerbero_sopra_new_CORR_down_two_exp"){
              if(fix && l==4){
                  func->FixParameter(l, parameters[l]);
                  func->SetParName(l, names[l].c_str());
              }
              else{
                  func->SetParameter(l, parameters[l]);
                  func->SetParName(l, names[l].c_str());
              }
          }
          // else{
          //     func->SetParameter(l, parameters[l]);
          //     func->SetParName(l, names[l].c_str());
          // }


      }
      //
      // func->SetParameter(0,5000);
      // func->SetParameter(1,2.1);
      // func->SetParameter(2,100);
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

          if (name == "cerbero_sopra_new_CORR_down_two_exp"){
              func->FixParameter(1, 2.026);
              func->FixParameter(3, 2.197);
              h->Fit("fun","RQ");


              // func->ReleaseParameter(1);
              func->ReleaseParameter(3);

              func->FixParameter(0, func->GetParameter(0));
              // func->FixParameter(1, func->GetParameter(1));
              func->SetParameter(2, func->GetParameter(2));
              // func->SetParameter(3, func->GetParameter(3));
              func->FixParameter(4, func->GetParameter(4));
              TFitResultPtr r = h->Fit("fun","SRQ");

              double tau_plus = func->GetParameter(3);
              double reduce_chi_plus = r->Chi2()/r->Ndf();
              if (!(tau_plus<2.25 && tau_plus>2.18)) cout << "\n\nerror on tau_plus\n" << tau_plus << "\n" << func->GetParameter(1)<< endl;

              func->ReleaseParameter(0);
              func->ReleaseParameter(1);


              // func->SetParameter(0, func->GetParameter(0));
              // func->SetParameter(1, func->GetParameter(1));
              func->FixParameter(2, func->GetParameter(2));
              func->FixParameter(3, func->GetParameter(3));
              r = h->Fit("fun","SRQ");

              func->ReleaseParameter(2);
              func->ReleaseParameter(3);
              func->ReleaseParameter(4);
              double tau_minus = func->GetParameter(1);
              double reduce_chi_minus = r->Chi2()/r->Ndf();

              if (!(tau_minus<2.09 && tau_minus>1.95)) cout << "\n\nerror on tau_minus\n" << tau_minus << endl;

              // cout << "N + ricavato: " << n_plus << " ± " << n_plus_err <<endl;
              // cout << "tau + ricavato: " << tau_plus << " ± " << tau_plus_err <<endl;
              //
              // cout << "N - ricavato: " << n_minus << " ± " << n_minus_err <<endl;
              // cout << "tau - ricavato: " << tau_minus << " ± " << tau_minus_err <<endl;

              //cout << "Fit status: "  << r->Status() << endl;
              if (r->Status()==0 && tau_minus<2.09 && tau_minus>1.95 && tau_plus<2.25 && tau_plus>2.18){

                  if (search==1){


                  // if(r->Status()==0 && tau_minus<4.0 && tau_minus>0.2){
                      tau << i <<"\t"<< j <<"\t"<<tau_minus<<endl;
                      chi << i <<"\t"<< j <<"\t"<<reduce_chi_minus<<endl;
                  // }
                  // else{
                  //     cout << "Error" << k << " " << bins << " " << x << endl;
                  //     k++;
                  // }
              }
                  else{
                  // if(r->Status()==0 && tau_plus<2.25 && tau_plus>2.05){
                      tau << i <<"\t"<< j <<"\t"<<tau_plus<<endl;
                      chi << i <<"\t"<< j <<"\t"<<reduce_chi_plus<<endl;
                  // }
                  // else{
                  //     cout << "Error" << k << " " << bins << " " << x << "\t" << tau_minus<< endl;
                  //     k++;
                  // }
              }
              }
              else{
                  cout << "Error" << k << " " << bins << " " << x << "\t" << tau_minus << "\t " << tau_plus << endl;
                  k++;
              }
          }
          else{
            TFitResultPtr r = h->Fit("fun","SRQ");
             //cout << "Fit status: "  << r->Status() << endl;
             if(r->Status()==0 && r->Parameter(1)<4.0 && r->Parameter(1)>0.2){
                 tau << i <<"\t"<< j <<"\t"<<r->Parameter(1)<<endl;
                 chi << i <<"\t"<< j <<"\t"<<r->Chi2()/r->Ndf()<<endl;
             }
             else{
                 cout << "Error" << k << " " << bins << " " << x << endl;
                 k++;
             }
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
