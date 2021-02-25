#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "map.h"
#include <cmath>

struct Point
{
  int x;
  int y;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};
enum Direction{
  left,
  right,
  top,
  bottom,
};
struct Enemy;

struct Player
{
  bool attack = false;
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords) {};

  Direction cur_direction = bottom;
  bool Moved() const;
  Point get_coords() const {return coords;}
  void ProcessInput(MovementDir dir, CellType* cm, bool * colls);
  void Draw(Image & character, Image & screen, float delta);
  void touch_enemy(){lives--;}
  void ProcessAnimation(Image & screen,Image * src_img1, CellType* c, bool* col,  float delta, Enemy& en1, Enemy& en2, Enemy& en3);
  int get_x() const{return coords.x;}
  int get_y() const{return coords.y;}
  int get_lives() const{return lives;}
  int xpos_fixed = 64;
  int ypos_fixed = 64;
private:
  int idx = 0;
  
  int idx_attack = 0;
  int lives = 3;
  float time = 0.1;
  float time_attack = 0.1;
  bool hide = false;
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 0, .g = 255, .b = 0, .a = 15};
  int move_speed = 3;
};


struct Enemy
{
  explicit Enemy(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords), xf((float)pos.x), yf((float)pos.y) {
         //         animation(a_path);
                 };
  Direction cur_direction = bottom;
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  int get_x() const{return coords.x;}
  int get_y() const{return coords.y;}
  void processMove(Point player_coord, bool* colls);
  bool Moved() const;
  float xf;
  float yf;
  bool animated = false;
  bool finish=false;
  float move_speed = 2.3;
 // Image character;
  int anim_iter = 0;
  float anim_time = 0;
  void Draw(Image & character,Image & animation, Image & screen, float delta, Point player_coords, bool* colls);
  void AnimationAttack(Image & animation,Image& screen, float delta);
  bool alive = true;
private:
  int idx = 0;
  float time = 0.1;
};

#endif //MAIN_PLAYER_H
