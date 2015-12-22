%Module _moogli

%Import QtGui/QtGuimod.sip
%Import QtOpenGL/QtOpenGLmod.sip

%ModuleHeaderCode
#include <osg/Geometry>
#include <osgUtil/LineSegmentIntersector>
#include <iostream>
#include <Python.h>
PyObject *
intersections_to_tuple_of_pairs(const osgUtil::LineSegmentIntersector::Intersections & intersections);
%End

%ModuleCode
#include <osg/Geometry>
#include <osgUtil/LineSegmentIntersector>
#include <iostream>
#include <Python.h>

PyObject *
intersections_to_tuple_of_pairs(const osgUtil::LineSegmentIntersector::Intersections & intersections)
{
    PyObject * tuple = PyTuple_New(intersections.size());
    uint index = 0;
    for(const osgUtil::LineSegmentIntersector::Intersection & hit : intersections)
    {
         std::cerr << hit.drawable -> className() << std::endl;
         osg::Geometry * geometry = hit.drawable -> asGeometry();
         if(geometry)
         {
             PyObject * pair = PyTuple_New(2);
             PyTuple_SET_ITEM( pair
                             , 0
                             , PyString_FromString(geometry -> getName().c_str())
                             );
             
             osg::Vec3f * point = new osg::Vec3f( hit.getWorldIntersectPoint().x()
                                                , hit.getWorldIntersectPoint().y()
                                                , hit.getWorldIntersectPoint().z()
                                                );
             PyTuple_SET_ITEM( pair
                             , 1
                             , sipConvertFromType( point 
                                                 , sipFindType("Vec3f")
                                                 , Py_None
                                                 )
                             );

             PyTuple_SET_ITEM(tuple, index, pair);
             index += 1;
         }
    }
    return tuple;
}
%End

class Vec2i
{
%TypeHeaderCode
#include <osg/Vec2i>
using osg::Vec2i;
%End
public:
 	  Vec2i();
 	  Vec2i(int x, int y);
    bool operator== (const Vec2i &v) const;
    bool operator!= (const Vec2i &v) const;
    bool operator< (const Vec2i &v) const;
    void set(int x, int y);
    void set(const Vec2i &rhs);
    PyObject * get_x();
    %MethodCode
        sipRes = PyInt_FromLong(sipCpp -> x());
    %End
    void set_x(int value);
    %MethodCode
        sipCpp -> x() = a0;
    %End
    PyObject * get_y();
    %MethodCode
        sipRes = PyInt_FromLong(sipCpp -> y());
    %End
    void set_y(int value);
    %MethodCode
        sipCpp -> y() = a0;
    %End
    Vec2i	operator* (int rhs) const;
    Vec2i	operator/ (int rhs) const;
    Vec2i	operator+ (int rhs) const;
    Vec2i	operator- (int rhs) const;
    Vec2i	operator+ (const Vec2i &rhs) const;
    Vec2i	operator- (const Vec2i &rhs) const;
    Vec2i	operator* (const Vec2i &rhs) const;
    PyObject *
    __str__();
    %MethodCode
        sipRes = PyString_FromString( ( std::string("Vec2i(")
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ")"
                                      ).c_str()
                                    );
    %End

    PyObject *
    __repr__();
    %MethodCode
        sipRes = PyString_FromString( ( std::string("Vec2i(")
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ")"
                                      ).c_str()
                                    );
    %End

};

class Vec2f
{
%TypeHeaderCode
#include <osg/Vec2f>
using osg::Vec2f;
%End
public:
 	  Vec2f();
 	  Vec2f(float x, float y);
    bool operator== (const Vec2f &v) const;
    bool operator!= (const Vec2f &v) const;
    bool operator< (const Vec2f &v) const;
    void set(float x, float y);
    PyObject * get_x();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> x());
    %End
    void set_x(float value);
    %MethodCode
        sipCpp -> x() = a0;
    %End
    PyObject * get_y();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> y());
    %End
    void set_y(float value);
    %MethodCode
        sipCpp -> y() = a0;
    %End
    bool valid() const;
    bool isNaN() const;
    float operator* (const Vec2f &rhs) const;
    const Vec2f	operator* (float rhs) const;
    const Vec2f	operator/ (float rhs) const;
    const Vec2f	operator+ (const Vec2f &rhs) const;
    const Vec2f	operator- (const Vec2f &rhs) const;
    const Vec2f	operator- () const;
    float length() const;
    float length2() const;
    float normalize();

    PyObject *
    __str__();
    %MethodCode
        sipRes = PyString_FromString( ( "Vec2f("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ")"
                                      ).c_str()
                                    );
    %End

    PyObject *
    __repr__();
    %MethodCode
        sipRes = PyString_FromString( ( "Vec2f("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ")"
                                      ).c_str()
                                    );
    %End


};


