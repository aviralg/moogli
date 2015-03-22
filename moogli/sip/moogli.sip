%Module _moogli

%Import QtGui/QtGuimod.sip
%Import QtOpenGL/QtOpenGLmod.sip


class Network
{

%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Network.hpp"
#include "core/Neuron.hpp"
%End

public:

    Network(const char * id);

    ~Network();

    const char *
    get_id();

    void
    hide();

    void
    show();

    bool
    is_visible();

    unsigned int
    size();

    unsigned int
    add_neuron(Neuron * neuron /Transfer/);

    unsigned int
    remove_neuron(Neuron * neuron);

    Neuron *
    get_neuron(const char * id);

};


class Neuron
{

%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Compartment.hpp"
#include "core/Neuron.hpp"
%End

public:

    Neuron(const char * id);

    const char *
    get_id();

    Network *
    get_network();

    void
    hide();

    void
    show();

    bool
    is_visible();

    unsigned int
    size();

    unsigned int
    add_geometry( PyObject * distal
                , PyObject * proximal = Py_None
                );

    unsigned int
    add_compartment(Compartment * compartment /Transfer/);

    unsigned int
    remove_compartment(Compartment * compartment);

    void
    show_geometry(unsigned int geometry_index, bool hide_others);

    void
    hide_geometry(unsigned int geometry_index);

    void
    show_all_geometries();

    void
    hide_all_geometries();

    Compartment *
    get_compartment(const char * id);

};

class Compartment
{
%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Compartment.hpp"
#include "core/Voxel.hpp"
%End

public:

    Compartment(const char * id);

    ~Compartment();

    const char *
    get_id();

    Neuron *
    get_neuron();

    void
    hide();

    void
    show();

    bool
    is_visible();

    unsigned int
    size();

    unsigned int
    add_geometry( PyObject * distal
                , PyObject * proximal = Py_None
                );

    unsigned int
    add_voxel(Voxel * voxel /Transfer/);

    unsigned int
    remove_voxel(Voxel * voxel);

    void
    show_geometry(unsigned int geometry_index, bool hide_others);

    void
    hide_geometry(unsigned int geometry_index);

    void
    show_all_geometries();

    void
    hide_all_geometries();
};

class Voxel
{
%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Voxel.hpp"
%End

public:

    Voxel(const char * id);

    ~Voxel();

    const char *
    get_id();

    Compartment *
    get_compartment();

    void
    hide();

    void
    show();

    bool
    is_visible();

    void
    set_geometry( PyObject * distal
                , PyObject * proximal = Py_None
                );

};

class NetworkViewer : public QGLWidget
{

%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Network.hpp"
#include "view/NetworkViewer.hpp"
%End

public:

    Network *
    get_network();

    NetworkViewer( Network * network
                          , QWidget * parent             = 0
                          , const QGLWidget* shareWidget = 0
                          , Qt::WindowFlags f            = 0
                          );
    void
    add_view( int x
            , int y
            , int width
            , int height
            );

    void
    split_horizontally( unsigned int view_index   = 0
                      , unsigned int width_factor = 2
                      );

    void
    split_vertically( unsigned int view_index    = 0
                    , unsigned int height_factor = 2
                    );

    void
    home(unsigned int index = 0);

    void
    forward( double distance
           , unsigned int index = 0
           );
    void
    backward( double distance
            , unsigned int index = 0
            );
    void
    left( double distance
        , unsigned int index = 0
        );
    void
    right( double distance
         , unsigned int index = 0
         );

    void
    up( double distance
      , unsigned int index = 0
      );

    void
    down( double distance
        , unsigned int index = 0
        );

    void
    zoom( double factor
        , unsigned int index = 0
        );

    void
    roll( double angle
        , unsigned int index = 0
        );

    void
    pitch( double angle
         , unsigned int index = 0
         );

    void
    yaw( double angle
       , unsigned int index = 0
       );

    // void
    // capture_continuous_toggle(unsigned int index);

    // void
    // capture_once(unsigned int index);

    virtual
    ~NetworkViewer();

protected:

    virtual void paintEvent( QPaintEvent* paintEvent );
    virtual void paintGL();
    virtual void resizeGL( int width, int height );

    virtual void keyPressEvent( QKeyEvent* event );
    virtual void keyReleaseEvent( QKeyEvent* event );

    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void wheelEvent( QWheelEvent* event );

    virtual bool event( QEvent* event );
};
