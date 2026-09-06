#pragma once
#include <string>
#include "util/vec3.h"
#include "camera.h"

class Material
{
public:
    virtual ~Material() = default;
    static Vec3 reflect(const Vec3& dir_in, const Vec3& normal);
    static Vec3 refract(const Vec3& dir_in, const Vec3& normal, double refractive_index);
    [[nodiscard]] virtual Ray get_scattered(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const = 0;
    [[nodiscard]] virtual Vec3 get_color(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const = 0;
    [[nodiscard]] virtual std::string get_type() const = 0;
};
