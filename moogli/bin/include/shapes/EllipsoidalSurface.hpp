#ifndef __ELLIPSOIDAL_SURFACE_HPP__
#define __ELLIPSOIDAL_SURFACE_HPP__

#include "shapes/Shape.hpp"
#include "utility/libraries.hpp"

class EllipsoidalSurface : public Shape
{

public:

    EllipsoidalSurface(const std::string & id);


    EllipsoidalSurface( const std::string & id
                      , const osg::Vec3f & center
                      , float a
                      , float b
                      , float c
                      , const osg::Vec3f & a_axis
                      , const osg::Vec3f & b_axis
                      , float theta_min
                      , float theta_max
                      , float phi_min
                      , float phi_max
                      , unsigned int vertices
                      , const osg::Vec4f & color
                      );

    void
    set_geometry( const osg::Vec3f & center
                , float a
                , float b
                , float c
                , const osg::Vec3f & a_axis
                , const osg::Vec3f & b_axis
                , float theta_min
                , float theta_max
                , float phi_min
                , float phi_max
                , unsigned int vertices
                );

    void
    set( const osg::Vec3f & center
       , float a
       , float b
       , float c
       , const osg::Vec3f & a_axis
       , const osg::Vec3f & b_axis
       , float theta_min
       , float theta_max
       , float phi_min
       , float phi_max
       , unsigned int vertices
       , const osg::Vec4f & color
       );

    void
    allocate();

    void
    construct_vertices();

    void
    construct_indices();

    void
    color();

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
    set_center(const osg::Vec3f & center);

    const osg::Vec3f &
    get_center() const;

    void
    set_color(const osg::Vec4f & color);

    const osg::Vec4f &
    get_color() const;

    void
    set_a_and_b_axes(const osg::Vec3f & a_axis, const osg::Vec3f & b_axis);

    void
    set_b_and_c_axes(const osg::Vec3f & b_axis, const osg::Vec3f & c_axis);

    void
    set_c_and_a_axes(const osg::Vec3f & c_axis, const osg::Vec3f & a_axis);

    const osg::Vec3f &
    get_a_axis() const;

    const osg::Vec3f &
    get_b_axis() const;

    const osg::Vec3f &
    get_c_axis() const;

    void
    set_vertices(unsigned int vertices);

    unsigned int
    get_vertices() const;

private:
    float _a;
    float _b;
    float _c;
    osg::Vec3f _a_axis;
    osg::Vec3f _b_axis;
    osg::Vec3f _c_axis;
    osg::Vec3f _center;
    float _phi_min, _phi_max;
    float _theta_min, _theta_max;
    osg::Vec4f _color;
    unsigned int _vertices;
};

#endif /* __ELLIPSOIDAL_SURFACE_HPP__ */
