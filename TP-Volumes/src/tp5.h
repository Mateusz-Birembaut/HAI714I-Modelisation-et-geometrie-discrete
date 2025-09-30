#ifndef TP_H
#define TP_H

#include "Vec3.h"
#include <vector>

enum Operation {
    INTERSECTION,
    UNION,
    SOUSTRACTION
};

enum VolumeType {
    VOLUMIQUE,
    SURFACIQUE
};


struct Quad {
    Vec3 centre;
    double sideLength;
};

void displayVoxel(Vec3 centre, double sideLength);
void displayVoxelSphere(Vec3 centre, double rayon, double resolution, VolumeType volumeType, std::vector<Quad>& quads);
void displayVoxelCylinder(Vec3 axisOrigin, Vec3 axisVector, double rayon, double resolution, VolumeType volumeType, std::vector<Quad>& quads);
void display_INTERSECTION_SphereCylinder(Vec3 centreSphere, double rayonSphere, Vec3 axisOriginCylinder, Vec3 axisVectorCylinder, double rayonCylinder, double resolution);
void display_SOUSTRACTION_SphereCylinder(Vec3 centreSphere, double rayonSphere, Vec3 axisOriginCylinder, Vec3 axisVectorCylinder, double rayonCylinder, double resolution);
void display_UNION_SphereCylinder(Vec3 centreSphere, double rayonSphere, Vec3 axisOriginCylinder, Vec3 axisVectorCylinder, double rayonCylinder, double resolution);

void display_SphereCylinderOperation(Vec3 centreSphere, double rayonSphere,Vec3 axisOriginCylinder, Vec3 axisVectorCylinder, double rayonCylinder,double resolution, Operation action,  std::vector<Quad>& quads);

#endif // TP_H