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

#define FLAT                                0
#define SMOOTH                              1

#define ACTIVADA                            1
#define DESACTIVADA                         0

#define NONE                               -1
#define SOL                                 0
#define BOMBILLA                            1
#define FOCO                                2
#define FOCO_OBJETO                         3

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
    vector3 normal;                     /* normal vector of the vertex */
} vertex;

/****************************
 * Structure to store       *
 * objects' faces or        *
 * polygons                 *
 ****************************/
typedef struct {
    GLint num_vertices;                 /* number of vertices in the face */
    GLint *vertex_table;                /* table with the index of each vertex */
    vector3 normal;                     /*normal vector of the face*/
} face;

/****************************
 * Structure to store       *
 * a list of matrixes       *
 ****************************/

struct matrices{
    GLfloat matriz[16];                 /* matrix info of matrix pointer */
    struct matrices *next;              /* pointer to the next matrix of the list */
};
typedef struct matrices matrices;

/*****************************
 * Structure to store the    *
 * Information of a material *
 *****************************/

typedef struct
{
    GLfloat m_diffuse[4];               /* vector of the diffusion values */
    GLfloat m_ambient[4];               /* vector of the ambient values */
    GLfloat m_specular[4];              /* vector of the specular values */
    GLfloat m_position[4];              /* vector of the position values */
    GLfloat no_shininess[1];            /* vector of the shininess values */
} material_light;


/****************************
 * Structure to store a     *
 * pile of 3D objects       *
 ****************************/
struct object3d{
    GLint num_vertices;                 /* number of vertices in the object*/
    vertex *vertex_table;               /* table of vertices */
    GLint num_faces;                    /* number of faces in the object */
    face *face_table;                   /* table of faces */
    matrices *matrix_table;             /* table of matrices*/
    point3 min;                         /* coordinates' lower bounds */
    point3 max;                         /* coordinates' bigger bounds */
    struct object3d *next;              /* next element in the pile of objects */
    GLint shade;                        /* shade property */
    material_light *material_light;     /* material of the object */
};

typedef struct object3d object3d;


/****************************
 * Structure to store the   *
 * projection information   *
 ****************************/

typedef struct projection
{
    GLfloat angle;                      /* angle of opening */
    GLfloat near;                       /* near of the camera */
    GLfloat far;                        /* far of the camera */
    GLfloat bottom;                     /* bottom of the camera */
    GLfloat top;                        /* top of the camera */
    GLfloat left;                       /* left edge of the projection*/
    GLfloat right;                      /* right edge of the projection*/
} projection;

/****************************
 * Structure to store a     *
 * pile of cameras          *
 ****************************/

struct camera{
    GLfloat m[16];                      /* matrix of the camera*/
    GLfloat minv[16];                   /* inverted matrix of the camera*/
    struct camera *next;                /* next element in the pile of cameras */
    int type;                           /* projection type*/
    projection *proj;                   /* projection data*/
    GLint shade;                        /* shade property */
    material_light *material_light;     /* material of the camera */
};

typedef struct camera camera;

/****************************
 * Structure to store a     *
 * source of light          *
 ****************************/

typedef struct
{
    GLfloat diffuse[4];                 /* diffusion of the source */
    GLfloat ambient[4];                 /* ambient of the source */
    GLfloat specular[4];                /* specular property of the source */
    GLfloat position[4];                /* position of the source */
    GLint is_on;                        /* value that determines if the source is on */
    GLint type;                         /* type of light-source */
    GLfloat spot_direction[3];          /* if the source is a spotlight, the direction */
    GLfloat m_obj[16];                  /* object matrix of the source to allocate it */
    GLfloat cut_off;                    /* if the source is a spotlight, the cut off */
}objetos_luz;


#endif // DEFINITIONS_H
