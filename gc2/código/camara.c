#include "definitions.h"
#include "io.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

extern projection *global_perspective, *global_ortho;

extern camera *_first_camera;
extern camera *_selected_camera;
extern camera *_object_camera;

void set_camera_projection(){
    global_perspective = (projection*)malloc(sizeof(projection));
    global_ortho = (projection*)malloc(sizeof(projection));

    global_perspective->left = -0.1;
    global_perspective->right = 0.1;
    global_perspective->top = 0.1;
    global_perspective->bottom = -0.1;
    global_perspective->near = 0.1;
    global_perspective->far = 100.0;

    global_ortho->left = -3.0;
    global_ortho->right = 3.0;
    global_ortho->top = 3.0;
    global_ortho->bottom = -3.0;
    global_ortho->near = 0.0;
    global_ortho->far = 100.0;
}

void create_camera(vector3 camera_pos, vector3 camera_front, vector3 camera_up, camera *cam){

    cam->next = 0;
    cam->type = PERSPECTIVA;
    cam->proj = global_perspective;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z,
              camera_front.x, camera_front.y, camera_front.z,
              camera_up.x, camera_up.y, camera_up.z);

    glGetFloatv(GL_MODELVIEW_MATRIX, cam->m);

    cam->minv[0] = cam->m[0];
    cam->minv[1] = cam->m[4];
    cam->minv[2] = cam->m[8];
    cam->minv[3] = 0;

    cam->minv[4] = cam->m[1];
    cam->minv[5] = cam->m[5];
    cam->minv[6] = cam->m[9];
    cam->minv[7] = 0;

    cam->minv[8] = cam->m[2];
    cam->minv[9] = cam->m[6];
    cam->minv[10] = cam->m[10];
    cam->minv[11] = 0;

    cam->minv[12] = camera_pos.x;
    cam->minv[13] = camera_pos.y;
    cam->minv[14] = camera_pos.z;
    cam->minv[15] = 1;

}

void add_camera_list(camera *l_camera){
    l_camera->next = _first_camera;
    _first_camera = l_camera;
    _selected_camera = _first_camera;
}

void default_cameras(){

    set_camera_projection();
    camera *aux = (camera*)malloc(sizeof(camera));

    vector3 cam_pos;
    cam_pos.x = 5.0f;
    cam_pos.y = 5.0f;
    cam_pos.z = -3.0f;

    vector3 cam_front;
    cam_front.x = 0.0f;
    cam_front.y = 0.0f;
    cam_front.z = 0.0f;

    vector3 cam_up;
    cam_up.x = 0.0f;
    cam_up.y = 1.0f;
    cam_up.z = 0.0f;

    create_camera(cam_pos, cam_front, cam_up, aux);

    _first_camera = (camera*)malloc(sizeof(camera));
    _first_camera = aux;

    _selected_camera = _first_camera;
}

void cambiar_camara(){

    if(_selected_camera->next == 0){
        _selected_camera = _first_camera;
    }else{
        _selected_camera = _selected_camera->next;
    }
}

void add_camera(){
    camera *aux = (camera*)malloc(sizeof(camera));

    vector3 pos;
    cam_pos.x = 5.0f;
    cam_pos.y = 5.0f;
    cam_pos.z = -3.0f;

    vector3 front;
    cam_front.x = 0.0f;
    cam_front.y = 0.0f;
    cam_front.z = 0.0f;

    vector3 up;
    cam_up.x = 0.0f;
    cam_up.y = 1.0f;
    cam_up.z = 0.0f;

    create_camera(pos, front, up, aux);

    add_camera_list(aux);



}

/**
 * obtenemos la matriz de cambio de sistema de referencia a partir de la de objeto
 * @param c la cámara
 */
void matriz_inversa(camera *c){
    c->m[0] = c->minv[0];
    c->m[4] = c->minv[1];
    c->m[8] = c->minv[2];
    c->m[12] = -(c->minv[12] * c->minv[0] +
                  c->minv[13] * c->minv[1] +
                  c->minv[14] * c->minv[2]);

    c->m[1] = c->minv[4];
    c->m[5] = c->minv[5];
    c->m[9] = c->minv[6];
    c->m[13] = -(c->minv[12] * c->minv[4] +
                 c->minv[13] * c->minv[5] +
                 c->minv[14] * c->minv[6]);

    c->m[2] = c->minv[8];
    c->m[6] = c->minv[9];
    c->m[10] = c->minv[10];
    c->m[14] = -(c->minv[12] * c->minv[8] +
                 c->minv[13] * c->minv[9] +
                 c->minv[14] * c->minv[10]);

    c->m[3] = 0;
    c->m[7] = 0;
    c->m[11] = 0;
    c->m[15] = 1;
}
/**
 * Obtenemos la matriz de objeto para una nueva cámara
 * @param obj el objeto
 */
