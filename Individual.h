#include <array>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "CONSTRAINTS.h"
#include "Presentation.h"

using namespace std;

extern  int unavailable_slots[];


class Individual{
	private:
		int chromosome[118];
		long penalty;
		
		
	public:
		Individual(){				//initializes random chromosome with undetermined fitness
			for(int i=0; i<118; i++){
				int slot = (rand() % 300) + 1;
				chromosome[i] = slot;
			}
			penalty = -1;
		}
		Individual(int arr[]){
			for(int i=0; i<118; i++)
				chromosome[i] = arr[i];
			penalty = -1;
		}
		void calcPenalty(bool print, Presentation presentations[], Supervisor supervisors[], int &hc02, int &hc03, int &hc04, int &sc01, int &sc02, int &sc03);
		void printChromosome();
		void setPenalty(int x);
		void printPenalty();
		long getPenalty() const;
		long getPen();
		Individual crossover(Individual parent, int mutationRate);
		void localSearchMutatation();
		int* getChromosomePointer();
		int isConsecutive(int sup, int currentSlot, Supervisor supervisors[]);
};

void Individual::setPenalty(int x){
	penalty = x;
}

void Individual::calcPenalty(bool print, Presentation presentations[], Supervisor supervisors[], int &hc02, int &hc03, int &hc04, int &sc01, int &sc02, int &sc03){
	
	hc02 = 0, hc03 = 0, hc04 = 0, sc01 = 0, sc02 = 0, sc03 = 0;
	penalty = 0;
	
	for(int i=0; i<47; i++){		//initialise schedule for all supervisors
		supervisors[i].schedule.clear();
	}
	
	for(int i=0; i<118; i++){	
		bool clashSup = false;
		int tempSlot = chromosome[i];
		int concurrentSlot1, concurrentSlot2, concurrentSlot3;
		
		if((tempSlot>=1 && tempSlot<=15)||(tempSlot>=61 && tempSlot<=75)||		//location is VIVA ROOM
		(tempSlot>=121 && tempSlot<=135)||(tempSlot>=181 && tempSlot<=195)||(tempSlot>=241 && tempSlot<=255)){
			concurrentSlot1 = tempSlot + 15;
			concurrentSlot2 = tempSlot + 30;
			concurrentSlot3 = tempSlot + 45;
		}
		
		else if((tempSlot>=16 && tempSlot<=30)||(tempSlot>=76 && tempSlot<=90)||	//location is MEETING ROOM
		(tempSlot>=136 && tempSlot<=150)||(tempSlot>=196 && tempSlot<=210)||(tempSlot>=256 && tempSlot<=270)){
			concurrentSlot1 = tempSlot - 15;
			concurrentSlot2 = tempSlot + 15;
			concurrentSlot3 = tempSlot + 30;
		}
			
		else if((tempSlot>=31 && tempSlot<=45)||(tempSlot>=91 && tempSlot<=105)||	//location is INTERACTION ROOM
		(tempSlot>=151 && tempSlot<=165)||(tempSlot>=211 && tempSlot<=225)||(tempSlot>=271 && tempSlot<=285)){
			concurrentSlot1 = tempSlot - 30;
			concurrentSlot2 = tempSlot - 15;
			concurrentSlot3 = tempSlot + 15;
		}
						
		else if((tempSlot>=46 && tempSlot<=60)||(tempSlot>=106 && tempSlot<=120)||	//location is BJIM
		(tempSlot>=166 && tempSlot<=180)||(tempSlot>=226 && tempSlot<=240)||(tempSlot>=286 && tempSlot<=300)){
			concurrentSlot1 = tempSlot - 45;
			concurrentSlot2 = tempSlot - 30;
			concurrentSlot3 = tempSlot - 15;
		}
		
		for(int t=0; t<118; t++){
			if(t==i)
				continue;
			else{		
							//HC02: checks if supervisor attends presentations concurrently
				if(chromosome[t]==concurrentSlot1||chromosome[t]==concurrentSlot2||chromosome[t]==concurrentSlot3){
					Supervisor* sup = presentations[i].getSupervisors();
					Supervisor* concurrentSup = presentations[t].getSupervisors();
					for(int j1=0; j1<3; j1++){
						for(int j2=0; j2<3; j2++){
							if(sup[j1].getID()==concurrentSup[j2].getID()){
								hc02++;
								if(print==true){
									cout << "HC02 Violation " << hc02 << endl;
									cout << "Slot= " << chromosome[t] << " " << chromosome[i] << endl;
									cout << "Presentations = " << presentations[t].getID() << " " << presentations[i].getID() << endl << endl;
								}
								clashSup = true;	
								penalty += 1000;
								break;
							}
						}
						if(clashSup==true)
							break;
					}
				}
				
				if(chromosome[i]==chromosome[t]){		//checks if 2 presentations have same slot
					penalty += 1000;
					if(print==true)
						cout << "Presentations having same slot= " << presentations[i].getID() << " " <<  presentations[t].getID() << endl << endl;
				}
			}				
		}
		
		
		for(int x=0; x<67; x++){		//HC03: checks for unavailable slots
			if(chromosome[i]==unavailable_slots[x]){
				hc03++;
				if(print==true){
					cout << "HC03 Violation " << hc03 << endl;
					cout << "Unavailable slots= " << chromosome[i] << "Presentation= " << presentations[i].getID() << endl <<endl;
				}	
				penalty += 1000;
				break;
			}
		}
		
		Supervisor* sup = presentations[i].getSupervisors();		
		
		for(int n=0; n<47; n++){		//assign supervisors' schedule
			if(supervisors[n].getID()==sup[0].getID()||supervisors[n].getID()==sup[1].getID()||supervisors[n].getID()==sup[2].getID()){
				supervisors[n].schedule.push_back(chromosome[i]);
			}
		}
		
		vector<int> sup1_Unavailable = sup[0].getUnavailable();		//HC04: checks if supervisor is available
		vector<int> sup2_Unavailable = sup[1].getUnavailable();
		vector<int> sup3_Unavailable = sup[2].getUnavailable();
		
		for(auto it=sup1_Unavailable.begin(); it!=sup1_Unavailable.end(); ++it){
			if(*it==chromosome[i]){
				hc04++;
				if(print==true){
					cout << "HC04 Violation " << hc04 << endl;
					cout << "Supervisor = " << sup[0].getID() << " at slot " << chromosome[i] << " for " << presentations[i].getID() <<  endl << endl;
				}
				penalty += 1000;
				break;
			}
		}
		
		for(auto it=sup2_Unavailable.begin(); it!=sup2_Unavailable.end(); ++it){
			if(*it==chromosome[i]){
				hc04++;
				if(print==true){
					cout << "HC04 Violation " << hc04 << endl;
					cout << "Supervisor = " << sup[0].getID() << " at slot " << chromosome[i] << " for " << presentations[i].getID() <<  endl << endl;
				}
				penalty += 1000;
				break;
			}
		}
		
		for(auto it=sup3_Unavailable.begin(); it!=sup3_Unavailable.end(); ++it){
			if(*it==chromosome[i]){
				hc04++;
				if(print==true){
					cout << "HC04 Violation " << hc04 << endl;
					cout << "Supervisor = " << sup[0].getID() << " at slot " << chromosome[i] << " for " << presentations[i].getID() <<  endl << endl;
				}
				penalty += 1000;
				break;
			}
		}		
	}
	
	for(int k=0; k<47; k++){
		int count = 0;
		int workDays = 0;
		int prev = 0;
		bool mon = false, tues = false, wed = false, thurs = false, fri = false;
		supervisors[k].sortSchedule();
		for(auto it=supervisors[k].schedule.begin(); it!=supervisors[k].schedule.end(); ++it){
			
			if(*it>=1 && *it<=60)
				mon = true;
			else if(*it>=61 && *it<=120)
				tues = true;
			else if(*it>=121 && *it<=180)
				wed = true;
			else if(*it>=181 && *it<=240)
				thurs = true;
			else if(*it>=241 && *it<=300)
				fri = true;


																
			prev = isConsecutive(k,*it, supervisors);	//find any consecutive slot									
																	//if presentations are consecutive
			if(prev!=-1 && ((prev>=1 && prev<=60)&&(*it>=1 && *it<=60)||(prev>=61 && prev<=120)&&(*it>=61 && *it<=120)||
			(prev>=121 && prev<=180)&&(*it>=121 && *it<=180)||(prev>=181 && prev<=240)&&(*it>=181 && *it<=240)
			||(prev>=241 && prev<=300)&&(*it>=241 && *it<=300))){
				if(supervisors[k].getVenuePref()==true){	//if supervisor prefer to have consecutive presentations at same venue
		
					if(((prev>=1 && prev<=15)||(prev>=61 && prev<=75)||		//prev is VIVA ROOM
					(prev>=121 && prev<=135)||(prev>=181 && prev<=195)||(prev>=241 && prev<=255)) && 
					((*it>=1 && *it<=15)||(*it>=61 && *it<=75)||		//current location is VIVA ROOM
					(*it>=121 && *it<=135)||(*it>=181 && *it<=195)||(*it>=241 && *it<=255))){}
				
					else if(((prev>=16 && prev<=30)||(prev>=76 && prev<=90)||		//prev is MEETING ROOM
					(prev>=136 && prev<=150)||(prev>=196 && prev<=210)||(prev>=256 && prev<=270)) && 
					((*it>=16 && *it<=30)||(*it>=76 && *it<=90)||		//current location is MEETING ROOM
					(*it>=136 && *it<=150)||(*it>=196 && *it<=210)||(*it>=256 && *it<=270))){}
				
					else if(((prev>=31 && prev<=45)||(prev>=91 && prev<=105)||		//prev is INTERACTION ROOM
					(prev>=151 && prev<=165)||(prev>=211 && prev<=225)||(prev>=271 && prev<=285)) && 
					((*it>=31 && *it<=45)||(*it>=91 && *it<=105)||		//current location is INTERACTION ROOM
					(*it>=151 && *it<=165)||(*it>=211 && *it<=225)||(*it>=271 && *it<=285))){}
				
					else if(((prev>=46 && prev<=60)||(prev>=106 && prev<=120)||		//prev is BJIM
					(prev>=166 && prev<=180)||(prev>=226 && prev<=240)||(prev>=286 && prev<=300)) && 
					((*it>=46 && *it<=60)||(*it>=106 && *it<=120)||		//current location is BJIM
					(*it>=166 && *it<=180)||(*it>=226 && *it<=240)||(*it>=286 && *it<=300))){}
					
					else{			//room is not same for consecutive presentations
						sc03++;
						if(print==true){
							cout << "SC03 Violation " << sc03 << endl;
							cout << "Supervisor: " << supervisors[k].getID() << " at slot " << prev <<"  and slot "<< *it << endl << endl;
						}	
						penalty+=10;
					}
				}
				count = 0;
				count++;
				for(int i=0; i<supervisors[k].getConsecPref()+1; i++){
				//	if(supervisors[k].getID()=="S005"&&i==0){cout<<"here s005; "<<endl;cout<<"current; "<<*it<<endl;}
				//	if(supervisors[k].getID()=="S005"){cout<<"prev; "<<prev<<"  count: "<<count<<endl;
				//	}
					if(prev==-1)
						break;
					else{
						prev = isConsecutive(k,prev, supervisors);
						if(prev!=-1)
							count++;
					}
				}//if(supervisors[k].getID()=="S005"){cout<<"count: "<<count<<endl;
				//}
			}
				
			
			
			if(count > supervisors[k].getConsecPref()-1){
				penalty+=10;
				sc01++;
				if(print==true){
					cout << "SC01 Violation " << sc01 << endl;
					cout << "Supervisor: " << supervisors[k].getID() << " Schedule: ";
					for(auto it1=supervisors[k].schedule.begin(); it1!=supervisors[k].schedule.end(); ++it1){
						cout << *it1 << " ";
					}
					cout << endl << endl;
				}
			}
			prev = *it;
		}
		
		if(mon==true)
			workDays++;
		if(tues==true)
			workDays++;
		if(wed==true)
			workDays++;
		if(thurs==true)
			workDays++;
		if(fri==true)
			workDays++;
			
		if(workDays > supervisors[k].getDaysPref()){
			penalty+=10;
			sc02++;
			if(print==true){
				cout << "SC02 Violation " << sc02 << endl;
				cout << "Supervisor: " << supervisors[k].getID() << " Schedule: ";
				for(auto it1=supervisors[k].schedule.begin(); it1!=supervisors[k].schedule.end(); ++it1){
					cout << *it1 << " ";
				}
				cout << endl << endl;
			}
		}
	}
}

