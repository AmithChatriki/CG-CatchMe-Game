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

// Set up the projection and modelview matrices
void myinit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);
}

bool checkIfInside(int x1, int y1, int x2, int y2, int x, int y)
{
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
            printf("Missed\n");
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
    glClearColor(0, 0, 0, 0);
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
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);  // Display each character using a font
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

int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutCreateWindow("CATCH ME!");
    myinit();
    
    glutTimerFunc(0, timerFunc, 0);

    glutDisplayFunc(dispFunc);
    glutMainLoop();

    return 0;
}


