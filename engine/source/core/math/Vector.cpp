//Vector3.cpp 
#include "Vector.h"
#include <iostream>
#include <cmath>
#include "mathcommon.h"
const double uZero = 1e-6;  
const Vector3 Vector3::zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::right = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::forward = Vector3(0.0f, 0.0f, 1.0f);


/****************************************************************************************/
//Vector2.cpp
/****************************************************************************************/

Vector2::Vector2(const Vector3& v):x(v.x), y(v.y)
{
}
bool Vector2::operator==(const Vector2& v) const
{
    return fabs(x - v.x) < uZero && fabs(y - v.y) < uZero;
}

bool Vector2::operator!=(const Vector2& v) const
{
    return fabs(x - v.x) > uZero || fabs(y - v.y) > uZero;
}

Vector2 &Vector2::operator=(const Vector3& v)
{
    x = v.x;
    y = v.y;
    return *this;
}


/****************************************************************************************/
//Vector3.cpp
/****************************************************************************************/
//复制构造函数，必须为常量引用参数，否则编译不通过  
Vector3::Vector3(const Vector3 &v):x(v.x), y(v.y), z(v.z)  
{  
}  
Vector3::Vector3(const Vector4 &v):x(v.x), y(v.y), z(v.z)
{
}
  
Vector3::~Vector3()
{  
}  


void Vector3::operator=(const Vector3 &v)  
{  
    x = v.x;  
    y = v.y;  
    z = v.z;  
}  


Vector3 Vector3::operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
Vector3 Vector3::operator+(const Vector3& v) { return Vector3(x+v.x, y+v.y, z+v.z); }
Vector3 Vector3::operator+(float f)const {return Vector3(x+f, y+f, z+f);}
Vector3& Vector3::operator+=(const Vector3 &v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
Vector3& Vector3::operator+=(float f) {x+=f; y+=f; z+=f; return *this;}
Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}
Vector3 Vector3::operator-()  
{  
    return Vector3(-x, -y, -z);  
}

Vector3 Vector3::operator-(const Vector3 &v)  
{  
    return Vector3(x-v.x, y-v.y, z-v.z);  
}  
  
Vector3 Vector3::operator/(const Vector3 &v)  
{  
    if (fabs(v.x) <= uZero || fabs(v.y) <= uZero || fabs(v.z) <= uZero)  
    {  
        std::cerr<<"Over flow!\n";  
        return *this;  
    }  
    return Vector3(x/v.x, y/v.y, z/v.z);  
}  
  
Vector3 Vector3::operator*(const Vector3 &v) const
{  
    return Vector3(x*v.x, y*v.y, z*v.z);  
}

Vector3 Vector3::operator*(float f) const
{
    return Vector3(x*f, y*f, z*f);
}
Vector3&  Vector3::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

  
Vector3 Vector3::operator-(float f)  
{  
    return Vector3(x-f, y-f, z-f);  
}
Vector3& Vector3::operator-=(float f)
{
    x -= f;
    y -= f;
    z -= f;
    return *this;
}

Vector3 Vector3::operator/(float f) const 
{  
    if (fabs(f) < uZero)  
    {  
        std::cerr<<"Over flow!\n";  
        return *this;  
    }  
    return Vector3(x/f, y/f, z/f);  
}  
Vector3& Vector3::operator/=(float f)
{
    if (fabs(f) < uZero)  
    {  
        std::cerr<<"Over flow!\n";  
        return *this;  
    }  
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

Vector3 Vector3::operator*(float f)  
{  
    return Vector3(x*f, y*f, z*f);  
}  

float& Vector3::operator[](int index)
{
    return (&x)[index];
}

float Vector3::dot(const Vector3 &v) const
{  
    return x*v.x + y*v.y + z*v.z;  
}  
  
float Vector3::length() const
{  
    return std::sqrt(dot(*this));  
}  

float Vector3::lengthSqr() const
{
    return dot(*this);
}
  
void Vector3::normalize()  
{  
    float len = length();  
    if (len < uZero) len = 1;  
    len = 1/len;  
  
    x *= len;  
    y *= len;  
    z *= len;  
}

Vector3 Vector3::normalized() const
{
    Vector3 result = *this;
    result.normalize();
    return result;
}
  
/* 
Cross Product叉乘公式 
aXb = | i,  j,  k  | 
     | a.x a.y a.z| 
     | b.x b.y b.z| = (a.x*b.z -a.z*b.y)i + (a.z*b.x - a.x*b.z)j + (a.x+b.y - a.y*b.x)k  
*/  
Vector3 Vector3::crossProduct(const Vector3 &v)  
{  
    return Vector3(y * v.z - z * v.y,  
                z * v.x - x * v.z,  
                x * v.y - y * v.x);  
}  
Vector3 Vector3::crossProduct(const Vector3 &v) const
{
    return Vector3(y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x);
}


Vector3 Vector3::reflect(const Vector3& normal) const
{
    return *this - 2 * dot(normal) * normal;
}


Vector3 Vector3::refract(const Vector3& normal, float etai_over_etat) const
{
    Vector3 unit = normalized();
    Vector3 n = normal.normalized();
    auto cos_theta = std::min((-unit).dot(n), 1.0f);
    Vector3 r_out_perp = etai_over_etat * (unit + cos_theta * n);
    Vector3 r_out_parallel = -sqrtf(1.0f - r_out_perp.lengthSqr()) * n;
    return r_out_perp + r_out_parallel;
}


void Vector3::tangentSpace(Vector3& tangent, Vector3& bitangent) const
{
    Vector3 a = (fabsf(x) > 0.9f) ? Vector3(0.0f, 1.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f);
    
    tangent = crossProduct(a).normalized();
    bitangent = crossProduct(tangent);
}

void Vector3::printVec3()  
{  
    std::cout<<"("<<x<<", "<<y<<", "<<z<<")"<<std::endl;  
}


bool Vector3::nearZero() const
{
    return fabs(x) < uZero && fabs(y) < uZero && fabs(z) < uZero;
}



Vector3 Vector3::Normalize(const Vector3& v)
{
    Vector3 result = v;
    result.normalize();
    return result;
}

float Vector3::Angle(const Vector3& v1, const Vector3& v2)
{
    return acosf(Vector3::Dot(v1, v2) / (v1.length() * v2.length()));
}

Vector3 Vector3::Projection(const Vector3& v1, const Vector3& v2)
{
    Vector3 v2n = v2 / v2.length();
    return v2n * Vector3::Dot(v1, v2n);
}
float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
    return v1.dot(v2);
}
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
{
    return v1.crossProduct(v2);
}

