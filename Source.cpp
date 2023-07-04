#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#pragma warning(disable : 4996)

// Define the polygon vertices
GLdouble p1 = rand() % 700 + 1;
GLdouble p2 = rand() % 700 + 1;
GLdouble x1[] = { p1, p2 };
GLdouble x2[] = { p1 + 150, p2 };
GLdouble x3[] = { p1 + 150, p2 + 150 };
GLdouble x4[] = { p1, p2 + 150 };
GLint direction = 1;

GLdouble speed = 5;
GLboolean movingRight = true;
GLboolean movingTop = false;
GLint cubeSize = 150;

GLint score = 0;
GLint chancesLeft = 20;

GLboolean check = false;
GLint x, y;
GLint blink = 0;

// Regenerate a new Cube at a new position 
void generateNewCube() {

    cubeSize -= 2;
    p1 = rand() % 700 + 1;
    p2 = rand() % 700 + 1;
    x1[0] = p1;
    x1[1] = p2;
    x2[0] = p1 + cubeSize;
    x2[1] = p2;
    x3[0] = p1 + cubeSize;
    x3[1] = p2 + cubeSize;
    x4[0] = p1;
    x4[1] = p2 + cubeSize;

    direction = rand() % 2 + 1;
}

// Set up the projection and modelview matrices
void myinit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 700, 0, 700);
    glMatrixMode(GL_MODELVIEW);
}

bool checkIfInside(int x1, int y1, int x2, int y2, int x, int y) {
    if (x > x1 and x < x2 and y > y1 and y < y2)
        return true;
    return false;
}

void mouse(int button, int state, int mx, int my) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        check = true;
        x = mx;
        y = 700 - my;

        GLint highScore;
        std::ifstream file("data.txt");
        if (file.is_open()) {
            file >> highScore;
            file.close();
        }
        else {
            printf("Failed to open the file.\n");
            return;
        }

        printf("%d, %d\n", x, y);
        if (checkIfInside(x1[0], x1[1], x3[0], x3[1], x, y)) {

            score++;
            if (score > highScore) {
                highScore = score;
                std::ofstream outFile("data.txt");
                if (outFile.is_open()) {
                    outFile << highScore;
                    outFile.close();
                    printf("Value updated and stored successfully.\n");
                }
                else {
                    printf("Failed to open the file.\n");
                    return;
                }
            }
            printf("Catched!!\n");
            if (score % 10 == 0) {
                speed += 1;
            }
            generateNewCube();
        }
        else {
            chancesLeft--;
            printf("Missed\n");
            if (chancesLeft == 0) {
                exit(0);
            }
        }
    }
    glutPostRedisplay();
}

// Set up a timer to control the movement speed
void timerFunc(int value) {
    if (direction == 1) {
        if (movingRight) {
            //Right Direction
            x1[0] += speed;
            x2[0] += speed;
            x3[0] += speed;
            x4[0] += speed;
        }
        else {
            //Left Direction
            x1[0] -= speed;
            x2[0] -= speed;
            x3[0] -= speed;
            x4[0] -= speed;
        }
    }
    else {
        if (movingTop) {
            //Top Direction
            x1[1] += speed;
            x2[1] += speed;
            x3[1] += speed;
            x4[1] += speed;
        }
        else {
            //Bottom Direction
            x1[1] -= speed;
            x2[1] -= speed;
            x3[1] -= speed;
            x4[1] -= speed;
        }
    }

    if (x1[0] > 700) {
        movingRight = false;
        generateNewCube();
    }
    if (x2[0] < 0) {
        movingRight = true;
        generateNewCube();
    }
    if (x2[1] > 700) {
        movingTop = false;
        generateNewCube();
    }
    if (x3[1] < 0) {
        movingTop = true;
        generateNewCube();
    }

    glutPostRedisplay();
    glutMouseFunc(mouse);
    glutTimerFunc(30, timerFunc, 0);
}

