#include "map.h"
#include "Player.h"

CellType* readMap(const char* fname, bool** collision)
{
    int fd = open("/home/dmitry/CG21/task1/map.txt", O_RDONLY);
    CellType *mapArr = new CellType[256];
    *collision = new bool[256];
    if(fd == -1)
    {
        return mapArr;
    }

    char buf[8], len;
    int i=0;
    while((len = read(fd, buf, 8))>0)
    {
        for(int j=0;j<len;j++)
        {
            switch(buf[j])
            {
                case ' ': (*collision)[i]=false; mapArr[i]=Space; break; 
                case 'a': (*collision)[i]=true; mapArr[i]=Wall_left; break; 
                case 'd': (*collision)[i]=true; mapArr[i]=Wall_right; break; 
                case 's': (*collision)[i]=true; mapArr[i]=Wall_bottom; break; 
                case 'w': (*collision)[i]=true; mapArr[i]=Wall_top; break; 
                case 'm': (*collision)[i]=true; mapArr[i]=Wall_mid; break; 
                case '.': (*collision)[i]=false; mapArr[i]=Ground; break; 
                case 'g': (*collision)[i]=false; mapArr[i]=Ground_mid; break; 
                case 't': (*collision)[i]=false; mapArr[i]=Ground_top; break; 
                case 'b': (*collision)[i]=false; mapArr[i]=Ground_bottom; break; 
                case 'h': (*collision)[i]=false; mapArr[i]=Ground_left; break; 
                case 'f': (*collision)[i]=false; mapArr[i]=Ground_right; break; 
                case 'r': (*collision)[i]=false; mapArr[i]=Ground_tl; break; 
                case 'y': (*collision)[i]=false; mapArr[i]=Ground_tr; break; 
                case 'v': (*collision)[i]=false; mapArr[i]=Ground_bl; break; 
                case 'n': (*collision)[i]=false; mapArr[i]=Ground_br; break; 
                case 'x': (*collision)[i]=false; mapArr[i]=Exit; break; 
                default: i--; break;
            }
            i++;
        }
    }
    close(fd);
    return mapArr;
}

void backScene::Draw(Image &screen, Image& gr,Image& wl,Image& spc,Image& ext,  CellType* CT,  int x_offs, int y_offs)
{
    for(int i=0; i<_m;++i)
    {
        for (int j=0; j<_n;++j)
        {
            int x_offset = 0, y_offset = 0;
            Image *src;
            switch(CT[i + _m*j])
            {
                case Wall_left:
                    src=&wl;
                    x_offset = 96;
                    y_offset = 64;
                    break;
                case Wall_right:
                    src=&wl;
                    x_offset = 32;
                    y_offset = 64;
                    break;
                case Wall_top:
                    src=&wl;
                    x_offset = 64;
                    y_offset = 32;
                    break;
                case Wall_bottom:
                    src=&wl;
                    x_offset = 64;
                    y_offset = 96;
                    break;
                case Wall_mid:
                    src=&wl;
                    x_offset = 160;
                    y_offset = 160;
                    break;
                case Ground:
                    src=&gr;
                    x_offset = 160;
                    y_offset = 160;
                    break;
                case Ground_mid:
                    src=&gr;
                    x_offset = 64;
                    y_offset = 64;
                    break;
                case Ground_top:
                    src=&gr;
                    x_offset = 64;
                    y_offset = 32;
                    break;
                case Ground_bottom:
                    src=&gr;
                    x_offset = 64;
                    y_offset = 96;
                    break;
                case Ground_left:
                    src=&gr;
                    x_offset = 96;
                    y_offset = 64;
                    break;
                case Ground_right:
                    src=&gr;
                    x_offset = 32;
                    y_offset = 64;
                    break;
                case Ground_tr:
                    src=&gr;
                    x_offset = 96;
                    y_offset = 32;
                    break;
                case Ground_tl:
                    src=&gr;
                    x_offset = 32;
                    y_offset = 32;
                    break;
                case Ground_bl:
                    src=&gr;
                    x_offset = 32;
                    y_offset = 96;
                    break;
                case Ground_br:
                    src=&gr;
                    x_offset = 96;
                    y_offset = 96;
                    break;
                case Space: src=&spc; break;
                case Exit: src=&ext; break;
            }
            
            for (int x=0; x< _blockSize;x++){
                for (int y=0; y<_blockSize;y++)
                {
                    Pixel p = (*src).GetPixel(x + x_offset, y + y_offset);
                    screen.PutPixel(x_offs+x+i*_blockSize, y_offs+y+j*_blockSize, p);
                }
            }
            


        }
    }
}


void layer_Draw(Image& screenBuffer, Image& avatar, Image& live_bar, Image& live, Image& anim,
    int idx, int x_offs, int y_offs, int lives)
{
    for (int x = 0; x <= avatar.Width(); ++x)
      for (int y = 0; y <= avatar.Height(); ++y) 
        screenBuffer.PutPixel(x_offs + x, y_offs + y, avatar.GetPixel(x, avatar.Height() - y));

    x_offs += (avatar.Width() - live_bar.Width()) / 2;
    y_offs += avatar.Height();

    for (int x = 0; x < live_bar.Width(); ++x)
      for (int y = 0; y < live_bar.Height(); ++y)
      {
          Pixel p = live_bar.GetPixel(x, live_bar.Height() - y);
          if (p.a < 10)
            continue;
          screenBuffer.PutPixel(x_offs + x, y_offs + y , p);
      }

    x_offs += live_bar.Width() / 4 - 5;
    y_offs += live_bar.Height() / 2 - 7;

    for (int k = 0; k < lives; k++)
    {
        for (int x = 0; x < 16; ++x)
        for (int y = 0; y < 14; ++y)
        {
            Pixel p = live.GetPixel(x, live.Height() - y);
            if (p.a < 10)
                continue;
            screenBuffer.PutPixel(x_offs + x, y_offs + y , p);
        }
        x_offs += 11;
    }

    if (idx)
    {
        int add = (idx) % 2 ;
        for (int x = 0; x < 16; ++x)
            for (int y = 0; y < 14; ++y)
        {
            Pixel p = anim.GetPixel(x + 16*add, anim.Height() - y);
            if (p.a < 10)
                continue;
            screenBuffer.PutPixel(x_offs + x, y_offs + y , p);
        }
    }

}