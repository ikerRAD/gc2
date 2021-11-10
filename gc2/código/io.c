#include "definitions.h"
#include "load_obj.h"
#include <GL/glut.h>
#include <stdio.h>
#include "transformaciones.h"
#include "camara.h"


extern object3d * _first_object;
extern object3d * _selected_object;

extern camera  * _first_camera;
extern camera * _selected_camera;
extern camera * _object_camera;

extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern int modo;
extern int sis_referencia;
extern int elemento;
extern int modo_camara;


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
    printf("<F,f>\t\t Cargar un objeto\n");
    printf("<TAB>\t\t Elegir un objeto entre los cargados\n");
    printf("<DEL>\t\t Borrar el objeto seleccionado\n");
    printf("<K>\t\t Visualizar punto de vista de los objetos (Si vuelves a pulsar K, o cambias de elemento, saldrás de este modo)\n");
    printf("<k>\t\t Cambiar de cámara.\n");
    printf("<n>\t\t Añadir nueva cámara.\n");
    printf("<P,p>\t\t Cambio de tipo de proyección: perspectiva (por defecto)/ paralela.\n");
    printf("\n\n");
    printf("MODOS DE TRANSFORMACIÓN \n");
    printf("<T,t>\t\t Modo de traslación (modo por defecto)\n");
    printf("<R,r>\t\t Modo de rotación\n");
    printf("<E,e>\t\t Modo de escalado / cambio de volumen\n");
    printf("\n\n");
    printf("SISTEMA DE REFERENCIA (EN MODO OBJETO) O MODO DE CÁMARA (EN MODO CÁMARA) \n");
    printf("<G,g>\t\t Transformaciones globales o modo análisis\n");
    printf("<L,l>\t\t Transformaciones locales o modo vuelo(por defecto)\n");
    printf("\n\n");
    printf("ELEMENTO A TRANSFORMAR \n");
    printf("<O,o>\t\t Transformaciones a objeto (modo por defecto)\n");
    printf("<C,c>\t\t Transformaciones a cámara\n");
    printf("<I,i>\t\t Transformaciones a iluminación (NO IMPLEMENTADO)\n");
    printf("\n\n");
    printf("MOVIMIENTOS POSIBLES \n");
    printf("<ARRIBA>\n");
    printf("<ABAJO>\n");
    printf("<IZQUIERDA>\n");
    printf("<DERECHA>\n");
    printf("<AVPAG>\n");
    printf("<REPAG>\n");
    printf("<+> (SOLO EN MODO ESCALADO/CAMBIO DE VOLUMEN)\n");
    printf("<-> (SOLO EN MODO ESCALADO/CAMBIO DE VOLUMEN)\n");
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
                            modo_analisis(-1,0);
                        }

                    }else if(modo == ROTACION){
                        if(modo_camara == VUELO){
                            aplicar_transformaciones(up_rotacion);
                        }else{
                            modo_analisis(-1,0);
                        }
                    }else if(modo == ESCALADO){
                        _selected_camera->proj->top -= 0.01;
                        _selected_camera->proj->bottom += 0.01;
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
                            modo_analisis(1,0);
                        }

                    }else if(modo == ROTACION){
                        if(modo_camara == VUELO){
                            aplicar_transformaciones(down_rotacion);
                        }else{
                            modo_analisis(1,0);
                        }
                    }else if(modo == ESCALADO){
                        _selected_camera->proj->top += 0.01;
                        _selected_camera->proj->bottom -= 0.01;
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
                        _selected_camera->proj->left += 0.01;
                        _selected_camera->proj->right -= 0.01;
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
                        _selected_camera->proj->left -= 0.01;
                        _selected_camera->proj->right += 0.01;
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
                            if (distancia_camara_objeto() > 0.5){
                                camara = (vector3 *)malloc(sizeof(vector3));
                                *camara = (vector3){
                                        .x = -_selected_camera->current_camera->m_invert[8],
                                        .y = -_selected_camera->current_camera->m_invert[9],
                                        .z = -_selected_camera->current_camera->m_invert[10]
                                };
                                aplicar_transformaciones(camara);
                            }
                        } else {
                            aplicar_transformaciones(repag_values);
                        }

                    }else if(modo == ROTACION){
                        if (modo_camara == VUELO){
                            aplicar_transformaciones(repag_values);
                        }else{
                            modo_camara = VUELO;
                            aplicar_transformaciones(repag_values);
                            modo_camara = ANALISIS;
                        }
                    }else if(modo == ESCALADO){
                        _selected_camera->current_camera->proj->near -= 0.01;
                        _selected_camera->current_camera->proj->far -= 0.01;

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
                                camara = (vector3 *)malloc(sizeof(vector3));
                                *camara = (vector3){
                                        .x = _selected_camera->current_camera->m_invert[8],
                                        .y = _selected_camera->current_camera->m_invert[9],
                                        .z = _selected_camera->current_camera->m_invert[10]
                                };
                                aplicar_transformaciones(camara);
                            }
                        } else {
                            aplicar_transformaciones(avpag_values);
                        }

                    }else if(modo == ROTACION){
                        if (modo_camara == VUELO){
                            aplicar_transformaciones(avpag_values);
                        }else{
                            modo_camara = VUELO;
                            aplicar_transformaciones(avpag_values);
                            modo_camara = ANALISIS;
                        }
                    }else if(modo == ESCALADO){
                        _selected_camera->current_camera->proj->near += 0.01;
                        _selected_camera->current_camera->proj->far += 0.01;
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
                    if(elemento == CAMARA && modo_camara = ANALISIS){
                        centre_camera_to_obj(_selected_object);
                    }else if (elemento == OBJETOCAMARA){
                        add_camera_mode_obj(_selected_object);
                    }

                    printf("%s\n", KG_MSSG_FILEREAD);
                    break;
            }
            break;
        //TODO
        case 9: /* <TAB> */
            if (_selected_object != 0) {
                _selected_object = _selected_object->next;
                /*The selection is circular, thus if we move out of the list we go back to the first element*/
                if (_selected_object == 0) _selected_object = _first_object;

                if(elemento == CAMARA && modo_camara = ANALISIS){
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
                    if(elemento == CAMARA && modo_camara = ANALISIS){
                        modo_camara = VUELO;
                    }else if (elemento == OBJETOCAMARA){
                        elemento = OBJETO;
                    }

                }else{

                    if(elemento == CAMARA && modo_camara = ANALISIS){
                        centre_camera_to_obj(_selected_object);
                    }else if (elemento == OBJETOCAMARA){
                        add_camera_mode_obj(_selected_object);
                    }
                }

            }
            break;

        case '-':
            if ((elemento == OBJETO || elemento == OBJETOCAMARA) && modo == ESCALADO && _selected_object != 0) {
                aplicar_transformaciones(menos_escalado);
            } else if (elemento == CAMARA) {
                wd = (_selected_camera->proj->right - _selected_camera->proj->left) / KG_STEP_ZOOM;
                he = (_selected_camera->proj->bottom - _selected_camera->proj->top) / KG_STEP_ZOOM;

                midx = (_selected_camera->proj->right + _selected_camera->proj->left) / 2;
                midy = (_selected_camera->proj->bottom + _selected_camera->proj->top) / 2;

                _selected_camera->proj->right = (midx + wd) / 2;
                _selected_camera->proj->left = (midx - wd) / 2;
                _selected_camera->proj->bottom = (midy + he) / 2;
                _selected_camera->proj->top = (midy - he) / 2;
            }
            break;

        case '+':
            if ((elemento == OBJETO || elemento == OBJETOCAMARA) && modo == ESCALADO && _selected_object != 0) {
                aplicar_transformaciones(mas_escalado);
            } else if (elemento == CAMARA) {
                wd = (_selected_camera->proj->right - _selected_camera->proj->left) * KG_STEP_ZOOM;
                he = (_selected_camera->proj->bottom - _selected_camera->proj->top) * KG_STEP_ZOOM;

                midx = (_selected_camera->proj->right + _selected_camera->proj->left) / 2;
                midy = (_selected_camera->proj->bottom + _selected_camera->proj->top) / 2;

                _selected_camera->proj->right = (midx + wd) / 2;
                _selected_camera->proj->left = (midx - wd) / 2;
                _selected_camera->proj->bottom = (midy + he) / 2;
                _selected_camera->proj->top = (midy - he) / 2;
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
            if (modo != ESCALADO) {
                printf("Modo cambiado a escalado o volumen de visión\n");
                modo = ESCALADO;
            }
            break;
        //TODO
        case 'g':
        case 'G':
            if (elemento == OBJETO) {
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
                        printf("PRIMERO CARGA UN OBJETO!!!");
                    }
                }
            }
            break;

    case 'l':
    case 'L':
        if(elemento==OBJETO){
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

    case 'i':
    case 'I':
        if(elemento!=ILUMINACION) {
            printf("Elemento cambiado a iluminación\n");
            elemento=ILUMINACION;
        }
        break;

    case 26 ://ctrl+z
        if(_selected_object!=0 && elemento == OBJETO) {
            if (_selected_object->matrix_table->next != 0) {
                printf("Deshaciendo...\n");
                _selected_object->matrix_table = _selected_object->matrix_table->next;

            }
        }

        break;

    case 'K'://K mayus, visualizar lo que ve el objeto seleccionado si hay objeto

        if(_selected_object != 0) {
            if (elemento = !OBJETOCAMARA) {
                elemento = OBJETOCAMARA;
                printf("Modo especial, visualizamos lo que el objeto seleccionado. Pulsa K, O, I o C para salir.\n");

                add_camera_mode_obj(_selected_object);


            } else {
                //pasamos a modo objeto
                elemento = OBJETO;
            }
        }

        break;
    //TODO
    case 'k': //k minus, cambiar camara
        cambiar_camara();
        if(modo_camara == ANALISIS && _selected_object != 0){
            centre_camera_to_obj(_selected_object);
        }
        break;

    case 'n'://nueva camara
        add_camera();
        if(modo_camara == ANALISIS && _selected_object != 0){
            centre_camera_to_obj(_selected_object);
        }
        break;

    case 'p':// cambiar perspectiva (propia de cada cámara)
    case 'P':
        //de esta manera si era 0 será 1 y viceversa
        _selected_camera->type = (_selected_camera->type + 1)%2;

        if(_selected_camera->type == PARALELA){
            printf("Proyección es paralela.\n");
        }else{
            printf("Proyección es perspectiva.\n");
        }

        break;

    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