void dispFunc() {

    glClearColor(0.65f, 0.45f, 0.45f, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(610, 680);
    const char* text = "Score: ";
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(667, 679);
    std::string s = std::to_string(score);
    for (const char& c : s) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    GLint highScore;
    std::ifstream file("data.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    else {
        printf("Failed to open the file.\n");
        return;
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(285, 680);
    const char* text2 = "High Score: ";
    for (const char* c = text2; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(390, 679);
    std::string s1 = std::to_string(highScore);
    for (const char& c : s1) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    if (chancesLeft <= 5) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2i(10, 680);
        const char* text = "Chances Left: ";
        for (const char* c = text; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2i(129, 679);
        std::string s = std::to_string(chancesLeft);
        for (const char& c : s) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }
    }

    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2dv(x1);
    glVertex2dv(x2);
    glVertex2dv(x3);
    glVertex2dv(x4);
    glEnd();

    glFlush();
}

void menu(int option) {
    switch (option) {
    case 1:
        speed = 5;
        break;
    case 2:
        speed = 10;
        break;
    case 3:
        speed = 20;
        break;
    case 4:
        exit(0);
        break;
    default:
        break;
    }
}

void mouseClick(int button, int state, int x, int y) {

    // Check if the "Start" button was clicked
    if (x >= 275 && x <= 425 && y >= 300 && y <= 350 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        printf("Clicked: %d, %d\n", x, y);

        int m1 = glutCreateMenu(menu);
        glutAddMenuEntry("Easy", 1);
        glutAddMenuEntry("Medium", 2);
        glutAddMenuEntry("Hard", 3);
        glutCreateMenu(menu);
        glutAddSubMenu("Speed", m1);
        glutAddMenuEntry("Quit", 4);
        glutAttachMenu(GLUT_RIGHT_BUTTON);

        glutDisplayFunc(dispFunc);
        glutTimerFunc(0, timerFunc, 0);
        glutPostRedisplay();
    }

    // Check if the "Quit" button was clicked
    if (x >= 275 && x <= 425 && y >= 400 && y <= 450 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        exit(0);
    }
}

void timerFuncStartPage(int value) {

    glutPostRedisplay();
    glutTimerFunc(500, timerFuncStartPage, 0);
}

void drawStartMenu() {

    glClearColor(0.185f, 0.243f, 0.252f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //College Heading
    glColor3f(0.655f, 0.646f, 0.491f);
    glRasterPos2i(160, 650);
    std::string title2 = "RNS INSTITUTE OF TECHNOLOGY";
    for (const char& c : title2) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glColor3f(0.655f, 0.646f, 0.491f);
    glRasterPos2i(190, 600);
    std::string title3 = "Computer Graphics Mini Project";
    for (const char& c : title3) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    // Draw a title label
    glColor3f(0.655f, 0.646f, 0.491f);
    glRasterPos2i(260, 500);
    std::string title = "CATCH ME GAME";
    for (const char& c : title) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    if (blink % 2 == 0) {
        glColor3f(0.955f, 0.346f, 0.391f);
        glRasterPos2i(200, 450);
        std::string title2 = "You have 20 Chances to miss the cube";
        for (const char& c : title2) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }
    }
    blink++;


    // "Start" button
    glColor3f(0.30f, 0.81f, 0.40f);
    glRecti(275, 350, 425, 400);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(325, 370);
    std::string start_label = "Start";
    for (const char& c : start_label) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // "Quit" button
    glColor3f(0.18f, 0.559f, 0.81f);
    glRecti(275, 250, 425, 300);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(325, 270);
    std::string quit_label = "Quit";
    for (const char& c : quit_label) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    //Students details
    glColor3f(0.955f, 0.346f, 0.291f);
    glRasterPos2i(290, 150);
    std::string title4 = "Submitted by: ";
    for (const char& c : title4) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glColor3f(0.855f, 0.846f, 0.991f);
    glRasterPos2i(190, 100);
    std::string title5 = "1RN20CS036 CHATRAKI AMITH";
    for (const char& c : title5) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glColor3f(0.855f, 0.846f, 0.991f);
    glRasterPos2i(190, 70);
    std::string title6 = "1RN20CS023 ANURAG N";
    for (const char& c : title6) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glutWireTeapot(1);
    glFlush();
    glutSwapBuffers();
}

void toggleGlutWindowMaximizeBox(char* WindowTitle)
{
    long dwStyle;
    HWND hwndGlut;

    wchar_t wtext[20];
    mbstowcs(wtext, WindowTitle, strlen(WindowTitle) + 1);//Plus null

    LPCWSTR WindowName = wtext;
    hwndGlut = FindWindow(NULL, WindowName);

    dwStyle = GetWindowLong(hwndGlut, GWL_STYLE);
    dwStyle ^= WS_MAXIMIZEBOX;
    SetWindowLong(hwndGlut, GWL_STYLE, dwStyle);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(350, 50);

    char WindowTitle[] = "CATCH ME";
    glutCreateWindow(WindowTitle);
    toggleGlutWindowMaximizeBox(WindowTitle);

    myinit();

    glutDisplayFunc(drawStartMenu);
    glutMouseFunc(mouseClick);
    glutTimerFunc(0, timerFuncStartPage, 0);

    glutMainLoop();

    return 0;
}