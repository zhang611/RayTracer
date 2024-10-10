#include "Camera.h"
#include "Hittable.h"
#include "Texture.h"
#include <iostream>
#include "Material.h"
#include "pdf.h"
#include "Lights.h"
namespace RayTracer
{
    Camera::Camera(unsigned int width, unsigned int height) : width(width), height(height)
    {
        aspect = static_cast<float>(width) / height;
        pRenderTex = std::make_shared<Texture>(width, height);
        cameraPos = Vector3(0.0f, 0.0f, 0.0f);
        background = Vector3(0.70f, 0.80f, 1.00f);
    }

    
    void Camera::Resize(unsigned int width, unsigned int height)
    {
        aspect = static_cast<float>(width) / height;
        pRenderTex = std::make_shared<Texture>(width, height);
    }

    Texture* Camera::Render(const Hittable& world, unsigned int samples_per_pixel)
    {
        Initialize();

        
        auto sqrt_spp = static_cast<unsigned int>(sqrt(samples_per_pixel));
        samples_per_pixel = sqrt_spp * sqrt_spp;
        
        this->recip_sqrt_spp = 1 / static_cast<float>(sqrt_spp);
        for (unsigned int j = 0; j < height; ++j) {
        std::clog << "\rScanlines remaining: " << (height - j) << ' ' << std::flush;
            for (unsigned  i = 0; i < width; ++i) {
                auto pixel_center = pixel00_loc + (static_cast<float>(i) * pixel_delta_u) + (static_cast<float>(j) * pixel_delta_v);
                auto ray_direction = pixel_center - cameraPos;

                Vector3 pixel_color = Vector3::zero;
                // for (unsigned int sample = 0; sample < samples_per_pixel; ++sample) {
                //     Ray r =samples_per_pixel > 1? get_ray(i, j) : Ray(cameraPos, ray_direction);
                //     pixel_color += ray_color(r, world, max_depth);
                // }

                for (unsigned int s_j = 0; s_j < sqrt_spp; ++s_j) {
                    for (unsigned int s_i = 0; s_i < sqrt_spp; ++s_i) {
                        Ray r = get_ray(i, j, s_i, s_j);
                        pixel_color += ray_color(r, world, max_depth);
                    }
                }
                // if(pixel_color.x != pixel_color.x) pixel_color.x = 0.0f;
                // if(pixel_color.y != pixel_color.y) pixel_color.y = 0.0f;
                // if(pixel_color.z != pixel_color.z) pixel_color.z = 0.0f;

                pixel_color /= static_cast<float>(samples_per_pixel);
                pRenderTex->PutPixel(i, j, Color::FromVector3(pixel_color, true));
            }
        }
        return pRenderTex.get();
    }

    void Camera::Initialize()
    {
        //Camera
        // auto focal_length = (lookat - cameraPos).length();
        auto theta = DegreeToRadian(fov);
        auto h = tan(theta/2);

        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * aspect;
        
        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = right() * viewport_width;
        auto viewport_v =-up() * viewport_height;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / static_cast<float>(width);
        pixel_delta_v = viewport_v / static_cast<float>(height);

        // Calculate the location of the upper left pixel.
        viewport_upper_left = cameraPos + forward() * focus_dist - 0.5f * (viewport_u + viewport_v);
        pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

        //Calculate the defocus disk
        auto defocus_radius = focus_dist * tan(DegreeToRadian(defocus_angle) * 0.5f);
        defocus_disk_u = right() * defocus_radius;
        defocus_disk_v = up() * defocus_radius;
    }
    
    Vector3 Camera::ray_color(const Ray& r, const Hittable& world, int depth) 
    {
        if(depth <= 0) return Vector3::zero;
        HitResult result{};
        ScatterResult scatter{};
        if(!world.Hit(r, result)) return background;
        Ray scatteredRay;
        Vector3 emitted = result.hit_material->Emitted(r, result);
        
        float pdf{};
        if(!result.hit_material->Scatter(r, result, scatter)) 
            return emitted;
        
        if(scatter.skip_pdf) 
            return emitted + scatter.albedo * ray_color(scatter.scatter_ray, world, --depth);
        
        if(Lights::lights->objects.size() > 0)
        {
            MixturePdf mixed_pdf(std::make_shared<HittablePdf>(Lights::lights, result.hit_point), scatter.pdf);
            scatteredRay = Ray(result.hit_point, mixed_pdf.generate());     //get a random ray to the light                
            pdf = mixed_pdf.Value(scatteredRay.direction());  //get the pdf of the ray
        }else
        {
            scatteredRay = Ray(result.hit_point, scatter.pdf->generate());     //get a random ray to the light                
            pdf = scatter.pdf->Value(scatteredRay.direction());  //get the pdf of the ray
        }
        if(pdf < kEpsilon)
                return emitted;
        
        // very hard coded light source
        // auto on_light = Vector3(random_float(213.0f , 343.0f ), 554.0f , random_float(-332.0f, -227.0f));
        // auto to_light = on_light - result.hit_point;
        // auto distance_squared = to_light.lengthSqr();
        // to_light = to_light.normalized();

        // if(to_light.dot(result.normal) < 0) return emitted;

        // float areaSize = (343.0f - 213.0f) * (332.0f - 227.0f);
        // float light_cosine = fabs(to_light.y);
        // if(light_cosine < 0.000001f) return emitted;

        // float pdf = distance_squared / (light_cosine * areaSize);
        // auto scatteredRay = Ray(result.hit_point, to_light);

        

        float brdf = result.hit_material->ScatteringPDF(r, result, scatteredRay);
        Vector3 Lin = ray_color(scatteredRay, world, --depth);
        Vector3 scattered  = (scatter.albedo * brdf * Lin) / pdf;
        
        return emitted + scattered;
    }

    Ray Camera::get_ray(int i, int j, int s_i, int s_j) const {
        // Get a randomly sampled camera ray for the pixel at location i,j.

        auto pixel_center = pixel00_loc + ((float)i * pixel_delta_u) + ((float)j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square(s_i, s_j);

        // auto ray_origin = cameraPos;
        auto ray_origin = (defocus_angle <= 0) ? cameraPos : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

    Vector3 Camera::pixel_sample_square(int s_i, int s_j) const {
        // Returns a random point in the square surrounding a pixel at the origin.
        auto px = -0.5f + recip_sqrt_spp * (static_cast<float>(s_i) + random01());
        auto py = -0.5f + recip_sqrt_spp * (static_cast<float>(s_j) + random01());
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    Vector3 Camera::defocus_disk_sample() const
    {
        auto p = Vector3::RandomUnitDisk();
        return cameraPos + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
    }
}