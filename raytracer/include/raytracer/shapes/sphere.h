#pragma once
#include "common/util/vec3.h"
#include "../camera.h"
#include "object.h"

namespace raytracer::raytracer
{
/**
 * A sphere, defined by its center and radius.
 */
class Sphere : public Object
{
    common::Vec3 center;
    double radius;

  public:
    /**
     * Creates a sphere with the given properties.
     *
     * @param _mat The material of the sphere, owned by the sphere.
     * @param _center The position of the center of the sphere.
     * @param _radius The radius of the sphere.
     */
    Sphere(std::unique_ptr<const Material> _mat, const common::Vec3 &_center, double _radius);

    /**
     * Getter for the center of the sphere.
     *
     * @return The position of the center of the sphere.
     */
    [[nodiscard]] common::Vec3 get_center() const;

    /**
     * Getter for the radius of the sphere.
     *
     * @return The radius of the sphere.
     */
    [[nodiscard]] double get_radius() const;

    [[nodiscard]] double ray_intersection(const Ray &r) const override;

    [[nodiscard]] common::Vec3 get_normal(const common::Vec3 &point) const override;

    [[nodiscard]] ObjectType get_type() const override
    {
        return ObjectType::SPHERE;
    }
};
} // namespace raytracer::raytracer
