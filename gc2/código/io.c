#include "definitions.h"
#include "load_obj.h"
#include <GL/glut.h>
#include <stdio.h>
#include "transformaciones.h"


extern object3d * _first_object;
extern object3d * _selected_object;

extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern int modo;
extern int sis_referencia;
extern int elemento;

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

/**
 * @brief This function just prints information about the use
 * of the keys
 */
void print_help(){
    printf("Práctica de la asignatura GC. Este programa visualiza y modifica \n");
    printf("objetos 3D.  \n\n");
    printf("\n\n");
    printf("FUNCIONES PRINCIPALES \n");
    printf("<?>\t\t Visualizar ayuda \n");
    printf("<ESC>\t\t Salir del programa \n");
    printf("<F>\t\t Cargar un objeto\n");
    printf("<TAB>\t\t Elegir un objeto entre los cargados\n");
    printf("<DEL>\t\t Borrar el objeto seleccionado\n");
    printf("<CTRL + ->\t Aumentar el volumen de visualización\n");
    printf("<CTRL + +>\t Disminuir el volumen de visualización\n");
    printf("\n\n");
    printf("MODOS DE TRANSFORMACIÓN \n");
    printf("<T>\t\t Modo de traslación (modo por defecto)\n");
    printf("<R>\t\t Modo de rotación\n");
    printf("<E>\t\t Modo de escalado\n");
    printf("\n\n");
    printf("SISTEMA DE REFERENCIA \n");
    printf("<G>\t\t Transformaciones globales\n");
    printf("<L>\t\t Transformaciones locales (transformaciones por defecto)\n");
    printf("\n\n");
    printf("ELEMENTO A TRANSFORMAR \n");
    printf("<O>\t\t Transformaciones a objeto (modo por defecto)\n");
    printf("<C>\t\t Transformaciones a cámara (NO IMPLEMENTADO)\n");
    printf("<I>\t\t Transformaciones a iluminación (NO IMPLEMENTADO)\n");
    printf("\n\n");
    printf("MOVIMIENTOS POSIBLES \n");
    printf("<ARRIBA>\n");
    printf("<ABAJO>\n");
    printf("<IZQUIERDA>\n");
    printf("<DERECHA>\n");
    printf("<AVPAG>\n");
    printf("<REPAG>\n");
    printf("<+> (SOLO EN MODO ESCALADO)\n");
    printf("<-> (SOLO EN MODO ESCALADO)\n");
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

    glMatrixMode(GL_MODELVIEW);
    if(_selected_object!=0) {
        switch (key) {

            case GLUT_KEY_UP:
                if (elemento == OBJETO) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(up_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(up_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(up_escalado);
                    }
                }
                break;
            case GLUT_KEY_DOWN:
                if (elemento == OBJETO) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(down_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(down_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(down_escalado);
                    }
                }
                break;
            case GLUT_KEY_LEFT:
                if (elemento == OBJETO) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(left_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(left_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(left_escalado);
                    }
                }
                break;
            case GLUT_KEY_RIGHT:
                if (elemento == OBJETO) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(right_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(right_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(right_escalado);
                    }
                }
                break;
            case GLUT_KEY_PAGE_UP:
                if (elemento == OBJETO) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(repag_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(repag_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(repag_escalado);
                    }
                }
                break;
            case GLUT_KEY_PAGE_DOWN:
                if (elemento == OBJETO) {
                    if (modo == TRASLACION) {
                        aplicar_transformaciones(avpag_traslacion);
                    } else if (modo == ROTACION) {
                        aplicar_transformaciones(avpag_rotacion);
                    } else if (modo == ESCALADO) {
                        aplicar_transformaciones(avpag_escalado);
                    }
                }
                break;

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

    switch (key) {
    case 'f':
    case 'F':
        /*Ask for file*/
        printf("%s", KG_MSSG_SELECT_FILE);
        scanf("%s", fname);
        /*Allocate memory for the structure and read the file*/
        auxiliar_object = (object3d *) malloc(sizeof (object3d));
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

            auxiliar_matrix=(matrices *)malloc(sizeof(matrices));
            identity(auxiliar_matrix->matriz);
            auxiliar_matrix->next=0;

            auxiliar_object->next = _first_object;
            auxiliar_object->matrix_table=auxiliar_matrix;
            _first_object = auxiliar_object;
            _selected_object = _first_object;
            printf("%s\n",KG_MSSG_FILEREAD);
            break;
        }
        break;

    case 9: /* <TAB> */
        if (_selected_object!=0) {
            _selected_object = _selected_object->next;
            /*The selection is circular, thus if we move out of the list we go back to the first element*/
            if (_selected_object == 0) _selected_object = _first_object;
        }
        break;

    case 127: /* <SUPR> */
        /*Erasing an object depends on whether it is the first one or not*/
        if(_selected_object!=0) {
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
        }
        break;

    case '-':
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
            /*Increase the projection plane; compute the new dimensions*/
            wd=(_ortho_x_max-_ortho_x_min)/KG_STEP_ZOOM;
            he=(_ortho_y_max-_ortho_y_min)/KG_STEP_ZOOM;
            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_ortho_x_max+_ortho_x_min)/2;
            midy = (_ortho_y_max+_ortho_y_min)/2;
            /*The the new limits are set, keeping the center of the plane*/
            _ortho_x_max = midx + wd/2;
            _ortho_x_min = midx - wd/2;
            _ortho_y_max = midy + he/2;
            _ortho_y_min = midy - he/2;
        }else if(elemento==OBJETO && modo==ESCALADO && _selected_object!=0){
            aplicar_transformaciones(menos_escalado);
        }
        break;

    case '+':
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
            /*Decrease the projection plane; compute the new dimensions*/
            wd=(_ortho_x_max-_ortho_x_min)*KG_STEP_ZOOM;
            he=(_ortho_y_max-_ortho_y_min)*KG_STEP_ZOOM;
            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_ortho_x_max+_ortho_x_min)/2;
            midy = (_ortho_y_max+_ortho_y_min)/2;
            /*The the new limits are set, keeping the center of the plane*/
            _ortho_x_max = midx + wd/2;
            _ortho_x_min = midx - wd/2;
            _ortho_y_max = midy + he/2;
            _ortho_y_min = midy - he/2;
        }else if(elemento==OBJETO && modo==ESCALADO && _selected_object!=0){
            aplicar_transformaciones(mas_escalado);
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
        if(modo!=TRASLACION) {
            printf("Modo cambiado a traslación\n");
            modo = TRASLACION;
        }
        break;

    case 'r':
    case 'R':
        if(modo!=ROTACION) {
            printf("Modo cambiado a rotación\n");
            modo = ROTACION;
        }
        break;

    case 'e':
    case 'E':
        if(modo!=ESCALADO) {
            printf("Modo cambiado a escalado\n");
            modo = ESCALADO;
        }
        break;

    case 'g':
    case 'G':
        if(sis_referencia!=GLOBALES) {
            printf("Sistema de referencia cambiado a global\n");
            sis_referencia=GLOBALES;
        }
        break;

    case 'l':
    case 'L':
        if(sis_referencia!=LOCALES) {
            printf("Sistema de referencia cambiado a local\n");
            sis_referencia=LOCALES;
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
        }
        break;

    case 'i':
    case 'I':
        if(elemento!=ILUMINACION) {
            printf("Elemento cambiado a iluminación\n");
            elemento=ILUMINACION;
        }
        break;

    case 26 ://ctrl+z
        if(_selected_object!=0) {
            if (_selected_object->matrix_table->next != 0) {
                printf("Deshaciendo...\n");
                _selected_object->matrix_table = _selected_object->matrix_table->next;

            }
        }

        break;


    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

