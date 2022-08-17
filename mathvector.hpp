

struct mathVector
{
    double x_ = 0.0;
    double y_ = 0.0;
    double z_ = 0.0;

    mathVector(double x, double y, double z) : x_(x), y_(y), z_(z) { };

};


mathVector operator+(mathVector rhs, mathVector lhs)
{
    return mathVector(rhs.x_ + lhs.x_, rhs.y_ + lhs.y_, rhs.z_ + lhs.z_);
}

double operator*(mathVector rhs, mathVector lhs)
{
    return (rhs.x_ * lhs.x_) + (rhs.y_ * lhs.y_) + (rhs.z_ * lhs.z_);
}

mathVector operator*(double mult, mathVector vec)
{
    vec.x_ *= mult;
    vec.y_ *= mult;
    vec.z_ *= mult;

    return vec;
}

mathVector operator*(mathVector vec, double mult)
{
    vec.x_ *= mult;
    vec.y_ *= mult;
    vec.z_ *= mult;

    return vec;
}