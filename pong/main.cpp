#include <cstdlib>
#include <time.h>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <ostream>
#include <vector>

using namespace std;
void print_game(vector< vector<char> > game);

int main() {

  // initialize the game board
  vector< vector<char> > game;
  for (int i = 0; i < 12; i++) {
    vector<char> temp;
    for (int j = 0; j < 14; j++) {
      if (j == 0) {
        temp.push_back('x');
      }
      else if (j == 13 && (i > 4 && i < 8)) {
        temp.push_back('#');
      }
      else if (j == 6 && (i == 6)) {
        temp.push_back('O');
      }
      else {
        temp.push_back(' ');
      }
    }
    game.push_back(temp);
  }

  int ball_row = 6;
  int ball_col = 6;
  int velocity_x = 1, velocity_y = 1;
  int paddle_x = 13, paddle_y = 6;

  int num_moves = 1;
  for (int w = 0; w < num_moves; w++) {
    game[paddle_y+3][paddle_x] = '#';
    game[paddle_y+2][paddle_x] = '#';
    game[paddle_y+1][paddle_x] = '#';
    game[paddle_y-1][paddle_x] = ' ';
    game[paddle_y][paddle_x] = ' ';
    paddle_y += 2;
  }
  int a=rand()%2;

  while (ball_col != 13) {
    int U = rand() % 2;
    game[ball_row][ball_col] = ' ';

    // boundary checking
    if (ball_row < 1) { // outside leftmost column
      velocity_y = -velocity_y;
      ball_row += velocity_y;
    }
    if (ball_row > 10) { // outside topmost row
      velocity_y = -velocity_y;
      ball_row += velocity_y;
    }
    if (ball_col < 2) { // outside bottommost row
      velocity_x = -velocity_x;
      ball_col += velocity_x;
    }
    if (ball_col > 11) {
      if (game[ball_row+velocity_y][ball_col+velocity_x] == '#') {
        velocity_x = -velocity_x;
        velocity_y = U;
        ball_col += velocity_x;
      }
      else {
        ball_row += velocity_y;
        ball_col += velocity_x;
      }
    }
    else {
      ball_row += velocity_y;
      ball_col += velocity_x;
    }

    //update ball position
    game[ball_row][ball_col] = 'O';

    cout << '\r';
    print_game(game);
    cout << std::flush;
    usleep(100000);

  }
  cout << "Game ended." <<endl;

  // int i = 0;
  // while (i < 101) {
  //   cout << "\r" << i << std::flush;
  //   usleep(100000);
  //   i++;
  // }
  // cout << endl;

  return 0;

}

void print_game(vector< vector<char> > game) {

  cout << "--------------"<<endl;
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 14; j++) {
      cout<< game[i][j];
    }
    cout << endl;
  }
  cout << "--------------"<<endl<<endl<<endl<<endl<<endl;
}