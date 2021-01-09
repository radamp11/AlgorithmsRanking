#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>
#include "ackley.hpp"
#include "Individual.hpp"
#include "algorithm.hpp"


Algorithm::Algorithm(const int dim, const int numOfIndi, const int numOfGenerations, const int selOpt) {
    _dim = dim;
    _numOfIndi = numOfIndi;
    _numOfGenerations = numOfGenerations;
    _selOption = selOpt;
}


std::vector<Individual> Algorithm::initPopulation(int size, unsigned dimension){
    std::vector<Individual> newPop;
    for (int i = 0; i < size; ++i) {
        double const lower_bound = -32.768; //value taken from website
        double const upper_bound = 32.768;
        unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count(); //generate a seed
        std::uniform_real_distribution<double> distri(lower_bound, upper_bound);
        std::default_random_engine engine(seed);
        std::vector<double> chromosomes;

        for (unsigned j = 0; j < dimension; ++j) {
            double x = distri(engine);
            chromosomes.push_back(x);
        }
        Individual newIndi(chromosomes);
        newPop.push_back(newIndi);
    }
    return newPop;
}


//2 point crossover

void Algorithm::cross(Individual& first, Individual& second) {
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::uniform_int_distribution<int> distri(0, _dim - 1);
    std::default_random_engine engine(seed);

    int crossPoint1 = distri(engine);
    int crossPoint2 = distri(engine);
    
    if (crossPoint1 > crossPoint2)
        std::swap(crossPoint1, crossPoint2);

    for (int i = 0; i < _dim; ++i) {
        if (!(i >= crossPoint1 && i <= crossPoint2))
            std::swap(first.getGenes()->at(i), second.getGenes()->at(i));
    }
}


void Algorithm::crossover(std::vector<Individual>& Population) {
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::uniform_int_distribution<int> distri(0, _numOfIndi - 1);
    std::default_random_engine engine(seed);

    int idx1;
    int idx2;

    for (unsigned i = 0; i < Population.size() / 2; ++i) {
        idx1 = distri(engine);
        idx2 = distri(engine);
        cross(Population[idx1], Population[idx2]);
    }
}


void Algorithm::mutate(double& x) {
    const double MUTATION_RANGE = 5;
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::normal_distribution<double> distri(0, MUTATION_RANGE);
    std::default_random_engine engine(seed);

    x += distri(engine);
}


void Algorithm::mutation(std::vector<Individual>& Population) {
    const double PROBABILITY_OF_CHOICE = 0.2;
    const double PROBABILITY_OF_MUTATION = 0.2;
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::uniform_real_distribution<double> distri(0, 1);
    std::default_random_engine engine(seed);

    for (unsigned i = 0; i < Population.size(); ++i) {
        double rand = distri(engine);
        if (rand <= PROBABILITY_OF_CHOICE) {
            std::vector<double>* chromosomes = Population[i].getGenes();
            for (unsigned j = 0; j < chromosomes->size(); ++j) {
                rand = distri(engine);
                if (rand <= PROBABILITY_OF_MUTATION)
                    mutate(chromosomes->at(j));
            }
        }
    }
}

/* roulettle selection */

std::vector<Individual> Algorithm::rouletteSelection(std::vector<Individual>& Population) {
    double sum = 0;
    std::vector<Individual> newPop;

    for (Individual indi : Population)
        sum += indi.getFitness();
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::uniform_real_distribution<double> distri(0, sum);
    std::default_random_engine engine(seed);
    for (unsigned i = 0; i < Population.size(); ++i) {
        double rand = distri(engine);
        double tempSum = 0;
        bool lookForRand = true;
        for (unsigned j = 0; lookForRand && j < Population.size(); ++j) {
            tempSum += Population[j].getFitness();
            if (tempSum > rand) {
                newPop.push_back(Population[j]);
                lookForRand = false;
            }
        }
    }
    return newPop;
}

