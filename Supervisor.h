#include <string>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

class Supervisor{
	private:
		string supID;
		vector<int> unavailable;
		int consec_pref;
		int days_pref;
		bool same_venue;
		
	public:
		vector<int> schedule;
		Supervisor(){
			supID = "";
			consec_pref = -1;
			days_pref = -1;
			same_venue = false;
		}
		void setID(string id);
		void setVenuePref(bool pref);
		void setUnavaliability(int x);
		void setConsecPref(int i);
		void setDaysPref(int i);
		string getID();
		int getConsecPref();
		int getDaysPref();
		bool getVenuePref();
		void printVector();
		vector<int> getUnavailable();
		void sortSchedule();		
};

void Supervisor::setID(string id){
	supID = id;
}

void Supervisor::setVenuePref(bool pref){
	same_venue = pref;
}

void Supervisor::setUnavaliability(int x){
	unavailable.push_back(x);
}

void Supervisor::setConsecPref(int i){
	consec_pref = i;
}

void Supervisor::setDaysPref(int i){
	days_pref = i;
}

string Supervisor::getID(){
	return supID;
}

int Supervisor::getConsecPref(){
	return consec_pref;
}

int Supervisor::getDaysPref(){
	return days_pref;
}

bool Supervisor::getVenuePref(){
	return same_venue;
}

void Supervisor::printVector(){
	for(auto it = unavailable.begin(); it!=unavailable.end(); ++it){
		cout << *it << " ";
	}
}

vector<int> Supervisor::getUnavailable(){
	return unavailable;
}

void Supervisor::sortSchedule(){
	sort(schedule.begin(), schedule.end(), [](int const &a, int const &b){
		return a < b;
	});
}
