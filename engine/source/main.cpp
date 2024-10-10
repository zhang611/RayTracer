#include <iostream>
#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "mathlib.h"
#include "Lights.h"
#include "scene/Scene.h"
#include "Camera.h"
#include "Texture.h"
#include "primitives.h"
#include "materials.h"

using Ray = RayTracer::Ray;
using Texture = RayTracer::Texture;
using Sphere = RayTracer::Sphere;

void DisplayTexture(const RayTracer::Texture *pTex)
{
    if (pTex == nullptr)
        return;
    glRasterPos2f(-1, 1);
    glPixelZoom(1, -1);
    glDrawPixels(pTex->GetWidth(), pTex->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pTex->GetData());
    // glPixelZoom(1.0f, 1.0f);
}

void get_random_sphere_world(RayTracer::Camera &cam, RayTracer::Scene &world);
void get_two_sphere_world(RayTracer::Camera &cam, RayTracer::Scene &world);
void earth(RayTracer::Camera &cam, RayTracer::Scene &world);
void perlin_sphere(RayTracer::Camera &cam, RayTracer::Scene &world);
void quads(RayTracer::Camera &cam, RayTracer::Scene &world);
void simple_light(RayTracer::Camera &cam, RayTracer::Scene &world);
void cornell_box(RayTracer::Camera &cam, RayTracer::Scene &world);
void cornell_smoke(RayTracer::Camera &cam, RayTracer::Scene &world);
void final_scene(RayTracer::Camera &cam, RayTracer::Scene &world);
void cornell_box_sphere(RayTracer::Camera &cam, RayTracer::Scene &world);
void triangle(RayTracer::Camera &cam, RayTracer::Scene &world);
void mesh(RayTracer::Camera &cam, RayTracer::Scene &world);
int main()
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    auto width = 800;
    auto height = 600;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Ray Tracer", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Camera
    RayTracer::Camera camera(width, height);

    RayTracer::Scene world;
    switch (12)
    {
    case 1: get_random_sphere_world(camera, world); break;
    case 2: get_two_sphere_world(camera, world); break;
    case 3: earth(camera, world); break;
    case 4: perlin_sphere(camera, world); break;
    case 5: quads(camera, world); break;
    case 6: simple_light(camera, world); break;
    case 7: cornell_box(camera, world); break;
    case 8: cornell_smoke(camera, world); break;
    case 9: final_scene(camera, world); break;
    case 10: cornell_box_sphere(camera, world); break;
    case 11: triangle(camera, world); break;
    case 12: mesh(camera, world); break;
    default:
        break;
    }
    
    world.BuildBVH();

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL(); // NOTE: we need load openGL here
    /* Loop until the user closes the window */

    auto renderTex = camera.Render(world, 64);
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        DisplayTexture(renderTex);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void get_random_sphere_world(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;
    cam.SetPos(Vector3(5.0f, 2.0f, -5.0f));
    cam.SetLookAt(Vector3(0.0f, 0.0f, 1.0f));
    cam.SetFov(90.0f);

    cam.defocus_angle = 0.0f;
    cam.focus_dist = 3.0f;

    auto checker = std::make_shared<TexChecker>(0.3f, Vector3(0.2f, 0.3f, 0.1f), Vector3(0.9f, 0.8f, 0.9f));
    auto material_ground = std::make_shared<RayTracer::Lambert>(checker);
    world.add(std::make_shared<Sphere>(Vector3(0.0f, -1000.0f, 0.0f), 1000.0f, material_ground));

    auto material_center = std::make_shared<RayTracer::Lambert>(Vector3(0.7f, 0.3f, 0.3f));
    // auto metal_left = std::make_shared<RayTracer::D>(Vector3(0.8f, 0.8f, 0.8f), 0.3f);
    auto d_left = std::make_shared<RayTracer::Dielectric>(0.8f);
    auto metal_right = std::make_shared<RayTracer::Metal>(Vector3(0.8f, 0.6f, 0.2f), 0.1f);

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random01();
            Vector3 center(a + 0.9f * random01(), 0.2f, b + 0.9f * random01());

            if ((center - Vector3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
            {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8f)
                {
                    // diffuse
                    auto albedo = Vector3::random() * Vector3::random();
                    sphere_material = make_shared<Lambert>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                }
                else if (choose_mat < 0.95f)
                {
                    // metal
                    auto albedo = Vector3::random(0.5f, 1.0f);
                    auto fuzz = random_float(0.0f, 0.5f);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5f);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5f);
    world.add(make_shared<Sphere>(Vector3(0.0f, 1.0f, 0.0f), 1.0f, material1));

    auto material2 = make_shared<Lambert>(Vector3(0.4f, 0.2f, 0.1f));
    world.add(make_shared<Sphere>(Vector3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

    auto material3 = make_shared<Metal>(Vector3(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(make_shared<Sphere>(Vector3(4.0f, 1.0f, 0.0f), 1.0f, material3));
}

void get_two_sphere_world(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(13.0f, 2.0f, -3.0f));
    cam.SetLookAt(Vector3(0.0f, 0.0f, 0.0f));
    cam.SetFov(30.0f);

    auto checker = make_shared<TexChecker>(0.8f, Vector3(.2f, .3f, .1f), Vector3(.9f, .9f, .9f));

    world.add(make_shared<Sphere>(Vector3(0.0f, -10.0f, 0.0f), 10.0f, make_shared<Lambert>(checker)));
    world.add(make_shared<Sphere>(Vector3(0.0f, 10.0f, 0.0f), 10.0f, make_shared<Lambert>(checker)));
}

void earth(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(0.0f, 2.0f, -3.0f));
    cam.SetLookAt(Vector3(0.0f, 0.0f, 0.0f));
    cam.SetFov(90.0f);

    auto earth_surface = make_shared<Lambert>(make_shared<ImageTexture>("asset/images/earthmap.jpg"));
    auto globe = make_shared<Sphere>(Vector3(0.0f, 0.0f, 0.0f), 2.0f, earth_surface);
    world.add(globe);
}

void perlin_sphere(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(0.0f, 5.0f, -3.0f));
    cam.SetLookAt(Vector3(0.0f, 0.0f, 0.0f));
    cam.SetFov(90.0f);

    auto pertext = make_shared<NoiseTexture>(5.0f);
    world.add(make_shared<Sphere>(Vector3(0.0f, -1000.0f, 0.0f), 1000.0f, make_shared<Lambert>(pertext)));
    world.add(make_shared<Sphere>(Vector3(0.0f, 2.0f, 0.0f), 2.0f, make_shared<Lambert>(pertext)));
}

void quads(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(0.0f, 0.0f, -9.0f));
    cam.SetLookAt(Vector3(0.0f, 0.0f, 0.0f));
    cam.SetFov(80.0f);
    cam.defocus_angle = 0.0f;
    auto left_red = make_shared<Lambert>(Vector3(1.0f, 0.2f, 0.2f));
    auto back_green = make_shared<Lambert>(Vector3(0.2f, 1.0f, 0.2f));
    auto right_blue = make_shared<Lambert>(Vector3(0.2f, 0.2f, 1.0f));
    auto upper_orange = make_shared<Lambert>(Vector3(1.0f, 0.5f, 0.0f));
    auto lower_teal = make_shared<Lambert>(Vector3(0.2f, 0.8f, 0.8f));

    // Quads
    world.add(make_shared<Quad>(Vector3(-3.f, -2.f, -5.f), Vector3(0.f, 0.f, 4.f), Vector3(0.f, 4.f, 0.f), left_red));
    world.add(make_shared<Quad>(Vector3(-2.f, -2.f, 0.f), Vector3(4.f, 0.f, 0.f), Vector3(0.f, 4.f, 0.f), back_green));
    world.add(make_shared<Quad>(Vector3(3.f, -2.f, -1.f), Vector3(0.f, 0.f, -4.f), Vector3(0.f, 4.f, 0.f), right_blue));
    world.add(make_shared<Quad>(Vector3(-2.f, 3.f, -1.f), Vector3(4.f, 0.f, 0.f), Vector3(0.f, 0.f, -4.f), upper_orange));
    world.add(make_shared<Quad>(Vector3(-2.f, -3.f, -5.f), Vector3(4.f, 0.f, 0.f), Vector3(0.f, 0.f, 4.f), lower_teal));
}

void simple_light(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(26.0f, 3.0f, -6.0f));
    cam.SetLookAt(Vector3(0.0f, 2.0f, 0.0f));
    cam.SetFov(20.0f);
    cam.defocus_angle = 0.0f;
    cam.background = Vector3(0.0f, 0.0f, 0.0f);

    auto pertext = make_shared<NoiseTexture>(4.0f);
    world.add(make_shared<Sphere>(Vector3(0.0f, -1000.0f, 0.0f), 1000.0f, make_shared<Lambert>(pertext)));
    world.add(make_shared<Sphere>(Vector3(0.0f, 2.0f, 0.0f), 2.0f, make_shared<Lambert>(pertext)));

    auto difflight = make_shared<DiffuseLight>(Vector3(4.0f, 4.0f, 4.0f));
    world.add(make_shared<Quad>(Vector3(3.0f, 1.0f, 2.0f), Vector3(0.0f, 2.0f, 0.0f), Vector3(2.0f, 0.0f, 0.0f), difflight));
}


