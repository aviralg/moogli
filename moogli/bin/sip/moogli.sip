%Module _moogli

%Import QtGui/QtGuimod.sip
%Import QtOpenGL/QtOpenGLmod.sip

// %ModuleCode

// PySequence * ELECTRICAL_COMPARTMENT_COLOR
// {
//     %GetCode
//         sipPy = vec4f_to_pysequence(DEFAULT_ELECTRICAL_COMPARTMENT_COLOR);
//     %End
// }

// %End

class Visualizable /Abstract/
{
%TypeHeaderCode
#include "core/Visualizable.hpp"
%End
public:
    const char * id
    {
        %GetCode
            sipPy = PyString_FromString(sipCpp -> id.c_str());
        %End
        %SetCode
            sipErr = 1;
            PyErr_SetString( PyExc_AttributeError
                           , "Cannot change id of object."
                           );
        %End
    };

    PyObject * proximal
    {
        %GetCode
            sipPy = PyTuple_New(4);
            PyTuple_SET_ITEM(sipPy, 0, PyFloat_FromDouble(sipCpp -> proximal[0]));
            PyTuple_SET_ITEM(sipPy, 1, PyFloat_FromDouble(sipCpp -> proximal[1]));
            PyTuple_SET_ITEM(sipPy, 2, PyFloat_FromDouble(sipCpp -> proximal[2]));
            PyTuple_SET_ITEM(sipPy, 3, PyFloat_FromDouble(sipCpp -> proximal[3]));
        %End
        %SetCode
        if(PySequence_Check(sipPy))
        {
            if(PySequence_Length(sipPy) == 4)
            {
                sipCpp -> proximal[0] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 0)));
                sipCpp -> proximal[1] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 1)));
                sipCpp -> proximal[2] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 2)));
                sipCpp -> proximal[3] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 3)));
            }
            else
            {
                sipErr = 1;
                PyErr_SetString( PyExc_IndexError
                               , "Expected a sequence of size 4, (x, y, z, r)"
                               );
            }
        }
        else
        {
            sipErr = 1;
            PyErr_SetString( PyExc_TypeError
                           , "Expected a sequence such as list, tuple or numpy array"
                           );
        }
        %End
    };

    PyObject * distal
    {
        %GetCode
            sipPy = PyTuple_New(4);
            PyTuple_SET_ITEM(sipPy, 0, PyFloat_FromDouble(sipCpp -> distal[0]));
            PyTuple_SET_ITEM(sipPy, 1, PyFloat_FromDouble(sipCpp -> distal[1]));
            PyTuple_SET_ITEM(sipPy, 2, PyFloat_FromDouble(sipCpp -> distal[2]));
            PyTuple_SET_ITEM(sipPy, 3, PyFloat_FromDouble(sipCpp -> distal[3]));
        %End
        %SetCode
        if(PySequence_Check(sipPy))
        {
            if(PySequence_Length(sipPy) == 4)
            {
                sipCpp -> distal[0] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 0)));
                sipCpp -> distal[1] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 1)));
                sipCpp -> distal[2] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 2)));
                sipCpp -> distal[3] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 3)));
            }
            else
            {
                sipErr = 1;
                PyErr_SetString( PyExc_IndexError
                               , "Expected a sequence of size 4, (x, y, z, r)"
                               );
            }
        }
        else
        {
            sipErr = 1;
            PyErr_SetString( PyExc_TypeError
                           , "Expected a sequence such as list, tuple or numpy array"
                           );
        }
        %End
    };

    void
    set_color(PyObject * color_seq, int index = -1);
    %MethodCode
        if(PySequence_Check(a0))
        {
            if(PySequence_Length(a0) == 4)
            {
                Vec4f color( static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 0)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 1)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 2)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 3)))
                           );
                sipCpp -> set_color(color, a1);
            }
            else
            {
                sipError = sipErrorFail;
                PyErr_SetString( PyExc_IndexError
                               , "Expected a sequence of size 4,(red, green, blue, alpha)"
                               );
            }
        }
        else
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_TypeError
                           , "Expected a sequence such as list, tuple or numpy array"
                           );
       }
    %End

    // PyObject *
    // get_color(int index = 1);
    // %MethodCode
    //     int max_index = static_cast<int>(sipCpp -> node -> getNumChildren());
    //     if(a0 < max_index)
    //     {
    //         Vec4f color = sipCpp -> get_color(a0);
    //         sipRes = PyTuple_New(4);
    //         PyTuple_SET_ITEM(sipRes, PyFloat_FromDouble(color[0]), 0)
    //         PyTuple_SET_ITEM(sipRes, PyFloat_FromDouble(color[1]), 1)
    //         PyTuple_SET_ITEM(sipRes, PyFloat_FromDouble(color[2]), 2)
    //         PyTuple_SET_ITEM(sipRes, PyFloat_FromDouble(color[3]), 3)
    //     }
    //     else
    //     {
    //         sipError = sipErrorFail;
    //         PyErr_SetString( PyExc_IndexError
    //                        , ("Expected an index < " + to_string(max_index)).c_str()
    //                        );
    //     }
    // %End

    void
    hide(int index);

    void
    show(int index);

    bool
    visible(int index);

    void
    add_representation(bool as_sphere = false);

    unsigned int
    representation_count();


    // double r
    // {
    //     %GetCode
    //         sipPy = PyFloat_FromDouble(sipCpp -> r);
    //     %End
    //     %SetCode
    //         if(PyFloat_Check(sipPy))
    //         {
    //             sipCpp -> r = PyFloat_AS_DOUBLE(sipPy);
    //         }
    //         else
    //         {
    //             sipErr = 1;
    //             PyErr_SetString( PyExc_TypeError
    //                            , "Expected a value of type float"
    //                            );
    //         }
    //     %End
    // };

    // double x
    // {
    //     %GetCode
    //         sipPy = PyFloat_FromDouble(sipCpp -> x);
    //     %End
    //     %SetCode
    //         if(PyFloat_Check(sipPy))
    //         {
    //             sipCpp -> x = PyFloat_AS_DOUBLE(sipPy);
    //         }
    //         else
    //         {
    //             sipErr = 1;
    //             PyErr_SetString( PyExc_TypeError
    //                            , "Expected a value of type float"
    //                            );
    //         }
    //     %End
    // };


    // double y
    // {
    //     %GetCode
    //         sipPy = PyFloat_FromDouble(sipCpp -> y);
    //     %End
    //     %SetCode
    //         if(PyFloat_Check(sipPy))
    //         {
    //             sipCpp -> y = PyFloat_AS_DOUBLE(sipPy);
    //         }
    //         else
    //         {
    //             sipErr = 1;
    //             PyErr_SetString( PyExc_TypeError
    //                            , "Expected a value of type float"
    //                            );
    //         }
    //     %End
    // };

    // double x
    // {
    //     %GetCode
    //         sipPy = PyFloat_FromDouble(sipCpp -> z);
    //     %End
    //     %SetCode
    //         if(PyFloat_Check(sipPy))
    //         {
    //             sipCpp -> z = PyFloat_AS_DOUBLE(sipPy);
    //         }
    //         else
    //         {
    //             sipErr = 1;
    //             PyErr_SetString( PyExc_TypeError
    //                            , "Expected a value of type float"
    //                            );
    //         }
    //     %End
    // };


    // Visualizable();
    // virtual const char *
    // get_id() = 0;//    virtual

    // virtual void
    // set_radius();
    /* data */
};


