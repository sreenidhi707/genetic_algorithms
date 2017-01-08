// naive_string_match.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <random>
#include <cassert>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

const string initial_seed = "aaaaaaaaaaaaaaaaaa";
const string target = "ithinkthereforeiam";
const uint32_t INITIAL_POPULATION = 10;
const uint32_t SPAWN_SIZE = 10;
const uint32_t REUSE_PERCENT = 10;

struct cand_str
{
	string s;
	uint32_t dist;
};

int32_t get_rand(int32_t min, int32_t max)
{
	return min + (rand() % (int32_t)(max - min + 1));
}

uint32_t char_dist(char c1, char c2)
{
	uint32_t distance = abs(c1 - c2);
	if (distance > 13) 
	{
		distance = 26 - distance;
	}
	return distance;
}

uint32_t string_dist(const string s1, const string s2)
{
	uint32_t edits = 0;
	assert(s1.size() == s2.size());

	for (size_t i = 0; i < s1.size(); i++)
	{
		if (s1[i] != s2[i])
		{
			edits += char_dist(s2[i], s1[i]);
		}
	}

	return edits;
}

string mod_rand(string s)
{
	uint32_t pos = get_rand(0, s.size() - 1);
	uint32_t change = get_rand(0, 1);

	string begin = s.substr(0, pos);
	string trailing = s.substr(pos + 1);
	char c = s[pos];
	if (change)
	{
		c = (c == 'z') ? 'a' : c + 1;
	}
	else
	{
		c = (c == 'a') ? 'z' : c - 1;
	}
	string temp = begin + c + trailing;
	return temp;
}

vector<cand_str> spawn_children(string s)
{
	vector<cand_str> children;
	for (size_t i = 0; i < SPAWN_SIZE; i++)
	{
		cand_str temp;
		temp.s = mod_rand(s);
		temp.dist = string_dist(target, temp.s);
		children.push_back(temp);
	}
	return children;
}

bool cand_str_compare(cand_str l, cand_str r)
{
	return l.dist < r.dist;
}

int main()
{
	uint32_t initial_seed_size = initial_seed.size();

	vector<string> init_pop;
	for (size_t i = 0; i < INITIAL_POPULATION; i++)
	{
		string temp = mod_rand(initial_seed);
		init_pop.push_back(temp);
	}

	/*cout << "Initial population of strings are" << endl;
	for (size_t i = 0; i < init_pop.size(); i++)
	{
		cout << init_pop[i] << endl;
	}*/

	vector<cand_str> children;
	bool target_achieved = false;
	uint32_t epochs = 0;
	while (!target_achieved)
	{
		for (size_t i = 0; i < init_pop.size(); i++)
		{
			vector<cand_str> temp = spawn_children(init_pop[i]);
			children.insert(children.begin(), temp.begin(), temp.end());
		}

		/*cout << "Entire population of strings BEFORE sorting are" << endl;
		for (size_t i = 0; i < children.size(); i++)
		{
			cout << children[i].s << ", dist:" << children[i].dist << endl;
		}*/

		sort(children.begin(), children.end(), cand_str_compare);

		/*cout << "Entire population of strings AFTER sorting are" << endl;
		for (size_t i = 0; i < children.size(); i++)
		{
			cout << children[i].s << ", dist:" << children[i].dist << endl;
		}*/

		//Check if target string has been achieved
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i].s == target)
			{
				cout << "Target Achieved !!" << endl;
				target_achieved = true;
				break;
			}

		}

		//Take top few samples
		uint32_t num_top_samples = children.size() / REUSE_PERCENT;
		cout << "Best string in epoch " << epochs++ << " is " << children[0].s << endl;

		init_pop.clear();
		for (size_t i = 0; i < num_top_samples; i++)
		{
			init_pop.push_back(children[i].s);
		}
		children.clear();
	}
	



    return 0;
}

