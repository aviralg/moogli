#ifndef __CATMULL_ROM_HPP__
#define __CATMULL_ROM_HPP__

#include "shapes/Shape.hpp"

class CatmullRom : public Shape
{
private:
    osg::Vec4f _color;

public:
    osg::Vec3f distal;
    float radius;

    CatmullRom( const string & id
              , const osg::Vec3f & distal
              , float radius
              );

    void
    set_color(const osg::Vec4f & color);

    const osg::Vec4f &
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
                                                 );


    void
    _interpolate_root_child_node_to_leaf_node( CatmullRom * parent
                                             , CatmullRom * child
                                             );

    void
    interpolate_internal_node_to_internal_node( CatmullRom * parent
                                              , CatmullRom * child
                                              , CatmullRom * grandchild
                                              );

    void
    _interpolate_internal_node_to_leaf_node( CatmullRom * parent
                                           , CatmullRom * child
                                           );

    void
    _interpolate_root_node_to_internal_node( CatmullRom * child
                                           , CatmullRom * grandchild
                                           );
    void
    _interpolate_leaf_node(CatmullRom * parent);

    void
    attach_geometry(osg::Geometry * geometry);

    void
    allocate();

    void
    construct_vertices();

    void
    construct_indices();

    void
    color();

protected:

    void
    interpolate( osg::Vec3Array * V0, size_t i0
               , osg::Vec3Array * V1, size_t i1
               , osg::Vec3Array * V2, size_t i2
               , osg::Vec3Array * V3, size_t i3
               , osg::Vec3Array * L,  size_t l
               , osg::Vec3Array * R,  size_t r
               , const uint       axial_segments
               , const uint       radial_segments
               );

    inline void
    spline( const osg::Vec3f &  P0
          , const osg::Vec3f &  P1
          , const osg::Vec3f &  P2
          , const osg::Vec3f &  P3
          , osg::Vec3Array   *  V0, size_t i0
          , osg::Vec3Array   *  V1, size_t i1
          , const uint          axial_segments
          , const uint          radial_segments
          , const uint          radial_segment_index
          );

    void
    point( const osg::Vec3f & P0
         , const osg::Vec3f & P1
         , const osg::Vec3f & P2
         , const osg::Vec3f & P3
         , const osg::Vec3f & P10
         , const osg::Vec3f & P21
         , const osg::Vec3f & P32
         , const float knot_factor_10
         , const float knot_factor_21
         , const float knot_factor_32
         , const float t
         , osg::Vec3f & C
         );

    void
    _fill_tangent_polygon( float rp // parent radius
                      , float rc // child radius
                      , float lc // length of child compartment
                      , osg::Vec3f & cp // center of parent 
                      , osg::Vec3f & dc // direction of child compartment
                      , osg::Vec3Array * T // array in which the ring is to be generated
                      , uint t // starting index for generation of ring
                      , uint axial_segments
                      , uint radial_segments
                      );


    void
    _fill_pre_tangent_polygon( float rp // parent radius
                          , float rc // child radius
                          , float lc // length of child compartment
                          , osg::Vec3f & cp // center of parent 
                          , osg::Vec3f & dc // direction of child compartment
                          , osg::Vec3Array * T // array in which the ring is to be generated
                          , uint t // starting index for generation of ring
                          , uint axial_segments
                          , uint radial_segments
                          );
};


#endif /* __CATMULL_ROM_HPP__ */
