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
    std::string title;

    Histogram(double start, double end, double binWidth, std::string title): start(start), binWidth(binWidth), title(std::move(title)) {
        int nBins = std::ceil((end-start)/binWidth);
        initialiseFrequencyVector(nBins);
    }
    
    template<class T>
    Histogram(const std::vector<T> &data, int nBins, std::string title): title(std::move(title)) {
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
            if(binWidth == 0.0) {
                binWidth = 1.0;
                start = min - 0.5 - nBins/2;
            }
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


    void plot() {
        Gnuplot gp;
        gp << "plot '-' with histeps title '" << title << "'" << std::endl;
        gp.send1d(frequency);
    }

    friend std::ostream &operator <<(std::ostream &out, const Histogram &histogram) {
        for(const auto &point: histogram.frequency) out << point.first << " " << point.second << std::endl;
        return out;
    }

    friend Gnuplot &operator <<(Gnuplot &gp, const Histogram &histogram) {
        gp << "plot '-' with histeps title '" << histogram.title << "'" << std::endl;
        gp.send1d(histogram.frequency);
        return gp;
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
