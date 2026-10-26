#include<iostream>
using namespace std;
#include <windows.h>
#include <GL/glut.h>
#include <math.h>


//int night =0;
int isLanding=0;
int landingRoll=0;
int isTakeoff=0;
float birdX = 0;
float birdY = 420;
float night = 0.0f;
float cycle = 0.0f;

int rainStart = 0;
// ===== CLOUD + RAIN =====
float cloudX = 0.0f;

float rainX[200];
float rainY[200];
int isRaining = 0;


void init()
{
    glClearColor(0.5, 0.8, 1.0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1000, 0, 500, -1, 1);
}

void keyboard(unsigned char key, int x, int y)
{
     if (key == 'l' || key == 'L')
    {
        isLanding = 1;
    }
    else if (key == 't' || key == 'T')
    {
        isTakeoff = 1;
        isLanding = 0;
    }
    else if (key == 'r' || key == 'R')   // ✅ START RAIN
    {
        rainStart = 1;
        isRaining = 1;
    }
    else if (key == 's' || key == 'S')   // ❌ STOP RAIN
    {
        rainStart = 0;
        isRaining = 0;
    }
    else if (key == 'q' || key == 'Q')
    {
        exit(0);
    }

    glutPostRedisplay();
}

void drawRain()
{
    if (!isRaining) return;

    glColor3f(0.65f, 0.85f, 1.0f);
    glLineWidth(1.0f);

    glBegin(GL_LINES);

    for (int i = 0; i < 200; i++)
    {
        // ❗ rain starts near cloud height (400–460 area)
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i] + 1, rainY[i] - 8);
    }

    glEnd();
}
void drawStars()
{
    if (night<=0.5) return;   // only at night

    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2);

    glBegin(GL_POINTS);

    // fixed star positions
    glVertex2f(100, 400);
    glVertex2f(200, 450);
    glVertex2f(300, 420);
    glVertex2f(400, 480);
    glVertex2f(500, 430);
    glVertex2f(600, 470);
    glVertex2f(700, 410);
    glVertex2f(800, 460);
    glVertex2f(900, 440);
    glVertex2f(150, 470);
    glVertex2f(350, 440);
    glVertex2f(650, 480);

    glEnd();
}

   void drawRunwayLights()
{

    for (int i = 0; i < 1000; i += 80)
{
    if(night>0.5)
       glColor3f(1, 1, 0); // yellow
     else
        glColor3f(0.9,0.9,0.9);

    glBegin(GL_QUADS);
    glVertex2f(i, 30);
    glVertex2f(i + 6, 30);
    glVertex2f(i + 6, 38);
    glVertex2f(i, 38);
    glEnd();

    if(night>0.5)
        glColor3f(1, 1, 0); // yellow
    else
        glColor3f(0.9,0.9,0.9);

    glBegin(GL_QUADS);
    glVertex2f(i, 122);
    glVertex2f(i + 6, 122);
    glVertex2f(i + 6, 130);
    glVertex2f(i, 130);
    glEnd();
}
}


void drawTree(float x, float y)
{
    // trunk
    glColor3f(0.4, 0.25, 0.1);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 8, y);
    glVertex2f(x + 8, y + 45);
    glVertex2f(x, y + 45);
    glEnd();

    // leaves (layered)

    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.65f, 0.25f);
    glVertex2f(x - 10, y + 45);
    glVertex2f(x + 16, y + 45);
    glVertex2f(x + 3, y + 75);
    glColor3f(0.0f, 0.75f, 0.30f);
    glVertex2f(x - 8, y + 65);
    glVertex2f(x + 14, y + 65);
     glVertex2f(x + 3, y + 95);
      glColor3f(0.1f, 0.85f, 0.35f);
      glVertex2f(x - 6, y + 85);
      glVertex2f(x + 12, y + 85);
      glVertex2f(x + 3, y + 110);
      glEnd();
}



