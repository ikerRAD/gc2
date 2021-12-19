#include <GL/glut.h>
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "io.h"
//git
extern object3d *_selected_object;
extern material_light *ruby, *obsidian ,*gold, *mat_camara, *mat_selec, *red_plastic, *mat_foco, *copper;
extern int _selected_light;
extern camera * _selected_camera;
extern objetos_luz global_lights[8];
extern object3d *cam_object;

/**
 * función para colocar las luces con los parámetros especificados
 * @param i indice de la luz
 */
void put_light(GLint i){
    switch (i){
        case 0:
            glLightfv(GL_LIGHT0, GL_POSITION, global_lights[i].position);
            glLightfv(GL_LIGHT0, GL_AMBIENT, global_lights[i].ambient);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, global_lights[i].diffuse);
            glLightfv(GL_LIGHT0, GL_SPECULAR, global_lights[i].specular);
            break;

        case 1:
            glLightfv(GL_LIGHT1, GL_AMBIENT, global_lights[i].ambient);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, global_lights[i].diffuse);
            glLightfv(GL_LIGHT1, GL_SPECULAR, global_lights[i].specular);
            glLightfv(GL_LIGHT1, GL_POSITION, global_lights[i].position);
            break;

        case 2:
            glLightfv(GL_LIGHT2, GL_AMBIENT, global_lights[i].ambient);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, global_lights[i].diffuse);
            glLightfv(GL_LIGHT2, GL_SPECULAR, global_lights[i].specular);
            glLightfv(GL_LIGHT2, GL_POSITION, global_lights[i].position);
            glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, global_lights[i].cut_off);
            break;

        case 3:
            glLightfv(GL_LIGHT3, GL_AMBIENT, global_lights[i].ambient);
            glLightfv(GL_LIGHT3, GL_DIFFUSE, global_lights[i].diffuse);
            glLightfv(GL_LIGHT3, GL_SPECULAR, global_lights[i].specular);
            glLightfv(GL_LIGHT3, GL_POSITION, global_lights[i].position);
            glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, global_lights[i].cut_off);

            break;

        case 4:
            glLightfv(GL_LIGHT4, GL_AMBIENT, global_lights[i].ambient);
            glLightfv(GL_LIGHT4, GL_DIFFUSE, global_lights[i].diffuse);
            glLightfv(GL_LIGHT4, GL_SPECULAR, global_lights[i].specular);
            glLightfv(GL_LIGHT4, GL_POSITION, global_lights[i].position);

            if (global_lights[i].type == FOCO){
                glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
                glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, global_lights[i].cut_off);
            }
            break;

        case 5:
            glLightfv(GL_LIGHT5, GL_AMBIENT, global_lights[i].ambient);
            glLightfv(GL_LIGHT5, GL_DIFFUSE, global_lights[i].diffuse);
            glLightfv(GL_LIGHT5, GL_SPECULAR, global_lights[i].specular);
            glLightfv(GL_LIGHT5, GL_POSITION, global_lights[i].position);

            if (global_lights[i].type == FOCO){
                glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
                glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, global_lights[i].cut_off);
            }
            break;

        case 6:
            glLightfv(GL_LIGHT6, GL_AMBIENT, global_lights[i].ambient);
            glLightfv(GL_LIGHT6, GL_DIFFUSE, global_lights[i].diffuse);
            glLightfv(GL_LIGHT6, GL_SPECULAR, global_lights[i].specular);
            glLightfv(GL_LIGHT6, GL_POSITION, global_lights[i].position);

            if (global_lights[i].type == FOCO){
                glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
                glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, global_lights[i].cut_off);
            }
            break;

        case 7:
            glLightfv(GL_LIGHT7, GL_AMBIENT, global_lights[i].ambient);
            glLightfv(GL_LIGHT7, GL_DIFFUSE, global_lights[i].diffuse);
            glLightfv(GL_LIGHT7, GL_SPECULAR, global_lights[i].specular);
            glLightfv(GL_LIGHT7, GL_POSITION, global_lights[i].position);

            if (global_lights[i].type == FOCO){
                glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
                glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, global_lights[i].cut_off);
            }
            break;

        default:
            break;
    }
}
/**
 * función para obtener la matriz de objeto de un foco asociado a una entidad
 * @param f 2-foco del objeto seleccionado; 3-foco de la cámara seleccionada
 */
