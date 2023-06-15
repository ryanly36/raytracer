#include "render_world.h"
#include "flat_shader.h"
// #include "phong_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"
//collaberated with Justin Lyu 862142329

extern bool enable_acceleration;

Render_World::~Render_World()
{
    for(auto a:all_objects) delete a;
    for(auto a:all_shaders) delete a;
    for(auto a:all_colors) delete a;
    for(auto a:lights) delete a;
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
std::pair<Shaded_Object,Hit> Render_World::Closest_Intersection(const Ray& ray) const
{
    // if(enable_acceleration) {
    //     std::pair<int, Hit> accPair = acceleration.Closest_Intersection(ray);
    // }
    //std::cout << "exited cls intr acc " << std::endl;
    double min_t = std::numeric_limits<double>::max();


    Hit hit;
    //Hit it;
    Shaded_Object closest_obj;
    for(unsigned i = 0; i < all_objects.size();i++) {

        Hit it = objects.at(i).object->Intersection(ray, -1);

        if(it.dist < min_t && it.dist >= small_t && it.Valid()) { //found new closest hit and is greater than small_t
            hit = it;
            min_t = it.dist;
            closest_obj = objects.at(i);
        }

    }

    //std::cout << "closest intersection " << "hit: " << hit << std::endl;
    std::pair<Shaded_Object, Hit> result (closest_obj,hit);
    return result;
    //return {closest_obj, hit};
    // TODO;
    // return {};
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    //TODO; // set up the initial view ray here
    Ray ray;
    ray.endpoint = camera.position; //start ray tracing
    ray.direction = (camera.World_Position(pixel_index) - ray.endpoint).normalized(); //set direction from selected pixel - camera position
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    // if(enable_acceleration) {
    //     for(unsigned i = 0; i < all_objects.size();i++) {
    //         acceleration.Add_Object(all_objects.at(i), i);
    //     }
    // }
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++) {
            //if(i == 350 && j == 250 ){
             // std::cout << "x: " << i << " y: " << j << std::endl; 
                Render_Pixel(ivec2(i,j));
            //}
            
        }
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth) const
{
    //std::cout << "cast ray: " << ray << std::endl;
    vec3 color;
    vec3 intersect;
    vec3 normal;
    Hit hit;
    Shaded_Object obj_hit;

    std::pair<Shaded_Object, Hit> pair = Closest_Intersection(ray);
    
    obj_hit = pair.first;
    hit = pair.second;
    
    if(hit.Valid()) {
        // std::cout << "here hit valid" << std::endl;
        intersect = ray.Point(hit.dist);
        normal = obj_hit.object->Normal(ray, hit);

        //std::cout << "shade surface with location " << intersect << " noraml: " << normal << std::endl;
        color = obj_hit.shader->Shade_Surface(*this, ray, hit, intersect, normal, recursion_depth);
    }
    else if(background_shader != nullptr) {
        //std::cout << "here" << std::endl;
        intersect = ray.Point(hit.dist);
        //std::cout << "shade surface with location " << intersect << " noraml: " << normal << std::endl;

        color = background_shader->Shade_Surface(*this, ray, hit, intersect, normal, recursion_depth);

    } 
    else {
        color = {0,0,0}; //return black
    }
    //TODO; // determine the color here
    return color;
}
