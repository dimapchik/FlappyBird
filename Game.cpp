#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <deque>
#include <stdio.h>
#include "letters.h"
#include <iostream>
#include <string>

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, VK_RETURN)
//
//  schedule_quit_game() - quit game after act()

#define BIRD_SIZE 10
#define YELLOW 0xFFFF00
#define GREEN 0xFF00FF00
#define WHITE 0xFFFFFF
#define PIPE_WIDTH 30
#define BIRDX 100
#define GRAVITY 500.0f
#define PIPESPEED 200.0f

int reset_width = 680;
int reset_width_diff = 1;
int pipe_gap = 100;
int pipe_gap_diff = 10;

float bird_y = SCREEN_HEIGHT / 2;
float bird_velocity = 0;
float jump_strength = -300.0f;

struct Pipe {
  float x;
  int gap_y;
  int gap = pipe_gap;
  bool completed = false;
  Pipe(float x, int y) : x(x), gap_y(y) {}
  Pipe() : x(SCREEN_WIDTH + PIPE_WIDTH - 1), gap_y(rand() % (SCREEN_HEIGHT - pipe_gap - 50) + 25) {}
};

std::deque<Pipe> pipes;

bool is_game_over = false;
int score = 0;

bool is_collision() {
  for (Pipe& pipe : pipes) {
    if (bird_y <= 0 || bird_y + BIRD_SIZE >= SCREEN_HEIGHT ||
    ((pipe.x - PIPE_WIDTH < BIRDX + BIRD_SIZE) && (BIRDX < pipe.x) &&
    (bird_y < pipe.gap_y || bird_y + BIRD_SIZE > pipe.gap_y + pipe.gap))) {
      return true;
    }
    if (pipe.x < BIRDX && !pipe.completed) {
      ++score;
      pipe.completed = true;
    }
  }
  return false;
}

bool is_point_correct(int x, int y) {
  return (y >= 0 && y < SCREEN_HEIGHT &&
      x >= 0 && x < SCREEN_WIDTH);
}

// initialize game data in this function
void initialize() {
  bird_y = SCREEN_HEIGHT / 2;
  bird_velocity = 0;
  pipes.push_back(Pipe());
  score = 0;
  is_game_over = false;
}

void move(float dt) {
  bird_y += bird_velocity * dt;
  bird_velocity += GRAVITY * dt;
  for (Pipe& pipe : pipes) {
    pipe.x -= PIPESPEED * dt;
  }
}

void update_pipes() {
  if (pipes.back().x < reset_width) {
    pipes.push_back(Pipe());
  }
  if (pipes.front().x < 0) {
    pipes.pop_front();
  }
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt) {
  if (is_key_pressed(VK_ESCAPE)) { schedule_quit_game(); }
  if (is_key_pressed(VK_RETURN) && is_game_over) {
    is_game_over = true;
    finalize();
    initialize();
  }
  if (is_key_pressed(VK_UP) && pipe_gap < SCREEN_HEIGHT - 100) { pipe_gap += pipe_gap_diff; }
  if (is_key_pressed(VK_DOWN) && pipe_gap > 0) { pipe_gap -= pipe_gap_diff; }
  if (is_key_pressed(VK_RIGHT) && reset_width < SCREEN_WIDTH) { reset_width += reset_width_diff; }
  if (is_key_pressed(VK_LEFT) && reset_width > PIPE_WIDTH) { reset_width -= reset_width_diff; }
  if (is_game_over) { return; }
  if (is_key_pressed(VK_SPACE)) { bird_velocity = jump_strength; }
  move(dt);
  update_pipes();
  if (is_collision()) { is_game_over = true; }
}

void draw_square(int x, int y, int side, int color) {
  for (int i = 0; i < side; ++i) {
    for (int j = 0; j < side; ++j) {
      if (is_point_correct(x + j, y + i)) {
        buffer[y + i][x + j] = color;
      }
    }
  }
}

void draw_bird() {
  draw_square(BIRDX, bird_y, 10, YELLOW);
}

void draw_pipes() {
  for (Pipe& pipe : pipes) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      for (int x = 0; x < PIPE_WIDTH; x++) {
        if (y < pipe.gap_y || y > pipe.gap_y + pipe.gap) {
          if (is_point_correct(pipe.x - x, y)) {
            buffer[y][(int)(pipe.x - x)] = GREEN;
          }
        }
      }
    }
  }
}

void draw_text(std::string text, int x, int y, uint32_t color, int font_size) {
  for (int i = 0; i < text.size(); ++i) {
    char c = text[i];
    int index = -1;
    if (c >= '0' && c <= '9') {
      index = c - '0';
    } else if (c >= 'A' && c <= 'Z') {
      index = c - 'A' + 10;
    }
    if (c == ':') index = 36;
    if (index < 0) continue;
    for (int row = 0; row < CHAR_HEIGHT; ++row) {
      for (int col = 0; col < CHAR_WIDTH; ++col) {
        if (font[index][row][col] == '1') {
          draw_square(x + (col + i * (CHAR_WIDTH + 1)) * font_size, y + row * font_size, font_size, WHITE);
        }
      }
    }
  }
}

void draw_current_score(int font_size) {
  draw_text("SCORE: " + std::to_string(score), 10, 10, WHITE, font_size);
}

void draw_end_text(int font_size) {
  std::string game_over = "GAME OVER";
  int len_game_over = game_over.length();
  draw_text(game_over, SCREEN_WIDTH / 2 - font_size * len_game_over / 2 * CHAR_WIDTH, SCREEN_HEIGHT / 2 - (CHAR_HEIGHT + 2) * font_size, WHITE, font_size);
  std::string end_score = "YOUR SCORE: " + std::to_string(score);
  int len_end_score = end_score.length();
  draw_text(end_score, SCREEN_WIDTH / 2 - font_size * len_end_score / 2 * CHAR_WIDTH, SCREEN_HEIGHT / 2, WHITE, font_size);
  std::string restart = "PRESS ENTER TO RESTART";
  int len_restart = restart.size();
  draw_text(restart, SCREEN_WIDTH / 2 - font_size * len_restart / 2 * CHAR_WIDTH, SCREEN_HEIGHT / 2 + (CHAR_HEIGHT + 2) * font_size, WHITE, font_size);
}

void draw_current_settings(int font_size) {
  draw_text("PIPE GAP: " + std::to_string(pipe_gap), 10, 10 + (CHAR_HEIGHT + 2) * font_size, WHITE, font_size);
  draw_text("BETWEEN PIPES: " + std::to_string(SCREEN_WIDTH - reset_width), 10, 10 + 2 * (CHAR_HEIGHT + 2) * font_size, WHITE, font_size);
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw() {
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  draw_bird();
  draw_pipes();
  draw_current_score(2);
  draw_current_settings(2);
  if (is_game_over) {
    draw_end_text(5);
  }
}

// free game data in this function
void finalize() {
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  score = 0;
  pipes.clear();
}

