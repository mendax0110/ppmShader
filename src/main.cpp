#include "../include/shader.h"
#include <cmath>
#include <chrono>
#include <iostream>

enum class Scene
{
    Tokamak,
    HelixNebula
};

int main(int argc, char** argv)
{
    int W = 640;
    int H = 360;
    int FPS = 30;
    int FRAMES = 300;

    if (argc >= 3)
    {
        W = std::atoi(argv[1]);
        H = std::atoi(argv[2]);
    }
    if (argc >= 4)
    {
        FRAMES = std::atoi(argv[3]);
    }
    if (argc >= 5)
    {
        FPS = std::atoi(argv[4]);
    }
    Scene scene = Scene::Tokamak;

    if (argc >= 6)
    {
        std::string arg = argv[5];
        if (arg == "helix" || arg == "Helix") scene = Scene::HelixNebula;
        else scene = Scene::Tokamak;
    }


    Shader shader(W, H, FPS);

    auto tokamakShader = ([W, H](float x, float y, float z) -> Vec3
    {
        float cx = (x - 0.5f) * 2.0f;
        float cy = (y - 0.5f) * 2.0f;

        // distance
        float r = std::sqrt(cx*cx + cy*cy);

        // Torus parameter
        const float R = 0.55f;
        const float T = 0.15f;

        // Distance from torus surface
        float d = std::abs(r - R) / T;
        float intensity = std::exp(-d * 4.0f);

        float p = 0.0f;
        p += std::sin(12.0f * cx + z * 4.0f);
        p += std::sin(12.0f * cy + z * 3.0f);
        p += 0.5f * std::sin(20.0f * r + z * 6.0f + std::sin(z * 2.0f));
        p += 0.25f * std::sin(40.0f * (cx + cy) + z * 10.0f);
        p *= 0.25f; // normaoze the turbulence

        float flicker = 0.5f + 0.5f * std::sin(z * 7.0f + r * 12.0f);
        float glow = intensity * (1.0f + p) * flicker;

        // purple color like a JET mode
        float rcol = glow * 0.7f;
        float gcol = glow * 0.2f;
        float bcol = glow * 1.5f;


        rcol = std::clamp(rcol, 0.0f, 1.0f);
        gcol = std::clamp(gcol, 0.0f, 1.0f);
        bcol = std::clamp(bcol, 0.0f, 1.0f);

        //radial falloff
        float vign = 1.0f - std::pow(r * 0.9f, 2.5f);
        vign = std::max(0.0f, vign);

        return {rcol * vign, gcol * vign, bcol * vign};
    });

    auto HelixNebulaShader = [W, H](float x, float y, float z) -> Vec3
    {
        float cx = (x - 0.5f) * 2.0f;
        float cy = (y - 0.5f) * 2.0f;

        //to polar coordinates
        float r = std::sqrt(cx*cx + cy*cy);
        float angle = std::atan2(cy, cx);

        float ring1 = std::exp(-std::pow(r - 0.4f, 2) * 60.0f);
        float ring2 = std::exp(-std::pow(r - 0.6f, 2) * 45.0f);

        float filaments = std::exp(-std::pow(r - 0.5f, 2) * 35.0f) * (0.8f + 0.4f * std::sin(angle * 24.0f + z * 0.5f));
        float centralStar = std::exp(-r * 20.0f);

        float halo = std::exp(-std::pow(r, 2) * 8.0f) * 0.3f;
        float knots = std::exp(-std::pow(r - 0.55f, 2) * 50.0f) * std::max(0.0f, std::sin(angle * 16.0f + z * 0.3f)) * 0.6f;

        float starNoise = (std::sin(150.0f * x + z * 2.0f) * std::sin(130.0f * y + z * 1.5f) > 0.999f) ? 1.0f : 0.0f;

        float R = ring2 * 1.4f + filaments * 0.7f + knots * 0.9f + starNoise;
        float G = ring1 * 1.2f + filaments * 1.1f + centralStar * 0.8f + halo* 0.5f + starNoise;
        float B = ring1 * 1.6f + centralStar * 1.3f + halo * 0.7f + starNoise;

        float pulse = 0.95f + 0.05f * std::sin(z * 1.5f);
        R *= pulse;
        G *= pulse;
        B *= pulse;

        float vign = std::max(0.0f, 1.0f - std::pow(r * 0.85f, 3.0f));
        R *= vign;
        G *= vign;
        B *= vign;

        R = std::clamp(R, 0.0f, 1.0f);
        G = std::clamp(G, 0.0f, 1.0f);
        B = std::clamp(B, 0.0f, 1.0f);

        return {R, G, B};
    };

    try
    {
        if (scene == Scene::Tokamak)
        {
            shader.setShaderFunction(tokamakShader);
        }
        else
        {
            shader.setShaderFunction(HelixNebulaShader);
        }

        shader.renderFrames(FRAMES);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