void drawGrass()
{
     glBegin(GL_QUADS);

    if (night)
        glColor3f(0.02f, 0.12f, 0.02f);   // dark green (night)
    else
        glColor3f(0.05f, 0.35f, 0.10f);   // light green (day)

    glVertex2f(0, 120);
    glVertex2f(1000, 120);

    if (night>0.5)
        glColor3f(0.03f, 0.35f, 0.10f);   // slightly brighter top
    else
        glColor3f(0.20f, 0.75f, 0.25f);   // bright green top

    glVertex2f(1000, 200);
    glVertex2f(0, 200);

    glEnd();

    glBegin(GL_LINES);

for (int i = 0; i < 1000; i += 8)
{
    float wave = 2.5f * sin(i * 0.05f);

    float height = 10 + (wave + 3.0f) * 6;

    glVertex2f(i, 120);
    glVertex2f(i + 2, 120 + height);
}

glEnd();

}
void drawBuilding(float x, float w, float h, float r, float g, float b)
{
// gradient building
    glBegin(GL_QUADS);
    glColor3f(r, g, b);
    glVertex2f(x, 150);
    glVertex2f(x + w, 150);

    glColor3f(r, g, b);
    glVertex2f(x + w, 150 + h);
    glVertex2f(x, 150 + h);
    glEnd();

    // windows
    for (float y = 170; y < 150 + h - 10; y += 25)
    {
        for (float i = x + 10; i < x + w - 10; i += 18)
        {
            glBegin(GL_QUADS);
            if (night>0.5)
                glColor3f(1.0, 0.9, 0.3);
            else
                glColor3f(0.8, 0.95, 1.0);

            glVertex2f(i, y);
            glVertex2f(i + 10, y);
            glVertex2f(i + 10, y + 12);
            glVertex2f(i, y + 12);
            glEnd();
        }
    }
}


void drawWatchTower()
{

   glBegin(GL_QUADS);
    glColor3f(0.35f, 0.35f, 0.40f);  // left side lighter
    glVertex2f(490, 150);
    glVertex2f(590, 150);

    glColor3f(0.20f, 0.20f, 0.25f);  // top darker
    glVertex2f(570, 300);
    glVertex2f(510, 300);
    glEnd();


    glBegin(GL_QUADS);
    if (night>0.5)
        glColor3f(0.9f, 0.9f, 0.5f); // glowing at night
    else
        glColor3f(0.6f, 0.85f, 1.0f); // sky reflection

    glVertex2f(505, 300);
    glVertex2f(575, 300);
    glVertex2f(565, 370);
    glVertex2f(515, 370);
    glEnd();


    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.5);

    glBegin(GL_LINES);
    // horizontal
    glVertex2f(505, 335);
    glVertex2f(575, 335);

    // vertical middle
    glVertex2f(540, 300);
    glVertex2f(540, 370);
    glEnd();


    glBegin(GL_TRIANGLES);
    glColor3f(0.15f, 0.15f, 0.20f);
    glVertex2f(500, 370);
    glVertex2f(580, 370);
    glVertex2f(540, 410);
    glEnd();
// ================= BIGGER WINDOWS =================
glColor3f(0.05f, 0.05f, 0.10f); // dark glass

float wx1 = 515, wx2 = 535;
float wx3 = 545, wx4 = 565;

// row 1
glBegin(GL_QUADS);
glVertex2f(wx1, 310); glVertex2f(wx2, 310);
glVertex2f(wx2, 330); glVertex2f(wx1, 330);

glVertex2f(wx3, 310); glVertex2f(wx4, 310);
glVertex2f(wx4, 330); glVertex2f(wx3, 330);
glEnd();

// row 2
glBegin(GL_QUADS);

// left window
glVertex2f(wx1, 340);
glVertex2f(wx2, 340);
glVertex2f(wx2, 360);
glVertex2f(wx1, 360);

// right window (FIXED ORDER)
glVertex2f(wx3, 340);
glVertex2f(wx4, 340);
glVertex2f(wx4, 360);
glVertex2f(wx3, 360);

