#pragma once

#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "Image.h"

#define BLOCK_SIZE 32
#define M_SIZE 16
#define N_SIZE 16

enum CellType {
    Space,
    Wall_left,
    Wall_right,
    Wall_bottom,
    Wall_top,
    Wall_mid,
    Ground,
    Ground_mid,
    Ground_top,
    Ground_bottom,
    Ground_left,
    Ground_right,
    Ground_tr,
    Ground_tl,
    Ground_br,
    Ground_bl,
    Character,
    Exit,
};

void layer_Draw(Image& screenBuffer, Image& avatar, Image& live_bar, Image& live, Image& anim,
    int idx, int x, int y, int lives);

CellType* readMap(const char*, bool** collision);

class backScene
{
    int _m, _n;
    int _blockSize;
    Pixel ** matrix_;
public:
    backScene(int m = M_SIZE, int n = N_SIZE, int blockSize=BLOCK_SIZE)
    {
        _m = m; _n = n; _blockSize=blockSize;
    }
    ~backScene(){}

    void Draw(Image &screen, Image&,Image&,Image&,Image&,  CellType*, int x_offs, int y_offs);

};