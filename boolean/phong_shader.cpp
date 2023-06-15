#include "light.h"
#include "parse.h"
#include "object.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
//collaborated with Justin Lyu 862142329
Phong_Shader::Phong_Shader(const Parse* parse,std::istream& in)
{
    in>>name;
    color_ambient=parse->Get_Color(in);
    color_diffuse=parse->Get_Color(in);
    color_specular=parse->Get_Color(in);
    in>>specular_power;
}

//phong = ambient + diffuse + specular
//L = K_a*I_a + K_dImax(0,n*1)+ k_sI *max(0, n*h)^n
vec3 Phong_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    //std::cout << "color: " << std::endl;

    vec3 color;
    vec3 ambient, diffuse, specular;
     if(render_world.ambient_color != nullptr && render_world.ambient_intensity != 0) {
        ambient = render_world.ambient_color->Get_Color({hit.uv}) * render_world.ambient_intensity * color_ambient->Get_Color({hit.uv});
        // std::cout << "world.ambient_color: " << render_world.ambient_color->Get_Color({hit.uv}) << std::endl;
    }
    else {
    //    std::cout << "world._ambient == nullptr" << std::endl;
        //ambient = color_ambient->Get_Color({});
        ambient = {0,0,0};
    }
    color = ambient;
    //std::cout << "ambient: " << ambient << std::endl;
    //  return color;

    //std::cout << "color_ambient: " << color_ambient->Get_Color({}) << std::endl;
     //std::cout << "color: " << color << std::endl;


    //reflection eq r = d -2(d*n)*n
    //std::cout << "world.objects.size()" << render_world.all_objects.size() << std::endl;
    for(unsigned i = 0; i < render_world.lights.size(); i++) {
        vec3 d = (render_world.lights.at(i)->position - intersection_point); //light_ray_direction
        //std::cout << "d: " << d << std::endl;
        //std::cout << "d magnitude: " << d.magnitude() << std::endl;
      //  vec3 d = (intersection_point - render_world.lights.at(i)->position).normalized();
        vec3 r = -d + 2.0 * dot(d,normal) * normal; //reflection ray
        r = r.normalized();

        if(render_world.enable_shadows) {
            Ray shadow_legends(intersection_point, d);
            std::pair<Shaded_Object,Hit> temp = render_world.Closest_Intersection(shadow_legends);
            Hit shadow_hit = temp.second;
            if(!shadow_hit.Valid() || shadow_hit.dist > d.magnitude()) {
                diffuse = color_diffuse->Get_Color({hit.uv}) * render_world.lights.at(i)->Emitted_Light(d);
                diffuse = diffuse * std::max(dot(normal, d.normalized()), 0.0);
                color = color + diffuse;
            
                specular = color_specular->Get_Color({hit.uv}) * render_world.lights.at(i)->Emitted_Light(d);
                specular = specular * pow(std::max(dot(-ray.direction, r.normalized()), 0.0), specular_power);
                color = color + specular;
            }

        }
        else {

            diffuse = color_diffuse->Get_Color({hit.uv}) * render_world.lights.at(i)->Emitted_Light(d);
            //std::cout << "color diffuse: " << color_diffuse->Get_Color({}) << std::endl;
            //std::cout << "diffuse0: " << diffuse << std::endl;

            diffuse = diffuse * std::max(dot(normal, d.normalized()), 0.0);
            //std::cout << "diffuse1: " << diffuse << std::endl;

            color = color + diffuse;

            specular = color_specular->Get_Color({hit.uv}) * render_world.lights.at(i)->Emitted_Light(d);
            specular = specular * pow(std::max(dot(-ray.direction, r.normalized()), 0.0), specular_power);
            color = color + specular;
        }
    }      
    // std::cout << "Ray: (end: " << ray.endpoint << "); dir: " << ray.direction << ")" << std::endl;
    // std::cout << "intersect test with Ps; hit: (" << hit.dist << std::endl;
    // std::cout << "shade_surface location: " << intersection_point << ", normal: " << normal << std::endl;
    //  std::cout << "diffuse: " << diffuse << std::endl;
    // std::cout << "specular: " << specular << std::endl;
    // std::cout << "color: " << color << std::endl;

    
    //color = color_ambient->Get_Color({}) + color_diffuse->Get_Color({}) + color_specular->Get_Color({});
    //TODO; //determine the color
    return color;
}
