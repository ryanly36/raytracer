#include "plane.h"
#include "hit.h"
#include "ray.h"
#include <cfloat>
#include <limits>
//collaberated with Justin Lyu 862142329
Plane::Plane(const Parse* parse,std::istream& in)
{
    in>>name>>x>>normal;
    normal=normal.normalized();
}

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
//plane data structure {vec3 point, vec3 normal}
//hit data structure {double dist, int triangle, vec2 uv}
//p(t) = e + t(s-e)
Hit Plane::Intersection(const Ray& ray, int part) const
{
    //check if dot product of ray is parrellel to plane
    //double num = 0;
    double denom = dot(ray.direction, normal);
    double t = 0;
    //std::cout << denom << std::endl;
    if(denom) {
        t = dot((x - ray.endpoint), normal) / denom;
        if(t > 0) {
            return {t, part,{0,0}};
        }
    }
    return {-1, part, {0,0}};
    //TODO;
    //return {-1,-1,0}; //default return
    //std::cout << result <<std::endl;
    // return result;
}

vec3 Plane::Normal(const Ray& ray, const Hit& hit) const
{
    return normal;
}

std::pair<Box,bool> Plane::Bounding_Box(int part) const
{
    Box b;
    b.Make_Full();
    return {b,true};
}
