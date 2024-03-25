#include <iostream>
#include <fstream>
#include "Mela.h"
#include "TVar.hh"
#include "TCouplingsBase.hh"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

using namespace std;

TLorentzVector ptEtaPhiVector(double pt, double eta, double phi, double m){ //constructs pt, eta, phi TLorentzVector
    TLorentzVector vec = TLorentzVector();
    vec.SetPtEtaPhiM(pt, eta, phi, m);
    return vec;
}

int main(int argc, char const *argv[])
{
    Mela m = Mela(13, 125, TVar::SILENT);

    TFile* dataFile = TFile::Open("../SAMPLE_DATA/EWSample_4l.root");
    // TFile* dataFile = TFile::Open("../SAMPLE_DATA/ggHSample_4l.root");
    // TFile* dataFile = TFile::Open("../SAMPLE_DATA/POWHEGSample_mH_125.root");

    TTreeReader myReader("eventTree", dataFile);

    TTreeReaderValue<vector<short>> LHEDaughterId(myReader,   "LHEDaughterId");
    TTreeReaderValue<vector<float>> LHEDaughterPt(myReader,   "LHEDaughterPt");
    TTreeReaderValue<vector<float>> LHEDaughterEta(myReader,  "LHEDaughterEta");
    TTreeReaderValue<vector<float>> LHEDaughterPhi(myReader,  "LHEDaughterPhi");
    TTreeReaderValue<vector<float>> LHEDaughterMass(myReader, "LHEDaughterMass");
    
    TTreeReaderValue<vector<short>> LHEAssociatedParticleId(myReader,   "LHEAssociatedParticleId");
    TTreeReaderValue<vector<float>> LHEAssociatedParticlePt(myReader,   "LHEAssociatedParticlePt");
    TTreeReaderValue<vector<float>> LHEAssociatedParticleEta(myReader,  "LHEAssociatedParticleEta");
    TTreeReaderValue<vector<float>> LHEAssociatedParticlePhi(myReader,  "LHEAssociatedParticlePhi");
    TTreeReaderValue<vector<float>> LHEAssociatedParticleMass(myReader, "LHEAssociatedParticleMass");
    
    TTreeReaderValue<vector<short>> LHEMotherId(myReader, "LHEMotherId");
    TTreeReaderValue<vector<float>> LHEMotherPz(myReader, "LHEMotherPz");
    TTreeReaderValue<vector<float>> LHEMotherE(myReader,  "LHEMotherE");

    vector<float> probs;
    ofstream outputFile;
    outputFile.open("probs_output.txt");

    while (myReader.Next()){

        vector<TLorentzVector> motherVecs;
        vector<TLorentzVector> daughterVecs;
        vector<TLorentzVector> associatedVecs;

        int i = 0;
        for(i = 0; i < (*LHEMotherId).size(); i++){
            motherVecs.push_back(
                TLorentzVector(0, 0, (*LHEMotherPz)[i], (*LHEMotherE)[i])
            );
        }

        for(i = 0; i < (*LHEDaughterId).size(); i++){
            daughterVecs.push_back(
                ptEtaPhiVector((*LHEDaughterPt)[i], (*LHEDaughterEta)[i], (*LHEDaughterPhi)[i], (*LHEDaughterMass)[i])
            );
        }

        for(i = 0; i < (*LHEAssociatedParticleId).size(); i++){
            associatedVecs.push_back(
                ptEtaPhiVector((*LHEAssociatedParticlePt)[i], (*LHEAssociatedParticleEta)[i], (*LHEAssociatedParticlePhi)[i], (*LHEAssociatedParticleMass)[i])
            );
        }

        /*SET UP MELA ATTRIBUTES HERE!*/
        float prob = 0;
        m.computeP(prob, false);
        probs.push_back(prob);

        outputFile << prob << "\n";
    }

    outputFile.close();
    dataFile->Close();

    return 0;
}
