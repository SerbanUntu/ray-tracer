#pragma once
#include <string>
#include "lambertian.h"
#include "cubemap.h"

class LambertianTexture : public Lambertian
{
    Cubemap cm;

public:
    explicit LambertianTexture(Cubemap _cm);
    [[nodiscard]] Vec3 get_color(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const override;
    [[nodiscard]] const Cubemap& get_cubemap() const;
    [[nodiscard]] std::string get_type() const override;
};
