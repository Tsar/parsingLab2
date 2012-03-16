#include <fstream>
#include <GL/glut.h>

#include "Parser.h"

std::ifstream inputFile("tests.txt");
Parser p;
int window;
Tree* treeToDraw = 0;

void haltProgram() {
    inputFile.close();
    glutDestroyWin(window);
    exit(0);
}

void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (treeToDraw) {
        //TODO: implement tree drawing
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
            getline(inputFile, s);
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
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 50);
    window = glutCreateWindow("Parse Tree Visualizer");
    
    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);
    
    glClearColor(0, 0, 0, 0);
    glutMainLoop();

    return 0;
}
