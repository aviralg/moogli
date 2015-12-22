#include "shapes/CatmullRom.hpp"

CatmullRom::CatmullRom( const string & id
                      , const osg::Vec3f & distal
                      , float radius
                      ) : Shape(id)
                        , distal(distal)
                        , radius(radius)
{

  //std::cerr << "Radius => " << radius << std::endl;
  //std::cerr << "Distal => " << distal.x() << ", " << distal.y() << ", " << distal.z() << std::endl;
  _geode -> removeDrawable(_geometry.get());
    //    _geode = nullptr;
}

void
CatmullRom::allocate()
{}

void
CatmullRom::construct_vertices()
{}

void
CatmullRom::construct_indices()
{}

void
CatmullRom::color()
{
  for(uint i = 0; i < _geode -> getNumDrawables(); ++i)
    {
      osg::Geometry * geometry = static_cast<osg::Geometry *>(_geode -> getDrawable(i));
      osg::Vec4Array * colors = static_cast<osg::Vec4Array *>(geometry -> getColorArray());
      (*colors)[0] = _color;
      colors -> dirty();
      _coloring_required = false;
  }
}

void
CatmullRom::set_color(const osg::Vec4f & color)
{
    _color = color;
    _coloring_required = true;
}

const osg::Vec4f &
CatmullRom::get_color() const
{
  return _color;
}

void
CatmullRom::attach_geometry(osg::Geometry * geometry)
{
  geometry -> setUpdateCallback(new ShapeUpdateCallback(this));
  geometry -> setUseDisplayList(false);
  geometry -> setUseVertexBufferObjects(true);
  _geode -> addDrawable(geometry);
}


inline void
CatmullRom::point( const osg::Vec3f & P0
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
                 )
{
    osg::Vec3f A1 = P0
                  + P10 * ( (t                                  )
                          / knot_factor_10
                          );
    osg::Vec3f A2 = P1
                  + P21 * ( (t - knot_factor_10                 )
                          / knot_factor_21
                          );
    osg::Vec3f A3 = P2
                  + P32 * ( (t - knot_factor_21 - knot_factor_10)
                          / knot_factor_32
                          );
    osg::Vec3f B1 = A1
                  + (A2 - A1) * ( t
                                / (knot_factor_21 + knot_factor_10)
                                );
    osg::Vec3f B2 = A2
                  + (A3 - A2) * ( (t - knot_factor_10)
                                / (knot_factor_32 + knot_factor_21)
                                );

    float a = (t - knot_factor_10) / (knot_factor_21);
    osg::Vec3f B21 = B2 - B1;
    C.set( B21.x() * a + B1.x()
         , B21.y() * a + B1.y()
         , B21.z() * a + B1.z()
         );
}


void
CatmullRom::interpolate( osg::Vec3Array * V0, size_t i0
                       , osg::Vec3Array * V1, size_t i1
                       , osg::Vec3Array * V2, size_t i2
                       , osg::Vec3Array * V3, size_t i3
                       , osg::Vec3Array * L,  size_t l
                       , osg::Vec3Array * R,  size_t r
                       , const uint       axial_segments
                       , const uint       radial_segments
                       )
{
  uint m;
  for(m = 0; m < radial_segments; ++m)
  {
    spline( (*V0)[i0 + m]
          , (*V1)[i1 + m]
          , (*V2)[i2 + m]
          , (*V3)[i3 + m]
          , L, l
          , R, r
          , axial_segments
          , radial_segments
          , m
          );
  }
}

