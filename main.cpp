#include <iostream>
#include <array>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include<chrono>
#include <stdio.h> 
#include "CONSTRAINTS.h"
#include "Population.h"

using namespace std;

typedef std::chrono::high_resolution_clock Clock;

static Supervisor supervisors[47];		//fixed info for supervisors(id, unavaliable slots, consecutive,days, and same venue preference)
static Presentation presentations[118];	//fixed info for presentations(id, supervisors)
static int hc02, hc03, hc04, sc01, sc02, sc03;

extern int unavailable_slots[];

void initialise();

int main(){
	srand(time(0));
	initialise();
					
	int popSize ;
	int genLimit;
	
	cout << "Please enter a population size(recommended: 20) : ";
	cin >> popSize;
	cout << endl;
	cout << "Please enter on which generation should the program terminates(recommended: 12000) : ";
	cin >> genLimit;
	
	system("CLS");
	
	auto start_time = Clock::now();
	
	Population population(popSize);		//initialise population

	population.calcPopPenalty(false, presentations, supervisors, hc02, hc03, hc04, sc01, sc02, sc03);		//calculate penalty for whole population
	
	long least = population.leastPenalty();
	long prevPenalty = 0;
	int generation = 0;
	int mutationRate = 1;
	int count = 0;
	
	cout << "Generation: " << generation << "   " << "Least Penalty: " << least << endl;
	
	while(least>0 && generation<genLimit && count<500){		//genetic algorithm
		if(least < 1000 && count <= 10)				//all hard constraints have been satisfied
			mutationRate = 1;			//exploit current solution space
		
		else{							//fittest solution remain same for 10 consecutive generations
			mutationRate = 10 + (count/10);		//increase the mutation rate to explore out of current solution space
			if(mutationRate > 100)
				mutationRate = 100;			
		}
		
		population.reproduce(mutationRate);		
		population.calcPopPenalty(false, presentations, supervisors, hc02, hc03, hc04, sc01, sc02, sc03);
		population.eliminate();
		least = population.leastPenalty();
		if(least==prevPenalty)
			count++;
		else
			count = 0;
		prevPenalty = least;
		generation++;
		cout << "Generation: " << generation << "   Least Penalty: " << least << "   Mutation rate: " << mutationRate <<
				"   Count(same fittest solution): " << count << endl;
	} 
	
	while(least>0 && generation<genLimit){				//local search
		population.localSearch();
		population.calcPopPenalty(false, presentations, supervisors, hc02, hc03, hc04, sc01, sc02, sc03);
		population.eliminate();
		least = population.leastPenalty();
		if(least==prevPenalty)
			count++;
		else
			count = 0;
		prevPenalty = least;
		generation++;
		cout << "Generation(Using Local Search): " << generation << "   Least Penalty: " << least <<
				"   Count(same fittest solution): " << count << endl;
	}

	auto end_time = Clock::now();	
	
	cout << endl << endl;
	
	Individual fittestIndi = population.getFittestIndividual();
	
	fittestIndi.calcPenalty(true, presentations, supervisors, hc02, hc03, hc04, sc01, sc02, sc03);
	
	cout << endl;
	cout << "Runtime: " << std::chrono::duration_cast<std::chrono::seconds>
			(end_time - start_time).count() << " seconds" << endl;
	cout << "Population size: " << popSize << endl << endl;
	cout << "Violations:" << endl;
	cout << "HC01: 0" << endl;
	cout << "HC02: " << hc02 << endl; 
	cout << "HC03: " << hc03 << endl;
	cout << "HC04: " << hc04 << endl;
	cout << "SC01: " << sc01 << endl;
	cout << "SC02: " << sc02 << endl;
	cout << "SC03: " << sc03 << endl << endl;
	
	fstream fout;
	
	fout.open("schedule.csv", ios::out | ios::trunc);
	
	int *chromosome = fittestIndi.getChromosomePointer();
	
	bool assigned;
	
	for(int slot=1; slot<301; slot++){
		assigned = false;
		for(int i=0; i<118; i++){
			if(slot==*(chromosome+i)){
				assigned = true;
				Supervisor* sup = presentations[i].getSupervisors();
				fout << presentations[i].getID(); //<< " " << sup[0].getID() << " " << sup[1].getID() << " " << sup[2].getID();
				break;	
			}	
		}
		if(!assigned){
			fout << "null";
		}
		if(slot==15||slot==30||slot==45||slot==60||slot==75||slot==90||slot==105||slot==120||slot==135||slot==150||
		slot==165||slot==180||slot==195||slot==210||slot==225||slot==240||slot==255||slot==270||slot==285||slot==300)
			fout << "\n";
		else
			fout << ",";
	}
	
	fout.close();
	
	cout << "Schedule with the least penalty has been exported to schedule.csv!" << endl << endl;
	
	system("pause");
	
}

