#include "view/Viewer.hpp"

Viewer::Viewer( const QGLFormat & format
              , QWidget * parent
              , const QGLWidget * share_widget
              , Qt::WindowFlags f
                ): GLWidget( format
                           , parent
                           , share_widget
                           , f
                           )
               , _viewer(new osgViewer::Viewer())
               , _shapes(new osg::PositionAttitudeTransform())
               , _manipulator(new osgGA::MultiTouchTrackballManipulator())
{
    //setFormat(format);
    qDebug() << format;
    qDebug() << this -> format();
    // std::cerr << "Multisample Antialiasing << " << f.samples() << std::endl;
    setForwardKeyEvents(true);
    // This ensures that the widget will receive keyboard events. This focus
    // policy is not set by default. The default, Qt::NoFocus, will result in
    // keyboard events that are ignored.
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(200, 200);
    // Ensures that the widget receives mouse move events even though no
    // mouse button has been pressed. We require this in order to let the
    // graphics window switch viewports properly.
    setMouseTracking(true);
    initializeGL();
    _viewer -> setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
    _viewer -> setSceneData(_shapes.get());
    _viewer -> setKeyEventSetsDone(0);
}

void
Viewer::initializeGL()
{
    GLWidget::initializeGL();
    _graphics_window = new osgQt::GraphicsWindowQt(this);
    setGraphicsWindow(_graphics_window.get());
    std::cerr << "Sample Buffers : " << _graphics_window -> getTraits() -> sampleBuffers << std::endl;
    _shapes -> getOrCreateStateSet() -> setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    _shapes -> getOrCreateStateSet() -> setMode(GL_BLEND, osg::StateAttribute::ON);
    _shapes -> getOrCreateStateSet() -> setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    create_cameras();
    _viewer -> setCameraManipulator(_manipulator.get());
}

void
Viewer::create_cameras()
{
    _main_camera = create_perspective_projection_camera();
    _heads_up_display_camera = create_orthographic_projection_camera();
    _viewer -> setCamera(_main_camera.get());
    _viewer -> addSlave(_heads_up_display_camera.get(), false);
}

osg::Camera *
Viewer::create_perspective_projection_camera()
{
    osg::Camera * camera = new osg::Camera();
    camera -> getOrCreateStateSet() -> setGlobalDefaults();
    camera -> setViewport(new osg::Viewport( _graphics_window -> getTraits() -> x
                                           , _graphics_window -> getTraits() -> y
                                           , _graphics_window -> getTraits() -> width
                                           , _graphics_window -> getTraits() -> height
                                           )
                         );
    std::cerr << _graphics_window -> getTraits() -> x << std::endl;
    std::cerr << _graphics_window -> getTraits() -> y << std::endl;
    std::cerr << _graphics_window -> getTraits() -> width << std::endl;
    std::cerr << _graphics_window -> getTraits() -> height << std::endl;
    camera -> setProjectionMatrixAsPerspective( 30.0f
                                              , camera -> getViewport() -> aspectRatio()
                                              , 0.01f
                                              , 10000.0f
                                              );
    camera -> setClearColor(_background_color);
    camera -> setClearMask(camera -> getClearMask()|GL_STENCIL_BUFFER_BIT);
    camera -> setClearStencil(0);

    camera -> setGraphicsContext(_graphics_window.get());
    //camera -> setPostDrawCallback(new CaptureView());
    return camera;
}


osg::Camera *
Viewer::create_orthographic_projection_camera()
{
    osg::Camera * camera = new osg::Camera;
    // set the projection matrix
    camera -> setViewport(new osg::Viewport( _graphics_window -> getTraits() -> x
                                           , _graphics_window -> getTraits() -> y
                                           , _graphics_window -> getTraits() -> width
                                           , _graphics_window -> getTraits() -> height
                                           )
                          );

    camera -> setProjectionMatrix(osg::Matrix::ortho2D( 0
                                                      , _graphics_window -> getTraits() -> width
                                                      , 0
                                                      , _graphics_window -> getTraits() -> height
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
    camera -> setGraphicsContext(_graphics_window.get());
    return camera;
}



void
Viewer::set_background_color(const osg::Vec4f & background_color)
{
    _background_color = background_color;
    _viewer -> getCamera() -> setClearColor(_background_color);
    for(unsigned int i = 0; i < _viewer -> getNumSlaves(); ++i)
    {
        _viewer -> getSlave(i)._camera -> setClearColor(_background_color);
    }
}

const osg::Vec4f &
Viewer::get_background_color()
{
    return _background_color;
}

void
Viewer::updateGL()
{
     paintGL();
}

void
Viewer::paintGL()
{
    _viewer -> frame();
}

//void Viewer::resizeGL(int width, int height)
//{
//     _graphics_window -> resized(0, 0, width, height);
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     //for(osgViewer::View * view : views) { view -> resize(); }
// }

void
Viewer::attach_shape(Shape * shape)
{
    _shapes -> addChild(shape -> root.get());
    _shapes -> dirtyBound();
}

void
Viewer::detach_shape(Shape * shape)
{
    _shapes -> removeChild(shape -> root.get());
    _shapes -> dirtyBound();
}


const osgUtil::LineSegmentIntersector::Intersections
Viewer::intersections(int x, int y)
{
    // std::cerr << "X : " << x << std::endl;
    // std::cerr << "Y : " << y << std::endl;
    // std::cerr << _graphics_window -> getEventQueue() -> getCurrentEventState() -> getXnormalized() << std::endl;
    // std::cerr << _graphics_window -> getEventQueue() -> getCurrentEventState() -> getYnormalized() << std::endl;

    osgUtil::LineSegmentIntersector::Intersections intersections;

     _viewer -> computeIntersections( _main_camera.get()
                                    , osgUtil::Intersector::CoordinateFrame::WINDOW
                                    , x
                                    , y
                                    , intersections
                                    );

    return intersections;
    // std::vector<std::string> names;//intersections.size());
    // for(const osgUtil::LineSegmentIntersector::Intersection & hit : intersections)
    // {
    //     std::cerr << hit.drawable -> className() << std::endl;
    //     if(hit.drawable -> asGeometry())
    //     {
    //         names.push_back(hit.drawable -> getName());
    //     }
    // }
    // return names;
}


// const osgUtil::LineSegmentIntersector::Intersections
// Viewer::intersections(int x, int y)
// {
// }

const osgUtil::LineSegmentIntersector::Intersections &
Viewer::intersections(const osg::Vec3f & start, const osg::Vec3f & end)
{
    osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
        new osgUtil::LineSegmentIntersector(start, end);
    osgUtil::IntersectionVisitor iv(intersector.get());
    _shapes -> accept(iv);
    return intersector -> getIntersections();
}


// void
// Viewer::focus(Shape * shape)
// {
    
// }
