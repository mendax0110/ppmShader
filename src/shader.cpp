#include <utility>

#include "../include/shader.h"

Shader::Shader(int width, int height, int fps)
    : width_(width)
    , height_(height)
    , fps_(fps)
    , shaderFunc_(nullptr)
{
    if (width_ <= 0 || height_ <= 0)
    {
        std::string errMsg = "Invalid dimensions: " + std::to_string(width_) + "x" + std::to_string(height_);
        throw std::invalid_argument(errMsg);
    }
    if (fps_ <= 0)
    {
        fps_ = 30;
    }
}

Shader::~Shader()
{

}

void Shader::setShaderFunction(ShaderFunc func)
{
    shaderFunc_ = std::move(func);
}

uint8_t Shader::floatToByte(float value)
{
    value = std::clamp(value, 0.0f, 1.0f);
    return static_cast<uint8_t>(std::floor(value * 255.0f + 0.5f));
}

void Shader::writePPMHeader(std::ostream &out) const
{
    out << "P6\n" << width_ << " " << height_ << "\n255\n";
}

void Shader::renderFrames(int frames, float durationSeconds)
{
    if (!shaderFunc_)
    {
        throw std::runtime_error("Shader function not set.");
    }
    if (frames <= 0) return;

    float frameDuration = 1.0f / static_cast<float>(fps_);
    if (durationSeconds > 0.0f)
    {
        frameDuration = durationSeconds / static_cast<float>(frames);
    }

    std::ios::sync_with_stdio(false);
    std::cout.setf(std::ios::fmtflags(0), std::ios::floatfield);

    const int W = width_;
    const int H = height_;
    std::vector<uint8_t> row(W*3);

    for (int f = 0; f < frames; f++)
    {
        writePPMHeader(std::cout);
        float z = static_cast<float>(f) * frameDuration;

        for (int j = 0; j < H; j++)
        {
            int idx = 0;
            float y = (H == 1) ? 0.5f : (float(j) + 0.5f) / float(H);

            for (int i = 0; i < W; i++)
            {
                float x = (W == 1) ? 0.5f : (float(i) + 0.5f) / float(W);
                Vec3 c = shaderFunc_(x, y, z);
                row[idx++] = floatToByte(c.r);
                row[idx++] = floatToByte(c.g);
                row[idx++] = floatToByte(c.b);
            }
            std::cout.write(reinterpret_cast<char*>(row.data()), row.size());
        }
        std::cout.flush();
    }
}