#include "shapes/EllipsoidalSurface.hpp"

EllipsoidalSurface::EllipsoidalSurface(const std::string & id) : Shape(id)
{
    set( osg::Vec3f(0.0, 0.0, 0.0)
       , 1
       , 2
       , 3
       , osg::X_AXIS
       , osg::Y_AXIS
       , 0.0f
       , osg::PI_2
       , 0.0f
       , osg::PI_2
       , 10
       , osg::Vec4f(1.0, 1.0, 0.0, 1.0)
       );
    allocate();
    construct_indices();
    construct_vertices();
    this -> color();
}


EllipsoidalSurface::EllipsoidalSurface( const std::string & id
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
                                      ) : Shape(id)
{
    set( center
       , a
       , b
       , c
       , a_axis
       , b_axis
       , theta_min
       , theta_max
       , phi_min
       , phi_max
       , vertices
       , color
       );
    allocate();
    construct_indices();
    construct_vertices();
    this -> color();
}

void
EllipsoidalSurface::set_geometry( const osg::Vec3f & center
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
                     )
{
    _a = a;
    _b = b;
    _c = c;
    _a_axis = a_axis;
    _b_axis = b_axis;
    _c_axis = _a_axis ^ _b_axis;
    _c_axis.normalize();
    _theta_min = theta_min;
    _theta_max = theta_max;
    _phi_min = phi_min;
    _phi_max = phi_max;
    _vertices = vertices;
    _allocation_required = true;
    _construction_required = true;
}

void
EllipsoidalSurface::set( const osg::Vec3f & center
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
            )
{
    set_geometry( center
                , a
                , b
                , c
                , a_axis
                , b_axis
                , theta_min
                , theta_max
                , phi_min
                , phi_max
                , vertices
                );
    set_color(color);
}

void
EllipsoidalSurface::allocate()
{
    // if(_geometry -> getVertexArray() == nullptr)
    // {
        _geometry -> setVertexArray(new osg::Vec3Array(_vertices * _vertices));
    // }
    // else
    // {
    //     _geometry -> getVertexArray() -> resizeArray(_vertices * _vertices);
    // }
    // if(_geometry -> getNormalArray() == nullptr)
    // {
        _geometry -> setNormalArray( new osg::Vec3Array(_vertices * _vertices)
                                   , osg::Array::BIND_PER_VERTEX
                                   );
    // }
    // else
    // {
    //     _geometry -> getNormalArray() -> resizeArray(_vertices * _vertices);
    // }
    // if(_geometry -> getColorArray() == nullptr)
    // {
        _geometry -> setColorArray( new osg::Vec3Array(1)
                                  , osg::Array::BIND_OVERALL
                                  );
    // }
    // else
    // {
    //     _geometry -> getColorArray() -> resizeArray();
    //     _geometry -> setColorArray(new osg::Vec4Array(1), osg::Array::BIND_OVERALL);
    // }


        _geometry -> insertPrimitiveSet(0, new osg::DrawElementsUShort(GL_TRIANGLES, 6 * (_vertices - 1) * (_vertices - 1)));
    _allocation_required = false;
}


void
EllipsoidalSurface::construct_vertices()
{
     osg::Vec3Array * V = static_cast<osg::Vec3Array *>(_geometry -> getVertexArray());
     osg::Vec3Array * N = static_cast<osg::Vec3Array *>(_geometry -> getNormalArray());
     float theta_diff = _theta_max - _theta_min;
     float delta_theta = theta_diff / (_vertices - 1);
     float phi_diff = _phi_max - _phi_min;
     float delta_phi = phi_diff / (_vertices - 1);
     float theta, cos_theta, sin_theta;
     float phi;
     unsigned int index = 0, theta_index, phi_index;
     for(theta_index  = 0; theta_index < _vertices; ++theta_index)
     {
         theta = _theta_min + theta_index * delta_theta;
         cos_theta = cos(theta);
         sin_theta = sin(theta);
         for(phi_index = 0; phi_index < _vertices; ++phi_index)
         {
             phi = _phi_min + phi_index * delta_phi;
             (*N)[index] = _a_axis * _a * sin_theta * cos(phi)
                         + _b_axis * _b * sin_theta * sin(phi)
                         + _c_axis * _c * cos_theta;

             (*V)[index] = (*N)[index] + _center;
             (*N)[index].normalize();
             ++index;
         }
    }
    V -> dirty();
    N -> dirty();
    _geometry -> dirtyBound();
    _construction_required = false;
}


