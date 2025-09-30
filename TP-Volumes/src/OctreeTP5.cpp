#include "OctreeTP5.h"
#include "Vec3.h"
#include "tp5.h"
#include <GL/gl.h>
#include <algorithm>
#include <map>

OctreeTP5::OctreeTP5() {
    base_octant = nullptr;
}

OctreeTP5::OctreeTP5(const Vec3& min, const Vec3& max, int maxDepth){ // créer le cube de départ avec 0 points et pas d'enfants
    base_octant = new OctantTP5(min, max, 0);
    this->maxDepth = maxDepth;
}


void OctreeTP5::subdivideSphere(OctantTP5* octant, VolumeType volumeType , const Vec3& center, double radius, std::vector<Quad>& quads) {
    // check si enfant intersecte la surface
    if (octant->intersectsSphere(center, radius)) {
        // si l'octant est entièrement à l'intérieur de la surface on ajoute l'octant à la liste des QUADS à dessiner
        if (octant->isInsideSphere(center, radius)) {
            if (volumeType == VOLUMIQUE){
                quads.push_back(Quad{octant->center, (octant->max[0] - octant->min[0]) });
            }
        } else if (octant->depth < maxDepth) {
            // si oui et que la profondeur est inférieure à maxDepth
            Vec3 mid = (octant->min + octant->max) / 2.0f;
            for (int i = 0; i < 8; ++i) {
                Vec3 child_min = octant->min;
                Vec3 child_max = octant->max;

                if (i & 1)
                    child_min[0] = mid[0];
                else
                    child_max[0] = mid[0];

                if (i & 2)
                    child_min[1] = mid[1];
                else
                    child_max[1] = mid[1];

                if (i & 4)
                    child_min[2] = mid[2];
                else
                    child_max[2] = mid[2];

                octant->children[i] = new OctantTP5(child_min, child_max, octant->depth + 1);
                subdivideSphere(octant->children[i], volumeType, center, radius, quads);
            }
        } else {
            // sinon on ajoute l'octant à la liste des QUADS à dessiner
            quads.push_back(Quad{octant->center, (octant->max[0] - octant->min[0]) });
        }
    }
}


void OctreeTP5::subdivideCylinder(OctantTP5* octant, VolumeType volumeType , Vec3 axisOrigin, Vec3 axisVector, double rayon, std::vector<Quad>& quads) {
    // check si enfant intersecte la surface
    if (octant->intersectsCylinder(axisOrigin, axisVector, rayon)) {
        std::cout << "intersectsCylinder" << std::endl;
        // si l'octant est entièrement à l'intérieur de la surface on ajoute l'octant à la liste des QUADS à dessiner
        if (octant->isInsideCylinder(axisOrigin, axisVector, rayon)) {
            if (volumeType == VOLUMIQUE){
                quads.push_back(Quad{octant->center, (octant->max[0] - octant->min[0]) });
            }
        } else if (octant->depth < maxDepth) {
            // si oui et que la profondeur est inférieure à maxDepth
            Vec3 mid = (octant->min + octant->max) / 2.0f;
            for (int i = 0; i < 8; ++i) {
                Vec3 child_min = octant->min;
                Vec3 child_max = octant->max;

                if (i & 1)
                    child_min[0] = mid[0];
                else
                    child_max[0] = mid[0];

                if (i & 2)
                    child_min[1] = mid[1];
                else
                    child_max[1] = mid[1];

                if (i & 4)
                    child_min[2] = mid[2];
                else
                    child_max[2] = mid[2];

                octant->children[i] = new OctantTP5(child_min, child_max, octant->depth + 1);
                subdivideCylinder(octant->children[i], volumeType, axisOrigin, axisVector, rayon, quads);
            }
        } else {
            // sinon on ajoute l'octant à la liste des QUADS à dessiner
            quads.push_back(Quad{octant->center, (octant->max[0] - octant->min[0]) });
        }
    }
}

