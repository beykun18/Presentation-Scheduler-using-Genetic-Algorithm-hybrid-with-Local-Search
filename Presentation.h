#include <string>
#include "Supervisor.h"

using namespace std;

class Presentation{
	private:
		string presentationID;
		Supervisor supervisors[3];
		
	public:
		Presentation(){
			presentationID = "";
		}
		void setID(string id);
		void setSupervisors(Supervisor sup1, Supervisor sup2, Supervisor sup3);
		string getID();
		void printSupervisors();
		Supervisor* getSupervisors();
};

void Presentation::setID(string id){
	presentationID = id;
}

void Presentation::setSupervisors(Supervisor sup1, Supervisor sup2, Supervisor sup3){
	supervisors[0] = sup1;
	supervisors[1] = sup2;
	supervisors[2] = sup3;
}

string Presentation::getID(){
	return presentationID;
}

void Presentation::printSupervisors(){
	for(int i=0; i<3; i++){
		cout << supervisors[i].getID() << " ";
	}
}

Supervisor* Presentation::getSupervisors(){
	return supervisors;
}
