#include "Octree.h"
#include "Vec3.h"
#include <GL/gl.h>
#include <algorithm>
#include <map>

Octree::Octree() {
    base_octant = nullptr;
}

Octree::Octree(const int nbVerticicesPerOctants, const Vec3& min, const Vec3& max){ // créer le cube de départ avec 0 points et pas d'enfants
    base_octant = new Octant(min, max);
    base_octant->id = 1;
    this->nbVerticicesPerOctants = nbVerticicesPerOctants;
}


void Octree::subdivide(Octant& octant) {
    Vec3 mid = (octant.min + octant.max) / 2.0f;
    for (int i = 0; i < 8; ++i) {
        Vec3 child_min = octant.min;
        Vec3 child_max = octant.max;

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

        octant.children[i] = new Octant(child_min, child_max);
        octant.children[i]->id = (octant.id << 3) | i; // id enfant = 3 bits du parent + 3 bits de l'enfant ()
    }
    
    for (int j = 0; j < octant.points.size(); ++j) {
        Vec3 point = octant.points[j];
        Vec3 normal = octant.normals[j];
        for (int i = 0; i < 8; ++i) {
            if (octant.children[i]->contains(point)) {
                octant.children[i]->points.push_back(point);
                octant.children[i]->normals.push_back(normal);
                if(octant.children[i]->points.size() > nbVerticicesPerOctants){
                    subdivide(*octant.children[i]);
                    octant.children[i]->isLeaf = false;
                }
                break;
            }
        }
    }
    octant.points.clear(); // enlever les points du parent
}


void Octree::addPoint(Octant& octant, const Vec3& point, const Vec3& normal) {
    if (octant.isLeaf) {
        octant.points.push_back(point);
        octant.normals.push_back(normal);
        if (octant.points.size() > nbVerticicesPerOctants) {
            subdivide(octant);
            octant.isLeaf = false;
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            if (octant.children[i]->contains(point)) {
                addPoint(*octant.children[i], point, normal);
                break;
            }
        }
    }
}

void Octree::printOctree(const Octant& octant, int depth) const {
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << "Octant: [" << octant.min[0] << ", " << octant.min[1] << ", " << octant.min[2] << "] - ["
              << octant.max[0] << ", " << octant.max[1] << ", " << octant.max[2] << "] with "
              << octant.points.size() << " points" << std::endl;

    if (!octant.isLeaf) {
        for (int i = 0; i < 8; ++i) {
            printOctree(*octant.children[i], depth + 1);
        }
    }
}

void Octree::print() const {
    printOctree(*base_octant, 0);
}


void Octree::drawOctant(const Octant& octant) const {
    glBegin(GL_LINES);

    glVertex3f(octant.min[0], octant.min[1], octant.min[2]);
    glVertex3f(octant.max[0], octant.min[1], octant.min[2]);

    glVertex3f(octant.min[0], octant.min[1], octant.min[2]);
    glVertex3f(octant.min[0], octant.max[1], octant.min[2]);

    glVertex3f(octant.min[0], octant.min[1], octant.min[2]);
    glVertex3f(octant.min[0], octant.min[1], octant.max[2]);

    glVertex3f(octant.max[0], octant.max[1], octant.max[2]);
    glVertex3f(octant.min[0], octant.max[1], octant.max[2]);

    glVertex3f(octant.max[0], octant.max[1], octant.max[2]);
    glVertex3f(octant.max[0], octant.min[1], octant.max[2]);

    glVertex3f(octant.max[0], octant.max[1], octant.max[2]);
    glVertex3f(octant.max[0], octant.max[1], octant.min[2]);

    glVertex3f(octant.min[0], octant.max[1], octant.min[2]);
    glVertex3f(octant.max[0], octant.max[1], octant.min[2]);

    glVertex3f(octant.min[0], octant.max[1], octant.min[2]);
    glVertex3f(octant.min[0], octant.max[1], octant.max[2]);

    glVertex3f(octant.max[0], octant.min[1], octant.min[2]);
    glVertex3f(octant.max[0], octant.max[1], octant.min[2]);

    glVertex3f(octant.max[0], octant.min[1], octant.min[2]);
    glVertex3f(octant.max[0], octant.min[1], octant.max[2]);

    glVertex3f(octant.min[0], octant.min[1], octant.max[2]);
    glVertex3f(octant.max[0], octant.min[1], octant.max[2]);

    glVertex3f(octant.min[0], octant.min[1], octant.max[2]);
    glVertex3f(octant.min[0], octant.max[1], octant.max[2]);

    glEnd();

    if (!octant.isLeaf) {
        for (int i = 0; i < 8; ++i) {
            drawOctant(*octant.children[i]);
        }
    }
}

void Octree::draw() const {
    drawOctant(*base_octant);
}

void Octree::SummPosAndNormalRecursive(Octant& octant) {
    if (octant.isLeaf) {
        int nbPoints = octant.points.size();
        for (size_t i = 0; i < octant.points.size(); ++i) {
            octant.pos += octant.points[i];
            octant.normal += octant.normals[i];
        }
        octant.pos = octant.pos / nbPoints;
        octant.normal = octant.normal / nbPoints;
        octant.normal.normalize();
    } else {
        for (int i = 0; i < 8; ++i) {
            SummPosAndNormalRecursive(*octant.children[i]);
        }
    }
}

void Octree::SummPosAndNormal() { 
    SummPosAndNormalRecursive(*base_octant);
}

int Octree::getOctantIndex(const Vec3 point) {
    return getOctantIndexRecursive(*base_octant, point);
}

int Octree::getOctantIndexRecursive(Octant& octant, const Vec3 point) {
    
    if (octant.isLeaf) {
        return octant.id;  
    }

    Vec3 mid = (octant.min + octant.max) / 2.0f;

    // Calculer l'index de l'enfant (0-7) basé sur la position relative au centre
    int childIndex = 0;
    if (point[0] >= mid[0]) childIndex |= 1;  // droite
    if (point[1] >= mid[1]) childIndex |= 2;  // haut
    if (point[2] >= mid[2]) childIndex |= 4;  // derrière

    return getOctantIndexRecursive(*octant.children[childIndex], point);
    

}


void Octree::traverseLeaves(Octant& octant, std::map<int, int>& idToVertexIndex, int& newIndex, std::vector<Vec3>& vertices, std::vector<Vec3>& normals) {
    if (octant.isLeaf && !octant.points.empty()) {
        idToVertexIndex[octant.id] = newIndex++;
        vertices.push_back(octant.pos);
        normals.push_back(octant.normal);
    } else if (!octant.isLeaf) {
        for (int i = 0; i < 8; ++i) {
            traverseLeaves(*octant.children[i], idToVertexIndex, newIndex, 
                          vertices, normals);
        }
    }
}



