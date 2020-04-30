//// License: Apache 2.0. See LICENSE file in root directory.
//// Copyright(c) 2020 Intel Corporation. All Rights Reserved.

#include "coeffs.h"
#include "calibration.h"
#include "frame-data.h"


namespace librealsense {
namespace algo {
namespace depth_to_rgb_calibration {


    double calculate_rotation_x_alpha_coeff(
        rotation_in_angles const & rot_angles,
        double3 const & v,
        double rc,
        double2 const & xy,
        const calib & yuy_intrin_extrin
    )
    {
        auto r = yuy_intrin_extrin.rot.rot;
        double t[3] = { yuy_intrin_extrin.trans.t1, yuy_intrin_extrin.trans.t2, yuy_intrin_extrin.trans.t3 };
        auto d = yuy_intrin_extrin.coeffs;
        auto ppx = (double)yuy_intrin_extrin.k_mat.ppx;
        auto ppy = (double)yuy_intrin_extrin.k_mat.ppy;
        auto fx = (double)yuy_intrin_extrin.k_mat.fx;
        auto fy = (double)yuy_intrin_extrin.k_mat.fy;

        auto sin_a = (double)sin( rot_angles.alpha );
        auto sin_b = (double)sin( rot_angles.beta );
        auto sin_g = (double)sin( rot_angles.gamma );

        auto cos_a = (double)cos( rot_angles.alpha );
        auto cos_b = (double)cos( rot_angles.beta );
        auto cos_g = (double)cos( rot_angles.gamma );
        auto x1 = (double)xy.x;
        auto y1 = (double)xy.y;

        auto x2 = x1 * x1;
        auto y2 = y1 * y1;
        auto xy2 = x2 + y2;
        auto x2_y2 = xy2 * xy2;

        auto x = v.x;
        auto y = v.y;
        auto z = v.z;


        auto exp1 = z * (0 * sin_b + 1 * cos_a*cos_b - 0 * cos_b*sin_a)
            + x * (0 * (cos_a*sin_g + cos_g * sin_a*sin_b)
                + 1 * (sin_a*sin_g - cos_a * cos_g*sin_b)
                + 0 * cos_b*cos_g)
            + y * (0 * (cos_a*cos_g - sin_a * sin_b*sin_g)
                + 1 * (cos_g*sin_a + cos_a * sin_b*sin_g)
                - 0 * cos_b*sin_g)
            + 1 * (0 * t[0] + 0 * t[1] + 1 * t[2]);

        auto res = (((x*(0 * (sin_a*sin_g - cos_a * cos_g*sin_b)
            - 1 * (cos_a*sin_g + cos_g * sin_a*sin_b)
            ) + y * (0 * (cos_g*sin_a + cos_a * sin_b*sin_g)
                - 1 * (cos_a*cos_g - sin_a * sin_b*sin_g)
                ) + z * (0 * cos_a*cos_b + 1 * cos_b*sin_a)
            )*(z*(fx*sin_b + ppx * cos_a*cos_b - 0 * cos_b*sin_a)
                + x * (0 * (cos_a*sin_g + cos_g * sin_a*sin_b)
                    + ppx * (sin_a*sin_g - cos_a * cos_g*sin_b)
                    + fx * cos_b*cos_g)
                + y * (0 * (cos_a*cos_g - sin_a * sin_b*sin_g)
                    + ppx * (cos_g*sin_a + cos_a * sin_b*sin_g)
                    - fx * cos_b*sin_g)
                + 1 * (fx*t[0] + 0 * t[1] + ppx * t[2])
                ) - (x*(0 * (sin_a*sin_g - cos_a * cos_g*sin_b)
                    - ppx * (cos_a*sin_g + cos_g * sin_a*sin_b)
                    ) + y * (0 * (cos_g*sin_a + cos_a * sin_b*sin_g)
                        - ppx * (cos_a*cos_g - sin_a * sin_b*sin_g)
                        ) + z * (0 * cos_a*cos_b + ppx * cos_b*sin_a)
                    )*(z*(0 * sin_b + 1 * cos_a*cos_b - 0 * cos_b*sin_a)
                        + x * (0 * (cos_a*sin_g + cos_g * sin_a*sin_b)
                            + 1 * (sin_a*sin_g - cos_a * cos_g*sin_b)
                            + 0 * cos_b*cos_g)
                        + y * (0 * (cos_a*cos_g - sin_a * sin_b*sin_g)
                            + 1 * (cos_g*sin_a + cos_a * sin_b*sin_g)
                            - 0 * cos_b*sin_g)
                        + 1 * (0 * t[0] + 0 * t[1] + 1 * t[2])
                        ))
            *(rc + 6 * d[3] * x1 + 2 * d[2] * y1 + x1 * (2 * d[0] * x1 + 4 * d[1] * x1*(xy2)+6 * d[4] * x1*x2_y2))
            ) / (exp1*exp1) + (fx*((x*(0 * (sin_a*sin_g - cos_a * cos_g*sin_b)
                - 1 * (cos_a*sin_g + cos_g * sin_a*sin_b)
                ) + y * (0 * (cos_g*sin_a + cos_a * sin_b*sin_g)
                    - 1 * (cos_a*cos_g - sin_a * sin_b*sin_g)
                    ) + z * (0 * cos_a*cos_b + 1 * cos_b*sin_a)
                )*(z*(0 * sin_b + ppy * cos_a*cos_b - fy * cos_b*sin_a)
                    + x * (fy*(cos_a*sin_g + cos_g * sin_a*sin_b)
                        + ppy * (sin_a*sin_g - cos_a * cos_g*sin_b)
                        + 0 * cos_b*cos_g)
                    + y * (fy*(cos_a*cos_g - sin_a * sin_b*sin_g)
                        + ppy * (cos_g*sin_a + cos_a * sin_b*sin_g)
                        - 0 * cos_b*sin_g)
                    + 1 * (0 * t[0] + fy * t[1] + ppy * t[2])
                    ) - (x*(fy*(sin_a*sin_g - cos_a * cos_g*sin_b)
                        - ppy * (cos_a*sin_g + cos_g * sin_a*sin_b)
                        ) + y * (fy*(cos_g*sin_a + cos_a * sin_b*sin_g)
                            - ppy * (cos_a*cos_g - sin_a * sin_b*sin_g)
                            ) + z * (fy*cos_a*cos_b + ppy * cos_b*sin_a)
                        )*(z*(0 * sin_b + 1 * cos_a*cos_b - 0 * cos_b*sin_a)
                            + x * (0 * (cos_a*sin_g + cos_g * sin_a*sin_b)
                                + 1 * (sin_a*sin_g - cos_a * cos_g*sin_b)
                                + 0 * cos_b*cos_g)
                            + y * (0 * (cos_a*cos_g - sin_a * sin_b*sin_g)
                                + 1 * (cos_g*sin_a + cos_a * sin_b*sin_g)
                                - 0 * cos_b*sin_g)
                            + 1 * (0 * t[0] + 0 * t[1] + 1 * t[2])
                            ))
                *(2 * d[2] * x1 + 2 * d[3] * y1 + x1 * (2 * d[0] * y1 + 4 * d[1] * y1*(xy2)+6 * d[4] * y1*x2_y2))
                ) / (fy*(exp1*exp1));
        return res;
    }

