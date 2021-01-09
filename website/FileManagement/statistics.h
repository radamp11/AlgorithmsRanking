#ifndef STATISTICS_H
#define STATISTICS_H

#include <vector>

struct Components{
    double best_;
    double worst_;
    double median_;
    double mean_;
    double std_;
};

class Statistics{
public:
    void calculateStatistics();
    void insertValue( double value );
    std::vector<double> getValues();
    void cleanValues();
    Components* getComponents();
private:
    std::vector<double> values;
    Components components;
};

#endif