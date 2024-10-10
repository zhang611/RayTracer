#pragma once
#include "mathlib.h"
#include "Hittable.h"
#include <memory>

namespace RayTracer
{
    class Ray;
    class HitResult;
    
    class Pdf
    {
        public:
        virtual ~Pdf() {}

        virtual float Value(const Vector3& direction) const = 0;
        virtual Vector3 generate() const = 0;
    };

    class SpherePdf : public Pdf {
    public:
        SpherePdf() { }

        float Value(const Vector3& direction) const override {
            return 1.0f / (4.0f * PI);
        }

        Vector3 generate() const override {
            return Vector3::RandomUnitSphere();
        }
    };

    class CosinePdf : public Pdf {
    public:
        CosinePdf(const Vector3& n) : normal(n) {
            normal.tangentSpace(tangent, bitangent);
        }

        float Value(const Vector3& direction) const override {
            float cosine = direction.normalized().dot(normal);
            return fmaxf(0, cosine / PI);
        }

        Vector3 generate() const override {
            auto local_dir = Random::RandomCosineDirection();
            return tangent * local_dir.x + bitangent * local_dir.y + normal * local_dir.z;
        }

        private:
            Vector3 tangent;
            Vector3 bitangent;
            Vector3 normal;
    };

    class HittablePdf : public Pdf {
    public:
        HittablePdf(std::shared_ptr<Hittable> obj, const Vector3& _origin) : hitObj(obj), origin(_origin) {}

        float Value(const Vector3& direction) const override {
            return hitObj->pdfValue(origin, direction);
        }

        Vector3 generate() const override {
            return hitObj->random(origin);
        }

        private:
            std::shared_ptr<Hittable> hitObj;
            Vector3 origin;
    };

    class MixturePdf : public Pdf {
    public:
        MixturePdf(std::shared_ptr<Pdf> p0, std::shared_ptr<Pdf> p1) : pdf0(p0), pdf1(p1) {}

        float Value(const Vector3& direction) const override {
            return 0.5f * pdf0->Value(direction) + 0.5f * pdf1->Value(direction);
        }

        Vector3 generate() const override {
            if(Random::Random01() < 0.5f)
                return pdf0->generate();
            else
                return pdf1->generate();
        }

        private:
            std::shared_ptr<Pdf> pdf0;
            std::shared_ptr<Pdf> pdf1;
    };
}