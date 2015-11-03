class Axis : Shape
{
public:

    Axis(const std::string id);

    Axis( const std::string id
        , const osg::Vec3f & direction
        , float head_length
        , float head_radius
        , const osg::Vec4f & head_color
        , float tail_length
        , float tail_radius
        , const osg::Vec4f & tail_color
        );

    void
    set( const osg::Vec3f & direction
       , float head_length
       , float head_radius
       , const osg::Vec4f & head_color
       , float tail_length
       , float tail_radius
       , const osg::Vec4f & tail_color
       );

    void
    set_tail(float tail_length, float tail_radius, const osg::Vec4f & tail_color);

    void
    set_head(float head_length, float head_radius, const osg::Vec4f & head_color);

    void
    set_direction();

    const osg::Vec3f &
    get_direction();

    void
    set_head_radius(float head_radius);

    float
    get_head_radius() const;

    void
    set_head_length(float head_length);

    float
    get_head_length() const;

    void
    set_head_color(const osg::Vec4f & head_color);

    const osg::Vec4f &
    get_head_color() const;

    void
    set_tail_radius(float tail_radius);

    float
    get_tail_radius() const;

    void
    set_tail_length(float tail_length);

    float
    get_tail_length() const;

    void
    set_tail_color(const osg::Vec4f & tail_color);

    const osg::Vec4f &
    get_tail_color() const;

private:
    const osg::Vec3f _direction;
    float _head_radius;
    float _head_length;
    osg::Vec4f _head_color;
    float _tail_radius;
    float _tail_length;
    osg::Vec4f _tail_color;
};




class Axis
{
public:

Axis::Axis(const std::string id) : _direction(osg::X_AXIS)
                                 , _direction(12.0f)
                                 , _radius(2.0f)
                                 , _color(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f)
{ }

Axis::Axis( const std::string id
           , float length
           , float radius
           , const osg::Vec4f & color
        );

void
Axis::set_direction(const osg::Vec3f & direction)
{
    _direction = direction;
}

const osg::Vec3f &
Axis::get_direction() const
{
    return _direction;
}

void
Axis::set_tail_radius(float radius)
{
    _radius = radius;
}

float
Axis::get_tail_radius() const
{
    return _radius;
}

void
Axis::set_length(float length)
{
    _length = length;
}

float
Axis::get_length() const
{
    return _length;
}

void
Axis::set_color(const osg::Vec4f & color)
{
    _color = color;
}

const osg::Vec4f &
Axis::get_color()
{
    return _color;
}


class CoordinateSystem
{
public:

    void
    set_a_axis(const osg::Vec3f & direction, float length, float radius, const osg::Vec4f & color);

    void
    set_b_axis(const osg::Vec3f & direction, float length, float radius, const osg::Vec4f & color);

    void
    set_c_axis(const osg::Vec3f & direction, float length, float radius, const osg::Vec4f & color);

    void
    set_a_axis_direction(const osg::Vec3f & a_axis);

    void
    set_b_axis_direction(const osg::Vec3f & b_axis);

    void
    set_c_axis_direction(const osg::Vec3f & c_axis);

    void
    set_a_axis_length(float length);

    void
    set_a_axis_radius(float radius);

    void
    set_b_axis_length(float length);

    void
    set_b_axis_radius(float radius);

    void
    set_c_axis_length(float length);

    void
    set_c_axis_radius(float radius);

    void
    set_a_axis_color(const osg::Vec4f & c_color);

    const osg::Vec4f &
    get_a_axis_color();

    void
    set_b_axis_color(const osg::Vec4f & b_color);

    const osg::Vec4f &
    get_b_axis_color();

    void
    set_c_axis_color(const osg::Vec4f & c_color);

    const osg::Vec4f &
    get_c_axis_color();

private:

    osg::Vec3f _a_axis;
    osg::Vec3f _b_axis;
    osg::Vec3f _c_axis;
    osg::Vec4f _a_color;
    osg::Vec4f _b_color;
    osg::Vec4f _c_color;
    float _a_length;
    float _a_radius;
    float _b_length;
    float _b_radius;
    float _c_length;
    float _c_radius;
};