inline void
CatmullRom::spline( const osg::Vec3f &  P0
                  , const osg::Vec3f &  P1
                  , const osg::Vec3f &  P2
                  , const osg::Vec3f &  P3
                  , osg::Vec3Array   *  V0, size_t i0
                  , osg::Vec3Array   *  V1, size_t i1
                  , const uint          axial_segments
                  , const uint          radial_segments
                  , const uint          radial_segment_index
                  )
{

/*
A compartment is generated from 2 Catmull Rom Splines, L (left) and R (right).
L interpolates between P1 and P2. Hence it requires P0, P1, P2, P3.
R interpolates between P2 and P3. Hence it requires P1, P2, P3, P4.
Based upon the number of axial segments, we can divide L and R into 2 parts.
LL represents the left half of L and LR represents the right half of L.
RL represents the left half of R and RR represents the right half of R.
For the compartment in question, C2, we need to find out LR and RL.
We have 2 cases -


Even number of segments (6)
---------------------------

|<----------C1--------->|<----------C2--------->|<---------C3---------->|
0   1   2   3   4   5   6                       |                       |
|                       0   1   2   3   4   5   6                       |
|                                               0   1   2   3   4   5   6
|           |<----------L---------->|<----------R---------->|           |
|           0   1   2   3   4   5   6                       |           |
|           |                       0   1   2   3   4   5   6           |
|           |           |<---LR---->|<----RL--->|           |           |
|           |           0   1   2   3   4   5   6           |           |
|           |           |  Segments to compute  |           |           |
=========================================================================

Odd number of segments (5)
---------------------------

|<--------C1------->|<--------C2------->|<-------C3-------->|
0   1   2   3   4   5                   |                   |
|                   0   1   2   3   4   5                   |
|                                       0   1   2   3   4   5
|           |<--------L-------->|<--------R-------->|       |
|           0   1   2   3   4   5                   |       |
|           |                   0   1   2   3   4   5       |
|           |       |<----LR--->|<-RL-->|           |       |
|           |       0   1   2   3   4   5           |       |
|           |       |Segments to compute|           |       |
=============================================================

L - 0 to axial_segment/2  => axial_segments/2 + 1 rings
R - axial_segments/2 to axial_segments => axial_segments - axial_segments/2 + 1 rings
tdelta = (t2 - t1) / (axial_segments - 1)
LR -> (axial_segments - 1) / 2 to axial_segments - 2
RL -> 1 to (axial_segments - 1) / 2
*/

    float tdelta;
    size_t axial_segment_index, m;

    const osg::Vec3f P10(P1 - P0);
    const osg::Vec3f P21(P2 - P1);
    const osg::Vec3f P32(P3 - P2);

    const float knot_factor_10 = pow(P10.length2(), 0.25);
    const float knot_factor_21 = pow(P21.length2(), 0.25);
    const float knot_factor_32 = pow(P32.length2(), 0.25);

/*******************************************************************************
*                           Code to generate LR
*******************************************************************************/
    tdelta = knot_factor_21 / axial_segments;
    for( m = 0, axial_segment_index = 1
       ; axial_segment_index <= axial_segments / 2
       ; ++axial_segment_index, ++m
       )
    {
        point( P0
             , P1
             , P2
             , P3
             , P10
             , P21
             , P32
             , knot_factor_10
             , knot_factor_21
             , knot_factor_32
             , knot_factor_10 + axial_segment_index * tdelta
             , (*V0)[i0 + m * radial_segments + radial_segment_index]
             );
    }

    // std::cerr << "First Loop => " << m << std::endl;
    // std::cerr << "First Size => " << V0 -> size() / radial_segments << std::endl;
    // std::cerr << "i0         => " << i0 << std::endl;

    for( m = 0, axial_segment_index = axial_segments / 2
       ; axial_segment_index < axial_segments
       ; ++axial_segment_index, ++m
       )
    {
        point( P0
             , P1
             , P2
             , P3
             , P10
             , P21
             , P32
             , knot_factor_10
             , knot_factor_21
             , knot_factor_32
             , knot_factor_10 + axial_segment_index * tdelta
             , (*V1)[i1 + m * radial_segments + radial_segment_index]
             );
    }

    // std::cerr << "Second Loop => " << m << std::endl;
    // std::cerr << "Second Size => " << V1 -> size() / radial_segments << std::endl;
    // std::cerr << "i1          => " << i1 << std::endl;
}

