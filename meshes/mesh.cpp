#include "mesh.h"
#include <fstream>
#include <limits>
#include <string>
#include <algorithm>
//collaberated with Justin Lyu 862142329
// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

Mesh::Mesh(const Parse* parse, std::istream& in)
{
    std::string file;
    in>>name>>file;
    Read_Obj(file.c_str());
}

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e, t;
    vec3 v;
    vec2 u;
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }

        if(sscanf(line.c_str(), "vt %lg %lg", &u[0], &u[1]) == 2)
        {
            uvs.push_back(u);
        }

        if(sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &e[0], &t[0], &e[1], &t[1], &e[2], &t[2]) == 6)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
            for(int i=0;i<3;i++) t[i]--;
            triangle_texture_index.push_back(t);
        }
    }
    num_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    //TODO;
    double min_t = std::numeric_limits<double>::max();
    Hit hit;
    if(part < 0) {
        for(unsigned i = 0; i < triangles.size(); i++) {
            Hit it = Intersect_Triangle(ray, i);
            if(it.dist < min_t && it.dist >= small_t && it.Valid()) {
                hit = it;
                min_t = it.dist;
            }
        }
    }
    else {
        hit = Intersect_Triangle(ray, part);
    }
    //std::cout << ray << std::endl;


    return hit;
    
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const Ray& ray, const Hit& hit) const
{
    assert(hit.triangle>=0);
    ivec3 curr_tri = triangles.at(hit.triangle);

    vec3 a = vertices.at(curr_tri[0]);
    vec3 b = vertices.at(curr_tri[1]);
    vec3 c = vertices.at(curr_tri[2]);

    vec3 ab = b - a;
    vec3 bc = c - a;


    return cross(ab, bc).normalized();
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
Hit Mesh::Intersect_Triangle(const Ray& ray, int tri) const
{
    double dist, alpha, beta, gamma = 0;

    ivec3 curr_tri = triangles.at(tri);
    vec3 a = vertices.at(curr_tri[0]);
    vec3 b = vertices.at(curr_tri[1]);
    vec3 c = vertices.at(curr_tri[2]);

    vec3 u = ray.direction;
    vec3 y = ray.endpoint - a;
    vec3 w = b - a;
    vec3 v = c - a;

    //distance
    //y + tu = beta*w + gamma*v
    vec3 m = cross(w, v);
    double denom = dot(u, m);
    if(denom) {
        dist = -dot(y, m)/denom;
    }

  

    //barycentric weights
    if(dist > small_t) {
        m = cross(u, v);
        beta = dot(y, m)/ dot(w, m);

        m = cross(u, w);
        gamma = dot(y, m) / dot(v, m);

        alpha = 1 - gamma - beta;
        // std::cout << "(" << alpha << ", " << beta << ", " << gamma << ")" << std::endl;

        if(alpha > -weight_tolerance && beta > -weight_tolerance && gamma > -weight_tolerance && dist > small_t) {

            return {dist, tri, {0,0}};
        }
    }

    return {-1, tri, {0,0}};
}

std::pair<Box,bool> Mesh::Bounding_Box(int part) const
{
    if(part<0)
    {
        Box box;
        box.Make_Empty();
        for(const auto& v:vertices)
            box.Include_Point(v);
        return {box,false};
    }

    ivec3 e=triangles[part];
    vec3 A=vertices[e[0]];
    Box b={A,A};
    b.Include_Point(vertices[e[1]]);
    b.Include_Point(vertices[e[2]]);
    return {b,false};
}