class Network
{
%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Network.hpp"
#include "core/Neuron.hpp"
%End

public:

    const char * id
    {
        %GetCode
            sipPy = PyString_FromString(sipCpp -> id.c_str());
        %End
        %SetCode
            sipErr = 1;
            PyErr_SetString( PyExc_AttributeError
                           , "Cannot change id of object."
                           );
        %End
    };

    Network(const char * id);

    ~Network();

    void
    hide();

    void
    show();

    bool
    visible() const;

    void
    attach(Neuron * neuron /Transfer/);
    %MethodCode
        if(!sipCpp -> attach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("Neuron(" + a0 -> id + ") is already attached to Network(" + sipCpp -> id + ").").c_str()
                           );
        }
    %End

    void
    detach(Neuron * neuron);
    %MethodCode
        if(!sipCpp -> detach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("Neuron(" + a0 -> id + ") is not attached to Network(" + sipCpp -> id + ").").c_str()
                           );
        }
    %End

    SIP_PYTUPLE
    neurons() const;
    %MethodCode
        sipRes = PyTuple_New(sipCpp -> size());
        SIP_SSIZE_T index = 0;
        for ( const auto &neuron_pair : sipCpp -> neuron_map )
        {
            PyTuple_SET_ITEM( sipRes
                            , index
                            , sipConvertFromType( neuron_pair.second
                                                , sipFindType("Neuron")
                                                , NULL
                                                )
                            );
            index += 1;
        }
    %End

    SIP_SSIZE_T
    size() const /__len__/;

    int
    __contains__(Neuron * neuron) const;
    %MethodCode
        return static_cast<int>(sipCpp -> get(a0 -> id) != nullptr);
    %End

    SIP_PYOBJECT
    __getitem__(const char * neuron_id) const;
    %MethodCode
        Neuron * neuron = sipCpp -> get(a0);
        if(neuron == nullptr)
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_KeyError
                           , a0
                           );
        }
        else
        {
            sipRes = sipConvertFromType( neuron
                                       , sipFindType("Neuron")
                                       , NULL
                                       );
        }
    %End

    SIP_PYOBJECT
    __str__() const;
    %MethodCode
        sipRes = PyString_FromString(("Network : " + sipCpp -> id).c_str());
    %End
};


