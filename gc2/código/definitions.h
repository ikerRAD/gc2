#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <GL/gl.h>

/** DEFINITIONS **/

#define KG_WINDOW_TITLE                     "Práctica GPO"
#define KG_WINDOW_WIDTH                     600
#define KG_WINDOW_HEIGHT                    400
#define KG_WINDOW_X                         50
#define KG_WINDOW_Y                         50

#define KG_MSSG_SELECT_FILE                 "Escribe el path al archivo: "
#define KG_MSSG_FILENOTFOUND                "Ese archivo no existe!!"
#define KG_MSSG_INVALIDFILE                 "Ha habido un problema con el fichero ..."
#define KG_MSSG_EMPTYFILE                   "El fichero está vacío"
#define KG_MSSG_FILEREAD                    "Fichero leído correctamente"

#define KG_STEP_MOVE                        5.0f
#define KG_STEP_ROTATE                      10.0f
#define KG_STEP_ZOOM                        0.75
#define KG_STEP_CAMERA_ANGLE                5.0f

#define KG_ORTHO_X_MIN_INIT                -5
#define KG_ORTHO_X_MAX_INIT                 5
#define KG_ORTHO_Y_MIN_INIT                -5
#define KG_ORTHO_Y_MAX_INIT                 5
#define KG_ORTHO_Z_MIN_INIT                -100
#define KG_ORTHO_Z_MAX_INIT                 10000

#define KG_COL_BACK_R                       0.30f
#define KG_COL_BACK_G                       0.30f
#define KG_COL_BACK_B                       0.30f
#define KG_COL_BACK_A                       1.00f

#define KG_COL_SELECTED_R                   1.00f
#define KG_COL_SELECTED_G                   0.75f
#define KG_COL_SELECTED_B                   0.00f

#define KG_COL_NONSELECTED_R                1.00f
#define KG_COL_NONSELECTED_G                1.00f
#define KG_COL_NONSELECTED_B                1.00f

#define KG_COL_X_AXIS_R                     1.0f
#define KG_COL_X_AXIS_G                     0.0f
#define KG_COL_X_AXIS_B                     0.0f

#define KG_COL_Y_AXIS_R                     0.0f
#define KG_COL_Y_AXIS_G                     1.0f
#define KG_COL_Y_AXIS_B                     0.0f

#define KG_COL_Z_AXIS_R                     0.0f
#define KG_COL_Z_AXIS_G                     0.0f
#define KG_COL_Z_AXIS_B                     1.0f

#define KG_MAX_DOUBLE                       10E25

/**  ESTADOS  **/

#define TRASLACION                          0
#define ROTACION                            1
#define ESCALADO                            2

#define GLOBALES                            0
#define LOCALES                             1

#define OBJETO                              0
#define CAMARA                              1
#define ILUMINACION                         2
#define OBJETOCAMARA                        3

#define PERSPECTIVA                         0
#define PARALELA                            1

#define VUELO                               0
#define ANALISIS                            1

/** STRUCTURES **/

/****************************
 * Structure to store the   *
 * coordinates of 3D points *
 ****************************/
typedef struct {
    GLdouble x, y, z;
} point3;

/*****************************
 * Structure to store the    *
 * coordinates of 3D vectors *
 *****************************/
typedef struct {
    GLdouble x, y, z;
} vector3;

/****************************
 * Structure to store the   *
 * colors in RGB mode       *
 ****************************/
typedef struct {
    GLfloat r, g, b;
} color3;

/****************************
 * Structure to store       *
 * objects' vertices         *
 ****************************/
typedef struct {
    point3 coord;                       /* coordinates,x, y, z */
    GLint num_faces;                    /* number of faces that share this vertex */
} vertex;

/****************************
 * Structure to store       *
 * objects' faces or        *
 * polygons                 *
 ****************************/
typedef struct {
    GLint num_vertices;                 /* number of vertices in the face */
    GLint *vertex_table;                /* table with the index of each vertex */
} face;

/****************************
 * Structure to store       *
 * a list of matrixes       *
 ****************************/

struct matrices{
    GLfloat matriz[16];
    struct matrices *next;
};
typedef struct matrices matrices;
/****************************
 * Structure to store a     *
 * pile of 3D objects       *
 ****************************/
struct object3d{
    GLint num_vertices;                 /* number of vertices in the object*/
    vertex *vertex_table;               /* table of vertices */
    GLint num_faces;                    /* number of faces in the object */
    face *face_table;                   /* table of faces */
    matrices *matrix_table;             /* table of matrixes*/
    point3 min;                         /* coordinates' lower bounds */
    point3 max;                         /* coordinates' bigger bounds */
    struct object3d *next;              /* next element in the pile of objects */
};

typedef struct object3d object3d;


/****************************
 * Structures to store a     *
 * pile of cameras       *
 ****************************/

typedef struct projection
{
    GLfloat angle;
    GLfloat near;
    GLfloat far;
    GLfloat bottom;
    GLfloat top;
    GLfloat left;
    GLfloat right;
} projection;

struct camera{
    GLint num_vertices;                 /* number of vertices in the camera*/
    vertex *vertex_table;               /* table of vertices */
    GLint num_faces;                    /* number of faces in the object */
    face *face_table;                   /* table of faces */
    GLfloat m[16];                      /* matrix of the camera*/
    GLfloat minv[16];                   /* inverted matrix of the camera*/
    point3 min;                         /* coordinates' lower bounds */
    point3 max;                         /* coordinates' bigger bounds */
    struct camera *next;                /* next element in the pile of cameras */
    int type;                           /* projection type*/
    projection *proj;                    /* projection data*/
};

typedef struct camera camera;


#endif // DEFINITIONS_H
