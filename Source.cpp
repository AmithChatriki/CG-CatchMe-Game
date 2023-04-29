#include <stdio.h>
#include <string>
#include <cstring>
#include <GL/glut.h>

// Define the polygon vertices
GLdouble p1 = rand() % 500 + 1;
GLdouble p2 = rand() % 500 + 1;
GLdouble x1[] = { p1, p2 };
GLdouble x2[] = { p1+50, p2 };
GLdouble x3[] = { p1+50, p2+50 };
GLdouble x4[] = { p1, p2+50 };

// Regenerate a new Cube at a new position 
void generateNewCube() {

    p1 = rand() % 500 + 1;
    p2 = rand() % 500 + 1;
    x1[0] = p1;
    x1[1] = p2;
    x2[0] = p1+50;
    x2[1] = p2;
    x3[0] = p1+50;
    x3[1] = p2+50;
    x4[0] = p1;
    x4[1] = p2+50;
}

GLint score = 0;
GLint chancesLeft = 20;

// Set up the projection and modelview matrices
void myinit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);
}

bool checkIfInside(int x1, int y1, int x2, int y2, int x, int y) {
    if (x > x1 and x < x2 and y > y1 and y < y2)
        return true;
    return false;
}

bool check = false;
int x, y;

void mouse(int button, int state, int mx, int my) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        check = true;
        x = mx;
        y = 500 - my;
        printf("%d, %d\n", x, y);
        if (checkIfInside(x1[0], x1[1], x3[0], x3[1], x, y)) {
            score++;
            printf("Catched!!\n");
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
GLdouble speed = 5;
GLint direction = 1;
GLboolean movingRight = true;
GLboolean movingTop = false;

void timerFunc(int value) {
    if (!direction) {
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

    if (x1[0] > 500) {
        movingRight = false;
        generateNewCube();
    }
    if (x2[0] < 0) {
        movingRight = true;
        generateNewCube();
    }
    if (x2[1] > 500) {
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
    glRasterPos2i(410, 480);
    const char* text = "Score: ";
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(467, 479);
    std::string s = std::to_string(score);
    for (const char& c : s) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    if (chancesLeft <= 5) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2i(10, 480);
        const char* text = "Chances Left: ";
        for (const char* c = text; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2i(129, 479);
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
    if (x >= 175 && x <= 325 && y >= 200 && y <= 250 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
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
    if (x >= 175 && x <= 325 && y >= 300 && y <= 350 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        exit(0);
    }
}

void timerFuncStartPage(int value) {
    glutPostRedisplay();
    glutTimerFunc(500, timerFuncStartPage, 0);
}

int blink = 0;

void drawStartMenu() {

    glClearColor(0.185f, 0.243f, 0.252f, 0.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a title label
    glColor3f(0.655f, 0.646f, 0.491f);
    glRasterPos2i(160, 450);
    std::string title = "CATCH ME GAME";
    for (const char& c : title) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    if (blink % 2 == 0) {
        glColor3f(0.955f, 0.346f, 0.391f);
        glRasterPos2i(100, 420);
        std::string title2 = "You have 20 Chances to miss the cube";
        for (const char& c : title2) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }
    }
    blink++;
    

    // Draw a "Start" button
    glColor3f(0.30f, 0.81f, 0.40f);
    glRecti(175, 250, 325, 300);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(225, 270);
    std::string start_label = "Start";
    for (const char& c : start_label) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Draw a "Quit" button
    glColor3f(0.18f, 0.559f, 0.81f);
    glRecti(175, 150, 325, 200);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(225, 170);
    std::string quit_label = "Quit";
    for (const char& c : quit_label) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glutWireTeapot(1);
    glFlush();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutCreateWindow("CATCH ME!");
    myinit();
    
    glutDisplayFunc(drawStartMenu);
    glutMouseFunc(mouseClick);
    glutTimerFunc(0, timerFuncStartPage, 0);

    glutMainLoop();

    return 0;
}


