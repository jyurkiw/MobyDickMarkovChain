#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <deque>

// Constant for our filename.
//const char FILENAME[] = "../MobyDick.txt";
const char FILENAME[] = "../dog.txt";
const int MARKOV_CHAIN_DEGREE = 2;

// Prototypes
void add(std::string, std::deque<std::string> *, std::unordered_map<std::string, std::unordered_set<std::string> > *);
std::string getKey(std::deque<std::string> *);

/**
	Read in a file.
	Create a 2nd order Markov Chain data structure of the contents.
	Use the Markov Chain data structure to generate random text files of various lengths.
*/
int main(int argc, char* argv[])
{
	// File manipulation variables
	std::ifstream file;
	file.open(FILENAME, std::ios::in);

	// Our Markov Chain data structures
	std::unordered_map<std::string, std::unordered_set<std::string> > mChain;
	std::deque<std::string> currentChain;

	if (file.is_open())
	{
		std::string line;

		while (std::getline(file, line))
		{
			std::istringstream buf(line);
			std::istream_iterator<std::string> beg(buf), end;
			std::vector<std::string> vline(beg, end);

			for (auto& t : vline)
			{
				if (currentChain.size() < MARKOV_CHAIN_DEGREE)
					currentChain.push_back(t);
				else if (currentChain.size() >= MARKOV_CHAIN_DEGREE)
				{
					add(t, &currentChain, &mChain);
					currentChain.push_back(t);
					currentChain.pop_front();
				}
			}
		}

		file.close();
	}
	else 
	{
		std::cout << "Could not open file: " << FILENAME << std::endl;
	}

	std::cout << std::endl;

	for (auto it = mChain.begin(); it != mChain.end(); ++it)
	{
		std::cout << "Key: " << it->first << " Values: ";
		for (auto vv : it->second)
			std::cout << vv << " ";
		std::cout << std::endl;
	}

	std::cin.get();

	return 0;
}

/**
	Add the passed word to the mChain set keyed to the current key.
	@param word The word we're adding to the chain.
	@param current The currentt key collection.
	@param mChain The markov chain data structure.
*/
void add(std::string word, std::deque<std::string> *current, std::unordered_map<std::string, std::unordered_set<std::string> > *mChain)
{
	std::string key = getKey(current);
	std::cout << "Working with key: " << key << "!" << std::endl;

	std::unordered_map<std::string, std::unordered_set<std::string> >::const_iterator chainSet = mChain->find(key);
	if (chainSet != mChain->end())
	{
		(*mChain)[key].emplace(word);
		std::cout << "Added " << word << " to " << key << std::endl;
	}
	else
	{
		mChain->emplace(key, std::unordered_set<std::string>({ word }));
		std::cout << "Added " << word << " under " << key << std::endl;
	}
}

/**
	Get the current key from the passed key collection.
	The key is a space-delimited join of the contents of the key collection.
	@param current The current key collection.
	@return The key.
*/
std::string getKey(std::deque<std::string> *current)
{
	std::string key;
	for (auto it = current->begin(); it != current->end(); ++it)
		key += *it + " ";
	key.pop_back();

	return key;
}