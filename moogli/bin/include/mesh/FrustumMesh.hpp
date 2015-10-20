#include "mesh/FrustumMesh.hpp"

class FrustumMesh : public Mesh
{
public:
    FrustumMesh();

    void
    set_direction(const osg::Vec3f & direction);

    osg::Vec3f
    get_direction() const;

    void
    set_length(float length);

    float
    get_length() const;

    void
    set_center(const osg::Vec3f & center);

    osg::Vec3f
    get_center() const;

    void
    set_base(const osg::Vec3f & base);

    osg::Vec3f
    get_base() const;

    void
    set_apex(const osg::Vec3f & apex);

    osg::Vec3f
    get_apex() const;

    void
    set_apex_radius(const float apex_radius);

    float
    get_apex_radius() const;

    void
    set_base_radius(const float base_radius);

    float
    get_base_radius() const;

    void
    move_apex_along_axis_by(const float movement_percentage);

    void
    move_base_along_axis_by(const float movement_percentage);

    void
    set_vertices(uint vertices);

    uint
    get_vertices() const;

protected:

    float _apex_radius;
    float _base_radius;
    osg::Vec3f _apex;
    osg::Vec3f _base;
    osg::Vec3f _center;
    osg::Vec3f _direction;
    float _length;
    uint _vertices;
};

