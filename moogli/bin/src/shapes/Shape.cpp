#include "shapes/Shape.hpp"

std::unordered_map<unsigned int, osg::ref_ptr<osg::Vec3Array> > Shape::_polygons;
std::unordered_map<unsigned int, osg::ref_ptr<osg::DrawElementsUShort> > Shape::_basal_triangles;
std::unordered_map<unsigned int, osg::ref_ptr<osg::DrawElementsUShort> > Shape::_axial_triangles;
//std
std::unordered_map<uint, osg::ref_ptr<osg::Vec3Array> > Shape::_regular_polygon_cache;

const osg::Vec3Array *
Shape::regular_polygon(unsigned int vertices)
{
    auto result = Shape::_polygons.find(vertices);
    if(result != Shape::_polygons.end()) { return result -> second.get(); }

    osg::Vec3Array * polygon = new osg::Vec3Array(vertices);
    float theta = 0.0f;
    float theta_delta = osg::PI * 2.0f / vertices;
    unsigned int i;
    for(i = 0; i < vertices; ++i)
    {
        theta = i * theta_delta;
        (*polygon)[i].set(cos(theta), sin(theta), 0.0f);
    }
    Shape::_polygons[vertices] = polygon;
    return polygon;
}

const osg::DrawElementsUShort *
Shape::basal_triangle(unsigned int vertices)
{
    auto result = Shape::_basal_triangles.find(vertices);
    if(result != Shape::_basal_triangles.end()) { return result -> second.get(); }

    unsigned int basal_triangle_count = vertices - 2;
    unsigned int basal_triangle_vertex_count = basal_triangle_count * 3;

    osg::DrawElementsUShort * basal_triangle = new osg::DrawElementsUShort(GL_TRIANGLES, basal_triangle_vertex_count);
    unsigned int i, j, k, l;
    for(i = 0; i < basal_triangle_count; ++i)
    {
        j = i * 3;
        k = j + 1;
        l = k + 1;
        (*basal_triangle)[j] = 0;
        (*basal_triangle)[k] = i + 2;
        (*basal_triangle)[l] = i + 1;
    }
    Shape::_basal_triangles[vertices] = basal_triangle;
    return basal_triangle;
}

const osg::DrawElementsUShort *
Shape::axial_triangle(unsigned int vertices)
{
    auto result = Shape::_axial_triangles.find(vertices);
    if(result != Shape::_axial_triangles.end()) { return result -> second.get(); }

    unsigned int axial_triangle_count = 2 * vertices;
    unsigned int axial_triangle_vertex_count = axial_triangle_count * 3;
    unsigned int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, o = 0;
    osg::DrawElementsUShort * axial_triangle = new osg::DrawElementsUShort(GL_TRIANGLES, axial_triangle_vertex_count);

    for(i = 0; i < vertices; ++i)
    {
        j = i * 6;
        k = j + 1;
        l = k + 1;
        m = l + 1;
        n = m + 1;
        o = n + 1;

        (*axial_triangle)[j] = i;
        (*axial_triangle)[k] = i + vertices;
        (*axial_triangle)[l] = i + 1;
        (*axial_triangle)[m] = i + vertices;
        (*axial_triangle)[n] = i + vertices + 1;
        (*axial_triangle)[o] = i + 1;
    }

    (*axial_triangle)[o] = 0;
    (*axial_triangle)[n] = vertices;
    (*axial_triangle)[l] = 0;

    Shape::_axial_triangles[vertices] = axial_triangle;
    return axial_triangle;
}


osg::Vec3f
Shape::face_normal(osg::Vec3f x, osg::Vec3f y, osg::Vec3f z)
{
    return (y - x) ^ (z - y);
}


void
Shape::fill_regular_polygon( osg::Vec3Array * T
                           , uint index
                           , uint vertices
                           , float radius
                           , const osg::Vec3f & center
                           , const osg::Vec3f & D
                           )
{
    // Fill vector T with the coordinates of a radial_segments sided regular polygon of radius 'radius' centered at 'center' with axis 'axis'
    // starting from index t.
    // A cache is used to speed up generation of coordinates. The cache ensures that sine and cosine operations are not recomputed and that only basic vector operations are required at each step.
    // generates the unit polygons in such a way that the first point is guaranteed to be in the XZ plane.

    std::unordered_map<uint, osg::ref_ptr<osg::Vec3Array> >::const_iterator result = _regular_polygon_cache.find(vertices);
    osg::Vec3Array * P;
    if(result != _regular_polygon_cache.end()) { P = result -> second.get(); }
    else
    {
        P = new osg::Vec3Array(vertices);
        float delta_theta = 2 * osg::PI / vertices;
        float theta;
        for(uint i = 0; i < vertices; ++i)
          {
            theta = i * delta_theta;
            (*P)[i] = osg::Vec3f(cos(theta) , sin(theta) , 0.0f);
          }
        _regular_polygon_cache[vertices] = P;
    }

    osg::Vec3f U(D.z(), 0.0f, -D.x());
    float l = U.length();

    if(l == 0.0f) { U = osg::Vec3f(1.0f, 0.0f, 0.0f); }
    else          { U = U / l; }

    osg::Vec3f V = D ^ U;

    for(size_t i = 0; i < vertices; ++i)
    {
        (*T)[index + i] =  U * (*P)[i][0] * radius
                        +  V * (*P)[i][1] * radius
                        + center;
    }
}

