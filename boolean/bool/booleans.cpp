#include "common.h"
//collaborated with Ryan Ly 862136697
 
bool found(const std::vector<Hit>& a,double t) {
 
    for(unsigned i = 0; i < a.size(); i++) {
        if(a.at(i).dist == t) {
            return true;
        }
    }
 
    return false;
}

bool checkIdentical(const std::vector<Hit>& tempA, const std::vector<Hit>& tempB) {
    bool check = false;
    if(tempA.size() == tempB.size()) {
        for(unsigned i = 0; i < tempA.size(); ++i) {
            if(tempA.at(i).dist == tempB.at(i).dist) {
                check = true;
            }
            else {
                return false;
            }
        } //checking the vectors are identical
    }
    return check;
}


void erase(std::vector<Hit>& tempA, double t) {
    unsigned pos = 99999;
    std::vector<Hit> result;
    if(!tempA.size()) {return;}
    for(unsigned i = 0; i < tempA.size(); i++) {
        if(tempA.at(i).dist == t) {
            pos = i;
            break;
            //std::cout << "FOUND" <<std::endl;
        }
    }
    for(unsigned i = 0; i < tempA.size(); i++) {
        if(i != pos) {
            result.push_back(tempA.at(i));
        }
    }
    tempA = result;
}
std::vector<Hit> Combine(
    const std::vector<Hit>& a,
    const std::vector<Hit>& b,
    bool_type type)
{
    std::vector<Hit> c;
    std::vector<Hit> tempA(a);
    std::vector<Hit> tempB(b);
    sort(tempA);
    sort(tempB);
 
    if(type == type_intersect) {    
        std::cout << "type_intersect" << std::endl;
        for(unsigned i = 0; i < tempA.size(); i++) {
            if(Test_Inside(tempB,tempA.at(i).dist)) {
                c.push_back(tempA.at(i));
            }
 
        }
        for(unsigned i = 0; i < tempB.size(); i++) {
            bool dupe = false;
            for(unsigned j = 0; j < tempA.size(); j++) {// dont add dupes
                if(tempA.at(j).dist == tempB.at(i).dist) {
                    dupe = true;
                    break;
                }
            }
            if(Test_Inside(tempA, tempB.at(i).dist) && !dupe) {
                c.push_back(tempB.at(i));
            }
        }
        sort(c);

    }
    if(type == type_union) {
        std::cout << "type_union" << std::endl;
       
        bool check1 = checkIdentical(tempA, tempB);
        if(check1) {
            for(unsigned i = 0; i < tempA.size(); ++i)
                c.push_back(tempA.at(i));
        }
        else  {
            for(unsigned i = 0; i < tempA.size(); i++) {
                if(!Test_Inside(tempB,tempA.at(i).dist)) {
                    c.push_back(tempA.at(i));
                }
 
            }
            for(unsigned i = 0; i < tempB.size(); i++) {
                if(!Test_Inside(tempA, tempB.at(i).dist) ) {
                    c.push_back(tempB.at(i));
                }
            }
            if(tempA.size() >= tempB.size()) {
                for(unsigned i = 0; i < tempB.size(); i++) {
                    for(unsigned j = 0; j < tempA.size(); j ++) {
                        if(tempB.at(i).dist == tempA.at(j).dist) {
                            c.push_back(tempB.at(i));
                        }
                    }
                }
            }
            else {
                for(unsigned i = 0; i < tempA.size(); i++) {
                    for(unsigned j = 0; j < tempB.size(); j ++) {
                        if(tempA.at(i).dist == tempB.at(j).dist) {
                            c.push_back(tempA.at(i));
                        }
                    }
                }
            }
           
 
        }
        sort(c);
 
    }
    if(type == type_difference) {    
        std::cout << "type_difference" << std::endl;
        bool check1 = checkIdentical(tempA, tempB);

        if(check1) {return c;}
        // Hit dummy = {"dummy head", 0};
        // std::vector<Hit> B_complement;

        // B_complement.push_back(dummy);
        // for(unsigned i = 0; i < tempB.size(); i++) {
        //     B_complement.push_back(tempB.at(i));
        // }

        // c = Combine(tempA, B_complement, type_intersect); // set difference using intersect closure
        // sort(c);
        // return c;


        for(unsigned i = 0; i < tempA.size(); i++) {
            if(!Test_Inside(tempB,tempA.at(i).dist)) {
                c.push_back(tempA.at(i));
            }
        }

        for(unsigned i = 0; i < tempB.size(); i++) {
            if(Test_Inside(tempA, tempB.at(i).dist)) {
                //if ((!found(tempA, tempB.at(i).dist)) && !found(c, tempB.at(i).dist) ) {//
                    if(!found(tempA,tempB.at(i).dist)) {
                     c.push_back(tempB.at(i));
                }
            }
        }
        if(tempB.size() == 1 && (tempA.size() && tempA.size())) {
            if(tempA.at(0).dist == tempB.at(0).dist) {
                c.clear();
            }
        }

        for(int i = 0; i < c.size(); ++i) {
            if((found(tempB, c.at(i).dist)) && found(tempA, c.at(i).dist)) {
                erase(c, i);
            }

        }
 
        sort(c); 
        //c = unique(c);
        //return c;
 
 
    }
    return c;
 
}
 
// Returns whether a point at distance t along a ray is inside an object whose
// hit list is given in a.
bool Test_Inside(const std::vector<Hit>& a, double t)
{
    //std::cout << "a.size: " << a.size() << std::endl;
    if(t < 0 ) {
        return false;
    }
 
    if(a.size() == 0) {
        return false; //no objects
    }
    else if(a.size() == 1) { //check if t is > than first element
        return (t >= a.at(0).dist);
    }
 
    if(a.size() % 2) { //check if t is > than last element
        if (t >= a.at(a.size()-1).dist) {
            return true;
        }
    }
 
    for(unsigned i = 0; i < a.size(); i++) {
        if(i%2 == 0 && a.size() > i + 1) {
            if (a.at(i).dist <= t && a.at(i+1).dist >= t) {
                //std::cout << "returning true t: " << t << "is between " << a.at(i).dist << " and " << a.at(i+1).dist <<std::endl;
                return true;
            }
        }
    }
 
 
    return false;
}
 
 
 

