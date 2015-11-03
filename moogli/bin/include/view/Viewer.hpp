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
    Viewer( QWidget * parent = nullptr
          , const QGLWidget * share_widget = nullptr
          , Qt::WindowFlags f = 0
          );

    void
    attach_view(MeshView * mesh_view);

    void
    detach_view(MeshView * mesh_view);

    void
    attach_shape(Shape * shape);

    void
    detach_shape(Shape * shape);

    osgViewer::View *
    get_view_with_focus();

protected:
    void
    initializeGL();

    void
    paintGL();

    bool
    event(QEvent * event);

signals:
    void
    selected(const char * id);

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> _shapes;
    osg::ref_ptr<osgQt::GraphicsWindowQt> _graphics_window;
    osg::ref_ptr<osgViewer::CompositeViewer> _viewer;

public Q_SLOTS:
    void
    updateGL();
};

#endif /* __VIEWER_HPP__ */