void
Shape::fill_surface_triangles( osg::DrawElementsUShort * I
                             , uint axial_segments
                             , uint radial_segments
                             )
{
    size_t triangle_index = 0;
    size_t rows = I -> size() / ( 6 * radial_segments);
    size_t row, col;
    for(row = 0; row < rows - 1; ++row)
    {
        for(col = 0 ; col < radial_segments; ++col)
        {
            /*************************************

            d---------------c
            |               |
            |               |
            |               |
            |               |
            |               |
            |               |
            a---------------b

            ***************************************/
            ushort a = row * radial_segments + col;
            ushort b = a + 1;
            ushort d = a + radial_segments;
            ushort c = d + 1;
            // std::cerr << a <<" " << b << " " << c << " " << d << std::endl;
            (*I)[triangle_index    ] = a;
            (*I)[triangle_index + 1] = b;
            (*I)[triangle_index + 2] = d;

            (*I)[triangle_index + 3] = d;
            (*I)[triangle_index + 4] = b;
            (*I)[triangle_index + 5] = c;
            triangle_index += 6;
        }

        (*I)[triangle_index - 1]     = (row + 1) * radial_segments;
        (*I)[triangle_index - 2]     = row       * radial_segments;
        (*I)[triangle_index - 5]     = row       * radial_segments;
    }
}