void cornell_box(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(278.0f, 278.0f, 800.0f));
    cam.SetLookAt(Vector3(278.0f, 278.0f, 0.0f));
    cam.SetFov(40.0f);
    cam.defocus_angle = 0.0f;
    cam.background = Vector3(0.0f, 0.0f, 0.0f);

    auto red = make_shared<Lambert>(Vector3(0.65f, 0.05f, 0.05f));
    auto white = make_shared<Lambert>(Vector3(0.73f, 0.73f, 0.73f));
    auto green = make_shared<Lambert>(Vector3(0.12f, 0.45f, 0.15f));
    auto light = make_shared<DiffuseLight>(Vector3(15.0f, 15.0f, 15.0f));
    auto aluminum = make_shared<Metal>(Vector3(0.8f, 0.85f, 0.88f), 0.0f);

    //create scene
    //walls
    world.add(make_shared<Quad>(Vector3(555.0f , 0.0f, 0.f), Vector3(0.0f, 0.0f, -555.0f), Vector3(0.0f, 555.0f, 0.0f), green));
    world.add(make_shared<Quad>(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -555.0f), Vector3(0.0f, 555.0f, 0.0f), red));
    world.add(make_shared<Quad>(Vector3(0.0f, 0.0f, 0.0f), Vector3(555.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -555.0f), white));
    world.add(make_shared<Quad>(Vector3(555.0f, 555.0f, -555.0f), Vector3(-555.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 555.0f), white));
    world.add(make_shared<Quad>(Vector3(0.0f, 0.0f, -555.0f), Vector3(555.0f, 0.0f, 0.0f), Vector3(0.0f, 555.0f, 0.0f), white));

    //lights
    auto _light  = make_shared<Quad>(Vector3(343.0f, 554.0f, -332.0f), Vector3(0.0f, 0.0f, 105.0f), Vector3(-130.0f, 0.0f, 0.0f), light);
    world.add(_light);
    Lights::lights->add(_light);
    // box

    std::shared_ptr<RayTracer::Hittable> box1 = make_shared<Box>(Vector3(0.0f, 0.0f, 0.0f), Vector3(165.0f, 165.0f, 165.0f), white);
    box1 = Translation::Translate(Rotation::Rotate(box1, Vector3(0.0f, 18.0f, 0.0f)), Vector3(130.0f+ 82.5f, 82.5f, -65.0f-82.5f));
    world.add(box1);
    std::shared_ptr<RayTracer::Hittable> box2 = make_shared<Box>(Vector3(0.0f, 0.0f, 0.0f), Vector3(165.0f, 330.0f, 165.0f), aluminum);
    box2 = Translation::Translate(Rotation::Rotate(box2, Vector3(0.0f, -15.0f, 0.0f)), Vector3(265.0f+82.5f, 165.0f, -295.0f-82.5f));
    world.add(box2);
}