class Neuron : public Visualizable
{

%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/ElectricalCompartment.hpp"
#include "core/Neuron.hpp"
%End

public:

    const char * network
    {
        %GetCode
            sipPy = sipConvertFromType( sipCpp -> network
                                      , sipType_Network
                                      , NULL
                                      );
        %End
        %SetCode
            sipErr = 1;
            PyErr_SetString( PyExc_AttributeError
                           , "Use Network.attach to change the network this neuron belongs to"
                           );
        %End
    };

    Neuron(const char * id);
    ~Neuron();

    SIP_SSIZE_T
    size() const /__len__/;

    SIP_PYTUPLE
    electrical_compartments() const;
    %MethodCode
        sipRes = PyTuple_New(sipCpp -> size());
        SIP_SSIZE_T index = 0;
        for (const auto &electrical_compartment_pair : sipCpp -> electrical_compartment_map)
        {
            PyTuple_SET_ITEM( sipRes
                            , index
                            , sipConvertFromType( electrical_compartment_pair.second
                                                , sipFindType("ElectricalCompartment")
                                                , NULL
                                                )
                            );
            index += 1;
        }
    %End

    int
    __contains__(ElectricalCompartment * electrical_compartment) const;
    %MethodCode
        return static_cast<int>(sipCpp -> get(a0 -> id) != nullptr);
    %End

    SIP_PYOBJECT
    __getitem__(const char * electrical_compartment_id) const;
    %MethodCode
        ElectricalCompartment * electrical_compartment = sipCpp -> get(std::string(a0));
        if(electrical_compartment == nullptr)
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_KeyError
                           , a0
                           );
        }
        else
        {
            sipRes = sipConvertFromType( electrical_compartment
                                       , sipFindType("ElectricalCompartment")
                                       , NULL
                                       );
        }
    %End

    SIP_PYOBJECT
    __str__() const;
    %MethodCode
        sipRes = PyString_FromString(("Neuron : " + sipCpp -> id).c_str());
    %End

    void
    attach(ElectricalCompartment * electrical_compartment /Transfer/);
    %MethodCode
        if(!sipCpp -> attach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("ElectricalCompartment : " + a0 -> id + " is already attached to Neuron : " + sipCpp -> id).c_str()
                           );
        }
    %End

    void
    detach(ElectricalCompartment * electrical_compartment);
    %MethodCode
        if(!sipCpp -> detach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("ElectricalCompartment : " + a0 -> id + " is not attached to Neuron : " + sipCpp -> id).c_str()
                           );
        }
    %End
};




