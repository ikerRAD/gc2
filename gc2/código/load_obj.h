#ifndef READ_OBJ_H
#define	READ_OBJ_H

#include "definitions.h"

int read_wavefront(char * file_name, object3d * object_ptr);
void load_camera_representation(camera *cam);

#endif	/* READ_OBJ_H */