void cornell_smoke(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(278.0f, 278.0f, 800.0f));
    cam.SetLookAt(Vector3(278.0f, 278.0f, 0.0f));
    cam.SetFov(40.0f);
    cam.defocus_angle = 0.0f;
    cam.background = Vector3(0.0f, 0.0f, 0.0f);

    auto red = make_shared<Lambert>(Vector3(0.65f, 0.05f, 0.05f));
    auto white = make_shared<Lambert>(Vector3(0.73f, 0.73f, 0.73f));
    auto green = make_shared<Lambert>(Vector3(0.12f, 0.45f, 0.15f));
    auto light = make_shared<DiffuseLight>(Vector3(15.0f, 15.0f, 15.0f));

    //create scene
    //walls
    world.add(make_shared<Quad>(Vector3(555.0f , 0.0f, 0.f), Vector3(0.0f, 0.0f, -555.0f), Vector3(0.0f, 555.0f, 0.0f), green));
    world.add(make_shared<Quad>(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -555.0f), Vector3(0.0f, 555.0f, 0.0f), red));
    world.add(make_shared<Quad>(Vector3(0.0f, 0.0f, 0.0f), Vector3(555.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -555.0f), white));
    world.add(make_shared<Quad>(Vector3(555.0f, 555.0f, -555.0f), Vector3(-555.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 555.0f), white));
    world.add(make_shared<Quad>(Vector3(0.0f, 0.0f, -555.0f), Vector3(555.0f, 0.0f, 0.0f), Vector3(0.0f, 555.0f, 0.0f), white));

    //lights
    world.add(make_shared<Quad>(Vector3(343.0f, 554.0f, -332.0f), Vector3(0.0f, 0.0f, 105.0f), Vector3(-130.0f, 0.0f, 0.0f), light));

    // box
    std::shared_ptr<RayTracer::Hittable> box1 = make_shared<Box>(Vector3(0.0f, 0.0f, 0.0f), Vector3(165.0f, 165.0f, 165.0f), white);
    box1 = Translation::Translate(Rotation::Rotate(box1, Vector3(0.0f, 18.0f, 0.0f)), Vector3(130.0f+ 82.5f, 82.5f, -65.0f-82.5f));
    
    std::shared_ptr<RayTracer::Hittable> box2 = make_shared<Box>(Vector3(0.0f, 0.0f, 0.0f), Vector3(165.0f, 330.0f, 165.0f), white);
    box2 = Translation::Translate(Rotation::Rotate(box2, Vector3(0.0f, -15.0f, 0.0f)), Vector3(265.0f+82.5f, 165.0f, -295.0f-82.5f));
    
    world.add(make_shared<ConstantMedium>(box1, 0.01f, Vector3(0.0f, 0.0f, 0.0f)));
    world.add(make_shared<ConstantMedium>(box2, 0.01f, Vector3(1.0f, 1.0f, 1.0f)));
}


