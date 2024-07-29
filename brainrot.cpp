#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <random>
#include <cassert>

void printSet(const auto& set) {
  for (const auto& elem : set) {
    std::cout << elem << ' ';
  std::cout << '\n';
  }
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string word;

  while (getline(ss, word, delim)) {
    result.push_back(word);
  }

  return result;
}

std::string strip(std::string& s) {
  std::string result; // TODO
  return s;
}

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 100);

  std::ifstream words;
  std::string line;
  
  std::unordered_set<std::string> nouns;
  std::unordered_set<std::string> adjs;
  std::vector<std::string> nounsBrainrot;
  std::vector<std::string> adjsBrainrot;

  char delimiter = ' ';

  // read nouns
  words.open("nouns.txt");
  if (words.is_open()) {
    while (getline(words, line)) {
      nouns.insert(line);
    }
  }
  words.close();

  // read adjectives
  words.open("adjs.txt");
  if (words.is_open()) {
    while (getline(words, line)) {
      adjs.insert(line);
    }
  }
  words.close();

  // read brainrot
  words.open("brainrot_nouns.txt");
  if (words.is_open()) {
    while (getline(words, line)) {
      nounsBrainrot.push_back(line);
    }
  }
  std::uniform_int_distribution<> nounsDistrib(0, nounsBrainrot.size() - 1);
  words.close();
  words.open("brainrot_adjs.txt");
  if (words.is_open()) {
    while (getline(words, line)) {
      adjsBrainrot.push_back(line);
    }
  }
  std::uniform_int_distribution<> adjsDistrib(0, adjsBrainrot.size() - 1);
  words.close();
  
  std::cout << nouns.size() << ' ' << adjs.size() << ' ' << nounsBrainrot.size() << ' ' << adjsBrainrot.size();

  // read input text
  std::string in;
  std::cout << "Enter path to input: ";
  std::cin >> in;
  
  std::vector<std::vector<std::string>> text; // outer vector: lines, inner vector: words
  
  words.open(in);
  if (words.is_open()) {
    while (getline(words, line)) {
      text.push_back(split(line, delimiter));
    }
  }
  
  // replace text 
  std::cout << "Replacement probability 1 to 100: ";
  std::cin >> in;
  int p = std::stoi(in);
  assert(p > 0 && p <= 100);

  std::string stripped;
  for (int j = 0; j < text.size(); j++) {
    for (int i = 0; i < text[j].size(); i++) {
      stripped = strip(text[j][i]);
      // check if in nouns or adjectives
      if (nouns.contains(stripped)) {
        if (distrib(gen) < p) {
          text[j][i] = nounsBrainrot[nounsDistrib(gen)];
        }
      } else if (adjs.contains(stripped)) {
        if (distrib(gen) < p) {
          text[j][i] = adjsBrainrot[adjsDistrib(gen)];
        }
      }
    }
  }

  // write output
  std::ofstream outfile;
  outfile.open("out.txt");
  std::string outstr("");
  for (std::vector<std::string> ln : text) {
    for (std::string s : ln) {
      outstr += s += ' ';
    }
    outstr += '\n';
  }
  outfile << outstr;
  outfile.close();
  
  std::cout << "Replacement written." << std::endl;
  return 0;
}

