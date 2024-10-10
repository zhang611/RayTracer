#pragma once 
#include "mathcommon.h"
#include "Vector.h"

class Perlin
{
    public:
        Perlin()
        {
            randVec = new Vector3[point_count];
            for (int i = 0; i < point_count; ++i) {
                randVec[i] = Vector3::random(-1, 1).normalized();
            }

            perm_x = perlin_generate_perm();
            perm_y = perlin_generate_perm();
            perm_z = perlin_generate_perm();
        }
        ~Perlin()
        {
            delete[] randVec;
            delete[] perm_x;
            delete[] perm_y;
            delete[] perm_z;
        }

        float Noise(const Vector3& point) const{
            auto u = point.x - floor(point.x);
            auto v = point.y - floor(point.y);
            auto w = point.z - floor(point.z);
            u = u*u*(3-2*u);
            v = v*v*(3-2*v);
            w = w*w*(3-2*w);

            auto i = static_cast<int>(floor(point.x));
            auto j = static_cast<int>(floor(point.y));
            auto k = static_cast<int>(floor(point.z));
            Vector3 c[2][2][2];

            for (int di=0; di < 2; di++)
                for (int dj=0; dj < 2; dj++)
                    for (int dk=0; dk < 2; dk++)
                        c[di][dj][dk] = randVec[
                            perm_x[(i+di) & 255] ^
                            perm_y[(j+dj) & 255] ^
                            perm_z[(k+dk) & 255]
                        ];

            return trilinear_interp(c, u, v, w);
        }

        float Turb(const Vector3& p, int depth=7) const {
            auto accum = 0.0f;
            auto temp_p = p;
            auto weight = 1.0f;

            for (int i = 0; i < depth; i++) {
                accum += weight* Noise(temp_p);
                weight *= 0.5f;
                temp_p *= 2.f;
            }

        return fabsf(accum);
    }

    private:
        static const int point_count = 256;
        Vector3* randVec;
        int* perm_x;
        int* perm_y;
        int* perm_z;

        static int* perlin_generate_perm() {
            auto p = new int[point_count];

            for (int i = 0; i < Perlin::point_count; i++)
                p[i] = i;
            permute(p, point_count);

        return p;
        }

        static void permute(int* p, int n) {
            for (int i = n-1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }

        static float trilinear_interp(Vector3 c[2][2][2], float u, float v, float w) {
            auto uu = u*u*(3-2*u);
            auto vv = v*v*(3-2*v);
            auto ww = w*w*(3-2*w);
            auto accum = 0.0f;

            for (int i=0; i < 2; i++)
                for (int j=0; j < 2; j++)
                    for (int k=0; k < 2; k++) {
                        Vector3 weight_v(u-i, v-j, w-k);
                        accum += (i*uu + (1-i)*(1-uu))
                            * (j*vv + (1-j)*(1-vv))
                            * (k*ww + (1-k)*(1-ww))
                            * Vector3::Dot(c[i][j][k], weight_v);
                    }

            return accum;
        }
};