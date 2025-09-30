// #include "Octree.h"
// #include "Octree.cpp"
#include "Vec3.h"
#include "tp5.h"
#include "OctreeTP5.h"

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>



void displayVoxel(Vec3 centre, double sideLength) {
    Vec3 bottomLeftVoxel = Vec3(centre[0] - sideLength/2, centre[1] - sideLength/2, centre[2] - sideLength/2);
    Vec3 topRightVoxel = Vec3(centre[0] + sideLength/2, centre[1] + sideLength/2, centre[2] + sideLength/2);

    glBegin(GL_QUADS);

    // BAS
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(bottomLeftVoxel[0], bottomLeftVoxel[1], bottomLeftVoxel[2]);
    glVertex3f(topRightVoxel[0], bottomLeftVoxel[1], bottomLeftVoxel[2]);
    glVertex3f(topRightVoxel[0], bottomLeftVoxel[1], topRightVoxel[2]);
    glVertex3f(bottomLeftVoxel[0], bottomLeftVoxel[1], topRightVoxel[2]);

    // HAUT
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(bottomLeftVoxel[0], topRightVoxel[1], bottomLeftVoxel[2]);
    glVertex3f(topRightVoxel[0], topRightVoxel[1], bottomLeftVoxel[2]);
    glVertex3f(topRightVoxel[0], topRightVoxel[1], topRightVoxel[2]);
    glVertex3f(bottomLeftVoxel[0], topRightVoxel[1], topRightVoxel[2]);

    // DEVANT
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(bottomLeftVoxel[0], bottomLeftVoxel[1], bottomLeftVoxel[2]);
    glVertex3f(topRightVoxel[0], bottomLeftVoxel[1], bottomLeftVoxel[2]);
    glVertex3f(topRightVoxel[0], topRightVoxel[1], bottomLeftVoxel[2]);
    glVertex3f(bottomLeftVoxel[0], topRightVoxel[1], bottomLeftVoxel[2]);

    // DERRIERE
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(bottomLeftVoxel[0], bottomLeftVoxel[1], topRightVoxel[2]);
    glVertex3f(topRightVoxel[0], bottomLeftVoxel[1], topRightVoxel[2]);
    glVertex3f(topRightVoxel[0], topRightVoxel[1], topRightVoxel[2]);
    glVertex3f(bottomLeftVoxel[0], topRightVoxel[1], topRightVoxel[2]);

    // GAUCHE
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(bottomLeftVoxel[0], bottomLeftVoxel[1], bottomLeftVoxel[2]);
    glVertex3f(bottomLeftVoxel[0], bottomLeftVoxel[1], topRightVoxel[2]);
    glVertex3f(bottomLeftVoxel[0], topRightVoxel[1], topRightVoxel[2]);
    glVertex3f(bottomLeftVoxel[0], topRightVoxel[1], bottomLeftVoxel[2]);

    // DROITE
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(topRightVoxel[0], bottomLeftVoxel[1], bottomLeftVoxel[2]);
    glVertex3f(topRightVoxel[0], bottomLeftVoxel[1], topRightVoxel[2]);
    glVertex3f(topRightVoxel[0], topRightVoxel[1], topRightVoxel[2]);
    glVertex3f(topRightVoxel[0], topRightVoxel[1], bottomLeftVoxel[2]);

    glEnd();
}

// void displayVoxelSphere(Vec3 centre, double rayon, double resolution, VolumeType volumeType, std::vector<Quad>& quads){ 
//     Vec3 bottomLeftVoxel = Vec3(centre[0] - rayon, centre[1] - rayon, centre[2] - rayon); 
//     Vec3 topRightVoxel = Vec3(centre[0] + rayon, centre[1] + rayon, centre[2] + rayon);

//     float step = 2 * rayon / resolution;
//     for (float i = bottomLeftVoxel[0]; i <= topRightVoxel[0]; i += step) {
//         for (float j = bottomLeftVoxel[1]; j <= topRightVoxel[1]; j += step) {
//             for (float k = bottomLeftVoxel[2]; k <= topRightVoxel[2]; k += step) {
//                 Vec3 currentPoint = Vec3(i + step/2, j+ step/2, k+ step/2);
//                 double distance = sqrt(pow(currentPoint[0] - centre[0], 2) +
//                                        pow(currentPoint[1] - centre[1], 2) +
//                                        pow(currentPoint[2] - centre[2], 2));

