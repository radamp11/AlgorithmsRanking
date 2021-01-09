#include <algorithm>
#include <cmath>
#include "statistics.h"

using namespace std;

void Statistics::calculateStatistics(){
    double sum = 0;
    double cur_value;
    int size = values.size();

    sort( values.begin(), values.end() );
    components.best_ = values.front();
    components.worst_ = values.back();

    if( size%2 == 0 )
        components.median_ = (( values[size/2] + values[size/2 - 1] ) / 2 );
    else
        components.median_  = values[size/2];

    for( int i = 0; i < size; ++i ){
        sum += values[i];
    }
    components.mean_ = sum / size;

    sum = 0;
    for( int i = 0; i < size; ++i ){
        sum += (( values[i] - components.mean_)*( values[i] - components.mean_ ));
    }
    components.std_ = sqrt( sum / size );

}

void Statistics::cleanValues(){
    values.clear();
}

void Statistics::insertValue( double value ){
    values.push_back( value );
}

Components* Statistics::getComponents(){
    return &components;
}

std::vector<double> Statistics::getValues(){
    return values;
}