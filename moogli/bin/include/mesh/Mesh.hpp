#include "utility/libraries.hpp"

class Mesh : public osg::Geometry
{
public:

    Mesh(osg::Drawable::UpdateCallback * callback);

    virtual void
    allocate() = 0;

    virtual void
    construct() = 0;

    virtual void
    color() = 0;

    bool
    allocation_required() const;

    bool
    construction_required() const;

    bool
    coloring_required() const;

protected:

    bool _allocation_required;
    bool _construction_required;
    bool _coloring_required;
};

