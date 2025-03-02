#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"

#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/lab_camera.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderScene(float deltaTimeSeconds, bool showText);
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, bool useBlending);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        bool CheckCollisionWithTree(glm::vec3 dronePosition, const std::vector<glm::vec3>& treePositions, glm::vec3& collisionNormal);
        bool CheckCollisionWithPackage(std::pair<glm::vec3, glm::vec3 > packagePos);
        bool CheckCollisionWithDestination(std::pair<glm::vec3, glm::vec3 > packagePos);

     protected:
        implemented::Camera* camera;
        gfxc::TextRenderer* textRenderer;
        Shader* shader;

        glm::mat4 modelMatrix;
        glm::mat4 projectionMatrix;

        // Positions
        glm::vec3 dronePosition;
        std::vector<glm::vec3> treePositions;

        // Drone parameters
        float angularStepOY = 0;
        float eliceAngularStep = 0;

        // Drone dimensions
        glm::vec3 bodyScale = glm::vec3(0.1f, 0.1f, 1.0f);
        glm::vec3 armScale = glm::vec3(0.1f, 0.1f, 0.1f);
        glm::vec3 eliceScale = glm::vec3(0.05f, 0.015f, 0.3f);
        float eliceOffset = 0.45f;

        // Tree dimensions
        glm::vec3 coneScale = glm::vec3(0.15f, 0.15f, 0.15f);
        glm::vec3 cylinderScale = glm::vec3(0.05f, 0.2f, 0.05f);

        // Drone movement
        float speed = 10.0f;
        glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        float dampingFactor = 0.9f;

        // Packages parameters
        float numPackages = 4;
        float packageNum = 0;
        std::vector<std::pair<glm::vec3, glm::vec3>> packagePositions;
        std::pair<glm::vec3, glm::vec3 > packagePos;
        bool hasPackage = false;
        bool takePackage = false;
        bool dropPackage = false;
        bool allPackages = false;

        // Collision
        glm::vec3 collisionNormal;

        // Direction arrow
        glm::vec3 direction;
        float angle;

        // Time parameters
        float elapsedTime = 0.0f;
        float gameDuration = 90.0f;
        bool gameOver = false;
        
        GLenum polygonMode;
        ViewportArea miniViewportArea;
        float angY = 0;
    };
}   // namespace m1
