#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "display.h"
#include "io.h"
#include "definitions.h"
#include "transformaciones.h"
#include "camara.h"
#include "iluminacion.h"
#include "load_obj.h"

/** GLOBAL VARIABLES **/

object3d * _first_object = 0;                /*List of objects*/
object3d * _selected_object = 0;            /*Object currently selected*/
object3d * cam_object = 0;
object3d * light_object = 0;

camera * _first_camera = 0;                 /*List of cameras*/
camera * _selected_camera = 0;              /*Camera currently selected*/
camera * _object_camera = 0;                /*Special object camera*/

objetos_luz global_lights[8];
int _selected_light = 0;
int luz = ACTIVADA;


material_light *ruby, *obsidian, *gold, *mat_camara, *mat_selec, *red_plastic, *mat_foco, *mat_bombilla;

int modo = TRASLACION;
int sis_referencia = LOCALES;
int elemento = OBJETO;
int modo_camara = VUELO;

/** GENERAL INITIALIZATION **/
void initialization (){

    /*Definition of the background color*/
    glClearColor(KG_COL_BACK_R, KG_COL_BACK_G, KG_COL_BACK_B, KG_COL_BACK_A);


}


/** MAIN FUNCTION **/
int main(int argc, char** argv) {

    /*First of all, print the help information*/
    print_help();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(KG_WINDOW_WIDTH, KG_WINDOW_HEIGHT);
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);

    /* set the callback functions */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(esp_keyboard);

    /*Definition of the method to draw the objects*/
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_NORMALIZE);
    //if(shade == SMOOTH){
    //glShadeModel(GL_SMOOTH);
    /*}else if(shade == FLAT){
        glShadeModel(GL_FLAT);
    }*/

    /* this initialization has to be AFTER the creation of the window */
    initialization();
    set_transformaciones();
    load_generic_representation();
    default_cameras();

    inicializar_luces();
    inicializar_materiales();

    /* start the main loop */
    glutMainLoop();
    return 0;
}