//                 if (volumeType == VOLUMIQUE) {
//                     if (distance <= rayon) {
//                         quads.push_back(Quad{currentPoint, step}); 
//                     }
//                 } else {
//                     if (distance <= rayon && distance >= rayon - step) {   
//                         quads.push_back(Quad{currentPoint, step}); 
//                     }
//                 }
//             }
//         }
//     }
// }

void displayVoxelSphere(Vec3 centre, double rayon, double resolution, VolumeType volumeType, std::vector<Quad>& quads){ 
    Vec3 bottomLeftVoxel = Vec3(centre[0] - rayon, centre[1] - rayon, centre[2] - rayon); 
    Vec3 topRightVoxel = Vec3(centre[0] + rayon, centre[1] + rayon, centre[2] + rayon);

    OctreeTP5 octree = OctreeTP5(bottomLeftVoxel, topRightVoxel, resolution);
    quads.clear();
    octree.subdivideSphere(octree.base_octant, volumeType , centre, rayon, quads);

}



// void displayVoxelCylinder(Vec3 axisOrigin, Vec3 axisVector, double rayon, double resolution, VolumeType volumeType, std::vector<Quad>& quads) {

//     double axisLength = axisVector.length();
//     Vec3 normalizedAxisVector = axisVector / axisLength;

//     Vec3 bottomLeftVoxel = Vec3(
//         std::min(axisOrigin[0], axisOrigin[0] + axisVector[0]) - rayon,
//         std::min(axisOrigin[1], axisOrigin[1] + axisVector[1]) - rayon,
//         std::min(axisOrigin[2], axisOrigin[2] + axisVector[2]) - rayon
//     );

//     Vec3 topRightVoxel = Vec3(
//         std::max(axisOrigin[0], axisOrigin[0] + axisVector[0]) + rayon,
//         std::max(axisOrigin[1], axisOrigin[1] + axisVector[1]) + rayon,
//         std::max(axisOrigin[2], axisOrigin[2] + axisVector[2]) + rayon
//     );

//     float step = 2 * rayon / resolution;
//     //double step = (topRightVoxel - bottomLeftVoxel).length() / resolution;
//     for (float i = bottomLeftVoxel[0]; i <= topRightVoxel[0]; i += step) {
//         for (float j = bottomLeftVoxel[1]; j <= topRightVoxel[1]; j += step) {
//             for (float k = bottomLeftVoxel[2]; k <= topRightVoxel[2]; k += step) {
//                 Vec3 currentPoint = Vec3(i + step / 2, j + step / 2, k + step / 2);

//                 Vec3 vecToPoint = currentPoint - axisOrigin;
//                 float dotProduct = Vec3::dot(vecToPoint, normalizedAxisVector);

//                 Vec3 projection = axisOrigin + dotProduct * normalizedAxisVector;

//                 double distance = sqrt(pow(currentPoint[0] - projection[0], 2) +
//                                        pow(currentPoint[1] - projection[1], 2) +
//                                        pow(currentPoint[2] - projection[2], 2));

//                 if (volumeType == VOLUMIQUE){
//                     if (distance <= rayon && dotProduct >= 0 && dotProduct <= axisLength) {
//                         quads.push_back(Quad{currentPoint, step}); 
//                     }
//                 }else {
//                     if (distance <= rayon && dotProduct >= 0 && dotProduct <= axisLength) {
//                         if (dotProduct <= step || dotProduct >= axisLength - step || distance >= rayon - step) {
//                             quads.push_back(Quad{currentPoint, step}); 
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

// void displayVoxelCylinder(Vec3 axisOrigin, Vec3 axisVector, double rayon, double resolution, VolumeType volumeType, std::vector<Quad>& quads) {

//     Vec3 bottomLeftVoxel = Vec3(
//         std::min(axisOrigin[0], axisOrigin[0] + axisVector[0]) - rayon,
//         std::min(axisOrigin[1], axisOrigin[1] + axisVector[1]) - rayon,
//         std::min(axisOrigin[2], axisOrigin[2] + axisVector[2]) - rayon
//     );