void m_foco(int f){
    int i;
    for (i = 0; i < 16; i++){
        if(f==2) {
            global_lights[f].m_obj[i] = _selected_object->matrix_table->matriz[i];
        }else if(f==3){
            global_lights[f].m_obj[i] = _selected_camera->minv[i];
        }
    }
}
/**
 * Establece los parámetros del foco asociado a la cámara.
 * Está pensado para aplicarse en el caso de que las cámaras tengan distintas representaciones
 */
void foco_camara(){
    global_lights[3].position[0] = (cam_object->max.x + cam_object->min.x) / 2;
    global_lights[3].position[1] = (cam_object->max.y + cam_object->min.y) / 2;
    global_lights[3].position[2] = (cam_object->max.z + cam_object->min.z) / 2;
    global_lights[3].position[3] = 1.0f;

    global_lights[3].ambient[0] = 1.5f;
    global_lights[3].ambient[1] = 1.5f;
    global_lights[3].ambient[2] = 1.5f;
    global_lights[3].ambient[3] = 1.0f;

    global_lights[3].diffuse[0] = 1.5f;
    global_lights[3].diffuse[1] = 1.5f;
    global_lights[3].diffuse[2] = 1.5f;
    global_lights[3].diffuse[3] = 1.0f;

    global_lights[3].specular[0] = 1.0f;
    global_lights[3].specular[1] = 1.0f;
    global_lights[3].specular[2] = 1.0f;
    global_lights[3].specular[3] = 1.0f;

    global_lights[3].cut_off = 45.0f;

    global_lights[3].spot_direction[0] = 0;
    global_lights[3].spot_direction[1] = 0;
    global_lights[3].spot_direction[2] = -1;

    m_foco(3);

}
/**
 * Función para establecer los valores por defecto del foco asociado al objeto
 */
void foco_obj(){

    global_lights[2].ambient[0] = 1.5f;
    global_lights[2].ambient[1] = 1.5f;
    global_lights[2].ambient[2] = 1.5f;
    global_lights[2].ambient[3] = 1.0f;

    global_lights[2].diffuse[0] = 1.5f;
    global_lights[2].diffuse[1] = 1.5f;
    global_lights[2].diffuse[2] = 1.5f;
    global_lights[2].diffuse[3] = 1.0f;

    global_lights[2].specular[0] = 1.0f;
    global_lights[2].specular[1] = 1.0f;
    global_lights[2].specular[2] = 1.0f;
    global_lights[2].specular[3] = 1.0f;

    global_lights[2].cut_off = 45.0f;

    global_lights[2].spot_direction[0] = 0.0f;
    global_lights[2].spot_direction[1] = 0.0f;
    global_lights[2].spot_direction[2] = 1.0f;

}
/**
 * Inicializa los valores por defecto de las luces
 */
