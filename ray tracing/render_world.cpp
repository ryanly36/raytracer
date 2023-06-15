#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

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
    //TODO;
    double min_t = std::numeric_limits<double>::max();
    
    Hit closest_hit;
    Shaded_Object closest_obj;

    // std::cout << "ray dir: " << ray.direction << std::endl;
    // std::cout << "ray end: " << ray.endpoint << std::endl;

    // std::cout << "object size: " << objects.size() << std::endl;
   for(unsigned i = 0; i < objects.size(); ++i) {
 
	    Hit hit = objects.at(i).object->Intersection(ray, -1);
       // std::cout << "hit dist: " << hit.dist << std::endl;
    
  
        
	if(hit.dist >= 0 && hit.dist >= small_t && hit.dist < min_t) {
		closest_hit = hit;
      //  std::cout << "closest dist: " <<  closest_hit.dist << std::endl;
		closest_obj = objects.at(i);
		min_t = hit.dist;
	}
    
}
    // std::cout << "hit dist: " << closest_hit.dist << std::endl;
 
   std::pair<Shaded_Object, Hit> pair1 (closest_obj, closest_hit);
  // pair1.first = closest_obj;
  // pair1.second = closest_hit;
    return pair1;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
   // TODO; // set up the initial view ray here
    Ray ray;
    ray.endpoint = camera.position; //because p(0) = e and endpoint of the ray where t = 0
    ray.direction = (camera.World_Position(pixel_index) - ray.endpoint).normalized();
   // std::cout << "ray.endpoint: " << ray.endpoint[0] << ", " << ray.endpoint[1] << ", " << ray.endpoint[2] << std::endl;
  //  std::cout << "ray.direction: " << ray.direction[0] << ", " << ray.direction[1] << ", " << ray.direction[2] << std::endl;
  //  Debug_Scope scope; 
  //  Pixel_Print("calling foo; pt: ",ray.endpoint,"; ray: ",ray);
    
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++) {
            Render_Pixel(ivec2(i,j));

        }

    
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth) const
{

    vec3 color;
    vec3 intersect;
    vec3 normal;
    Hit closest_temp;
    //vec3 bckrndcolor = this->background_shader->Shade_Surface(Render_World(), ray, closest_temp, intersect, normal, 1);
    //TODO; // determine the color here
    //std::cout << "cast_Ra1y" << std::endl;
    std::pair<Shaded_Object, Hit> pair1 = Closest_Intersection(ray);


    Hit closest_hit = pair1.second; //now it basically called the closest_intersection
    Shaded_Object obj = pair1.first;

   // break;
    //std::cout << "cast_Ray" << std::endl;

    if(closest_hit.dist >= 0) {
        //std::cout << "Cast_ray35" << std::endl;
   
        vec3 intersection_point = ray.Point(closest_hit.dist);
        vec3 temp_normal = obj.object->Normal(ray, closest_hit);
        color = obj.shader->Shade_Surface(Render_World(), ray, closest_hit, intersection_point, temp_normal, recursion_depth);
    }
    else if (closest_hit.dist < 0) {
        //color = this->background_shader->Shade_Surface(Render_World(), ray, closest_temp, intersect, normal, 1);
        color = {0,0,0};
    }
     else {
         
        color = {0,0,0}; //black
     }

    


    return color;
}