void
EllipsoidalSurface::construct_indices()
{
    osg::DrawElementsUShort * I = static_cast<osg::DrawElementsUShort *>(_geometry -> getPrimitiveSet(0));
    unsigned int index = 0;
    unsigned int row_index, col_index;
    unsigned int row;

    for(row_index = 0; row_index < _vertices - 1; ++row_index)
    {
        row = row_index * _vertices;
        for(col_index = 0; col_index < _vertices - 1; ++col_index)
        {
            (*I)[index    ] = row + col_index;
            (*I)[index + 1] = row + col_index + _vertices;
            (*I)[index + 2] = row + col_index + _vertices + 1;
            (*I)[index + 3] = row + col_index + _vertices + 1;
            (*I)[index + 4] = row + col_index + 1;
            (*I)[index + 5] = row + col_index;
            index += 6;
        }
    }
    I -> dirty();
}

void
EllipsoidalSurface::color()
{
    osg::Vec4Array * C = static_cast<osg::Vec4Array *>(_geometry -> getColorArray());
    (*C)[0] = _color;
    C -> dirty();
    _coloring_required = false;
}


float
EllipsoidalSurface::get_phi_min() const
{
    return _phi_min;
}

void
EllipsoidalSurface::set_phi_min(float phi_min)
{
    _phi_min = phi_min;
    _construction_required = true;
}

float
EllipsoidalSurface::get_phi_max() const
{
  return _phi_max;
}

void
EllipsoidalSurface::set_phi_max(float phi_max)
{
  _phi_max = phi_max;
  _construction_required = true;
}

float
EllipsoidalSurface::get_theta_min() const
{
  return _theta_min;
}

void
EllipsoidalSurface::set_theta_min(float theta_min)
{
  _theta_min = theta_min;
  _construction_required = true;
}

float
EllipsoidalSurface::get_theta_max() const
{
  return _theta_max;
}

void
EllipsoidalSurface::set_theta_max(float theta_max)
{
  _theta_max = theta_max;
  _construction_required = true;
}

float
EllipsoidalSurface::get_a() const
{
  return _a;
}

void
EllipsoidalSurface::set_a(float a)
{
  _a = a;
  _construction_required = true;
}

float
EllipsoidalSurface::get_b() const
{
  return _b;
}

void
EllipsoidalSurface::set_b(float b)
{
  _b = b;
  _construction_required = true;
}

float
EllipsoidalSurface::get_c() const
{
  return _c;
}

void
EllipsoidalSurface::set_c(float c)
{
  _c = c;
  _construction_required = true;
}

void
EllipsoidalSurface::set_abc(float a, float b, float c)
{
  _a = a;
  _b = b;
  _c = c;
  _construction_required = true;
}

void
EllipsoidalSurface::set_theta(float theta_min, float theta_max)
{
  _theta_min = theta_min;
  _theta_max = theta_max;
  _construction_required = true;
}

void
EllipsoidalSurface::set_phi(float phi_min, float phi_max)
{
  _phi_min = phi_min;
  _phi_max = phi_max;
  _construction_required = true;
}

void
EllipsoidalSurface::set_center(const osg::Vec3f & center)
{
  _center = center;
  _construction_required = true;
}

const osg::Vec3f &
EllipsoidalSurface::get_center() const
{
  return _center;
}

void
EllipsoidalSurface::set_color(const osg::Vec4f & color)
{
  _color = color;
  _coloring_required = true;
}

const osg::Vec4f &
EllipsoidalSurface::get_color() const
{
  return _color;
}

void
EllipsoidalSurface::set_a_and_b_axes(const osg::Vec3f & a_axis, const osg::Vec3f & b_axis)
{
    _a_axis = a_axis;
    _b_axis = b_axis;
    _c_axis = a_axis ^ b_axis;
    _c_axis.normalize();
    _construction_required = true;
}

void
EllipsoidalSurface::set_b_and_c_axes(const osg::Vec3f & b_axis, const osg::Vec3f & c_axis)
{
    _b_axis = b_axis;
    _c_axis = c_axis;
    _a_axis = b_axis ^ c_axis;
    _a_axis.normalize();
    _construction_required = true;
}

void
EllipsoidalSurface::set_c_and_a_axes(const osg::Vec3f & c_axis, const osg::Vec3f & a_axis)
{
    _c_axis = c_axis;
    _a_axis = a_axis;
    _b_axis = c_axis ^ a_axis;
    _b_axis.normalize();
    _construction_required = true;
}

const osg::Vec3f &
EllipsoidalSurface::get_a_axis() const
{
  return _a_axis;
}

const osg::Vec3f &
EllipsoidalSurface::get_b_axis() const
{
  return _b_axis;
}

const osg::Vec3f &
EllipsoidalSurface::get_c_axis() const
{
  return _c_axis;
}

void
EllipsoidalSurface::set_vertices(unsigned int vertices)
{
    _vertices = vertices;
    _allocation_required = true;
    _construction_required = true;
}

unsigned int
EllipsoidalSurface::get_vertices() const
{
    return _vertices;
}
