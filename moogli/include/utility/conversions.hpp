#ifndef __CONVERSIONS_HPP__
#define __CONVERSIONS_HPP__

#include "Python.h"
#include "osg/Vec3f"
#include "osg/Vec4f"
#include "osg/Vec3d"
#include "osg/Vec4d"

osg::Vec4d
pysequence_to_vec4d(PyObject * sequence);

osg::Vec4f
pysequence_to_vec4f(PyObject * sequence);

osg::Vec3d
pysequence_to_vec3d(PyObject * sequence);

osg::Vec3f
pysequence_to_vec3f(PyObject * sequence);

#endif