void inicializar_luces(){


    global_lights[0].position[0] = 1.0f;
    global_lights[0].position[1] = 1.0f;
    global_lights[0].position[2] = 0.0f;
    global_lights[0].position[3] = 1.0f;
    global_lights[0].ambient[0] = 1.2f;
    global_lights[0].ambient[1] = 1.2f;
    global_lights[0].ambient[2] = 1.2f;
    global_lights[0].ambient[3] = 1.0f;
    global_lights[0].diffuse[0] = 1.0f;
    global_lights[0].diffuse[1] = 1.0f;
    global_lights[0].diffuse[2] = 1.0f;
    global_lights[0].diffuse[3] = 1.0f;
    global_lights[0].specular[0] = 1.0f;
    global_lights[0].specular[1] = 1.0f;
    global_lights[0].specular[2] = 1.0f;
    global_lights[0].specular[3] = 1.0f;

    identity(global_lights[0].m_obj);
    global_lights[0].type = BOMBILLA;
    global_lights[0].is_on = 0;


    global_lights[1].position[0] = 0.0f;
    global_lights[1].position[1] = 1.0f;
    global_lights[1].position[2] = 0.0f;
    global_lights[1].position[3] = 0.0f;
    global_lights[1].ambient[0] = 1.2f;
    global_lights[1].ambient[1] = 1.2f;
    global_lights[1].ambient[2] = 1.2f;
    global_lights[1].ambient[3] = 1.0f;
    global_lights[1].diffuse[0] = 1.0f;
    global_lights[1].diffuse[1] = 1.0f;
    global_lights[1].diffuse[2] = 1.0f;
    global_lights[1].diffuse[3] = 1.0f;
    global_lights[1].specular[0] = 1.0f;
    global_lights[1].specular[1] = 1.0f;
    global_lights[1].specular[2] = 1.0f;
    global_lights[1].specular[3] = 1.0f;

    identity(global_lights[1].m_obj);
    global_lights[1].type = SOL;
    global_lights[1].is_on = 0;

    global_lights[2].type = FOCO_OBJETO;
    global_lights[3].type = FOCO_OBJETO;
    for(int i=2; i<8; i++) {
        global_lights[i].is_on = 0;
        if(i>3){
            global_lights[i].type = NONE;
        }
    }

    foco_camara();
    foco_obj();

}
/**
 * Función que cambia los valores necesarios al cambiar de objeto seleccionado
 */
void cambio_foco(){

    if(_selected_object!=0) {
        global_lights[2].position[0] = (_selected_object->max.x + _selected_object->min.x) / 2;
        global_lights[2].position[1] = (_selected_object->max.y + _selected_object->min.y) / 2;
        global_lights[2].position[2] = (_selected_object->max.z + _selected_object->min.z) / 2;
        global_lights[2].position[3] = 1;

        m_foco(2);
    }else{
        global_lights[2].is_on = 0;
    }

}
/**
 * función para añadir una luz a los espacios 5,6,7,8
 */
void anadir_luz(){
    GLint luzz, values;


    if(_selected_light>=0 && _selected_light<=3){
        printf("Error, ten seleccionada una luz entre la 5 y la 8\n");
        return;
    }


    printf("Elige el tipo de luz: 1 SOL, 2 BOMBILLA, 3 FOCO.\n");
    scanf("%d", &luzz);

    while (luzz < 1 || luzz > 3){
        printf("Error, elije entre 1 y 3");
        scanf("%d", &luzz);
    }

    global_lights[_selected_light].ambient[0] = 1.2f;
    global_lights[_selected_light].ambient[1] = 1.2f;
    global_lights[_selected_light].ambient[2] = 1.2f;
    global_lights[_selected_light].ambient[3] = 1.0f;

    global_lights[_selected_light].diffuse[0] = 1.0f;
    global_lights[_selected_light].diffuse[1] = 1.0f;
    global_lights[_selected_light].diffuse[2] = 1.0f;
    global_lights[_selected_light].diffuse[3] = 1.0f;

    global_lights[_selected_light].specular[0] = 1.0f;
    global_lights[_selected_light].specular[1] = 1.0f;
    global_lights[_selected_light].specular[2] = 1.0f;
    global_lights[_selected_light].specular[3] = 1.0f;

    if(luzz == 1) {
        global_lights[_selected_light].position[1] = 1.0;
        global_lights[_selected_light].position[3] = 0.0;
    }else{

        global_lights[_selected_light].position[1] = 0.0;
        global_lights[_selected_light].position[3] = 1.0;
    }
    global_lights[_selected_light].position[2] = 0.0;
    global_lights[_selected_light].position[0] = 0.0;

    identity(global_lights[_selected_light].m_obj);

    switch (luzz){
        case 1:
            global_lights[_selected_light].type = SOL;
            break;

        case 2:
            global_lights[_selected_light].type = BOMBILLA;
            break;

        case 3:
            global_lights[_selected_light].type = FOCO;

            global_lights[_selected_light].cut_off = 45.0f;
            global_lights[_selected_light].spot_direction[0] = 0.0f;
            global_lights[_selected_light].spot_direction[1] = 0.0f;
            global_lights[_selected_light].spot_direction[2] = 1.0f;

            break;

    }

    global_lights[_selected_light].is_on = 0;
    printf("luz %d preparada, enciendela para usarla\n",_selected_light+1);

}
/**
 * Función que inicaliza todos los materiales disponibles
 */