    static double calculate_rotation_x_beta_coeff(
        rotation_in_angles const & rot_angles,
        double3 const & v,
        double rc,
        double2 const & xy,
        const calib & yuy_intrin_extrin
    )
    {
        auto r = yuy_intrin_extrin.rot.rot;
        double t[3] = { yuy_intrin_extrin.trans.t1, yuy_intrin_extrin.trans.t2, yuy_intrin_extrin.trans.t3 };
        auto d = yuy_intrin_extrin.coeffs;
        auto ppx = (double)yuy_intrin_extrin.k_mat.ppx;
        auto ppy = (double)yuy_intrin_extrin.k_mat.ppy;
        auto fx = (double)yuy_intrin_extrin.k_mat.fx;
        auto fy = (double)yuy_intrin_extrin.k_mat.fy;

        auto sin_a = sin( rot_angles.alpha );
        auto sin_b = sin( rot_angles.beta );
        auto sin_g = sin( rot_angles.gamma );

        auto cos_a = cos( rot_angles.alpha );
        auto cos_b = cos( rot_angles.beta );
        auto cos_g = cos( rot_angles.gamma );
        auto x1 = (double)xy.x;
        auto y1 = (double)xy.y;

        auto x2 = x1 * x1;
        auto y2 = y1 * y1;
        auto xy2 = x2 + y2;
        auto x2_y2 = xy2 * xy2;

        auto x = (double)v.x;
        auto y = (double)v.y;
        auto z = (double)v.z;

        auto exp1 = z * (cos_a*cos_b) +
            x * ((sin_a*sin_g - cos_a * cos_g*sin_b))
            + y * ((cos_g*sin_a + cos_a * sin_b*sin_g))
            + (t[2]);

        auto res = -(((z*(0 * cos_b - 1 * cos_a*sin_b + 0 * sin_a*sin_b)
            - x * (0 * cos_g*sin_b + 1 * cos_a*cos_b*cos_g - 0 * cos_b*cos_g*sin_a)
            + y * (0 * sin_b*sin_g + 1 * cos_a*cos_b*sin_g - 0 * cos_b*sin_a*sin_g)
            )*(z*(fx*sin_b + ppx * cos_a*cos_b - 0 * cos_b*sin_a)
                + x * (0 * (cos_a*sin_g + cos_g * sin_a*sin_b)
                    + ppx * (sin_a*sin_g - cos_a * cos_g*sin_b)
                    + fx * cos_b*cos_g)
                + y * (0 * (cos_a*cos_g - sin_a * sin_b*sin_g)
                    + ppx * (cos_g*sin_a + cos_a * sin_b*sin_g)
                    - fx * cos_b*sin_g)
                + 1 * (fx*t[0] + 0 * t[1] + ppx * t[2])
                ) - (z*(fx*cos_b - ppx * cos_a*sin_b + 0 * sin_a*sin_b)
                    - x * (fx*cos_g*sin_b + ppx * cos_a*cos_b*cos_g - 0 * cos_b*cos_g*sin_a)
                    + y * (fx*sin_b*sin_g + ppx * cos_a*cos_b*sin_g - 0 * cos_b*sin_a*sin_g)
                    )*(z*(0 * sin_b + 1 * cos_a*cos_b - 0 * cos_b*sin_a)
                        + x * (0 * (cos_a*sin_g + cos_g * sin_a*sin_b)
                            + 1 * (sin_a*sin_g - cos_a * cos_g*sin_b)
                            + 0 * cos_b*cos_g)
                        + y * (0 * (cos_a*cos_g - sin_a * sin_b*sin_g)
                            + 1 * (cos_g*sin_a + cos_a * sin_b*sin_g)
                            - 0 * cos_b*sin_g)
                        + 1 * (0 * t[0] + 0 * t[1] + 1 * t[2])))
            *(rc + 6 * d[3] * x1 + 2 * d[2] * y1 + x1 * (2 * d[0] * x1 + 4 * d[1] * x1*(xy2)+6 * d[4] * x1*x2_y2))
            ) / (exp1* exp1) - (fx*((z*(0 * cos_b - 1 * cos_a*sin_b + 0 * sin_a*sin_b)
                - x * (0 * cos_g*sin_b + 1 * cos_a*cos_b*cos_g - 0 * cos_b*cos_g*sin_a)
                + y * (0 * sin_b*sin_g + 1 * cos_a*cos_b*sin_g - 0 * cos_b*sin_a*sin_g)
                )*(z*(0 * sin_b + ppy * cos_a*cos_b - fy * cos_b*sin_a)
                    + x * (fy*(cos_a*sin_g + cos_g * sin_a*sin_b)
                        + ppy * (sin_a*sin_g - cos_a * cos_g*sin_b)
                        + 0 * cos_b*cos_g)
                    + y * (fy*(cos_a*cos_g - sin_a * sin_b*sin_g)
                        + ppy * (cos_g*sin_a + cos_a * sin_b*sin_g)
                        - 0 * cos_b*sin_g)
                    + 1 * (0 * t[0] + fy * t[1] + ppy * t[2])
                    ) - (z*(0 * cos_b - ppy * cos_a*sin_b + fy * sin_a*sin_b)
                        - x * (0 * cos_g*sin_b + ppy * cos_a*cos_b*cos_g - fy * cos_b*cos_g*sin_a)
                        + y * (0 * sin_b*sin_g + ppy * cos_a*cos_b*sin_g - fy * cos_b*sin_a*sin_g)
                        )*(z*(0 * sin_b + 1 * cos_a*cos_b - 0 * cos_b*sin_a)
                            + x * (0 * (cos_a*sin_g + cos_g * sin_a*sin_b)
                                + 1 * (sin_a*sin_g - cos_a * cos_g*sin_b)
                                + 0 * cos_b*cos_g)
                            + y * (0 * (cos_a*cos_g - sin_a * sin_b*sin_g)
                                + 1 * (cos_g*sin_a + cos_a * sin_b*sin_g)
                                - 0 * cos_b*sin_g)
                            + 1 * (0 * t[0] + 0 * t[1] + 1 * t[2])
                            ))*(2 * d[2] * x1 + 2 * d[3] * y1 + x1 * (2 * d[0] * y1 + 4 * d[1] * y1*(xy2)+6 * d[4] * y1*x2_y2))
                ) / (fy*(exp1*exp1));

        return res;
    }