void add_camera_mode_obj(object3d *obj)
{
    camera * objcam = (camera*)malloc(sizeof(camera));

    objcam->minv[0] = obj->matrix_table->matriz[0];
    objcam->minv[1] = obj->matrix_table->matriz[4];
    objcam->minv[2] = -obj->matrix_table->matriz[8];
    objcam->minv[3] = 0;

    objcam->minv[4] = obj->matrix_table->matriz[1];
    objcam->minv[5] = obj->matrix_table->matriz[5];
    objcam->minv[6] = -obj->matrix_table->matriz[9];
    objcam->minv[7] = 0;

    objcam->minv[8] = obj->matrix_table->matriz[2];
    objcam->minv[9] = obj->matrix_table->matriz[6];
    objcam->minv[10] = -obj->matrix_table->matriz[10];
    objcam->minv[11] = 0;

    objcam->minv[12] = obj->matrix_table->matriz[12];
    objcam->minv[13] = obj->matrix_table->matriz[13];
    objcam->minv[14] = obj->matrix_table->matriz[14];
    objcam->minv[15] = 1;

    _object_camera = objcam;

    matriz_inversa(objcam);
}



void centre_camera_to_obj(object3d *obj){
    camera *aux = (camera*)malloc(sizeof(camera));

    create_camera(
            (vector3) { .x = _selected_camera->minv[12], .y = _selected_camera->minv[13], .z = _selected_camera->minv[14] },
            (vector3) { .x = obj->matrix_table->matriz[12], .y = obj->matrix_table->matriz[13], .z = obj->matrix_table->matriz[14] },
            (vector3) { .x = 0, .y = 1, .z = 0 },
            aux
    );

    aux->next = _selected_camera->next;
    aux->proj = _selected_camera->proj;
    _selected_camera = aux;
}
//TODO
void modo_analisis(int x, int y){
    GLfloat px, py, pz, distance;
    GLfloat m_minus[16], m_plus[16], m_rot[16];

    px = _selected_object->list_matrix->m[12] - _selected_camera->current_camera->m_invert[12];
    py = _selected_object->list_matrix->m[13] - _selected_camera->current_camera->m_invert[13];
    pz = _selected_object->list_matrix->m[14] - _selected_camera->current_camera->m_invert[14];

    distance = sqrt(pow(px, 2) + pow(py, 2) + pow(pz, 2));

    identity(m_minus); identity(m_plus); identity(m_rot);

    m_minus[12] = 0; m_minus[13] = 0; m_minus[14] = -distance;
    m_plus[12] = 0; m_plus[13] = 0; m_plus[14] = distance;

    if (x != 0){
        m_rot[5] = cos(x * KG_STEP_ROTATE * M_PI / 180.0);
        m_rot[6] = sin(x * KG_STEP_ROTATE * M_PI / 180.0);
        m_rot[9] = -1*(sin(x * KG_STEP_ROTATE * M_PI / 180.0));
        m_rot[10] = cos(x * KG_STEP_ROTATE * M_PI / 180.0);
    } else {
        m_rot[0] = cos(y * KG_STEP_ROTATE * M_PI / 180.0);
        m_rot[2] = -1 * (sin(y * KG_STEP_ROTATE * M_PI / 180.0));
        m_rot[8] = sin(y * KG_STEP_ROTATE * M_PI / 180.0);
        m_rot[10] = cos(y * KG_STEP_ROTATE * M_PI / 180.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(m_minus); glMultMatrixf(m_rot); glMultMatrixf(m_plus);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_rot);

    glLoadIdentity();
    glMultMatrixf(_selected_camera->current_camera->m_invert);
    glMultMatrixf(m_rot);
    glGetFloatv(GL_MODELVIEW_MATRIX, _selected_camera->current_camera->m_invert);

    matriz_inversa(_selected_camera);
}