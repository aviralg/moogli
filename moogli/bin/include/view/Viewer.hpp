#ifndef __VIEWER_HPP__
#define __VIEWER_HPP__

#include "utility/libraries.hpp"
#include "utility/constants.hpp"
//#include "core/Network.hpp"
//#include "handlers/GeometrySelector.hpp"
//#include "callbacks/CaptureView.hpp"
//#include "core/CoordinateSystem.hpp"
#include "view/View.hpp"
#include "shapes/Shape.hpp"
//#include "callbacks/MeshUpdateCallback.hpp"

class Viewer : public osgQt::GLWidget
{
  Q_OBJECT

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
    create_cameras();

    osg::Camera *
    create_perspective_projection_camera();

    osg::Camera *
    create_orthographic_projection_camera();

    void
    set_background_color(const osg::Vec4f & background_color);

    const osg::Vec4f &
    get_background_color();

protected:
    virtual void
    initializeGL();

    void
    paintGL();

signals:
    void
    selected(const char * id);

private:
    osg::ref_ptr<osgGA::MultiTouchTrackballManipulator> _manipulator;
    osg::ref_ptr<osg::Camera> _main_camera;
    osg::ref_ptr<osg::Camera> _heads_up_display_camera;
    osg::ref_ptr<osg::PositionAttitudeTransform> _shapes;
    osg::ref_ptr<osgQt::GraphicsWindowQt> _graphics_window;
    osg::Vec4f _background_color;
    osg::ref_ptr<osgViewer::Viewer> _viewer;

public Q_SLOTS:
    void
    updateGL();

    // const osgUtil::LineSegmentIntersector::Intersections
    // intersections(float x, float y);

    const osgUtil::LineSegmentIntersector::Intersections
    intersections(int x, int y);

    const osgUtil::LineSegmentIntersector::Intersections &
    intersections(const osg::Vec3f & start, const osg::Vec3f & end);
};

#endif /* __VIEWER_HPP__ */