    double calculate_rotation_x_gamma_coeff(
        rotation_in_angles const & rot_angles,
        double3 const & v,
        double rc,
        double2 const & xy,
        const calib & yuy_intrin_extrin
    )
    {
        auto r = yuy_intrin_extrin.rot.rot;
        double t[3] = { yuy_intrin_extrin.trans.t1, yuy_intrin_extrin.trans.t2, yuy_intrin_extrin.trans.t3 };
        auto d = yuy_intrin_extrin.coeffs;
        auto ppx = (double)yuy_intrin_extrin.k_mat.ppx;
        auto ppy = (double)yuy_intrin_extrin.k_mat.ppy;
        auto fx = (double)yuy_intrin_extrin.k_mat.fx;
        auto fy = (double)yuy_intrin_extrin.k_mat.fy;

        auto sin_a = (double)sin( rot_angles.alpha );
        auto sin_b = (double)sin( rot_angles.beta );
        auto sin_g = (double)sin( rot_angles.gamma );

        auto cos_a = (double)cos( rot_angles.alpha );
        auto cos_b = (double)cos( rot_angles.beta );
        auto cos_g = (double)cos( rot_angles.gamma );
        auto x1 = (double)xy.x;
        auto y1 = (double)xy.y;

        auto x2 = x1 * x1;
        auto y2 = y1 * y1;
        auto xy2 = x2 + y2;
        auto x2_y2 = xy2 * xy2;

        auto x = (double)v.x;
        auto y = (double)v.y;
        auto z = (double)v.z;

        auto exp1 = z * cos_a*cos_b +
            x * (sin_a*sin_g - cos_a * cos_g*sin_b) +
            y * (cos_g*sin_a + cos_a * sin_b*sin_g) +
            t[2];

        auto res = (
            ((y*(sin_a*sin_g - cos_a * cos_g*sin_b) - x * (cos_g*sin_a + cos_a * sin_b*sin_g))*
            (z*(fx*sin_b + ppx * cos_a*cos_b) +
                x * (ppx*(sin_a*sin_g - cos_a * cos_g*sin_b) + fx * cos_b*cos_g) +
                y * (ppx*(cos_g*sin_a + cos_a * sin_b*sin_g) - fx * cos_b*sin_g) +
                (fx*t[0] + ppx * t[2])) -
                (y*(ppx* (sin_a*sin_g - cos_a * cos_g*sin_b) + fx * cos_b*cos_g) -
                    x * (ppx*(cos_g*sin_a + cos_a * sin_b*sin_g) - fx * cos_b*sin_g))*
                    (z*(cos_a*cos_b) + x * (sin_a*sin_g - cos_a * cos_g*sin_b) +
                        y * (cos_g*sin_a + cos_a * sin_b*sin_g) + t[2]))*
                        (rc + 6 * d[3] * x1 + 2 * d[2] * y1 + x1 * (2 * d[0] * x1 + 4 * d[1] * x1*(xy2)+6 * d[4] * x1*x2_y2))
            )
            /
            (exp1* exp1) + (fx*((y*(sin_a*sin_g - cos_a * cos_g*sin_b) -
                x * (cos_g*sin_a + cos_a * sin_b*sin_g))*
                (z*(ppy*cos_a*cos_b - fy * cos_b*sin_a) +
                    x * (fy*(cos_a*sin_g + cos_g * sin_a*sin_b) + ppy * (sin_a*sin_g - cos_a * cos_g*sin_b)) + y *
                    (fy*(cos_a*cos_g - sin_a * sin_b*sin_g) + ppy *
                    (cos_g*sin_a + cos_a * sin_b*sin_g)) +
                        (fy * t[1] + ppy * t[2])) -
                    (y*(fy*(cos_a*sin_g + cos_g * sin_a*sin_b) +
                        ppy * (sin_a*sin_g - cos_a * cos_g*sin_b)) - x *
                        (fy*(cos_a*cos_g - sin_a * sin_b*sin_g) + ppy * (cos_g*sin_a + cos_a * sin_b*sin_g)))*
                        (z*cos_a*cos_b + x * ((sin_a*sin_g - cos_a * cos_g*sin_b)) +
                            y * ((cos_g*sin_a + cos_a * sin_b*sin_g)) + t[2]))*(2 * d[2] * x1 + 2 * d[3] * y1 + x1 *
                            (2 * d[0] * y1 + 4 * d[1] * y1*xy2 + 6 * d[4] * y1*x2_y2)) / (fy*exp1*exp1));

        return res;
    }

