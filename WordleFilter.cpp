#include <iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>
std::string filename = "possible_words.txt";

//here we go
class WordleRepo {
private:
	struct TrieNode
	{
		TrieNode* child[26] = { NULL };
		bool isValid = false;
		bool isEnd = false;
	};
	bool notList[26] = { false };
	std::string inputStr;
	std::string inStr;
	TrieNode* root;
public:	
	WordleRepo() {
		root = new TrieNode();  //要delete
	}

	~WordleRepo() {
		delete root; //WARNING: 这样好像会导致整个树memory leak
	}

	void loadData(std::string filename) {
		std::ifstream inputfile(filename);
		std::string word;
		while (getline(inputfile, word))
		{
			insert(word);
		}
	}

	void insert(std::string word) {
		int len = word.size();  
		TrieNode* currNode = root; 
		for (int i = 0; i != len; i++) { 
			int ch = word[i] - 'a'; 
			if (ch > -1 && ch < 26) { 
				if (!currNode->child[ch]) currNode->child[ch] = new TrieNode();
				currNode = currNode->child[ch]; 
			}
		}
		currNode->isEnd = true;
	}

	void ynFilter(TrieNode* curNode, int index) {
		if (index == inputStr.size()) return;
		if (inputStr[index] == '.') {
			++index;
			for (int j = 0; j != 26; j++) {
				if (curNode->child[j] && notList[j] == true) {
					curNode->child[j]->isValid = false;
				}
				else if (curNode->child[j]) {
					curNode->child[j]->isValid = true;
					ynFilter(curNode->child[j], index);
				}
			}
		}
		else
		{
			int ch = inputStr[index] - 'a';
			//除ch外都要set invalid
			for (int i = 0; i != ch; i++) {
				if (curNode->child[i]) {
					curNode->child[i]->isValid = false;
				}
			}
			for (int j = ch + 1; j != 26; j++) {
				if (curNode->child[j]) {
					curNode->child[j]->isValid = false;
				}
			}

			if (curNode->child[ch]) {
				++index;
				curNode->child[ch]->isValid = true;
				ynFilter(curNode->child[ch], index);
			}
		}
	}

	// 还要改二次输入
	bool inListFilter(std::string ansWord) {
		for (int i = 0; i != inStr.size(); i++) {
			if (ansWord.find(inStr[i]) == std::string::npos) return false;
		}
		return true;
	}

	void directPrint(TrieNode* curNode, std::string ansWord) {
		for (int j = 0; j != 26; j++) {
			if (curNode->child[j] && curNode->child[j]->isValid) { //verifying  
				std::string newAnsWord = ansWord;
				newAnsWord += 'a' + j;
				if (curNode->child[j]->isEnd) {		
					bool inListTestPassed = inListFilter(newAnsWord);
					if (inListTestPassed) {
						std::cout << newAnsWord << std::endl;
					}
					else
					{
						curNode->child[j]->isValid = false;
					}
				}
				else
				{		
					directPrint(curNode->child[j], newAnsWord);
				}
			}
		}//else do nothing(pop)
	}

	void startGame() {
		loadData(filename);
		std::cout << "Start Your Game With WordleFilter...\n";
		std::cout << "====================================\n";
		std::cout << "WordleFilter is used to search suitable words based on your feedback from game.\n";
		std::cout << "Let's say answer is 'robin' but we put 'bonus'.Then Rules like:\n";
		std::cout << "first input goes with: \".o...\"(five characters) \".\" for any, \"o\" for green o at the second.\n";
		std::cout << "second input goes with: \"us\"(grey characters) for those absolutely not in word.\n";
		std::cout << "third input goes with: \"bn\"(yellow characters) for those in word but not in position.\n";
		std::cout << "==============Let's Go==============\n";
		bool Finish = false;

		while (!Finish) {
			std::cout << "first input: ";
			getline(std::cin, inputStr, '\n');
			std::cout << "second input: ";
			std::string notStr;
			getline(std::cin, notStr, '\n');
			std::cout << "third input: ";
			std::string tempStr;
			getline(std::cin, tempStr, '\n');
			inStr.append(tempStr);

			int len1 = notStr.size();
			for (int i = 0; i != len1; i++) {
				int ch = notStr[i] - 'a';
				notList[ch] = true;
			}

			int index = 0;
			ynFilter(root, index);
			
			std::string ansWord;
			directPrint(root, ansWord);

			std::cout << "solved the wordle?[y/n]: ";
			std::string ans = "";
			getline(std::cin, ans, '\n');
			if (ans == "y") break;
			std::cout << "======again(appending)======\n";
		}
	}
};

int main()
{
	using namespace std;
	WordleRepo wordleFilter;
	wordleFilter.startGame();
	std::cin.get();
}