#include "nodes/RelativePositionNode.hpp"

RelativePositionNode::RelativePositionNode(): PositionAttitudeTransform()
                                            , _position(osg::Vec3f())
{
    setReferenceFrame(osg::Transform::ABSOLUTE_RF);
}

RelativePositionNode::RelativePositionNode( const RelativePositionNode & pat
                                          , const CopyOp & copyop
                                          ): PositionAttitudeTransform(pat, copyop)
{ }


void
RelativePositionNode::accept(NodeVisitor& nv)
{
    if (nv.validNodeMask(*this))
    {
        // if app traversal update the frame count.
        if (nv.getVisitorType()==NodeVisitor::CULL_VISITOR)
        {
            osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(&nv);
            cv -> setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
            if(cv)
            {
                float z = z_near + (z_far - z_near) * _position.z();
                float height = 2 * z * tan(osg::PI * fovy / 360.0f);
                float width  = height * aspect_ratio;
                osg::Camera * camera = cv -> getCurrentCamera();
                osg::Vec3d eye, center, up;
                camera -> getViewMatrixAsLookAt(eye, center, up);
                osg::Vec3f direction = center - eye;
                direction.normalize();
                osg::Vec3f side = direction ^ up;
                side.normalize();
                osg::Vec3f position = eye
                                    + side * width  * (_position.x() - 0.5)
                                    + up   * height * (_position.y() - 0.5)
                                    + direction * z;

                setPosition(position);
            }
        }
        PositionAttitudeTransform::accept(nv);
    }
}

void
RelativePositionNode::set_position(const osg::Vec3f & position);
{
    _position = position;
}

const osg::Vec3f &
RelativePositionNode::get_position() const
{
    return _position;
}

