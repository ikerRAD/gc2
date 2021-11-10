#include "definitions.h"

void default_cameras();
void add_camera();
void set_camera_projection();
void cambiar_camara();
void add_camera_list(camera *l_camera);
void centre_camera_to_obj(object3d *obj);
void add_camera_mode_obj(object3d *obj);
void matriz_inversa(camera *c);
void modo_analisis(int x, int y);