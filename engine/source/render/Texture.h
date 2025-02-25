#pragma once
#include <memory>
#include <string>

#include "mathlib.h"

namespace RayTracer {
class SpatialTexture {
   public:
    virtual Vector3 Value(Vector2 uv, Vector3 p) const = 0;
};

class Texture {
   public:
    Texture(int width, int height);
    Texture(int width, int height, std::unique_ptr<Color[]> pBuffer,
            int channels);

    unsigned int GetWidth() const { return width; }
    unsigned int GetHeight() const { return height; }

    void PutPixel(unsigned int x, unsigned int y, Color c);
    Color GetPixel(unsigned int x, unsigned int y) const;

    Color* GetData() const { return pBuffer.get(); }
    static std::shared_ptr<Texture> LoadFrom(const std::string name);

   protected:
    std::unique_ptr<Color[]> pBuffer;
    unsigned int width;
    unsigned int height;
    unsigned int pitch;
    int channels;
};

class SolidTexture : public SpatialTexture {
   public:
    SolidTexture(Vector3 c) : color(c) {}
    Vector3 Value(Vector2 uv, Vector3 p) const override { return color; }

   private:
    Vector3 color;
};

class TexChecker : public SpatialTexture {
   public:
    TexChecker(float _scale, std::shared_ptr<SpatialTexture> _even,
               std::shared_ptr<SpatialTexture> _odd)
        : inv_scale(1.0f / _scale), even(_even), odd(_odd) {}

    TexChecker(float _scale, Vector3 c1, Vector3 c2)
        : inv_scale(1.0f / _scale),
          even(std::make_shared<SolidTexture>(c1)),
          odd(std::make_shared<SolidTexture>(c2)) {}

    Vector3 Value(Vector2 uv, Vector3 p) const override {
        auto xInteger = static_cast<int>(std::floor(inv_scale * p.x));
        auto yInteger = static_cast<int>(std::floor(inv_scale * p.y));
        auto zInteger = static_cast<int>(std::floor(inv_scale * p.z));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->Value(uv, p) : odd->Value(uv, p);
    }

   private:
    float inv_scale;
    std::shared_ptr<SpatialTexture> even;
    std::shared_ptr<SpatialTexture> odd;
};

class ImageTexture : public SpatialTexture {
   public:
    ImageTexture(std::shared_ptr<Texture> tex) : pTex(tex) {}
    ImageTexture(std::string filepath) : pTex(Texture::LoadFrom(filepath)) {}
    Vector3 Value(Vector2 uv, Vector3 p) const override {
        if (pTex) {
            auto u = uv.x - std::floor(uv.x);
            auto v = 1.0f - (uv.y - std::floor(uv.y));

            auto i = static_cast<unsigned int>(u * pTex->GetWidth());
            auto j = static_cast<unsigned int>(v * pTex->GetHeight());
            if (i >= pTex->GetWidth()) i = pTex->GetWidth() - 1;
            if (j >= pTex->GetHeight()) j = pTex->GetHeight() - 1;
            return pTex->GetPixel(i, j).ToVector3();
        }

        return Vector3(0.0f, 1.0f, 1.0f);
    }

   private:
    std::shared_ptr<Texture> pTex{nullptr};
};

class NoiseTexture : public SpatialTexture {
   public:
    NoiseTexture() : scale(1.0f) {}
    NoiseTexture(float sc) : scale(sc) {}
    Vector3 Value(Vector2 uv, Vector3 p) const override {
        auto s = scale * p;
        return Vector3(1.0f, 1.0f, 1.0f) * 0.5 *
               (1.0f + sinf(s.z + 10.0f * perlin.Turb(s)));

        // return Vector3(1.0f, 1.0f, 1.0f) * 0.5f * (1.0f + perlin.Noise(scale
        // * p));
    }

   private:
    float scale;
    Perlin perlin;
};

}  // namespace RayTracer