void
Shape::fill_surface_normals( osg::Vec3Array * V
                           , osg::Vec3Array * N
                           , uint axial_segments
                           , uint radial_segments
                           )
{
    size_t rows = V -> size() / radial_segments;
    size_t row, col;
    size_t current_index;
    ushort a;
    ushort b;
    ushort e;
    ushort d;
    ushort c;
    ushort f;
    ushort g;
    ushort h;
    ushort i;

    /*****************************************
        d---------------c---------------f
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        a---------------b---------------e

      ***************************************/
      row = 0;
      col = 0;

      b = row * radial_segments;
      e = b + 1;
      a = b + radial_segments - 1;
      d = a + radial_segments;
      c = b + radial_segments;
      f = e + radial_segments;
      (*N)[b] = face_normal((*V)[a], (*V)[b], (*V)[d])
              + face_normal((*V)[d], (*V)[b], (*V)[c])
              + face_normal((*V)[b], (*V)[e], (*V)[c]);
      (*N)[b].normalize();

      for(col = 1; col < radial_segments - 1; ++col)
      {
            /*************************************

        d---------------c---------------f
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        a---------------b---------------e

            ***************************************/
            b = row * radial_segments + col;
            a = b - 1;
            e = b + 1;
            d = a + radial_segments;
            c = b + radial_segments;
            f = e + radial_segments;
            // std::cerr << a <<" " << b << " " << c << " " << d << std::endl;
            (*N)[b] = face_normal((*V)[a], (*V)[b], (*V)[d])
                    + face_normal((*V)[d], (*V)[b], (*V)[c])
                    + face_normal((*V)[b], (*V)[e], (*V)[c]);
            (*N)[b].normalize();
        }

      /*************************************
        d---------------c---------------f
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        a---------------b---------------e
      ***************************************/


      b = row * radial_segments + col;
      e = row * radial_segments;
      a = b - 1;
      d = a + radial_segments;
      c = b + radial_segments;
      f = e + radial_segments;
      (*N)[b] = face_normal((*V)[a], (*V)[b], (*V)[d])
              + face_normal((*V)[d], (*V)[b], (*V)[c])
              + face_normal((*V)[b], (*V)[e], (*V)[c]);
      (*N)[b].normalize();

    for(row = 1; row < rows - 1; ++row)
    {

      /*************************************
        d---------------c---------------f
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        a---------------b---------------e
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        g---------------h---------------i

      ***************************************/

      col = 0;

      b = row * radial_segments;
      e = b + 1;
      a = b + radial_segments - 1;
      d = a + radial_segments;
      c = b + radial_segments;
      f = e + radial_segments;
      g = a - radial_segments;
      h = b - radial_segments;
      i = e - radial_segments;
      (*N)[b] = face_normal((*V)[a], (*V)[b], (*V)[d])
              + face_normal((*V)[d], (*V)[b], (*V)[c])
              + face_normal((*V)[h], (*V)[i], (*V)[b])
              + face_normal((*V)[b], (*V)[i], (*V)[e])
              + face_normal((*V)[a], (*V)[h], (*V)[b])
              + face_normal((*V)[b], (*V)[e], (*V)[c]);
      (*N)[b].normalize();
 
      for(col = 1; col < radial_segments - 1; ++col)
      {
            /*************************************

            d---------------c---------------f
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            a---------------b---------------e
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            g---------------h---------------i

            ***************************************/
            b = row * radial_segments + col;
            a = b - 1;
            e = b + 1;
            d = a + radial_segments;
            c = b + radial_segments;
            f = e + radial_segments;
            g = a - radial_segments;
            h = b - radial_segments;
            i = e - radial_segments;
            // std::cerr << a <<" " << b << " " << c << " " << d << std::endl;
            (*N)[b] = face_normal((*V)[a], (*V)[b], (*V)[d])
                                + face_normal((*V)[d], (*V)[b], (*V)[c])
                                + face_normal((*V)[h], (*V)[i], (*V)[b])
                                + face_normal((*V)[b], (*V)[i], (*V)[e])
                                + face_normal((*V)[a], (*V)[h], (*V)[b])
                                + face_normal((*V)[b], (*V)[e], (*V)[c]);
            (*N)[b].normalize();
        }

      /*************************************
        d---------------c---------------f
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        a---------------b---------------e
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        g---------------h---------------i

      ***************************************/


      b = row * radial_segments + col;
      e = row * radial_segments;
      a = b - 1;
      d = a + radial_segments;
      c = b + radial_segments;
      f = e + radial_segments;
      g = a - radial_segments;
      h = b - radial_segments;
      i = e - radial_segments;
      (*N)[b] = face_normal((*V)[a], (*V)[b], (*V)[d])
              + face_normal((*V)[d], (*V)[b], (*V)[c])
              + face_normal((*V)[h], (*V)[i], (*V)[b])
              + face_normal((*V)[b], (*V)[i], (*V)[e])
              + face_normal((*V)[a], (*V)[h], (*V)[b])
              + face_normal((*V)[b], (*V)[e], (*V)[c]);
      (*N)[b].normalize();
 
    }

      /*************************************
        a---------------b---------------e
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        g---------------h---------------i

      ***************************************/

      col = 0;

      b = row * radial_segments;
      e = b + 1;
      a = b + radial_segments - 1;
      g = a - radial_segments;
      h = b - radial_segments;
      i = e - radial_segments;
      (*N)[b] = face_normal((*V)[h], (*V)[i], (*V)[b])
              + face_normal((*V)[b], (*V)[i], (*V)[e])
              + face_normal((*V)[a], (*V)[h], (*V)[b]);
      (*N)[b].normalize();
 
      for(col = 1; col < radial_segments - 1; ++col)
      {
            /*************************************

            a---------------b---------------e
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            g---------------h---------------i

            ***************************************/
            b = row * radial_segments + col;
            a = b - 1;
            e = b + 1;
            g = a - radial_segments;
            h = b - radial_segments;
            i = e - radial_segments;
            // std::cerr << a <<" " << b << " " << c << " " << d << std::endl;
            (*N)[b] = face_normal((*V)[h], (*V)[i], (*V)[b])
                    + face_normal((*V)[b], (*V)[i], (*V)[e])
              + face_normal((*V)[a], (*V)[h], (*V)[b]);
            (*N)[b].normalize();
        }

      /*************************************
        a---------------b---------------e
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        g---------------h---------------i

      ***************************************/


      b = row * radial_segments + col;
      e = row * radial_segments;
      a = b - 1;
      g = a - radial_segments;
      h = b - radial_segments;
      i = e - radial_segments;
      (*N)[b] = face_normal((*V)[h], (*V)[i], (*V)[b])
              + face_normal((*V)[b], (*V)[i], (*V)[e])
              + face_normal((*V)[a], (*V)[h], (*V)[b]);
      (*N)[b].normalize();
}


