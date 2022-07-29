//
// Created by daniel on 25/07/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_HISTOGRAM_H
#define ECONOMICSMICROFOUNDATIONS_HISTOGRAM_H


#include <vector>
#include <ostream>

class Histogram {
public:
    double start;
    double step;
    std::vector<std::pair<double,int>> data;
//    std::vector<int> counts;

    Histogram(double start, double end, double step): start(start), step(step) {
        data.reserve((end-start)/step);
        for(double x=start+step/2.0; x<end; x += step) {
            data.emplace_back(x,0);
        }
    }

    bool push(double dataPoint) {
        int i = (dataPoint-start)/step;
        if(i>=0 && i < data.size()) {
            ++(data[(dataPoint-start)/step].second);
            return true;
        }
        return false;
    }

    friend std::ostream &operator <<(std::ostream &out, const Histogram &histogram) {
        for(const auto &point: histogram.data) out << point.first << " " << point.second << std::endl;
        return out;
    }
};


#endif //ECONOMICSMICROFOUNDATIONS_HISTOGRAM_H