bool Vector3::Approximately(const Vector3& v1, const Vector3& v2)
{
    return fabs(v1.x - v2.x) < uZero && fabs(v1.y - v2.y) < uZero && fabs(v1.z - v2.z) < uZero;
}


Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2)
{
    return Vector3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
}

Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2)
{
    return Vector3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
}

Vector3 Vector3::random()
{
    return Vector3(random01(), random01(), random01());
}

Vector3 Vector3::random(float min, float max)
{
    return Vector3(random_float(min, max), random_float(min, max), random_float(min, max));
}



Vector3 Vector3::RandomUnitDisk()
{
    while (true) {
        auto p = Vector3(random_float(-1,1), random_float(-1,1), 0);
        if (p.lengthSqr() < 1)
            return p;
    }
}

Vector3 Vector3::RandomUnitSphere()
{
    Vector3 unit_sphere;
    while (true) {
        unit_sphere = Vector3::random(-1.0f,1.0f);
        if (unit_sphere.length() < 1.0f) break;
    }

    return unit_sphere.normalized();
}
Vector3 Vector3::RandomOnHemisphere(const Vector3 & normal)
{
    Vector3 on_unit_sphere = RandomUnitSphere();
    if (on_unit_sphere.dot(normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

Vector3 Vector3::Clamp(const Vector3& v, float min, float max)
{
    return Vector3(std::clamp(v.x, min, max), std::clamp(v.y, min, max), std::clamp(v.z, min, max));
}
Vector3 Vector3::Abs(const Vector3& v)
{
    return Vector3(fabs(v.x), fabs(v.y), fabs(v.z));
}

void Vector3::GetTangentSpace(const Vector3& normal, Vector3& tangent, Vector3& bitangent)
{
    Vector3 a = (fabsf(normal.x) > 0.9f) ? Vector3(0.0f, 1.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f);
    tangent = normal.crossProduct(a).normalized();
    bitangent = normal.crossProduct(tangent);
}
/****************************************************************************************/
//Vector4.cpp
/****************************************************************************************/
//复制构造函数，必须为常量引用参数，否则编译不通过
Vector4::Vector4(const Vector4 &v):x(v.x), y(v.y), z(v.z), w(v.w)
{
}

Vector4::Vector4(const Vector3 &v, float w):x(v.x), y(v.y), z(v.z), w(w)
{
}
Vector4::~Vector4()
{
}

void Vector4::operator=(const Vector4 &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

Vector4 Vector4::operator+(const Vector4 &v)
{
    return Vector4(x+v.x, y+v.y, z+v.z, w+v.w);
}

Vector4 Vector4::operator-(const Vector4 &v)
{
    return Vector4(x-v.x, y-v.y, z-v.z, w-v.w);
}

Vector4 Vector4::operator/(const Vector4 &v)
{
    if (fabs(v.x) <= uZero || fabs(v.y) <= uZero || fabs(v.z) <= uZero || fabs(v.w) <= uZero)
    {
        std::cerr<<"Over flow!\n";
        return *this;
    }
    return Vector4(x/v.x, y/v.y, z/v.z, w/v.w);
}

Vector4 Vector4::operator*(const Vector4 &v)
{
    return Vector4(x*v.x, y*v.y, z*v.z, w*v.w);
}

Vector4 Vector4::operator+(float f)
{
    return Vector4(x+f, y+f, z+f, w+f);
}

Vector4 Vector4::operator-(float f)
{
    return Vector4(x-f, y-f, z-f, w-f);
}

Vector4 Vector4::operator/(float f)
{
    if (fabs(f) < uZero)
    {
        std::cerr<<"Over flow!\n";
        return *this;
    }
    return Vector4(x/f, y/f, z/f, w/f);
}

Vector4 Vector4::operator*(float f)
{
    return Vector4(x*f, y*f, z*f, w*f);
}


float& Vector4::operator[](int index)
{
    return (&x)[index];
}

float Vector4::dot(const Vector4 &v)
{
    return x*v.x + y*v.y + z*v.z + w*v.w;
}

float Vector4::length()
{
    return std::sqrt(dot(*this));
}

void Vector4::normalize()
{
    float len = length();
    if (len < uZero) len = 1;
    len = 1/len;

    x *= len;
    y *= len;
    z *= len;
    w *= len;
}

/*
Cross Product叉乘公式
aXb = | i,  j,  k  |
     | a.x a.y a.z|
     | b.x b.y b.z| = (a.x*b.z -a.z*b.y)i + (a.z*b.x - a.x*b.z)j + (a.x+b.y - a.y*b.x)k
*/

Vector4 Vector4::crossProduct(const Vector4 &v)
{
    return Vector4(y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x,
                0);
}

void Vector4::printVec4()
{
    std::cout<<"("<<x<<", "<<y<<", "<<z<<", "<<w<<")"<<std::endl;
}

