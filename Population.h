#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "Individual.h"

class Population{
	private:
		vector<Individual> individuals;
		int populationSize;
		
	public:
		Population(int size){
			populationSize = size;
			for(int i=0; i<size; i++){
				Individual indi;
				individuals.push_back(indi);
		 	}
		}
		void calcPopPenalty(bool print, Presentation presentations[], Supervisor supervisors[], int &hc02, int &hc03, int &hc04, int &sc01, int &sc02, int &sc03);
		void printAllIndividual();
		void printAllPenalty();
		vector<Individual> getIndividuals();
		void sortPopulation();
		long leastPenalty();
		void reproduce(int mutationRate);
		void localSearch();
		void eliminate();
		Individual getFittestIndividual();
};

void Population::calcPopPenalty(bool print, Presentation presentations[], Supervisor supervisors[], int &hc02, int &hc03, int &hc04, int &sc01, int &sc02, int &sc03){
	for(int i=0; i<individuals.size(); i++){
		individuals.at(i).calcPenalty(print, presentations, supervisors, hc02, hc03, hc04, sc01, sc02, sc03);
	}
	sortPopulation();
}

void Population::printAllIndividual(){
	for(int i=0; i<individuals.size(); i++){
		individuals.at(i).printChromosome();
	}
}

void Population::printAllPenalty(){
	for(int i=0; i<individuals.size(); i++){
		individuals.at(i).printPenalty();
	}
}

vector<Individual> Population::getIndividuals(){
	return individuals;
}


void Population::sortPopulation(){
	sort(individuals.begin(), individuals.end(), [](Individual const &a, Individual const &b){
		return a.getPenalty() < b.getPenalty();
	});
}

long Population::leastPenalty(){
	sortPopulation();
	return individuals.at(0).getPenalty();
}

void Population::localSearch(){
	int size = individuals.size();
	Individual mutateChromosome;
	for(int i=0; i<size; i++){
		mutateChromosome = individuals.at(i);
		mutateChromosome.localSearchMutatation();
		individuals.push_back(mutateChromosome);
	}
}

//void Population::reproduce(int mutationRate){			//rank based selection
//	int size, random, prob, probDeduct = 0;
//	Individual child1, child2;
//	size = individuals.size();
//	for(int i=0; i<size; i+=2){
//		probDeduct = (i/2)*100/(individuals.size()/2);
//		prob = 100 - probDeduct;
//		random = rand()%100;
//		if(random < prob){
//			child1 = individuals.at(i);
//			child2 = individuals.at(i+1);
//			child2 = child1.crossover(individuals.at(i+1), mutationRate);
//			individuals.push_back(child1);
//			individuals.push_back(child2);
//		}
//	}
//}

//void Population::reproduce(int mutationRate){			//roulette selection
//	vector<Individual> matingPool;
//	int totalPen = 0;
//	for(int i=0; i<individuals.size(); i++){
//		totalPen += individuals.at(i).getPen();
//	}
//	float prob;
//	for(int i=0; i<individuals.size(); i++){
//		prob = (float)(totalPen - individuals.at(i).getPen())/(float)totalPen;
//		for(int j=0; j<prob*10; j++)
//			matingPool.push_back(individuals.at(i));
//	}
//	int size, random;
//	Individual child1, child2;
//	size = individuals.size();
//	for(int i=0; i<size; i+=2){
//		random = rand()%matingPool.size();
//		child1 = matingPool.at(random);
//		random = rand()%matingPool.size();
//		child2 = matingPool.at(random);
//	
//		child2 = child1.crossover(child2, mutationRate);
//		individuals.push_back(child1);
//		individuals.push_back(child2);		
//	}
//	matingPool.clear();
//}

void Population::reproduce(int mutationRate){			//tournament selection
	int random, best, size;
	Individual child1, child2;
	size = individuals.size();
	for(int i=0; i<size; i+=2){
		best = 9999;
		for(int i=0; i<5; i++){
		random = rand()%individuals.size();
		if(random<best)
			best = random;
		}
		child1 = individuals.at(best);
		best = 9999;
		for(int i=0; i<5; i++){
			random = rand()%individuals.size();
			if(random<best)
				best = random;
		}
		child2 = individuals.at(best);
			
		child2 = child1.crossover(child2, mutationRate);
		individuals.push_back(child1);
		individuals.push_back(child2);
		
	}
}
	
void Population::eliminate(){
	individuals.resize(populationSize);
}	

Individual Population::getFittestIndividual(){
	return individuals.at(0);
}
	
	
	
	
