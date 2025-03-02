#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, -length, 0), color),
        VertexFormat(corner + glm::vec3(length, -length, 0), color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(leftBottomCorner, color),
        VertexFormat(leftBottomCorner + glm::vec3(length, 0, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(length, width, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(0, width, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTrapezoid(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float baseWidth,
    float topWidth,
    float height,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices = {
        VertexFormat(leftBottomCorner + glm::vec3(-baseWidth / 2, 0, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(baseWidth / 2, 0, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(topWidth / 2, height, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(-topWidth / 2, height, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0, 2 };

    Mesh* trapezoid = new Mesh(name);
    if (fill) {
        indices.push_back(1);
        indices.push_back(3);
    }
    else {
        trapezoid->SetDrawMode(GL_LINE_LOOP);
    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    int numSegments)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(center, color));
    indices.push_back(0);

    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        vertices.push_back(VertexFormat(center + glm::vec3(x, y, 0), color));
        indices.push_back(i + 1);
    }

    // Connect the last vertex back to the first to complete the circle
    indices.push_back(1);

    Mesh* circle = new Mesh(name);

    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);
    return circle;
}
