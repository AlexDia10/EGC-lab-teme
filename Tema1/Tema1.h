#pragma once

#include "components/simple_scene.h"

// Define the Projectile struct
struct Projectile {
    glm::vec2 position;
    glm::vec2 velocity;
    bool active;

    Projectile(const glm::vec2& pos, const glm::vec2& vel)
        : position(pos), velocity(vel), active(true) {}
};

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:

        glm::mat3 modelMatrix;
        glm::mat3 modelMatrix2;
        glm::mat3 lineModelMatrix;
        glm::mat3 projectileMatrix;
        glm::mat3 healthBarMatrix;

        // terrain
        std::vector<float> heightMap;
        int numPoints = 2000;
        float x, y;

        float xA, xB, yA, yB;
        float translateX, translateY;
        float scaleX, scaleY;
        float shear;
        
        // tanks
        float scale = 1.5f;
        float gunLength;
        float tankHeight;

        // tank 1
        float tankX = 50.0f; // First tank initial position
        float tankY, tankRotation;

        // 180(pi)____|____0(2*pi)
        float gunRotation = static_cast<float>(M_PI)* (1.0f - 172.0f / 180.0f); // 8 grade

        // tank 2
        float tank2X = 1250.0f; // Second tank initial position
        float tank2Y, tank2Rotation;
        float gun2Rotation = 160.0f * static_cast<float>(M_PI) / 180.0f; // 160 grade

        // all projectiles
        std::vector<Projectile> projectiles;
        bool projectileActive = false;

        // projectile for tank 1
        glm::vec2 projectilePosition;
        glm::vec2 projectileVelocity;

        // projectile for tank 2
        glm::vec2 projectilePosition2;
        glm::vec2 projectileVelocity2;

        // trajectory
        float initX, initY;
        float init2X, init2Y;

        float timeStep = 0.1f;
        float totalTime = 10.0f;

        // Tank collision
        int tank1Life = 100;
        int tank2Life = 100;

        float tankRadius = 20.0f * scale;
        float projectileRadius = 5.0f;

        // Terrain collision
        float collisionThreshold = 3.0f;
        float explosionRadius = 75.0f;

        // Terrain slip
        float heightDifferenceThreshold = 3.0f;
        float transferAmount = 30.0f;

        float gravity = 150.0f;
        float magnitude = 150.0f;

    };
}   // namespace m1
