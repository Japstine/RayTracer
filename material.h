#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "color.h"

class material{
    public:
        virtual ~material() = default;

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered 
        )const {
            return false;
        }
};

class lambertian : public material { // Lambertian Scatter / Diffuse Material
    public:
        lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
    private:
        color albedo;
};

class metal : public material{
    public:
        metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + fuzz * random_unit_vector();
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return dot(scattered.direction(), rec.normal) > 0;
        }

    private:
        color albedo;
        double fuzz;
};

class dielectric : public material {
    public:

        dielectric(double refractive_index) : refractive_index(refractive_index) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            attenuation = color(1.0, 1.0, 1.0);
            // Checking if the ray is going in the denser medium or coming out of denser medium

            double ri = rec.front_face ? (1.0 / refractive_index) : refractive_index; 

            vec3 unit_direction = unit_vector(r_in.direction());

            double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = -std::sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract)
                direction = reflect(unit_direction, rec.normal);
            else 
                direction = refract(unit_direction, rec.normal, ri);

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        double refractive_index;
};

#endif