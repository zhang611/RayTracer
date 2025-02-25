#include "Lights.h"

namespace RayTracer {
std::shared_ptr<Lights> Lights::lights = std::make_shared<Lights>();
}