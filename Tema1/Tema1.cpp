#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    for (int i = 0; i < numPoints; ++i) {
        x = i;
        y = 300
            + 50 * sin(0.0075 * x)
            + 75 * sin(0.00015 * x)
            + 25 * sin(0.03 * x);

        heightMap.push_back(y);
    }

    for (int i = 0; i < heightMap.size() - 1; ++i) {
        Mesh* square = (object2D::CreateSquare("square" + std::to_string(i), glm::vec3(0, 0, 0), 1, glm::vec3(1, 0.7, 0.4), true));
        meshes["square" + std::to_string(i)] = square;
    }

    // Create tanks
    Mesh* tankGun = object2D::CreateRectangle("tankGun", glm::vec3(0.0f, 0.0f, 0.0f), 20.0f * scale, 3.0f * scale, glm::vec3(0.1f, 0.1f, 0.1f), true);
    meshes["tankGun"] = tankGun;

    Mesh* tankTop = object2D::CreateCircle("tankTop", glm::vec3(0.0f, 13.0f * scale, 0.0f), 10.0f * scale, glm::vec3(0.5f, 0.0f, 1.0f), 30);
    meshes["tankTop"] = tankTop;
    Mesh* tankTop2 = object2D::CreateCircle("tankTop2", glm::vec3(0.0f, 13.0f * scale, 0.0f), 10.0f * scale, glm::vec3(1.0f, 0.0f, 0.5f), 30);
    meshes["tankTop2"] = tankTop2;

    Mesh* tankBody = object2D::CreateTrapezoid("tankBody", glm::vec3(0.0f, 5.0f * scale, 0.0f), 50.0f * scale, 37.5f * scale, 10.0f * scale, glm::vec3(0.5f, 0.0f, 1.0f), true);
    meshes["tankBody"] = tankBody;
    Mesh* tankBody2 = object2D::CreateTrapezoid("tankBody2", glm::vec3(0.0f, 5.0f * scale, 0.0f), 50.0f * scale, 37.5f * scale, 10.0f * scale, glm::vec3(1.0f, 0.0f, 0.5f), true);
    meshes["tankBody2"] = tankBody2;

    Mesh* tankBase = object2D::CreateTrapezoid("tankBase", glm::vec3(0.0f, 0.0f, 0.0f), 30.0f * scale, 37.5f * scale, 5.0f * scale, glm::vec3(0.2f, 0.0f, 0.4f), true);
    meshes["tankBase"] = tankBase;
    Mesh* tankBase2 = object2D::CreateTrapezoid("tankBase2", glm::vec3(0.0f, 0.0f, 0.0f), 30.0f * scale, 37.5f * scale, 5.0f * scale, glm::vec3(0.4f, 0.0f, 0.2f), true);
    meshes["tankBase2"] = tankBase2;

    // Create projectile
    Mesh* projectile = object2D::CreateCircle("projectile", glm::vec3(0.0f, 0.0f, 0.0f), 5.0f, glm::vec3(0.1f, 0.1f, 0.1f), 30);
    meshes["projectile"] = projectile;

    // Create trajectory
    Mesh* trajectoryLine = object2D::CreateRectangle("trajectoryLine", glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 2.0f, glm::vec3(1.0f, 1.0f, 1.0f), true);
    meshes["trajectoryLine"] = trajectoryLine;

    // Create health bar
    Mesh* healthBarFrame = object2D::CreateRectangle("healthBarFrame", glm::vec3(0.0f, 0.0f, 0.0f), 50.0f, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f), false);
    meshes["healthBarFrame"] = healthBarFrame;

    Mesh* healthBarFilled = object2D::CreateRectangle("healthBarFilled", glm::vec3(0.0f, 0.0f, 0.0f), 50.0f, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f), true);
    meshes["healthBarFilled"] = healthBarFilled;

    // Create cloud
    Mesh* cloudPart1 = object2D::CreateCircle("cloud1", glm::vec3(0.0f, 0.0f, 0.0f), 20.0f, glm::vec3(1.0f, 1.0f, 1.0f), 30);
    meshes["cloud1"] = cloudPart1;

    Mesh* cloudPart2 = object2D::CreateCircle("cloud2", glm::vec3(0.0f, 0.0f, 0.0f), 30.0f, glm::vec3(1.0f, 1.0f, 1.0f), 30);
    meshes["cloud2"] = cloudPart2;

    Mesh* cloudPart3 = object2D::CreateCircle("cloud3", glm::vec3(0.0f, 0.0f, 0.0f), 25.0f, glm::vec3(1.0f, 1.0f, 1.0f), 30);
    meshes["cloud3"] = cloudPart3;

    // Create sun
    Mesh* sun = object2D::CreateCircle("sun", glm::vec3(0.0f, 0.0f, 0.0f), 35.0f, glm::vec3(1.0f, 1.0f, 0.0f), 30);
    meshes["sun"] = sun;
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render clouds in the sky
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(500.0f, 650.0f);
    RenderMesh2D(meshes["cloud1"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(40.0f, 0.0f);
    RenderMesh2D(meshes["cloud2"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(40.0f, 0.0f);
    RenderMesh2D(meshes["cloud3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1000.0f, 550.0f);
    RenderMesh2D(meshes["cloud1"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(40.0f, 0.0f);
    RenderMesh2D(meshes["cloud2"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(40.0f, 0.0f);
    RenderMesh2D(meshes["cloud3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(200.0f, 600.0f);
    RenderMesh2D(meshes["cloud1"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(40.0f, 0.0f);
    RenderMesh2D(meshes["cloud2"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(40.0f, 0.0f);
    RenderMesh2D(meshes["cloud3"], shaders["VertexColor"], modelMatrix);

    // Render sun in the sky
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(750.0f, 600.0f);
    RenderMesh2D(meshes["sun"], shaders["VertexColor"], modelMatrix);

    // Terrain rendering
    for (int i = 0; i < heightMap.size() - 1; ++i) {
        xA = i;                         // Calculate x for point A
        yA = heightMap[i];              // Get y for point A from heightMap

        xB = (i + 1);                   // Calculate x for point B
        yB = heightMap[i + 1];          // Get y for point B from heightMap

        modelMatrix = glm::mat3(1);

        translateX = xA;
        translateY = yA;
        modelMatrix *= transform2D::Translate(translateX, translateY);

        shear = (yB - yA) / (xB - xA);
        modelMatrix *= transform2D::Shear(shear);

        scaleX = xB - xA;
        scaleY = std::max(yA, yB);
        modelMatrix *= transform2D::Scale(scaleX, scaleY);

        RenderMesh2D(meshes["square" + std::to_string(i)], shaders["VertexColor"], modelMatrix);
    }

    // Terrain SLIP
    std::vector<float> smoothedHeightMap = heightMap;
    std::vector<float> gaussianKernel = { 0.25f, 0.5f, 0.25f };

    for (int i = 1; i < heightMap.size() - 1; ++i) {
        float d = abs(heightMap[i] - heightMap[i + 1]);

        if (d > heightDifferenceThreshold) {
            float transfer = transferAmount * deltaTimeSeconds;

            float adjustment;
            if (heightMap[i] > heightMap[i + 1]) {
                adjustment = -transfer;
            }
            else {
                adjustment = transfer;
            }

            smoothedHeightMap[i - 1] += gaussianKernel[0] * adjustment;
            smoothedHeightMap[i] += gaussianKernel[1] * adjustment;
            smoothedHeightMap[i + 1] += gaussianKernel[2] * adjustment;
        }
    }

    // Update original heightMap after smoothing
    heightMap = smoothedHeightMap;

    // Calculate rotation for tank 1
    float index = tankX;
    if (index < heightMap.size() - 1) {
        float xA = index;
        float yA = heightMap[index];
        float xB = (index + 1);
        float yB = heightMap[index + 1];

        float t = (tankX - xA) / (xB - xA);
        tankY = yA + t * (yB - yA);
        tankRotation = atan2(yB - yA, xB - xA);
    }

    // Calculate rotation for tank 2
    float index2 = tank2X;
    if (index2 < heightMap.size() - 1) {
        float x2A = index2;
        float y2A = heightMap[index2];
        float x2B = (index2 + 1);
        float y2B = heightMap[index2 + 1];

        float t2 = (tank2X - x2A) / (x2B - x2A);
        tank2Y = y2A + t2 * (y2B - y2A);
        tank2Rotation = atan2(y2B - y2A, x2B - x2A);
    }

    // Render tank 1
    if (tank1Life > 0) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tankX, tankY);
        modelMatrix *= transform2D::Rotate(tankRotation);
        RenderMesh2D(meshes["tankBase"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["tankBody"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["tankTop"], shaders["VertexColor"], modelMatrix);

        // Set a limit for the gun rotation
        gunRotation = glm::clamp(gunRotation, static_cast<float>(M_PI) * (1.0f - 172.0f / 180.0f), 160.0f * static_cast<float>(M_PI) / 180.0f);

        modelMatrix *= transform2D::Translate(0.0f, 15.0f * scale); // Move the gun to the top of the tank
        modelMatrix *= transform2D::Rotate(gunRotation);
        RenderMesh2D(meshes["tankGun"], shaders["VertexColor"], modelMatrix);

        // Tip of the gun
        modelMatrix *= transform2D::Translate(20.0f * scale, 0.0f);
    }

    // Render tank 2
    if (tank2Life > 0) {
        modelMatrix2 = glm::mat3(1);
        modelMatrix2 *= transform2D::Translate(tank2X, tank2Y);
        modelMatrix2 *= transform2D::Rotate(tank2Rotation);
        RenderMesh2D(meshes["tankBase2"], shaders["VertexColor"], modelMatrix2);
        RenderMesh2D(meshes["tankBody2"], shaders["VertexColor"], modelMatrix2);
        RenderMesh2D(meshes["tankTop2"], shaders["VertexColor"], modelMatrix2);

        gun2Rotation = glm::clamp(gun2Rotation, static_cast<float>(M_PI) * (1.0f - 172.0f / 180.0f), 160.0f * static_cast<float>(M_PI) / 180.0f);

        modelMatrix2 *= transform2D::Translate(0.0f, 15.0f * scale);
        modelMatrix2 *= transform2D::Rotate(gun2Rotation);
        RenderMesh2D(meshes["tankGun"], shaders["VertexColor"], modelMatrix2);
        modelMatrix2 *= transform2D::Translate(20.0f * scale, 0.0f);
    }

    // TRAJECTORY for tank 1
    std::vector<glm::vec2> trajectoryPoints;

    // [scaleX    shearX   translateX]
    // [shearY    scaleY   translateY]
    // [0           0          1     ]

    initX = modelMatrix[2].x;
    initY = modelMatrix[2].y;

    glm::vec2 initialPosition(initX, initY);
    glm::vec2 velocity(cos(gunRotation + tankRotation), sin(gunRotation + tankRotation));
    velocity *= magnitude;

    for (float t = 0; t < totalTime; t += timeStep) {
        glm::vec2 position = initialPosition + velocity * t + 0.5f * glm::vec2(0, -gravity) * t * t;
        trajectoryPoints.push_back(position);

        if (position.y < 0) break;
    }

    // Render trajectory as a series of connected rectangle segments
    for (size_t i = 0; i < trajectoryPoints.size() - 1; ++i) {
        lineModelMatrix = glm::mat3(1);
        lineModelMatrix *= transform2D::Translate(trajectoryPoints[i].x, trajectoryPoints[i].y);

        glm::vec2 lineEnd = trajectoryPoints[i + 1] - trajectoryPoints[i];
        float lineLength = glm::length(lineEnd);
        float angle = atan2(lineEnd.y, lineEnd.x);

        lineModelMatrix *= transform2D::Rotate(angle);
        lineModelMatrix *= transform2D::Scale(lineLength, 2.0f);

        if (tank1Life > 0) {
            RenderMesh2D(meshes["trajectoryLine"], shaders["VertexColor"], lineModelMatrix);
        }
    }

    // TRAJECTORY for tank 2
    std::vector<glm::vec2> trajectoryPoints2;

    init2X = modelMatrix2[2].x;
    init2Y = modelMatrix2[2].y;

    glm::vec2 initialPosition2(init2X, init2Y);
    glm::vec2 velocity2(cos(gun2Rotation + tank2Rotation), sin(gun2Rotation + tank2Rotation));
    velocity2 *= magnitude;

    for (float t = 0; t < totalTime; t += timeStep) {
        glm::vec2 position2 = initialPosition2 + velocity2 * t + 0.5f * glm::vec2(0, -gravity) * t * t;
        trajectoryPoints2.push_back(position2);

        if (position2.y < 0) break;
    }

    // Render trajectory as a series of connected rectangle segments
    for (size_t i = 0; i < trajectoryPoints2.size() - 1; ++i) {
        lineModelMatrix = glm::mat3(1);
        lineModelMatrix *= transform2D::Translate(trajectoryPoints2[i].x, trajectoryPoints2[i].y);

        glm::vec2 lineEnd2 = trajectoryPoints2[i + 1] - trajectoryPoints2[i];
        float lineLength2 = glm::length(lineEnd2);
        float angle2 = atan2(lineEnd2.y, lineEnd2.x);

        lineModelMatrix *= transform2D::Rotate(angle2);
        lineModelMatrix *= transform2D::Scale(lineLength2, 2.0f);

        if (tank2Life > 0) {
            RenderMesh2D(meshes["trajectoryLine"], shaders["VertexColor"], lineModelMatrix);
        }
    }

    // Update and render all projectiles
    for (auto& projectile : projectiles) {
        if (projectile.active) {
            projectile.position += projectile.velocity * deltaTimeSeconds;
            projectile.velocity.y -= gravity * deltaTimeSeconds;

            projectileMatrix = glm::mat3(1);
            projectileMatrix *= transform2D::Translate(projectile.position.x, projectile.position.y);
            RenderMesh2D(meshes["projectile"], shaders["VertexColor"], projectileMatrix);
        }
    }

    // Projectile-tank collision
    for (auto& projectile : projectiles) {
        if (projectile.active) {
            float distanceToTank1 = glm::length(projectile.position - glm::vec2(tankX, tankY));
            if (distanceToTank1 <= tankRadius + projectileRadius) {
                projectile.active = false;
                tank1Life = std::max(0, tank1Life - 10);
            }
            float distanceToTank2 = glm::length(projectile.position - glm::vec2(tank2X, tank2Y));
            if (distanceToTank2 <= tankRadius + projectileRadius) {
                projectile.active = false;
                tank2Life = std::max(0, tank2Life - 10);
            }
        }
    }

    // Projectile-terrain collision
    for (auto& projectile : projectiles) {
        if (projectile.position.x >= 0 && projectile.position.x < numPoints) {

            float index = static_cast<int>(projectile.position.x);

            if (index < heightMap.size() - 1) {
                xA = index;
                xB = (index + 1);

                yA = heightMap[index];
                yB = heightMap[index + 1];

                float t = (projectile.position.x - xA) / (xB - xA);
                float yTerrain = yA * (1 - t) + yB * t;

                if (std::abs(projectile.position.y - yTerrain) < collisionThreshold) {
                    projectile.active = false;

                    for (int i = 0; i < heightMap.size(); i++) {
                        float dist = sqrt((i - projectile.position.x) * (i - projectile.position.x) +
                            (heightMap[i] - projectile.position.y) * (heightMap[i] - projectile.position.y));
                        if (dist <= explosionRadius) {
                            float deformation = sqrt(explosionRadius * explosionRadius - dist * dist);
                            heightMap[i] = std::max(0.0f, heightMap[i] - deformation);
                        }
                    }

                    // Post-explosion smoothing
                    std::vector<float> smoothedHeightMap = heightMap;

                    int smoothingRadius = 5.0f;

                    for (int i = smoothingRadius; i < heightMap.size() - smoothingRadius; i++) {
                        float average = 0.0f;
                        for (int j = -smoothingRadius; j <= smoothingRadius; j++) {
                            average += heightMap[i + j];
                        }
                        smoothedHeightMap[i] = average / (2 * smoothingRadius + 1);
                    }
                    heightMap = smoothedHeightMap;
                }
            }
        }
    }

    // Remove inactive projectiles
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& p) { return !p.active; }),
        projectiles.end()
    );

    // Render HEALTH BAR
    float barWidth = 50.0f;
    float barHeight = 10.0f;

    float healthRatio1 = (float)tank1Life / 100.0f;
    float healthRatio2 = (float)tank2Life / 100.0f;

    // Tank 1 health bar
    if (tank1Life > 0) {
        healthBarMatrix = glm::mat3(1);
        healthBarMatrix *= transform2D::Translate(tankX - barWidth / 2, tankY + 80);
        RenderMesh2D(meshes["healthBarFrame"], shaders["VertexColor"], healthBarMatrix);

        healthBarMatrix *= transform2D::Scale(healthRatio1, 1.0f);
        RenderMesh2D(meshes["healthBarFilled"], shaders["VertexColor"], healthBarMatrix);
    }

    // Tank 2 health bar
    if (tank2Life > 0) {
        healthBarMatrix = glm::mat3(1);
        healthBarMatrix *= transform2D::Translate(tank2X - barWidth / 2, tank2Y + 80);
        RenderMesh2D(meshes["healthBarFrame"], shaders["VertexColor"], healthBarMatrix);

        healthBarMatrix *= transform2D::Scale(healthRatio2, 1.0f);
        RenderMesh2D(meshes["healthBarFilled"], shaders["VertexColor"], healthBarMatrix);
    }
}


void Tema1::FrameEnd()
{
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // Move tank left and right
    if (window->KeyHold(GLFW_KEY_D)) {
        tankX += 100 * deltaTime; // Move right
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        tankX -= 100 * deltaTime; // Move left
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        gunRotation += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        gunRotation -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        tank2X += 100 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        tank2X -= 100 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        gun2Rotation -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        gun2Rotation += deltaTime;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) {

        projectilePosition = glm::vec2(initX, initY);
        projectileVelocity = glm::vec2(cos(gunRotation + tankRotation), sin(gunRotation + tankRotation));
        projectileVelocity *= magnitude;

        projectiles.emplace_back(projectilePosition, projectileVelocity);
    }
    if (key == GLFW_KEY_ENTER) {

        projectilePosition2 = glm::vec2(init2X, init2Y);
        projectileVelocity2 = glm::vec2(cos(gun2Rotation + tank2Rotation), sin(gun2Rotation + tank2Rotation));
        projectileVelocity2 *= magnitude;

        projectiles.emplace_back(projectilePosition2, projectileVelocity2);
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
