#ifndef __COMPOSITE_VIEWER_HPP__
#define __COMPOSITE_VIEWER_HPP__

#include "utility/libraries.hpp"

class CompositeViewer: public QWidget, public osgViewer::CompositeViewer
{
public:
    CompositeViewer(QWidget * parent = 0, Qt::WindowFlags f = 0);
protected:
    virtual void
    paintEvent(QPaintEvent * event);
};


#endif /* __COMPOSITE_VIEWER_HPP__ */
