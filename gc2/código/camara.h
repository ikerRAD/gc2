#include "definitions.h"

void default_cameras();
void add_camera();
void cambiar_camara();
void add_camera_list(camera *l_camera);
void centre_camera_to_obj(object3d *obj);
void add_camera_mode_obj(object3d *obj);
void matriz_inversa(camera *c);
void modo_analisis(int x, int y);
void set_objectlike_matrix(GLfloat *m, GLfloat *mobj);