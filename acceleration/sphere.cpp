#include "sphere.h"
#include "ray.h"
//collaberated with Justin Lyu 862142329

Sphere::Sphere(const Parse* parse, std::istream& in)
{
    in>>name>>center>>radius;
}

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    double t_0 = 0;
    double t_1 = 0;
    double discrim = pow(dot(ray.direction, ray.endpoint - center), 2)
                     - (dot(ray.direction, ray.direction) 
                     * (dot(ray.endpoint - center, ray.endpoint - center) 
                     - pow(radius, 2)));

    // TODO;
    if(discrim > 0){ //checking if there is any real solution from discriminant
        t_0 = -dot(ray.direction, ray.endpoint - center) + sqrt(discrim)
                 / dot(ray.direction, ray.direction);
        t_1 = -dot(ray.direction, ray.endpoint - center) - sqrt(discrim)
                 / dot(ray.direction, ray.direction);
        if(t_0 < t_1 && t_0 >= small_t) { //return closest intersection that is greater than small_t
            return {t_0, part, {0,0}};
        }
        else if(t_1 < t_0 && t_1 >= small_t) {
            return {t_1, part, {0,0}};
        }
                
    } 
    return {-1,part, {0,0}}; //return no intersection
}

vec3 Sphere::Normal(const Ray& ray, const Hit& hit) const
{
    vec3 normal;
    //TODO; // compute the normal direction

    vec3 point = ray.Point(hit.dist);
    normal = (point - center) / radius;
    return normal.normalized();
}

std::pair<Box,bool> Sphere::Bounding_Box(int part) const
{
    return {{center-radius,center+radius},false};
}
