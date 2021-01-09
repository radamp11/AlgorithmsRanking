#include "Individual.hpp"
#include "ackley.hpp"
#include <iostream>


void Individual::evaluate() {
	const double ePlus20 = 22.7182818;  //max Ackley
	double ackley_result;
	ackley_result = ackley_func(_genes, _genes.size());
	_fitnessVal = 1 - ackley_result / ePlus20;
}
