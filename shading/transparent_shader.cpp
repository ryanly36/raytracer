#include "transparent_shader.h"
#include "parse.h"
#include "ray.h"
#include "render_world.h"

//collaberated with Justin Lyu 862142329
Transparent_Shader::
Transparent_Shader(const Parse* parse,std::istream& in)
{
    in>>name>>index_of_refraction>>opacity;
    shader=parse->Get_Shader(in);
    assert(index_of_refraction>=1.0);
}

// Use opacity to determine the contribution of this->shader and the Schlick
// approximation to compute the reflectivity.  This routine shades transparent
// objects such as glass.  Note that the incoming and outgoing indices of
// refraction depend on whether the ray is entering the object or leaving it.
// You may assume that the object is surrounded by air with index of refraction
// 1.
//opacity, index of refraction
//cos theta -dot(d,n)
vec3 Transparent_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    //TODO;
    //std::cout << "opacity: " << opacity << std::endl;
    //std::cout << "index of refract: " << index_of_refraction << std::endl;
    vec3 color;
    vec3 refl_color;
    vec3 trans_color;
    vec3 combined_color;

    //std::cout << "Ray: (end: " << ray.endpoint << "); dir: " << ray.direction << ")" << std::endl;
    color = shader->Shade_Surface(render_world, ray, hit,intersection_point, normal, recursion_depth); //setting color from our base shader (phong)
    //std::cout << "obbject color: " << color << std::endl;
    double schlick = 0;
    double R_0 = 0;
    //needs to be conidtional on if entering or leaving
    R_0 = pow(((index_of_refraction-1)/(index_of_refraction+1)),2);
    schlick = R_0 + (1 - R_0)*pow((1 + dot(ray.direction, normal)),5);

    //c = - dot(ray.direction, normal)
    //std::cout << "schlick reflectivity: " << schlick << std::endl;



    if(recursion_depth < render_world.recursion_depth_limit){ // recursion condition
        //r = d - 2(d dot n)n
        vec3 r = ray.direction - 2 * dot(ray.direction,normal) * normal;

        Ray reflection_ray(intersection_point + .0001 * r, r); // add epsilon to avoid self reflection
        //std::cout << "reflection ray: " << reflection_ray << " refl_color: " << refl_color << std::endl;

        refl_color = render_world.Cast_Ray(reflection_ray, recursion_depth+1);
        //return refl_color;

        //transmission
        double check_CIR = 1-((1-pow(dot(ray.direction, normal),2))/pow(index_of_refraction,2));
        if(check_CIR > 0) {
            vec3 tR = ((ray.direction - normal * dot(ray.direction, normal)))/index_of_refraction 
                        - normal * sqrt(1-((1-pow(dot(ray.direction, normal),2))/pow(index_of_refraction,2)));
            Ray transmission_ray(intersection_point , tR);
            //std::cout << "transmitted ray : " << transmission_ray << "trans_color: " << trans_color << std::endl;
            trans_color = render_world.Cast_Ray(transmission_ray, recursion_depth+1);
        //    return trans_color;
            //std::cout << 
            //return trans_color;
        }
        
    }
   combined_color = opacity*color + (1 - opacity)*(schlick*refl_color + (1-schlick)*trans_color);    //std::cout << "combined color: " << combined_color << std::endl;
    //  vec3 final_color = (opacity)*color + (1-opacity)*combined_color;
    // return final_color;
    return combined_color;
}
