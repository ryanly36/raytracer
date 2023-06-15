#include "parse.h"
#include "texture.h"
#include "dump_png.h"
//collaborated with Justin Lyu 862142329
Texture::Texture(const Parse* parse,std::istream& in)
{
    std::string filename;
    in>>name>>filename>>use_bilinear_interpolation;
    Read_png(data,width,height,filename.c_str());
}

Texture::~Texture()
{
    delete [] data;
}

// uses nearest neighbor interpolation to determine color at texture coordinates
// (uv[0],uv[1]).  To match the reference image, details on how this mapping is
// done will matter.

// 1. Assume that width=5 and height=4.  Texture coordinates with 0<=u<0.2 and
//    0<=v<0.25 should map to pixel index i=0, j=0.  Texture coordinates with
//    0.8<=u<1 and 0.75<=v<1 should map to pixel index i=4, j=3.

// 2. Texture coordinates should "wrap around."  Some of the objects contain
//    (u,v) coordinates that are less than 0 or greater than 1.  u=0.4 and u=1.4
//    and u=-0.6 should be considered equivalent.  There is a wrap function in
//    misc.h that you may use.

// 3. Be very careful about indexing out of bounds.  For example, u=0.999999
//    should result in i=width-1, not i=width.  The latter is out of bounds.

// 4. Be careful with your rounding.  For example, u=-0.0001 should map to
//    i=width-1 in accordance to the wrapping rule.  Remember that casting from
//    float to integer rounds towards zero, so that (int)u would produce 0.  You
//    may find the cmath functions floor, ceil, and rint to be helpful, as they
//    provide precise control over rounding.

// 5. Although there is a flag called use_bilinear_interpolation, none of the
//    test cases exercise this feature.  You do not need to implement it, though
//    of course you are welcome to do so if you like.  You may assume nearest
//    neighbor interpolation.

vec3 Texture::Get_Color(const vec2& uv) const
{   
    // std::cout << "width: " << width << " height: " << height << std::endl;
    // std::cout << "uv[0]: " << uv[0] << " uv[1]: " << uv[1] << std::endl;


    // std::cout << "in hurr" << std::endl;
    double i = floor(uv[0]*width);
    double j = floor(uv[1]*height);

    // std::cout << "i: " << i << std::endl;
    // std::cout << "j: " << j << std::endl;

    // std::cout << "data[" << static_cast<int>(i*j) << "]: " << data[static_cast<int>(i*j)] << std::endl;

    vec3 color = From_Pixel(data[static_cast<int>(j*width + i)]);

    // std::cout << "color " << color << std::endl;
    return color;
    
//     TODO;
//     return {1,1,1};
}
