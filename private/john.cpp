#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <thread>
#include <vector>
using namespace std;
using namespace std::chrono_literals;

vector<pair<int, int>> pos; // Positions (x, y)
vector<pair<int, int>> vc;  // Velocities (vx, vy)
vector<vector<int>> new_map(103, vector<int>(101, 0));

regex getnum(R"((\d+),(\d+) v=(-?\d+),(-?\d+))");
smatch match;

void getinput() {
  ifstream file("input.txt");
  string line;
  while (getline(file, line)) {
    sregex_iterator begin(line.begin(), line.end(), getnum);
    sregex_iterator end;
    while (begin != end) {
      smatch match = *begin;
      pos.push_back({stoi(match[2].str()), stoi(match[1].str())});
      vc.push_back({stoi(match[4].str()), stoi(match[3].str())});
      ++begin;
    }
  }
}

pair<int, int> getFinal(int next_r, int next_c, int count, int i) {
  // base case
  if (count == 0)
    return {next_r, next_c};

  next_r = (next_r + vc[i].first) % 103;
  if (next_r < 0)
    next_r += 103;

  next_c = (next_c + vc[i].second) % 101;
  if (next_c < 0)
    next_c += 101;

  return getFinal(next_r, next_c, count - 1, i);
}

void makeNew(int row, int col) { new_map[row][col]++; }

int safetyFactor() {
  int rmid = new_map.size() / 2;
  int cmid = new_map[0].size() / 2;
  int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
  for (int i = 0; i < new_map.size(); i++) {
    for (int j = 0; j < new_map[0].size(); j++) {
      if (j > cmid && i < rmid) {
        q2 += new_map[i][j];
      } else if (j < cmid && i < rmid) {
        q1 += new_map[i][j];
      } else if (j < cmid && i > rmid) {
        q3 += new_map[i][j];
      } else if (j > cmid && i > rmid) {
        q4 += new_map[i][j];
      }
    }
  }

  return q1 * q2 * q3 * q4;
}

int main() {
  getinput();

  for (int j = 6630; j < 7000; j++) {
    for (int i = 0; i < pos.size(); i++) {
      pair<int, int> next = getFinal(pos[i].first, pos[i].second, j, i);
      makeNew(next.first, next.second);
    }
    for (auto &vec : new_map) {
      for (auto &ele : vec) {
        if (ele == 0) {
          cout << " ";
        } else
          cout << "@";
      }
      cout << "\n";
      fill(vec.begin(), vec.end(), 0);
    }
    cout << "\nAfter " << j << " iterations : \n";
    const auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(20ms);
    const auto end = std::chrono::high_resolution_clock::now();
  }
  return 0;
}