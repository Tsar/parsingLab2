#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glut.h>

#include "Parser.h"

#define DEFAULT_WINDOW_SIZE_X 800
#define DEFAULT_WINDOW_SIZE_Y 600
#define DEFAULT_WINDOW_POS_X  100
#define DEFAULT_WINDOW_POS_Y   50

//tree element half width/height
#define TEHW 0.04
#define TEHH 0.04

std::ifstream inputFile("tests.txt");
Parser p;
int window;
Tree* treeToDraw = 0;
bool fullscreenMode = false;
bool testingFinished = false;
bool currentlyParseException = false;
ParseException parseException("", 0);
std::string currentTest;

void haltProgram() {
    inputFile.close();
    glutDestroyWindow(window);
    exit(0);
}

void drawTextXY(double x, double y, void* font, std::string const& text) {
    glRasterPos2d(x, y);
    for (size_t i = 0; i < text.length(); ++i) {
        glutBitmapCharacter(font, text[i]);
    }
}

void drawTree(Tree* t, double rX1, double rY1, double rX2, double rY2, double cellH) {
    int h = t->getHeight();
    
    //drawing children
    size_t childrenNum = t->getChildren().size();
    double subCellW = (rX2 - rX1) / childrenNum;
    for (size_t i = 0; i < childrenNum; ++i) {
        //drawing edges to children
        glColor3d(1, 0, 0);
        glBegin(GL_LINES);
        glVertex2d((rX1 + rX2) / 2, rY1 + cellH / 2);
        glVertex2d(rX1 + subCellW * (i + 0.5), rY1 + cellH * 1.5);
        glEnd();
        
        drawTree(t->getChildren()[i], rX1 + subCellW * i, rY1 + cellH, rX1 + subCellW * (i + 1), rY2, cellH);
    }

    //drawing self
    glColor3d(0, 0, 0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2d((rX1 + rX2) / 2 - TEHW, rY1 + cellH / 2 - TEHH);
    glVertex2d((rX1 + rX2) / 2 + TEHW, rY1 + cellH / 2 - TEHH);
    glVertex2d((rX1 + rX2) / 2 - TEHW, rY1 + cellH / 2 + TEHH);
    glVertex2d((rX1 + rX2) / 2 + TEHW, rY1 + cellH / 2 + TEHH);
    glEnd();

    glColor3d(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    glVertex2d((rX1 + rX2) / 2 - TEHW, rY1 + cellH / 2 - TEHH);
    glVertex2d((rX1 + rX2) / 2 + TEHW, rY1 + cellH / 2 - TEHH);
    glVertex2d((rX1 + rX2) / 2 + TEHW, rY1 + cellH / 2 + TEHH);
    glVertex2d((rX1 + rX2) / 2 - TEHW, rY1 + cellH / 2 + TEHH);
    glVertex2d((rX1 + rX2) / 2 - TEHW, rY1 + cellH / 2 - TEHH);
    glEnd();
    
    glColor3d(1, 1, 1);
    drawTextXY((rX1 + rX2) / 2 - 0.02, rY1 + cellH / 2 - 0.01, GLUT_BITMAP_HELVETICA_12, t->getNode());
}

void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (!testingFinished) {
        if (treeToDraw) {
            drawTree(treeToDraw, -1.0, 1.0, 1.0, -1.0, -2.0 / treeToDraw->getHeight());
        }
        
        if (currentlyParseException) {
            glColor3d(1, 1, 0);
            drawTextXY(-0.15, 0, GLUT_BITMAP_HELVETICA_12, "ParseException:");
            glColor3d(1, 1, 1);
            std::ostringstream oss;
            oss << parseException.getMessage() << " " << parseException.getErrorOffset();
            drawTextXY(-0.15, -0.05, GLUT_BITMAP_HELVETICA_12, oss.str());
        }

        glColor3d(1, 1, 0);
        drawTextXY(-0.90, 0.70, GLUT_BITMAP_HELVETICA_12, "Current test: " + currentTest);

        glColor3d(1, 1, 1);
        drawTextXY(-0.90, 0.90, GLUT_BITMAP_HELVETICA_12, "Esc or Q - quit");
        drawTextXY(-0.90, 0.85, GLUT_BITMAP_HELVETICA_12, "F - toggle fullscreen");
        drawTextXY(-0.90, 0.80, GLUT_BITMAP_HELVETICA_12, "Other key - parse next test");
    } else {
        glColor3d(1, 1, 1);
        drawTextXY(-0.90, 0.85, GLUT_BITMAP_HELVETICA_12, "F - toggle fullscreen");
        drawTextXY(-0.90, 0.80, GLUT_BITMAP_HELVETICA_12, "Other key - quit");

        glColor3d(1, 1, 0);
        drawTextXY(-0.1, 0, GLUT_BITMAP_HELVETICA_12, "ALL TESTS PASSED");
    }
    
    glFlush();
}

void buildNextTree() {
    if (inputFile.eof()) {
        testingFinished = true;
        return;
    }
    currentTest = "";
    while (currentTest.empty()) {
        getline(inputFile, currentTest);
        if (inputFile.eof()) {
            testingFinished = true;
            return;
        }
    }
    if (treeToDraw)
        delete treeToDraw;
    try {
        treeToDraw = p.parse(currentTest);
        currentlyParseException = false;
    } catch (ParseException const& pe) {
        treeToDraw = 0;
        currentlyParseException = true;
        parseException = pe;
    }
}

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
        case 'q':
        case 'Q':
            haltProgram();
        case 'f':
        case 'F':
            fullscreenMode = !fullscreenMode;
            if (fullscreenMode) {
                glutFullScreen();
            } else {
                glutReshapeWindow(DEFAULT_WINDOW_SIZE_X, DEFAULT_WINDOW_SIZE_Y);
                glutPositionWindow(DEFAULT_WINDOW_POS_X, DEFAULT_WINDOW_POS_Y);
            }
            break;
        default:
            if (testingFinished)
                haltProgram();
            buildNextTree();
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(DEFAULT_WINDOW_SIZE_X, DEFAULT_WINDOW_SIZE_Y);
    glutInitWindowPosition(DEFAULT_WINDOW_POS_X, DEFAULT_WINDOW_POS_Y);
    window = glutCreateWindow("Parse Tree Visualizer");
    
    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);
    
    glClearColor(0, 0, 0, 0);

    buildNextTree();

    glutMainLoop();

    return 0;
}