void initialise(){
	
	ifstream ip("SC03.csv");
	string name, pref;
	
	if(!ip.is_open())
		cout << "File not found" << endl;
	
	int it = 0;
	
	while(!ip.eof()){
		getline(ip, name, ',');
		getline(ip, pref, '\n');
		
		if(it==sizeof(supervisors)/sizeof(supervisors[0]))
			break;
		
		supervisors[it].setID(name);
		if(pref=="yes")
			supervisors[it].setVenuePref(true);

		it++;
	}
	
	ip.close();
	
	ifstream ip2("HC04.csv");
	
	if(!ip2.is_open())
		cout << "File not found" << endl;
		
	vector<string> row; 
    string line, word;	
    it = 0;
		
	while(getline(ip2, line)){
		row.clear();
		
		stringstream s(line);
		
		while(getline(s, word, ',')){
			row.push_back(word);
		}
		
		if(it==sizeof(supervisors)/sizeof(supervisors[0]))
			break;
			
		row.erase(row.begin());
		while(!row.empty()){
				supervisors[it].setUnavaliability(stoi(row.front()));
				row.erase(row.begin());
			}
		
		it++;
	}
	
	ip2.close();
	
	ifstream ip4("SC01.csv");
	
	if(!ip4.is_open())
		cout << "File not found" << endl;
	
	string consec;
	it = 0;
	
	while(!ip4.eof()){
		getline(ip4, name, ',');
		getline(ip4, consec, '\n');
		
		if(it==sizeof(supervisors)/sizeof(supervisors[0]))
			break;
		
		supervisors[it].setConsecPref(stoi(consec));
		it++;		
	}
	
	ip4.close();
	
	ifstream ip5("SC02.csv");
	
	if(!ip5.is_open())
		cout << "File not found" << endl;
		
	string days;
	it = 0;
	
	while(!ip5.eof()){
		getline(ip5, name, ',');
		getline(ip5, days, '\n');
		
		if(it==sizeof(supervisors)/sizeof(supervisors[0]))
			break;
		
		supervisors[it].setDaysPref(stoi(days));
		it++;
	}
	
	ip5.close();
	
	ifstream ip3("SupExaAssign.csv");
	
	if(!ip3.is_open())
		cout << "File not found" << endl;
		
	string id, name1, name2, name3;
	Supervisor sup1, sup2, sup3;
	it = 0;
	
	getline(ip3,id);
	
	while(!ip3.eof()){
		getline(ip3, id, ',');
		getline(ip3, name1, ',');
		getline(ip3, name2, ',');
		getline(ip3, name3, '\n');
		
		for(int i=0; i<47; i++){
			if(supervisors[i].getID()==name1)
				sup1 = supervisors[i];
			else if(supervisors[i].getID()==name2)
				sup2 = supervisors[i];
			else if(supervisors[i].getID()==name3)
				sup3 = supervisors[i];
		}
		
		if(it==sizeof(presentations)/sizeof(presentations[0]))
			break;
		
		presentations[it].setID(id);
		presentations[it].setSupervisors(sup1, sup2, sup3);
		
		it++;
	}
	
	ip3.close();
	
	ifstream ip6("HC03.csv");
	
	if(!ip6.is_open())
		cout << "File not found" << endl;
			
    it = 0;
		
	while(getline(ip6, line)){
		row.clear();
		
		stringstream s(line);
		
		while(getline(s, word, ',')){
			row.push_back(word);
		}
		
		if(it==sizeof(unavailable_slots)/sizeof(unavailable_slots[0]))
			break;
			
		row.erase(row.begin());
		while(!row.empty()){
				unavailable_slots[it] = stoi(row.front());
				row.erase(row.begin());
				it++;
			}
	}
	
	ip6.close();
}