//     Vec3 topRightVoxel = Vec3(
//         std::max(axisOrigin[0], axisOrigin[0] + axisVector[0]) + rayon,
//         std::max(axisOrigin[1], axisOrigin[1] + axisVector[1]) + rayon,
//         std::max(axisOrigin[2], axisOrigin[2] + axisVector[2]) + rayon
//     );

//     OctreeTP5 octree = OctreeTP5(bottomLeftVoxel, topRightVoxel, resolution);
//     quads.clear();
//     octree.subdivideCylinder(octree.base_octant, volumeType , axisOrigin, axisVector, rayon , quads);

// }


void displayVoxelCylinder(Vec3 axisOrigin, Vec3 axisVector, double rayon, double resolution, VolumeType volumeType, std::vector<Quad>& quads) {

    double axisLength = axisVector.length();
    Vec3 normalizedAxisVector = axisVector / axisLength;

    Vec3 bottomLeftVoxel = Vec3(
        std::min(axisOrigin[0], axisOrigin[0] + axisVector[0]) - rayon,
        std::min(axisOrigin[1], axisOrigin[1] + axisVector[1]) - rayon,
        std::min(axisOrigin[2], axisOrigin[2] + axisVector[2]) - rayon
    );

    Vec3 topRightVoxel = Vec3(
        std::max(axisOrigin[0], axisOrigin[0] + axisVector[0]) + rayon,
        std::max(axisOrigin[1], axisOrigin[1] + axisVector[1]) + rayon,
        std::max(axisOrigin[2], axisOrigin[2] + axisVector[2]) + rayon
    );

    float stepX = (topRightVoxel[0] - bottomLeftVoxel[0]) / resolution;
    float stepY = (topRightVoxel[1] - bottomLeftVoxel[1]) / resolution;
    float stepZ = (topRightVoxel[2] - bottomLeftVoxel[2]) / resolution;

    float step = std::min(stepX, std::min(stepY, stepZ));

    for (float i = bottomLeftVoxel[0]; i <= topRightVoxel[0]; i += step) {
        for (float j = bottomLeftVoxel[1]; j <= topRightVoxel[1]; j += step) {
            for (float k = bottomLeftVoxel[2]; k <= topRightVoxel[2]; k += step) {
                Vec3 currentPoint = Vec3(i + step / 2, j + step / 2, k + step / 2);

                Vec3 vecToPoint = currentPoint - axisOrigin;
                float dotProduct = Vec3::dot(vecToPoint, normalizedAxisVector);

                Vec3 projection = axisOrigin + dotProduct * normalizedAxisVector;

                double distance = sqrt(pow(currentPoint[0] - projection[0], 2) +
                                       pow(currentPoint[1] - projection[1], 2) +
                                       pow(currentPoint[2] - projection[2], 2));

                if (volumeType == VOLUMIQUE) {
                    if (distance <= rayon && dotProduct >= -step && dotProduct <= axisLength + step) {
                        quads.push_back(Quad{currentPoint, step});
                    }
                } else {
                    if (distance <= rayon && dotProduct >= 0 && dotProduct <= axisLength) {
                        if (dotProduct <= step || dotProduct >= axisLength - step || (distance >= rayon - step && distance <= rayon)) {
                            quads.push_back(Quad{currentPoint, step});
                        }
                    }
                }
            }
        }
    }
}

