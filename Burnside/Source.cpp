#include <iostream>
#include <vector>
using namespace std;

//
// The goal of the program is to verify Burnside's lemma.
// Burnside's lemma is useful for counting things with symmetry.
//
// This program uses the lemma to compute the number of necklaces
// that can be formed by n = 5 beads with c = 10 colors such that
// all reflections and rotations of the same coloring is counted 
// only once.
//
// Here is the key ingredient in the proof for the Burnside's lemma
// 
// y in Orbit of x means y = g x, define a map from X to 2^G as follow:
//
// Map an element y to the set of group element g such that y = g x
// y = h x => y = g g^{-1} h x = g (g^{-1} h) x, 
// Notice g^{-1} h x = g^{-1} g x = x, therefore for every group element 
// y maps to, it is a coset element.
// On the other hand, every coset element is valid to be mapped to by y, 
// Therefore we can map injectively an element in the orbit to a coset
// Therefore, the size of the orbit is the number of cosets = |G|/|Gx|
//
// Last but not least, if for each element, we add up the size its stablizer group, 
// we get this:
//
//   sum for each x, sum |Gx|
// = for each orbit, for each orbit element, sum |Gx|
// = for each orbit, sum |G|/|Gx| * |Gx|
// = for each orbit, sum |G|
// = number of orbit * |G|
// 
// So this is the Burnside's lemma, you can calculate the number of orbits
// by summing the size of all the stablizer group and then divide it by the
// size of the group.
//
// Note that when we compute sum for each x, sum |Gx|, it is equivalent to 
// flip to loop and ask, for each permutation, sum the number of configuration it fixes.
//
// This is where the Polya enumeration formula comes in. After we factor the 
// permutation into disjoint cycles, we figure that in order for a configuration is to be fixed
// It must have the same color for all nodes within a cycle, therefore, 
// for each cycle, we can make c choices for the color, and therefore we can count the number
// of configuration get fixed by a permutation without enumerating the configurations at all.
//

int main(int argc, char** argv)
{
	vector<vector<int>> group_elements;
	int n = 5;
	int c = 10; // Polya's enumeration work much better when c is large - enumerating the configuration space is expensive
	for (int offset = 0; offset < n; offset++)
	{
		vector<int> forward;
		for (int i = 0; i < n; i++)
		{
			forward.push_back((i + offset) % n);
		}
		group_elements.push_back(forward);
		vector<int> backward;
		for (int i = 0; i < n; i++)
		{
			backward.push_back(((n - i) + offset) % n);
		}
		group_elements.push_back(backward);
	}

	bool burnside = true;
	bool polya = true;

	cout << "Begin Group" << endl;
	for (size_t i = 0; i < group_elements.size(); i++)
	{
		for (size_t j = 0; j < group_elements[i].size(); j++)
		{
			cout << group_elements[i][j] << " ";
		}
		cout << endl;
	}
	cout << "End Group" << endl;
	if (burnside)
	{
		cout << "Starting brute force verification of Burnside's lemma" << endl;
		// This version of the code directly exercise the Burnside's lemma as is:
		// This is inefficient because we have to go through all configurations
		vector<int> config;
		vector<int> mirror;
		config.resize(n);
		mirror.resize(n);

		int all_stablizer_count = 0;
		for (size_t k = 0; k < group_elements.size(); k++)
		{
			for (size_t j = 0; j < group_elements[k].size(); j++)
			{
				cout << group_elements[k][j] << " ";
			}
			int fixed_count = 0;
			int seq = 0;

			// A simple odometer to loop through all the config
			while (true)
			{
				int cur = seq;
				bool last = true;
				for (int i = 0; i < n; i++)
				{
					config[i] = cur % c;
					cur = cur / c;
					last = last & (config[i] == c - 1);
				}

				// Here we have got a configuration - check if it is fixed by the current permutation

				bool is_fixed = true;
				for (int l = 0; is_fixed && l < n; l++)
				{
					is_fixed = is_fixed && (config[group_elements[k][l]] == config[l]);
				}

				if (is_fixed)
				{
					fixed_count++;
				}

				if (last)
				{
					break;
				}
				seq++;
			}
			cout << " fixes " << fixed_count << " configurations" << endl;
			all_stablizer_count += fixed_count;
		}
		cout << all_stablizer_count << " " << group_elements.size() << endl;
	}

	if (polya)
	{
		cout << "Starting polya's enumeration formula" << endl;
		int all_stablizer_count = 0;
		for (size_t i = 0; i < group_elements.size(); i++)
		{
			vector<bool> used;
			used.resize(group_elements[i].size());
			for (size_t j = 0; j < used.size(); j++)
			{
				used[j] = false;
			}
			for (size_t j = 0; j < group_elements[i].size(); j++)
			{
				cout << group_elements[i][j] << " ";
			}
			int fixed_count = 1;
			for (size_t j = 0; j < group_elements[i].size(); j++)
			{
				size_t k = j;
				int element_in_cycle = 0;
				cout << "(";
				while (!used[k])
				{
					element_in_cycle++;
					cout << k << " ";
					used[k] = true;
					k = group_elements[i][k];
				}
				cout << ") [" << element_in_cycle << "] ";
				if (element_in_cycle != 0)
				{
					fixed_count *= c;
				}
			}
			cout << " fixes " << fixed_count << " configurations" << endl;
			all_stablizer_count += fixed_count;
		}
		cout << all_stablizer_count << " " << group_elements.size() << endl;
	}
}