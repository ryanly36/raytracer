#include "acceleration.h"
#include "object.h"
#include "hit.h"
//collaberated with Justin Lyu 862142329
extern int acceleration_grid_size;

Acceleration::Acceleration()
{
    domain.Make_Empty();
    num_cells.fill(acceleration_grid_size);
}

void Acceleration::Add_Object(const Object* obj, int id)
{
    std::pair<Box, bool> check = obj->Bounding_Box(id);
    std::cout << "obj->Bounding_Box(" << id << ") = box: lo " << check.first.lo << " hi " << check.first.hi << std::endl;

    Primitive temp;

    temp.obj = obj;
    //temp.part = 
    temp.id = id;
    if(check.second) {
        finite_objects.push_back(temp);
    }
    else {
        infinite_objects.push_back(temp);
    }
    std::cout << "finite_objects size: " << finite_objects.size() << std::endl;
    std::cout << "infinite_objects size: " << infinite_objects.size() << std::endl;

    for(unsigned i = 0; i < finite_objects.size(); i++) {
        std::cout << "finite objects[" << i <<"] = " << finite_objects.at(i).obj->name << std::endl;
    }
    for(unsigned i = 0; i < infinite_objects.size(); i++) {
        std::cout << "infinite objects[" << i <<"] = " << infinite_objects.at(i).obj->name << std::endl;
    }
    //std::cout << "THIS IS CALLED SOMEWHERE" <<std::endl;
    TODO;
}

ivec3 Acceleration::Cell_Index(const vec3& pt) const
{
    TODO;
    return {};
}

void Acceleration::Initialize()
{


    //bounding box of all finite objects

    for(unsigned i = 0; i < finite_objects.size(); i++) {
        domain.Union(finite_objects.at(i).obj->Bounding_Box(0).first);
    }
    infinite_objects.clear();
    TODO;
}

std::pair<int,Hit> Acceleration::Closest_Intersection(const Ray& ray) const
{
    TODO;
    //return {};
}