    double calculate_rotation_y_alpha_coeff(
        rotation_in_angles const & rot_angles,
        double3 const & v,
        double rc,
        double2 const & xy,
        const calib & yuy_intrin_extrin
    )
    {
        auto r = yuy_intrin_extrin.rot.rot;
        double t[3] = { yuy_intrin_extrin.trans.t1, yuy_intrin_extrin.trans.t2, yuy_intrin_extrin.trans.t3 };
        auto d = yuy_intrin_extrin.coeffs;
        auto ppx = (double)yuy_intrin_extrin.k_mat.ppx;
        auto ppy = (double)yuy_intrin_extrin.k_mat.ppy;
        auto fx = (double)yuy_intrin_extrin.k_mat.fx;
        auto fy = (double)yuy_intrin_extrin.k_mat.fy;

        auto sin_a = (double)sin( rot_angles.alpha );
        auto sin_b = (double)sin( rot_angles.beta );
        auto sin_g = (double)sin( rot_angles.gamma );

        auto cos_a = (double)cos( rot_angles.alpha );
        auto cos_b = (double)cos( rot_angles.beta );
        auto cos_g = (double)cos( rot_angles.gamma );
        auto x1 = (double)xy.x;
        auto y1 = (double)xy.y;

        /* x1 = 1;
            y1 = 1;*/

        auto x2 = x1 * x1;
        auto y2 = y1 * y1;
        auto xy2 = x2 + y2;
        auto x2_y2 = xy2 * xy2;

        auto x = (double)v.x;
        auto y = (double)v.y;
        auto z = (double)v.z;


        auto exp1 = z * (cos_a*cos_b) + x * ((sin_a*sin_g - cos_a * cos_g*sin_b)) +
            y * ((cos_g*sin_a + cos_a * sin_b*sin_g)) + t[2];

        auto res = (((x*(-(cos_a*sin_g + cos_g * sin_a*sin_b)) + y * (-1 * (cos_a*cos_g - sin_a * sin_b*sin_g)) + z *
            (cos_b*sin_a))*(z*(ppy * cos_a*cos_b - fy * cos_b*sin_a) + x * (fy*(cos_a*sin_g + cos_g * sin_a*sin_b) +
                ppy * (sin_a*sin_g - cos_a * cos_g*sin_b)) + y * (fy*(cos_a*cos_g - sin_a * sin_b*sin_g) +
                    ppy * (cos_g*sin_a + cos_a * sin_b*sin_g)) + (fy * t[1] + ppy * t[2])) - (x*(fy*(sin_a*sin_g - cos_a * cos_g*sin_b) -
                        ppy * (cos_a*sin_g + cos_g * sin_a*sin_b)) + y * (fy*(cos_g*sin_a + cos_a * sin_b*sin_g) -
                            ppy * (cos_a*cos_g - sin_a * sin_b*sin_g)) + z * (fy*cos_a*cos_b + ppy * cos_b*sin_a))*
                            (z*(cos_a*cos_b) + x * ((sin_a*sin_g - cos_a * cos_g*sin_b)) + y * ((cos_g*sin_a + cos_a * sin_b*sin_g) - 0 * cos_b*sin_g) + (t[2])))*
            (rc + 2 * d[3] * x1 + 6 * d[2] * y1 + y1 * (2 * d[0] * y1 + 4 * d[1] * y1*(xy2)+6 * d[4] * y1*x2_y2))) /
            (exp1*exp1) + (fy*((x*(-(cos_a*sin_g + cos_g * sin_a*sin_b)) + y * (-(cos_a*cos_g - sin_a * sin_b*sin_g)) +
                z * (cos_b*sin_a))*(z*(fx*sin_b + ppx * cos_a*cos_b) + x * (ppx*(sin_a*sin_g - cos_a * cos_g*sin_b) + fx * cos_b*cos_g) + y *
                (ppx*(cos_g*sin_a + cos_a * sin_b*sin_g) - fx * cos_b*sin_g) + (fx*t[0] + ppx * t[2])) - (x*(-ppx * (cos_a*sin_g + cos_g * sin_a*sin_b)) +
                    y * (-ppx * (cos_a*cos_g - sin_a * sin_b*sin_g)) + z * (ppx*cos_b*sin_a))*(z*(cos_a*cos_b - 0 * cos_b*sin_a) + x * ((sin_a*sin_g - cos_a * cos_g*sin_b)) +
                        y * ((cos_g*sin_a + cos_a * sin_b*sin_g)) + (t[2])))*(2 * d[2] * x1 + 2 * d[3] * y1 + y1 * (2 * d[0] * x1 + 4 * d[1] * x1*(xy2)+6 * d[4] * x1*x2_y2))) / (fx*(exp1*exp1));

        return res;
    }