void final_scene(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(478.0f, 278.0f, 600.0f));
    cam.SetLookAt(Vector3(278.0f, 278.0f, 0.0f));
    cam.SetFov(40.0f);
    cam.defocus_angle = 0.0f;
    cam.background = Vector3(0.0f, 0.0f, 0.0f);



    auto ground = make_shared<Lambert>(Vector3(0.48f, 0.83f, 0.53f));
    int boxes_per_side = 20;
    std::shared_ptr<Scene> subScene1 = make_shared<Scene>();
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0f;
            auto x0 = -1000.0f + i*w;
            auto z0 = 1000.0f - j*w;
            auto y0 = 0.0f;
            auto x1 = x0 + w;
            auto y1 = random_float(1.0f,101.0f);
            auto z1 = z0 - w;

            Vector3 center = Vector3(0.5f*(x0+x1), 0.5f*(y0+y1), 0.5f*(z0+z1));
            Vector3 size = Vector3(x1-x0, y1-y0, z1-z0);
            subScene1->add(make_shared<Box>(center, size, ground));
        }
    }

    subScene1->BuildBVH();
    world.add(subScene1);
    
    auto light = make_shared<DiffuseLight>(Vector3(7.0f, 7.0f, 7.0f));
    world.add(make_shared<Quad>(Vector3(123.0f, 554.0f, -147.0f), Vector3(300.0f ,0.0f , 0.0f ), Vector3(0.0f, 0.0f, -265.0f), light));
    
    // auto center1 = Vector3(400.0f, 400.0f, -200.0f);
    // auto center2 = center1 + Vector3(30.0f,0.0f,0.0f);
    // auto sphere_material = make_shared<Lambert>(Vector3(0.7f, 0.3f, 0.1f));
    // world.add(make_shared<Sphere>(center1, center2, 50.0f, sphere_material));

    world.add(make_shared<Sphere>(Vector3(260.f, 150.f, -45.f), 50.f, make_shared<Dielectric>(1.5f)));
    world.add(make_shared<Sphere>( Vector3(0.f, 150.f, -145.f), 50.f, make_shared<Metal>(Vector3(0.8f, 0.8f, 0.9f), 1.0f)));

    auto boundary = make_shared<Sphere>(Vector3(360.0f, 150.0f, -145.0f), 70.0f, make_shared<Dielectric>(1.5f));
    world.add(boundary);
    world.add(make_shared<ConstantMedium>(boundary, 0.2f, Vector3(0.2f, 0.4f, 0.9f)));
    boundary = make_shared<Sphere>(Vector3(0.0f, 0.0f, 0.0f), 5000.0f, make_shared<Dielectric>(1.5f));
    world.add(make_shared<ConstantMedium>(boundary, .0001f, Vector3(1.f,1.f,1.f)));

    auto emat = make_shared<Lambert>(make_shared<ImageTexture>("asset/images/earthmap.jpg"));
    world.add(make_shared<Sphere>(Vector3(400.f,200.f, -400.f), 100.f, emat));
    auto pertext = make_shared<NoiseTexture>(0.1f);
    world.add(make_shared<Sphere>(Vector3(220.f,280.f, -300.f), 80.f, make_shared<Lambert>(pertext)));

    std::shared_ptr<Scene> subScene2 = make_shared<Scene>();
    auto white = make_shared<Lambert>(Vector3(.73f, .73f, .73f));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        subScene2->add(make_shared<Sphere>(Vector3::random(0.0f ,165.0f), 10.0f, white));
    }
    subScene2->BuildBVH();

    world.add(Translation::Translate(Rotation::Rotate(subScene2, Vector3(0.0f, -15.f, 0.0f)), Vector3(-100.0f, 270, -395.0f)));
}