void display_SphereCylinderOperation(Vec3 centreSphere, double rayonSphere, Vec3 axisOriginCylinder, Vec3 axisVectorCylinder, double rayonCylinder, double resolution, Operation action, std::vector<Quad>& quads){
    
    double axisLength = axisVectorCylinder.length();
    Vec3 normalizedAxisVector = axisVectorCylinder / axisLength;

    Vec3 bottomLeftVoxel = Vec3(
        std::min(centreSphere[0] - rayonSphere, std::min(axisOriginCylinder[0], axisOriginCylinder[0] + axisVectorCylinder[0]) - rayonCylinder),
        std::min(centreSphere[1] - rayonSphere, std::min(axisOriginCylinder[1], axisOriginCylinder[1] + axisVectorCylinder[1]) - rayonCylinder),
        std::min(centreSphere[2] - rayonSphere, std::min(axisOriginCylinder[2], axisOriginCylinder[2] + axisVectorCylinder[2]) - rayonCylinder)
    );

    Vec3 topRightVoxel = Vec3(
        std::max(centreSphere[0] + rayonSphere, std::max(axisOriginCylinder[0], axisOriginCylinder[0] + axisVectorCylinder[0]) + rayonCylinder),
        std::max(centreSphere[1] + rayonSphere, std::max(axisOriginCylinder[1], axisOriginCylinder[1] + axisVectorCylinder[1]) + rayonCylinder),
        std::max(centreSphere[2] + rayonSphere, std::max(axisOriginCylinder[2], axisOriginCylinder[2] + axisVectorCylinder[2]) + rayonCylinder)
    );

    float stepX = (topRightVoxel[0] - bottomLeftVoxel[0])/resolution;
    float stepY = (topRightVoxel[1] - bottomLeftVoxel[1])/resolution;
    float stepZ = (topRightVoxel[2] - bottomLeftVoxel[2])/resolution;

    float step = std::min(stepX, std::min(stepY, stepZ));

    for (float i = bottomLeftVoxel[0]; i <= topRightVoxel[0]; i += step) {
        for (float j = bottomLeftVoxel[1]; j <= topRightVoxel[1]; j += step) {
            for (float k = bottomLeftVoxel[2]; k <= topRightVoxel[2]; k += step) {
                Vec3 currentPoint = Vec3(i + step / 2, j + step / 2, k + step / 2);

                Vec3 vecToPoint = currentPoint - axisOriginCylinder;
                float dotProduct = Vec3::dot(vecToPoint, normalizedAxisVector);

                Vec3 projection = axisOriginCylinder + dotProduct * normalizedAxisVector;

                double distanceToCylinder = sqrt(pow(currentPoint[0] - projection[0], 2) +
                                       pow(currentPoint[1] - projection[1], 2) +
                                       pow(currentPoint[2] - projection[2], 2));
        
                double distanceToSphere = sqrt(pow(currentPoint[0] - centreSphere[0], 2) +
                                               pow(currentPoint[1] - centreSphere[1], 2) +
                                               pow(currentPoint[2] - centreSphere[2], 2));

                bool insideSphere = distanceToSphere <= rayonSphere; 
                
                bool insideCylinder = false;
                 if (distanceToCylinder <= rayonCylinder && dotProduct >= 0 && dotProduct <= axisLength) {
                    insideCylinder = true;
                }

                if(action == INTERSECTION){
                    if (insideSphere && insideCylinder) {
                        quads.push_back(Quad{currentPoint, step}); 
                    }
                } else if(action == UNION){
                    if (insideSphere || insideCylinder) {
                        quads.push_back(Quad{currentPoint, step}); 
                    }
                } else if(action == SOUSTRACTION){
                    if (insideSphere && !insideCylinder) {
                        quads.push_back(Quad{currentPoint, step}); 
                    }
                }

            }
        }
    }

}


// void display_INTERSECTION_SphereCylinder(Vec3 centreSphere, double rayonSphere, Vec3 axisOriginCylinder, Vec3 axisVectorCylinder, double rayonCylinder, double resolution){

//     double axisLength = axisVectorCylinder.length();
//     Vec3 normalizedAxisVector = axisVectorCylinder / axisLength;

//     Vec3 bottomLeftVoxel = Vec3(
//         std::min(centreSphere[0] - rayonSphere, std::min(axisOriginCylinder[0], axisOriginCylinder[0] + axisVectorCylinder[0]) - rayonCylinder),
//         std::min(centreSphere[1] - rayonSphere, std::min(axisOriginCylinder[1], axisOriginCylinder[1] + axisVectorCylinder[1]) - rayonCylinder),
//         std::min(centreSphere[2] - rayonSphere, std::min(axisOriginCylinder[2], axisOriginCylinder[2] + axisVectorCylinder[2]) - rayonCylinder)
//     );

//     Vec3 topRightVoxel = Vec3(
//         std::max(centreSphere[0] + rayonSphere, std::max(axisOriginCylinder[0], axisOriginCylinder[0] + axisVectorCylinder[0]) + rayonCylinder),
//         std::max(centreSphere[1] + rayonSphere, std::max(axisOriginCylinder[1], axisOriginCylinder[1] + axisVectorCylinder[1]) + rayonCylinder),
//         std::max(centreSphere[2] + rayonSphere, std::max(axisOriginCylinder[2], axisOriginCylinder[2] + axisVectorCylinder[2]) + rayonCylinder)
//     );


