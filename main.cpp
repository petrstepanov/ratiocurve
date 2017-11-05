//
//  main.cpp
//  ratiocurve
//
//  Created by Petr Stepanov on 11/12/15.
//  Copyright © 2015 Petr Stepanov. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <math.h>

using namespace std;

int integrateMap(std::map<float,int> spectrum){
    int totalCounts = 0;
    std::map<float,int>::iterator it;
    for (it = spectrum.begin(); it != spectrum.end(); ++it){
        totalCounts += it->second;
    }
    return totalCounts;
}

std::map<float,double> normalizeMap(std::map<float,int> spectrum){
    int totalCounts = integrateMap(spectrum);
    std::map<float,double> normSpectrum;
    std::map<float,int>::iterator it;
    for (it = spectrum.begin(); it != spectrum.end(); ++it){
        normSpectrum[it->first] = (double)(it->second)/(double)totalCounts;
    }
    
    return normSpectrum;
}

std::map<float,int> importSpectrum(char* filename){
    ifstream inFile;
    inFile.open(filename);
    if (!inFile){
        cout << "File \"" << filename << "\" not found." << endl;
        exit(1);
    }
    
    std::map<float,int> spectrum;

    float energy;
    int count;
    string str;
    std::getline(inFile,str);
    while(!(inFile.eof())) {
        std::getline(inFile,str);
        stringstream ss (str);
        ss.precision(2);
        ss >> energy;
        if (ss.peek() == ',') ss.ignore();
        ss >> count;
        spectrum[round(energy*10)] = count;  // multiplying by 10 - hack :-)
    }
    return spectrum;
}

double relativeRatioError(double x, double y){
    if(y == 0) return 0;
    if(x == 0) return 0;
    double dx = sqrt(x);
    double dy = sqrt(y);
    double err = (x/y)*sqrt(pow((dx/x),2)+pow((dy/y),2));
    return err;
}

void makeRatioCurve (char* filenameSpec, char* filenameAlSpec, char* filenameOutput){
    std::map<float,int> spectrumAl = importSpectrum(filenameAlSpec);
    std::map<float,int> spectrum = importSpectrum(filenameSpec);

    int spectAlInt = integrateMap(spectrumAl);
    int spectInt = integrateMap(spectrum);

//    std::map<double,double> spectrumAlNorm = normalizeMap(spectrumAl);
//    std::map<double,double> spectrumNorm = normalizeMap(spectrum);
    
    // open output file
    ofstream file;
    file.open (filenameOutput);
    
    // iterate first spectrum
    std::map<float,int>::iterator itAl;
    for (itAl = spectrumAl.begin(); itAl != spectrumAl.end(); ++itAl){
        // get energy value and find same energy value in second spectrum
        std::map<float,int>::iterator it = spectrum.find(itAl->first);
        // if same energy found - divide counts
        if (itAl->second != 0 && it->second != 0 && it != spectrum.end()){
            // output to file energy, counts ratio and error
            double coefficient = (double)(spectAlInt)/(double)(spectInt);
            file << (it->first)/10 << ", " // dividing by 10 - hack :-)
                 << (it->first)/10.0 * 3.91 << ", "
                 << (((double)(it->second)/(double)(itAl->second))*coefficient) << ", "
                 << (coefficient * relativeRatioError(it->second, itAl->second)) << "\n";
        }
    }
    
    file.close();
}

// char filenameAlSpec[] = "/Users/petrstepanov/Development/workspace-xcode/ratiocurve/ratiocurve/Energy_1_sum_Al.txt";
// char filenameAlSpec[] = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/Al/2016-03-20-Al-Annealed-TLIST-FD-32768/2016-03-20-Al-Annealed-TLIST-FD-32768-doppler.txt";
char filenameAlSpec[] = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/Zn/2016-04-23-Zn-TLIST-FD-32768/2016-04-23-Zn-TLIST-FD-32768-2-doppler.txt";

char filenameAsGrown[]   = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/ZnO/2016-03-19-ZnO-As-Grown-TLIST-FD-32768/2016-03-19-ZnO-As-Grown-TLIST-FD-32768-2-doppler.txt";
char filenameAsGrownOut[] = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/ZnO/2016-03-19-ZnO-As-Grown-TLIST-FD-32768/2016-03-19-ZnO-As-Grown-TLIST-FD-32768-2-ratio-zn.txt";

char filenameHDoped[]   = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/ZnO/2016-04-05-ZnO#3-TLIST-FD-32768/2016-04-05-ZnO#3-TLIST-FD-32768-doppler.txt";
char filenameHDopedOut[] = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/ZnO/2016-04-05-ZnO#3-TLIST-FD-32768/2016-04-05-ZnO#3-TLIST-FD-32768-ratio-zn.txt";

char filenameHDoped2[]   = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/ZnO/2016-04-08-ZnO#2-TLIST-FD-32768/2016-04-08-ZnO#2-TLIST-FD-32768-2-doppler.txt";
char filenameHDoped2Out[] = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/ZnO/2016-04-08-ZnO#2-TLIST-FD-32768/2016-04-08-ZnO#2-TLIST-FD-32768-2-ratio-zn.txt";

char filenameODoped[]   = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/ZnO/2016-04-06-ZnO#6-TLIST-FD-32768/2016-04-06-ZnO#6-TLIST-FD-32768-doppler.txt";
char filenameODopedOut[] = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/ZnO/2016-04-06-ZnO#6-TLIST-FD-32768/2016-04-06-ZnO#6-TLIST-FD-32768-ratio-zn.txt";

char filenameDDoped[]   = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/ZnO/2016-04-07-ZnO#5-TLIST-FD-32768/2016-04-07-ZnO#5-TLIST-FD-32768-2-doppler.txt";
char filenameDDopedOut[] = "/Users/petrstepanov/Dropbox/SelimLab Public Folder/DopplerBroadening/Data/ZnO/2016-04-07-ZnO#5-TLIST-FD-32768/2016-04-07-ZnO#5-TLIST-FD-32768-2-ratio-zn.txt";


//char filenameSpecAsGrown[]   = "/Users/petrstepanov/Development/workspace-xcode/ratiocurve/ratiocurve/Energy_1_sum_AsGrown.txt";
//char filenameAsGrownOutput[] = "/Users/petrstepanov/Development/workspace-xcode/ratiocurve/ratiocurve/ratioCurve_As.txt";
//
//char filenameSpecD2Doped[]   = "/Users/petrstepanov/Development/workspace-xcode/ratiocurve/ratiocurve/Energy_1_sum_D2.txt";
//char filenameD2Output[] = "/Users/petrstepanov/Development/workspace-xcode/ratiocurve/ratiocurve/ratioCurve_D2.txt";

int main(void) {
    makeRatioCurve(filenameAsGrown, filenameAlSpec, filenameAsGrownOut);
    makeRatioCurve(filenameHDoped, filenameAlSpec, filenameHDopedOut);
    makeRatioCurve(filenameHDoped2, filenameAlSpec, filenameHDoped2Out);
    makeRatioCurve(filenameODoped, filenameAlSpec, filenameODopedOut);
    makeRatioCurve(filenameDDoped, filenameAlSpec, filenameDDopedOut);
}
