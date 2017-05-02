#include <vector>
#include <string>
#include <sstream>
#include <iostream>

std::vector<std::string> split(std::string const &s, char delimiter) {
  std::vector<std::string> out {};
  std::stringstream ss {s};
  std::string item;

  while (std::getline(ss, item, delimiter)) {
    out.push_back(item);
  }

  return out;
}

int main() {

  char const *test = "aa vv gg errt";

  std::vector<std::string> out = split(test, ' ');

  for (auto v: out) {
    std::cout << v << '\n';
  }

  return 0;
}

