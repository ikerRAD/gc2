#include <GL/glut.h>
#include "definitions.h"
#include "load_obj.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern object3d *_first_object;
extern object3d *_selected_object;

extern int modo;
extern int sis_referencia;
extern int elemento;

vector3 *up_traslacion;
vector3 *up_rotacion;
vector3 *up_escalado;

vector3 *down_traslacion;
vector3 *down_rotacion;
vector3 *down_escalado;

vector3 *right_traslacion;
vector3 *right_rotacion;
vector3 *right_escalado;

vector3 *left_traslacion;
vector3 *left_rotacion;
vector3 *left_escalado;

vector3 *avpag_traslacion;
vector3 *avpag_rotacion;
vector3 *avpag_escalado;

vector3 *repag_traslacion;
vector3 *repag_rotacion;
vector3 *repag_escalado;

vector3 *menos_escalado;

vector3 *mas_escalado;


void set_transformaciones(){

    up_traslacion=(vector3 *)malloc(sizeof(vector3));
    up_rotacion=(vector3 *)malloc(sizeof(vector3));
    up_escalado=(vector3 *)malloc(sizeof(vector3));

    down_traslacion=(vector3 *)malloc(sizeof(vector3));
    down_rotacion=(vector3 *)malloc(sizeof(vector3));
    down_escalado=(vector3 *)malloc(sizeof(vector3));

    right_traslacion=(vector3 *)malloc(sizeof(vector3));
    right_rotacion=(vector3 *)malloc(sizeof(vector3));
    right_escalado=(vector3 *)malloc(sizeof(vector3));

    left_traslacion=(vector3 *)malloc(sizeof(vector3));
    left_rotacion=(vector3 *)malloc(sizeof(vector3));
    left_escalado=(vector3 *)malloc(sizeof(vector3));

    avpag_traslacion=(vector3 *)malloc(sizeof(vector3));
    avpag_rotacion=(vector3 *)malloc(sizeof(vector3));
    avpag_escalado=(vector3 *)malloc(sizeof(vector3));

    repag_traslacion=(vector3 *)malloc(sizeof(vector3));
    repag_rotacion=(vector3 *)malloc(sizeof(vector3));
    repag_escalado=(vector3 *)malloc(sizeof(vector3));

    menos_escalado=(vector3 *)malloc(sizeof(vector3));

    mas_escalado=(vector3 *)malloc(sizeof(vector3));


    *up_traslacion = (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f};
    *up_rotacion = (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f};
    *up_escalado = (vector3){.x = 1.0f, .y = 1.5f, .z = 1.0f};

    *down_traslacion = (vector3){.x = 0.0f, .y = -1.0f, .z = 0.0f};
    *down_rotacion = (vector3){.x = -1.0f, .y = 0.0f, .z = 0.0f};
    *down_escalado = (vector3){.x = 1.0f, .y = 0.75f, .z = 1.0f};

    *right_traslacion = (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f};
    *right_rotacion = (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f};
    *right_escalado = (vector3){.x = 1.5f, .y = 1.0f, .z = 1.0f};

    *left_traslacion = (vector3){.x = -1.0f, .y = 0.0f, .z = 0.0f};
    *left_rotacion = (vector3){.x = 0.0f, .y = -1.0f, .z = 0.0f};
    *left_escalado = (vector3){.x = 0.75f, .y = 1.0f, .z = 1.0f};

    *avpag_traslacion = (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f};
    *avpag_rotacion = (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f};
    *avpag_escalado = (vector3){.x = 1.0f, .y = 1.0f, .z = 1.5f};

    *repag_traslacion = (vector3){.x = 0.0f, .y = 0.0f, .z = -1.0f};
    *repag_rotacion = (vector3){.x = 0.0f, .y = 0.0f, .z = -1.0f};
    *repag_escalado = (vector3){.x = 1.0f, .y = 1.0f, .z = 0.75f};

    *mas_escalado = (vector3){.x = 1.5f, .y = 1.5f, .z = 1.5f};

    *menos_escalado = (vector3){.x = 0.75f, .y = 0.75f, .z = 0.75f};
}


void transformation_matrix(){

    glMatrixMode(GL_MODELVIEW);
    if (elemento == OBJETO){
        if (sis_referencia == LOCALES)
            glLoadMatrixf(_selected_object->matrix_table->matriz);
        else
            glLoadIdentity();
    }
}

void set_transformation_matrix(){

    GLfloat m_aux[16];
    if (elemento == OBJETO){
        matrices *n_ptr = (matrices*)malloc(sizeof(matrices));
        if (sis_referencia == LOCALES) {
            glGetFloatv(GL_MODELVIEW_MATRIX, n_ptr->matriz);
        }else {
            glMultMatrixf(_selected_object->matrix_table->matriz);
            glGetFloatv(GL_MODELVIEW_MATRIX, n_ptr->matriz);
        }

        n_ptr->next = _selected_object->matrix_table;
        _selected_object->matrix_table = n_ptr;

    }
}

void aplicar_transformaciones(vector3 *value){

    transformation_matrix();

    switch (modo){
        case TRASLACION:
            glTranslatef(value->x, value->y, value->z);
            break;

        case ROTACION:
            glRotatef(15.0f, value->x, value->y, value->z);
            break;

        case ESCALADO:
            if (elemento == OBJETO){
                glScalef(value->x, value->y, value->z);
            }
            break;
    }

    set_transformation_matrix();
}
