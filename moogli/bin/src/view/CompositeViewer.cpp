#include "view/CompositeViewer.hpp"

CompositeViewer::CompositeViewer( QWidget * parent
                                , Qt::WindowFlags f
                                ): QWidget(parent, f)
                                 , osgViewer::CompositeViewer()
{
    setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
    // disable the default setting of viewer.done() by pressing Escape.
    setKeyEventSetsDone(0);
    setMouseTracking(true);
}

void
CompositeViewer::paintEvent( QPaintEvent* event )
{
    frame();
}