    double calculate_rotation_y_beta_coeff(
        rotation_in_angles const & rot_angles,
        double3 const & v,
        double rc,
        double2 const & xy,
        const calib & yuy_intrin_extrin
    )
    {
        auto r = yuy_intrin_extrin.rot.rot;
        double t[3] = { yuy_intrin_extrin.trans.t1, yuy_intrin_extrin.trans.t2, yuy_intrin_extrin.trans.t3 };
        auto d = yuy_intrin_extrin.coeffs;
        auto ppx = (double)yuy_intrin_extrin.k_mat.ppx;
        auto ppy = (double)yuy_intrin_extrin.k_mat.ppy;
        auto fx = (double)yuy_intrin_extrin.k_mat.fx;
        auto fy = (double)yuy_intrin_extrin.k_mat.fy;

        auto sin_a = (double)sin( rot_angles.alpha );
        auto sin_b = (double)sin( rot_angles.beta );
        auto sin_g = (double)sin( rot_angles.gamma );

        auto cos_a = (double)cos( rot_angles.alpha );
        auto cos_b = (double)cos( rot_angles.beta );
        auto cos_g = (double)cos( rot_angles.gamma );
        auto x1 = (double)xy.x;
        auto y1 = (double)xy.y;

        auto x2 = x1 * x1;
        auto y2 = y1 * y1;
        auto xy2 = x2 + y2;
        auto x2_y2 = xy2 * xy2;

        auto x = (double)v.x;
        auto y = (double)v.y;
        auto z = (double)v.z;

        auto exp1 = z * (cos_a*cos_b) + x * ((sin_a*sin_g - cos_a * cos_g*sin_b))
            + y * ((cos_g*sin_a + cos_a * sin_b*sin_g)) + (t[2]);

        auto res = -(((z*(-cos_a * sin_b) - x * (cos_a*cos_b*cos_g)
            + y * (cos_a*cos_b*sin_g))*(z*(ppy * cos_a*cos_b - fy * cos_b*sin_a)
                + x * (fy*(cos_a*sin_g + cos_g * sin_a*sin_b) + ppy * (sin_a*sin_g - cos_a * cos_g*sin_b))
                + y * (fy*(cos_a*cos_g - sin_a * sin_b*sin_g) + ppy * (cos_g*sin_a + cos_a * sin_b*sin_g))
                + (fy * t[1] + ppy * t[2])) - (z*(0 * cos_b - ppy * cos_a*sin_b + fy * sin_a*sin_b)
                    - x * (ppy * cos_a*cos_b*cos_g - fy * cos_b*cos_g*sin_a) + y * (ppy * cos_a*cos_b*sin_g - fy * cos_b*sin_a*sin_g))*
                    (z*(cos_a*cos_b) + x * ((sin_a*sin_g - cos_a * cos_g*sin_b)) + y * ((cos_g*sin_a + cos_a * sin_b*sin_g)) + t[2]))
            *(rc + 2 * d[3] * x1 + 6 * d[2] * y1 + y1 * (2 * d[0] * y1 + 4 * d[1] * y1*(xy2)+6 * d[4] * y1*x2_y2))) /
            (exp1*exp1) - (fy*((z*(-cos_a * sin_b) - x * (cos_a*cos_b*cos_g) + y * (cos_a*cos_b*sin_g))*(z*(fx*sin_b + ppx * cos_a*cos_b)
                + x * (ppx * (sin_a*sin_g - cos_a * cos_g*sin_b) + fx * cos_b*cos_g) + y * (+ppx * (cos_g*sin_a + cos_a * sin_b*sin_g) - fx * cos_b*sin_g)
                + (fx*t[0] + ppx * t[2])) - (z*(fx*cos_b - ppx * cos_a*sin_b) - x * (fx*cos_g*sin_b + ppx * cos_a*cos_b*cos_g) + y
                    * (fx*sin_b*sin_g + ppx * cos_a*cos_b*sin_g))*(z*(cos_a*cos_b) + x * (sin_a*sin_g - cos_a * cos_g*sin_b) + y
                        * (cos_g*sin_a + cos_a * sin_b*sin_g) + t[2]))*(2 * d[2] * x1 + 2 * d[3] * y1 + y1 *
                        (2 * d[0] * x1 + 4 * d[1] * x1*(xy2)+6 * d[4] * x1*x2_y2))) / (fx*(exp1*exp1));

        return res;

    }

    double calculate_rotation_y_gamma_coeff(
        rotation_in_angles const & rot_angles,
        double3 const & v,
        double rc,
        double2 const & xy,
        const calib & yuy_intrin_extrin
    )
    {
        auto r = yuy_intrin_extrin.rot.rot;
        double t[3] = { yuy_intrin_extrin.trans.t1, yuy_intrin_extrin.trans.t2, yuy_intrin_extrin.trans.t3 };
        auto d = yuy_intrin_extrin.coeffs;
        auto ppx = (double)yuy_intrin_extrin.k_mat.ppx;
        auto ppy = (double)yuy_intrin_extrin.k_mat.ppy;
        auto fx = (double)yuy_intrin_extrin.k_mat.fx;
        auto fy = (double)yuy_intrin_extrin.k_mat.fy;

        auto sin_a = (double)sin( rot_angles.alpha );
        auto sin_b = (double)sin( rot_angles.beta );
        auto sin_g = (double)sin( rot_angles.gamma );

        auto cos_a = (double)cos( rot_angles.alpha );
        auto cos_b = (double)cos( rot_angles.beta );
        auto cos_g = (double)cos( rot_angles.gamma );
        auto x1 = (double)xy.x;
        auto y1 = (double)xy.y;

        auto x2 = x1 * x1;
        auto y2 = y1 * y1;
        auto xy2 = x2 + y2;
        auto x2_y2 = xy2 * xy2;

        auto x = v.x;
        auto y = v.y;
        auto z = v.z;

        auto exp1 = z * (cos_a*cos_b) + x * (+(sin_a*sin_g - cos_a * cos_g*sin_b))
            + y * ((cos_g*sin_a + cos_a * sin_b*sin_g)) + t[2];

        auto res = (((y*(+(sin_a*sin_g - cos_a * cos_g*sin_b)) - x * ((cos_g*sin_a + cos_a * sin_b*sin_g)))
            *(z*(ppy * cos_a*cos_b - fy * cos_b*sin_a) + x * (fy*(cos_a*sin_g + cos_g * sin_a*sin_b)
                + ppy * (sin_a*sin_g - cos_a * cos_g*sin_b)) + y * (fy*(cos_a*cos_g - sin_a * sin_b*sin_g)
                    + ppy * (cos_g*sin_a + cos_a * sin_b*sin_g))
                + (fy * t[1] + ppy * t[2])) - (y*(fy*(cos_a*sin_g + cos_g * sin_a*sin_b) + ppy * (sin_a*sin_g - cos_a * cos_g*sin_b))
                    - x * (fy*(cos_a*cos_g - sin_a * sin_b*sin_g) + ppy * (cos_g*sin_a + cos_a * sin_b*sin_g)))*(z*(cos_a*cos_b)
                        + x * ((sin_a*sin_g - cos_a * cos_g*sin_b)) + y * (+(cos_g*sin_a + cos_a * sin_b*sin_g)) + (t[2])))
            *(rc + 2 * d[3] * x1 + 6 * d[2] * y1 + y1 * (2 * d[0] * y1 + 4 * d[1] * y1*(xy2)+6 * d[4] * y1*x2_y2)))
            / (exp1*exp1) + (fy*((y*(+(sin_a*sin_g - cos_a * cos_g*sin_b)) - x * (+(cos_g*sin_a + cos_a * sin_b*sin_g)))
                *(z*(fx*sin_b + ppx * cos_a*cos_b) + x * (ppx * (sin_a*sin_g - cos_a * cos_g*sin_b) + fx * cos_b*cos_g)
                    + y * (+ppx * (cos_g*sin_a + cos_a * sin_b*sin_g) - fx * cos_b*sin_g) + (fx*t[0] + ppx * t[2]))
                - (y*(ppx * (sin_a*sin_g - cos_a * cos_g*sin_b) + fx * cos_b*cos_g) - x
                    * (+ppx * (cos_g*sin_a + cos_a * sin_b*sin_g) - fx * cos_b*sin_g))*(z*(cos_a*cos_b)
                        + x * ((sin_a*sin_g - cos_a * cos_g*sin_b)) + y * ((cos_g*sin_a + cos_a * sin_b*sin_g)) + (t[2])))
                *(2 * d[2] * x1 + 2 * d[3] * y1 + y1 * (2 * d[0] * x1 + 4 * d[1] * x1*(xy2)+6 * d[4] * x1*x2_y2))
                ) / (fx*(exp1*exp1));

        return res;
    }