void cornell_box_sphere(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(278.0f, 278.0f, 800.0f));
    cam.SetLookAt(Vector3(278.0f, 278.0f, 0.0f));
    cam.SetFov(40.0f);
    cam.defocus_angle = 0.0f;
    cam.background = Vector3(0.0f, 0.0f, 0.0f);

    auto red = make_shared<Lambert>(Vector3(0.65f, 0.05f, 0.05f));
    auto white = make_shared<Lambert>(Vector3(0.73f, 0.73f, 0.73f));
    auto green = make_shared<Lambert>(Vector3(0.12f, 0.45f, 0.15f));
    auto light = make_shared<DiffuseLight>(Vector3(15.0f, 15.0f, 15.0f));
    auto aluminum = make_shared<Metal>(Vector3(0.8f, 0.85f, 0.88f), 0.0f);

    //create scene
    //walls
    world.add(make_shared<Quad>(Vector3(555.0f , 0.0f, 0.f), Vector3(0.0f, 0.0f, -555.0f), Vector3(0.0f, 555.0f, 0.0f), green));
    world.add(make_shared<Quad>(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -555.0f), Vector3(0.0f, 555.0f, 0.0f), red));
    world.add(make_shared<Quad>(Vector3(0.0f, 0.0f, 0.0f), Vector3(555.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -555.0f), white));
    world.add(make_shared<Quad>(Vector3(555.0f, 555.0f, -555.0f), Vector3(-555.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 555.0f), white));
    world.add(make_shared<Quad>(Vector3(0.0f, 0.0f, -555.0f), Vector3(555.0f, 0.0f, 0.0f), Vector3(0.0f, 555.0f, 0.0f), white));

    //lights
    auto _light  = make_shared<Quad>(Vector3(343.0f, 554.0f, -332.0f), Vector3(0.0f, 0.0f, 105.0f), Vector3(-130.0f, 0.0f, 0.0f), light);
    world.add(_light);
    // Lights::lights.emplace_back(_light);

    // std::shared_ptr<RayTracer::Hittable> box1 = make_shared<Box>(Vector3(0.0f, 0.0f, 0.0f), Vector3(165.0f, 165.0f, 165.0f), white);
    // box1 = Translation::Translate(Rotation::Rotate(box1, Vector3(0.0f, 18.0f, 0.0f)), Vector3(130.0f+ 82.5f, 82.5f, -65.0f-82.5f));
    // world.add(box1);

    auto Glass = make_shared<Dielectric>(1.5f);
    auto glass = make_shared<Sphere>(Vector3(190.0f,90.0f, -190.0f), 90.0f, Glass);
    world.add(glass);

    std::shared_ptr<RayTracer::Hittable> box2 = make_shared<Box>(Vector3(0.0f, 0.0f, 0.0f), Vector3(165.0f, 330.0f, 165.0f), white);
    box2 = Translation::Translate(Rotation::Rotate(box2, Vector3(0.0f, -15.0f, 0.0f)), Vector3(265.0f+82.5f, 165.0f, -295.0f-82.5f));
    world.add(box2);
    
    Lights::lights->add(glass);
    Lights::lights->add(_light);
    Lights::lights->BuildBVH();
}


