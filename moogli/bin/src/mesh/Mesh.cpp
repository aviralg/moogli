#include "mesh/Mesh.hpp"

Mesh::Mesh(osg::Drawable::UpdateCallback * callback) : osg::Geometry()
                                                     , _allocation_required(true)
                                                     , _construction_required(true)
                                                     , _coloring_required(true)
{
    setUpdateCallback(callback);
    setUseDisplayList(false);
    setUseVertexBufferObjects(true);
}

bool
Mesh::allocation_required()   { return _allocation_required;   }

bool
Mesh::construction_required() { return _construction_required; }

bool
Mesh::coloring_required()     { return _coloring_required;     }


void
Mesh::create_regular_polygon()

void
Mesh::fill_regular_polygon( osg::Vec3Array * V
                          , const unsigned int index
                          , const unsigned int vertices
                          , const float radius
                          , const osg::Vec3f & direction
                          , const osg::Vec3f & center
                          )
{
    _polygons.get
}

CylinderMesh::CylinderMesh(osg::Vec3f center, osg::Vec3f direction, float length, float radius)
{
_center = center;
_direction = direction;
_length = length;
_base_radius = radius;
_apex_radius = radius;
_radial_segments = radial_segments;
_update_geometry = true;
}

class CylinderMesh : public Mesh
{
  osg::Vec3f base;
  osg::Vec3f apex;
  osg::Vec3f base_radius;
  osg::Vec3f apex_radius;

  bool _update_geometry;

  CylinderMesh(osg::Vec3f center, osg::Vec3f direction, float length, float radius);

  CylinderMesh(osg::Vec3f center, osg::Vec3f direction, float length, float base_radius, float apex_radius);

  CylinderMesh(osg::Vec3f base, osg::Vec3f apex, float base_radius, float apex_radius);

  void
  set_base(osg::Vec3f base);

  osg::Vec3f
  get_base();


  void
  set_apex();

  void
  get_apex();

  void
  set_quality();

  void
  get_quality();

  void
  set_radius();

  void
  set_base_radius();

  void
  set_apex_radius();

  void
  set_center();

  void
  get_center();

  void
  get_direction();

  void
  set_direction();

  void
  get_length();

  void
  set_length();
}

