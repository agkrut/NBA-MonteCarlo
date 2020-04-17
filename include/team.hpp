#ifndef TEAM_HPP_
#define TEAM_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <json.hpp>

using std::string;
using std::vector;
using json = nlohmann::json;

class Team
{
public:
	Team(json j, string dataDir);
	~Team();
	
	string getCity();
	void setCity(string city);
	
	string getName();
	void setName(string name);
	
	string getTricode();
	void setTricode(string tricode);
	
	vector<double> getRsELO();
	void setRsELO(vector<double> rsELO);

	vector<double> getPsELO();
	void setPsELO(vector<double> psELO);

	string getTeamID();
	void setTeamID(string teamID);

	int getRsWins();
	void setRsWins(int rsWins);

	int getRsLosses();
	void setRsLosses(int rsLosses);

	void addPsELO(double newElo);
	
private:
	string city;
	string name;
	string tricode;
	vector<double> rsELO;
	vector<double> psELO;
	string teamID;
	int rsWins;
	int rsLosses;

	json readScheduleJSON(string dataDir);
};

#endif