void Individual::printChromosome(){
	cout << "chromosome : ";
	for(int i=0; i<118; i++){
		cout << chromosome[i] << " ";
	}
	cout << endl;
}

void Individual::printPenalty(){
	cout << "penalty= " << penalty << endl;
}

long Individual::getPenalty() const{
	return penalty;
}

long Individual::getPen(){
	return penalty;
}

Individual Individual::crossover(Individual parent, int mutationRate){
	int toChange, temp, mutation;
	for(int i=0; i<118; i++){
		toChange = rand()%2;
		if(toChange==1){
			temp = chromosome[i];
			chromosome[i] = parent.chromosome[i];
			parent.chromosome[i] = temp;
		}
	}
	mutation = rand()%100;
	if(mutation < mutationRate){
		int mutatePos = rand()%118;
		int mutateVal = rand()%300 + 1;
		chromosome[mutatePos] = mutateVal;
		
		mutatePos = rand()%118;
		mutateVal = rand()%300 + 1;
		parent.chromosome[mutatePos] = mutateVal;
	}
	return parent;
}

void Individual::localSearchMutatation(){

	int mutatePos = rand()%118;
	int mutateVal = rand()%300 + 1;
	chromosome[mutatePos] = mutateVal;
}

int* Individual::getChromosomePointer(){
	return chromosome;
}

int Individual::isConsecutive(int sup, int currentSlot, Supervisor supervisors[]){
	for(auto it=supervisors[sup].schedule.begin(); it!=supervisors[sup].schedule.end(); ++it){
		if(*it==currentSlot-1||*it==currentSlot+14||*it==currentSlot+29||*it==currentSlot+44||*it==currentSlot-16||
		*it==currentSlot-31||*it==currentSlot-46)
			return *it;
	}
	return -1;
}

