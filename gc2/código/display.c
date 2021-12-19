#include "definitions.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "camara.h"
#include <stdio.h>
#include "iluminacion.h"

/** EXTERNAL VARIABLES **/

extern object3d *_first_object;
extern object3d *_selected_object;
extern object3d *light_object;
extern object3d *cam_object;

extern camera  * _first_camera;
extern camera * _selected_camera;
extern camera * _object_camera;
extern material_light *mat_camara, *mat_selec, *mat_foco;

extern objetos_luz global_lights[8];

extern int elemento;
extern int shade;
extern int luz;

void matriz_csr(GLfloat *mcsr, GLfloat *m);

/**
 * @brief Function to draw the axes
 */
void draw_axes()
{
    /*Draw X axis*/
    glColor3f(KG_COL_X_AXIS_R,KG_COL_X_AXIS_G,KG_COL_X_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(KG_MAX_DOUBLE,0,0);
    glVertex3d(-1*KG_MAX_DOUBLE,0,0);
    glEnd();
    /*Draw Y axis*/
    glColor3f(KG_COL_Y_AXIS_R,KG_COL_Y_AXIS_G,KG_COL_Y_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,KG_MAX_DOUBLE,0);
    glVertex3d(0,-1*KG_MAX_DOUBLE,0);
    glEnd();
    /*Draw Z axis*/
    glColor3f(KG_COL_Z_AXIS_R,KG_COL_Z_AXIS_G,KG_COL_Z_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,0,KG_MAX_DOUBLE);
    glVertex3d(0,0,-1*KG_MAX_DOUBLE);
    glEnd();
}


/**
 * @brief Callback reshape function. We just store the new proportions of the window
 * @param width New width of the window
 * @param height New height of the window
 */
void reshape(int width, int height) {
    glViewport(0, 0, width, height);

}

/**
 * función para calcular el producto escalar entre la cámara y un polígono
 * @param uno punto del polígono
 * @param normal vector normal del polígono
 * @param mo  matriz de objeto del polígono
 * @param mc  matriz de la cámara
 * @return
 */
GLfloat producto_escalar(point3 uno, vector3 normal, GLfloat *mo, GLfloat *mc){

    vector3 n, co, cero;
    GLfloat m[16];
    matriz_csr(m, mo);

    n.x = mc[12] * m[0] + mc[13] * m[4] + mc[14] * m[8] + m[12];
    n.y = mc[12] * m[1] + mc[13] * m[5] + mc[14] * m[9] + m[13];
    n.z = mc[12] * m[2] + mc[13] * m[6] + mc[14] * m[10] + m[14];

    co.x = n.x - uno.x;
    co.y = n.y - uno.y;
    co.z = n.z - uno.z;



    return (co.x * normal.x) + (co.y * normal.y) + (co.z * normal.z);
}
/**
 * función para obtener la matriz de cambio de sistema de referencia a partir de una matriz de objeto
 * @param mcsr matriz a convertir en la matriz de cambio de sistema de referencia
 * @param m  matriz de objeto
 */
void matriz_csr(GLfloat *mcsr, GLfloat *m){
    mcsr[0] = m[0];
    mcsr[4] = m[1];
    mcsr[8] = m[2];
    mcsr[12] = -(m[12] * m[0] +
            m[13] * m[1] +
            m[14] * m[2]);

    mcsr[1] = m[4];
    mcsr[5] = m[5];
    mcsr[9] = m[6];
    mcsr[13] = -(m[12] * m[4] +
            m[13] * m[5] +
            m[14] * m[6]);

    mcsr[2] = m[8];
    mcsr[6] = m[9];
    mcsr[10] = m[10];
    mcsr[14] = -(m[12] * m[8] +
            m[13] * m[9] +
            m[14] * m[10]);

    mcsr[3] = 0;
    mcsr[7] = 0;
    mcsr[11] = 0;
    mcsr[15] = 1;
}

/**
 * @brief Callback display function
 */
void display(void) {
    GLint v_index, v, f, v_aux;
    object3d *aux_obj = _first_object;
    camera  *aux_cam = _first_camera;
    camera *cam2;

    if(elemento == OBJETOCAMARA){
        cam2 = _object_camera;
    }else{
        cam2 = _selected_camera;
    }

    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Define the projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*When the window is wider than our original projection plane we extend the plane in the X axis*/
    if(elemento == OBJETOCAMARA){
        glFrustum(_object_camera->proj->left,
                  _object_camera->proj->right,
                  _object_camera->proj->bottom,
                  _object_camera->proj->top,
                  _object_camera->proj->near,
                  _object_camera->proj->far);
    }else if (_selected_camera->type == PERSPECTIVA){
        glFrustum(_selected_camera->proj->left,
                  _selected_camera->proj->right,
                  _selected_camera->proj->bottom,
                  _selected_camera->proj->top,
                  _selected_camera->proj->near,
                  _selected_camera->proj->far);
    } else {

        glOrtho(_selected_camera->proj->left ,//* _window_ratio,
                _selected_camera->proj->right ,//* _window_ratio,
                _selected_camera->proj->bottom,
                _selected_camera->proj->top,
                _selected_camera->proj->near,
                _selected_camera->proj->far);

    }

    /* Now we start drawing the object */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(elemento != OBJETOCAMARA) {
        glLoadMatrixf(_selected_camera->m);
    }else{
        glLoadMatrixf(_object_camera->m);
    }

    /*First, we draw the axes*/
    //draw_axes();
    if(luz == ACTIVADA) {
        //las fuentes de luz se representan con shadders flat
        glShadeModel(GL_FLAT);

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_foco->m_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_foco->m_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_foco->m_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_foco->no_shininess);

        for (int i = 0; i < 8; i++) {
            if (global_lights[i].type != NONE) {
                glPushMatrix();
                glMultMatrixf(global_lights[i].m_obj);

                if(global_lights[i].type == FOCO){

                    for (f = 0; f < light_object->num_faces; f++) {
                        v_aux = light_object->face_table[f].vertex_table[0];

                        if (producto_escalar(light_object->vertex_table[v_aux].coord, light_object->face_table[f].normal,
                                             global_lights[i].m_obj, cam2->minv) > 0.0) {

                            glBegin(GL_POLYGON);


                            glNormal3d(light_object->face_table[f].normal.x,
                                       light_object->face_table[f].normal.y,
                                       light_object->face_table[f].normal.z);


                            for (v = 0; v < light_object->face_table[f].num_vertices; v++) {
                                v_index = light_object->face_table[f].vertex_table[v];

                                glVertex3d(light_object->vertex_table[v_index].coord.x,
                                           light_object->vertex_table[v_index].coord.y,
                                           light_object->vertex_table[v_index].coord.z);

                            }
                            glEnd();
                        }
                    }

                }

                if(global_lights[i].is_on == 1) {
                    put_light(i);
                }
                glPopMatrix();
            }
        }
    }
    /*Now each of the objects in the list*/
    while (aux_obj != 0) {
        /*Si estamos proyectando lo que ve el objeto, no enseñamos el objeto*/
        if(elemento != OBJETOCAMARA || aux_obj != _selected_object) {
            /* Select the color, depending on whether the current object is the selected one or not */

            if(luz == ACTIVADA) {
                if (aux_obj == _selected_object) {
                    glMaterialfv(GL_FRONT, GL_AMBIENT, aux_obj->material_light->m_ambient);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, aux_obj->material_light->m_diffuse);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, aux_obj->material_light->m_specular);
                    glMaterialfv(GL_FRONT, GL_SHININESS, aux_obj->material_light->no_shininess);
                }else {
                    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_selec->m_ambient);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_selec->m_diffuse);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_selec->m_specular);
                    glMaterialfv(GL_FRONT, GL_SHININESS, mat_selec->no_shininess);
                }
            }else {
                if (aux_obj == _selected_object) {
                    glColor3f(KG_COL_SELECTED_R, KG_COL_SELECTED_G, KG_COL_SELECTED_B);
                } else {
                    glColor3f(KG_COL_NONSELECTED_R, KG_COL_NONSELECTED_G, KG_COL_NONSELECTED_B);

                }
            }
            /* Draw the object; for each face create a new polygon with the corresponding vertices */
            glPushMatrix();

            glMultMatrixf(aux_obj->matrix_table->matriz);

            if(aux_obj->shade == FLAT){
                glShadeModel(GL_FLAT);
            }else{
                glShadeModel(GL_SMOOTH);
            }

            for (f = 0; f < aux_obj->num_faces; f++) {

                v_aux = aux_obj->face_table[f].vertex_table[0];

                if (producto_escalar(aux_obj->vertex_table[v_aux].coord, aux_obj->face_table[f].normal,
                                     aux_obj->matrix_table->matriz, cam2->minv) > 0.0) {

                    glBegin(GL_POLYGON);

                    if (aux_obj->shade == FLAT && luz == ACTIVADA) {
                        glNormal3d(aux_obj->face_table[f].normal.x,
                                   aux_obj->face_table[f].normal.y,
                                   aux_obj->face_table[f].normal.z);
                    }

                    for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                        v_index = aux_obj->face_table[f].vertex_table[v];

                        if (aux_obj->shade == SMOOTH && luz == ACTIVADA) {
                            glNormal3d(aux_obj->vertex_table[v_index].normal.x,
                                       aux_obj->vertex_table[v_index].normal.y,
                                       aux_obj->vertex_table[v_index].normal.z);
                        }

                        glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                                   aux_obj->vertex_table[v_index].coord.y,
                                   aux_obj->vertex_table[v_index].coord.z);

                    }
                    glEnd();
                }

            }

            glPopMatrix();
        }
        aux_obj = aux_obj->next;
    }


    /*Now each of the cameras in the list*/

    //las cámaras solo las dibujaremos con flat shadders
    glShadeModel(GL_FLAT);
    while (aux_cam != 0) {
        /*Si estamos proyectando lo que ve el objeto, enseñamos todas las cámaras, sino, la seleccionada no se enseña*/
        if(elemento == OBJETOCAMARA || aux_cam != _selected_camera) {
            if(luz == ACTIVADA) {
                glMaterialfv(GL_FRONT, GL_AMBIENT, mat_camara->m_ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_camara->m_diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, mat_camara->m_specular);
                glMaterialfv(GL_FRONT, GL_SHININESS, mat_camara->no_shininess);
            }else{
                glColor3f(KG_COL_NONSELECTED_R, 0.0, 0.0);
            }

            /* Draw the object; for each face create a new polygon with the corresponding vertices */
            //glLoadIdentity();
            glPushMatrix();

            glMultMatrixf(aux_cam->minv);
            for (f = 0; f < cam_object->num_faces; f++) {

                v_aux = cam_object->face_table[f].vertex_table[0];

                if(producto_escalar(cam_object->vertex_table[v_aux].coord, cam_object->face_table[f].normal, aux_cam->minv, cam2->minv) > 0.0) {
                    glBegin(GL_POLYGON);

                    if(luz == ACTIVADA) {
                    glNormal3d(cam_object->face_table[f].normal.x,
                               cam_object->face_table[f].normal.y,
                               cam_object->face_table[f].normal.z);
                    }

                    for (v = 0; v < cam_object->face_table[f].num_vertices; v++) {
                        v_index = cam_object->face_table[f].vertex_table[v];

                        glVertex3d(cam_object->vertex_table[v_index].coord.x,
                                   cam_object->vertex_table[v_index].coord.y,
                                   cam_object->vertex_table[v_index].coord.z);

                    }
                    glEnd();
                }

            }
            glPopMatrix();
        }
        aux_cam = aux_cam->next;
    }


    /*Do the actual drawing*/
    glFlush();
    glutSwapBuffers();
}
