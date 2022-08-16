//
// Created by daniel on 25/07/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_HISTOGRAM_H
#define ECONOMICSMICROFOUNDATIONS_HISTOGRAM_H


#include <vector>
#include <ostream>
#include "gnuplot-iostream/gnuplot-iostream.h"

class Histogram {
public:
    double start; // lowest value of first bin
    double binWidth;  // width of bin
    std::vector<std::pair<double,int>> frequency; // <bin-midpoint, frequency>

    Histogram(double start, double end, double binWidth): start(start), binWidth(binWidth) {
        int nBins = (end-start)/binWidth - 0.5;
        initialiseFrequencyVector(nBins);
    }
    
    template<class T>
    Histogram(const std::vector<T> &data, int nBins) {
        if(data.size() != 0) {
            double min = data[0];
            double max = data[0];
            for (int i = 1; i < data.size(); ++i) {
                const double & point = data[i]; 
                if(point < min) min = point;
                if(point > max) max = point;
            }
            double dX = ((max-min)/nBins)*1e-4;
            binWidth = ((max - min) + 2.0 * dX) / nBins;
            start = min - dX;
            initialiseFrequencyVector(nBins);
            for(const T &point: data) push(point);
        }
    }


    bool push(double dataPoint, int multiplicity = 1) {
        int i = (dataPoint-start) / binWidth;
        if(i>=0 && i < frequency.size()) {
            frequency[i].second += multiplicity;
            return true;
        }
        return false;
    }


    void plot(std::string title) {
        Gnuplot gp;
        gp << "plot '-' with histeps title '" << title << "'" << std::endl;
        gp.send1d(frequency);
    }

    friend std::ostream &operator <<(std::ostream &out, const Histogram &histogram) {
        for(const auto &point: histogram.frequency) out << point.first << " " << point.second << std::endl;
        return out;
    }

protected:

    void initialiseFrequencyVector(int nBins) {
        frequency.reserve(nBins);
        double end = start + nBins * binWidth;
        for(double x = start + binWidth/2.0; x < end; x += binWidth) {
            frequency.emplace_back(x, 0);
        }
    }
};


#endif //ECONOMICSMICROFOUNDATIONS_HISTOGRAM_H