    coeffs<rotation_in_angles> calc_rotation_coefs(
        const z_frame_data & z_data,
        const yuy2_frame_data & yuy_data,
        const calib & yuy_intrin_extrin,
        const std::vector<double>& rc,
        const std::vector<double2>& xy
    )
    {
        coeffs<rotation_in_angles> res;
        auto engles = extract_angles_from_rotation( yuy_intrin_extrin.rot.rot );
        auto v = z_data.vertices;
        res.x_coeffs.resize( v.size() );
        res.y_coeffs.resize( v.size() );

        for( auto i = 0; i < v.size(); i++ )
        {
            res.x_coeffs[i].alpha = calculate_rotation_x_alpha_coeff( engles, v[i], rc[i], xy[i], yuy_intrin_extrin );
            res.x_coeffs[i].beta = calculate_rotation_x_beta_coeff( engles, v[i], rc[i], xy[i], yuy_intrin_extrin );
            res.x_coeffs[i].gamma = calculate_rotation_x_gamma_coeff( engles, v[i], rc[i], xy[i], yuy_intrin_extrin );

            res.y_coeffs[i].alpha = calculate_rotation_y_alpha_coeff( engles, v[i], rc[i], xy[i], yuy_intrin_extrin );
            res.y_coeffs[i].beta = calculate_rotation_y_beta_coeff( engles, v[i], rc[i], xy[i], yuy_intrin_extrin );
            res.y_coeffs[i].gamma = calculate_rotation_y_gamma_coeff( engles, v[i], rc[i], xy[i], yuy_intrin_extrin );
        }

        return res;
    }

    translation calculate_translation_x_coeff( double3 v, double rc, double2 xy, const calib & yuy_intrin_extrin )
    {
        translation res;

        auto x1 = (double)xy.x;
        auto y1 = (double)xy.y;

        auto x2 = x1 * x1;
        auto y2 = y1 * y1;
        auto xy2 = x2 + y2;
        auto x2_y2 = xy2 * xy2;

        auto r = yuy_intrin_extrin.rot.rot;
        double t[3] = { yuy_intrin_extrin.trans.t1, yuy_intrin_extrin.trans.t2, yuy_intrin_extrin.trans.t3 };
        auto d = yuy_intrin_extrin.coeffs;
        auto ppx = (double)yuy_intrin_extrin.k_mat.ppx;
        auto ppy = (double)yuy_intrin_extrin.k_mat.ppy;
        auto fx = (double)yuy_intrin_extrin.k_mat.fx;
        auto fy = (double)yuy_intrin_extrin.k_mat.fy;

        auto x = (double)v.x;
        auto y = (double)v.y;
        auto z = (double)v.z;

        auto exp1 = rc + 6 * (double)d[3] * x1 + 2 * (double)d[2] * y1 + x1 *
            (2 * (double)d[0] * x1 + 4 * (double)d[1] * x1*(xy2)+6 * (double)d[4] * x1*(x2_y2));
        auto exp2 = fx * (double)r[2] * x + fx * (double)r[5] * y + fx * (double)r[8] * z + fx * (double)t[2];
        auto exp3 = (double)r[2] * x + (double)r[5] * y + (double)r[8] * z + (double)t[2];

        res.t1 = (exp1 * exp2) / (exp3 * exp3);

        auto exp4 = 2 * (double)d[2] * x1 + 2 * (double)d[3] * y1 + x1 *
            (2 * (double)d[0] * y1 + 4 * (double)d[1] * y1*xy2 + 6 * (double)d[4] * y1*x2_y2);
        auto exp5 = -fy * (double)r[2] * x - fy * (double)r[5] * y - fy * (double)r[8] * z - fy * (double)t[2];
        auto exp6 = (double)r[2] * x + (double)r[5] * y + (double)r[8] * z + (double)t[2];

        res.t2 = -(fx*exp4 * exp5) / (fy*exp6 * exp6);

        exp1 = rc + 6 * (double)d[3] * x1 + 2 * (double)d[2] * y1 + x1
            * (2 * (double)d[0] * x1 + 4 * (double)d[1] * x1*(xy2)+6 * (double)d[4] * x1*x2_y2);
        exp2 = fx * (double)r[0] * x + fx * (double)r[3] * y + fx * (double)r[6] * z + fx * (double)t[0];
        exp3 = (double)r[2] * x + (double)r[5] * y + (double)r[8] * z + (double)t[2];
        exp4 = fx * (2 * (double)d[2] * x1 + 2 * (double)d[3] * y1 +
            x1 * (2 * (double)d[0] * y1 + 4 * (double)d[1] * y1*(xy2)+6 * (double)d[4] * y1*x2_y2));
        exp5 = +fy * (double)r[1] * x + fy * (double)r[4] * y + fy * (double)r[7] * z + fy * (double)t[1];
        exp6 = (double)r[2] * x + (double)r[5] * y + (double)r[8] * z + (double)t[2];

        res.t3 = -(exp1 * exp2) / (exp3 * exp3) - (exp4 * exp5) / (fy*exp6 * exp6);

        return res;

    }