glEnd();
// ===================================================


    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(540, 410);
    glVertex2f(540, 460);
    glEnd();

    // antenna top light
    glPointSize(5);
    glBegin(GL_POINTS);
    if (night>0.5)
        glColor3f(1, 0, 0); // red light at night
    else
        glColor3f(0.3f, 0.3f, 0.3f);
    glVertex2f(540, 460);
    glEnd();

}


void drawControlTower()
{
    // body
    glBegin(GL_QUADS);
    glColor3f(0.25, 0.25, 0.3);
    glVertex2f(620, 150);
    glVertex2f(660, 150);

    glColor3f(0.15, 0.15, 0.2);
    glVertex2f(660, 420);
    glVertex2f(620, 420);
    glEnd();

    // windows
    for (float y = 170; y < 400; y += 25)
    {
        glBegin(GL_QUADS);
        if (night>0.5)
            glColor3f(1.0, 0.9, 0.3);
        else
            glColor3f(0.8, 0.95, 1.0);

        glVertex2f(630, y);
        glVertex2f(650, y);
        glVertex2f(650, y + 15);
        glVertex2f(630, y + 15);
        glEnd();
    }

    // top glass room
    glBegin(GL_QUADS);
    glColor3f(0.6, 0.85, 1.0);
    glVertex2f(610, 420);
    glVertex2f(670, 420);
    glVertex2f(660, 460);
    glVertex2f(620, 460);
    glEnd();
}

void drawRunway()
{
    // wider runway
    glBegin(GL_QUADS);
    glColor3f(0.25, 0.25, 0.25);
    glVertex2f(0, 30);
    glVertex2f(1000, 30);
    glVertex2f(1000, 130);
    glVertex2f(0, 130);
    glEnd();
    glBegin(GL_QUADS);
glColor3f(0.08f, 0.08f, 0.10f);
glVertex2f(0, 150);
glVertex2f(1000, 150);
glVertex2f(1000, 160);
glVertex2f(0, 160);
glEnd();

    // center dashed line
    for (int i = 0; i < 1000; i += 80)
    {
        glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
        glVertex2f(i, 80);
        glVertex2f(i + 45, 80);
        glVertex2f(i + 45, 85);
        glVertex2f(i, 85);
        glEnd();
    }
}

float r = 10.0;
float f = 70.0;