class Vec3i
{
%TypeHeaderCode
#include <osg/Vec3i>
using osg::Vec3i;
%End
public:
    Vec3i();
 	  Vec3i(int r, int g, int b);
    bool operator== (const Vec3i &v) const;
    bool operator!= (const Vec3i &v) const;
    bool operator< (const Vec3i &v) const;
    void set(int r, int g, int b);
    void set(const Vec3i &rhs);
    PyObject * get_x();
    %MethodCode
        sipRes = PyInt_FromLong(sipCpp -> x());
    %End
    void set_x(int value);
    %MethodCode
        sipCpp -> x() = a0;
    %End
    PyObject * get_y();
    %MethodCode
        sipRes = PyInt_FromLong(sipCpp -> y());
    %End
    void set_y(int value);
    %MethodCode
        sipCpp -> y() = a0;
    %End
    PyObject * get_z();
    %MethodCode
        sipRes = PyInt_FromLong(sipCpp -> z());
    %End
    void set_z(int value);
    %MethodCode
        sipCpp -> z() = a0;
    %End
    Vec3i	operator* (int rhs) const;
    Vec3i	operator/ (int rhs) const;
    Vec3i	operator+ (int rhs) const;
    Vec3i	operator- (int rhs) const;
    Vec3i	operator+ (const Vec3i &rhs) const;
    Vec3i	operator- (const Vec3i &rhs) const;
    Vec3i	operator* (const Vec3i &rhs) const;

    PyObject *
    __str__();
    %MethodCode
        sipRes = PyString_FromString( ( "Vec3i("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ", "
                                      + std::to_string(sipCpp -> z())
                                      + ")"
                                      ).c_str()
                                    );
    %End

    PyObject *
    __repr__();
    %MethodCode
        sipRes = PyString_FromString( ( "Vec3i("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ", "
                                      + std::to_string(sipCpp -> z())
                                      + ")"
                                      ).c_str()
                                    );
    %End



};

class Vec3f
{
%TypeHeaderCode
#include <osg/Vec3f>
using  osg::Vec3f;
%End
public:
    Vec3f();
    Vec3f(const Vec2f &v2, float z);
 	  Vec3f(float x, float y, float z);
    bool operator== (const Vec3f &v) const;
    bool operator!= (const Vec3f &v) const;
    bool operator< (const Vec3f &v) const;
    void set(float x, float y, float z);
    void set(const Vec3f &rhs);
    PyObject * get_x();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> x());
    %End
    void set_x(float value);
    %MethodCode
        sipCpp -> x() = a0;
    %End
    PyObject * get_y();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> y());
    %End
    void set_y(float value);
    %MethodCode
        sipCpp -> y() = a0;
    %End
    PyObject * get_z();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> z());
    %End
    void set_z(float value);
    %MethodCode
        sipCpp -> z() = a0;
    %End
    bool valid() const;
    bool isNaN () const;
    float operator* (const Vec3f &rhs) const;
    const Vec3f operator^ (const Vec3f &rhs) const;
    const Vec3f	operator* (float rhs) const;
    const Vec3f operator/ (float rhs) const;
    const Vec3f operator+ (const Vec3f &rhs) const;
    const Vec3f operator- (const Vec3f &rhs) const;
    const Vec3f operator- () const;
    float length() const;
    float length2() const;
    float normalize();

    PyObject *
    __str__();
    %MethodCode
        sipRes = PyString_FromString( ( "Vec3f("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ", "
                                      + std::to_string(sipCpp -> z())
                                      + ")"
                                      ).c_str()
                                    );
    %End

    PyObject *
    __repr__();
    %MethodCode
        sipRes = PyString_FromString( ( "Vec3f("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ", "
                                      + std::to_string(sipCpp -> z())
                                      + ")"
                                      ).c_str()
                                    );
    %End



};