    translation calculate_translation_y_coeff( double3 v, double rc, double2 xy, const calib& yuy_intrin_extrin )
    {
        translation res;

        auto x1 = (double)xy.x;
        auto y1 = (double)xy.y;

        auto x2 = x1 * x1;
        auto y2 = y1 * y1;
        auto xy2 = x2 + y2;
        auto x2_y2 = xy2 * xy2;

        auto r = yuy_intrin_extrin.rot.rot;
        double t[3] = { yuy_intrin_extrin.trans.t1, yuy_intrin_extrin.trans.t2, yuy_intrin_extrin.trans.t3 };
        auto d = yuy_intrin_extrin.coeffs;
        auto ppx = (double)yuy_intrin_extrin.k_mat.ppx;
        auto ppy = (double)yuy_intrin_extrin.k_mat.ppy;
        auto fx = (double)yuy_intrin_extrin.k_mat.fx;
        auto fy = (double)yuy_intrin_extrin.k_mat.fy;

        auto x = (double)v.x;
        auto y = (double)v.y;
        auto z = (double)v.z;

        auto exp1 = (double)r[2] * x + (double)r[5] * y + (double)r[8] * z + (double)t[2];
        auto exp2 = fx * (double)r[2] * x + fx * (double)r[5] * y + fx * (double)r[8] * z + fx * (double)t[2];
        auto exp3 = fx * exp1 * exp1;
        auto exp4 = fy * (2 * (double)d[2] * x1 + 2 * (double)d[3] * y1 + y1 *
            (2 * (double)d[0] * x1 + 4 * (double)d[1] * x1*xy2 + 6 * (double)d[4] * x1*x2_y2))*exp2;

        res.t1 = exp4 / exp3;

        exp1 = rc + 2 * (double)d[3] * x1 + 6 * (double)d[2] * y1 + y1 *
            (2 * (double)d[0] * y1 + 4 * (double)d[1] * y1*xy2 + 6 * (double)d[4] * y1*x2_y2);
        exp2 = -fy * (double)r[2] * x - fy * (double)r[5] * y - fy * (double)r[8] * z - fy * (double)t[2];
        exp3 = (double)r[2] * x + (double)r[5] * y + (double)r[8] * z + (double)t[2];

        res.t2 = -(exp1*exp2) / (exp3* exp3);

        exp1 = rc + 2 * (double)d[3] * x1 + 6 * (double)d[2] * y1 + y1 * (2 * (double)d[0] * y1 + 4 * (double)d[1] * y1*xy2 + 6 * (double)d[4] * y1*x2_y2);
        exp2 = fy * (double)r[1] * x + fy * (double)r[4] * y + fy * (double)r[7] * z + fy * (double)t[1];
        exp3 = (double)r[2] * x + (double)r[5] * y + (double)r[8] * z + (double)t[2];
        exp4 = 2 * (double)d[2] * x1 + 2 * (double)d[3] * y1 + y1 *
            (2 * (double)d[0] * x1 + 4 * (double)d[1] * x1*xy2 + 6 * (double)d[4] * x1*x2_y2);

        auto exp5 = fx * (double)r[0] * x + fx * (double)r[3] * y + fx * (double)r[6] * z + fx * (double)t[0];
        auto exp6 = (double)r[2] * x + (double)r[5] * y + (double)r[8] * z + (double)t[2];
        auto exp7 = fx * exp6 * exp6;

        res.t3 = -(exp1 * exp2) / (exp3 * exp3) - (fy*(exp4)*(exp5)) / exp7;

        return res;
    }

    coeffs< translation > calc_translation_coefs( const z_frame_data& z_data, const yuy2_frame_data& yuy_data, const calib & yuy_intrin_extrin, const std::vector<double>& rc, const std::vector<double2>& xy )
    {
        coeffs<translation> res;

        auto v = z_data.vertices;
        res.y_coeffs.resize( v.size() );
        res.x_coeffs.resize( v.size() );

        for( auto i = 0; i < rc.size(); i++ )
        {
            res.y_coeffs[i] = calculate_translation_y_coeff( v[i], rc[i], xy[i], yuy_intrin_extrin );
            res.x_coeffs[i] = calculate_translation_x_coeff( v[i], rc[i], xy[i], yuy_intrin_extrin );

        }

        return res;
    }

