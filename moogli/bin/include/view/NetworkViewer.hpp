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
    set_background_color(const osg::Vec4f & color);

    void
    add_view( int x
            , int y
            , int width
            , int height
            );

    osg::Projection *
    create_heads_up_display( int x
                           , int y
                           , int width
                           , int height
                           , osg::Node * node
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


    bool
    create_color_bar( const char * id
                    , const char * title
                    , float min_scalar
                    , float max_scalar
                    , std::vector< osg::Vec4 > colors
                    , int num_colors
                    , int num_labels
                    , float angle
                    , float aspect_ratio
                    , float width
                    , osg::Vec3f position
                    , osg::Vec4 label_color
                    , const char * label_font_file
                    , std::pair<int, int> label_font_resolution
                    , float label_character_size
                    );

    osgText::Text *
    create_two_d_texts( const char * id
                      , const char * content
                      , const osg::Vec3 position
                      );

    osg::Camera*
    create_hud_camera( int x
                     , int y
                     , int width
                     , int height
                     );

    osg::Billboard *
    create_billboard();

    osg::Camera *
    get_hud_camera(unsigned int view_index);

    osg::Billboard *
    get_billboard(unsigned int view_index);

    bool
    hide_color_bar( const char * id
                  , unsigned int view_index
                  );

    bool
    show_color_bar( const char * id
                  , unsigned int view_index
                  );

    // void
    // export(const char * filename);

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

    osg::ref_ptr<osgViewer::View> _hud_view;
    std::unordered_map<std::string, osg::ref_ptr<osgViewer::View> > _views;
    std::unordered_map<std::string, osg::ref_ptr<osgViewer::View> > _hud_views;
    osg::ref_ptr<osg::MatrixTransform> _root;
    osg::ref_ptr<osg::Billboard> _billboard;
    std::unordered_map<std::string, osg::ref_ptr<osg::MatrixTransform> > _scalar_bars;
  std::unordered_map<std::string, osg::ref_ptr<osg::Geode> > _two_d_texts;
    std::unordered_map<std::string, osg::ref_ptr<osgText::Text3D> > _three_d_texts;
};

#endif /* _NETWORK_VIEWER_HPP_ */
