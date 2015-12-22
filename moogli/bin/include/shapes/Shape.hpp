#ifndef __SHAPE_HPP__
#define __SHAPE_HPP__

#include "utility/libraries.hpp"

class Shape
{
private:
    static std::unordered_map<unsigned int, osg::ref_ptr<osg::Vec3Array> > _polygons;
    static std::unordered_map<unsigned int, osg::ref_ptr<osg::DrawElementsUShort> > _basal_triangles;
    static std::unordered_map<unsigned int, osg::ref_ptr<osg::DrawElementsUShort> > _axial_triangles;
public:
    static const osg::Vec3Array *
    regular_polygon(unsigned int vertices);

    static const osg::DrawElementsUShort *
    basal_triangle(unsigned int vertices);

    static const osg::DrawElementsUShort *
    axial_triangle(unsigned int vertices);

    static osg::Vec3f
    face_normal(osg::Vec3f x, osg::Vec3f y, osg::Vec3f z);

    static void
    fill_regular_polygon( osg::Vec3Array * T
                        , uint index
                        , uint vertices
                        , float radius
                        , const osg::Vec3f & center
                        , const osg::Vec3f & D
                        , const osg::Vec3f & U
                        );

    static void
    fill_regular_polygon( osg::Vec3Array * V
                        , uint index
                        , uint vertices
                        , float radius
                        , const osg::Vec3f & center
                        , const osg::Vec3f & D
                        );

    static void
    fill_surface_normals( osg::Vec3Array * V
                        , osg::Vec3Array * N
                        , uint axial_segments
                        , uint radial_segments
                        );

    static void
    fill_surface_triangles( osg::DrawElementsUShort * I
                          , uint axial_segments
                          , uint radial_segments
                          );

    osg::ref_ptr<osg::Node> root;

    Shape(const string & id);

    const std::string &
    id() const;

    virtual void
    allocate() = 0;

    virtual void
    construct_vertices() = 0;

    virtual void
    construct_indices() = 0;

    virtual void
    color() = 0;

    bool
    allocation_required() const;

    bool
    construction_required() const;

    bool
    coloring_required() const;

    void
    hide();

    void
    show();

    bool
    visible();

    bool
    export_geometry(const std::string & filename, bool with_effects = false) const;

    void
    create_effect_nodes();

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
    set_outline_effect_line_color(const osg::Vec4f & color);

    const osg::Vec4f &
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
    set_cartoon_effect_outline_color(const osg::Vec4f & color);

    const osg::Vec4f &
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
    set_wireframe_effect_line_color(const osg::Vec4f & color);

    const osg::Vec4f &
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
    get_specular_highlights_effect_color(const osg::Vec4f & color);

    const osg::Vec4f &
    get_specular_highlights_effect_color();

    virtual
    ~Shape();

protected:

    bool _allocation_required;
    bool _construction_required;
    bool _coloring_required;

    osg::ref_ptr<osgFX::Cartoon> _cartoon;
    osg::ref_ptr<osgFX::Outline> _outline;
    osg::ref_ptr<osgFX::Scribe>  _wireframe;
    osg::ref_ptr<osgFX::SpecularHighlights> _specular_highlights;
    osg::ref_ptr<osg::Geode> _geode;
    osg::ref_ptr<osg::Geometry> _geometry;
    static std::unordered_map<uint, osg::ref_ptr<osg::Vec3Array> > _regular_polygon_cache;

public:

    class ShapeUpdateCallback : public osg::Drawable::UpdateCallback
    {
    public:
        ShapeUpdateCallback(Shape * shape);

        virtual void
        update( osg::NodeVisitor * nv
              , osg::Drawable * drawable
              );
    private:
        Shape * shape;
    };
};

#endif /* __SHAPE_HPP__ */
