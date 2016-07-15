#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <stdlib.h>
#include <time.h>

// Constant for our filename.
//const char FILENAME[] = "../MobyDick.txt";
const char FILENAME[] = "../dog.txt";
const int MARKOV_CHAIN_DEGREE = 2;

// Typedefs
typedef std::deque<std::string> key_chain;
typedef std::unordered_map<std::string, std::unordered_set<std::string> > chain_store;

// Prototypes
void add(std::string, std::string, std::unordered_map<std::string, std::unordered_set<std::string> > *);
std::string getKey(key_chain *);
std::string getWord(std::string, chain_store *);
std::string getWord(key_chain *, chain_store *, chain_store *);

/**
	Read in a file.
	Create a 2nd order Markov Chain data structure of the contents.
	Use the Markov Chain data structure to generate random text files of various lengths.
*/
int main(int argc, char* argv[])
{
	// Seed rand()
	srand(time(NULL));

	// File manipulation variables
	std::ifstream file;
	file.open(FILENAME, std::ios::in);

	// Our Markov Chain data structures
	chain_store mChain;	// First order Markov Chain
	chain_store m2Chain;	// Second order Markov Chain
	key_chain currentChain, paraKeys;
	std::vector<std::string> vKeys;

	if (file.is_open())
	{
		std::string line;

		// Step through the file a line at a time.
		while (std::getline(file, line))
		{
			// Tokenize the string on spaces.
			// I'm not exactly sure why this works...
			std::istringstream buf(line);
			std::istream_iterator<std::string> beg(buf), end;
			std::vector<std::string> vline(beg, end);

			// Foreach through the tokens
			for (auto& t : vline)
			{
				// Process the first order chain
				// Take the previous word, and associate the current word
				if (currentChain.size() > 0)
					add(t, currentChain.back(), &mChain);

				// Process the second order chain
				// Check chain size. If less than the degree requirement, push back and move onto the next word.
				// Otherwise process the key and add the word to the markov chain data-structure.
				if (currentChain.size() < MARKOV_CHAIN_DEGREE)
					currentChain.push_back(t);
				else if (currentChain.size() >= MARKOV_CHAIN_DEGREE)
				{
					add(t, getKey(&currentChain), &m2Chain);
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

	// Compile a vector of keys so we can randomly pick out our first words.
	for (auto it = mChain.begin(); it != mChain.end(); ++it)
		vKeys.push_back(it->first);

	// Paragraph generation variables
	std::string paragraph = "";
	std::string nextWord;
	int paragraphLen = 52;

	// Pick the first words, and begin generating a paragraph
	currentChain.clear();

	// TODO: Generate a paragraph
	currentChain.push_back("fast");
	currentChain.push_back("fox");
	std::cout << "fast fox: " << getWord(&currentChain, &mChain, &m2Chain) << std::endl;
	

	std::cin.get();

	return 0;
}

/**
	Add the passed word to the mChain set keyed to the current key.
	@param word The word we're adding to the chain.
	@param current The currentt key collection.
	@param mChain The markov chain data structure.
*/
void add(std::string word, std::string key, chain_store *mChain)
{
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
std::string getKey(key_chain *current)
{
	std::string key;
	for (auto it = current->begin(); it != current->end(); ++it)
		key += *it + " ";
	key.pop_back();

	return key;
}

/**
	Get a word from the unordered set associated with the passed key.
	@param word The key to use.
	@param chain The markov chain store to search.
	@return A random word from the store.
*/
std::string getWord(std::string word, chain_store *chain)
{
	if ((*chain)[word].size() > 0)
	{
		int idx = rand() % (*chain)[word].size();
		std::unordered_set<std::string>::iterator it = (*chain)[word].begin();
		for (int i = 1; i < idx; i++) it++;

		return *it;
	} else return "";
}

/**
	Get a random word from one of the two passed markov chain stores.
	First, check the 2nd order store against the full key in the chain.
	If nothing is found for that key, use the last word in the keyChain
	to check the 1st order store.
	If that fails, return an empty string for error handling.
	@param keyChain The key chain structure to use as a key.
	@param mChain the 1st order Markov Chain store to use.
	@param m2Chain the 2nd order Markov Chain store to use.
	@return A word from one of the stores, or an empty string.
*/
std::string getWord(key_chain *keyChain, chain_store *mChain, chain_store *m2Chain)
{
	std::string key = getKey(keyChain);
	std::string word = getWord(key, m2Chain);

	//if (word != "") return word;
	//else return getWord(keyChain->back(), mChain);
	if (word != "")
	{
		std::cout << "oops" << std::endl;
		return word;
	}
	else
	{
		std::cout << "here" << std::endl;
		return getWord(keyChain->back(), mChain);
	}
}