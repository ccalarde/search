#include <iostream>
#include <string>

#include "include/search.h"

using namespace std;

int main() {
  string filename;

  cout << "Enter a filename: ";

  getline(cin, filename);

  searchEngine(filename);
  return 0;
}
