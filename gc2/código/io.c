#include "definitions.h"
#include "load_obj.h"
#include <GL/glut.h>
#include <stdio.h>
#include "transformaciones.h"
#include "camara.h"
#include "iluminacion.h"


extern object3d * _first_object;
extern object3d * _selected_object;

extern camera  * _first_camera;
extern camera * _selected_camera;
extern camera * _object_camera;

extern int modo;
extern int sis_referencia;
extern int elemento;
extern int modo_camara;
extern int luz;


extern vector3 *up_traslacion;
extern vector3 *up_rotacion;
extern vector3 *up_escalado;

extern vector3 *down_traslacion;
extern vector3 *down_rotacion;
extern vector3 *down_escalado;

extern vector3 *right_traslacion;
extern vector3 *right_rotacion;
extern vector3 *right_escalado;

extern vector3 *left_traslacion;
extern vector3 *left_rotacion;
extern vector3 *left_escalado;

extern vector3 *avpag_traslacion;
extern vector3 *avpag_rotacion;
extern vector3 *avpag_escalado;

extern vector3 *repag_traslacion;
extern vector3 *repag_rotacion;
extern vector3 *repag_escalado;

extern vector3 *menos_escalado;

extern vector3 *mas_escalado;

extern objetos_luz global_lights[8];
extern int _selected_light;
extern int luz;

extern material_light *ruby, *obsidian;

/**
 * @brief This function just prints information about the use
 * of the keys
 */
void print_help(){
    printf("Práctica de la asignatura GC. Este programa visualiza y modifica \n");
    printf("objetos 3D.  \n\n");
    printf("\n\n");
    printf("FUNCIONES PRINCIPALES \n");
    printf("<?>\t\t Visualizar ayuda. \n");
    printf("<ESC>\t\t Salir del programa. \n");
    printf("<F,f>\t\t Cargar un objeto.\n");
    printf("<TAB>\t\t Elegir un objeto entre los cargados.\n");
    printf("<DEL>\t\t Borrar el objeto seleccionado.\n");
    printf("<K>\t\t Visualizar punto de vista de los objetos (Si vuelves a pulsar K, o cambias de elemento, saldrás de este modo).\n");
    printf("<k>\t\t Cambiar de cámara.\n");
    printf("<n>\t\t Añadir nueva cámara.\n");
    printf("<P,p>\t\t Cambio de tipo de proyección: perspectiva (por defecto)/ paralela.\n");
    printf("<M,m>\t\t Cambio de material del objeto.\n");
    printf("\n\n");
    printf("FUNCIONES DE ILUMINACIÓN \n");
    printf("<f9>\t\t Activar/Desactivar iluminación. \n");
    printf("<f1-f4>\t\t Encender/Apagar fuente de luz correspondiente del. Bombilla, sol, foco del objeto seleccionado y foco de la cámara. \n");
    printf("<f5-f8>\t\t Encender/Apagar fuente de luz correspondiente del. \n");
    printf("<1-8>\t\t Seleccionar la fuente de luz correspondiente. \n");
    printf("<0>\t\t Asignar tipo de fuente de luz a fuente de 5-8 seleccionada. \n");
    printf("<f12>\t\t Cambiar tipo de iluminación del objeto seleccionado. \n");
    printf("\n\n");
    printf("MODOS DE TRANSFORMACIÓN \n");
    printf("<T,t>\t\t Modo de traslación (modo por defecto).\n");
    printf("<R,r>\t\t Modo de rotación.\n");
    printf("<E,e>\t\t Modo de escalado / cambio de volumen.\n");
    printf("\n\n");
    printf("SISTEMA DE REFERENCIA (EN MODO OBJETO) O MODO DE CÁMARA (EN MODO CÁMARA). \n");
    printf("<G,g>\t\t Transformaciones globales o modo análisis.\n");
    printf("<L,l>\t\t Transformaciones locales o modo vuelo(por defecto).\n");
    printf("\n\n");
    printf("ELEMENTO A TRANSFORMAR \n");
    printf("<O,o>\t\t Transformaciones a objeto (modo por defecto).\n");
    printf("<C,c>\t\t Transformaciones a cámara.\n");
    printf("<A,a>\t\t Transformaciones a iluminación.\n");
    printf("\n\n");
    printf("MOVIMIENTOS POSIBLES \n");
    printf("<ARRIBA>\n");
    printf("<ABAJO>\n");
    printf("<IZQUIERDA>\n");
    printf("<DERECHA>\n");
    printf("<AVPAG>\n");
    printf("<REPAG>\n");
    printf("<+> (SOLO EN MODO ESCALADO/CAMBIO DE VOLUMEN/SI LA FUENTE DE LUZ ES UN FOCO)\n");
    printf("<-> (SOLO EN MODO ESCALADO/CAMBIO DE VOLUMEN/SI LA FUENTE DE LUZ ES UN FOCO)\n");
}