void drawAirplane()
{
    // tail
    glBegin(GL_POLYGON);
    glColor3f(0.70, 0.85, 0.90);
    glVertex2i(r, f + 50);
    glVertex2i(r + 40, f + 50);
    glVertex2i(r + 33, f + 60);
    glVertex2i(r + 10, f + 100);
    glVertex2i(r-10 , f + 100);
    glEnd();

    //upper wing
    glBegin(GL_POLYGON);
    glColor3f(0.20, 0.60, 0.60);
    glVertex2i(r+90, f + 50);//lower left corner
    glVertex2i(r + 120, f + 50);
    glVertex2i(r + 113, f + 60);//lower right corner
    glVertex2i(r + 85, f + 80);//upper right corner
    glVertex2i(r+70 , f + 80);//upper left corner
    glEnd();

    // lower wing
    glBegin(GL_POLYGON);
    glColor3f(0.20, 0.60, 0.60);
    glVertex2i(r+90,  f+25);
    glVertex2i(r+120, f+25);
    glVertex2i(r+113, f+15);
    glVertex2i(r+85,  f-5);
    glVertex2i(r+70,  f-5);

    glEnd();

    // tail wing
    glBegin(GL_POLYGON);
    glColor3f(0.20, 0.60, 0.60);
    glVertex2i(r+3,  f+52);
    glVertex2i(r+33, f+52);
    glVertex2i(r+26, f+42);
    glVertex2i(r-2,  f+22);
    glVertex2i(r-17,  f+22);
    glEnd();

    // wheel 1
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0.43f, 0.43f, 0.48f);
    glVertex2f(r + 40, f + 20);
    glVertex2f(r + 40, f);
    glEnd();

    // mini circle
    glColor3f(0.236f, 0.154f, 0.32f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(r + 40, f); // circle er center position
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * 3.1416f * i / 100;
        float x = 5 * cosf(angle); // value ta radius
        float y = 5 * sinf(angle);  // value ta radius
        glVertex2f(x + r + 40, y + f); // .. + circle er center position
    }
    glEnd();

    // wheel 2
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0.43f, 0.43f, 0.48f);
    glVertex2f(r + 140, f + 30);
    glVertex2f(r + 140, f);
    glEnd();
    // mini circle
    glColor3f(0,0,0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(r + 140, f); // circle er center position
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * 3.1416f * i / 100;
        float x = 5 * cosf(angle); // value ta radius
        float y = 5 * sinf(angle);  // value ta radius
        glVertex2f(x + r + 140, y + f); // .. + circle er center position
    }
    glEnd();

    // wheel 3
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0.43f, 0.43f, 0.48f);
    glVertex2f(180 + r, f + 30);
    glVertex2f(180 + r, f);
    glEnd();
    // mini circle
    glColor3f(0,0,0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(180 + r, f); // circle er center position
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * 3.1416f * i / 100;
        float x = 5 * cosf(angle); // value ta radius
        float y = 5 * sinf(angle);  // value ta radius
        glVertex2f(x + r + 180, y + f); // .. + circle er center position
    }
    glEnd();

    // body upside down
    glBegin(GL_POLYGON);
    glColor3f(0.15, 0.20, 0.35);

    glVertex2i(r + 25, f + 35);
    glVertex2i(r + 215, f + 35);
    glVertex2i(r + 215, f + 35);
    glVertex2i(r + 205, f + 25);
    glVertex2i(r + 205, f + 25);
    glVertex2i(r + 185, f + 15);
    glVertex2i(r + 45,  f + 15);
    glVertex2i(r+5,       f + 25);
    glVertex2i(r + 10,   f + 35);

    glEnd();

    // body
    glBegin(GL_POLYGON);
    glColor3f(0.70, 0.85, 0.90);
    glVertex2i(r + 20, f + 30);
    glVertex2i(r + 210, f + 30);//lower line front
    glVertex2i(r + 210, f + 40);
    glVertex2i(r + 200, f + 50);
    glVertex2i(r + 200, f + 50);
    glVertex2i(r + 180, f + 60);//front upper line
    glVertex2i(r + 30, f + 60); //upper line
    glVertex2i(r, f + 50);//
    glVertex2i(r+5, f + 40);//
    glEnd();

    //cockpit
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2i(r + 170, f + 40);
    glVertex2i(r + 210, f + 40);
    glVertex2i(r + 200, f + 50);
    glVertex2i(r + 170, f + 50);
    glEnd();


    // window
    glLineWidth(13);
    glBegin(GL_LINES);
    glColor3f(0.15, 0.20, 0.35);
    glVertex2f(r + 40, f + 45);
    glVertex2f(r + 50, f + 45);
    glEnd();
    glLineWidth(13);
    glBegin(GL_LINES);
    glColor3f(0.15, 0.20, 0.35);
    glVertex2f(r + 60, f + 45);
    glVertex2f(r + 70, f + 45);
    glEnd();
    glLineWidth(13);
    glBegin(GL_LINES);
    glColor3f(0.15, 0.20, 0.35);
    glVertex2f(r + 80, f + 45);
    glVertex2f(r + 90, f + 45);
    glEnd();
    glLineWidth(13);
    glBegin(GL_LINES);
    glColor3f(0.15, 0.20, 0.35);
    glVertex2f(r + 100, f + 45);
    glVertex2f(r + 110, f + 45);
    glEnd();
    glLineWidth(13);
    glBegin(GL_LINES);
    glColor3f(0.15, 0.20, 0.35);
    glVertex2f(r + 120, f + 45);
    glVertex2f(r + 130, f + 45);
    glEnd();
    glLineWidth(13);
    glBegin(GL_LINES);
    glColor3f(0.15, 0.20, 0.35);
    glVertex2f(r + 140, f + 45);
    glVertex2f(r + 150, f + 45);
    glEnd();
}

