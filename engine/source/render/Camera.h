#pragma once
#include "mathlib.h"
#include <memory>

namespace RayTracer
{
    class Hittable;
    class Texture;
    class Ray;

    /***
     * @brief Camera
     * Job:
     *  1. Construct and dispatch rays into the world.
     *  2. Use the results of these rays to construct the rendered image.
    */
    class Camera
    {
        public:
            Camera(unsigned int width, unsigned int height);

            
            void SetPos(const Vector3 pos) { cameraPos = pos; }
            void SetLookAt(const Vector3 look) { lookat = look; }
            void SetFov(float fov) { this->fov = fov; }
            void Resize(unsigned int width, unsigned int height);

            Texture* Render(const Hittable& world, unsigned int samples_per_pixel = 1);
            
            Vector3 forward() const { return (lookat - cameraPos).normalized(); }
            Vector3 right() const { 
                // left handed coordinate system
                return Vector3::Cross(Vector3::up, forward()).normalized();
            }
            Vector3 up() const { return Vector3::Cross(forward(), right()).normalized(); }
        protected:
            Vector3 ray_color(const Ray& r, const Hittable& world, int depth);
            Ray get_ray(int i, int j, int s_i, int s_j) const;
            Vector3 pixel_sample_square(int s_i, int s_j) const;
            Vector3 defocus_disk_sample() const;
            
            void Initialize();
        
        public:
            float defocus_angle = 0.0f; // Variation angle of rays through the each pixel
            float focus_dist = 10;      //  Distance from camera to plane of perfect focus
            Vector3 background;

        private:
            unsigned int width;
            unsigned int height;
            float aspect;

            float fov = 90.0f; // vertical field-of-view in degrees
            int    max_depth         = 10;   // Maximum number of ray bounces into scene

            Vector3 cameraPos;
            Vector3 lookat;

            float recip_sqrt_spp = 1.0f;
            Vector3 pixel_delta_u;
            Vector3 pixel_delta_v;
            Vector3 viewport_upper_left;
            Vector3 pixel00_loc;
            Vector3 defocus_disk_u; // horizontal vector of defocus disk
            Vector3 defocus_disk_v; // vertical vector of defocus disk

            std::shared_ptr<Texture> pRenderTex{nullptr};
    };
}