/**
*Function to actually free the space of 
*a 3dObject
*/
void better_free(object3d *tofree){
    vertex *vfree=tofree->vertex_table;
    free(vfree);
    GLint i;
    face *ftbl=tofree->face_table;
    for(i=0;i<tofree->num_faces;i++){
        face *facetofree=ftbl+i;
        free(facetofree->vertex_table);
    }
    free(tofree->matrix_table);
    free(ftbl);
    free(tofree);
}


void identity(GLfloat *m){
    int i;
    for(i=0;i<16;i++){
        if(i%5==0){//si es multiplo de 5, en este caso, hay que poner uno para que sea identidad
            m[i]=1.0f;
        }else{
            m[i]=0.0f;
        }
    }
}

/**
 * @brief Callback function to control the especial keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */
void esp_keyboard(int key, int x, int y){
    vector3 *camara;

    glMatrixMode(GL_MODELVIEW);
    if(_selected_object!=0) {
        switch (key) {

            case GLUT_KEY_UP:
                if ((elemento == OBJETO || elemento == OBJETOCAMARA)) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(up_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(up_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(up_escalado);
                    }
                }else if(elemento == CAMARA){
                    if (modo == TRASLACION){
                        if(modo_camara == VUELO){
                            modo = ROTACION;
                            aplicar_transformaciones(up_rotacion);
                            modo = TRASLACION;
                        }else{
                            modo_analisis(1,0);
                        }

                    }else if(modo == ROTACION){
                        if(modo_camara == VUELO){
                            aplicar_transformaciones(up_rotacion);
                        }else{
                            modo_analisis(1,0);
                        }
                    }else if(modo == ESCALADO){
                        _selected_camera->proj->top -= 0.01;
                        _selected_camera->proj->bottom += 0.01;
                    }
                }else if (elemento == ILUMINACION){
                    switch (modo){

                        case TRASLACION:
                            if (global_lights[_selected_light].type == BOMBILLA ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(up_traslacion);
                            break;

                        case ROTACION:
                            if (global_lights[_selected_light].type == SOL ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(up_rotacion);
                            break;
                    }
                }
                break;
            case GLUT_KEY_DOWN:
                if ((elemento == OBJETO || elemento == OBJETOCAMARA)) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(down_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(down_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(down_escalado);
                    }
                }else if(elemento == CAMARA){
                    if (modo == TRASLACION){
                        if(modo_camara == VUELO){
                            modo = ROTACION;
                            aplicar_transformaciones(down_rotacion);
                            modo = TRASLACION;
                        }else{
                            modo_analisis(-1,0);
                        }

                    }else if(modo == ROTACION){
                        if(modo_camara == VUELO){
                            aplicar_transformaciones(down_rotacion);
                        }else{
                            modo_analisis(-1,0);
                        }
                    }else if(modo == ESCALADO){
                        _selected_camera->proj->top += 0.01;
                        _selected_camera->proj->bottom -= 0.01;
                    }
                }else if (elemento == ILUMINACION){
                    switch (modo){

                        case TRASLACION:
                            if (global_lights[_selected_light].type == BOMBILLA ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(down_traslacion);
                            break;

                        case ROTACION:
                            if (global_lights[_selected_light].type == SOL ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(down_rotacion);
                            break;
                    }
                }
                break;
            case GLUT_KEY_LEFT:
                if ((elemento == OBJETO || elemento == OBJETOCAMARA)) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(left_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(left_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(left_escalado);
                    }
                }else if(elemento == CAMARA){
                    if (modo == TRASLACION){
                        if(modo_camara == VUELO){
                            modo = ROTACION;
                            aplicar_transformaciones(right_rotacion);
                            modo = TRASLACION;
                        }else{
                            modo_analisis(0,1);
                        }

                    }else if(modo == ROTACION){
                        if(modo_camara == VUELO){
                            aplicar_transformaciones(right_rotacion);
                        }else{
                            modo_analisis(0,1);
                        }
                    }else if(modo == ESCALADO){
                        _selected_camera->proj->left += 0.01;
                        _selected_camera->proj->right -= 0.01;
                    }
                }else if (elemento == ILUMINACION){
                    switch (modo){

                        case TRASLACION:
                            if (global_lights[_selected_light].type == BOMBILLA ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(left_traslacion);
                            break;

                        case ROTACION:
                            if (global_lights[_selected_light].type == SOL ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(left_rotacion);
                            break;
                    }
                }
                break;
            case GLUT_KEY_RIGHT:
                if ((elemento == OBJETO || elemento == OBJETOCAMARA)) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(right_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(right_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(right_escalado);
                    }
                }else if(elemento == CAMARA){
                    if (modo == TRASLACION){
                        if(modo_camara == VUELO){
                            modo = ROTACION;
                            aplicar_transformaciones(left_rotacion);
                            modo = TRASLACION;
                        }else{
                            modo_analisis(0,-1);
                        }

                    }else if(modo == ROTACION){
                        if(modo_camara == VUELO){
                            aplicar_transformaciones(left_rotacion);
                        }else{
                            modo_analisis(0,-1);
                        }
                    }else if(modo == ESCALADO){
                        _selected_camera->proj->left -= 0.01;
                        _selected_camera->proj->right += 0.01;
                    }
                }else if (elemento == ILUMINACION){
                    switch (modo){

                        case TRASLACION:
                            if (global_lights[_selected_light].type == BOMBILLA ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(right_traslacion);
                            break;

                        case ROTACION:
                            if (global_lights[_selected_light].type == SOL ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(right_rotacion);
                            break;
                    }
                }
                break;
            case GLUT_KEY_PAGE_UP:
                if ((elemento == OBJETO || elemento == OBJETOCAMARA)) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(repag_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(repag_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(repag_escalado);
                    }
                }else if(elemento == CAMARA){
                    if (modo == TRASLACION){
                        if (modo_camara == ANALISIS){
                            if (distancia_camara_objeto() > 1.0){
                                /*camara = (vector3 *)malloc(sizeof(vector3));
                                *camara = (vector3){
                                        .x = -_selected_camera->minv[8],
                                        .y = -_selected_camera->minv[9],
                                        .z = -_selected_camera->minv[10]
                                };*/
                                aplicar_transformaciones(repag_traslacion);
                            }
                        } else {
                            aplicar_transformaciones(repag_traslacion);
                        }

                    }else if(modo == ROTACION){
                        if (modo_camara == VUELO){
                            aplicar_transformaciones(repag_rotacion);
                        }else{
                            modo_camara = VUELO;
                            aplicar_transformaciones(repag_rotacion);
                            modo_camara = ANALISIS;
                        }
                    }else if(modo == ESCALADO){
                        _selected_camera->proj->near -= 0.01;
                        _selected_camera->proj->far -= 0.01;

                    }
                }else if (elemento == ILUMINACION){
                    switch (modo){

                        case TRASLACION:
                            if (global_lights[_selected_light].type == BOMBILLA ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(repag_traslacion);
                            break;

                        case ROTACION:
                            if (global_lights[_selected_light].type == SOL ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(repag_rotacion);
                            break;
                    }
                }
                break;
            case GLUT_KEY_PAGE_DOWN:
                if ((elemento == OBJETO || elemento == OBJETOCAMARA)) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(avpag_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(avpag_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(avpag_escalado);
                    }
                }else if(elemento == CAMARA){
                    if (modo == TRASLACION){
                        if (modo_camara == ANALISIS){
                            if (distancia_camara_objeto() < 1000){
                                /*camara = (vector3 *)malloc(sizeof(vector3));
                                *camara = (vector3){
                                        .x = _selected_camera->minv[8],
                                        .y = _selected_camera->minv[9],
                                        .z = _selected_camera->minv[10]
                                };*/
                                aplicar_transformaciones(avpag_traslacion);
                            }
                        } else {
                            aplicar_transformaciones(avpag_traslacion);
                        }

                    }else if(modo == ROTACION){
                        if (modo_camara == VUELO){
                            aplicar_transformaciones(avpag_rotacion);
                        }else{
                            modo_camara = VUELO;
                            aplicar_transformaciones(avpag_rotacion);
                            modo_camara = ANALISIS;
                        }
                    }else if(modo == ESCALADO){
                        _selected_camera->proj->near += 0.01;
                        _selected_camera->proj->far += 0.01;
                    }
                }else if (elemento == ILUMINACION){
                    switch (modo){

                        case TRASLACION:
                            if (global_lights[_selected_light].type == BOMBILLA ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(repag_traslacion);
                            break;

                        case ROTACION:
                            if (global_lights[_selected_light].type == SOL ||
                                global_lights[_selected_light].type == FOCO)
                                aplicar_transformaciones(repag_rotacion);
                            break;
                    }
                }
                break;

            case GLUT_KEY_F1:
                if(luz==ACTIVADA) {
                    switch (global_lights[0].is_on) {
                        case 0:
                            global_lights[0].is_on = 1;
                            glEnable(GL_LIGHT0);
                            printf("BOMBILLA encendida\n");
                            break;
                        case 1:
                            global_lights[0].is_on = 0;
                            glDisable(GL_LIGHT0);
                            printf("BOMBILLA apagada\n");
                            break;
                    }
                }
                break;

            case GLUT_KEY_F2:
                if(luz==ACTIVADA) {
                    switch (global_lights[1].is_on) {
                        case 0:
                            global_lights[1].is_on = 1;
                            glEnable(GL_LIGHT1);
                            printf("SOL encendido\n");
                            break;
                        case 1:
                            global_lights[1].is_on = 0;
                            glDisable(GL_LIGHT1);
                            printf("SOL apagado\n");
                            break;
                    }
                }else{
                    printf("primero activa la iluminación\n");
                }
                break;

            case GLUT_KEY_F3:
                if(luz == ACTIVADA) {
                    switch (global_lights[2].is_on) {
                        case 0:
                            global_lights[2].is_on = 1;
                            glEnable(GL_LIGHT2);
                            printf("FOCO-OBJETO encendido\n");
                            break;
                        case 1:
                            global_lights[2].is_on = 0;
                            glDisable(GL_LIGHT2);
                            printf("FOCO-OBJETO apagado\n");
                            break;
                    }
                }else{
                    printf("primero activa la iluminación\n");
                }
                break;

            case GLUT_KEY_F4:
                if(luz==ACTIVADA) {
                    switch (global_lights[3].is_on) {
                        case 0:
                            global_lights[3].is_on = 1;
                            glEnable(GL_LIGHT3);
                            printf("FOCO-CÁMARA encendido\n");
                            break;
                        case 1:
                            global_lights[3].is_on = 0;
                            glDisable(GL_LIGHT3);
                            printf("FOCO-CÁMARA apagado\n");
                            break;
                    }
                }else{
                    printf("primero activa la iluminación\n");
                }
                break;

            case GLUT_KEY_F5:
                if(luz==ACTIVADA) {
                    if (global_lights[4].type != NONE) {
                        switch (global_lights[4].is_on) {
                            case 0:
                                global_lights[4].is_on = 1;
                                glEnable(GL_LIGHT4);
                                printf("luz 5 encendida\n");
                                break;
                            case 1:
                                global_lights[4].is_on = 0;
                                glDisable(GL_LIGHT4);
                                printf("luz 5 apagada\n");
                                break;
                        }
                    } else {
                        printf("primero incializa la luz 5\n");
                    }
                }else{
                    printf("primero activa la iluminación\n");
                }
                break;

            case GLUT_KEY_F6:
                if(luz == ACTIVADA) {
                    if (global_lights[5].type != NONE) {
                        switch (global_lights[5].is_on) {
                            case 0:
                                global_lights[5].is_on = 1;
                                glEnable(GL_LIGHT5);
                                printf("luz 6 encendida\n");
                                break;
                            case 1:
                                global_lights[5].is_on = 0;
                                glDisable(GL_LIGHT5);
                                printf("luz 6 apagada\n");
                                break;
                        }
                    } else {
                        printf("primero incializa la luz 6\n");
                    }
                }else{
                    printf("primero activa la iluminación\n");
                }
                break;

            case GLUT_KEY_F7:
                if(luz == ACTIVADA) {
                    if (global_lights[6].type != NONE) {
                        switch (global_lights[6].is_on) {
                            case 0:
                                global_lights[6].is_on = 1;
                                glEnable(GL_LIGHT6);
                                printf("luz 7 encendida\n");
                                break;
                            case 1:
                                global_lights[6].is_on = 0;
                                glDisable(GL_LIGHT6);
                                printf("luz 7 apagada\n");
                                break;
                        }
                    } else {
                        printf("primero incializa la luz 7\n");
                    }
                }else{
                    printf("primero activa la iluminación\n");
                }
                break;

            case GLUT_KEY_F8:
                if(luz == ACTIVADA) {
                    if (global_lights[7].type != NONE) {
                        switch (global_lights[7].is_on) {
                            case 0:
                                global_lights[7].is_on = 1;
                                glEnable(GL_LIGHT7);
                                printf("luz 8 encendida\n");
                                break;
                            case 1:
                                global_lights[7].is_on = 0;
                                glDisable(GL_LIGHT7);
                                printf("luz 8 apagada\n");
                                break;
                        }
                    } else {
                        printf("primero incializa la luz 8\n");
                    }
                }else{
                    printf("primero activa la iluminación\n");
                }
                break;

            case GLUT_KEY_F9:
                if(luz == DESACTIVADA){
                    luz = ACTIVADA;
                    glEnable(GL_LIGHTING);
                    printf("Iluminación activada\n");
                }else{
                    luz = DESACTIVADA;
                    glDisable(GL_LIGHTING);
                    printf("Iluminación desactivada\n");
                    if(elemento == ILUMINACION){
                        elemento = OBJETO;
                        printf("Elemento cambiado a objeto debido a que la iluminación ha sido desactivada\n");
                    }
                }
                break;

            case GLUT_KEY_F12:
                if(luz == ACTIVADA) {
                    _selected_object->shade = (_selected_object->shade + 1) % 2;
                }
                break;

        }
        /*Si el objeto es la cámara*/
        if(elemento == OBJETOCAMARA){
            add_camera_mode_obj(_selected_object);
            m_foco(2);
            m_foco(3);
        }else if(elemento == CAMARA){
            m_foco(3);
        }else if(elemento == OBJETO){
            m_foco(2);
        }

    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}
/**
 * @brief Callback function to control the basic keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */
void keyboard(unsigned char key, int x, int y) {

    char* fname = malloc(sizeof (char)*128); /* Note that scanf adds a null character at the end of the vector*/
    int read = 0;

    object3d *auxiliar_object = 0;
    GLdouble wd,he,midx,midy;

    matrices *auxiliar_matrix;
    camera *auxiliar_camera = 0;

    switch (key) {
        case 'f':
        case 'F':
            /*Ask for file*/
            printf("%s", KG_MSSG_SELECT_FILE);
            scanf("%s", fname);
            /*Allocate memory for the structure and read the file*/
            auxiliar_object = (object3d *) malloc(sizeof(object3d));
            read = read_wavefront(fname, auxiliar_object);
            switch (read) {
                /*Errors in the reading*/
                case 1:
                    printf("%s: %s\n", fname, KG_MSSG_FILENOTFOUND);
                    break;
                case 2:
                    printf("%s: %s\n", fname, KG_MSSG_INVALIDFILE);
                    break;
                case 3:
                    printf("%s: %s\n", fname, KG_MSSG_EMPTYFILE);
                    break;
                    /*Read OK*/
                case 0:
                    /*Insert the new object in the list*/

                    auxiliar_matrix = (matrices *) malloc(sizeof(matrices));
                    identity(auxiliar_matrix->matriz);
                    auxiliar_matrix->next = 0;

                    auxiliar_object->next = _first_object;
                    auxiliar_object->matrix_table = auxiliar_matrix;
                    _first_object = auxiliar_object;
                    _selected_object = _first_object;

                    //En caso de que la estemos visualizando lo que ve el objeto seleccionado o que estemos en modo análisis
                    if(elemento == CAMARA && modo_camara == ANALISIS){
                        centre_camera_to_obj(_selected_object);
                    }else if (elemento == OBJETOCAMARA){
                        add_camera_mode_obj(_selected_object);
                    }
                    anadir_material();
                    _selected_object->shade = FLAT;
                    foco_obj();

                    printf("%s\n", KG_MSSG_FILEREAD);
                    break;
            }
            break;

        case 9: /* <TAB> */
            if (_selected_object != 0) {
                _selected_object = _selected_object->next;
                /*The selection is circular, thus if we move out of the list we go back to the first element*/
                if (_selected_object == 0) {
                    _selected_object = _first_object;
                }
                foco_obj();

                if(elemento == CAMARA && modo_camara == ANALISIS){
                    centre_camera_to_obj(_selected_object);
                }else if (elemento == OBJETOCAMARA){
                    add_camera_mode_obj(_selected_object);
                }

            }
            break;

        case 127: /* <SUPR> */
            /*Erasing an object depends on whether it is the first one or not*/
            if (_selected_object != 0) {
                if (_selected_object == _first_object) {
                    /*To remove the first object we just set the first as the current's next*/
                    _first_object = _first_object->next;
                    /*Once updated the pointer to the first object it is save to free the memory*/
                    better_free(_selected_object);
                    /*Finally, set the selected to the new first one*/
                    _selected_object = _first_object;


                } else {
                    /*In this case we need to get the previous element to the one we want to erase*/
                    auxiliar_object = _first_object;
                    while (auxiliar_object->next != _selected_object)
                        auxiliar_object = auxiliar_object->next;
                    /*Now we bypass the element to erase*/
                    auxiliar_object->next = _selected_object->next;
                    /*free the memory*/
                    better_free(_selected_object);
                    /*and update the selection*/
                    _selected_object = auxiliar_object;

                }

                if(_selected_object == 0){

                    /*Necesitamos asegurarnos de que estos modos solo se ejecutan si hay objetos*/
                    if(elemento == CAMARA && modo_camara == ANALISIS){
                        modo_camara = VUELO;
                    }else if (elemento == OBJETOCAMARA){
                        elemento = OBJETO;
                    }

                }else{

                    if(elemento == CAMARA && modo_camara == ANALISIS){
                        centre_camera_to_obj(_selected_object);
                    }else if (elemento == OBJETOCAMARA){
                        add_camera_mode_obj(_selected_object);
                    }
                }
                foco_obj();

            }
            break;

        case '-':
            if ((elemento == OBJETO || elemento == OBJETOCAMARA) && modo == ESCALADO && _selected_object != 0) {
                aplicar_transformaciones(menos_escalado);
                if(elemento == OBJETOCAMARA) {
                    add_camera_mode_obj(_selected_object);
                }
            } else if (elemento == CAMARA && modo == ESCALADO) {
                wd = (_selected_camera->proj->right - _selected_camera->proj->left) / KG_STEP_ZOOM;
                he = (_selected_camera->proj->bottom - _selected_camera->proj->top) / KG_STEP_ZOOM;

                midx = (_selected_camera->proj->right + _selected_camera->proj->left) / 2;
                midy = (_selected_camera->proj->bottom + _selected_camera->proj->top) / 2;

                _selected_camera->proj->right = (midx + wd) / 2;
                _selected_camera->proj->left = (midx - wd) / 2;
                _selected_camera->proj->bottom = (midy + he) / 2;
                _selected_camera->proj->top = (midy - he) / 2;
            }else if(elemento == ILUMINACION &&
                    (global_lights[_selected_light].type == FOCO || global_lights[_selected_light].type == FOCO_OBJETO)
                    && global_lights[_selected_light].is_on == 1){
                global_lights[_selected_light].cut_off -= 5;
            }
            break;

        case '+':
            if ((elemento == OBJETO || elemento == OBJETOCAMARA) && modo == ESCALADO && _selected_object != 0) {
                aplicar_transformaciones(mas_escalado);
                if (elemento == OBJETOCAMARA) {
                    add_camera_mode_obj(_selected_object);
                }
            } else if (elemento == CAMARA && modo == ESCALADO) {
                wd = (_selected_camera->proj->right - _selected_camera->proj->left) * KG_STEP_ZOOM;
                he = (_selected_camera->proj->bottom - _selected_camera->proj->top) * KG_STEP_ZOOM;

                midx = (_selected_camera->proj->right + _selected_camera->proj->left) / 2;
                midy = (_selected_camera->proj->bottom + _selected_camera->proj->top) / 2;

                _selected_camera->proj->right = (midx + wd) / 2;
                _selected_camera->proj->left = (midx - wd) / 2;
                _selected_camera->proj->bottom = (midy + he) / 2;
                _selected_camera->proj->top = (midy - he) / 2;
            }else if(elemento == ILUMINACION &&
                     (global_lights[_selected_light].type == FOCO || global_lights[_selected_light].type == FOCO_OBJETO)
                     && global_lights[_selected_light].is_on == 1){
                global_lights[_selected_light].cut_off += 5;
            }
            break;

        case '?':
            print_help();
            break;

        case 27: /* <ESC> */
            exit(0);
            break;

        case 't':
        case 'T':
            if (modo != TRASLACION) {
                printf("Modo cambiado a traslación\n");
                modo = TRASLACION;
            }
            break;

        case 'r':
        case 'R':
            if (modo != ROTACION) {
                printf("Modo cambiado a rotación\n");
                modo = ROTACION;
            }
            break;

        case 'e':
        case 'E':
            if (modo != ESCALADO && elemento != ILUMINACION) {
                printf("Modo cambiado a escalado o volumen de visión\n");
                modo = ESCALADO;
            }else if(elemento == ILUMINACION){
                printf("En la iluminación no hay escalado, utiliza los otros modos de transformación\n");
            }
            break;

        case 'g':
        case 'G':
            if (elemento == OBJETO || elemento == OBJETOCAMARA) {
                if (sis_referencia != GLOBALES) {
                    printf("Sistema de referencia cambiado a global\n");
                    sis_referencia = GLOBALES;
                }
            } else if (elemento == CAMARA) {
                //solo entramos en este modo si hay un objeto al que analizar
                if (modo_camara != ANALISIS) {
                    if(_selected_object != 0) {
                        printf("Cámara en modo análisis\n");
                        modo_camara = ANALISIS;
                        centre_camera_to_obj(_selected_object);
                    }else{
                        printf("PRIMERO CARGA UN OBJETO!!!\n");
                    }
                }
            }
            break;

    case 'l':
    case 'L':
        if(elemento==OBJETO || elemento == OBJETOCAMARA){
            if (sis_referencia != LOCALES) {
                printf("Sistema de referencia cambiado a local\n");
                sis_referencia = LOCALES;
            }
        }else if (elemento == CAMARA) {
            if (modo_camara != VUELO) {
                printf("Cámara en modo vuelo\n");
                modo_camara = VUELO;
            }
        }
        break;

    case 'o':
    case 'O':
        if(elemento!=OBJETO) {
            printf("Elemento cambiado a objeto\n");
            elemento=OBJETO;
        }
        break;

    case 'c':
    case 'C':
        if(elemento!=CAMARA) {
            printf("Elemento cambiado a cámara\n");
            elemento=CAMARA;
            if(modo_camara == ANALISIS){
                if(_selected_object != 0) {
                    centre_camera_to_obj(_selected_object);
                }else{
                    modo_camara = VUELO;
                }

            }

        }
        break;

    case 'a':
    case 'A':
        if(elemento!=ILUMINACION && luz == ACTIVADA) {
            printf("Elemento cambiado a iluminación\n");
            elemento=ILUMINACION;
            if(modo == ESCALADO){
                modo = TRASLACION;
                printf("El escalado no existe en la iluminación. Modo cambiado a traslación\n");
            }
        }else if(luz == DESACTIVADA){
            printf("Activa la iluminación primero!\n");
        }
        break;

    case 26 ://ctrl+z
        if(_selected_object!=0) {
            if (_selected_object->matrix_table->next != 0) {
                printf("Deshaciendo...\n");
                _selected_object->matrix_table = _selected_object->matrix_table->next;
                if(elemento == OBJETOCAMARA){
                    add_camera_mode_obj(_selected_object);
                }
                foco_obj();
            }
        }

        break;

    case 'K'://K mayus, visualizar lo que ve el objeto seleccionado si hay objeto

        if(_selected_object != 0) {
            if(elemento != OBJETOCAMARA) {

                printf("Modo especial, visualizamos lo que el objeto seleccionado. Pulsa K, O, I o C para salir.\n");

                add_camera_mode_obj(_selected_object);
                elemento = OBJETOCAMARA;

            } else {
                //pasamos a modo objeto
                elemento = OBJETO;
            }
        }

        break;

    case 'k': //k minus, cambiar camara
        cambiar_camara();
        if(modo_camara == ANALISIS && _selected_object != 0){
            centre_camera_to_obj(_selected_object);
        }
        foco_camara();
        break;

    case 'n'://nueva camara
        add_camera();
        if(modo_camara == ANALISIS && _selected_object != 0){
            centre_camera_to_obj(_selected_object);
        }
        foco_camara();
        break;

    case 'p':// cambiar perspectiva (propia de cada cámara)
    case 'P':
        //en este modo no tiene sentido
        if(elemento != OBJETOCAMARA) {
            //de esta manera si era 0 será 1 y viceversa
            _selected_camera->type = (_selected_camera->type + 1) % 2;

            if (_selected_camera->type == PARALELA) {
                printf("Proyección es paralela.\n");
                _selected_camera->proj->left = -5.0;
                _selected_camera->proj->right = 5.0;
                _selected_camera->proj->top = 5.0;
                _selected_camera->proj->bottom = -5.0;
                _selected_camera->proj->near = 0;
                _selected_camera->proj->far = 1000.0;
            } else {
                printf("Proyección es perspectiva.\n");
                _selected_camera->proj->left = -0.1;
                _selected_camera->proj->right = 0.1;
                _selected_camera->proj->top = 0.1;
                _selected_camera->proj->bottom = -0.1;
                _selected_camera->proj->near = 0.1;
                _selected_camera->proj->far = 1000.0;
            }
        }
        break;

    case '0':
        anadir_luz();
        break;

    case '1':
        if(_selected_light != 0 && luz == ACTIVADA) {
            _selected_light = 0;
            printf("BOMBILLA seleccionada.\n");
        }
        break;

    case '2':
        if(_selected_light != 1 && luz == ACTIVADA) {
            _selected_light = 1;
            printf("SOL seleccionada.\n");
        }
        break;

    case '3':
        if(_selected_light != 2 && luz == ACTIVADA) {
            _selected_light = 2;
            printf("FOCO-OBJETO seleccionado.\n");
        }
        break;

    case '4':
        if(_selected_light != 3 && luz == ACTIVADA) {
            _selected_light = 3;
            printf("FOCO-CÁMARA seleccionado.\n");
        }
        break;

    case '5':
        if(_selected_light != 4 && luz == ACTIVADA) {
            _selected_light = 4;
            printf("Seleccionada LUZ 5.\n");
        }
        break;

    case '6':
        if(_selected_light != 5 && luz == ACTIVADA) {
            _selected_light = 5;
            printf("Seleccionada LUZ 6.\n");
        }
        break;

    case '7':
        if(_selected_light != 6 && luz == ACTIVADA) {
            _selected_light = 6;
            printf("Seleccionada LUZ 7.\n");
        }
        break;

    case '8':
        if(_selected_light != 7 && luz == ACTIVADA) {
            _selected_light = 7;
            printf("Seleccionada LUZ 8.\n");
        }
        break;
    case 'm':
    case 'M':
        if(luz == ACTIVADA) {
            cambiar_material();
        }
        break;

    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