/* tournament selection */

std::vector<Individual> Algorithm::initTournament(const unsigned tourSize, const std::vector<Individual>& Population) {
    std::vector<Individual> newTour;
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::uniform_int_distribution<int> distri(0, Population.size() - 1);
    std::default_random_engine engine(seed);
    
    for (unsigned i = 0; i < tourSize; ++i) {
        newTour.push_back(Population[distri(engine)]);
    }
    
    return newTour;
}


Individual Algorithm::chooseWinner(const unsigned tourSize, std::vector<Individual>& tournament) {
    Individual winner(tournament[0]);

    for (unsigned i = 1; i < tourSize; ++i)
        if (tournament[i].getFitness() > winner.getFitness())
            winner = tournament[i];

    return winner;
}


std::vector<Individual> Algorithm::tournamentSel(const unsigned tournamentSize, std::vector<Individual>& Population) {
    int numOfTournaments = Population.size();
    std::vector<Individual> newPop;
    std::vector<Individual> tournament;

    for (int i = 0; i < numOfTournaments; ++i) {
        tournament = initTournament(tournamentSize, Population);
        newPop.push_back(chooseWinner(tournamentSize, tournament));
    }

    return newPop;
}

/** best fraction selection 
  * returns population filled with the best 100/breaks percent individuals
  */

std::vector<Individual> Algorithm::bestFractionSel(std::vector<Individual>& Population, const int breaks) {
    std::vector<Individual> newPop;
    int range = Population.size()/breaks;
    std::sort(Population.begin(), Population.end());
    std::reverse(Population.begin(), Population.end());

    int i = 0;
    for (; i < breaks; ++i) {
        for( int j = 0; j < range; ++j)
            newPop.push_back(Population[j]);
    }
    i = 0;
    while (newPop.size() != Population.size()) {
        newPop.push_back(Population[i]);
        ++i;
    }
    
    return newPop;
}

double Algorithm::getBestFit(std::vector<Individual>& Population) {
    double bestFit = 0;
    for (unsigned i = 0; i < Population.size(); ++i) {
        if (Population[i].getFitness() > bestFit) {
            bestFit = Population[i].getFitness();
        }
    }
    return bestFit;
}

double Algorithm::getWorstFit(std::vector<Individual>& Population) {
    double worstFit = 1;
    for (unsigned i = 0; i < Population.size(); ++i) {
        if (Population[i].getFitness() < worstFit) {
            worstFit = Population[i].getFitness();
        }
    }
    return worstFit;
}

std::vector<Individual> Algorithm::runAlgorithm(std::ofstream& outfile, int loopNumber){
    const unsigned TOUR_SIZE = 2;
    const int BREAKS_NUM = 2;

    std::vector<Individual> Population, newPop;
    Population = Algorithm::initPopulation(_numOfIndi, _dim);       // inicjalizacja


    for (unsigned i = 0; i < Population.size(); ++i) {              // ocena
        Population[i].evaluate();
    }

    outfile << loopNumber << ": " << getBestFit(Population) << " ";   //wypisanie do pliku pierwszego bestFita

    for (int i = 0; i < _numOfGenerations; ++i) {              // selekcja
        switch (_selOption) {
            case 1:
                newPop = rouletteSelection(Population);
                break;
            case 2:
                newPop = tournamentSel(TOUR_SIZE, Population);
                break;
            case 3:
                newPop = bestFractionSel(Population, BREAKS_NUM);
                break;
            default:
                std::cerr << "Invalid data!" << std::endl;
                break;
        }
        crossover(newPop);                                          // krzyzowanie
        mutation(newPop);                                           // mutacja
        for (unsigned i = 0; i < newPop.size(); ++i) {              // ocena
            newPop[i].evaluate();
        }
        Population = newPop;                                        // sukcesja
        outfile << getBestFit(Population) << " ";
    }
    outfile << "\n";
    return Population;
}