Shape::Shape(const string & id): _outline(new osgFX::Outline())
                               , _cartoon(new osgFX::Cartoon())
                               , _wireframe(new osgFX::Scribe())
                               , _specular_highlights(new osgFX::SpecularHighlights())
                               , _geode(new osg::Geode())
                               , _geometry(new osg::Geometry())
                               , _allocation_required(true)
                               , _construction_required(true)
                               , _coloring_required(true)
{
    _outline -> setName(id);
    _outline -> addChild(_cartoon.get());
    _cartoon -> setName(id);
    _cartoon -> addChild(_wireframe.get());
    _wireframe -> setName(id);
    _wireframe -> addChild(_specular_highlights.get());
    _specular_highlights -> setName(id);
    _specular_highlights -> addChild(_geode.get());
    _geode -> setName(id);
    _geode -> addDrawable(_geometry.get());
    _geode -> getOrCreateStateSet() -> setMode(GL_BLEND,
                                               osg::StateAttribute::ON);
    _geode -> getOrCreateStateSet() -> setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    _geometry -> setName(id);
    _geometry -> setUpdateCallback(new ShapeUpdateCallback(this));
    //setUpdateCallback(callback);
    _geometry -> setUseDisplayList(false);
    _geometry -> setUseVertexBufferObjects(true);
    disable_effects();
    root = _outline;
}

const std::string &
Shape::id() const
{
    return root -> getName();
}
// void
// Shape::allocate()
// { }

// void
// Shape::construct_indices()
// { }

// void
// Shape::construct_vertices()
// { }

// void
// Shape::color()
// { }

bool
Shape::allocation_required() const
{
  return _allocation_required;
}

bool
Shape::construction_required() const
{
  return _construction_required;
}

bool
Shape::coloring_required() const
{
  return _coloring_required;
}

void
Shape::hide()
{
    _geode -> setNodeMask(0x0);
}

void
Shape::show()
{
    _geode -> setNodeMask(0xffffffff);
}

bool
Shape::visible()
{
    return _geode -> getNodeMask();
}

bool
Shape::export_geometry(const std::string & filename, bool with_effects) const
{
  return with_effects ? osgDB::writeNodeFile(*root.get(), filename)
                      : osgDB::writeNodeFile(*_geode.get(), filename);
}


void
Shape::enable_effects()
{
    enable_cartoon_effect();
    enable_outline_effect();
    enable_wireframe_effect();
    enable_specular_highlights_effect();
}

void
Shape::disable_effects()
{
    disable_cartoon_effect();
    disable_outline_effect();
    disable_wireframe_effect();
    disable_specular_highlights_effect();
}

void
Shape::toggle_effects()
{
    toggle_cartoon_effect();
    toggle_outline_effect();
    toggle_wireframe_effect();
    toggle_specular_highlights_effect();
}

void
Shape::enable_outline_effect()
{
    _outline -> setEnabled(true);
}

void
Shape::disable_outline_effect()
{
    _outline -> setEnabled(false);
}

void
Shape::toggle_outline_effect()
{
    _outline -> setEnabled(! _outline -> getEnabled());
}

bool
Shape::outline_effect_enabled() const
{
    return _outline -> getEnabled();
}

void
Shape::set_outline_effect_line_width(float width)
{
  _outline -> setWidth(width);
}

float
Shape::get_outline_effect_line_width() const
{
    return _outline -> getWidth();
}

void
Shape::set_outline_effect_line_color(const osg::Vec4f & color)
{
    _outline -> setColor(color);
}

const osg::Vec4f &
Shape::get_outline_effect_line_color() const
{
    return _outline -> getColor();
}

void
Shape::enable_cartoon_effect()
{
    _cartoon -> setEnabled(true);
}

void
Shape::disable_cartoon_effect()
{
    _cartoon -> setEnabled(false);
}

void
Shape::toggle_cartoon_effect()
{
    _cartoon -> setEnabled(! _cartoon -> getEnabled());
}

bool
Shape::cartoon_effect_enabled() const
{
    return _cartoon -> getEnabled();
}

void
Shape::set_cartoon_effect_outline_width(float width)
{
    _cartoon -> setOutlineLineWidth(width);
}

float
Shape::get_cartoon_effect_outline_width() const
{
    return _cartoon -> getOutlineLineWidth();
}

void
Shape::set_cartoon_effect_outline_color(const osg::Vec4f & color)
{
  _cartoon -> setOutlineColor(color);
}

const osg::Vec4f &
Shape::get_cartoon_effect_outline_color() const
{
  return _cartoon -> getOutlineColor();
}


void
Shape::enable_wireframe_effect()
{
    _wireframe -> setEnabled(true);
}

