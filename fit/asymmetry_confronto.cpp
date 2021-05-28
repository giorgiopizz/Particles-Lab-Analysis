/*
   c++ -o asymmetry_confronto asymmetry_confronto.cpp `root-config --cflags --glibs` ../lib/CfgParser.cc -lm
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

        TApplication* myApp = new TApplication("myApp", NULL, NULL);



        TCanvas* cnv = new TCanvas("myC1","myC1",10,10,1200,800);
        cnv->Divide(1,2);
        gStyle->SetOptStat(11);
        gStyle->SetOptFit(1111);
        TF1 * func = new TF1("fun", "[0]*TMath::Cos([1]*x[0]+[3])+[2]",0, 6.10);


        func->SetParName(0,"A");
        func->FixParameter(0,0.0289);
        // func->SetParLimits(0,0.01,0.1);
        func->SetParName(1,"#omega");
        func->FixParameter(1,1.8487);
        func->SetParLimits(1,1,3);
        func->SetParName(2,"c");
        func->FixParameter(2,-0.3816);
        func->SetParName(3,"#phi");
        func->FixParameter(3, 0);

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

        double x_axis[(int)bin];
        double y_axis[(int)bin];
        double x_error[(int)bin];
        double y_error[(int)bin];
        double up, down;
        for(int i=1;i<bin+1;i++){
            x_axis[i] = h_up->GetBinCenter(i);
            x_error[i] = (11-0)/(bin*sqrt(12));
            up = h_up->GetBinContent(i);
            down = h_down->GetBinContent(i);
            if (up+down!=0){
                y_axis[i] = (up-down)/(up+down);
                y_error[i] = 2*sqrt(up*down/pow(up+down,3));
            }
            else y_axis[i] = 0;
        }

        TGraph * g = new TGraphErrors(bin, x_axis, y_axis, x_error, y_error);

        // cnv->cd();
        auto mg = new TMultiGraph();
        g->SetMarkerStyle(20);
        mg->Add(g);
        // g->Draw("ACP");
        // func->SetParameter(1,1.70);
        g->GetXaxis()->SetRangeUser(0.3,11);
        TFitResultPtr r = g->Fit(func,"SR");


        TFile * sim = new TFile("../asymmetry_confronto_simulazione.root", "read");

        sim->ls();
        TGraphErrors * h1 = (TGraphErrors*)sim->Get("acceso");
        // for(int i=0;i<h1->GetN();i++){
        //     h1->SetPointError(i, 0,0);
        // }
        // h1->MovePoints(0, -0.3113);
        h1->SetLineStyle(10);

        h1->SetMarkerStyle(21);
        h1->GetXaxis()->SetLimits(0,7);
        func->SetParameter(1,1.70);
        h1->Fit(func, "SR");
        // h1->Draw("same");
        auto mg1 = new TMultiGraph();
        mg1->Add(h1);
        TGraphErrors * h2 = (TGraphErrors*)sim->Get("spento");
        // for(int i=0;i<h2->GetN();i++){
        //     h2->SetPointError(i, 0,0);
        // }
        // h2->MovePoints(0, -0.3113);
        h2->SetLineStyle(10);
        h2->SetMarkerColor(3);
        h2->SetMarkerStyle(21);
        h2->GetXaxis()->SetLimits(0,7);
        // h2->Draw("same");
        mg1->Add(h2);
        sim->Close();
        cnv->cd(2);
        mg1->SetTitle("Simulazione");
        mg1->Draw("ACP");

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

        double x2_axis[(int)bin];
        double y2_axis[(int)bin];
        double x2_error[(int)bin];
        double y2_error[(int)bin];
        for(int i=1;i<bin+1;i++){
            x2_axis[i] = h_up->GetBinCenter(i);
            x2_error[i] = (11-0)/(bin*sqrt(12));
            up = h_up->GetBinContent(i);
            down = h_down->GetBinContent(i);
            if (up+down!=0){
                y2_axis[i] = (up-down)/(up+down);
                y2_error[i] = 2*sqrt(up*down/pow(up+down,3));
            }
            else y2_axis[i] = 0;
        }

        TGraph * g1 = new TGraphErrors(bin, x2_axis, y2_axis, x2_error, y2_error);
        g1->SetLineColor(3);
        g1->SetMarkerColor(3);
        g1->SetMarkerSize(1);
        g1->SetMarkerStyle(20);
        // g1->Draw("same");
        mg->Add(g1);
        cnv->cd(1);
        mg->SetTitle("Esperimento");
        mg->Draw("ACP");

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




    auto legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->SetHeader("Legenda","C"); // option "C" allows to center the header
   legend->AddEntry(g,"Magnetico Acceso","lep");
   legend->AddEntry(g1,"Magnetico Spento","lep");
   legend->Draw();
   cnv->Modified();
   cnv->Update();
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

        myApp->Run();
        return 0;
}
