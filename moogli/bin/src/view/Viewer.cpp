#include "view/Viewer.hpp"

Viewer::Viewer( QWidget * parent
              , const QGLWidget * share_widget
              , Qt::WindowFlags f
              ): GLWidget( parent
                         , share_widget
                         , f)
               , _viewer(new osgViewer::CompositeViewer())
               , _shapes(new osg::PositionAttitudeTransform())
{

    QGLWidget::setFormat(QGLFormat(QGL::SampleBuffers));
    setForwardKeyEvents(true);
    // osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
    // traits -> x = this -> x();
    // traits -> y = this -> y();
    // traits -> width = this -> width();
    // traits -> height = this -> height();
    // traits -> windowDecoration = false;
    // traits -> doubleBuffer = true;
    // traits -> sharedContext = 0;
    // traits -> samples = 4;

    // This ensures that the widget will receive keyboard events. This focus
    // policy is not set by default. The default, Qt::NoFocus, will result in
    // keyboard events that are ignored.
    this -> setFocusPolicy(Qt::StrongFocus);
    this -> setMinimumSize(200, 200);

    // Ensures that the widget receives mouse move events even though no
    // mouse button has been pressed. We require this in order to let the
    // graphics window switch viewports properly.
    this -> setMouseTracking(true);
}

void
Viewer::initializeGL()
{
    std::cerr << "Initialize GL";
    GLWidget::initializeGL();
    _graphics_window = new osgQt::GraphicsWindowQt(this);
    setGraphicsWindow(_graphics_window.get());
    _shapes -> getOrCreateStateSet() -> setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    _shapes -> getOrCreateStateSet() -> setMode(GL_BLEND, osg::StateAttribute::ON);
    _shapes -> getOrCreateStateSet() -> setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    _viewer -> setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
}
// void Viewer::paintEvent(QPaintEvent * paintEvent)
// {
//     // this->makeCurrent();
//     // QPainter painter(this);
//     // painter.setRenderHint(QPainter::Antialiasing);
//     // paintGL();
//     // painter.end();
//     // swapBuffers();
//     // doneCurrent();
//     _viewer -> frame();
// }

void
Viewer::updateGL()
{
     std::cerr << "Called updateGL" << std::endl;
     paintGL();
}

void
Viewer::paintGL()
{

    std::cerr << "Called paintGL" << std::endl;
    _viewer -> frame();
}

//void Viewer::resizeGL(int width, int height)
//{
//     _graphics_window -> resized(0, 0, width, height);
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     //for(osgViewer::View * view : views) { view -> resize(); }
// }

bool
Viewer::event(QEvent * event)
{
    bool handled = GLWidget::event(event);
    updateGL();
    return handled;
}

osgViewer::View *
Viewer::get_view_with_focus()
{
     return _viewer -> getViewWithFocus();
}


void
Viewer::attach_view(MeshView * mesh_view)
{
    mesh_view -> set_graphics_window(_graphics_window.get());
    mesh_view -> get_view() -> setSceneData(_shapes.get());
    _viewer -> addView(mesh_view -> get_view());
    mesh_view -> home();
}

void
Viewer::detach_view(MeshView * mesh_view)
{
  _viewer -> removeView(mesh_view -> get_view());
}

void
Viewer::attach_shape(Shape * shape)
{
    _shapes -> addChild(shape -> root.get());
}

void
Viewer::detach_shape(Shape * shape)
{
    _shapes -> removeChild(shape -> root.get());
}

