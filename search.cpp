#include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  string newToken;
  int len = token.size();
  bool alpha = false;
  for (int i = 0; i < len; i++) {
    if (isalpha(token[i])) {
      alpha = true;
    }
  }

  for (int i = 0; i < len; i++) {
    newToken += tolower(token.at(i));
  }

  if (!alpha) {
    return "";
  }

  for (int i = 0; i < len; i++) {
    if (ispunct(newToken[i])) {
      newToken.erase(i--, 1);
      len = newToken.size();
    } else {
      break;
    }
  }

  len = newToken.size();
  for (int i = len - 1; i >= 0; i--) {
    if (ispunct(newToken[i])) {
      newToken.erase(i, 1);
      len = newToken.size();
    } else {
      break;
    }
  }
  return newToken;
}
set<string> gatherTokens(const string& text) {
  string str = cleanToken(text);
  string str2;

  str2 = "";

  // clean spaces
  stringstream stream(str);
  while (stream >> str) {
    if (!str2.empty()) {
      str2 += ' ';
    }
    str2 += str;
  }

  //
  set<string> v;
  string cleanedString = str2;
  stringstream ss(cleanedString);
  string str3;
  while (getline(ss, str3, ' ')) {
    str3 = cleanToken(str3);
    if (str3 != "") {
      v.emplace(str3);
    }
  }

  return v;
}
string clean(const string& s) {
  string str;
  string lowerCase;
  string str2;

  for (int i = 0; i < s.size(); i++) {
    str += (char)s.at(i);
  }

  for (int i = 0; i < str.size(); i++) {
    lowerCase += (char)tolower(str.at(i));
  }

  str = "";
  str2 = "";

  // clean spaces
  stringstream stream(lowerCase);
  while (stream >> str) {
    if (!str2.empty()) {
      str2 += ' ';
    }
    str2 += str;
  }

  return str2;
}
vector<string> splitBySpaces(const string& s) {
  vector<string> v;
  string cleanedString = clean(s);
  stringstream ss(cleanedString);
  string str;
  while (getline(ss, str, ' ')) {
    v.push_back(str);
  }

  return v;
}
int buildIndex(const string& filename, map<string, set<string>>& index) {
  // TODO student

  // step 1 break into pairs of website-content

  string str;
  vector<string> websites;
  vector<string> strings;
  set<string> tokens;
  ifstream inFS;
  set<string> websiteSet;
  int webpageCounter = 0;

  inFS.open(filename);

  if (!inFS.is_open()) {
    return 0;
  }

  int count = 0;
  while (getline(inFS, str)) {  // add words in the file into respective
                                // websites vector and strings vector
    count++;
    if (count % 2 != 0) {
      websites.push_back(str);
    } else {
      strings.push_back(str);
    }
  }

  // step 2 use gatherTokens to break down the file into separate words

  for (int i = 0; i < strings.size(); i++) {
    tokens = gatherTokens(strings.at(i));  // set of individual words in string
    for (auto itr : tokens) {              // iterate through set
      for (int k = 0; k < strings.size();
           k++) {  // iterate through every possible word
        if (strings.at(k).find(itr) !=
            string::npos) {  // check if individual tokens equal any
                             // other words in the file
          websiteSet.emplace(websites.at(k));
          // if they do, add the website to the set
        }
      }

      index.emplace(itr, websiteSet);

      websiteSet.clear();
    }
  }

  // step 3 iterate through the set of gatherTokens and place the strings into
  // the map

  // step 4 for the map the word is the key and the website is the index/value
  return websites.size();
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  string str = clean(sentence);
  vector<string> v = splitBySpaces(str);
  set<string> s;

  // main problem

  // how to check if a website contains a word or not

  for (int i = 0; i < v.size(); i++) {
    if (v.at(i).find('-') != string::npos) {  // not including word

      for (auto j :
           index.at(cleanToken(v.at(i)))) {  // check if there is an index with
                                             // a website // check if there
                                             // is an index with a website
        s.erase(j);
      }
      // remove corresponding website from the set
    } else if (v.at(i).find('+') !=
               string::npos) {  // intersection remove website from set IF it
                                // does not contain the current word

      for (auto k : s) {  // iterate through current set of websites

        if (index.at(cleanToken(v.at(i))).count(k) == 0) {
          s.erase(k);
        }
      }

    } else {                                        // or word
      if (index.count(cleanToken(v.at(i))) == 1) {  // checks for websites that
                                                    // have the word
        for (auto j : index.at(cleanToken(v.at(i)))) {
          s.emplace(j);
        }
      }
    }
  }

  return s;
}

void searchEngine(const string& filename) {
  map<string, set<string>> index;
  ifstream inFS;
  string str;
  cout << "Enter a filename: " << endl;
  inFS.open(filename);

  cout << "Indexed " << buildIndex(filename, index) << " pages containing "
       << index.size() << " unique terms" << endl;

  cout << "Enter query sentence (press enter to quit):" << endl;

  getline(cin, str);

  while (str != "") {
    getline(cin, str);
    cout << "Found " << findQueryMatches(index, str).size() << " matching pages"
         << endl;
    for (auto k : findQueryMatches(index, str)) {
      cout << k << endl;
    }
  }
}