void
CatmullRom::_interpolate_root_node_to_leaf_node( CatmullRom * child
                                               , const uint   axial_segments
                                               , const uint   radial_segments
                                               )
{
  uint half = axial_segments/2;
  uint left_vertex_rows = half + 1;
  uint left_vertex_count = left_vertex_rows * radial_segments;
  uint right_vertex_rows = (axial_segments - half + 1);
  uint right_vertex_count = right_vertex_rows * radial_segments;
  osg::Vec3f dc = child -> distal - this -> distal;
  float lc = dc.normalize();
  float h = (lc + this -> radius) / 2.0f;

  osg::Vec3Array * V0 = new osg::Vec3Array(radial_segments);
  CatmullRom::_fill_pre_tangent_polygon( this -> radius // parent radius
                                       , child -> radius // child radius
                                       , lc // length of child compartment
                                       , this -> distal // center of parent 
                                       , dc // direction of child compartment
                                       , V0 // array in which the ring is to be generated
                                       , 0 // starting index for generation of ring
                                       , axial_segments
                                       , radial_segments
                                       );

  osg::Vec3Array * V1 = new osg::Vec3Array(left_vertex_count);
  CatmullRom::_fill_tangent_polygon( this -> radius // parent radius
                                   , child -> radius // child radius
                                   , lc // length of child compartment
                                   , this -> distal // center of parent
                                   , dc // direction of child compartment
                                   , V1 // array in which the ring is to be generated
                                   , 0 // starting index for generation of ring
                                   , axial_segments
                                   , radial_segments
                                   );

  osg::Vec3Array * V2 = new osg::Vec3Array(right_vertex_count);
  Shape::fill_regular_polygon( V2
                             , right_vertex_count - radial_segments
                             , radial_segments
                             , child -> radius
                             , this -> distal + dc * h
                             , dc
                             );

  osg::Vec3Array * V3 = new osg::Vec3Array(radial_segments);
  Shape::fill_regular_polygon( V3
                             , 0
                             , radial_segments
                             , child -> radius
                             , child -> distal
                             , dc
                             );

  CatmullRom::interpolate( V0, 0
                         , V1, 0
                         , V2, right_vertex_count - radial_segments
                         , V3, 0
                         , V1, radial_segments
                         , V2, 0
                         , axial_segments
                         , radial_segments
                         );

    osg::Geometry * GL = new osg::Geometry();
    GL -> setName(this -> id());
    GL -> setVertexArray(V1);
    osg::Vec3Array * LN = new osg::Vec3Array(V1 -> size());
    fill_surface_normals(V1, LN, axial_segments, radial_segments);
    GL -> setNormalArray(LN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CL = new osg::Vec4Array(1);
    (*CL)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GL -> setColorArray(CL , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * LI = new osg::DrawElementsUShort(GL_TRIANGLES , (left_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(LI, axial_segments, radial_segments);
    GL -> insertPrimitiveSet(0, LI);
    attach_geometry(GL);

    osg::Geometry * GR = new osg::Geometry();
    GR -> setName(child -> id());
    GR -> setVertexArray(V2);
    osg::Vec3Array * RN = new osg::Vec3Array(V2 -> size());
    Shape::fill_surface_normals(V2, RN, axial_segments, radial_segments);
    GR -> setNormalArray(RN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CR = new osg::Vec4Array(1);
    (*CR)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GR -> setColorArray(CR , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * RI = new osg::DrawElementsUShort(GL_TRIANGLES , (right_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(RI, axial_segments, radial_segments);
    GR -> insertPrimitiveSet(0, RI);
    child -> attach_geometry(GR);
}

void
CatmullRom::_fill_tangent_polygon( float rp // parent radius
                                 , float rc // child radius
                                 , float lc // length of child compartment
                                 , osg::Vec3f & cp // center of parent 
                              , osg::Vec3f & dc // direction of child compartment
                              , osg::Vec3Array * T // array in which the ring is to be generated
                              , uint t // starting index for generation of ring
                              , uint axial_segments
                              , uint radial_segments
                              )
{
    float h = (lc + rp) / 2.0f;
    float g = sqrt(rc * rc + h * h);
    float cos_alpha = h / g;
    float sin_alpha = rc / g;
    float cos_beta = rp / g;
    float sin_beta = sqrt(rc * rc + h * h - rp * rp) / g;
    float rt = rp * (sin_alpha * cos_beta + cos_alpha * sin_beta);
    float lt = rp * (cos_alpha * cos_beta - sin_alpha * sin_beta);

    // std::cerr << "RT => " << rt << std::endl;
    // std::cerr << "LT => " << lt << std::endl;
    osg::Vec3f center = cp + dc * lt;
    Shape::fill_regular_polygon(T, t, radial_segments, rt, center, dc);
}


void
CatmullRom::_fill_pre_tangent_polygon( float rp // parent radius
                                  , float rc // child radius
                                  , float lc // length of child compartment
                                  , osg::Vec3f & cp // center of parent 
                                  , osg::Vec3f & dc // direction of child compartment
                                  , osg::Vec3Array * T // array in which the ring is to be generated
                                  , uint t // starting index for generation of ring
                                  , uint axial_segments
                                  , uint radial_segments
                                  )
{
    Shape::fill_regular_polygon(T, t, radial_segments, rp, cp, dc);
}


void
CatmullRom::_interpolate_root_child_node_to_internal_node( CatmullRom * parent
                                                         , CatmullRom * child
                                                         , CatmullRom * grandchild
                                                         , const uint   axial_segments
                                                         , const uint   radial_segments
                                                         )
{
  uint half = axial_segments/2;
  uint left_vertex_rows = half + 1;
  uint left_vertex_count = left_vertex_rows * radial_segments;
  uint right_vertex_rows = (axial_segments - half + 1);
  uint right_vertex_count = right_vertex_rows * radial_segments;
  osg::Vec3f dc = this -> distal - parent -> distal;
  float lc = dc.normalize();
  float h = (lc + parent -> radius) / 2.0f;

  osg::Vec3Array * V0 = new osg::Vec3Array(radial_segments);
  CatmullRom::_fill_tangent_polygon( parent -> radius // parent radius
                                   , this -> radius // child radius
                                   , lc // length of child compartment
                                   , parent -> distal // center of parent
                                   , dc // direction of child compartment
                                   , V0 // array in which the ring is to be generated
                                   , 0 // starting index for generation of ring
                                   , axial_segments
                                   , radial_segments
                                   );


  osg::Vec3Array * V1 = new osg::Vec3Array(left_vertex_count);
  Shape::fill_regular_polygon( V1
                             , 0
                             , radial_segments
                             , this -> radius
                             , parent -> distal + dc * h
                             , dc
                             );

  osg::Vec3Array * V2 = new osg::Vec3Array(right_vertex_count);
  osg::Vec3f dgc = child -> distal - this -> distal;
  dgc.normalize();
  Shape::fill_regular_polygon( V2
                             , right_vertex_count - radial_segments
                             , radial_segments
                             , child -> radius
                             , (this -> distal + child -> distal) / 2.0f
                             , dgc
                             );

  osg::Vec3Array * V3 = new osg::Vec3Array(radial_segments);
  osg::Vec3f dggc = grandchild -> distal - child -> distal;
  dggc.normalize();
  Shape::fill_regular_polygon( V3
                               , 0
                               , radial_segments
                               , grandchild -> radius
                               , (child -> distal + grandchild -> distal) / 2.0f
                               , dggc
                               );


  CatmullRom::interpolate( V0, 0
                         , V1, 0
                         , V2, right_vertex_count - radial_segments
                         , V3, 0
                         , V1, radial_segments
                         , V2, 0
                         , axial_segments
                         , radial_segments
                         );

    osg::Geometry * GL = new osg::Geometry();
    GL -> setName(this -> id());
    GL -> setVertexArray(V1);
    osg::Vec3Array * LN = new osg::Vec3Array(V1 -> size());
    fill_surface_normals(V1, LN, axial_segments, radial_segments);
    GL -> setNormalArray(LN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CL = new osg::Vec4Array(1);
    (*CL)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GL -> setColorArray(CL , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * LI = new osg::DrawElementsUShort(GL_TRIANGLES , (left_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(LI, axial_segments, radial_segments);
    GL -> insertPrimitiveSet(0, LI);
    attach_geometry(GL);

    osg::Geometry * GR = new osg::Geometry();
    GR -> setName(child -> id());
    GR -> setVertexArray(V2);
    osg::Vec3Array * RN = new osg::Vec3Array(V2 -> size());
    Shape::fill_surface_normals(V2, RN, axial_segments, radial_segments);
    GR -> setNormalArray(RN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CR = new osg::Vec4Array(1);
    (*CR)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GR -> setColorArray(CR , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * RI = new osg::DrawElementsUShort(GL_TRIANGLES , (right_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(RI, axial_segments, radial_segments);
    GR -> insertPrimitiveSet(0, RI);
    child -> attach_geometry(GR);
}


void
CatmullRom::_interpolate_root_child_node_to_leaf_node( CatmullRom * parent
                                                     , CatmullRom * child
                                                     , const uint   axial_segments
                                                     , const uint   radial_segments
                                                     )
{
   uint half = axial_segments/2;
  uint left_vertex_rows = half + 1;
  uint left_vertex_count = left_vertex_rows * radial_segments;
  uint right_vertex_rows = (axial_segments - half + 1);
  uint right_vertex_count = right_vertex_rows * radial_segments;
  osg::Vec3f dc = this -> distal - parent -> distal;
  float lc = dc.normalize();
  float h = (lc + parent -> radius) / 2.0f;

  osg::Vec3Array * V0 = new osg::Vec3Array(radial_segments);
  CatmullRom::_fill_tangent_polygon( parent -> radius // parent radius
                                   , this -> radius // child radius
                                   , lc // length of child compartment
                                   , parent -> distal // center of parent
                                   , dc // direction of child compartment
                                   , V0 // array in which the ring is to be generated
                                   , 0 // starting index for generation of ring
                                   , axial_segments
                                   , radial_segments
                                   );


  osg::Vec3Array * V1 = new osg::Vec3Array(left_vertex_count);
  Shape::fill_regular_polygon( V1
                             , 0
                             , radial_segments
                             , this -> radius
                             , parent -> distal + dc * h
                             , dc
                             );

  osg::Vec3Array * V2 = new osg::Vec3Array(right_vertex_count);
  osg::Vec3f dgc = child -> distal - this -> distal;
  dgc.normalize();
  Shape::fill_regular_polygon( V2
                             , right_vertex_count - radial_segments
                             , radial_segments
                             , child -> radius
                             , (this -> distal + child -> distal) / 2.0f
                             , dgc
                             );

  osg::Vec3Array * V3 = new osg::Vec3Array(radial_segments);
  Shape::fill_regular_polygon( V3
                             , 0
                             , radial_segments
                             , child -> radius
                             , child -> distal
                             , dgc
                             );


  CatmullRom::interpolate( V0, 0
                         , V1, 0
                         , V2, right_vertex_count - radial_segments
                         , V3, 0
                         , V1, radial_segments
                         , V2, 0
                         , axial_segments
                         , radial_segments
                         );

    osg::Geometry * GL = new osg::Geometry();
    GL -> setName(this -> id());
    GL -> setVertexArray(V1);
    osg::Vec3Array * LN = new osg::Vec3Array(V1 -> size());
    fill_surface_normals(V1, LN, axial_segments, radial_segments);
    GL -> setNormalArray(LN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CL = new osg::Vec4Array(1);
    (*CL)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GL -> setColorArray(CL , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * LI = new osg::DrawElementsUShort(GL_TRIANGLES , (left_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(LI, axial_segments, radial_segments);
    GL -> insertPrimitiveSet(0, LI);
    attach_geometry(GL);

    osg::Geometry * GR = new osg::Geometry();
    GR -> setName(child -> id());
    GR -> setVertexArray(V2);
    osg::Vec3Array * RN = new osg::Vec3Array(V2 -> size());
    Shape::fill_surface_normals(V2, RN, axial_segments, radial_segments);
    GR -> setNormalArray(RN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CR = new osg::Vec4Array(1);
    (*CR)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GR -> setColorArray(CR , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * RI = new osg::DrawElementsUShort(GL_TRIANGLES , (right_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(RI, axial_segments, radial_segments);
    GR -> insertPrimitiveSet(0, RI);
    child -> attach_geometry(GR);
}

void
CatmullRom::_interpolate_internal_node_to_internal_node( CatmullRom * grandparent
                                                       , CatmullRom * parent
                                                       , CatmullRom * child
                                                       , CatmullRom * grandchild
                                                       , const uint   axial_segments
                                                       , const uint   radial_segments
                                                       )
{
  uint half = axial_segments / 2;
  uint left_vertex_rows = half + 1;
  uint left_vertex_count = left_vertex_rows * radial_segments;
  uint right_vertex_rows = (axial_segments - half + 1);
  uint right_vertex_count = right_vertex_rows * radial_segments;

  osg::Vec3f D, C, U, V, E;
  float l;
  osg::Quat Q;

  osg::Vec3Array * V0 = parent -> get_geode() -> getVertexArray();

  D = parent -> distal - grandparent -> distal;
  l = D.normalize();
  C = (grandparent -> distal + parent -> distal) / 2.0f;
  Shape::fill_regular_polygon( V0
                              , 0
                              , radial_segments
                              , parent -> radius
                              , C
                              , D
                              );

  U = (*V0)[0] - C;
  U.normalize();

  osg::Vec3Array * V1 = new osg::Vec3Array(left_vertex_count);
  E = D;
  D = this -> distal - parent -> distal;
  l = D.normalize();
  C = (this -> distal + parent -> distal) / 2.0f;
  Q.makeRotate(E, D);
  V = Q * U;
  V.normalize();
  Shape::fill_regular_polygon( V1
                             , 0
                             , radial_segments
                             , this -> radius
                             , C
                             , D
                             , V
                             );
  U = (*V1)[0] - C;
  U.normalize();

  osg::Vec3Array * V2 = new osg::Vec3Array(right_vertex_count);
  E = D;
  D = child -> distal - this -> distal;
  l = D.normalize();
  C = (this -> distal + child -> distal) / 2.0f;
  Q.makeRotate(E, D);
  V = Q * U;
  V.normalize();
  Shape::fill_regular_polygon( V2
                             , right_vertex_count - radial_segments
                             , radial_segments
                             , child -> radius
                             , C
                             , D
                             , V
                             );


  U = (*V1)[0] - C;
  U.normalize();

  osg::Vec3Array * V3 = new osg::Vec3Array(radial_segments);
  E = D;
  D = grandchild -> distal - child -> distal;
  l = D.normalize();
  C = (grandchild -> distal + child -> distal) / 2.0f;
  Q.makeRotate(E, D);
  V = Q * U;
  V.normalize();
  Shape::fill_regular_polygon( V3
                             , 0
                             , radial_segments
                             , child -> radius
                             , C
                             , D
                             , V
                             );


  CatmullRom::interpolate( V0, 0
                         , V1, 0
                         , V2, right_vertex_count - radial_segments
                         , V3, 0
                         , V1, radial_segments
                         , V2, 0
                         , axial_segments
                         , radial_segments
                         );

    osg::Geometry * GL = new osg::Geometry();
    GL -> setName(this -> id());
    GL -> setVertexArray(V1);
    osg::Vec3Array * LN = new osg::Vec3Array(V1 -> size());
    fill_surface_normals(V1, LN, axial_segments, radial_segments);
    GL -> setNormalArray(LN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CL = new osg::Vec4Array(1);
    (*CL)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GL -> setColorArray(CL , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * LI = new osg::DrawElementsUShort(GL_TRIANGLES , (left_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(LI, axial_segments, radial_segments);
    GL -> insertPrimitiveSet(0, LI);
    attach_geometry(GL);

    osg::Geometry * GR = new osg::Geometry();
    GR -> setName(child -> id());
    GR -> setVertexArray(V2);
    osg::Vec3Array * RN = new osg::Vec3Array(V2 -> size());
    Shape::fill_surface_normals(V2, RN, axial_segments, radial_segments);
    GR -> setNormalArray(RN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CR = new osg::Vec4Array(1);
    (*CR)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GR -> setColorArray(CR , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * RI = new osg::DrawElementsUShort(GL_TRIANGLES , (right_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(RI, axial_segments, radial_segments);
    GR -> insertPrimitiveSet(0, RI);
    child -> attach_geometry(GR);

}

void
CatmullRom::_interpolate_internal_node_to_leaf_node( CatmullRom * grandparent
                                                   , CatmullRom * parent
                                                   , CatmullRom * child
                                                   , const uint   axial_segments
                                                   , const uint   radial_segments
                                                   )
{
  uint half = axial_segments / 2;
  uint left_vertex_rows = half + 1;
  uint left_vertex_count = left_vertex_rows * radial_segments;
  uint right_vertex_rows = (axial_segments - half + 1);
  uint right_vertex_count = right_vertex_rows * radial_segments;
  osg::Vec3f D, C, U, V, E;
  osg::Quat Q;
  float l;

  osg::Vec3Array * V0 = new osg::Vec3Array(radial_segments);
  D = parent -> distal - grandparent -> distal;
  l = D.normalize();
  C = (grandparent -> distal + parent -> distal) / 2.0f;
  Shape::fill_regular_polygon( V0
                              , 0
                              , radial_segments
                              , parent -> radius
                              , C
                              , D
                              );
  U = (*V0)[0] - C;
  U.normalize();

  osg::Vec3Array * V1 = new osg::Vec3Array(left_vertex_count);
  E = D;
  D = this -> distal - parent -> distal;
  l = D.normalize();
  C = (this -> distal + parent -> distal) / 2.0f;
  Q.makeRotate(E, D);
  V = Q * U;
  V.normalize();
  Shape::fill_regular_polygon( V1
                             , 0
                             , radial_segments
                             , this -> radius
                             , C
                             , D
                             , V
                             );
  U = (*V1)[0] - C;
  U.normalize();

  osg::Vec3Array * V2 = new osg::Vec3Array(right_vertex_count);
  E = D;
  D = child -> distal - this -> distal;
  l = D.normalize();
  C = (this -> distal + child -> distal) / 2.0f;
  Q.makeRotate(E, D);
  V = Q * U;
  V.normalize();
  Shape::fill_regular_polygon( V2
                             , right_vertex_count - radial_segments
                             , radial_segments
                             , child -> radius
                             , C
                             , D
                             , V
                             );

  osg::Vec3Array * V3 = new osg::Vec3Array(radial_segments);
  C = child -> distal;
  Shape::fill_regular_polygon( V3
                             , 0
                             , radial_segments
                             , child -> radius
                             , C
                             , D
                             , V
                             );


  CatmullRom::interpolate( V0, 0
                         , V1, 0
                         , V2, right_vertex_count - radial_segments
                         , V3, 0
                         , V1, radial_segments
                         , V2, 0
                         , axial_segments
                         , radial_segments
                         );

    osg::Geometry * GL = new osg::Geometry();
    GL -> setName(this -> id());
    GL -> setVertexArray(V1);
    osg::Vec3Array * LN = new osg::Vec3Array(V1 -> size());
    fill_surface_normals(V1, LN, axial_segments, radial_segments);
    GL -> setNormalArray(LN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CL = new osg::Vec4Array(1);
    (*CL)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GL -> setColorArray(CL , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * LI = new osg::DrawElementsUShort(GL_TRIANGLES , (left_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(LI, axial_segments, radial_segments);
    GL -> insertPrimitiveSet(0, LI);
    attach_geometry(GL);

    osg::Geometry * GR = new osg::Geometry();
    GR -> setName(child -> id());
    GR -> setVertexArray(V2);
    osg::Vec3Array * RN = new osg::Vec3Array(V2 -> size());
    Shape::fill_surface_normals(V2, RN, axial_segments, radial_segments);
    GR -> setNormalArray(RN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CR = new osg::Vec4Array(1);
    (*CR)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GR -> setColorArray(CR , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * RI = new osg::DrawElementsUShort(GL_TRIANGLES , (right_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(RI, axial_segments, radial_segments);
    GR -> insertPrimitiveSet(0, RI);
    child -> attach_geometry(GR);
}

void
CatmullRom::_interpolate_root_node_to_internal_node( CatmullRom * child
                                                   , CatmullRom * grandchild
                                                   , const uint   axial_segments
                                                   , const uint   radial_segments
                                                   )
{
  uint half = axial_segments/2;
  uint left_vertex_rows = half + 1;
  uint left_vertex_count = left_vertex_rows * radial_segments;
  uint right_vertex_rows = (axial_segments - half + 1);
  uint right_vertex_count = right_vertex_rows * radial_segments;
  osg::Vec3f dc = child -> distal - this -> distal;
  float lc = dc.normalize();
  float h = (lc + this -> radius) / 2.0f;

  osg::Vec3Array * V0 = new osg::Vec3Array(radial_segments);
  CatmullRom::_fill_pre_tangent_polygon( this -> radius // parent radius
                                       , child -> radius // child radius
                                       , lc // length of child compartment
                                       , this -> distal // center of parent 
                                       , dc // direction of child compartment
                                       , V0 // array in which the ring is to be generated
                                       , 0 // starting index for generation of ring
                                       , axial_segments
                                       , radial_segments
                                       );

  osg::Vec3Array * V1 = new osg::Vec3Array(left_vertex_count);
  CatmullRom::_fill_tangent_polygon( this -> radius // parent radius
                                   , child -> radius // child radius
                                   , lc // length of child compartment
                                   , this -> distal // center of parent
                                   , dc // direction of child compartment
                                   , V1 // array in which the ring is to be generated
                                   , 0 // starting index for generation of ring
                                   , axial_segments
                                   , radial_segments
                                   );


  osg::Vec3Array * V2 = new osg::Vec3Array(right_vertex_count);
  Shape::fill_regular_polygon( V2
                             , right_vertex_count - radial_segments
                             , radial_segments
                             , child -> radius
                             , this -> distal + dc * h
                             , dc
                             );

  osg::Vec3Array * V3 = new osg::Vec3Array(radial_segments);
  osg::Vec3f dgc = grandchild -> distal - child -> distal;
  dgc.normalize();
  Shape::fill_regular_polygon( V3
                             , 0
                             , radial_segments
                             , grandchild -> radius
                             , (child -> distal + grandchild -> distal) / 2.0f
                             , dgc
                             );

  CatmullRom::interpolate( V0, 0
                         , V1, 0
                         , V2, right_vertex_count - radial_segments
                         , V3, 0
                         , V1, radial_segments
                         , V2, 0
                         , axial_segments
                         , radial_segments
                         );

    osg::Geometry * GL = new osg::Geometry();
    GL -> setName(this -> id());
    GL -> setVertexArray(V1);
    osg::Vec3Array * LN = new osg::Vec3Array(V1 -> size());
    fill_surface_normals(V1, LN, axial_segments, radial_segments);
    GL -> setNormalArray(LN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CL = new osg::Vec4Array(1);
    (*CL)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GL -> setColorArray(CL , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * LI = new osg::DrawElementsUShort(GL_TRIANGLES , (left_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(LI, axial_segments, radial_segments);
    GL -> insertPrimitiveSet(0, LI);
    attach_geometry(GL);

    osg::Geometry * GR = new osg::Geometry();
    GR -> setName(child -> id());
    GR -> setVertexArray(V2);
    osg::Vec3Array * RN = new osg::Vec3Array(V2 -> size());
    Shape::fill_surface_normals(V2, RN, axial_segments, radial_segments);
    GR -> setNormalArray(RN, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CR = new osg::Vec4Array(1);
    (*CR)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    GR -> setColorArray(CR , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * RI = new osg::DrawElementsUShort(GL_TRIANGLES , (right_vertex_rows - 1) * 2 * radial_segments * 3);
    Shape::fill_surface_triangles(RI, axial_segments, radial_segments);
    GR -> insertPrimitiveSet(0, RI);
    child -> attach_geometry(GR);
}

void
CatmullRom::_interpolate_leaf_node( CatmullRom * parent
                                  , const uint   axial_segments
                                  , const uint   radial_segments
                                  )
{
    osg::Vec3Array * V = new osg::Vec3Array(2 * radial_segments);
    osg::Vec3f D = this -> distal - parent -> distal;
    D.normalize();
    osg::Vec3f C = (this -> distal + parent -> distal) / 2.0f;
    Shape::fill_regular_polygon( V
                               , 0
                               , radial_segments
                               , radius
                               , C
                               , D
                               );
    Shape::fill_regular_polygon( V
                               , radial_segments
                               , radial_segments
                               , radius
                               , this -> distal
                               , D
                               );
    osg::Geometry * G = new osg::Geometry();
    G -> setName(this -> id());
    G -> setVertexArray(V);
    osg::Vec3Array * N = new osg::Vec3Array(V -> size());
    Shape::fill_surface_normals(V, N, axial_segments, radial_segments);
    G -> setNormalArray(N, osg::Array::BIND_PER_VERTEX);
    osg::Vec4Array * CA = new osg::Vec4Array(1);
    (*CA)[0] = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    G -> setColorArray(CA , osg::Array::BIND_OVERALL);
    osg::DrawElementsUShort * I = new osg::DrawElementsUShort(GL_TRIANGLES , 2 * radial_segments * 3);
    Shape::fill_surface_triangles(I, axial_segments, radial_segments);
    G -> insertPrimitiveSet(0, I);
    attach_geometry(G);
}

