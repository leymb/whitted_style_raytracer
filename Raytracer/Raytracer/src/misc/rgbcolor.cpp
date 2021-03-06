#include "precomp.h"

#include "mathm.h"

rgbColor::rgbColor(int aR, int aG, int aB) : r(aR), g(aG), b(aB)
{
}

rgbColor::rgbColor(vec3 aVec3)
{
    r = static_cast<int>(256.0f * aVec3.x);
    mathm::Clamp(r, 255, 0);

    g = static_cast<int>(256.0f * aVec3.y);
    mathm::Clamp(g, 255, 0);

    b = static_cast<int>(256.0f * aVec3.z);
    mathm::Clamp(b, 255, 0);
}