class ElectricalCompartment : public Visualizable
{
%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/ElectricalCompartment.hpp"
#include "core/ChemicalCompartment.hpp"
%End

public:

    SIP_PYOBJECT neuron
    {
        %GetCode
            sipPy = sipConvertFromType( sipCpp -> neuron
                                      , sipFindType("Neuron")
                                      , NULL
                                      );
        %End
        %SetCode
            sipErr = 1;
            PyErr_SetString( PyExc_AttributeError
                           , "Use Neuron.attach to change the neuron this electrical compartment belongs to"
                           );
        %End
    };

    SIP_PYOBJECT parent
    {
        %GetCode
            sipPy = sipConvertFromType( sipCpp -> parent
                                      , sipFindType("ElectricalCompartment")
                                      , NULL
                                      );
        %End
        %SetCode
            if(sipCanConvertToType(sipPy, sipFindType("ElectricalCompartment"), SIP_NO_CONVERTORS))
            {
                int state = 0;
                int iserr = 0;
                ElectricalCompartment * electrical_compartment =
                    static_cast<ElectricalCompartment *>( sipConvertToType( sipPy
                                                                          , sipFindType("ElectricalCompartment")
                                                                          , NULL
                                                                          , SIP_NO_CONVERTORS
                                                                          , NULL
                                                                          , &iserr
                                                                          )
                                                        );
                sipCpp -> set_parent(electrical_compartment);
            }
            else
            {
                sipErr = 1;
                PyErr_SetString( PyExc_TypeError
                               , "Expected a value of type ElectricalCompartment"
                               );
            }
        %End
    };

    SIP_PYOBJECT children();
    %MethodCode
        sipRes = PyTuple_New(sipCpp -> children.size());
        SIP_SSIZE_T index = 0;
        for (const auto &electrical_compartment_pair : sipCpp -> children)
        {
            PyTuple_SET_ITEM( sipRes
                            , index
                            , sipConvertFromType( electrical_compartment_pair.second
                                                , sipFindType("ElectricalCompartment")
                                                , NULL
                                                )
                            );
            index += 1;
        }
    %End

    ElectricalCompartment(const char * id);

    ~ElectricalCompartment();

    SIP_SSIZE_T
    size() const /__len__ /;

    void
    attach(ChemicalCompartment * chemical_compartment /Transfer/);
    %MethodCode
        if(!sipCpp -> attach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("ChemicalCompartment : " + a0 -> id + " is already attached to ElectricalCompartment : " + sipCpp -> id).c_str()
                           );
        }
    %End

    void
    detach(ChemicalCompartment * chemical_compartment);
    %MethodCode
        if(!sipCpp -> detach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("ChemicalCompartment : " + a0 -> id + " is not attached to ElectricalCompartment : " + sipCpp -> id).c_str()
                           );
        }
    %End

    void
    subdivide(PyObject * ids);

    SIP_PYTUPLE
    chemical_compartments() const;
    %MethodCode
        sipRes = PyTuple_New(sipCpp -> size());
        SIP_SSIZE_T index = 0;
        for (const auto &chemical_compartment_pair : sipCpp -> chemical_compartment_map)
        {
            PyTuple_SET_ITEM( sipRes
                            , index
                            , sipConvertFromType( chemical_compartment_pair.second
                                                , sipType_ChemicalCompartment
                                                , NULL
                                                )
                            );
            index += 1;
        }
    %End

    int
    __contains__(ChemicalCompartment * chemical_compartment) const;
    %MethodCode
        return static_cast<int>(sipCpp -> get(a0 -> id) != nullptr);
    %End

    SIP_PYOBJECT
    __getitem__(const char * chemical_compartment_id) const;
    %MethodCode
        ChemicalCompartment * chemical_compartment = sipCpp -> get(std::string(a0));
        if(chemical_compartment == nullptr)
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_KeyError
                           , a0
                           );
        }
        else
        {
            sipRes = sipConvertFromType( chemical_compartment
                                       , sipType_ChemicalCompartment
                                       , NULL
                                       );
        }
    %End

    SIP_PYOBJECT
    __str__() const;
    %MethodCode
        sipRes = PyString_FromString(("ElectricalCompartment : " + sipCpp -> id).c_str());
    %End
};