class Vec4i
{
%TypeHeaderCode
#include <osg/Vec4i>
using osg::Vec4i;
%End
public:
    Vec4i();
 	  Vec4i(int x, int y, int z, int w);
    bool operator== (const Vec4i &v) const;
    bool operator!= (const Vec4i &v) const;
    bool operator< (const Vec4i &v) const;
    void set(int x, int y, int z, int w);
    PyObject * get_x();
    %MethodCode
        sipRes = PyInt_FromLong(sipCpp -> x());
    %End
    void set_x(int value);
    %MethodCode
        sipCpp -> x() = a0;
    %End
    PyObject * get_y();
    %MethodCode
        sipRes = PyInt_FromLong(sipCpp -> y());
    %End
    void set_y(int value);
    %MethodCode
        sipCpp -> y() = a0;
    %End
    PyObject * get_z();
    %MethodCode
        sipRes = PyInt_FromLong(sipCpp -> z());
    %End
    void set_z(int value);
    %MethodCode
        sipCpp -> z() = a0;
    %End
    PyObject * get_w();
    %MethodCode
        sipRes = PyInt_FromLong(sipCpp -> w());
    %End
    void set_w(int value);
    %MethodCode
        sipCpp -> w() = a0;
    %End
    Vec4i	operator* (int rhs) const;
    Vec4i	operator/ (int rhs) const;
    Vec4i	operator+ (int rhs) const;
    Vec4i	operator- (int rhs) const;
    Vec4i	operator+ (const Vec4i &rhs) const;
    Vec4i operator- (const Vec4i &rhs) const;
    Vec4i	operator* (const Vec4i &rhs) const;

    PyObject *
    __str__();
    %MethodCode
        sipRes = PyString_FromString( ( "Vec4i("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ", "
                                      + std::to_string(sipCpp -> z())
                                      + ", "
                                      + std::to_string(sipCpp -> w())
                                      + ")"
                                      ).c_str()
                                    );
    %End

    PyObject *
    __repr__();
    %MethodCode
        sipRes = PyString_FromString( ( "Vec4i("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ", "
                                      + std::to_string(sipCpp -> z())
                                      + ", "
                                      + std::to_string(sipCpp -> w())
                                      + ")"
                                      ).c_str()
                                    );
    %End

};


class Vec4f
{
%TypeHeaderCode
#include <osg/Vec4f>
using osg::Vec4f;
%End
public:
 	  Vec4f();
 	  Vec4f(float x, float y, float z, float w);
 	  Vec4f(const Vec3f &v3, float w);
    bool operator== (const Vec4f &v) const;
    bool operator!= (const Vec4f &v) const;
    bool operator< (const Vec4f &v) const;
    void set(float x, float y, float z, float w);
    PyObject * get_x();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> x());
    %End
    void set_x(float value);
    %MethodCode
        sipCpp -> x() = a0;
    %End
    PyObject * get_y();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> y());
    %End
    void set_y(float value);
    %MethodCode
        sipCpp -> y() = a0;
    %End
    PyObject * get_z();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> z());
    %End
    void set_z(float value);
    %MethodCode
        sipCpp -> z() = a0;
    %End
    PyObject * get_w();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> w());
    %End
    void set_w(float value);
    %MethodCode
        sipCpp -> w() = a0;
    %End
    bool valid() const;
    bool isNaN() const;
    float operator* (const Vec4f &rhs) const;
    Vec4f operator* (float rhs) const;
    Vec4f operator/ (float rhs) const;
    Vec4f operator+ (const Vec4f &rhs) const;
    Vec4f operator- (const Vec4f &rhs) const;
    const Vec4f operator- () const;
    float length() const;
    float length2() const;
    float normalize();

    PyObject *
    __str__();
    %MethodCode
        sipRes = PyString_FromString( ( "Vec4f("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ", "
                                      + std::to_string(sipCpp -> z())
                                      + ", "
                                      + std::to_string(sipCpp -> w())
                                      + ")"
                                      ).c_str()
                                    );
    %End

    PyObject *
    __repr__();
    %MethodCode
        sipRes = PyString_FromString( ( "Vec4f("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ", "
                                      + std::to_string(sipCpp -> z())
                                      + ", "
                                      + std::to_string(sipCpp -> w())
                                      + ")"
                                      ).c_str()
                                    );
    %End


};