//     float step = (topRightVoxel - bottomLeftVoxel).length() / resolution;

//     for (float i = bottomLeftVoxel[0]; i <= topRightVoxel[0]; i += step) {
//         for (float j = bottomLeftVoxel[1]; j <= topRightVoxel[1]; j += step) {
//             for (float k = bottomLeftVoxel[2]; k <= topRightVoxel[2]; k += step) {
//                 Vec3 currentPoint = Vec3(i + step / 2, j + step / 2, k + step / 2);
                
//                 Vec3 vecToPoint = currentPoint - axisOriginCylinder;
//                 float dotProduct = Vec3::dot(vecToPoint, normalizedAxisVector);

//                 Vec3 projection = axisOriginCylinder + dotProduct * normalizedAxisVector;

//                 double distanceToCylinder = sqrt(pow(currentPoint[0] - projection[0], 2) +
//                                        pow(currentPoint[1] - projection[1], 2) +
//                                        pow(currentPoint[2] - projection[2], 2));
        
//                 double distanceToSphere = sqrt(pow(currentPoint[0] - centreSphere[0], 2) +
//                                                pow(currentPoint[1] - centreSphere[1], 2) +
//                                                pow(currentPoint[2] - centreSphere[2], 2));

//                 bool insideSphere = distanceToSphere <= rayonSphere; 
                
//                 bool insideCylinder = false;
//                  if (distanceToCylinder <= rayonCylinder && dotProduct >= 0 && dotProduct <= axisLength) {
//                     insideCylinder = true;
//                 }

//                 if (insideSphere && insideCylinder) {
//                     displayVoxel(currentPoint, step);
//                 }

//             }
//         }
//     }

// }

// void display_SOUSTRACTION_SphereCylinder(Vec3 centreSphere, double rayonSphere, Vec3 axisOriginCylinder, Vec3 axisVectorCylinder, double rayonCylinder, double resolution){

//     double axisLength = axisVectorCylinder.length();
//     Vec3 normalizedAxisVector = axisVectorCylinder / axisLength;

//     Vec3 bottomLeftVoxel = Vec3(
//         std::min(centreSphere[0] - rayonSphere, std::min(axisOriginCylinder[0], axisOriginCylinder[0] + axisVectorCylinder[0]) - rayonCylinder),
//         std::min(centreSphere[1] - rayonSphere, std::min(axisOriginCylinder[1], axisOriginCylinder[1] + axisVectorCylinder[1]) - rayonCylinder),
//         std::min(centreSphere[2] - rayonSphere, std::min(axisOriginCylinder[2], axisOriginCylinder[2] + axisVectorCylinder[2]) - rayonCylinder)
//     );

//     Vec3 topRightVoxel = Vec3(
//         std::max(centreSphere[0] + rayonSphere, std::max(axisOriginCylinder[0], axisOriginCylinder[0] + axisVectorCylinder[0]) + rayonCylinder),
//         std::max(centreSphere[1] + rayonSphere, std::max(axisOriginCylinder[1], axisOriginCylinder[1] + axisVectorCylinder[1]) + rayonCylinder),
//         std::max(centreSphere[2] + rayonSphere, std::max(axisOriginCylinder[2], axisOriginCylinder[2] + axisVectorCylinder[2]) + rayonCylinder)
//     );


//     float step = (topRightVoxel - bottomLeftVoxel).length() / resolution;

//     for (float i = bottomLeftVoxel[0]; i <= topRightVoxel[0]; i += step) {
//         for (float j = bottomLeftVoxel[1]; j <= topRightVoxel[1]; j += step) {
//             for (float k = bottomLeftVoxel[2]; k <= topRightVoxel[2]; k += step) {
//                 Vec3 currentPoint = Vec3(i + step / 2, j + step / 2, k + step / 2);
                
//                 Vec3 vecToPoint = currentPoint - axisOriginCylinder;
//                 float dotProduct = Vec3::dot(vecToPoint, normalizedAxisVector);

