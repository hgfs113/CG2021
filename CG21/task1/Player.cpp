#include "Player.h"

#define hero_size 48

bool Player::Moved() const
{
  if((coords.x == old_coords.x) && (coords.y == old_coords.y))
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir, CellType* cm, bool * colls)
{
  int move_dist = move_speed * 1;

  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y;
      coords.y += move_dist;
      cur_direction = top;
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      coords.y -= move_dist;
      cur_direction = bottom;
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      coords.x -= move_dist;
      cur_direction = left;
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      coords.x += move_dist;
      cur_direction = right;
      break;
    default:
      break;
  }

  int i = coords.x  / BLOCK_SIZE;
  int j = coords.y / BLOCK_SIZE;
  bool add_i = coords.x  % BLOCK_SIZE;
  bool add_j = coords.y  % BLOCK_SIZE;
  
  if(colls[i + M_SIZE*j] || colls[i + add_i + M_SIZE*j] ||
  colls[i + M_SIZE*(j + add_j)] || colls[i + add_i + M_SIZE*(j + add_j)] )
  {
    coords.x = old_coords.x;
    coords.y = old_coords.y;
  }

}

void Player::Draw(Image & character, Image &screen, float delta)
{
  if (!Moved())
  {
    idx = 1;
  }
  else
    time += delta;
  if (time > 0.07){
    idx++;
    idx %= 3;
    time = 0;
  }
  int x_offs = 0, y_offs = 0; 
  x_offs = 48*idx;
  switch(cur_direction)
  {
    case left:
      y_offs = 96; break;
    case right:
      y_offs = 48; break;
    case top:
      y_offs = 144; break;
    case bottom:
      y_offs = 0; break;
  }

  for(int y = 0; y <= hero_size; ++y)
  {
    for(int x = 0; x <= hero_size; ++x)
    {
      if (character.GetPixel(x + x_offs, y + y_offs).a == 0)
        continue;
      screen.PutPixel(hero_size - x + coords.x - 8, hero_size  -  y + coords.y, character.GetPixel(x + x_offs, y + y_offs));
    }
  }
  old_coords.x = coords.x;
  old_coords.y = coords.y;
}

void Player::ProcessAnimation(Image& screen, Image * src_img, CellType* c, bool* col, float delta, Enemy& en1, Enemy& en2, Enemy& en3)
{
  if (!attack)
    return;
  int size = 64;
  time_attack += delta;
  for(int x = 0; x < size; x++)
    for(int y = 0; y < size; y++)
    {
      if (src_img[idx_attack % 10].GetPixel(size-x, size-y).a < 10)
      {
        continue;
      }
      screen.PutPixel(x + xpos_fixed - 32, y + ypos_fixed - 32, src_img[idx_attack % 10].GetPixel(size-x, size-y));
    }
  if (time_attack >= 0.1)
  {
    time_attack = 0;
    ++idx_attack;
    if (idx_attack == 10)
    {
      idx_attack = 0;
      attack = false;
      float l2 = sqrt(pow(en1.coords.x + 16 - xpos_fixed, 2) + pow(en1.coords.y + 16  - ypos_fixed, 2));
      if (l2 < 30)
      {
        en1.alive = false;
      }
      l2 = sqrt(pow(en2.coords.x + 16 - xpos_fixed, 2) + pow(en2.coords.y + 16  - ypos_fixed, 2));
      if (l2 < 30)
      {
        en2.alive = false;
      }
      l2 = sqrt(pow(en3.coords.x + 16 - xpos_fixed, 2) + pow(en3.coords.y + 16  - ypos_fixed, 2));
      if (l2 < 30)
      {
        en3.alive = false;
      }
    }
  }
}

