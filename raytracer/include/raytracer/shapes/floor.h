#pragma once
#include "object.h"

namespace raytracer::raytracer
{
/**
 * An infinite horizontal plane, facing upwards.
 */
class Floor : public Object
{
    double y;

  public:
    /**
     * Creates a floor with the given properties.
     *
     * @param _mat The material of the floor, owned by the floor.
     * @param _y The height of the floor.
     */
    Floor(std::unique_ptr<const Material> _mat, double _y);

    /**
     * Getter for the height of the floor.
     *
     * @return The height of the floor.
     */
    [[nodiscard]] double get_y() const;

    [[nodiscard]] double ray_intersection(const Ray &r) const override;

    [[nodiscard]] common::Vec3 get_normal(const common::Vec3 &point) const override;

    [[nodiscard]] ObjectType get_type() const override
    {
        return ObjectType::FLOOR;
    }
};
} // namespace raytracer::raytracer