void
Shape::disable_wireframe_effect()
{
    _wireframe -> setEnabled(false);
}

void
Shape::toggle_wireframe_effect()
{
    _wireframe -> setEnabled(! _wireframe -> getEnabled());
}

bool
Shape::wireframe_effect_enabled() const
{
  return _wireframe -> getEnabled();
}

void
Shape::set_wireframe_effect_line_width(float width)
{
    _wireframe -> setWireframeLineWidth(width);
}

float
Shape::get_wireframe_effect_line_width() const
{
  return _wireframe -> getWireframeLineWidth();
}

void
Shape::set_wireframe_effect_line_color(const osg::Vec4f & color)
{
    _wireframe -> setWireframeColor(color);
}

const osg::Vec4f &
Shape::get_wireframe_effect_line_color() const
{
    return _wireframe -> getWireframeColor();
}


void
Shape::enable_specular_highlights_effect()
{
    _specular_highlights -> setEnabled(true);
}

void
Shape::disable_specular_highlights_effect()
{
    _specular_highlights -> setEnabled(false);
}

void
Shape::toggle_specular_highlights_effect()
{
    _specular_highlights -> setEnabled(! _specular_highlights -> getEnabled());
}

bool
Shape::specular_highlights_effect_enabled() const
{
    return _specular_highlights -> getEnabled();
}

void
Shape::set_specular_highlights_effect_exponent(float exponent)
{
    _specular_highlights -> setSpecularExponent(exponent);
}

float
Shape::get_specular_highlights_effect_exponent() const
{
    return _specular_highlights -> getSpecularExponent();
}

void
Shape::get_specular_highlights_effect_color(const osg::Vec4f & color)
{
    _specular_highlights -> setSpecularColor(color);
}

const osg::Vec4f &
Shape::get_specular_highlights_effect_color() const
{
    return _specular_highlights -> getSpecularColor();
}


Shape::ShapeUpdateCallback::ShapeUpdateCallback(Shape * shape): UpdateCallback()
                                                              , shape(shape)
{ }

void
Shape::ShapeUpdateCallback::update( osg::NodeVisitor *nv
                                  , osg::Drawable *drawable
                                  )
{
    if(shape -> allocation_required())
    {
        shape -> allocate();
        shape -> construct_vertices();
        shape -> construct_indices();
        shape -> color();
    }
    if(shape -> construction_required())
    {
        shape -> construct_vertices();
    }
    if(shape -> coloring_required())
    {
        shape -> color();
    }
}

Shape::~Shape()
{
    _outline = nullptr;
    _cartoon = nullptr;
    _wireframe = nullptr;
    _specular_highlights = nullptr;
    _geode = nullptr;
    _geometry = nullptr;
    root = nullptr;
}


// void
// Shape::fill_rotated_regular_polygon( osg::Vec3Array * V
//                             , const unsigned int index
//                             , const unsigned int vertices
//                             , const float radius
//                             , const osg::Vec3f & direction
//                             , const osg::Vec3f & center
//                             )
// {
//   osg::Vec3Array * polygon = regular_polygon(vertices);
//   osg::Quat rotation(osg::Z_AXIS, direction);
//   for(unsigned int i = 0; i < vertices; ++i)
//     {
//       (*V)[index + i] = ((*polygon)[i] * radius) * rotation + center;
//     }
// }

// void
// Shape::transform_regular_polygon( osg::Vec3Array * V
//                                  , const unsigned int index
//                                  , const unsigned int vertices
//                                  , const float radius
//                                  , const osg::Vec3f & direction
//                                  , const osg::Vec3f & center
//                                  )
// {
//   osg::Vec3Array * polygon = regular_polygon(vertices);
//   osg::Quat rotation(osg::Z_AXIS, direction);
//   for(unsigned int i = 0; i < vertices; ++i)
//     {
//       (*V)[index + i] = ((*polygon)[i] * radius) * rotation + center;
//     }
// }


// void
// Shape::transform_regular_polygon( osg::Vec3Array * V
//                                , const unsigned int index
//                                , const unsigned int vertices
//                                , const float radius
//                                , const osg::Vec3f & direction
//                                , const osg::Vec3f & center
//                                )
// {
//   osg::Vec3Array * polygon = regular_polygon(vertices);
//   osg::Quat rotation(osg::Z_AXIS, direction);
//   for(unsigned int i = 0; i < vertices; ++i)
//   {
//     (*V)[index + i] = ((*polygon)[i] * radius) * rotation + center;
//   }
// }
//_fill_regular_polygon(V0, 0, radial_segments, this -> radius, this -> distal, dc);