void drawSunMoon()
{
    float cx = 800;
    float cy = 420;
    float radius = 40;

    //day
    if (night <=0.5 )
    {
        glColor3f(1.0, 0.9, 0.2);

        glBegin(GL_POLYGON);
        for (int i = 0; i < 200; i++)
        {
            float A = (i * 2 * 3.1416) / 200;

            float x = radius * cos(A);
            float y = radius * sin(A);

            glVertex2f(x + cx, y + cy);

        }
        glEnd();
    }

    else
    {
        glColor3f(0.95, 0.95, 1.0);

        glBegin(GL_POLYGON);
        for (int i = 0; i < 200; i++)
        {
            float A = (i * 2 * 3.1416) / 200;
            float x = radius * cos(A);
            float y = radius * sin(A);

            glVertex2f(x + cx, y + cy);
        }
        glEnd();

        // shadow to make crescent effect
        glColor3f(0.036, 0.1, 0.276); // same as night sky

        glBegin(GL_POLYGON);
        for (int i = 0; i < 200; i++)
        {
            float A = (i * 2 * 3.1416) / 200;
            float x = radius * cos(A);
            float y = radius * sin(A);

            glVertex2f(x + 1.008*cx + 20, y + cy); // shift right for crescent

        }
        glEnd();
    }
}

void drawCloud(float x, float y)
{
    glColor3f(1.0, 1.0, 1.0);

    for (int i = 0; i < 3; i++)
    {
        glBegin(GL_POLYGON);
        for (int j = 0; j < 50; j++)
        {
            float angle = 2 * 3.1416 * j / 50;
            float cx = x + i * 20 + cos(angle) * 15;
            float cy = y + sin(angle) * 12;
            glVertex2f(cx, cy);
        }
        glEnd();
    }
}

void drawBird(float x, float y)
{
    glColor3f(0.1, 0.1, 0.1);
    glLineWidth(2);

    glBegin(GL_LINES);

    // simple "V" shape bird
    glVertex2f(x, y);
    glVertex2f(x - 10, y - 5);

    glVertex2f(x -10, y - 5);
    glVertex2f(x -20, y);

    glEnd();
}

 //motion of airplane
//float cycle = 0.0f;
//float cycleSpeed = 1.0f / 333.33f;  // if 10s = 10000ms then , 6000/30 = 333.33 steps per cycle

void update(int value)
{
        cycle += 0.0005f;   // slow cycle speed

if(cycle > 1.0f)
    cycle = 0.0f;

// convert cycle → night intensity
if(cycle < 0.5f)
    night = 0.0f + cycle * 2.0f;      // day → evening
else
    night = 1.0f - (cycle - 0.5f) * 2.0f;  // night → morning

// clamp
if(night < 0.0f) night = 0.0f;
if(night > 1.0f) night = 1.0f;


    // runway speed (slow)
    if (r < 20) //r = plane moving left to right
                  // f = up moving to down
    {
        r += 6.0;   // slow roll on runway here r+= speed
        f = 70;     // stay on ground
    }
    // takeoff phase (fast + rising quickly)
    else if (r < 300)
    {
        r += 2.3;   // faster acceleration
        f += 1.5;   // quick lift (takeoff)
    }
    // flying phase (steady climb)
    else
{
    //  TAKEOFF FIRST PRIORITY
    if (isTakeoff == 1)
    {
        if (f < 70)
        {
            f = 70; // ensure runway position
        }

        if (r < 300)
        {
            r += 5.0;  // speed up on runway
        }
        else
        {
            f += 2.0;  // lift off
            r += 4.0;  // forward motion
        }

        if (f >= 330)
        {
            isTakeoff = 0; // finished takeoff
        }
    }
    // LANDING SEQUENCE
    else if (isLanding == 1)
    {
        if (f > 70)
        {
            f -= 1.7;   // descend
            r += 3.0;   // slow forward movement
        }
        else
        {
            f = 70;         // touch runway exactly
            isLanding = 2;  // roll phase
            landingRoll = 15;
        }
    }

    // ROLL AFTER TOUCHDOWN
    else if (isLanding == 2)
    {
        if (landingRoll > 0)
        {
            r += 1.5;   // slow roll
            landingRoll--;
        }
        else
        {
            isLanding = 3; // stop completely
        }
    }

    // STOPPED
    else if (isLanding == 3)
    {
        // no movement at all
    }

    else
    {
        r += 4.0;

        if (f < 330)
            f += 1.5;
    }
}
    // reset after leaving screen
    if (r > 1500)
    {
        r = -200;
        f = 70;
    }

    // bird movement (left to right looping)
    // CLOUD MOVEMENT
cloudX += 0.5f;
if (cloudX > 1000) cloudX = -200;

// RAIN MOVEMENT
if (isRaining)
{
    for (int i = 0; i < 200; i++)
    {
        rainY[i] -= 6;

        if (rainY[i] < 30)   // reach runway
        {
            rainY[i] = 500;            // restart from top
            rainX[i] = rand() % 1000;  // random across screen
        }
    }
}
    birdX += 2.0;

    if (birdX > 1000)
        birdX = -100;
    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);

