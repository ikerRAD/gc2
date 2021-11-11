#include "definitions.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "camara.h"

/** EXTERNAL VARIABLES **/

extern GLdouble _window_ratio;
extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern object3d *_first_object;
extern object3d *_selected_object;

extern camera  * _first_camera;
extern camera * _selected_camera;
extern camera * _object_camera;

extern int elemento;
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
    /*  Take care, the width and height are integer numbers, but the ratio is a GLdouble so, in order to avoid
     *  rounding the ratio to integer values we need to cast width and height before computing the ratio */
    _window_ratio = (GLdouble) width / (GLdouble) height;
}


GLfloat producto_escalar(vector3 uno, vector3 dos){
    return (uno.x * dos.x) + (uno.y * dos.y) + (uno.z * dos.z);
}

vector3 get_face_camera_vector(camera *cam, point3 pt){
    GLfloat x, y, z;

    x = cam->minv[12];
    y = cam->minv[13];
    z = cam->minv[14];

    //desde camara hasta punto
    return (vector3){.x = pt.x-x , .y = pt.y-y , .z = pt.z-z};
}

/**
 * @brief Callback display function
 */
void display(void) {
    GLint v_index, v, f, v_aux;
    object3d *aux_obj = _first_object;
    camera  *aux_cam = _first_camera;
    GLfloat mcam[16];
    vector3  cpt;

    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT);

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
    draw_axes();

    /*Now each of the objects in the list*/
    while (aux_obj != 0) {
        /*Si estamos proyectando lo que ve el objeto, no enseñamos el objeto*/
        if(elemento != OBJETOCAMARA || aux_obj != _selected_object) {
            /* Select the color, depending on whether the current object is the selected one or not */
            if (aux_obj == _selected_object) {
                glColor3f(KG_COL_SELECTED_R, KG_COL_SELECTED_G, KG_COL_SELECTED_B);
            } else {
                glColor3f(KG_COL_NONSELECTED_R, KG_COL_NONSELECTED_G, KG_COL_NONSELECTED_B);
            }

            /* Draw the object; for each face create a new polygon with the corresponding vertices */
            //glLoadIdentity();
            glPushMatrix();

            glMultMatrixf(aux_obj->matrix_table->matriz);
            for (f = 0; f < aux_obj->num_faces; f++) {
                glBegin(GL_POLYGON);

                /*v_aux = aux_obj->face_table[f].vertex_table[0];
                if (elemento != OBJETOCAMARA) {
                    cpt = get_face_camera_vector(_selected_camera, aux_obj->vertex_table[v_aux].coord);
                } else {
                    cpt = get_face_camera_vector(_object_camera, aux_obj->vertex_table[v_aux].coord);
                }*/

                /*glNormal3d(aux_obj->face_table[f].normal.x,
                           aux_obj->face_table[f].normal.y,
                           aux_obj->face_table[f].normal.z);*/

                //if(producto_escalar(cpt, aux_obj->face_table[f].normal) > 0){
                    for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                        v_index = aux_obj->face_table[f].vertex_table[v];
                        glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                                   aux_obj->vertex_table[v_index].coord.y,
                                   aux_obj->vertex_table[v_index].coord.z);

                    }
                //}
                glEnd();

            }
            glPopMatrix();
        }
        aux_obj = aux_obj->next;
    }


    /*Now each of the cameras in the list*/
    while (aux_cam != 0) {
        /*Si estamos proyectando lo que ve el objeto, enseñamos todas las cámaras, sino, la seleccionada no se enseña*/
        if(elemento == OBJETOCAMARA || aux_cam != _selected_camera) {
            /* Select the color*/
            glColor3f(KG_COL_NONSELECTED_R, 0.0, 0.0);


            /* Draw the object; for each face create a new polygon with the corresponding vertices */
            //glLoadIdentity();
            glPushMatrix();

            set_objectlike_matrix(aux_cam->minv, mcam);

            glMultMatrixf(mcam);
            for (f = 0; f < aux_cam->num_faces; f++) {
                glBegin(GL_POLYGON);

                /*v_aux = aux_cam->face_table[f].vertex_table[0];
                if (elemento != OBJETOCAMARA) {
                    cpt = get_face_camera_vector(_selected_camera, aux_cam->vertex_table[v_aux].coord);
                } else {
                    cpt = get_face_camera_vector(_object_camera, aux_cam->vertex_table[v_aux].coord);
                }*/

                /*glNormal3d(aux_cam->face_table[f].normal.x,
                           aux_cam->face_table[f].normal.y,
                           aux_cam->face_table[f].normal.z);*/
                //if(producto_escalar(cpt, aux_cam->face_table[f].normal) > 0) {
                    for (v = 0; v < aux_cam->face_table[f].num_vertices; v++) {
                        v_index = aux_cam->face_table[f].vertex_table[v];
                        glVertex3d(aux_cam->vertex_table[v_index].coord.x,
                                   aux_cam->vertex_table[v_index].coord.y,
                                   aux_cam->vertex_table[v_index].coord.z);

                    }
                //}
                glEnd();
            }
            glPopMatrix();
        }
        aux_cam = aux_cam->next;
    }


    /*Do the actual drawing*/
    glFlush();
    glutSwapBuffers();
}