    k_matrix calculate_k_gradients_x_coeff( double3 v, double rc, double2 xy, const calib & yuy_intrin_extrin )
    {
        k_matrix res;

        auto r = yuy_intrin_extrin.rot.rot;
        double t[3] = { yuy_intrin_extrin.trans.t1, yuy_intrin_extrin.trans.t2, yuy_intrin_extrin.trans.t3 };
        auto d = yuy_intrin_extrin.coeffs;
        auto ppx = (double)yuy_intrin_extrin.k_mat.ppx;
        auto ppy = (double)yuy_intrin_extrin.k_mat.ppy;
        auto fx = (double)yuy_intrin_extrin.k_mat.fx;
        auto fy = (double)yuy_intrin_extrin.k_mat.fy;

        auto x1 = (double)xy.x;
        auto y1 = (double)xy.y;
        auto x = (double)v.x;
        auto y = (double)v.y;
        auto z = (double)v.z;

        auto x2 = x1 * x1;
        auto y2 = y1 * y1;
        auto xy2 = x2 + y2;
        auto x2_y2 = xy2 * xy2;

        res.fx = ((r[0] * x + r[3] * y + r[6] * z + t[0])
            *(rc + 6 * d[3] * x1 + 2 * d[2] * y1 + x1 * (2 * d[0] * x1 + 4 * d[1] * x1*(xy2)+6 * d[4] * x1*x2_y2)))
            / (x*r[2] + y * r[5] + z * r[8] + t[2]);


        res.ppx = ((r[2] * x + r[5] * y + r[8] * z + t[2] * 1)
            *(rc + 6 * d[3] * x1 + 2 * d[2] * y1 + x1 * (2 * d[0] * x1 + 4 * d[1] * x1*(xy2)+6 * d[4] * x1*x2_y2))
            ) / (x*r[2] + y * (r[5]) + z * (r[8]) + (t[2]));


        res.fy = (fx*(2 * d[2] * x1 + 2 * d[3] * y1 + x1 * (2 * d[0] * y1 + 4 * d[1] * y1*(xy2)+6 * d[4] * y1*x2_y2))*
            (r[1] * x + r[4] * y + r[7] * z + t[1] * 1))
            / (fy*(x*(r[2]) + y * (r[5]) + z * (r[8]) + (t[2])));

        res.ppy = (fx*(2 * d[2] * x1 + 2 * d[3] * y1 + x1 * (2 * d[0] * y1 + 4 * d[1] * y1*(xy2)+6 * d[4] * y1*x2_y2))*(r[2] * x + r[5] * y + r[8] * z + t[2] * 1))
            / (fy*(x*(r[2]) + y * (r[5]) + z * (r[8]) + (t[2])));

        return res;
    }

    k_matrix calculate_k_gradients_y_coeff( double3 v, double rc, double2 xy, const calib & yuy_intrin_extrin )
    {
        k_matrix res;

        auto r = yuy_intrin_extrin.rot.rot;
        double t[3] = { yuy_intrin_extrin.trans.t1, yuy_intrin_extrin.trans.t2, yuy_intrin_extrin.trans.t3 };
        auto d = yuy_intrin_extrin.coeffs;
        auto ppx = (double)yuy_intrin_extrin.k_mat.ppx;
        auto ppy = (double)yuy_intrin_extrin.k_mat.ppy;
        auto fx = (double)yuy_intrin_extrin.k_mat.fx;
        auto fy = (double)yuy_intrin_extrin.k_mat.fy;

        auto x1 = (double)xy.x;
        auto y1 = (double)xy.y;
        auto x = (double)v.x;
        auto y = (double)v.y;
        auto z = (double)v.z;

        auto x2 = x1 * x1;
        auto y2 = y1 * y1;
        auto xy2 = x2 + y2;
        auto x2_y2 = xy2 * xy2;

        res.fx = (fy*(2 * d[2] * x1 + 2 * d[3] * y1 + y1 * (2 * d[0] * x1 + 4 * d[1] * x1*(xy2)+6 * d[4] * x1*x2_y2))
            *(r[0] * x + r[3] * y + r[6] * z + t[0]))
            / (fx*(x*r[2] + y * r[5] + z * r[8] + t[2]));

        res.ppx = (fy*(2 * d[2] * x1 + 2 * d[3] * y1 + y1 * (2 * d[0] * x1 + 4 * d[1] * x1*(xy2)+6 * d[4] * x1*x2_y2))
            *(r[2] * x + r[5] * y + r[8] * z + t[2]))
            / (fx*(x*r[2] + y * r[5] + z * (r[8]) + (t[2])));

        res.fy = ((r[1] * x + r[4] * y + r[7] * z + t[1])*
            (rc + 2 * d[3] * x1 + 6 * d[2] * y1 + y1 * (2 * d[0] * y1 + 4 * d[1] * y1*xy2 + 6 * d[4] * y1*x2_y2)))
            / (x*r[2] + y * r[5] + z * r[8] + t[2]);

        res.ppy = ((r[2] * x + r[5] * y + r[8] * z + t[2])*
            (rc + 2 * d[3] * x1 + 6 * d[2] * y1 + y1 * (2 * d[0] * y1 + 4 * d[1] * y1*xy2 + 6 * d[4] * y1*x2_y2)))
            / (x*r[2] + y * r[5] + z * r[8] + t[2]);

        return res;
    }

    coeffs< k_matrix > calc_k_gradients_coefs( const z_frame_data & z_data, const yuy2_frame_data & yuy_data, const calib & yuy_intrin_extrin, const std::vector<double>& rc, const std::vector<double2>& xy )
    {
        coeffs<k_matrix> res;
        auto v = z_data.vertices;
        res.x_coeffs.resize( v.size() );
        res.y_coeffs.resize( v.size() );

        for( auto i = 0; i < v.size(); i++ )
        {
            res.x_coeffs[i] = calculate_k_gradients_x_coeff( v[i], rc[i], xy[i], yuy_intrin_extrin );
            res.y_coeffs[i] = calculate_k_gradients_y_coeff( v[i], rc[i], xy[i], yuy_intrin_extrin );
        }

        return res;
    }

}
}
}
