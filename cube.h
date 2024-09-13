#ifndef CUBE_H
#define CUBE_H

#include "rtweekend.h"
#include "hittable.h"

class cube : public hittable {
public:
    cube() {}
    cube(const point3& cen, double half_extent, shared_ptr<material> mat)
        : center(cen), half_extent(half_extent), mat_ptr(mat) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    point3 center;
    double half_extent;
    shared_ptr<material> mat_ptr;
};

bool cube::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    const point3& o = r.origin();
    const vec3& d = r.direction();

    // Compute intersections with each pair of planes (xy, xz, yz)

    // xy plane
    auto t_xy_min = (center.z() - half_extent - o.z()) / d.z();
    auto t_xy_max = (center.z() + half_extent - o.z()) / d.z();

    if (t_xy_min > t_xy_max) std::swap(t_xy_min, t_xy_max);

    // xz plane
    auto t_xz_min = (center.y() - half_extent - o.y()) / d.y();
    auto t_xz_max = (center.y() + half_extent - o.y()) / d.y();

    if (t_xz_min > t_xz_max) std::swap(t_xz_min, t_xz_max);

    // yz plane
    auto t_yz_min = (center.x() - half_extent - o.x()) / d.x();
    auto t_yz_max = (center.x() + half_extent - o.x()) / d.x();

    if (t_yz_min > t_yz_max) std::swap(t_yz_min, t_yz_max);

    // Get the largest min and smallest max to find the intersection range
    auto t_enter = fmax(t_xy_min, fmax(t_xz_min, t_yz_min));
    auto t_exit = fmin(t_xy_max, fmin(t_xz_max, t_yz_max));

    // Check if the intersection range is valid
    if (t_enter > t_exit || t_exit < t_min || t_enter > t_max)
        return false;

    // Calculate the intersection point and normal
    double t = t_enter;
    point3 p = r.at(t);
    vec3 outward_normal;
    bool is_inside = false;

    if (t == t_xy_min) {
        outward_normal = vec3(0, 0, -1);
    }
    else if (t == t_xy_max) {
        outward_normal = vec3(0, 0, 1);
    }
    else if (t == t_xz_min) {
        outward_normal = vec3(0, -1, 0);
    }
    else if (t == t_xz_max) {
        outward_normal = vec3(0, 1, 0);
    }
    else if (t == t_yz_min) {
        outward_normal = vec3(-1, 0, 0);
    }
    else if (t == t_yz_max) {
        outward_normal = vec3(1, 0, 0);
    }

    if (dot(outward_normal, d) > 0) {
        outward_normal = -outward_normal;
        is_inside = true;
    }

    rec.t = t;
    rec.p = p;
    rec.set_face_normal(r, outward_normal); 
    rec.mat_ptr = mat_ptr;
    rec.is_inside = is_inside;

    return true;
}


#endif