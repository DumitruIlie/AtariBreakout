//Ilie Dumitru
#include <cstdio>
#include <graphics.h>

struct Block
{
    bool block;
    int xL, xR, yU, yD, nrHits;
};

const int LAT=5, INA=5, BLKLAT=64, BLKH=32, MAXH=INA*(BLKH+2)+242, MAXW=LAT*(BLKLAT+3)+25, D=101, R=3;
const float L=D*0.5f;
int level=6, CULORI[7]={COLOR(255, 240, 200), COLOR(200, 20, 20), COLOR(20, 200, 20), COLOR(20, 20, 200), COLOR(255, 255, 255), COLOR(0, 0, 0), COLOR(64, 64, 64)}, score=0, scorelevel;

bool playBrickBreak();

inline int sgn(float x) {return (x>0)-(x<0);}

int main ()
{
    bool Game=playBrickBreak();
    while(Game)
        Game=playBrickBreak();
    return 0;
}

void ball(int x, int y)
{
    setcolor(COLOR(0, 0, 0));
    setfillstyle(SOLID_FILL, COLOR(200, 150, 200));
    fillellipse(x, y, R, R);
}

bool playBrickBreak()
{
    Block mat[INA][LAT];
    int x;
    FILE *f=fopen("levels.dat", "r");
    fscanf(f, "%i", &x);
    while(x!=level)
        fscanf(f, "%i", &x);
    scorelevel=0;
    int xMouse, yMouse;
    float xAdd=-0.5, yAdd=-0.5, xBall=MAXW/2, yBall=MAXH-30;
    for(int i=0;i<INA;++i)
        for(int j=0;j<LAT;++j)
        {
            fscanf(f, "%i", &mat[i][j].nrHits);
            mat[i][j].block=mat[i][j].nrHits;
            mat[i][j].xL=j*BLKLAT+21;
            mat[i][j].xR=mat[i][j].xL+BLKLAT-3;
            mat[i][j].yU=i*BLKH+21;
            mat[i][j].yD=mat[i][j].yU+BLKH-2;
        }
    initwindow(MAXW, MAXH, "Atari Breakout", (GetSystemMetrics(SM_CXSCREEN)-MAXW)>>1, (GetSystemMetrics(SM_CYSCREEN)-MAXH)>>1);
    setbkcolor(COLOR(255, 240, 200));
    cleardevice();
    for(int i=0;i<INA;++i)
        for(int j=0;j<LAT;++j)
            if(mat[i][j].block)
            {
                if(mat[i][j].nrHits==-1)
                {
                    mat[i][j].nrHits=7;
                    setfillstyle(1, CULORI[6]);
                    bar(mat[i][j].xL, mat[i][j].yU, mat[i][j].xL+BLKLAT-3, mat[i][j].yU+BLKH-4);
                }
                else
                {
                    setfillstyle(1, CULORI[mat[i][j].nrHits]);
                    bar(mat[i][j].xL, mat[i][j].yU, mat[i][j].xL+BLKLAT-3, mat[i][j].yU+BLKH-4);
                }
            }
    setfillstyle(1, COLOR(20, 20, 200));
    bar((MAXW-D)>>1, MAXH-14, ((MAXW+D)>>1)-1, MAXH-6);
    setcolor(COLOR(0, 0, 0));
    setfillstyle(SOLID_FILL, COLOR(200, 150, 200));
    fillellipse(xBall, yBall, R, R);
    bool ok=true;
    setcolor(BLACK);
    {
        char text[]="Press SPACE to begin the game";
        int xaux=textwidth(text)>>1, yaux=textheight(text)>>1;
        outtextxy((MAXW>>1)-xaux, (MAXH>>1)-yaux, text);
        while(!GetAsyncKeyState(VK_SPACE));
        getch();
        setfillstyle(1, COLOR(255, 240, 200));
        bar((MAXW>>1)-xaux, (MAXH>>1)-yaux, (MAXW>>1)+xaux-1, (MAXH>>1)+yaux-1);
    }
    int nrBlk;
    do
    {
        bool one=true;
        nrBlk=0;
        for(int i=0;i<INA;++i)
            for(int j=0;j<LAT;++j)
                if(mat[i][j].block)
                {
                    if(one)
                    {
                        float closestX=std::max(std::min(xBall, (float)mat[i][j].xR), (float)mat[i][j].xL), closestY=std::max(std::min(yBall, (float)mat[i][j].yD), (float)mat[i][j].yU);
                        if(closestX==xBall && closestY==yBall)
                        {
                            float dx1=xBall-mat[i][j].xL, dx2=mat[i][j].xR-xBall, dy1=yBall-mat[i][j].yU, dy2=yBall-mat[i][j].yD;
                            if(xAdd<0)
                            {
                                if(dx2<dx1)
                                {
                                    mat[i][j].nrHits--;
                                    mat[i][j].block=mat[i][j].nrHits;
                                    xAdd=-xAdd;
                                    one=false;
                                    if(mat[i][j].nrHits<6)
                                        scorelevel++;
                                }
                            }
                            else
                            {
                                if(dx1<dx2)
                                {
                                    mat[i][j].nrHits--;
                                    mat[i][j].block=mat[i][j].nrHits;
                                    xAdd=-xAdd;
                                    one=false;
                                    if(mat[i][j].nrHits<6)
                                        scorelevel++;
                                }
                            }
                            if(one)
                            {
                                if(yAdd<0)
                                {
                                    if(dy1>dy2)
                                    {
                                        mat[i][j].nrHits--;
                                        mat[i][j].block=mat[i][j].nrHits;
                                        yAdd=-yAdd;
                                        one=false;
                                        if(mat[i][j].nrHits<6)
                                            scorelevel++;
                                    }
                                }
                                else
                                {
                                    if(dy2<dy1)
                                    {
                                        mat[i][j].nrHits--;
                                        mat[i][j].block=mat[i][j].nrHits;
                                        yAdd=-yAdd;
                                        one=false;
                                        if(mat[i][j].nrHits<6)
                                            scorelevel++;
                                    }
                                }
                            }
                        }
                        else if((closestX-xBall)*(closestX-xBall)+(closestY-yBall)*(closestY-yBall)<=R*R)
                        {
                            mat[i][j].nrHits--;
                            mat[i][j].block=mat[i][j].nrHits;
                            one=false;
                            if(mat[i][j].nrHits<6)
                                scorelevel++;
                            int dirx=sgn(xBall-closestX), diry=sgn(yBall-closestY);
                            int dx=sgn(xAdd), dy=sgn(yAdd);
                            if(!dirx)
                                yAdd=-yAdd;
                            else if(!diry)
                                xAdd=-xAdd;
                            else if(dx!=dirx)
                                xAdd=-xAdd;
                            else if(dy!=diry)
                                yAdd=-yAdd;
                        }
                    }
                    setfillstyle(1, CULORI[mat[i][j].nrHits]);
                    bar(mat[i][j].xL, mat[i][j].yU, mat[i][j].xL+BLKLAT-3, mat[i][j].yU+BLKH-4);
                    mat[i][j].nrHits+=(mat[i][j].nrHits==6);
                    if(mat[i][j].nrHits<6 && mat[i][j].nrHits>0)
                        ++nrBlk;
                }
        xBall+=xAdd*3.5f;
        yBall+=yAdd*3.5f;
        ball(xBall, yBall);
        xMouse=mousex();
        setfillstyle(1, COLOR(20, 20, 200));
        bar(xMouse-L, MAXH-14, xMouse+L-1, MAXH-6);
        if(xBall<=R||xBall>=MAXW-R)
            xAdd=-xAdd;
        if(yBall<=R)
            yAdd=-yAdd;
        if(MAXH-yBall<=R)
            ok=false;
        if(yBall>=MAXH-14-R && xBall<=xMouse+R+L && xBall>=xMouse-R-L && yAdd>0)
        {
            xAdd=((float)(xBall-xMouse))/D;
            yAdd=-yAdd;
        }
        swapbuffers();
        cleardevice();
        Sleep(1);
    }while(ok&&nrBlk);
    closegraph();
    char s[5];
    sprintf(s, "%i", score+scorelevel);
    if(nrBlk==0)
    {
        initwindow(500, 200, "You Won", (GetSystemMetrics(SM_CXSCREEN)-500)>>1, (GetSystemMetrics(SM_CYSCREEN)-200)>>1);
        ++level;
    }
    else
        initwindow(500, 200, "You Lost", (GetSystemMetrics(SM_CXSCREEN)-500)>>1, (GetSystemMetrics(SM_CYSCREEN)-200)>>1);
    setfillstyle(1, COLOR(20, 200, 20));
    bar(10, 150, 49, 189);
    setfillstyle(1, COLOR(200, 20, 20));
    bar(450, 150, 489, 189);
    settextstyle(BOLD_FONT, 0, 1);
    setcolor(COLOR(200, 20, 20));
    outtextxy(10, 10, (char*)("Press the red button to exit"));
    setcolor(COLOR(20, 200, 20));
    if(!nrBlk)
    {
        outtextxy(10, 30, (char*)("or the green one to play the next level!"));
        score+=scorelevel;
    }
    else
        outtextxy(10, 30, (char*)("or the green one to play this level again!"));
    setcolor(WHITE);
    outtextxy(190, 100, (char*)("Your score:"));
    moveto(310, 100);
    outtext(s);
    while(true)
    {
        while(!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, xMouse, yMouse);
        if(xMouse>=10 && xMouse<=50 && yMouse>=150 && yMouse<=190)
        {
            closegraph();
            return true;
        }
        else if(xMouse>=450 && xMouse<=490 && yMouse>=150 && yMouse<=190)
        {
            closegraph();
            score+=scorelevel;
            return false;
        }
    }
}