class ChemicalCompartment : public Visualizable
{
%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/ChemicalCompartment.hpp"
%End

public:

    SIP_PYOBJECT electrical_compartment
    {
        %GetCode
            sipPy = sipConvertFromType( sipCpp -> electrical_compartment
                                      , sipFindType("ElectricalCompartment")
                                      , NULL
                                      );
        %End
        %SetCode
            sipErr = 1;
            PyErr_SetString( PyExc_AttributeError
                           , "Use ElectricalCompartment.attach to change the electrical compartment this chemical compartment belongs to"
                           );
        %End
    };

    ChemicalCompartment(const char * id);

    ~ChemicalCompartment();

    SIP_PYOBJECT
    __str__() const;
    %MethodCode
        sipRes = PyString_FromString(("ChemicalCompartment : " + sipCpp -> id).c_str());
    %End
};

class Visualizables
{
%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/conversions.hpp"
#include "core/Visualizable.hpp"
#include "core/Visualizables.hpp"
%End

public:

    Visualizables(const char * id);

    const char *
    get_id();

    uint
    add_visualizable(Visualizable * visualizable);

    uint
    remove_visualizable(PyObject * visualizable);

    void
    set_color(PyObject * color_seq);

    void
    set_color_mapping( PyObject * value_range
                     , PyObject * color_range
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
    set_background_color(PyObject * color_seq);
    %MethodCode
        if(PySequence_Check(a0))
        {
            if(PySequence_Length(a0) == 4)
            {
                Vec4f color( static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 0)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 1)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 2)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 3)))
                           );
                sipCpp -> set_background_color(color);
            }
            else
            {
                sipError = sipErrorFail;
                PyErr_SetString( PyExc_IndexError
                               , "Expected a sequence of size 4,(red, green, blue, alpha)"
                               );
            }
        }
        else
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_TypeError
                           , "Expected a sequence such as list, tuple or numpy array"
                           );
        }
    %End

    SIP_PYOBJECT
    create_color_bar( const char * id
                    , const char * title
                    , float min_scalar
                    , float max_scalar
                    , PyObject * colors
                    , int num_colors
                    , int num_labels
                    , float angle
                    , float aspect_ratio
                    , float width
                    , PyObject * position
                    , PyObject * label_color
                    , const char * label_font_file
                    , PyObject * label_font_resolution
                    , float label_character_size
                    );
    %MethodCode
        bool result = sipCpp -> create_color_bar( a0
                                                , a1
                                                , a2
                                                , a3
                                                , pysequence_to_vec4f_vector(a4)
                                                , a5
                                                , a6
                                                , a7
                                                , a8
                                                , a9
                                                , pysequence_to_vec3f(a10)
                                                , pysequence_to_vec4f(a11)
                                                , a12
                                                , pysequence_to_int_pair(a13)
                                                , a14
                                                );
        sipRes = PyBool_FromLong(result);
    %End

    bool
    hide_color_bar( const char * id
                  , unsigned int view_index = 0
                  );

    bool
    show_color_bar( const char * id
                  , unsigned int view_index = 0
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

