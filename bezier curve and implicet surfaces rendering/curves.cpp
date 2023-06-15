#include "common.h"

//collaberated with Justin Lyu 862142329

// double pow(double a, int count) { //not needed anymore
//   double result = 1.0;
//   while (count > 0) {
//     if (count % 2 == 1) {
//       result = a;
//     }
//     a= a;
//     count /= 2;
//   }
//   return result;
// }
// Evaluate curve c at t and return the result.  If sub_a or sub_b are not null,
// perform subdivision on curve c at t as well.  If sub_a is not null, assign it
// the first piece (so that sub_a and c agree at their endpoint A.)  If sub_b is
// not null, assign it the second piece (so that sub_b and c agree at their
// endpoint D.)
vec2 Evaluate(const Curve& c, double t, Curve* sub_a, Curve* sub_b)
{
    //using de casteljau's algorithm
    vec2 ab = (1-t)*c.A + t*c.B;
    vec2 bc = (1-t)*c.B + t*c.C;
    vec2 cd = (1-t)*c.C + t*c.D;
    vec2 abc = (1-t)*ab + t*bc;
    vec2 bcd = (1-t)*bc + t*cd;
    vec2 final = (1-t)*abc + t*bcd;

    if (sub_a != NULL || sub_b != NULL) {
        if(sub_a != NULL) {
            sub_a->A = c.A;
            sub_a->B = ab;
            sub_a->C = abc;
            sub_a->D = final;
        }
        if(sub_b != NULL) {
            sub_b->A = final;
            sub_b->B = bcd;
            sub_b->C = cd;
            sub_b->D = c.D;
        }
    }

    return final;
}