//                 Vec3 projection = axisOriginCylinder + dotProduct * normalizedAxisVector;

//                 double distanceToCylinder = sqrt(pow(currentPoint[0] - projection[0], 2) +
//                                        pow(currentPoint[1] - projection[1], 2) +
//                                        pow(currentPoint[2] - projection[2], 2));
        
//                 double distanceToSphere = sqrt(pow(currentPoint[0] - centreSphere[0], 2) +
//                                                pow(currentPoint[1] - centreSphere[1], 2) +
//                                                pow(currentPoint[2] - centreSphere[2], 2));

//                 bool insideSphere = distanceToSphere <= rayonSphere; 
                
//                 bool insideCylinder = false;
//                  if (distanceToCylinder <= rayonCylinder && dotProduct >= 0 && dotProduct <= axisLength) {
//                     insideCylinder = true;
//                 }

//                 if (insideSphere && !insideCylinder) {
//                     displayVoxel(currentPoint, step);
//                 }

//             }
//         }
//     }

// }


// void display_UNION_SphereCylinder(Vec3 centreSphere, double rayonSphere, Vec3 axisOriginCylinder, Vec3 axisVectorCylinder, double rayonCylinder, double resolution){

//     double axisLength = axisVectorCylinder.length();
//     Vec3 normalizedAxisVector = axisVectorCylinder / axisLength;

//     Vec3 bottomLeftVoxel = Vec3(
//         std::min(centreSphere[0] - rayonSphere, std::min(axisOriginCylinder[0], axisOriginCylinder[0] + axisVectorCylinder[0]) - rayonCylinder),
//         std::min(centreSphere[1] - rayonSphere, std::min(axisOriginCylinder[1], axisOriginCylinder[1] + axisVectorCylinder[1]) - rayonCylinder),
//         std::min(centreSphere[2] - rayonSphere, std::min(axisOriginCylinder[2], axisOriginCylinder[2] + axisVectorCylinder[2]) - rayonCylinder)
//     );

//     Vec3 topRightVoxel = Vec3(
//         std::max(centreSphere[0] + rayonSphere, std::max(axisOriginCylinder[0], axisOriginCylinder[0] + axisVectorCylinder[0]) + rayonCylinder),
//         std::max(centreSphere[1] + rayonSphere, std::max(axisOriginCylinder[1], axisOriginCylinder[1] + axisVectorCylinder[1]) + rayonCylinder),
//         std::max(centreSphere[2] + rayonSphere, std::max(axisOriginCylinder[2], axisOriginCylinder[2] + axisVectorCylinder[2]) + rayonCylinder)
//     );


//     float step = (topRightVoxel - bottomLeftVoxel).length() / resolution;

//     for (float i = bottomLeftVoxel[0]; i <= topRightVoxel[0]; i += step) {
//         for (float j = bottomLeftVoxel[1]; j <= topRightVoxel[1]; j += step) {
//             for (float k = bottomLeftVoxel[2]; k <= topRightVoxel[2]; k += step) {
//                 Vec3 currentPoint = Vec3(i + step / 2, j + step / 2, k + step / 2);
                
//                 Vec3 vecToPoint = currentPoint - axisOriginCylinder;
//                 float dotProduct = Vec3::dot(vecToPoint, normalizedAxisVector);

//                 Vec3 projection = axisOriginCylinder + dotProduct * normalizedAxisVector;

//                 double distanceToCylinder = sqrt(pow(currentPoint[0] - projection[0], 2) +
//                                        pow(currentPoint[1] - projection[1], 2) +
//                                        pow(currentPoint[2] - projection[2], 2));
        
//                 double distanceToSphere = sqrt(pow(currentPoint[0] - centreSphere[0], 2) +
//                                                pow(currentPoint[1] - centreSphere[1], 2) +
//                                                pow(currentPoint[2] - centreSphere[2], 2));

//                 bool insideSphere = distanceToSphere <= rayonSphere; 
                
//                 bool insideCylinder = false;
//                  if (distanceToCylinder <= rayonCylinder && dotProduct >= 0 && dotProduct <= axisLength) {
//                     insideCylinder = true;
//                 }

//                 if (insideSphere || insideCylinder) {
//                     displayVoxel(currentPoint, step);
//                 }

//             }
//         }
//     }

// }

