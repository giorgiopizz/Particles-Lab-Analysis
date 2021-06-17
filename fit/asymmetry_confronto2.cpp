/*
   c++ -o asymmetry_confronto2 asymmetry_confronto2.cpp `root-config --cflags --glibs` ../lib/CfgParser.cc -lm
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
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TMath.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "../lib/CfgParser.h"

using namespace std;

//background shoul be 2*r_start*r_stop*(10e-6)/bins*T_measure

string trimTitle (const string & title){
    string result = title;
    while (result.find(" ") != string::npos){
        result.erase(result.find(" "), 1);
    }
    return result;
}

int main(int argc, char** argv)
{
        //char config_file[] = argv[1];


        TApplication* myApp = new TApplication("myApp", NULL, NULL);



        TCanvas* cnv = new TCanvas("myC1","myC1",10,10,1200,800);

        cnv->Divide(2,2);
        gStyle->SetOptStat(11);
        gStyle->SetOptFit(1111);



        if (argc>1){
            // x_axis[0]-0.1, 4.3
            double range_min = 5, range_max = 11;
            TF1 * func = new TF1("fun", "[0]*TMath::Cos([1]*x[0]+[3])+[2]",range_min, range_max);


            func->SetParName(0,"A");
            func->SetParameter(0,0.0187);
            func->SetParLimits(0,0.001,0.8);
            func->SetParName(1,"#omega");
            func->SetParameter(1,1.78);
            func->SetParLimits(1,0,3);
            func->SetParName(2,"c");
            func->SetParameter(2,-0.3816);
            func->SetParName(3,"#phi");
            func->SetParameter(3, 0);


            TF1 * func_lin = new TF1("func_lin", "[0]*x[0]+[1]",range_min, range_max);



            // double bfield = 1.0e-4;
            //  // = func->GetParameter(1)*1.e6 ; //MHz
            // double hbar=1.054e-34;  // J s
            // double q=1.602e-19;     //  C
            // double c=2.998e8; // m/s
            // double m=105.658e6*q/(c*c);
            // double magneton=q*hbar/(2.*m);  // 4.485e-26 * J/T
            // double g_teor = 2.00233184122;
            // // double g_stim=omega*hbar/(magneton*bfield);
            // double omega = g_teor*(magneton*bfield)/(hbar)/1e6;
            // cout << omega << endl;


            // "../asymmetry_confronto_simulazione.root"
            TFile * sim = new TFile(argv[1], "read");

            sim->ls();
            TGraphErrors * g1 = (TGraphErrors*)sim->Get("acceso");
            TGraphErrors * g3 =(TGraphErrors*) g1->Clone();
            // for(int i=0;i<h1->GetN();i++){
            //     h1->SetPointError(i, 0,0);
            // }
            // h1->MovePoints(0, -0.3113);
            // g1->SetLineStyle(10);
            g1->SetMarkerStyle(21);
            cnv->cd(1);
            g1->Draw("ACP");
            TFitResultPtr r1 = g1->Fit(func,"SR");


            // g3->SetLineStyle(10);
            g3->SetMarkerStyle(21);
            cnv->cd(3);
            g3->Draw("ACP");
            TFitResultPtr r3 = g3->Fit(func_lin,"SR");

            // g1->GetXaxis()->SetLimits(0,7);
            // func->SetParameter(1,1.70);
            // h1->Fit(func, "SR");
            // h1->Draw("same");
            // auto mg1 = new TMultiGraph();
            // mg1->Add(h1);
            TGraphErrors * g2 = (TGraphErrors*)sim->Get("spento");
            TGraphErrors * g4 = (TGraphErrors*)g2->Clone();
            // for(int i=0;i<h2->GetN();i++){
            //     h2->SetPointError(i, 0,0);
            // }
            // h2->MovePoints(0, -0.3113);
            // g2->SetLineStyle(10);
            g2->SetMarkerColor(3);
            g2->SetMarkerStyle(21);
            // h2->GetXaxis()->SetLimits(0,7);
            // h2->Draw("same");
            // mg1->Add(h2);

            cnv->cd(2);
            g2->Draw("ACP");
            func->SetParameter(1,0.08);
            TFitResultPtr r2 = g2->Fit(func,"SR");


            g2->SetMarkerColor(3);
            g2->SetMarkerStyle(21);
            // h2->GetXaxis()->SetLimits(0,7);
            // h2->Draw("same");
            // mg1->Add(h2);

            cnv->cd(4);
            g4->Draw("ACP");
            TFitResultPtr r4 = g4->Fit(func_lin,"SR");

            // mg1->SetTitle("Simulazione");
            // mg1->Draw("ACP");
            sim->Close();
            cnv->Modified();
            cnv->Update();
            cnv->SaveAs("images/asym_sim_nostroExp_3.png");

        }
        else{

            char config_file[] = "../fit.cfg";
            CfgParser * gConfigParser = new CfgParser (config_file);


            string name1 = "magnetico_acceso_asym";
            string name2 = "magnetico_spento_perp_merged_tot";

            // int function_type = gConfigParser->readIntOpt (name+"::f_type");


            // string filename = gConfigParser->readStringOpt(name+"::filename");

            // string title = gConfigParser->readStringOpt(name+"::title");
            //
            // int save = gConfigParser->readIntOpt(name+"::save");

            // vector<float> parameters = gConfigParser->readFloatListOpt (name+"::parameters");

            vector<float> range = gConfigParser->readFloatListOpt (name1+"::range");
            float bin = gConfigParser->readFloatOpt (name1+"::bin");

            string filename1_up = gConfigParser->readStringOpt(name1+"::filename_up");
            string filename1_down = gConfigParser->readStringOpt(name1+"::filename_down");

            string filename2_up = gConfigParser->readStringOpt(name2+"::filename_up");
            string filename2_down = gConfigParser->readStringOpt(name2+"::filename_down");


            //int search = gConfigParser->readIntOpt (name+"::search");





            // TF1 * func;
            // if(function_type==1){
            //     func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1]) + [2]",range[0], range[1]);
            //
            // }
            // else if(function_type==2){
            //     func = new TF1("fun", "[0]*TMath::Exp(-x[0]/[1]) +  [2]*TMath::Exp(-x[0]/[3])+ [4]",range[0], range[1]);
            // }
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
            // vector<string>  names;
            // string n1[] = {"N", "\\tau_{\\mu}", "Bkg"};
            // string n2[] = {"N_{-}", "\\tau_{\\mu^-}","N_{+}", "\\tau_{\\mu^+}", "Bkg"};
            // if (function_type==1){
            //     for (int i=0;i<3;i++)   names.push_back(n1[i]);
            //     cout << "minor" <<endl;
            // }
            // else{
            //     for (int i=0;i<5;i++)   names.push_back(n2[i]);
            // }
            //
            // for (int i=0; i<parameters.size(); i++) {
            //
            //         func->SetParameter(i, parameters[i]);
            //         func->SetParName(i, names[i].c_str());
            // }
            // vector<double> delays, omegas;
            // double delay;
            // for(int i=0;i<30;i++){
            //     TF1 * func = new TF1("fun", "[0]*TMath::Cos([1]*x[0]+[3])+[2]",0.59, 5);
            //
            //
            //     func->SetParName(0,"A");
            //     func->SetParameter(0,0.0289);
            //     func->SetParLimits(0,0.01,0.1);
            //     func->SetParName(1,"#omega");
            //     func->SetParameter(1,1.91);
            //     func->SetParLimits(1,1,3);
            //     func->SetParName(2,"c");
            //     func->SetParameter(2,0);
            //     func->SetParName(3,"#phi");
            //     delay = i*0.05+0.01;
            //     delays.push_back(delay);
            //     TH1F *h_up = new TH1F("hup", "example histogram",bin,0,11);
            //     ifstream inp;
            //     double x;
            //     inp.open(filename_up);
            //     while(!(inp >> x)==0) {h_up->Fill(x-delay);}
            //     inp.close();
            //
            //     TH1F *h_down = new TH1F("hdown", "exampole histogram",bin,0,11);
            //     inp.open(filename_down);
            //     while(!(inp >> x)==0) {h_down->Fill(x);}
            //     inp.close();
            //
            //     // TCanvas* cnv3 =  new TCanvas("myC3","myC3",10,10,1200,800);
            //     // cnv3->cd();
            //     // h_up->Draw();
            //     // TF1 * func3 = new TF1("fun3", "[0]*TMath::Exp(-x[0]/[1]) + [2]",0, 11);
            //     // func3->SetParameter(1,2.2);
            //     // h_up->Fit(func3);
            //     // cnv3->Modified();
            //     // cnv3->Update();
            //     //
            //     // TCanvas* cnv4 =  new TCanvas("myC4","myC4",10,10,1200,800);
            //     // cnv4->cd();
            //     // h_down->Draw();
            //     // TF1 * func4 = new TF1("fun4", "[0]*TMath::Exp(-x[0]/[1]) + [2]",0, 11);
            //     // func4->SetParameter(1,2.2);
            //     // h_down->Fit(func4);
            //     // cnv4->Modified();
            //     // cnv4->Update();
            //
            //     double x_axis[(int)bin];
            //     double y_axis[(int)bin];
            //     double x_error[(int)bin];
            //     double y_error[(int)bin];
            //     double up, down;
            //     for(int i=1;i<bin+1;i++){
            //         x_axis[i] = h_up->GetBinCenter(i);
            //         x_error[i] = (11-0)/(bin*sqrt(12));
            //         up = h_up->GetBinContent(i);
            //         down = h_down->GetBinContent(i);
            //         if (up+down!=0){
            //             y_axis[i] = (up-down)/(up+down);
            //             y_error[i] = 2*sqrt(up*down/pow(up+down,3));
            //         }
            //         else y_axis[i] = 0;
            //     }
            //
            //     TGraph * g = new TGraphErrors(bin, x_axis, y_axis, x_error, y_error);
            //
            //     // cnv->cd();
            //     // g->Draw("ALP");
            //     g->GetXaxis()->SetRangeUser(0.3,11);
            //     TFitResultPtr r = g->Fit(func,"SR");
            //     omegas.push_back(func->GetParameter(1));
            //     cout << delays[i] << " " << omegas[i] << endl;
            // }
            //

            double delay = 0.02;
            TH1F *h_up = new TH1F("hup", "example histogram",bin,0,11);
            ifstream inp;
            double x;
            inp.open(filename1_up);
            while(!(inp >> x)==0) {h_up->Fill(x-delay);}
            inp.close();

            TH1F *h_down = new TH1F("hdown", "exampole histogram",bin,0,11);
            inp.open(filename1_down);
            while(!(inp >> x)==0) {h_down->Fill(x);}
            inp.close();
            //
            // TCanvas* cnv3 =  new TCanvas("myC3","myC3",10,10,1200,800);
            // cnv3->cd();
            // h_up->Draw();
            // TF1 * func3 = new TF1("fun3", "[0]*TMath::Exp(-x[0]/[1]) + [2]",0, 11);
            // func3->SetParameter(1,2.2);
            // h_up->Fit(func3);
            // cnv3->Modified();
            // cnv3->Update();
            //
            // TCanvas* cnv4 =  new TCanvas("myC4","myC4",10,10,1200,800);
            // cnv4->cd();
            // h_down->Draw();
            // TF1 * func4 = new TF1("fun4", "[0]*TMath::Exp(-x[0]/[1]) + [2]",0, 11);
            // func4->SetParameter(1,2.2);
            // h_down->Fit(func4);
            // cnv4->Modified();
            // cnv4->Update();

            // double x_axis[(int)bin];
            // double y_axis[(int)bin];
            // double x_error[(int)bin];
            // double y_error[(int)bin];
            vector<double> x_axis, y_axis, x_error,y_error;
            double up, down;
            for(int i=1;i<bin+1;i++){

                up = h_up->GetBinContent(i);
                down = h_down->GetBinContent(i);
                if ((up+down)!=0){
                    x_axis.push_back(h_up->GetBinCenter(i));
                    x_error.push_back((11-0)/(bin*sqrt(12)));
                    y_axis.push_back((up-down)/(up+down));
                    y_error.push_back(2*sqrt(up*down/pow(up+down,3)));
                }
                // else y_axis[i] = 0;
            }
            // x_axis[0]-0.1, 4.3
            double range_min = 4.5, range_max = 11;
            TF1 * func = new TF1("fun", "[0]*TMath::Cos([1]*x[0]+[3])+[2]",range_min, range_max);


            func->SetParName(0,"A");
            func->SetParameter(0,0.0289);
            func->SetParLimits(0,0.001,0.8);
            func->SetParName(1,"#omega");
            func->SetParameter(1,1.788);
            func->SetParLimits(1,0,3);
            func->SetParName(2,"c");
            func->SetParameter(2,-0.3816);
            func->SetParName(3,"#phi");
            func->SetParameter(3, 0);


            TF1 * func_lin = new TF1("func_lin", "[0]*x[0]+[1]",range_min, range_max);



            TGraph * g1 = new TGraphErrors(x_axis.size(), x_axis.data(), y_axis.data(), x_error.data(), y_error.data());
            TGraph * g3 = new TGraphErrors(x_axis.size(), x_axis.data(), y_axis.data(), x_error.data(), y_error.data());
            cnv->cd(1);
            // auto mg = new TMultiGraph();
            g1->SetMarkerStyle(20);
            g1->SetTitle("With Magnetic Field (21.7 Gauss) - Cos fit");
            // mg->Add(g);
            g1->Draw("ACP");
            // func->SetParameter(1,1.70);
            // g1->GetXaxis()->SetRangeUser(0.3,11);

            TFitResultPtr r1 = g1->Fit(func,"SR");

            cnv->cd(3);
            // auto mg = new TMultiGraph();
            g3->SetMarkerStyle(20);
            g3->SetTitle("With Magnetic Field (21.7 Gauss) - Lin fit");
            // mg->Add(g);
            g3->Draw("ACP");
            // func->SetParameter(1,1.70);
            // g3->GetXaxis()->SetRangeUser(0.3,11);

            TFitResultPtr r3 = g3->Fit(func_lin,"SR");




            delete h_up, h_down;

            h_up = new TH1F("hup", "example histogram",bin,0,11);
            inp.open(filename2_up);
            while(!(inp >> x)==0) {h_up->Fill(x-delay);}
            inp.close();

            h_down = new TH1F("hdown", "exampole histogram",bin,0,11);
            inp.open(filename2_down);
            while(!(inp >> x)==0) {h_down->Fill(x);}
            inp.close();
            //
            // TCanvas* cnv3 =  new TCanvas("myC3","myC3",10,10,1200,800);
            // cnv3->cd();
            // h_up->Draw();
            // TF1 * func3 = new TF1("fun3", "[0]*TMath::Exp(-x[0]/[1]) + [2]",0, 11);
            // func3->SetParameter(1,2.2);
            // h_up->Fit(func3);
            // cnv3->Modified();
            // cnv3->Update();
            //
            // TCanvas* cnv4 =  new TCanvas("myC4","myC4",10,10,1200,800);
            // cnv4->cd();
            // h_down->Draw();
            // TF1 * func4 = new TF1("fun4", "[0]*TMath::Exp(-x[0]/[1]) + [2]",0, 11);
            // func4->SetParameter(1,2.2);
            // h_down->Fit(func4);
            // cnv4->Modified();
            // cnv4->Update();

            // double x2_axis[(int)bin];
            // double y2_axis[(int)bin];
            // double x2_error[(int)bin];
            // double y2_error[(int)bin];
            // for(int i=1;i<bin+1;i++){
            //     x2_axis[i] = h_up->GetBinCenter(i);
            //     x2_error[i] = (11-0)/(bin*sqrt(12));
            //     up = h_up->GetBinContent(i);
            //     down = h_down->GetBinContent(i);
            //     if (up+down!=0){
            //         y2_axis[i] = (up-down)/(up+down);
            //         y2_error[i] = 2*sqrt(up*down/pow(up+down,3));
            //     }
            //     else y2_axis[i] = 0;
            // }
            //
            // TGraph * g2 = new TGraphErrors(bin, x2_axis, y2_axis, x2_error, y2_error);
            // TGraph * g4 = new TGraphErrors(bin, x2_axis, y2_axis, x2_error, y2_error);


            vector<double> x2_axis, y2_axis, x2_error,y2_error;
            for(int i=2;i<bin+1;i++){

                up = h_up->GetBinContent(i);
                down = h_down->GetBinContent(i);
                if ((up+down)!=0){
                    x2_axis.push_back(h_up->GetBinCenter(i));
                    x2_error.push_back((11-0)/(bin*sqrt(12)));
                    y2_axis.push_back((up-down)/(up+down));
                    y2_error.push_back(2*sqrt(up*down/pow(up+down,3)));
                }
                // else y_axis[i] = 0;
            }

            TGraph * g2 = new TGraphErrors(x2_axis.size(), x2_axis.data(), y2_axis.data(), x2_error.data(), y2_error.data());
            TGraph * g4 = new TGraphErrors(x2_axis.size(), x2_axis.data(), y2_axis.data(), x2_error.data(), y2_error.data());


            g2->SetLineColor(3);
            g2->SetMarkerColor(3);
            g2->SetMarkerSize(1);
            g2->SetMarkerStyle(20);
            // g1->Draw("same");
            // mg->Add(g1);
            cnv->cd(2);
            g2->SetTitle("No Magnetic Field - Cos fit");
            g2->Draw("ACP");
            func->SetParameter(1,0.08);
            TFitResultPtr r2 = g2->Fit(func,"SR");
            // g2->GetXaxis()->SetRangeUser(0.3,11);

            g4->SetLineColor(3);
            g4->SetMarkerColor(3);
            g4->SetMarkerSize(1);
            g4->SetMarkerStyle(20);

            cnv->cd(4);
            g4->SetTitle("No Magnetic Field - Lin fit");
            g4->Draw("ACP");
            TFitResultPtr r4 = g4->Fit(func_lin,"SR");
            // g4->GetXaxis()->SetRangeUser(0.3,11);



            double csi = func->GetParameter(0)*6/0.54;
            cout << "La csi: " << csi << endl;
            double bfield = 21.74e-4;
            double omega = func->GetParameter(1)*1.e6 ; //MHz
            double hbar=1.054e-34;  // J s
            double q=1.602e-19;     //  C
            double c=2.998e8; // m/s
            double m=105.658e6*q/(c*c);
            double magneton=q*hbar/(2.*m);  // 4.485e-26 * J/T

            double g_stim=omega*hbar/(magneton*bfield);
            cout << "Val g: " << g_stim <<endl;
            // h->Draw();

            // TFitResultPtr r = h->Fit("fun","SR");
            //
            // double tot = func->GetParameter(0)+func->GetParameter(2);
            // double prop = func->GetParameter(0)/tot*100;
            // cout << "proprorzioni:\nneg: " << prop << "\npos: " << 100-prop <<endl;
            //
            // cout << "Vita media: " << func->GetParameter(1) << " microsec ± " <<  r->ParError(1) <<endl;
            // cout << "errore percentuale: " << r->ParError(1)/func->GetParameter(1) * 100 << endl;
            // double tau = 2.1969811;
            // double t = abs(tau-func->GetParameter(1))/r->ParError(1);
            // cout << "t: " << t <<endl;

            /*stringstream s;
            s << "Vita media: " << round(func->GetParameter(1)*1000)/1000 << " +/- " <<  round(r->ParError(1)*1000)/1000 << " microsec. t: " << round(t*1000)/1000;
            h->SetTitle(s.str().c_str());
            */
            // h->SetTitle(title.c_str());




       //  auto legend = new TLegend(0.1,0.7,0.48,0.9);
       // legend->SetHeader("Legenda","C"); // option "C" allows to center the header
       // legend->AddEntry(g1,"Magnetico Acceso","lep");
       // legend->AddEntry(g2,"Magnetico Spento","lep");
       // legend->Draw();
       cnv->Modified();
       cnv->Update();
       cnv->SaveAs("images/asym_exp_3.png");
            //
            //
            // TF1* f_down = new TF1("down","[0]*exp(-x[0]/[1])*(1 + [2]*cos([3]*x[0] + [4]) )", 0.2, 11);
            //
            // f_down->SetParName(0,"A");
            // f_down->FixParameter(0,1000);
            // f_down->SetParName(1,"tau");
            // f_down->FixParameter(1,2.196);
            // f_down->SetParName(2,"B");
            // f_down->SetParameter(2,0.1);
            // f_down->SetParName(3,"omega");
            // f_down->SetParameter(3,1.91);
            // f_down->SetParName(4,"delta");
            // f_down->SetParameter(4,0);
            //
            //
            // TCanvas* cnv2 = new TCanvas("myC2","myC1",10,10,1200,800);
            // cnv2->cd();
            // h_down = new TH1F("h_down", "down histo",100,0,11);
            // inp.open(filename_down);
            // while(!(inp >> x)==0) {h_down->Fill(x);}
            //
            //
            //
            // h_down->Draw();
            // h_down->Fit(f_down,"R");
            // cnv2->Modified();
            // cnv2->Update();


        }
        myApp->Run();
        return 0;
}
