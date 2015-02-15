#include "view/CompositeView.hpp"

CompositeView::CompositeView( QWidget * parent
                            , const QGLWidget * share_widget
                            , Qt::WindowFlags f
                            ) : GLWidget(parent, share_widget, f)
                              , _view(new osgViewer::View())
                              , _shapes(new osg::PositionAttitudeTransform())
                              , _manipulator(new osgGA::MultiTouchTrackballManipulator())

{
    QGLWidget::setFormat(QGLFormat(QGL::SampleBuffers));
    setForwardKeyEvents(true);
    setFocusPolicy(Qt::StrongFocus);
    create_cameras();
    _view -> setCameraManipulator(_manipulator.get());
    _view -> setSceneData(_shapes.get());
    // Ensures that the widget receives mouse move events even though no
    // mouse button has been pressed. We require this in order to let the
    // graphics window switch viewports properly.
    setMouseTracking(true);
    initializeGL();
}

void
CompositeView::initializeGL()
{
    GLWidget::initializeGL();
    setGraphicsWindow(new osgQt::GraphicsWindowQt(this));
    _shapes -> getOrCreateStateSet() -> setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    _shapes -> getOrCreateStateSet() -> setMode(GL_BLEND, osg::StateAttribute::ON);
}

void
CompositeView::setParent(QWidget * parent, Qt::WindowFlags f)
{
    std::cerr << "Setting Parent" << std::endl;
    osgViewer::CompositeViewer * viewer = dynamic_cast<osgViewer::CompositeViewer *>(this -> parent());
    if(viewer != nullptr)
    {
        viewer -> removeView(_view.get());
    }
    viewer = dynamic_cast<osgViewer::CompositeViewer *>(parent);
    if(viewer != nullptr)
    {
        std::cerr << "Found Composite Viewer" << std::endl;
        viewer -> addView(_view.get());
        viewer -> setDone(false);
    }
    GLWidget::setParent(parent, f);
}

void
CompositeView::setParent(QWidget * parent)
{
    setParent(parent, 0);
}


void
CompositeView::attach_shape(Shape * shape)
{
    _shapes -> addChild(shape -> root.get());
}

void
CompositeView::detach_shape(Shape * shape)
{
    _shapes -> removeChild(shape -> root.get());
}

void
CompositeView::create_cameras()
{
    _main_camera = create_perspective_projection_camera();
    _heads_up_display_camera = create_orthographic_projection_camera();
    _view -> setCamera(_main_camera.get());
    _view -> addSlave(_heads_up_display_camera.get(), false);
}

osg::Camera *
CompositeView::create_perspective_projection_camera()
{
    osg::Camera * camera = new osg::Camera();
    //camera -> setViewport(viewport);
    camera -> setProjectionMatrixAsPerspective( 30.0f
                                              , 1.0f
                                              , 1.0f
                                              , 10000.0f
                                              );
    camera -> setClearColor(_background_color);
    camera -> setClearMask(camera -> getClearMask()|GL_STENCIL_BUFFER_BIT);
    camera -> setClearStencil(0);
    camera -> setGraphicsContext(getGraphicsWindow());
    camera -> getOrCreateStateSet() -> setGlobalDefaults();
    //camera -> setPostDrawCallback(new CaptureView());
    return camera;
}


osg::Camera *
CompositeView::create_orthographic_projection_camera()
{
    osg::Camera * camera = new osg::Camera;
    // set the projection matrix
    camera -> setProjectionMatrix(osg::Matrix::ortho2D( 0
                                                      , 100
                                                      , 0
                                                      , 100
                                                      )
                                 );
    // set the view matrix
    camera -> setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera -> setViewMatrix(osg::Matrix::identity());

    // only clear the depth buffer
    camera -> setClearMask(GL_DEPTH_BUFFER_BIT);

    // draw subgraph after main camera view.
    camera -> setRenderOrder(osg::Camera::POST_RENDER);

    // we don't want the camera to grab event focus from the viewers main camera(s).
    camera -> setAllowEventFocus(false);
    camera -> setGraphicsContext(getGraphicsWindow());
    return camera;
}

void
CompositeView::set_background_color(const osg::Vec4f & background_color)
{
    _background_color = background_color;
    _view -> getCamera() -> setClearColor(_background_color);
    for(unsigned int i = 0; i < _view -> getNumSlaves(); ++i)
    {
        _view -> getSlave(i)._camera -> setClearColor(_background_color);
    }
}

const osg::Vec4f &
CompositeView::get_background_color()
{
    return _background_color;
}

void
CompositeView::attach_color_bar(ColorBar * color_bar)
{
    _heads_up_display_camera -> addChild(color_bar -> root.get());
    osg::Viewport * viewport = _main_camera -> getViewport();
    color_bar -> resize( 0//viewport -> x()
                         , 0//viewport -> y()
                         , 100//viewport -> width()
                         , 100//viewport -> height()
                       );
}

void
CompositeView::detach_color_bar(ColorBar * color_bar)
{
    _heads_up_display_camera -> removeChild(color_bar -> root.get());
}

void
CompositeView::home()
{
    _manipulator -> home(0.0);
}

void
CompositeView::forward(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye + look * distance_delta, center + look * distance_delta, up);
}

void
CompositeView::backward(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye - look * distance_delta, center - look * distance_delta, up);
}

void
CompositeView::left(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye + side * distance_delta, center + side * distance_delta, up);
}

void
CompositeView::right(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye - side * distance_delta, center - side * distance_delta, up);
}

void
CompositeView::up(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye - up * distance_delta, center - up * distance_delta, up);
}

void
CompositeView::down(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye + up * distance_delta, center + up * distance_delta, up);
}

void
CompositeView::zoom(double factor)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye + look * factor * distance, center, up);
}

void
CompositeView::_get_transformation( osg::Vec3d & eye
                             , osg::Vec3d & center
                             , double & distance
                             , osg::Vec3d & up
                             , osg::Vec3d & look
                             , osg::Vec3d & side
                             )
{
    _manipulator -> getTransformation(eye, center, up);
    up.normalize();
    look = center - eye;
    distance = look.normalize();
    side = look ^ up;
    side.normalize();
}

void
CompositeView::_set_transformation( const osg::Vec3d & eye
                               , const osg::Vec3d & center
                               , const osg::Vec3d & up
                               )
{
    _manipulator->setTransformation(eye, center, up);
}

void
CompositeView::roll(double angle)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    osg::Quat rotation(-angle, look);
    osg::Vec3f rotated_up = rotation * up;
    rotated_up.normalize();
    _set_transformation(eye, center, rotated_up);
}

void
CompositeView::pitch(double angle)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    osg::Quat rotation(-angle, side);//osg::X_AXIS);//side);
    osg::Vec3f rotated_up = rotation * up;
    rotated_up.normalize();
    osg::Vec3f rotated_look = rotation * look;
    rotated_look.normalize();
    _set_transformation( center - rotated_look * distance
                       , center
                       , rotated_up
                       );
}

void
CompositeView::yaw(double angle)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    osg::Quat rotation = osg::Quat(-angle, up);
    osg::Vec3f rotated_look = rotation * look;
    rotated_look.normalize();
    _set_transformation(center - rotated_look * distance, center, up);
}

    // void
    // attach_text_twod();

    // void
    // detach_text_twod();

    // void
    // attach_image();

    // void
    // detach_image();

    // void
    // attach_text_threed();

    // void
    // detach_text_threed();

    // void
    // attach_coordinate_axes();

    // void
    // detach_coordinate_axes();

    // void
    // attach_scale_mesh();

    // void
    // detach_scale_mesh();

    /* data */
