#include "utility/libraries.hpp"
#include "meshes/Mesh.hpp"

class MeshUpdateCallback : public osg::Drawable::UpdateCallback
{
public:
    MeshUpdateCallback();

    virtual void
    update( osg::NodeVisitor * nv
          , osg::Drawable * drawable
          );
};

