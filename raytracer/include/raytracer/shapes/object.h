#pragma once
#include <memory>
#include "common/util/vec3.h"
#include "../materials/material.h"

namespace raytracer::raytracer
{
enum class ObjectType : uint8_t
{
    FLOOR,
    SPHERE
};

/**
 * A shape in the scene that rays can hit, made of a single material.
 */
class Object
{
    std::unique_ptr<const Material> mat;

  public:
    virtual ~Object() = default;

    /**
     * Creates an object with the given material.
     *
     * @param _mat The material of the object, owned by the object.
     */
    explicit Object(std::unique_ptr<const Material> _mat) : mat(std::move(_mat))
    {
    }

    /**
     * Returns a non-owning pointer to the material of the object.
     *
     * @return A non-owning pointer to the material of the object.
     */
    [[nodiscard]] const Material *get_material() const
    {
        return mat.get();
    }

    /**
     * Computes the distance along a ray at which it first hits the object.
     *
     * @param r The ray to intersect with the object.
     * @return The value t such that r.origin + r.direction * t is the intersection point, or a negative value if the
     * ray does not hit the object.
     */
    [[nodiscard]] virtual double ray_intersection(const Ray &r) const = 0;

    /**
     * Returns the normal to the surface of the object at a given point.
     *
     * @param point A point on the surface of the object.
     * @return The normal to the surface at that point.
     */
    [[nodiscard]] virtual common::Vec3 get_normal(const common::Vec3 &point) const = 0;

    /**
     * Returns the type of the object, used to identify the concrete class.
     *
     * @return The type of the object.
     */
    [[nodiscard]] virtual ObjectType get_type() const = 0;
};
} // namespace raytracer::raytracer
