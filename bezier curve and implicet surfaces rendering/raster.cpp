#include <vector>
#include <string>
#include <iostream>
#include "common.h"
#include <algorithm>
#include <cmath>
//collaberated with Justin Lyu 862142329
inline vec3 to_vec3(const vec4& p)
{
    return vec3(p[0],p[1],p[2])/p[3];
}

void printTriangles(const std::vector<Triangle> & tris) {
    for(unsigned i = 0; i < tris.size(); i++) {
        std::cout << "tris(" << i << "): A:" << tris.at(i).A << " B: " << tris.at(i).B  << " C: " << tris.at(i).C << std::endl;
        std::cout << "color(" << i << "): A:" << tris.at(i).Ca << " B: " << tris.at(i).Cb  << " C: " << tris.at(i).Cc << std::endl;
        std::cout << "------" << std::endl;
    }
}

void calculateBarycentricWeights(const vec3& A, const vec3& B, const vec3& C, const vec2& pt, double& alpha, double& beta, double& gamma)
{
    double tot_area = (0.5 * ((B[0]*C[1] - C[0]*B[1]) - (A[0]*C[1] - C[0]*A[1]) + (A[0]*B[1] - B[0]*A[1])));
    alpha = (0.5 * ((B[0] * C[1] - C[0] * B[1]) + (B[1] - C[1])*pt[0] + (C[0] - B[0])*pt[1])) / tot_area;
    beta =  (0.5 * ((C[0] * A[1] - A[0] * C[1]) + (C[1] - A[1])*pt[0] + (A[0] - C[0])*pt[1])) / tot_area;
    gamma = (0.5 * ((A[0] * B[1] - B[0] * A[1]) + (A[1] - B[1])*pt[0] + (B[0] - A[0])*pt[1])) / tot_area;
}

    //w = 640, h = 480

void Rasterize(Pixel* pixels, int width, int height, const std::vector<Triangle>& tris)
{
    //std::cout<<"TODO: implement rasterization"<<std::endl;
    std::vector<double> z_buffer(width * height, std::numeric_limits<double>::infinity());

    double alpha, beta, gamma = 0;
    vec3 A, B, C;
    vec3 color;

    //printTriangles(tris);

    for(unsigned i = 0; i < tris.size(); i++) {
        Triangle currTri = tris.at(i);

        A = to_vec3(currTri.A);
        B = to_vec3(currTri.B);
        C = to_vec3(currTri.C);

        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                vec2 pt = {static_cast<double>(x), static_cast<double>(y)};
                //convert pixel index to normalized device coordinates
                pt[0] = 2.0*pt[0]/width - 1;
                pt[1] = 2.0*pt[1]/height - 1;


                calculateBarycentricWeights(A, B, C , pt, alpha, beta, gamma);

                if((alpha >= 0) && (beta >= 0) && (gamma >= 0)) {
                    //std::cout << "alpha: " << alpha << " beta: " << beta << " gamma: " << gamma << std::endl;

                    double depth = (alpha * A[2]) + (beta * B[2]) + (gamma * C[2]);
                    if (depth < z_buffer[x + width * y])
                    {
                        //update z-buffer
                        z_buffer.at(x+width*y) = depth; 
                        
                        //perspective correct interpolation
                        double sum = alpha / currTri.A[3] + beta / currTri.B[3] + gamma / currTri.C[3];
                        double pca = alpha / currTri.A[3];
                        double pcb = beta / currTri.B[3];
                        double pcc = gamma / currTri.C[3];
                        pca /= sum;
                        pcb /= sum;
                        pcc /= sum;

                        color = pca*currTri.Ca + pcb*currTri.Cb + pcc*currTri.Cc;
                        //std::cout << "color: " << color << std::endl;
                        set_pixel(pixels, width, height, x, y, color);

                    }
                }
            }
        }
    }




}
