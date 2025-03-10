#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable { // Sphere extends hittable
    public:
        sphere(const point3& center, double radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {} 
            

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius * radius;
            auto discriminant = h * h - a * c;

            if (discriminant < 0)
                return false;

            auto sqrt_disc = std::sqrt(discriminant);

            auto root = (h - sqrt_disc) / a; // First and smaller root

            if (!ray_t.surrounds(root)){
                root = (h + sqrt_disc) / a; // Second and the larger root

                if (!ray_t.surrounds(root)){
                    return false;
                }
            }

            // After checking both the roots update the hit record

            rec.t = root;
            rec.p = r.pt(rec.t);
            vec3 outward_normal = (rec.p - center) / radius; // Dividing by radius as it is the length of the normal in a sphere 
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;


            return true;
        }
    private:
        point3 center;
        double radius;
        shared_ptr<material> mat;
};

#endif