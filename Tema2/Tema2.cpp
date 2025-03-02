#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    polygonMode = GL_FILL;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    textRenderer->Load(PATH_JOIN(RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 24);

    {
        shader = new Shader("Tema2ShaderTerrain");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShaderTerrain.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        shader = new Shader("Tema2Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Cone.glb");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cylinder");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Cylinder.glb");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a cube
    {
        vector<VertexFormat> vertices = {
            // Front face
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),

            // Back face
            VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
        };

        vector<unsigned int> indices = {
            // Front face
            0, 1, 2,  1, 3, 2,

            // Back face
            4, 5, 6,  5, 7, 6,

            // Left face
            0, 4, 2,  4, 6, 2,

            // Right face
            1, 5, 3,  5, 7, 3,

            // Bottom face
            0, 1, 4,  1, 5, 4,

            // Top face
            2, 3, 6,  3, 7, 6
        };

        Mesh* cube = new Mesh("cube");
        cube->InitFromData(vertices, indices);
        meshes[cube->GetMeshID()] = cube;
    }

    // Create a circle
    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        const float outerRadius = 0.5f;  // Outer radius of the ring
        const float innerRadius = 0.3f;  // Inner radius of the ring
        const unsigned int numSlices = 50; // Number of slices (segments)

        // Generate vertices for both the inner and outer rings
        for (unsigned int i = 0; i <= numSlices; ++i) {
            float angle = 2.0f * glm::pi<float>() * i / numSlices;

            // Outer ring vertex
            float xOuter = outerRadius * cos(angle);
            float zOuter = outerRadius * sin(angle);
            vertices.push_back(VertexFormat(glm::vec3(xOuter, 0.0f, zOuter), glm::vec3(0, 1, 0), glm::vec3(0.2f, 0.8f, 0.6f)));

            // Inner ring vertex
            float xInner = innerRadius * cos(angle);
            float zInner = innerRadius * sin(angle);
            vertices.push_back(VertexFormat(glm::vec3(xInner, 0.0f, zInner), glm::vec3(0, 1, 0), glm::vec3(0.8f, 0.2f, 0.4f)));
        }

        // Generate indices for the triangle strip
        for (unsigned int i = 0; i < numSlices; ++i) {
            unsigned int outerIndex = 2 * i;     // Outer vertex index
            unsigned int innerIndex = 2 * i + 1; // Inner vertex index
            unsigned int nextOuterIndex = 2 * (i + 1);
            unsigned int nextInnerIndex = 2 * (i + 1) + 1;

            // Create two triangles for each segment
            indices.push_back(outerIndex);
            indices.push_back(innerIndex);
            indices.push_back(nextOuterIndex);

            indices.push_back(innerIndex);
            indices.push_back(nextInnerIndex);
            indices.push_back(nextOuterIndex);
        }

        // Create and initialize the mesh
        Mesh* hollowCircle = new Mesh("circle");
        hollowCircle->InitFromData(vertices, indices);
        meshes[hollowCircle->GetMeshID()] = hollowCircle;
    }

    // Create terrain
    {
        int m = 100; // Number of divisions along the X-axis
        int n = 100; // Number of divisions along the Z-axis
        float cellSize = 1.0f;

        // Calculate the offset to center the grid
        float offsetX = m * cellSize / 2.0f;
        float offsetZ = n * cellSize / 2.0f;

        // Generate vertices
        std::vector<VertexFormat> vertices;
        for (int i = 0; i <= m; ++i) {
            for (int j = 0; j <= n; ++j) {
                float x = i * cellSize - offsetX;  // Center along the X-axis
                float z = j * cellSize - offsetZ;  // Center along the Z-axis
                vertices.emplace_back(glm::vec3(x, 0, z), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1));
            }
        }

        // Generate indices
        std::vector<unsigned int> indices;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                int topLeft = i * (n + 1) + j;
                int topRight = topLeft + 1;
                int bottomLeft = (i + 1) * (n + 1) + j;
                int bottomRight = bottomLeft + 1;

                // First triangle
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                // Second triangle
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        Mesh* gridMesh = new Mesh("terrain");
        gridMesh->InitFromData(vertices, indices);
        meshes[gridMesh->GetMeshID()] = gridMesh;
    }

    float rangeX = 45;
    float rangeZ = 45;

    // Generate random positions for trees within a defined range
    for (int i = 0; i < 100; ++i) {
        float randomX = (rand() % (int)(2 * rangeX * 100)) / 100.0f - rangeX;
        float randomZ = (rand() % (int)(2 * rangeZ * 100)) / 100.0f - rangeZ;

        treePositions.push_back(glm::vec3(randomX, 0.0f, randomZ));
    }

    for (int i = 0; i < numPackages; ++i) {
        float randomX1 = (rand() % (int)(2 * rangeX * 100)) / 100.0f - rangeX;
        float randomZ1 = (rand() % (int)(2 * rangeZ * 100)) / 100.0f - rangeZ;
        float randomX2 = (rand() % (int)(2 * rangeX * 100)) / 100.0f - rangeX;
        float randomZ2 = (rand() % (int)(2 * rangeZ * 100)) / 100.0f - rangeZ;

        packagePositions.emplace_back(glm::vec3(randomX1, 0.0f, randomZ1), glm::vec3(randomX2, 0.0f, randomZ2));
    }

    packagePos = packagePositions[0];

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 4.f, resolution.y / 4.f);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    if (!gameOver) {
        glClearColor(0.6, 0.8, 0.9, 1);
    }
    else {
        glClearColor(0, 0, 0, 1);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema2::RenderScene(float deltaTimeSeconds, bool showText)
{
    if (!gameOver) {

        if (showText) {
            float remainingTime = glm::max(0.0f, gameDuration - elapsedTime);

            // Display remaining time
            std::string timerText = "Time Left: " + std::to_string((int)remainingTime) + "s";
            textRenderer->RenderText(timerText, 10, 10, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

            if (!allPackages) {
                std::string packageText = "Number of packages delivered: " + std::to_string((int)packageNum);
                textRenderer->RenderText(packageText, 825, 10, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            }
            else {
                std::string packageText = "Number of packages delivered: " + std::to_string((int)packageNum + 1);
                textRenderer->RenderText(packageText, 825, 10, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                textRenderer->RenderText("YOU DELIVERED ALL PACKAGES!", 450, 100, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            }
        }

        // Update propeller rotation
        eliceAngularStep += deltaTimeSeconds * 15;

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        dronePosition = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);

        // Draw the drone arms (intersecting in an "X" shape)
        for (int i = 0; i < 2; ++i) {
            float angle = glm::radians(45.0f + i * 90.0f); // 45° and 135° for the body parts

            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(dronePosition.x, dronePosition.y, dronePosition.z);
            modelMatrix *= transform3D::RotateOY(angularStepOY);
            modelMatrix *= transform3D::RotateOY(angle);                // Rotate the body part
            modelMatrix *= transform3D::Scale(bodyScale.x, bodyScale.y, bodyScale.z);

            RenderMesh(meshes["cube"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.6f, 0.6f, 0.6f), false);
        }

        for (int i = 0; i < 4; ++i) {
            float angle = glm::radians(45.0f + i * 90.0f); // 45°, 135°, 225°, 315°
            float offsetX = cos(angle) * eliceOffset;
            float offsetZ = sin(angle) * eliceOffset;

            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(dronePosition.x, dronePosition.y, dronePosition.z);
            modelMatrix *= transform3D::RotateOY(angularStepOY);
            modelMatrix *= transform3D::Translate(offsetX, 0.05f, offsetZ); // Place arms
            modelMatrix *= transform3D::RotateOY(angle);
            modelMatrix *= transform3D::Scale(armScale.x, armScale.y, armScale.z);

            RenderMesh(meshes["cube"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.6f, 0.6f, 0.6f), false);
        }

        // Draw the drone propellers
        for (int i = 0; i < 4; ++i) {
            float angle = glm::radians(45.0f + i * 90.0f); // 45°, 135°, 225°, 315°
            float offsetX = cos(angle) * eliceOffset;
            float offsetZ = sin(angle) * eliceOffset;

            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(dronePosition.x, dronePosition.y, dronePosition.z);
            modelMatrix *= transform3D::RotateOY(angularStepOY);
            modelMatrix *= transform3D::Translate(offsetX, 0.108f, offsetZ); // Place propellers
            modelMatrix *= transform3D::RotateOY(eliceAngularStep);     // Rotate the propellers
            modelMatrix *= transform3D::Scale(eliceScale.x, eliceScale.y, eliceScale.z);

            RenderMesh(meshes["cube"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f), false);
        }

        // Draw the package
        {
            if (!allPackages) {
                if (hasPackage) {
                    // Draw package with the drone if drone is in package range
                    modelMatrix = glm::mat4(1);
                    modelMatrix *= transform3D::Translate(dronePosition.x, dronePosition.y, dronePosition.z);
                    modelMatrix *= transform3D::RotateOY(angularStepOY);
                    modelMatrix *= transform3D::Translate(0.0f, -0.3f, 0.0f);
                    modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
                    RenderMesh(meshes["cube"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.3f, 0.0f), false);

                    // Draw destination circle
                    modelMatrix = glm::mat4(1);
                    modelMatrix *= transform3D::Translate(packagePos.second.x, 0.7f, packagePos.second.z);
                    modelMatrix *= transform3D::Scale(2.5f, 2.5f, 2.5f);
                    RenderMesh(meshes["circle"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.0f, 0.0f), false);

                    // Draw destination arrow
                    modelMatrix = glm::mat4(1);
                    modelMatrix *= transform3D::Translate(packagePos.second.x, 6.0f, packagePos.second.z);
                    modelMatrix *= transform3D::RotateOX(glm::radians(180.0f));
                    modelMatrix *= transform3D::Scale(0.1f, 0.1f, 0.1f);
                    RenderMesh(meshes["cone"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.0f, 0.0f), false);

                    modelMatrix = glm::mat4(1);
                    modelMatrix *= transform3D::Translate(packagePos.second.x, 6.0f, packagePos.second.z);
                    modelMatrix *= transform3D::Scale(0.05f, 0.1f, 0.05f);
                    RenderMesh(meshes["cylinder"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.0f, 0.0f), false);

                    // Draw direction arrow
                    modelMatrix = glm::mat4(1);
                    modelMatrix *= transform3D::Translate(dronePosition.x, dronePosition.y, dronePosition.z - 0.75f);
                    modelMatrix *= transform3D::RotateOY(angle);
                    modelMatrix *= transform3D::RotateOX(glm::radians(90.0f));
                    modelMatrix *= transform3D::Scale(0.01f, 0.02f, 0.01f);
                    RenderMesh(meshes["cone"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.0f, 0.0f), false);
                }
                else {
                    // Draw package on the terrain if drone is not in package range
                    modelMatrix = glm::mat4(1);

                    modelMatrix *= transform3D::Translate(packagePos.first.x, 0.85f, packagePos.first.z);
                    modelMatrix *= transform3D::Scale(1.0f, 1.0f, 1.0f);
                    RenderMesh(meshes["cube"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.3f, 0.0f), false);
                }
            }
        }

        {
            // Draw the trees at their random positions
            for (size_t i = 0; i < treePositions.size(); ++i) {
                const glm::vec3& position = treePositions[i];

                // Draw tree trunk
                modelMatrix = glm::mat4(1);
                modelMatrix *= transform3D::Translate(position.x, 0.0f, position.z);
                modelMatrix *= transform3D::Scale(cylinderScale.x, cylinderScale.y, cylinderScale.z);
                RenderMesh(meshes["cylinder"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.3f, 0.2f, 0.0f), false);

                // Draw tree foliage
                for (int j = 0; j < 2; ++j) {
                    modelMatrix = glm::mat4(1);
                    modelMatrix *= transform3D::Translate(position.x, 0.0f, position.z);
                    modelMatrix *= transform3D::Translate(0.0f, 2.0f + j, 0.0f);
                    modelMatrix *= transform3D::Scale(coneScale.x, coneScale.y, coneScale.z);
                    RenderMesh(meshes["cone"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.2f + j * 0.05f, 0.0f), false);
                }
            }
        }

        // Draw the terrain
        {
            modelMatrix = glm::mat4(1);
            RenderMesh(meshes["terrain"], shaders["Tema2ShaderTerrain"], modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f), true);
        }
    }
    else 
    {
        if (showText) {
            textRenderer->RenderText("Game Over!", 400, 300, 3.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        }
    }
}


void Tema2::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    if (!gameOver) {
        elapsedTime += deltaTimeSeconds;

        if (elapsedTime >= gameDuration) {
            gameOver = true;
        }
    }

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderScene(deltaTimeSeconds, true);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    // Save the original camera state
    glm::vec3 originalPosition = camera->position;
    glm::vec3 originalForward = camera->forward;
    glm::vec3 originalUp = camera->up;
    glm::vec3 originalRight = camera->right;

    // Update the camera for the minimap
    camera->position = dronePosition + glm::vec3(0, 2, 0); // Position above the drone
    camera->forward = glm::normalize(glm::vec3(0, -1, 0)); // Look directly down
    camera->up = glm::vec3(0, 0, -1);                      // Up vector adjusted for top-down view
    camera->right = glm::cross(camera->forward, camera->up);

    // Render the scene for the minimap
    RenderScene(deltaTimeSeconds, false);

    direction = glm::normalize(packagePos.second - dronePosition);

    angle = atan2(direction.z, direction.x);
    angle = -(angle - glm::radians(90.0f));

    if (!allPackages && hasPackage) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(dronePosition.x, dronePosition.y, dronePosition.z - 0.75f);
        modelMatrix *= transform3D::RotateOY(angle);
        modelMatrix *= transform3D::RotateOX(glm::radians(90.0f));
        modelMatrix *= transform3D::Scale(0.01f, 0.02f, 0.01f);
        RenderMesh(meshes["cone"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.0f, 0.0f), false);
    }

    // Restore the original camera state
    camera->position = originalPosition;
    camera->forward = originalForward;
    camera->up = originalUp;
    camera->right = originalRight;
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& objectColor, bool useBlending)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();

    // Pass transformation matrices
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Pass color and blending mode
    if (useBlending) {
        // Terrain blending mode
        glUniform1i(glGetUniformLocation(shader->program, "use_height_blending"), 1);
        glUniform3f(glGetUniformLocation(shader->program, "color1"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shader->program, "color2"), 0.0f, 0.5f, 0.0f);
    }
    else {
        // Solid color mode for drone
        glUniform1i(glGetUniformLocation(shader->program, "use_height_blending"), 0);
        glUniform3f(glGetUniformLocation(shader->program, "solid_color"), objectColor.r, objectColor.g, objectColor.b);
    }

    // Render the mesh
    mesh->Render();
}


bool Tema2::CheckCollisionWithTree(glm::vec3 dronePosition, const std::vector<glm::vec3>& treePositions, glm::vec3& collisionNormal)
{
    collisionNormal = glm::vec3(0.0f);
    bool collisionDetected = false;

    // Drone parameters
    float droneRadius = 0.75f;
    float droneHeight = droneRadius * 2.0f;

    // Tree parameters
    float trunkRadius = 0.25f;
    float trunkHeight = 3.0f;

    float coneBaseRadius = 0.6f;
    float coneHeight = 1.0f;
    float foliageYOffset = 3.5f;

    for (const auto& treePos : treePositions) {
        bool localCollision = false;

        // Collision with the cylindrical trunk
        {
            // Check horizontal collision
            float distanceXZ = glm::length(glm::vec2(dronePosition.x, dronePosition.z) - glm::vec2(treePos.x, treePos.z));
            bool horizontalCollision = distanceXZ < (trunkRadius + droneRadius);

            // Check vertical collision
            bool verticalCollision = (dronePosition.y < (treePos.y + trunkHeight)) && (dronePosition.y + droneHeight > treePos.y);

            if (horizontalCollision && verticalCollision) {
                localCollision = true;
                glm::vec3 horizontalNormal = glm::normalize(glm::vec3(dronePosition.x, 0.0f, dronePosition.z) - glm::vec3(treePos.x, 0.0f, treePos.z));
                collisionNormal += horizontalNormal;
            }
        }

        // Collision with the cone foliage
        for (int j = 0; j < 2; ++j) {
            // Position of the current cone
            float coneBaseY = treePos.y + foliageYOffset + j * coneHeight;

            // Check vertical bounds of the cone
            if (dronePosition.y + droneHeight < coneBaseY || dronePosition.y > coneBaseY + coneHeight) {
                continue; // Drone is outside the vertical bounds of this cone
            }

            // Calculate the relative height within the cone
            float relativeHeight = (dronePosition.y - coneBaseY) / coneHeight;

            // Calculate the effective cone radius at the drone's height
            float effectiveRadius = coneBaseRadius * (1.0f - relativeHeight);

            // Check horizontal collision
            float distanceXZ = glm::length(glm::vec2(dronePosition.x, dronePosition.z) - glm::vec2(treePos.x, treePos.z));
            if (distanceXZ < (effectiveRadius + droneRadius)) {
                localCollision = true;
                glm::vec3 horizontalNormal = glm::normalize(glm::vec3(dronePosition.x, 0.0f, dronePosition.z) - glm::vec3(treePos.x, 0.0f, treePos.z));
                collisionNormal += horizontalNormal;
            }
        }

        // If there is a collision with this tree, set the global flag
        if (localCollision) {
            collisionDetected = true;
        }
    }

    if (collisionDetected) {
        collisionNormal = glm::normalize(collisionNormal);
    }

    return collisionDetected;
}


bool Tema2::CheckCollisionWithPackage(std::pair<glm::vec3, glm::vec3 > packagePos)
{
    float packageRadius = 1.0f;

    float packageDistanceXZ = glm::length(glm::vec2(dronePosition.x, dronePosition.z) - glm::vec2(packagePos.first.x, packagePos.first.z));

    if (packageDistanceXZ < packageRadius) {
        takePackage = true;
    }

    return takePackage;
}

bool Tema2::CheckCollisionWithDestination(std::pair<glm::vec3, glm::vec3 > packagePos)
{
    float destinationRadius = 1.0f;

    float destinationDistanceXZ = glm::length(glm::vec2(dronePosition.x, dronePosition.z) - glm::vec2(packagePos.second.x, packagePos.second.z));

    if (destinationDistanceXZ < destinationRadius) {
        dropPackage = true;
    }

    return dropPackage;
}


void Tema2::OnInputUpdate(float deltaTime, int mods) {
    // Calculate the desired movement (translation) for the Z axis
    if (window->KeyHold(GLFW_KEY_W)) {
        velocity.z = speed;
    }
    else if (window->KeyHold(GLFW_KEY_S)) {
        velocity.z = -speed;
    }
    else {
        velocity.z *= dampingFactor;  // Decelerate towards zero
    }

    // Calculate the desired movement for the X axis
    if (window->KeyHold(GLFW_KEY_A)) {
        velocity.x = -speed;
    }
    else if (window->KeyHold(GLFW_KEY_D)) {
        velocity.x = speed;
    }
    else {
        velocity.x *= dampingFactor;
    }

    // Calculate the desired movement for the Y axis
    if (window->KeyHold(GLFW_KEY_Q)) {
        velocity.y = -speed;
    }
    else if (window->KeyHold(GLFW_KEY_SPACE)) {
        velocity.y = speed;
    }
    else {
        velocity.y *= dampingFactor;
    }

    // Check collision and adjust movement
    if (CheckCollisionWithTree(dronePosition, treePositions, collisionNormal)) {
        // Resolve collision: slide along the tree surface
        glm::vec3 correctedVelocity = velocity - glm::dot(velocity, collisionNormal) * collisionNormal;
        camera->TranslateRight(correctedVelocity.x * deltaTime);
        camera->TranslateUpward(correctedVelocity.y * deltaTime);
        camera->MoveForward(correctedVelocity.z * deltaTime);
    }
    else {
        // No collision: move freely
        camera->TranslateRight(velocity.x * deltaTime);
        camera->TranslateUpward(velocity.y * deltaTime);
        camera->MoveForward(velocity.z * deltaTime);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_E)
    {
        takePackage = CheckCollisionWithPackage(packagePos);

        if (takePackage)
        {
            hasPackage = true;
            takePackage = false;
        }
    }
    if (key == GLFW_KEY_R)
    {
        if (hasPackage) {
            packagePos = packagePositions[packageNum];
            dropPackage = CheckCollisionWithDestination(packagePos);

            if (dropPackage) {
                if (packageNum < numPackages - 1) {
                    packageNum++;
                    std::cout << "Number of packages delivered: " << packageNum << std::endl;
                }
                else {
                    allPackages = true;
                    std::cout << "Number of packages delivered: " << packageNum + 1 << std::endl;
                    std::cout << "YOU DELIVERED ALL PACKAGES!" << std::endl;
                }
                packagePos = packagePositions[packageNum];
                hasPackage = false;
                dropPackage = false;
            }
        }
    }
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);

            angularStepOY += -deltaX * 0.001;
        }
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
