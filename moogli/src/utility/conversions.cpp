#include "utility/conversions.hpp"

osg::Vec4d
pysequence_to_vec4d(PyObject * sequence)
{
    return osg::Vec4d( PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 0))
                     , PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 1))
                     , PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 2))
                     , PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 3))
                     );
}


osg::Vec4f
pysequence_to_vec4f(PyObject * sequence)
{
    return osg::Vec4f( static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 0))
                                         )
                     , static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 1))
                                         )
                     , static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 2))
                                         )
                     , static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 3))
                                         )
                     );
}

osg::Vec3d
pysequence_to_vec3d(PyObject * sequence)
{
    return osg::Vec3d( PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 0))
                     , PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 1))
                     , PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 2))
                     );
}


osg::Vec3f
pysequence_to_vec3f(PyObject * sequence)
{
    return osg::Vec3f( static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 0))
                                         )
                     , static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 1))
                                         )
                     , static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 2))
                                         )
                     );
}
