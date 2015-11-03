#ifndef __RELATIVE_POSITION_NODE_HPP__
#define __RELATIVE_POSITION_NODE_HPP__

#include "utility/libraries.hpp"

class OSG_EXPORT RelativePositionNode : public PositionAttitudeTransform
{
public :
    RelativePositionNode();

    RelativePositionNode( const CoordinateSystem & pat
                        , const CopyOp & copyop = CopyOp::SHALLOW_COPY
                        );
    virtual void accept(NodeVisitor& nv);

    void
    set_position(const osg::Vec3f & position);

    const osg::Vec3f &
    get_position() const;

private:
    osg::Vec3f _position;
};

#endif /* __RELATIVE_POSITION_NODE_HPP__ */
