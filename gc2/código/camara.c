#include "definitions.h"
#include "io.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>



extern camera *_first_camera;
extern camera *_selected_camera;
extern camera *_object_camera;

extern object3d *_selected_object;



/**
 * Método para crear una cámara
 * @param camera_pos posición de la cámara
 * @param camera_front punto de mira de la cámara
 * @param camera_up verticalidad de la cámara
 * @param cam cámara a crear
 */
void create_camera(vector3 camera_pos, vector3 camera_front, vector3 camera_up, camera *cam){

    cam->next = 0;
    cam->type = PERSPECTIVA;
    cam->proj = (projection*)malloc(sizeof (projection));
    cam->proj->left = -0.1;
    cam->proj->right = 0.1;
    cam->proj->top = 0.1;
    cam->proj->bottom = -0.1;
    cam->proj->near = 0.1;
    cam->proj->far = 1000.0;

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

/**
 * método para añadir una nueva cámara a la lista
 * @param l_camera cámara a añadir
 */
void add_camera_list(camera *l_camera){
    l_camera->next = _first_camera;
    _first_camera = l_camera;
    _selected_camera = _first_camera;
}

/**
 * Método para inicializar todas las cámaras
 */
void default_cameras(){


    camera *aux = (camera*)malloc(sizeof(camera));

    _object_camera = (camera*)malloc(sizeof(camera));
    _object_camera->type = PERSPECTIVA;
    _object_camera->proj = (projection*)malloc(sizeof (projection));
    _object_camera->proj->left = -0.1;
    _object_camera->proj->right = 0.1;
    _object_camera->proj->top = 0.1;
    _object_camera->proj->bottom = -0.1;
    _object_camera->proj->near = 0.1;
    _object_camera->proj->far = 1000.0;

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

/**
 * Método de cambio de cámara, pasa a la siguiente de manera circular
 */
void cambiar_camara(){

    if(_selected_camera->next == 0){
        _selected_camera = _first_camera;
    }else{
        _selected_camera = _selected_camera->next;
    }
}

/**
 * Método para crear una nueva cámara y añadirla, esta cámara se puede encontar en la cámara seleccionada
 */
void add_camera(){
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

    _object_camera->minv[0] = obj->matrix_table->matriz[0];
    _object_camera->minv[1] = obj->matrix_table->matriz[1];
    _object_camera->minv[2] = obj->matrix_table->matriz[2];
    _object_camera->minv[3] = 0;

    _object_camera->minv[4] = obj->matrix_table->matriz[4];
    _object_camera->minv[5] = obj->matrix_table->matriz[5];
    _object_camera->minv[6] = obj->matrix_table->matriz[6];
    _object_camera->minv[7] = 0;

    _object_camera->minv[8] = obj->matrix_table->matriz[8];
    _object_camera->minv[9] = obj->matrix_table->matriz[9];
    _object_camera->minv[10] = -obj->matrix_table->matriz[10];
    _object_camera->minv[11] = 0;

    _object_camera->minv[12] = obj->matrix_table->matriz[12];
    _object_camera->minv[13] = obj->matrix_table->matriz[13];
    _object_camera->minv[14] = obj->matrix_table->matriz[14];
    _object_camera->minv[15] = 1;


    matriz_inversa(_object_camera);
}


/**
 * Método que centra una cámara a un objeto haciendo que lo mire
 * @param obj objeto a centrar
 */
void centre_camera_to_obj(object3d *obj){

    vector3 camera_pos = (vector3) { .x = _selected_camera->minv[12], .y = _selected_camera->minv[13], .z = _selected_camera->minv[14] };
    vector3 camera_front = (vector3) { .x = obj->matrix_table->matriz[12], .y = obj->matrix_table->matriz[13], .z = obj->matrix_table->matriz[14] };
    vector3 camera_up = (vector3) { .x = 0, .y = 1, .z = 0 };


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z,
              camera_front.x, camera_front.y, camera_front.z,
              camera_up.x, camera_up.y, camera_up.z);

    glGetFloatv(GL_MODELVIEW_MATRIX, _selected_camera->m);

    _selected_camera->minv[0] = _selected_camera->m[0];
    _selected_camera->minv[1] = _selected_camera->m[4];
    _selected_camera->minv[2] = _selected_camera->m[8];
    _selected_camera->minv[3] = 0;

    _selected_camera->minv[4] = _selected_camera->m[1];
    _selected_camera->minv[5] = _selected_camera->m[5];
    _selected_camera->minv[6] = _selected_camera->m[9];
    _selected_camera->minv[7] = 0;

    _selected_camera->minv[8] = _selected_camera->m[2];
    _selected_camera->minv[9] = _selected_camera->m[6];
    _selected_camera->minv[10] = _selected_camera->m[10];
    _selected_camera->minv[11] = 0;

    _selected_camera->minv[12] = camera_pos.x;
    _selected_camera->minv[13] = camera_pos.y;
    _selected_camera->minv[14] = camera_pos.z;
    _selected_camera->minv[15] = 1;


}
/**
 * Método para moverse en modo análisis, realiza los cálculos necesarios para rotar positiva o negativamente sobre la x o y
 * @param x sentido y módulo de la rotación centrada al objeto sobre el eje x
 * @param y sentido y módulo de la rotación centrada al objeto sobre el eje y
 */
void modo_analisis(int x, int y){

    if(_selected_object != 0) {
        GLfloat px, py, pz, distance;

        px = _selected_object->matrix_table->matriz[12] - _selected_camera->minv[12];
        py = _selected_object->matrix_table->matriz[13] - _selected_camera->minv[13];
        pz = _selected_object->matrix_table->matriz[14] - _selected_camera->minv[14];

        distance = sqrt(pow(px, 2) + pow(py, 2) + pow(pz, 2));

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(_selected_camera->minv);
        glTranslatef(0, 0, -distance);
        glRotatef(8.0f, -x, -y, 0);
        glTranslatef(0, 0, distance);
        glGetFloatv(GL_MODELVIEW_MATRIX, _selected_camera->minv);

        matriz_inversa(_selected_camera);
    }

}