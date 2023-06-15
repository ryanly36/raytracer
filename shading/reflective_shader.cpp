#include "reflective_shader.h"
#include "parse.h"
#include "ray.h"
#include "render_world.h"
//collaberated with Justin Lyu 862142329

Reflective_Shader::Reflective_Shader(const Parse* parse,std::istream& in)
{
    in>>name;
    shader=parse->Get_Shader(in);
    in>>reflectivity;
    reflectivity=std::max(0.0,std::min(1.0,reflectivity));
}

vec3 Reflective_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
//TODO; // determine the color
    vec3 color;
    color = shader->Shade_Surface(render_world, ray, hit,intersection_point, normal, recursion_depth); //setting color from our base shader (phong)
    //    std::cout << "Ray: (end: " << ray.endpoint << "); dir: " << ray.direction << ")" << std::endl;
    //    std::cout << "intersect test with Ps; hit: (" << hit.dist << std::endl;

    //    std::cout << "shade_surface location: " << intersection_point << ", normal: " << normal << std::endl;
      //  std::cout << "reflected_color: " << reflected_color << std::endl;
    //    std::cout << "object color: " << color << std::endl << std::endl << std::endl << std::endl;
       // std::cout << "final color: " << 
    //std::cout << "checking if" << recursion_depth << " <= " << render_world.recursion_depth_limit << std::endl;
    vec3 base_r_shader = (1-reflectivity) * color;
    if(recursion_depth < render_world.recursion_depth_limit){ // recursion condition
        //r = d - 2(d dot n)n
        vec3 r = ray.direction - 2 * dot(ray.direction,normal) * normal;
        Ray reflection_ray(intersection_point + .0001 * r, r); // add epsilon to avoid self reflection
    //std::cout << "reflection_ray dir: " << reflection_ray << std::endl;

        color = base_r_shader + reflectivity * render_world.Cast_Ray(reflection_ray, recursion_depth + 1);
    }
    else{ //base condition 
        color = base_r_shader; //apply base reflective shader
    } 

    //std::cout << " final color: " << color <<std::endl;
    return color;
}
