#ifndef _INDIVIDUAL_HPP
#define _INDIVIDUAL_HPP
#include <iostream>
#include <vector>

class Individual{
public:
    Individual(const std::vector<double> newChromosomes) : _fitnessVal(-1.0), _genes(newChromosomes) {}
    Individual(const Individual& ind) : _fitnessVal(ind._fitnessVal), _genes(ind._genes) {}
    std::vector<double>* getGenes() { return &_genes; }
    double getFitness() { return _fitnessVal; }
    void evaluate();
    bool operator<(const Individual& other) { return (this->_fitnessVal < other._fitnessVal); }
    ~Individual() {}

private:
    double _fitnessVal;
    std::vector<double> _genes;   // genes (coordinates); number of elems = dimension
};

#endif  // _INDIVIDUAL_HPP