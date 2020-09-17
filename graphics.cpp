//
// Created by Matheus Jorge on 10/31/19.
//

#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "graphics.h"
#include "shapes.h"
// #include "shapez.cpp"
#include <vector>

using namespace std;

enum screen_type {start, game};
enum fractal_type {triangle, square};

screen_type screen;
fractal_type fractal;
GLdouble width, height;
Rectangle start_background;
Circle A;
Circle B;
Circle C;
Circle st;
Circle newDot;
color blk;
vector<Circle> dots;
int wd;

point loc;
int choice;
int prev_choice;

point midPoint(point a, point b) {
    point c;
    c.x = (a.x + b.x)/2;
    c.y = (a.y + b.y)/2;
    return c;
}

void init() {

    width = 1600;
    height = 800;

    /**
     * CHOOSE FRACTAL TYPE HERE
     */
    fractal = triangle;
    screen = start;

    blk.red = 1;
    blk.blue = 1;
    blk.green = 1;

    A.set_position(width/2, 10);
    A.set_radius(5);
    A.set_fill(blk);

    B.set_position(10, height - 10);
    B.set_radius(5);
    B.set_fill(blk);

    C.set_position( width - 10, height - 10);
    C.set_radius(5);
    C.set_fill(blk);

    if (fractal == square) {
        A.set_position(10, 10);
        A.set_radius(5);
        A.set_fill(blk);
    }

    st.set_position(width - 10, 10);
    st.set_radius(5);
    st.set_fill(blk);

    newDot = st;

    dots.push_back(A);
    dots.push_back(B);
    dots.push_back(C);

    if (fractal == square) {
        dots.push_back(st); // 4th big dot
        prev_choice = 0;
    }


    srand(time(NULL));
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

void display_game() {

    // delay
    //for (int i = 0; i < 1000000000; i++) {
    // nothing
    //}

    for (int i = 0; i < dots.size(); i++) {
        if (dots.size() < 50000) {
            dots[i].draw();
        }
    }


    // triangle
    choice = rand() % 3 + 1;

    //square
    if (fractal == square) {
        choice = rand() % 4 + 1;
        while (choice == prev_choice) {
            choice = rand() % 4 + 1;
        }
        prev_choice = choice;
    }



    if (choice == 1) { // A
        loc = midPoint(newDot.get_position(), A.get_position());
    }
    else if (choice == 2) { // B
        loc = midPoint(newDot.get_position(), B.get_position());
    }
    else if (choice == 3){ // C
        loc = midPoint(newDot.get_position(), C.get_position());
    }
    else { // st
        loc = midPoint(newDot.get_position(), st.get_position());
    }

    newDot = Circle(1, blk, loc);
    dots.push_back(newDot);

}


/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /*
     * Draw here
     */

    display_game();

    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {

    switch(key) {
        case 27: // escape
            glutDestroyWindow(wd);
            cout << "good-bye" << endl;
            exit(0);
            break;
    }

    glutPostRedisplay();

    return;
}
//int[] = [2, 3, 5, 8, 13, 21, 34, 55, 89, -89, -55, -34, -21, -13, -8, -2];
void kbdS(int key, int x, int y) {
    if (screen == game) {
        // move Rectangle r1 with the arrow keys

    }
    glutPostRedisplay();

    return;
}

void cursor(int x, int y) {

}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {

    if (screen == game) {
        // when I click down on the left button, move Rectangle r1
        // to the mouse coordinates
        /*
        if (button == 3) { // scroll up
        }
        else if (button == 4) { // scroll down
        }
        */

        // when I click down on the right button, toggle the
        // color of the Rectangle r1 between yellow and green
        //if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        //displaySinfo = true;
        //}
    }

    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        screen = game;
    }
    glutPostRedisplay();
}

void timer(int extra) {

    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

/* Main function: GLUT runs as a console application starting at main()  */
int graphicsPlay(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(300, 300); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Drawing Fun!" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}