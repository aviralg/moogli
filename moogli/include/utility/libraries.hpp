#ifndef __LIBRARIES_HPP__
#define __LIBRARIES_HPP__


#undef ANY
/******************************************************************************/
/* C++ STANDARD LIBRARY HEADERS                                               */
/******************************************************************************/
#include <cmath>
#include <cfloat>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <utility>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include <algorithm>

/******************************************************************************/
/* OSG HEADERS                                                                */
/******************************************************************************/
#include <osg/Shape>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Vec3d>
#include <osg/Vec4d>
#include <osg/ref_ptr>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osg/DisplaySettings>
#include <osg/ShapeDrawable>
#include <osg/StateSet>

#include <osgGA/EventQueue>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/PolytopeIntersector>

#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>

#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/CompositeViewer>
#include <osgViewer/GraphicsWindow>

#include <osgQt/GraphicsWindowQt>

#include <osgGA/TrackballManipulator>

#include <osg/ShadeModel>
#include <osg/Material>
#include <osg/LightSource>
#include <osg/Light>
#include <osg/StateSet>
#include <osg/Depth>
#include <osgFX/Outline>

/******************************************************************************/
/* QT HEADERS                                                                 */
/******************************************************************************/
#include <QTimer>
#include <QtOpenGL>
#include <QApplication>
#include <QGridLayout>
#include <QScrollArea>
#include <QMenu>
#include <QCursor>
#include <QDrag>
#include <QToolBar>
#include <QPushButton>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>

/******************************************************************************/
/* UTILITY HEADERS                                                            */
/******************************************************************************/
#include "utility/record.hpp"
#include "utility/conversions.hpp"
#include "utility/globals.hpp"

#include "Python.h"
#define ANY void
#endif  /* __LIBRARIES_HPP__ */