// TOP SKY
glColor3f(
    0.5f * (1 - night) + 0.02f * night,
    0.8f * (1 - night) + 0.02f * night,
    1.0f * (1 - night) + 0.1f * night
);

glVertex2f(0, 120);
glVertex2f(1000, 120);

// BOTTOM SKY (same original blend)
glColor3f(
    0.1f * (1 - night) + 0.0f * night,
    0.5f * (1 - night) + 0.0f * night,
    0.9f * (1 - night) + 0.2f * night
);

// extra bottom left color
glVertex2f(1000, 500);
glColor3f(
    0.2f * (1 - night),
    0.6f * (1 - night),
    0.9f * (1 - night)
);
glVertex2f(0, 500);
glEnd();



    drawGrass();

    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
    glEnd();

    drawBuilding(50, 70, 120, 0.6, 0.6, 0.7);
    drawBuilding(150, 60, 100, 0.8, 0.4, 0.5);
    drawBuilding(250, 70, 130, 0.4, 0.7, 0.8);
    drawBuilding(330, 50, 80, 0.7, 0.5, 0.9);
    drawBuilding(900, 90, 150, 0.9, 0.3, 0.5);


    drawSunMoon();

    drawCloud(cloudX + 100, 420);
drawCloud(cloudX + 400, 450);
drawCloud(cloudX + 700, 430);


    if (night<=0.5)
{
    drawBird(birdX, birdY);
    drawBird(birdX - 50, birdY + 20);
    drawBird(birdX - 100, birdY - 10);
     drawBird(birdX - 150, birdY + 15);
    drawBird(birdX - 200, birdY - 20);
}
    drawStars();
    drawWatchTower();
    drawControlTower();
    drawRunway();
    drawRunwayLights();
    drawTree(140, 130);
    drawTree(200, 130);
    drawTree(260, 130);
    drawTree(320, 130);
    drawTree(700, 130);
    drawTree(760, 130);
    drawTree(820, 130);
    drawTree(880, 130);
    drawAirplane();
    drawRain();

    glutSwapBuffers();

}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(-2, -1);
    glutInitWindowSize(1400, 780);
    glutCreateWindow("Airport Scene");

    glutTimerFunc(30, update, 0);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(Draw);

    printf("_______CONTROLS_______\n");
    //printf("N : Night\n");
    //printf("D : Day\n");
    printf("L : Land\n");
    printf("T : Takeoff\n");
    printf("R : Rain\n");
    printf("S : Stop\n");
    printf("Q : Quit\n");
    printf("______________________\n");


    for (int i = 0; i < 200; i++)
{
    rainX[i] = rand() % 1000;
    rainY[i] = rand() % 500;
}
    init();
    glutMainLoop();
    return 0;
}

