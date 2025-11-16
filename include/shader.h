#ifndef PPMSHADER_SHADER_H
#define PPMSHADER_SHADER_H

#include <functional>
#include <cstdint>
#include <vector>
#include <iostream>

struct Vec3
{
    float r, g, b;
    Vec3() : r(0), g(0), b(0) {}
    Vec3(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
};

class Shader
{
public:
    using ShaderFunc = std::function<Vec3(float, float, float)>;

    Shader(int width, int height, int fps = 30);
    ~Shader();

    void setShaderFunction(ShaderFunc func);
    void renderFrames(int frames, float durationSeconds = 0.0f);

    int width() const { return width_; }
    int height() const { return height_; }
    int fps() const { return fps_; }

private:
    int width_;
    int height_;
    int fps_;
    ShaderFunc shaderFunc_;

    static inline uint8_t floatToByte(float value);
    void writePPMHeader(std::ostream &out) const;
};


#endif //PPMSHADER_SHADER_H
