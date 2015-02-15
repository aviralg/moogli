#ifndef __COMPOSITE_VIEW_HPP__
#define __COMPOSITE_VIEW_HPP__

#include "utility/libraries.hpp"
#include "shapes/Shape.hpp"
#include "widgets/ColorBar.hpp"

class CompositeView : public osgQt::GLWidget
{
public:

    CompositeView( QWidget * parent = nullptr
                 , const QGLWidget * share_widget = nullptr
                 , Qt::WindowFlags f = 0
                 );

    void
    attach_shape(Shape * shape);

    void
    detach_shape(Shape * shape);

    void
    set_background_color(const osg::Vec4f & background_color);

    const osg::Vec4f &
    get_background_color();

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

    void
    create_cameras();

    osg::Camera *
    create_perspective_projection_camera();

    osg::Camera *
    create_orthographic_projection_camera();


private:

    void
    _get_transformation( osg::Vec3d & eye
                       , osg::Vec3d & center
                       , double & distance
                       , osg::Vec3d & up
                       , osg::Vec3d & look
                       , osg::Vec3d & side
                       );

    void
    _set_transformation( const osg::Vec3d & eye
                       , const osg::Vec3d & center
                       , const osg::Vec3d & up
                       );

    osg::ref_ptr<osg::PositionAttitudeTransform> _shapes;
    osg::ref_ptr<osgGA::MultiTouchTrackballManipulator> _manipulator;
    osg::ref_ptr<osg::Camera> _main_camera;
    osg::ref_ptr<osg::Camera> _heads_up_display_camera;
    osg::ref_ptr<osgViewer::View> _view;
    osg::Vec4f _background_color;
};

#endif /* __COMPOSITE_VIEW_HPP__ */