class Quat
{
%TypeHeaderCode
#include <osg/Quat>
using namespace osg;
%End
public:
    Quat();
 	  Quat(float x, float y, float z, float w);
 	  Quat(const Vec4f &v);
 	  Quat(float angle, const Vec3f &axis);
 	  Quat(float angle1, const Vec3f &axis1, float angle2, const Vec3f &axis2, float angle3, const Vec3f &axis3);
    bool operator== (const Quat &v) const;
    bool operator!= (const Quat &v) const;
    bool operator< (const Quat &v) const;
    Vec4f asVec4() const;
    Vec3f asVec3() const;
    void set(float x, float y, float z, float w);
    void set(const Vec4f &v);
    PyObject * get_x();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> x());
    %End
    void set_x(float value);
    %MethodCode
        sipCpp -> x() = a0;
    %End
    PyObject * get_y();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> y());
    %End
    void set_y(float value);
    %MethodCode
        sipCpp -> y() = a0;
    %End
    PyObject * get_z();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> z());
    %End
    void set_z(float value);
    %MethodCode
        sipCpp -> z() = a0;
    %End
    PyObject * get_w();
    %MethodCode
        sipRes = PyFloat_FromDouble(sipCpp -> w());
    %End
    void set_w(float value);
    %MethodCode
        sipCpp -> w() = a0;
    %End
    bool zeroRotation() const;
    const Quat operator* (float rhs) const;
    const Quat operator* (const Quat &rhs) const;
    Quat operator/ (float rhs) const;
    const Quat operator/ (const Quat &denom) const;
    const Quat operator+ (const Quat &rhs) const;
    const Quat operator- (const Quat &rhs) const;
    const Quat operator- () const;
    float length() const;
    float length2() const;
    Quat conj() const;
    const Quat inverse() const;
    void makeRotate(float angle, float x, float y, float z);
    void makeRotate(float angle, const Vec3f &vec);
    void makeRotate(float angle1, const Vec3f &axis1, float angle2, const Vec3f &axis2, float angle3, const Vec3f &axis3);
    void makeRotate(const Vec3f &vec1, const Vec3f &vec2);
    Vec3f operator* (const Vec3f &v) const;

    PyObject *
    __str__();
    %MethodCode
        sipRes = PyString_FromString( ( "Quat("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ", "
                                      + std::to_string(sipCpp -> z())
                                      + ", "
                                      + std::to_string(sipCpp -> w())
                                      + ")"
                                      ).c_str()
                                    );
    %End

    PyObject *
    __repr__();
    %MethodCode
        sipRes = PyString_FromString( ( "Quat("
                                      + std::to_string(sipCpp -> x())
                                      + ", "
                                      + std::to_string(sipCpp -> y())
                                      + ", "
                                      + std::to_string(sipCpp -> z())
                                      + ", "
                                      + std::to_string(sipCpp -> w())
                                      + ")"
                                      ).c_str()
                                    );
    %End

};
// template<typename VT>
// class BoundingBoxImpl
// {
// %TypeHeaderCode
// #include <osg/BoundingBox>
// %End
//     BoundingBoxImpl(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);
//  	  BoundingBoxImpl(const Vec3f & min, const Vec3f & max);
//     void init();
//     bool valid() const;
//     void set(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);
//     void set(const Vec3f & min, const Vec3f & max);
//     float xMin() const;
//     float yMin() const;
//     float zMin() const;
//     float xMax() const;
//     float yMax() const;
//     float zMax() const;
//     const Vec3f & center() const;
//     float radius() const;
//     float radius2() const;
// };
// // const vec_type 	corner (unsigned int pos) const
// //  	Returns a specific corner of the bounding box.
// // void 	expandBy (const vec_type &v)
// //  	Expands the bounding box to include the given coordinate.
// // void 	expandBy (value_type x, value_type y, value_type z)
// //  	Expands the bounding box to include the given coordinate.
// // void 	expandBy (const BoundingBoxImpl &bb)
// //  	Expands this bounding box to include the given bounding box.
// // void 	expandBy (const BoundingSphereImpl< VT > &sh)
// //  	Expands this bounding box to include the given sphere.
// // BoundingBoxImpl 	intersect (const BoundingBoxImpl &bb) const
// //  	Returns the intersection of this bounding box and the specified bounding box.
// // bool 	intersects (const BoundingBoxImpl &bb) const
// //  	Return true if this bounding box intersects the specified bounding box.
// // bool 	contains (const vec_type &v) const
// //  	Returns true if this bounding box contains the specified coordinate. 


class Shape /Abstract/ 
{
%TypeHeaderCode
#include "shapes/Shape.hpp"
%End

public:
    Shape(const char * id);

    PyObject *
    id() const;
    %MethodCode
        sipRes = PyString_FromString(sipCpp -> id().c_str());
    %End

    // Mesh *
    // get_parent() const;

    // void
    // set_parent(Mesh * parent /Transfer/);

    // SIP_PYTUPLE
    // subdivide(unsigned int n) const;
    // %MethodCode
    //     sipRes = PyTuple_New(n);
    //     SIP_SSIZE_T index = 0;
    //     for(const Mesh * mesh : sipCpp -> subdivide(a0))
    //     {
    //         PyTuple_SET_ITEM( sipRes
    //                         , index
    //                         , sipConvertFromType( mesh
    //                                             , sipFindType("Mesh")
    //                                             , NULL
    //                                             )
    //                         );
    //         index += 1;
    //     }
    // %End
    void
    hide();

    void
    show();

    bool
    visible();

    bool
    export_geometry(const char * filename, bool with_effects = false) const;

    void
    enable_effects();

    void
    disable_effects();

    void
    toggle_effects();

    void
    enable_outline_effect();