void Enemy::processMove(Point player_coord, bool* colls)
{
  if (!alive)
    return;
  float move_dist = move_speed * 1;
  float dx = xf - (float)player_coord.x;
  float dy = yf - (float)player_coord.y;
  float cos_a = -dx / sqrt(pow(dx, 2) + pow(dy, 2));
  float sin_a = -dy / sqrt(pow(dx, 2) + pow(dy, 2));
  
  MovementDir dir;

  if ((cos_a > (sqrt(2) / 2)) || (cos_a < -(sqrt(2) / 2)))
  {
    if (dx > 0)
      dir = MovementDir::LEFT;
    else
      dir = MovementDir::RIGHT;
  }
  else
  {
    if (dy > 0)
      dir = MovementDir::DOWN;
    else
      dir = MovementDir::UP;
  }

  xf += move_dist*cos_a;
  yf += move_dist*sin_a;
  old_coords.y = coords.y;
  coords.y = round(yf);
  old_coords.x = coords.x;
  coords.x = round(xf);

  switch(dir)
  {
    case MovementDir::UP:
      cur_direction = top;
      break;
    case MovementDir::DOWN:
      cur_direction = bottom;
      break;
    case MovementDir::LEFT:
      cur_direction = left;
      break;
    case MovementDir::RIGHT:
      cur_direction = right;
      break;
    default:
      break;
  }

  int i = coords.x  / BLOCK_SIZE;
  int j = coords.y / BLOCK_SIZE;
  bool add_i = coords.x  % BLOCK_SIZE;
  bool add_j = coords.y  % BLOCK_SIZE;
  
  if(colls[i + M_SIZE*j] || colls[i + add_i + M_SIZE*j] ||
  colls[i + M_SIZE*(j + add_j)] || colls[i + add_i + M_SIZE*(j + add_j)] )
  {
    xf -= move_dist*cos_a;
    yf -= move_dist*sin_a;
    coords.x = old_coords.x;
    coords.y = old_coords.y;
  }
}

void Enemy::AnimationAttack(Image & animation,Image& screen, float delta)
{
  if (!alive)
    return;
  anim_time += delta;
  if (anim_time > 0.09)
  {
    anim_iter++;
    anim_time = 0;
  }
  if (anim_iter > 4)
  {
    finish = true;
    anim_iter = 0;
    animated = false;
  }
  int x_offs = anim_iter*hero_size; 
  for(int y = 0; y < hero_size; ++y)
  {
    for(int x = 0; x < hero_size; ++x)
    {
      if (animation.GetPixel(x + x_offs, y).a == 0)
        continue;
      if ((cur_direction == right) || (cur_direction == top))
          screen.PutPixel(hero_size - x + coords.x - 8, hero_size  -  y + coords.y, animation.GetPixel(x + x_offs, y));
      else
          screen.PutPixel(x + coords.x - 8, hero_size  -  y + coords.y, animation.GetPixel(x + x_offs, y));

    }
  }
}

void Enemy::Draw(Image & character, Image & animation, Image &screen, float delta, Point player_coords, bool * colls)
{
  if (!alive)
    return;
  // logic
  float t = sqrt(pow(player_coords.x - coords.x, 2) + pow(player_coords.y - coords.y, 2));
  if (t < 32)
  {
    AnimationAttack(animation, screen, delta);
    animated = true;
  }
  else
  {
    animated = false;
    anim_iter = 0;
    anim_time = 0;
  }
  if ((t < 100) && (t > 26))
    processMove(player_coords, colls);
    

  int x_offs = 0, y_offs = 0; 
  if (!Moved())
  {
    idx = 1;
  }
  else
    time += delta;
  if (time > 0.1){
    idx++;
    idx %= 3;
    time = 0;
  }
  x_offs = 48*idx;
  switch(cur_direction)
  {
    case left:
      y_offs = 96; break;
    case right:
      y_offs = 48; break;
    case top:
      y_offs = 144; break;
    case bottom:
      y_offs = 0; break;
  }

  
  if (animated)
    return;
  for(int y = 0; y <= hero_size; ++y)
  {
    for(int x = 0; x <= hero_size; ++x)
    {
      if (character.GetPixel(x + x_offs, y + y_offs).a == 0)
        continue;
      screen.PutPixel(hero_size - x + coords.x - 8, hero_size  -  y + coords.y, character.GetPixel(x + x_offs, y + y_offs));
    }
  }
  old_coords.x = coords.x;
  old_coords.y = coords.y;
}

bool Enemy::Moved() const
{
  if((coords.x == old_coords.x) && (coords.y == old_coords.y))
    return false;
  else
    return true;
}