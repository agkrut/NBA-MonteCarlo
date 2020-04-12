#ifndef TEAM_HPP_
#define TEAM_HPP_

#include <string>
#include <vector>
#include <json.hpp>

using std::string;
using std::vector;

class Team
{
public:
	Team(nlohmann::json j);
	~Team();
	
	string getCity();
	void setCity(string city);
	
	string getName();
	void setName(string name);
	
	string getTricode();
	void setTricode(string tricode);
	
	vector<double> getRsELO();
	void setRsELO(vector<double> rsELO);

	double getCurrELO();
	void setCurrELO(double currELO);

	string getTeamID();
	void setTeamID(string teamID);

private:
	string city;
	string name;
	string tricode;
	vector<double> rsELO;
	double currELO;
	string teamID;
};

#endif