/*
* trade.cpp
*
*  Created on: Nov 11, 2018
*      Author: madhurin
*/

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <assert.h>


using namespace std;

// To execute C++, please define "int main()"

struct symbolInfo{
	long Quantity;
	long Price;
	long MaxTimeGap = 0;
	long Volume = 0;
	long TotalPrice = 0;
	long MaxPrice = 0;
	long long startTime =0;
};

void parse_csv(map< string, symbolInfo > &symbol){

	std::ifstream ip;

	ip.open("C:\\input.csv", std::ifstream::in);
	if (!ip.is_open()){
		assert(0 && "Cannot open input file to read");
		exit(-1);
	}

	string line;
	string data;
	string key = "";
	vector<string> row;
	long time_gap = 0;

	for (unsigned int i = 0; std::getline(ip, line); i++)
	{
		stringstream ss(line);

		for (unsigned int i = 0; std::getline(ss, data, ','); i++)
			row.push_back(data);   //<TimeStamp>,<Symbol>,<Quantity>,<Price>

		key = row[1];   

		if (symbol.find(key) == symbol.end()){
			symbol[key].startTime = (atoll(row[0].c_str()));
			symbol[key].Quantity = (atol(row[2].c_str()));
			symbol[key].Price = (atol(row[3].c_str()));
		}
		else{
			time_gap = abs(symbol[key].startTime - (atoll(row[0].c_str())));    //find max time gap
			if (symbol[key].MaxTimeGap < time_gap){
				symbol[key].MaxTimeGap = time_gap;
			}

			symbol[key].Volume += atol(row[2].c_str());  //find trade vol

			if (atol(row[3].c_str()) > symbol[key].MaxPrice){
				symbol[key].MaxPrice = atol(row[3].c_str());   //find max price
			}

			long tot = atol(row[2].c_str())*atol(row[3].c_str());  //find total price
			symbol[key].TotalPrice += tot;
		}
		row.clear();
	}
	ip.close();
}

void write_to_file(map< string, symbolInfo > &symbol){

	map< string, symbolInfo >::iterator it;
	ofstream myfile;
	myfile.open("C:\\Users\\madhurin\\Documents\\output.csv");
	if (!myfile.is_open()){
		assert(0 && "Cannot open output file to write");
		exit(-1);
	}
	//<symbol>, <MaxTimeGap>, <Volume>, <WeightedAveragePrice>, <MaxPrice>
	for (it = symbol.begin(); it != symbol.end(); it++){
		myfile << it->first << ",";
		myfile << it->second.MaxTimeGap << ",";
		myfile << it->second.Volume << ",";
		myfile << (it->second.TotalPrice / it->second.Volume) << ",";
		myfile << it->second.MaxPrice << "\n";
	}

	myfile.close();
}

int main() {
	map< string, symbolInfo > symbol;
	parse_csv(symbol);
	write_to_file(symbol);
	return 0;
}