    void
    disable_outline_effect();

    void
    toggle_outline_effect();

    bool
    outline_effect_enabled();

    void
    set_outline_effect_line_width(float width);

    float
    get_outline_effect_line_width();

    void
    set_outline_effect_line_color(const Vec4f & color);

    const Vec4f &
    get_outline_effect_line_color();

    void
    enable_cartoon_effect();

    void
    disable_cartoon_effect();

    void
    toggle_cartoon_effect();

    bool
    cartoon_effect_enabled();

    void
    set_cartoon_effect_outline_width(float width);

    float
    get_cartoon_effect_outline_width();

    void
    set_cartoon_effect_outline_color(const Vec4f & color);

    const Vec4f &
    get_cartoon_effect_outline_color();

    void
    enable_wireframe_effect();

    void
    disable_wireframe_effect();

    void
    toggle_wireframe_effect();

    bool
    wireframe_effect_enabled();

    void
    set_wireframe_effect_line_width(float width);

    float
    get_wireframe_effect_line_width();

    void
    set_wireframe_effect_line_color(const Vec4f & color);

    const Vec4f &
    get_wireframe_effect_line_color();

    void
    enable_specular_highlights_effect();

    void
    disable_specular_highlights_effect();

    void
    toggle_specular_highlights_effect();

    bool
    specular_highlights_effect_enabled();

    void
    set_specular_highlights_effect_exponent(float exponent);

    float
    get_specular_highlights_effect_exponent();

    void
    get_specular_highlights_effect_color(const Vec4f & color);

    const Vec4f &
    get_specular_highlights_effect_color();
};


class Frustum : public Shape 
{
%TypeHeaderCode
#include "shapes/Frustum.hpp"
%End

public:

    Frustum(const char * id);

    Frustum( const char * id
           , const Vec3f & center
           , const Vec3f & axis
           , float length
           , float base_radius
           , float apex_radius
           , unsigned int vertices
           , const Vec4f & base_color
           , const Vec4f & apex_color
           );

    Frustum( const char * id
           , const Vec3f & base
           , const Vec3f & apex
           , float base_radius
           , float apex_radius
           , unsigned int vertices
           , const Vec4f & base_color
           , const Vec4f & apex_color
           );
    void
    set_geometry( const Vec3f & center
                , const Vec3f & axis
                , float length
                , float base_radius
                , float apex_radius
                , unsigned int vertices
                );

    void
    set_geometry( const Vec3f & base
                , const Vec3f & apex
                , float base_radius
                , float apex_radius
                , unsigned int vertices
                );

    void
    set( const Vec3f & center
       , const Vec3f & axis
       , float length
       , float base_radius
       , float apex_radius
       , unsigned int vertices
       , const Vec4f & base_color
       , const Vec4f & apex_color
       );

    void
    set( const Vec3f & base
       , const Vec3f & apex
       , float base_radius
       , float apex_radius
       , unsigned int vertices
       , const Vec4f & base_color
       , const Vec4f & apex_color
       );

    void
    set_vertices(unsigned int vertices);

    unsigned int
    get_vertices();

    void
    set_axis(const Vec3f & axis);

    Vec3f
    get_axis() const;

    void
    set_length(const float & length);

    float
    get_length() const;

    void
    set_center(const Vec3f & center);

    Vec3f
    get_center() const;

    void
    set_apex(const Vec3f & apex);

    Vec3f
    get_apex() const;

    void
    set_base(const Vec3f & base);

    Vec3f
    get_base() const;

    void
    set_base_radius(const float base_radius);

    float
    get_base_radius() const;

    void
    set_apex_radius(const float apex_radius);

    float
    get_apex_radius() const;

    void
    set_radii(float base_radius, float apex_radius);

    void
    set_radius(float radius);

    void
    set_base_color(const Vec4f & base_color);

    void
    set_apex_color(const Vec4f & apex_color);

    const Vec4f &
    get_base_color() const;

    const Vec4f &
    get_apex_color() const;

    void
    set_color(const Vec4f & color);

    void
    set_colors(const Vec4f & base_color, const Vec4f & apex_color);

    void
    move_apex_by(float dl);

    void
    move_base_by(float dl);

    void
    move_apex_along(const Vec3f & direction, float dl);

    void
    move_base_along(const Vec3f & direction, float dl);

    void
    move_center_by(const Vec3f & displacement);

    void
    rotate_axis_along(const Vec3f & rotation_axis, float angle);

};

class Sphere : public Shape 
{
%TypeHeaderCode
#include "shapes/Sphere.hpp"
%End

public:

    Sphere(const char * id);

    Sphere( const char * id
          , const Vec3f & center
          , const float radius
          , unsigned int vertices
          , const Vec4f & color
          );

