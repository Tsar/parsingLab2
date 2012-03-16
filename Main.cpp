#include <fstream>
#include <GL/glut.h>

#include "Parser.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

std::ifstream inputFile("tests.txt");
Parser p;
int window;
Tree* treeToDraw = 0;

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

void drawTree(Tree* t, double rX1, double rY1, double rX2, double rY2) {
    int h = t->getHeight();
    double cellH = (rY2 - rY1) / h;
    
    //drawing children
    int childrenNum = t->getChildren().size();
    double subCellW = (rX2 - rX1) / childrenNum;
    for (size_t i = 0; i < childrenNum; ++i) {
        //drawing edges to children
        glColor3d(1, 0, 0);
        glBegin(GL_LINES);
        glVertex2d((rX1 + rX2) / 2, rY1 + cellH / 2);
        glVertex2d(rX1 + subCellW * (i + 0.5), rY1 + cellH * 1.5);
        glEnd();
        
        drawTree(t->getChildren()[i], rX1 + subCellW * i, rY1 + cellH, rX1 + subCellW * (i + 1), rY2);
    }

    //drawing self
    glColor3d(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    glVertex2d((rX1 + rX2) / 2 - 0.07, rY1 + cellH / 2 - 0.07);
    glVertex2d((rX1 + rX2) / 2 + 0.07, rY1 + cellH / 2 - 0.07);
    glVertex2d((rX1 + rX2) / 2 + 0.07, rY1 + cellH / 2 + 0.07);
    glVertex2d((rX1 + rX2) / 2 - 0.07, rY1 + cellH / 2 + 0.07);
    glEnd();
    
    glColor3d(1, 1, 1);
    drawTextXY((rX1 + rX2) / 2 - 0.05, rY1 + cellH / 2, GLUT_BITMAP_HELVETICA_12, t->getNode());
}

void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (treeToDraw) {
        drawTree(treeToDraw, -1.0, -1.0, 1.0, 1.0);
    }
    
    glFlush();
}

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            haltProgram();
        default:
            if (inputFile.eof())
                haltProgram();
            std::string s;
            while (s.empty()) {
                getline(inputFile, s);
                if (inputFile.eof())
                    haltProgram();
            }
            if (treeToDraw)
                delete treeToDraw;
            treeToDraw = p.parse(s);
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 50);
    window = glutCreateWindow("Parse Tree Visualizer");
    
    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);
    
    glClearColor(0, 0, 0, 0);
    glutMainLoop();

    return 0;
}