void inicializar_materiales(){
    ruby            = (material_light*)malloc(sizeof(material_light));
    obsidian        = (material_light*)malloc(sizeof(material_light));
    gold            = (material_light*)malloc(sizeof(material_light));
    mat_camara      = (material_light*)malloc(sizeof(material_light));
    mat_selec       = (material_light*)malloc(sizeof(material_light));
    red_plastic         = (material_light*)malloc(sizeof(material_light));
    mat_foco        = (material_light*)malloc(sizeof(material_light));
    copper    = (material_light*)malloc(sizeof(material_light));

    ruby->m_ambient[0] = 0.1745f;
    ruby->m_ambient[1] = 0.01175f;
    ruby->m_ambient[2] = 0.01175f;
    ruby->m_ambient[3] = 0.55f;
    ruby->m_diffuse[0] = 0.61424f;
    ruby->m_diffuse[1] = 0.04136f;
    ruby->m_diffuse[2] = 0.04136f;
    ruby->m_diffuse[3] = 0.55f;
    ruby->m_specular[0] = 0.727811f;
    ruby->m_specular[1] = 0.626959f;
    ruby->m_specular[2] = 0.626959f;
    ruby->m_specular[3] = 0.55f;
    ruby->no_shininess[0] = 76.8f;

    obsidian->m_ambient[0] = 0.05375f;
    obsidian->m_ambient[1] = 0.05f;
    obsidian->m_ambient[2] = 0.06625f;
    obsidian->m_ambient[3] = 0.82f;
    obsidian->m_diffuse[0] = 0.18275f;
    obsidian->m_diffuse[1] = 0.17f;
    obsidian->m_diffuse[2] = 0.22525f;
    obsidian->m_diffuse[3] = 0.82f;
    obsidian->m_specular[0] = 0.332741f;
    obsidian->m_specular[1] = 0.328634f;
    obsidian->m_specular[2] = 0.346435f;
    obsidian->m_specular[3] = 0.82f;
    obsidian->no_shininess[0] = 38.4f;

    gold->m_ambient[0] = 0.24725f;
    gold->m_ambient[1] = 0.1995f;
    gold->m_ambient[2] = 0.0745f;
    gold->m_ambient[3] = 1.0f;
    gold->m_diffuse[0] = 0.75164f;
    gold->m_diffuse[1] = 0.60648f;
    gold->m_diffuse[2] = 0.22648f;
    gold->m_diffuse[3] = 1.0f;
    gold->m_specular[0] = 0.797357f;
    gold->m_specular[1] = 0.723991f;
    gold->m_specular[2] = 0.208006f;
    gold->m_specular[3] = 1.0f;
    gold->no_shininess[0] = 83.2f;

    //ESMERALDA
    mat_camara->m_ambient[0] =0.0215f;
    mat_camara->m_ambient[1] = 0.1745f;
    mat_camara->m_ambient[2] = 0.0215f;
    mat_camara->m_ambient[3] = 0.55f ;
    mat_camara->m_diffuse[0] = 0.07568f;
    mat_camara->m_diffuse[1] = 0.61424f;
    mat_camara->m_diffuse[2] = 0.07568f;
    mat_camara->m_diffuse[3] = 0.55f ;
    mat_camara->m_specular[0] = 0.633f;
    mat_camara->m_specular[1] = 0.727811f;
    mat_camara->m_specular[2] = 0.633f;
    mat_camara->m_specular[3] = 0.55f ;
    mat_camara->no_shininess[0] = 76.8f;

    //PERL
    mat_selec->m_ambient[0] =0.25f;
    mat_selec->m_ambient[1] = 0.20725f;
    mat_selec->m_ambient[2] = 0.20725f;
    mat_selec->m_ambient[3] = 0.922f ;
    mat_selec->m_diffuse[0] = 1.0f;
    mat_selec->m_diffuse[1] = 0.829f;
    mat_selec->m_diffuse[2] = 0.829f;
    mat_selec->m_diffuse[3] = 0.922f ;
    mat_selec->m_specular[0] = 0.296648f;
    mat_selec->m_specular[1] = 0.296648f;
    mat_selec->m_specular[2] = 0.296648f;
    mat_selec->m_specular[3] =  0.922f ;
    mat_selec->no_shininess[0] =11.264f;

    //RED PLASTIC
    red_plastic->m_ambient[0] =0.0f;
    red_plastic->m_ambient[1] = 0.0f;
    red_plastic->m_ambient[2] = 0.0f;
    red_plastic->m_ambient[3] = 1.0f ;
    red_plastic->m_diffuse[0] = 0.5f;
    red_plastic->m_diffuse[1] = 0.0f;
    red_plastic->m_diffuse[2] = 0.0f;
    red_plastic->m_diffuse[3] = 1.0f ;
    red_plastic->m_specular[0] = 0.7f;
    red_plastic->m_specular[1] = 0.6f;
    red_plastic->m_specular[2] = 0.6f;
    red_plastic->m_specular[3] =  1.0f ;
    red_plastic->no_shininess[0] =32.0f;

    //CYAN RUBBER
    mat_foco->m_ambient[0] = 0.0f;
    mat_foco->m_ambient[1] = 0.05f;
    mat_foco->m_ambient[2] = 0.05f;
    mat_foco->m_ambient[3] = 1.0f;
    mat_foco->m_diffuse[0] = 0.4f;
    mat_foco->m_diffuse[1] = 0.5f;
    mat_foco->m_diffuse[2] = 0.5f;
    mat_foco->m_diffuse[3] = 1.0f;
    mat_foco->m_specular[0] = 0.04f;
    mat_foco->m_specular[1] = 0.7f;
    mat_foco->m_specular[2] = 0.7f;
    mat_foco->m_specular[3] =  1.0f;
    mat_foco->no_shininess[0] =10.0f;

    //copper
    copper->m_ambient[0] = 0.19125f;
    copper->m_ambient[1] = 0.0735f;
    copper->m_ambient[2] = 0.0225f;
    copper->m_ambient[3] = 1.0f;
    copper->m_diffuse[0] = 0.7038f;
    copper->m_diffuse[1] = 0.27048f;
    copper->m_diffuse[2] = 0.0828f;
    copper->m_diffuse[3] = 1.0f;
    copper->m_specular[0] = 0.256777f;
    copper->m_specular[1] = 0.137622f;
    copper->m_specular[2] = 0.086014f;
    copper->m_specular[3] =  1.0f;
    copper->no_shininess[0] =12.8f;
}
/**
 * función que agreaga un material por defecto a un objeto
 */
void anadir_material(){
    _selected_object->material_light = ruby;
}
/**
 * función que shiftea el material del objeto
 */
void cambiar_material(){
    if (_selected_object->material_light == ruby){
        _selected_object->material_light = obsidian;
    } else if (_selected_object->material_light == obsidian){
        _selected_object->material_light = gold;
    }else if (_selected_object->material_light == gold){
        _selected_object->material_light = red_plastic;
    }else if (_selected_object->material_light == red_plastic){
        _selected_object->material_light = copper;
    }else{
        _selected_object->material_light = ruby;
    }
}