    void
    set_geometry( const Vec3f & center
                , float radius
                , unsigned int vertices
                );

    void
    set( const Vec3f & center
       , float radius
       , unsigned int vertices
       , const Vec4f & color
       );

    void
    set_vertices(unsigned int vertices);

    unsigned int
    get_vertices();

    void
    set_center(const Vec3f & center);

    Vec3f
    get_center() const;

    void
    set_radius(float radius);

    float
    get_radius() const;

    const Vec4f &
    get_color() const;

    void
    set_color(const Vec4f & color);

    void
    move_center_by(const Vec3f & displacement);
};


class EllipsoidalSurface : public Shape
{
%TypeHeaderCode
#include "shapes/EllipsoidalSurface.hpp"
%End

public:

    EllipsoidalSurface(const char * id);


    EllipsoidalSurface( const char * id
                      , const Vec3f & center
                      , float a
                      , float b
                      , float c
                      , const Vec3f & a_axis
                      , const Vec3f & b_axis
                      , float theta_min
                      , float theta_max
                      , float phi_min
                      , float phi_max
                      , unsigned int vertices
                      , const Vec4f & color
                      );

    void
    set_geometry( const Vec3f & center
                , float a
                , float b
                , float c
                , const Vec3f & a_axis
                , const Vec3f & b_axis
                , float theta_min
                , float theta_max
                , float phi_min
                , float phi_max
                , unsigned int vertices
                );

    void
    set( const Vec3f & center
       , float a
       , float b
       , float c
       , const Vec3f & a_axis
       , const Vec3f & b_axis
       , float theta_min
       , float theta_max
       , float phi_min
       , float phi_max
       , unsigned int vertices
       , const Vec4f & color
       );

    float
    get_phi_min() const;

    void
    set_phi_min(float phi_min);

    float
    get_phi_max() const;

    void
    set_phi_max(float phi_max);

    float
    get_theta_min() const;

    void
    set_theta_min(float theta_min);

    float
    get_theta_max() const;

    void
    set_theta_max(float theta_max);

    float
    get_a() const;

    void
    set_a(float a);

    float
    get_b() const;

    void
    set_b(float b);

    float
    get_c() const;

    void
    set_c(float c);

    void
    set_abc(float a, float b, float c);

    void
    set_theta(float theta_min, float theta_max);

    void
    set_phi(float phi_min, float phi_max);

    void
    set_center(const Vec3f & center);

    const Vec3f &
    get_center() const;

    void
    set_color(const Vec4f & color);

    const Vec4f &
    get_color() const;

    void
    set_a_and_b_axes(const Vec3f & a_axis, const Vec3f & b_axis);

    void
    set_b_and_c_axes(const Vec3f & b_axis, const Vec3f & c_axis);

    void
    set_c_and_a_axes(const Vec3f & c_axis, const Vec3f & a_axis);

    const Vec3f &
    get_a_axis() const;

    const Vec3f &
    get_b_axis() const;

    const Vec3f &
    get_c_axis() const;

    void
    set_vertices(unsigned int vertices);

    unsigned int
    get_vertices() const;
//     PyObject *
//     __str__();
//     %MethodCode
//         sipRes = PyString_FromString( ( std::string("EllipsoidalSurface(")
//                                       + "id => " + sipCpp -> id()
//                                       + "a  => " + sipCpp -> get_a()
//                                       + "b  => " + sipCpp -> get_b()
//                                       + "c  => " + sipCpp -> get_c()
//        , float b
//        , float c
//        , const Vec3f & a_axis
//        , const Vec3f & b_axis
//        , float theta_min
//        , float theta_max
//        , float phi_min
//        , float phi_max

// + " "
//                                       std::to_string(sipCpp -> x())
//                                       + ", "
//                                       + std::to_string(sipCpp -> y())
//                                       + ")"
//                                       ).c_str()
//                                     );
//     %End

//     PyObject *
//     __repr__();
//     %MethodCode
//         sipRes = PyString_FromString( ( std::string("Vec2i(")
//                                       + std::to_string(sipCpp -> x())
//                                       + ", "
//                                       + std::to_string(sipCpp -> y())
//                                       + ")"
//                                       ).c_str()
//                                     );
//     %End

};

class CatmullRom : public Shape
{
%TypeHeaderCode
#include "shapes/CatmullRom.hpp"
%End

public:

    CatmullRom( const char * id
              , const Vec3f & distal
              , float radius
              );

    void
    set_color(const Vec4f & color);

    const Vec4f &
    get_color() const;

    void
    _interpolate_root_node_to_leaf_node( CatmullRom * child
                                       , const uint   axial_segments
                                       , const uint   radial_segments
                                       );

