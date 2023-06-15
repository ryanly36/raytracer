#include <limits>
#include "box.h"
//collaborated with Justin Lyu 862142329
// Return whether the ray intersects this box.
std::pair<bool,double> Box::Intersection(const Ray& ray) const
{
    TODO;
    vec3 invrRay;
    double t_xmax,t_xmin,t_ymax,t_ymin,t_zmax,t_zmin = 0;
    invrRay[0] = 1/ray.direction[0];
    invrRay[1] = 1/ray.direction[1];
    invrRay[2] = 1/ray.direction[2];

    for(unsigned i = 0; i < 3; i++) {
        if(invrRay[i] >= 0) {
            t_xmin = (lo[i] - ray.endpoint[0])*invrRay[i];
            t_ymin = (lo[i] - ray.endpoint[0])*invrRay[i];
            t_zmin = (lo[i] - ray.endpoint[0])*invrRay[i];
        }
        else {
            t_xmax = (hi[0] - ray.endpoint[0])*invrRay[i];
            t_ymax = (hi[i] - ray.endpoint[0])*invrRay[i];
            t_zmax = (hi[i] - ray.endpoint[0])*invrRay[i];
        }
    }

    if(t_xmin > t_ymax || (t_ymin > t_xmax) || (ray.endpoint[0] <= lo[0]) || (hi[0] <= ray.endpoint[0])) {
        return {false,-1};
    }



    //return {true, }
    return {true,0};
}

// Compute the smallest box that contains both *this and bb.
Box Box::Union(const Box& bb) const
{
    Box box;
    for(unsigned i = 0; i < 3; i++) {//take max and min of x y and z values
        box.lo[i] = std::min(hi[i], bb.hi[i]);
        box.hi[i] = std::max(hi[i], bb.hi[i]);
    }
    //TODO;
    return box;
}

// Compute the smallest box that contains both *this and bb.
Box Box::Intersection(const Box& bb) const
{
    Box box;
    for(unsigned i = 0; i < 3; i++) {
        if(lo[i] > bb.hi[i] && hi[i] < bb.lo[i]) {
            return box;
        }
        else if(lo[i] > bb.hi[i] && hi[i] >= bb.lo[i]) {
            box.hi[i] = std::min(hi[i], bb.hi[i]);
        }
        else if(lo[i] <= bb.hi[i] && hi[i] < bb.lo[i]) {
            box.lo[i] = std::max(lo[i], bb.lo[i]);

        }
        else {
            box.lo[i] = std::max(lo[i], bb.lo[i]);
            box.hi[i] = std::min(hi[i], bb.hi[i]);
        }

    }
    // TODO;
    return box;
}

// Enlarge this box (if necessary) so that pt also lies inside it.
void Box::Include_Point(const vec3& pt)
{
    for(unsigned i = 0 ;i < 3; i++) {
        if(pt[i] < lo[i]) {
            lo[i] = pt[i];
        }
        if(pt[i] > hi[i]) {
            hi[i] = pt[i];
        }
    }
    // TODO;
}

// Create a box to which points can be correctly added using Include_Point.
void Box::Make_Empty()
{
    lo.fill(std::numeric_limits<double>::infinity());
    hi=-lo;
}

// Create a box that contains everything.
void Box::Make_Full()
{
    hi.fill(std::numeric_limits<double>::infinity());
    lo=-hi;
}

bool Box::Test_Inside(const vec3& pt) const
{
    for(int i=0;i<3;i++)
        if(pt[i]<lo[i] || pt[i]>hi[i])
            return false;
    return true;
}

// Useful for debugging
std::ostream& operator<<(std::ostream& o, const Box& b)
{
    return o << "(lo: " << b.lo << "; hi: " << b.hi << ")";
}
