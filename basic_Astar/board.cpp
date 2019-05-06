// Lession 1: A* search.
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::abs;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      State s = n == 0 ? State::kEmpty : State::kObstacle;
      row.push_back(s);
    }
    return row;
}

vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

bool Compare(vector<int> node1, vector<int> node2) {
  int f1 = node1[2] + node1[3];
  int f2 = node2[2] + node2[3];

  if (f1 > f2) {
    return true;
  } else {
    return false;
  }
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

int Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

/**
 * Check that a cell is valid: on the grid, not an obstacle, and clear.
 */
bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
  bool on_grid_x = (x >= 0 && x < grid.size());
  bool on_grid_y = (y >= 0 && y < grid[0].size());
  if (on_grid_x && on_grid_y)
    return grid[x][y] == State::kEmpty;
  return false;
}

void AddToOpen(int x, int y, int g, int h, vector<vector<int>>& openlist, vector<vector<State>> &grid)
{
    vector<int> node {x, y, g, h};
    openlist.push_back(node);
    grid[x][y] = State::kClosed;
}

void ExpandNeighbors(vector<int>& current, int goal[2], vector<vector<int>>& open, vector<vector<State>>& grid) {
  int x = current[0];
  int y = current[1];
  int g = current[2];

  int deltaX, deltaY, nbrX, nbrY, nbrG, nbrH;
  for (int i = 0; i < 4; i++) {
    deltaX = delta[i][0];
    deltaY = delta[i][1];
    nbrX = x + deltaX;
    nbrY = y + deltaY;
    if (CheckValidCell(nbrX, nbrY, grid)) {
      nbrG = g + 1;
      nbrH = Heuristic(nbrX, nbrY, goal[0], goal[1]);
      AddToOpen(nbrX, nbrY, nbrG, nbrH, open, grid);
    }
  }
}

vector<vector<State>> Search(vector<vector<State>>& grid, int init[2], int goal[2]) {
  // Create the vector of open nodes.
  vector<vector<int>> open {};

  int x = init[0];
  int y = init[1];
  grid[x][y] = State::kStart;
  int g = 0;
  int h = Heuristic(x, y, goal[0], goal[1]);
  AddToOpen(x, y, g, h, open, grid);

  while (!open.empty()) {
    CellSort(&open);
    vector<int> current = open.back();
    open.pop_back();
    int x = current[0];
    int y = current[1];
    grid[x][y] = State::kPath;

    if (x == goal[0] && y == goal[1]) {
      grid[x][y] = State::kFinish;
      return grid;
    }
    // If we're not done, expand search to current node's neighbors.
    ExpandNeighbors(current, goal, open, grid);
  }

  cout << "No path found!\n";
  return vector<vector<State>>{};
}

string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    case State::kPath: return "🚗   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
    default: return "0   ";
  }
}

void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}

#include "test.cpp"

int main() {
  auto board = ReadBoardFile("1.board");
  int init[2] = {0, 0};
  int goal[2] = {4, 5};
  auto solution = Search(board, init, goal);
  PrintBoard(solution);
  // Tests
  TestHeuristic();
  TestAddToOpen();
  TestCompare();
  TestSearch();
  TestCheckValidCell();
  TestExpandNeighbors();

}
