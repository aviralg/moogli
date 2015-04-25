#ifndef _NETWORK_VIEWER_HPP_
#define _NETWORK_VIEWER_HPP_

#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Network.hpp"
#include "handlers/GeometrySelector.hpp"

using namespace std;

class NetworkViewer : public QGLWidget
{
  Q_OBJECT

public:

    double up_distance;
    double down_distance;
    double left_distance;
    double right_distance;
    double forward_distance;
    double backward_distance;
    double zoom_factor;
    double roll_angle;
    double pitch_angle;
    double yaw_angle;

    Network *
    get_network();

    NetworkViewer( Network * network
                          , QWidget * parent             = 0
                          , const QGLWidget* shareWidget = 0
                          , Qt::WindowFlags f            = 0
                          );
    void
    add_view( int x
            , int y
            , int width
            , int height
            );

    void
    split_horizontally( unsigned int view_index   = 0
                      , unsigned int width_factor = 2
                      );

    void
    split_vertically( unsigned int view_index    = 0
                    , unsigned int height_factor = 2
                    );

    void
    home(unsigned int index = 0);

    void
    forward( double distance
           , unsigned int index = 0
           );
    void
    backward( double distance
            , unsigned int index = 0
            );
    void
    left( double distance
        , unsigned int index = 0
        );
    void
    right( double distance
         , unsigned int index = 0
         );

    void
    up( double distance
      , unsigned int index = 0
      );

    void
    down( double distance
        , unsigned int index = 0
        );

    void
    zoom( double factor
        , unsigned int index = 0
        );

    void
    roll( double angle
        , unsigned int index = 0
        );

    void
    pitch( double angle
         , unsigned int index = 0
         );

    void
    yaw( double angle
       , unsigned int index = 0
       );

    // void
    // capture_continuous_toggle(unsigned int index);

    // void
    // capture_once(unsigned int index);

    virtual
    ~NetworkViewer();

signals:
    void selected(const char * id);


protected:

    virtual void paintEvent( QPaintEvent* paintEvent );
    virtual void paintGL();
    virtual void resizeGL( int width, int height );

    virtual void keyPressEvent( QKeyEvent* event );
    virtual void keyReleaseEvent( QKeyEvent* event );

    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void wheelEvent( QWheelEvent* event );

    virtual bool event( QEvent* event );

private:
    void
    _get_transformation( unsigned int index
                       ,  osg::Vec3d  & eye
                       , osg::Vec3d  & center
                       , double      & distance
                       , osg::Vec3d  & up
                       , osg::Vec3d  & look
                       , osg::Vec3d  & side
                       );

    void
    _set_transformation( unsigned int index
                       , const osg::Vec3d & eye
                       , const osg::Vec3d & center
                       , const osg::Vec3d & up
                       );

    unsigned int
    _get_view_index_with_focus();

    virtual void
    onHome();

    virtual void
    onResize(int width, int height);

    osgViewer::View *
    createView();

    osg::Camera *
    createCamera();

    osgGA::EventQueue*
    getEventQueue() const;

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _graphics_window;
    osg::ref_ptr<osgViewer::CompositeViewer> _viewer;

    int _previous_width;
    int _previous_height;

    string capture_location;
    string capture_format;

    Network * network;

};

#endif /* _NETWORK_VIEWER_HPP_ */