    void
    _interpolate_root_child_node_to_internal_node( CatmullRom * parent
                                                 , CatmullRom * child
                                                 , CatmullRom * grandchild
                                                 , const uint axial_segments
                                                 , const uint radial_segments
                                                 );


    void
    _interpolate_root_child_node_to_leaf_node( CatmullRom * parent
                                             , CatmullRom * child
                                             , const uint axial_segments
                                             , const uint radial_segments
                                             );

    void
    _interpolate_internal_node_to_internal_node( CatmullRom * grandparent
                                               , CatmullRom * parent
                                               , CatmullRom * child
                                               , CatmullRom * grandchild
                                               , const uint axial_segments
                                               , const uint radial_segments
                                               );

    void
    _interpolate_internal_node_to_leaf_node( CatmullRom * grandparent
                                           , CatmullRom * parent
                                           , CatmullRom * child
                                           , const uint axial_segments
                                           , const uint radial_segments
                                           );

    void
    _interpolate_root_node_to_internal_node( CatmullRom * child
                                           , CatmullRom * grandchild
                                           , const uint axial_segments
                                           , const uint radial_segments
                                           );
    void
    _interpolate_leaf_node( CatmullRom * parent
                          , const uint axial_segments
                          , const uint radial_segments
                          );

};

class ColorBar
{
%TypeHeaderCode
#include "widgets/ColorBar.hpp"
%End

public:
    ColorBar(const char * id);

    // ColorBar( const char * id
    //         , Vec2f & scalar_range
    //         , const std::vector<osg::Vec4f> & colors
    //         , int label_count
    //         , const char * title
    //         , const Vec2f & position
    //         , const Vec2f & size
    //         , float orientation
    //         , const char * text_font
    //         , const osg::Vec2i text_font_resolution
    //         , float text_character_size
    //         , const osg::Vec4f & text_color
    //         );

    void
    resize();

    void
    resize(int width, int height);

    void
    resize(int x, int y, int width, int height);

    void
    set_parent_viewport(const Vec4i & viewport);

    void
    set_scalar_range(const Vec2f & scalar_range);

    Vec2f
    get_scalar_range() const;

    void
    set_colors(PyObject * colors);
    %MethodCode
        unsigned int length = PySequence_Length(a0);
        unsigned int i;
        int iserr = 0;
        int state;

        std::vector<osg::Vec4f> colors(length);
        for(i = 0; i < length; ++i)
        {
            osg::Vec4f * color = static_cast<osg::Vec4f *>(sipConvertToType( PySequence_GetItem(a0, i)
                                                                           , sipFindType("Vec4f")
                                                                           , NULL
                                                                           , SIP_NO_CONVERTORS
                                                                           , NULL
                                                                           , &iserr
                                                                           )
                                                          );
            colors[i] = *color;
        }
        sipCpp -> set_colors(colors);
    %End

    void
    set_label_count(int label_count);

    int
    get_label_count() const;

    void
    set_title(const char * title);

    PyObject *
    get_title () const;
    %MethodCode
        sipRes = PyString_FromString(sipCpp -> get_title().c_str());
    %End

    void
    set_position(const Vec2f & position);

    const Vec2f &
    get_position() const;

    void
    set_size(const Vec2f & size);

    const Vec2f &
    get_size() const;

    void
    set_orientation(float orientation);

    float
    get_orientation() const;

    void
    set_text_font(const char * font);

    PyObject *
    get_text_font() const;
    %MethodCode
        sipRes = PyString_FromString(sipCpp -> get_text_font().c_str());
    %End

    void
    set_text_font_resolution(const Vec2i & font_resolution);

    Vec2i
    get_text_font_resolution() const;

    void
    set_text_character_size(float character_size);

    float
    get_text_character_size() const;

    void
    set_text_color(const Vec4f & color);

    const Vec4f &
    get_text_color() const;

    void
    set_label_formatting_precision(unsigned int precision);

    unsigned int
    get_label_formatting_precision() const;
};

class MeshView : public QObject
{
%TypeHeaderCode
#include "view/View.hpp"
%End

public:
    MeshView();

    bool
    event(QEvent * event);

    void
    set_position(const Vec2f & position);

    const Vec2f &
    get_position() const;

    void
    set_size(const Vec2f & size);

    const Vec2f &
    get_size() const;

    void
    set_background_color(const Vec4f & background_color);

    const Vec4f &
    get_background_color();

    void
    resize(int width, int height);

    void
    attach_color_bar(ColorBar * color_bar);

    void
    detach_color_bar(ColorBar * color_bar);

    void
    home();

    void
    forward(double distance_delta);

