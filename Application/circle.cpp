#include "Mesh.h"

Mesh MeshGenerator::Circle(unsigned int sides) {

    // TODO : Fix generation of normals

	std::vector<Vector3> Positions;
	std::vector<Vector2> UV;
	std::vector<Vector3> Normals;
	std::vector<unsigned int> indices;

    float angleStep = (double)360 / sides;
    Vector3 cursor(0, 0.5f, 0);
    Vector3 center = Vector3(0);

    Positions.push_back(center);
    UV.push_back(Vector2(0.5f, 0.5f));
    Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

    for (int i = 0; i < sides; i++) {
        Positions.push_back(cursor);
        UV.push_back(Vector2::Distance(cursor.xy(), Vector2(-1, -1)).Normalized());
        Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

        cursor = Vector3(Vector2::RotateAround(cursor.xy(), center.xy(), Math::deg2rad(angleStep)), 0);
    }
    unsigned int maxSize = Positions.size() - 1;
    for (int i = 0; i < Positions.size(); i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        if (i + 2 > maxSize) {
            indices.push_back(1);
            break;
        }
        else
            indices.push_back(i + 2);
    }
    return ToMesh(GL_TRIANGLES, Positions, UV, Normals, indices);
}