void triangle(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(0.0f, 0.0f, -9.0f));
    cam.SetLookAt(Vector3(0.0f, 0.0f, 0.0f));
    cam.SetFov(80.0f);
    cam.defocus_angle = 0.0f;
    auto left_red = make_shared<Lambert>(Vector3(1.0f, 0.2f, 0.2f));
    auto back_green = make_shared<Lambert>(Vector3(0.2f, 1.0f, 0.2f));
    auto right_blue = make_shared<Lambert>(Vector3(0.2f, 0.2f, 1.0f));
    auto upper_orange = make_shared<Lambert>(Vector3(1.0f, 0.5f, 0.0f));
    auto lower_teal = make_shared<Lambert>(Vector3(0.2f, 0.8f, 0.8f));

    //left
    world.add(make_shared<Triangle>(Vector3(-3.f, 2.f, -1.f), Vector3(-3.f, 2.f, -5.f), Vector3(-3.f, -2.f, -5.f), left_red));
    world.add(make_shared<Triangle>(Vector3(-3.f, 2.f, -1.f), Vector3(-3.f, -2.f, -1.f), Vector3(-3.f, -2.f, -5.f), back_green));
    
    world.add(make_shared<Triangle>(Vector3(-2.f, -2.f, 0.f), Vector3(2.f, -2.f, 0.f), Vector3(-2.f, 2.f, 0.f), back_green));
    world.add(make_shared<Triangle>(Vector3(2.f, 2.f, 0.f), Vector3(2.f, -2.f, 0.f), Vector3(-2.f, 2.f, 0.f), right_blue));

    world.add(make_shared<Triangle>(Vector3(3.f, 2.f, -1.f), Vector3(3.f, 2.f, -5.f), Vector3(3.f, -2.f, -5.f), right_blue));
    world.add(make_shared<Triangle>(Vector3(3.f, 2.f, -1.f), Vector3(3.f, -2.f, -1.f), Vector3(3.f, -2.f, -5.f), lower_teal));

    world.add(make_shared<Triangle>(Vector3(-2.f, 3.f, -5.f), Vector3(2.f, 3.f, -1.f), Vector3(2.f, 3.f, -5.f), upper_orange));
    world.add(make_shared<Triangle>(Vector3(-2.f, 3.f, -5.f), Vector3(2.f, 3.f, -1.f), Vector3(-2.f, 3.f, -1.f), left_red));

    world.add(make_shared<Triangle>(Vector3(-2.f, -3.f, -5.f), Vector3(2.f, -3.f, -1.f), Vector3(-2.f, -3.f, -1.f), lower_teal));
    world.add(make_shared<Triangle>(Vector3(-2.f, -3.f, -5.f), Vector3(2.f, -3.f, -1.f), Vector3(2.f, -3.f, -5.f), upper_orange));

}

void mesh(RayTracer::Camera &cam, RayTracer::Scene &world)
{
    using namespace std;
    using namespace RayTracer;

    cam.SetPos(Vector3(0.0f, 8.0f,  10.0f));
    cam.SetLookAt(Vector3(0.0f, 0.0f, 0.0f));
    cam.SetFov(90.0f);
    cam.defocus_angle = 0.0f;

    auto checker = std::make_shared<TexChecker>(0.3f, Vector3(0.2f, 0.3f, 0.1f), Vector3(0.9f, 0.8f, 0.9f));
    auto material_ground = std::make_shared<RayTracer::Lambert>(checker);



    auto diffuse = make_shared<Lambert>(Vector3(0.8f, 0.8f, 0.8f));
    auto isotropic = make_shared<Isotropic>(Vector3(0.8f, 0.8f, 0.8f));
    auto dielectric = make_shared<Dielectric>(0.8f);
    auto metal = make_shared<Metal>(Vector3(0.8f, 0.8f, 0.8f), 0.01f);

    auto mesh = make_shared<MeshTriangle>("asset/models/bunny/bunny.obj", diffuse);
    world.add(Translation::Translate(mesh, Vector3(1.0f, -2.0f, 0.0f)));
    // world.add(mesh);
    // auto sphere = ;
    
    // world.add(make_shared<Sphere>(Vector3(0.0f, 3.0f, 0.0f), 3.0f, metal));
    auto ground = make_shared<Sphere>(Vector3(0.0f, -1000.0f, 0.0f), 1000.0f, material_ground);
    world.add(ground);
}