    void
    backward(double distance_delta);

    void
    left(double distance_delta);

    void
    right(double distance_delta);

    void
    up(double distance_delta);

    void
    down(double distance_delta);

    void
    zoom(double factor);

    void
    roll(double angle);

    void
    pitch(double angle);

    void
    yaw(double angle);

};

class GLWidget : public QGLWidget
{
%TypeHeaderCode
#include <osgQt/GraphicsWindowQt>
using namespace osgQt;
%End

public:
    GLWidget( QWidget* parent = NULL, const QGLWidget* shareWidget = NULL, Qt::WindowFlags f = 0, bool forwardKeyEvents = false );
    GLWidget( QGLContext* context, QWidget* parent = NULL, const QGLWidget* shareWidget = NULL, Qt::WindowFlags f = 0, bool forwardKeyEvents = false );
    GLWidget( const QGLFormat& format, QWidget* parent = NULL, const QGLWidget* shareWidget = NULL, Qt::WindowFlags f = 0, bool forwardKeyEvents = false );
    virtual ~GLWidget();

    bool getForwardKeyEvents() const; 
    virtual void setForwardKeyEvents( bool f );
    
    void setKeyboardModifiers( QInputEvent* event );

    virtual void keyPressEvent( QKeyEvent* event );
    virtual void keyReleaseEvent( QKeyEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseDoubleClickEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void wheelEvent( QWheelEvent* event );
    //virtual bool gestureEvent( QGestureEvent* event );

protected:

    virtual void resizeEvent( QResizeEvent* event );
    virtual void moveEvent( QMoveEvent* event );
    virtual void glDraw();
    virtual bool event( QEvent* event );
};

class Viewer : public GLWidget
{
%TypeHeaderCode
#include "view/Viewer.hpp"
using namespace osgQt;
%End

public:

    Viewer( const QGLFormat & format
          , QWidget * parent = nullptr
          , const QGLWidget * share_widget = nullptr
          , Qt::WindowFlags f = 0
          );

    void
    attach_shape(Shape * shape);

    void
    detach_shape(Shape * shape);

    void
    set_background_color(const Vec4f & background_color);

    const Vec4f &
    get_background_color();

 
protected:
    virtual void
    initializeGL();

    void
    paintGL();

public Q_SLOTS:
    void
    updateGL();

    PyObject *
    intersections(int x, int y);
    %MethodCode
        std::cerr << "Called int sip function" << std::endl;
        const auto result = sipCpp -> intersections(a0, a1);
        sipRes = intersections_to_tuple_of_pairs(result);
        // const auto result = sipCpp -> intersections((int)(a0), (int)(a1));
        // sipRes = PyTuple_New(result.size());
        // for(uint i = 0; i < result.size(); ++i)
        // {
        //     PyTuple_SET_ITEM(sipRes, i, PyString_FromString(result[i].c_str()));
        // }
    %End

    PyObject *
    intersections(const Vec3f & start, const Vec3f & end);
    %MethodCode
        const auto result = sipCpp -> intersections(*a0, *a1);
        sipRes = intersections_to_tuple_of_pairs(result);
    %End

signals:
    void
    selected(const char * id);
};


class CompositeView : public GLWidget
{
%TypeHeaderCode
#include "view/CompositeView.hpp"
using namespace osgQt;
%End

public:

    CompositeView( QWidget * parent = nullptr
                 , const QGLWidget * share_widget = nullptr
                 , Qt::WindowFlags f = 0
                 );

    void
    attach_shape(Shape * shape);

    void
    detach_shape(Shape * shape);

    const Vec4f &
    get_background_color();

    void
    set_background_color(const Vec4f & background_color);

    void
    attach_color_bar(ColorBar * color_bar);

    void
    detach_color_bar(ColorBar * color_bar);

    void
    home();

    void
    forward(double distance_delta);

    void
    backward(double distance_delta);

    void
    left(double distance_delta);

    void
    right(double distance_delta);

    void
    up(double distance_delta);

    void
    down(double distance_delta);

    void
    zoom(double factor);

    void
    roll(double angle);

    void
    pitch(double angle);

    void
    yaw(double angle);

    void
    setParent(QWidget * parent, Qt::WindowFlags f);

    void
    setParent(QWidget * parent);

protected:
    virtual void
    initializeGL();

signals:
    void
    selected(const char * id);
};


class CompositeViewer: public QWidget
{
%TypeHeaderCode
#include "view/CompositeViewer.hpp"
using namespace osgQt;
%End

public:
    CompositeViewer(QWidget * parent = 0, Qt::WindowFlags f = 0);
protected:
    virtual void
    paintEvent(QPaintEvent * event);
};