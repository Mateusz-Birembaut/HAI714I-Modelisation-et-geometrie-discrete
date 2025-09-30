#ifndef OCTREETP5_H
#define OCTREETP5_H

#include "Vec3.h"
#include "tp5.h"
#include <vector>
#include <map>

enum ObjectType {
    SPHERE,
    CYLINDER
};

class OctantTP5 {
public:
    Vec3 min; // bas gauche
    Vec3 max; // coordonée haut droite
    Vec3 center; // centre de l'octant
    float radius; // rayon de l'octant

    OctantTP5 *children[8]; // adresse des fils

    int depth;
    bool isLeaf = true; 

    OctantTP5(const Vec3& min, const Vec3& max, int depth){
        this->min = min;
        this->max = max;
        this->center = (min + max) / 2.0f;
        this->radius = (max - min).length() / 2.0f;
        this->depth = depth;
        this->isLeaf = true;

        for (int i = 0; i < 8; ++i) {
            children[i] = nullptr;
        }
    }


    void getCorners(Vec3 corners[8]) const {
        corners[0] = Vec3(min[0], min[1], min[2]);
        corners[1] = Vec3(max[0], min[1], min[2]);
        corners[2] = Vec3(min[0], max[1], min[2]);
        corners[3] = Vec3(max[0], max[1], min[2]);
        corners[4] = Vec3(min[0], min[1], max[2]);
        corners[5] = Vec3(max[0], min[1], max[2]);
        corners[6] = Vec3(min[0], max[1], max[2]);
        corners[7] = Vec3(max[0], max[1], max[2]);
    }

    bool isInsideSphere(const Vec3& center, double radius) const {
        Vec3 corners[8];
        getCorners(corners);

        for (int i = 0; i < 8; ++i) {
            Vec3 diff = corners[i] - center;
            if (diff.length() > radius) {
                return false;
            }
        }
        return true;
    }

    bool intersectsSphere(const Vec3& center, double radius) const {
        double squaredDistance = 0.0;
        for (int i = 0; i < 3; ++i) {
            double v = center[i];
            if (v < min[i]) {
                double diff = min[i] - v;
                squaredDistance += diff * diff;
            } else if (v > max[i]) {
                double diff = v - max[i];
                squaredDistance += diff * diff;
            }
        }
        return squaredDistance <= radius * radius;
    }

    bool intersectsCylinder(const Vec3& axisOrigin, const Vec3& axisVector, double rayon) const {
        Vec3 corners[8];
        getCorners(corners);
        Vec3 normalizedAxisVector = axisVector/axisVector.length();
        bool oneIsInside = false;
        bool oneIsOutside = false;

        // Vérifier si l'un des coins de l'octant est à l'intérieur du cylindre
        for (int i = 0; i < 8; ++i) {
                Vec3 vecToPoint = corners[i] - axisOrigin;
                float dotProduct = Vec3::dot(vecToPoint, normalizedAxisVector);

                Vec3 projection = axisOrigin + dotProduct * normalizedAxisVector;

                double distance = sqrt(pow(corners[i][0] - projection[0], 2) +
                                       pow(corners[i][1] - projection[1], 2) +
                                       pow(corners[i][2] - projection[2], 2));

            // std::cout << "distance : " << distance << std::endl;
            // std::cout << "rayon : " << rayon << std::endl;
            // std::cout << "dot product : " << dotProduct << std::endl;
            // std::cout << "axis length : " << axisVector.length() << std::endl;

            if (distance <= rayon && dotProduct >= 0 && dotProduct <= axisVector.length()) {
                oneIsInside = true;
            }else {
                oneIsOutside = true;
            }
        }
        
        return oneIsInside && oneIsOutside;
    }

    bool isInsideCylinder(const Vec3& axisOrigin, const Vec3& axisVector, double rayon) const {
        Vec3 corners[8];
        getCorners(corners);
        Vec3 normalizedAxisVector = axisVector/axisVector.length();

        // Vérifier si l'un des coins de l'octant est à l'intérieur du cylindre
        for (int i = 0; i < 8; ++i) {
                Vec3 vecToPoint = corners[i] - axisOrigin;
                float dotProduct = Vec3::dot(vecToPoint, normalizedAxisVector);

                Vec3 projection = axisOrigin + dotProduct * normalizedAxisVector;

                double distance = sqrt(pow(corners[i][0] - projection[0], 2) +
                                       pow(corners[i][1] - projection[1], 2) +
                                       pow(corners[i][2] - projection[2], 2));

            if (distance > rayon || dotProduct < 0 || dotProduct > axisVector.length()) {
                return false;
            }
        }

        // Vérifier si l'axe du cylindre intersecte l'octant
        // (à implémenter si nécessaire)

        return true;
    }
};

class OctreeTP5 {
public:
    OctantTP5* base_octant; 

    int maxDepth; 

    OctreeTP5(const Vec3& min, const Vec3& max, int maxDepth);
    OctreeTP5();

    void subdivideSphere(OctantTP5* octant, VolumeType volumeType , const Vec3& center, double radius, std::vector<Quad>& quads);
    
    void subdivideCylinder(OctantTP5* octant, VolumeType volumeType , Vec3 axisOrigin, Vec3 axisVector, double rayon, std::vector<Quad>& quads);

private:

};


#endif // OCTREE_H