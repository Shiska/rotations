#if defined _inc_rotation_euler_
	#endinput
#else
	#define _inc_rotation_euler_
#endif

enum E_EULER {
    Float: E_EULER_ALPHA,
    Float: E_EULER_BETA,
    Float: E_EULER_GAMMA
}
/// <summary>Euler angle enum (alpha, beta, gamma)</summary>

#if !defined E_AANGLE
    #include "rotation_axis_angle"
#elseif !defined E_QUAT
    #include "rotation_quaternion"
#elseif !defined E_RMATRIX
    #include "rotation_rotation_matrix"
#endif

stock SetRotationFromEuler(euler[E_EULER], rotationtype: type, & Float: alpha, & Float: beta, & Float: gamma) {
    /// <summary>Set the rotation array from euler angle</summary>
    /// <param name="euler">Euler angle array [<ref name="E_EULER"/>]</param>
    /// <param name="type">Source rotation type, unused</param>
    /// <param name="alpha">First source euler angle</param>
    /// <param name="beta">Second source euler angle</param>
    /// <param name="gamma">Third source euler angle</param>
    /// <remarks>Used internally</remarks>

    debug("SetRotationFromEuler(%f, %f, %f)", alpha, beta, gamma);

    #pragma unused type

    euler[E_EULER_ALPHA] = alpha;
    euler[E_EULER_BETA]  = beta;
    euler[E_EULER_GAMMA] = gamma;
}

#define EULER_SINGULARITY_THRESHOLD 0.9999
#define EULER_SINGULARITY_THRESHOLD_H 0.49995 // EULER_SINGULARITY_THRESHOLD / 2.0 - preprocessor can't handle floats :/

stock ConvertAxisAngleToEuler(aangle[E_AANGLE], rotationtype: type[2], euler[E_EULER]) {
    /// <summary>Convert the given axis angle into euler angle</summary>
    /// <param name="aangle">Source axis angle [<ref name="E_AANGLE"/>]</param>
    /// <param name="type">Rotation type</param>
    /// <param name="euler">Dest euler angle [<ref name="E_EULER"/>]</param>
    /// <remarks>Used internally</remarks>
    /// <remarks>Not optimized, uses <ref name="ConvertAxisAngleToQuat"/> and <ref name="ConvertQuatToEuler"/> for the conversion</remarks>

    new quat[E_QUAT]; // leaving that unoptimised, see no use for it

    type[0] = rtype_quaternion;

    ConvertAxisAngleToQuat(aangle, rotationtype: {rtype_axis_angle, rtype_quaternion}, quat);
    ConvertQuatToEuler(quat, type, euler);

    debug("ConvertAxisAngleToEuler(%f, %f, %f)", euler[E_EULER_ALPHA], euler[E_EULER_BETA], euler[E_EULER_GAMMA]);
}

stock ConvertEulerToEuler(src[E_EULER], rotationtype: type[2], dest[E_EULER]) {
    /// <summary>Convert the given euler angle into euler angle</summary>
    /// <param name="src">Source euler angle [<ref name="E_EULER"/>]</param>
    /// <param name="type">Rotation type</param>
    /// <param name="dest">Dest euler angle [<ref name="E_EULER"/>]</param>
    /// <remarks>Used internally</remarks>
    /// <remarks>Not optimized, uses <ref name="ConvertEulerToQuat"/> and <ref name="ConvertQuatToEuler"/> for the conversion</remarks>
    /// <remarks>Doesn't check for same rotation type, that case should be caught in <ref name="ConvertRotation"/></remarks>

    new quat[E_QUAT]; // to much work I guess
    new rotationtype: type2[2] = rtype_quaternion;

    type2[1] = type[1];
    type[1] = rtype_quaternion;

    ConvertEulerToQuat(src, type, quat);
    ConvertQuatToEuler(quat, type2, dest);

    debug("ConvertEulerToEuler(%f, %f, %f)", dest[E_EULER_ALPHA], dest[E_EULER_BETA], dest[E_EULER_GAMMA]);
}

stock ConvertQuatToEuler(quat[E_QUAT], rotationtype: type[2], euler[E_EULER]) {
    /// <summary>Convert the given quaternion into euler angle</summary>
    /// <param name="quat">Source quaternion [<ref name="E_QUAT"/>]</param>
    /// <param name="type">Rotation type</param>
    /// <param name="euler">Dest euler angle [<ref name="E_EULER"/>]</param>
    /// <remarks>Used internally</remarks>
    /// <remarks>Based on <ref name="ConvertQuatToMatrix"/> and <ref name="ConvertMatrixToEuler"/> but without calculating all matrix items</remarks>

    // Funny thing, for the Tait-Bryan angles, I didn't use alpha beta gamma before, also first second third rotation
    // Instead I had rX rY rZ which matched the X Y or Z rotation whick kept we wondering what the fuck is wrong here :/
    // Although it is a nice feature to have "matching" notation but it isn't the typical so I changed it to the greek letters

    new Float: w = quat[E_QUAT_W];
    new Float: x = quat[E_QUAT_X];
    new Float: y = quat[E_QUAT_Y];
    new Float: z = quat[E_QUAT_Z];

    switch(type[1]) {
        case rtype_euler_xzx: {
            new Float: m00 = 0.5 - y * y - z * z;

            if(m00 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(y * z - w * x, 0.5 - x * x - z * z);
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m00 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(y * z - w * x, 0.5 - x * x - z * z);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(x * z + w * y, x * y - w * z);
                euler[E_EULER_BETA]  = acos(2.0 * m00);
                euler[E_EULER_GAMMA] = atan2(x * z - w * y, -(x * y + w * z));
            }
        }
        case rtype_euler_xyx: {
            new Float: m00 = 0.5 - y * y - z * z;

            if(m00 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(y * z - w * x, 0.5 - x * x - z * z);
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m00 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(y * z - w * x, 0.5 - x * x - z * z);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(x * y - w * z, -(x * z + w * y));
                euler[E_EULER_BETA]  = acos(2.0 * m00);
                euler[E_EULER_GAMMA] = atan2(x * y + w * z, x * z - w * y);
            }
        }
        case rtype_euler_yxy: {
            new Float: m11 = 0.5 - x * x - z * z;

            if(m11 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * z - w * y, 0.5 - x * x - y * y);
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m11 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * z - w * y, 0.5 - x * x - y * y);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(x * y + w * z, y * z - w * x);
                euler[E_EULER_BETA]  = acos(2.0 * m11);
                euler[E_EULER_GAMMA] = atan2(x * y - w * z, -(y * z + w * x));
            }
        }
        case rtype_euler_yzy: {
            new Float: m11 = 0.5 - x * x - z * z;

            if(m11 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * z - w * y, 0.5 - x * x - y * y);
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m11 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * z - w * y, 0.5 - x * x - y * y);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(y * z - w * x, -(x * y + w * z));
                euler[E_EULER_BETA]  = acos(2.0 * m11);
                euler[E_EULER_GAMMA] = atan2(y * z + w * x, x * y - w * z);
            }
        }
        case rtype_euler_zyz: {
            new Float: m22 = 0.5 - x * x - y * y;

            if(m22 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * y - w * z, 0.5 - y * y - z * z);
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m22 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * y - w * z, 0.5 - y * y - z * z);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(y * z + w * x, x * z - w * y);
                euler[E_EULER_BETA]  = acos(2.0 * m22);
                euler[E_EULER_GAMMA] = atan2(y * z - w * x, -(x * z + w * y));
            }
        }
        case rtype_euler_zxz: {
            new Float: m22 = 0.5 - x * x - y * y;

            if(m22 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * y - w * z, 0.5 - y * y - z * z);
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m22 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * y - w * z, 0.5 - y * y - z * z);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(x * z - w * y, -(y * z + w * x));
                euler[E_EULER_BETA]  = acos(2.0 * m22);
                euler[E_EULER_GAMMA] = atan2(x * z + w * y, y * z - w * x);
            }
        }
        case rtype_euler_xzy: {
            new Float: m01 = x * y + w * z;

            if(m01 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * z + w * y, x * y - w * z);
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m01 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * z + w * y, x * y - w * z);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_BETA] = asin(-2.0 * m01);

                m01 = 0.5 - z * z;

                euler[E_EULER_ALPHA] = atan2(y * z - w * x, m01 - x * x);
                euler[E_EULER_GAMMA] = atan2(x * z - w * y, m01 - y * y);
            }
        }
        case rtype_euler_xyz: {
            new Float: m02 = x * z - w * y;

            if(m02 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(w * x - y * z, x * z + w * y);
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m02 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(y * z - w * x, x * z + w * y);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_BETA]  = asin(2.0 * m02);

                m02 = 0.5 - y * y;

                euler[E_EULER_ALPHA] = -atan2(y * z + w * x, m02 - x * x);
                euler[E_EULER_GAMMA] = -atan2(x * y + w * z, m02 - z * z);
            }
        }
        case rtype_euler_yxz: {
            new Float: m12 = y * z + w * x;

            if(m12 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * y + w * z, y * z - w * x);
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m12 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * y + w * z, y * z - w * x);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_BETA]  = asin(-2.0 * m12);

                m12 = 0.5 - x * x;

                euler[E_EULER_ALPHA] = atan2(x * z - w * y, m12 - y * y);
                euler[E_EULER_GAMMA] = atan2(x * y - w * z, m12 - z * z);
            }
        }
        case rtype_euler_yzx: {
            new Float: m10 = x * y - w * z;

            if(m10 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(w * y - x * z, x * y + w * z);
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m10 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * z - w * y, x * y + w * z);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_BETA] = asin(2.0 * m10);

                m10 = 0.5 - z * z;

                euler[E_EULER_ALPHA] = -atan2(x * z + w * y, m10 - y * y);
                euler[E_EULER_GAMMA] = -atan2(y * z + w * x, m10 - x * x);
            }
        }
        case rtype_euler_zyx: {
            new Float: m20 = x * z + w * y;

            if(m20 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(y * z + w * x, x * z - w * y);
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m20 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(y * z + w * x, x * z - w * y);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_BETA]  = asin(-2.0 * m20);

                m20 = 0.5 - y * y;

                euler[E_EULER_ALPHA] = atan2(x * y - w * z, m20 - z * z);
                euler[E_EULER_GAMMA] = atan2(y * z - w * x, m20 - x * x);
            }
        }
        case rtype_euler_zxy: {
            new Float: m21 = y * z - w * x;

            if(m21 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(w * y - x * z, y * z + w * x);
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m21 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(w * y - x * z, y * z + w * x);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_BETA] = asin(2.0 * m21);

                m21 = 0.5 - x * x;

                euler[E_EULER_ALPHA] = -atan2(x * y + w * z, m21 - z * z);
                euler[E_EULER_GAMMA] = -atan2(x * z + w * y, m21 - y * y);
            }
        }
        case rtype_euler_re_xzy: {
            new Float: m01 = x * y + w * z;

            if(m01 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * z + w * y, x * y - w * z);
                euler[E_EULER_BETA]  = 0.0;
                euler[E_EULER_GAMMA] = 90.0;
            } else if(m01 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(x * z + w * y, x * y - w * z);
                euler[E_EULER_BETA]  = 0.0;
                euler[E_EULER_GAMMA] = -90.0;
            } else {
                euler[E_EULER_GAMMA] = asin(-2.0 * m01);

                m01 = 0.5 - z * z;

                euler[E_EULER_ALPHA] = atan2(y * z - w * x, m01 - x * x);
                euler[E_EULER_BETA]  = atan2(x * z - w * y, m01 - y * y);
            }
        }
        case rtype_euler_re_yxz: {
            new Float: m12 = y * z + w * x;

            if(m12 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = 90.0;
                euler[E_EULER_BETA]  = atan2(x * y + w * z, y * z - w * x);
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m12 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = -90.0;
                euler[E_EULER_BETA]  = atan2(x * y + w * z, y * z - w * x);
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = asin(-2.0 * m12);

                m12 = 0.5 - x * x;

                euler[E_EULER_BETA]  = atan2(x * z - w * y, m12 - y * y);
                euler[E_EULER_GAMMA] = atan2(x * y - w * z, m12 - z * z);
            }
        }
        case rtype_euler_re_yzx: {
            new Float: m10 = x * y - w * z;

            if(m10 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = 0.0;
                euler[E_EULER_BETA]  = atan2(w * y - x * z, x * y + w * z);
                euler[E_EULER_GAMMA] = 90.0;
            } else if(m10 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = 0.0;
                euler[E_EULER_BETA]  = atan2(x * z - w * y, x * y + w * z);
                euler[E_EULER_GAMMA] = -90.0;
            } else {
                euler[E_EULER_GAMMA] = asin(2.0 * m10);

                m10 = 0.5 - z * z;

                euler[E_EULER_ALPHA] = -atan2(y * z + w * x, m10 - x * x);
                euler[E_EULER_BETA]  = -atan2(x * z + w * y, m10 - y * y);
            }
        }
        case rtype_euler_re_zyx: {
            new Float: m20 = x * z + w * y;

            if(m20 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = 0.0;
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = atan2(y * z + w * x, x * z - w * y);
            } else if(m20 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = 0.0;
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = atan2(y * z + w * x, x * z - w * y);
            } else {
                euler[E_EULER_BETA]  = asin(-2.0 * m20);

                m20 = 0.5 - y * y;

                euler[E_EULER_ALPHA] = atan2(y * z - w * x, m20 - x * x);
                euler[E_EULER_GAMMA] = atan2(x * y - w * z, m20 - z * z);
            }
        }
        case rtype_euler_re_zxy: {
            new Float: m21 = y * z - w * x;

            if(m21 > EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = 90.0;
                euler[E_EULER_BETA]  = 0.0;
                euler[E_EULER_GAMMA] = atan2(w * y - x * z, y * z + w * x);
            } else if(m21 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = -90.0;
                euler[E_EULER_BETA]  = 0.0;
                euler[E_EULER_GAMMA] = atan2(w * y - x * z, y * z + w * x);
            } else {
                euler[E_EULER_ALPHA] = asin(2.0 * m21);

                m21 = 0.5 - x * x;

                euler[E_EULER_BETA]  = -atan2(x * z + w * y, m21 - y * y);
                euler[E_EULER_GAMMA] = -atan2(x * y + w * z, m21 - z * z);
            }
        }
    }
    debug("ConvertQuatToEuler(%f, %f, %f)", euler[E_EULER_ALPHA], euler[E_EULER_BETA], euler[E_EULER_GAMMA]);
}

stock ConvertMatrixToEuler(rmatrix[E_RMATRIX], rotationtype: type[2], euler[E_EULER]) {
    /// <summary>Convert the given rotation matrix into euler angle</summary>
    /// <param name="rmatrix">Source rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="type">Rotation type</param>
    /// <param name="euler">Dest euler angle [<ref name="E_EULER"/>]</param>
    /// <remarks>Used internally</remarks>

    switch(type[1]) {
        case rtype_euler_xzx: {
            new Float: m00 = rmatrix[E_RMATRIX_00];

            if(m00 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_21], rmatrix[E_RMATRIX_11]); // -12, 22
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m00 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_21], rmatrix[E_RMATRIX_11]);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_02], -rmatrix[E_RMATRIX_01]);
                euler[E_EULER_BETA]  = acos(m00); // order swapped to prevent overwrite
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_10]);
            }
        }
        case rtype_euler_xyx: {
            new Float: m00 = rmatrix[E_RMATRIX_00];

            if(m00 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_21], rmatrix[E_RMATRIX_11]); // -12, 22
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m00 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_21], rmatrix[E_RMATRIX_11]);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_01], rmatrix[E_RMATRIX_02]);
                euler[E_EULER_BETA]  = acos(m00); // order swapped to prevent overwrite
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_10], -rmatrix[E_RMATRIX_20]);
            }
        }
        case rtype_euler_yxy: {
            new Float: m11 = rmatrix[E_RMATRIX_11];

            if(m11 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_22]); // -20, 00
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m11 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_22]);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_01], rmatrix[E_RMATRIX_21]);
                euler[E_EULER_BETA]  = acos(m11);
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_10], -rmatrix[E_RMATRIX_12]);
            }
        }
        case rtype_euler_yzy: {
            new Float: m11 = rmatrix[E_RMATRIX_11];

            if(m11 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_22]); // -20, 00
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m11 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_22]);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_21], -rmatrix[E_RMATRIX_01]);
                euler[E_EULER_BETA]  = acos(m11);
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_12], rmatrix[E_RMATRIX_10]);
            }
        }
        case rtype_euler_zyz: {
            new Float: m22 = rmatrix[E_RMATRIX_22];

            if(m22 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]); // -01, 11
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m22 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_12], rmatrix[E_RMATRIX_02]);
                euler[E_EULER_BETA]  = acos(m22);
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_21], -rmatrix[E_RMATRIX_20]);
            }
        }
        case rtype_euler_zxz: {
            new Float: m22 = rmatrix[E_RMATRIX_22];

            if(m22 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]); // -01, 11
                euler[E_EULER_BETA]  =
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m22 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_BETA]  = 180.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_02], -rmatrix[E_RMATRIX_12]);
                euler[E_EULER_BETA]  = acos(m22);
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_21]);
            }
        }
        case rtype_euler_xzy: {
            new Float: m01 = rmatrix[E_RMATRIX_01];

            if(m01 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_10]); // -12, 22
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m01 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_10]);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_BETA]  = -asin(m01); // order swapped to prevent overwrite
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_21], rmatrix[E_RMATRIX_11]);
            }
        }
        case rtype_euler_xyz: {
            new Float: m02 = rmatrix[E_RMATRIX_02];

            if(m02 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_21], rmatrix[E_RMATRIX_11]); // 10, -20
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m02 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_21], rmatrix[E_RMATRIX_11]);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_GAMMA] = -atan2(rmatrix[E_RMATRIX_01], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_BETA]  = asin(m02); // order swapped to prevent overwrite
                euler[E_EULER_ALPHA] = -atan2(rmatrix[E_RMATRIX_12], rmatrix[E_RMATRIX_22]);
            }
        }
        case rtype_euler_yxz: {
            new Float: m12 = rmatrix[E_RMATRIX_12];

            if(m12 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_01], rmatrix[E_RMATRIX_21]); // -20, 00
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m12 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_01], rmatrix[E_RMATRIX_21]);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_22]);
                euler[E_EULER_BETA]  = -asin(m12);
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_11]);
            }
        }
        case rtype_euler_yzx: {
            new Float: m10 = rmatrix[E_RMATRIX_10];

            if(m10 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_22]); // 21, -01
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m10 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_22]);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = -atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_BETA]  = asin(m10);
                euler[E_EULER_GAMMA] = -atan2(rmatrix[E_RMATRIX_12], rmatrix[E_RMATRIX_11]);
            }
        }
        case rtype_euler_zyx: {
            new Float: m20 = rmatrix[E_RMATRIX_20];

            if(m20 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_12], rmatrix[E_RMATRIX_02]); // -01, 11
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m20 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_12], rmatrix[E_RMATRIX_02]);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_BETA]  = -asin(m20);
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_21], rmatrix[E_RMATRIX_22]);
            }
        }
        case rtype_euler_zxy: {
            new Float: m21 = rmatrix[E_RMATRIX_21]; // singularitie at euler[E_EULER_ALPHA] = +- 90.0

            if(m21 > EULER_SINGULARITY_THRESHOLD) { // ~ 89.2 degrees
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]); // 02, -12
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m21 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else { // it does work without the singularities but it gets inaccurate
                euler[E_EULER_ALPHA] = -atan2(rmatrix[E_RMATRIX_01], rmatrix[E_RMATRIX_11]);
                euler[E_EULER_BETA]  = asin(m21);
                euler[E_EULER_GAMMA] = -atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_22]);
            }
        }
        case rtype_euler_re_xzy: {
            new Float: m01 = rmatrix[E_RMATRIX_01];

            if(m01 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_10]); // -12, 22
                euler[E_EULER_BETA]  = 0.0;
                euler[E_EULER_GAMMA] = 90.0;
            } else if(m01 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_10]);
                euler[E_EULER_BETA]  = 0.0;
                euler[E_EULER_GAMMA] = -90.0;
            } else {
                euler[E_EULER_BETA]  = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_GAMMA] = -asin(m01); // order swapped to prevent overwrite
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_21], rmatrix[E_RMATRIX_11]);
            }
        }
        case rtype_euler_re_yxz: {
            new Float: m12 = rmatrix[E_RMATRIX_12];

            if(m12 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = 90.0;
                euler[E_EULER_BETA]  = atan2(rmatrix[E_RMATRIX_01], rmatrix[E_RMATRIX_21]); // -20, 00
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m12 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = -90.0;
                euler[E_EULER_BETA]  = atan2(rmatrix[E_RMATRIX_01], rmatrix[E_RMATRIX_21]);
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_ALPHA] = -asin(m12);
                euler[E_EULER_BETA]  = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_22]);
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_11]);
            }
        }
        case rtype_euler_re_yzx: {
            new Float: m10 = rmatrix[E_RMATRIX_10];

            if(m10 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = 0.0;
                euler[E_EULER_BETA]  = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_22]); // 21, -01
                euler[E_EULER_GAMMA] = 90.0;
            } else if(m10 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = 0.0;
                euler[E_EULER_BETA]  = atan2(rmatrix[E_RMATRIX_02], rmatrix[E_RMATRIX_22]);
                euler[E_EULER_GAMMA] = -90.0;
            } else {
                euler[E_EULER_BETA]  = -atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_GAMMA] = asin(m10); // order swapped to prevent overwrite
                euler[E_EULER_ALPHA] = -atan2(rmatrix[E_RMATRIX_12], rmatrix[E_RMATRIX_11]);
            }
        }
        case rtype_euler_re_zyx: {
            new Float: m20 = rmatrix[E_RMATRIX_20];

            if(m20 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = 0.0;
                euler[E_EULER_BETA]  = 90.0;
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_12], rmatrix[E_RMATRIX_02]); // -01, 11
            } else if(m20 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = 0.0;
                euler[E_EULER_BETA]  = -90.0;
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_12], rmatrix[E_RMATRIX_02]);
            } else {
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_ALPHA] = atan2(rmatrix[E_RMATRIX_21], rmatrix[E_RMATRIX_22]);
                euler[E_EULER_BETA]  = -asin(m20); // order swapped to prevent overwrite
            }
        }
        case rtype_euler_re_zxy: {
            new Float: m21 = rmatrix[E_RMATRIX_21];

            if(m21 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_ALPHA] = 90.0; // order swapped to prevent overwrite
                euler[E_EULER_BETA]  = 0.0;
            } else if(m21 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]);
                euler[E_EULER_ALPHA] = -90.0; // order swapped to prevent overwrite
                euler[E_EULER_BETA]  = 0.0;
            } else {
                euler[E_EULER_GAMMA] = -atan2(rmatrix[E_RMATRIX_01], rmatrix[E_RMATRIX_11]);
                euler[E_EULER_ALPHA] = asin(m21); // order swapped to prevent overwrite
                euler[E_EULER_BETA]  = -atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_22]);
            }
        }
    }
    debug("ConvertMatrixToEuler(%f, %f, %f)", euler[E_EULER_ALPHA], euler[E_EULER_BETA], euler[E_EULER_GAMMA]);
}

#undef EULER_SINGULARITY_THRESHOLD_H
#undef EULER_SINGULARITY_THRESHOLD

stock GetEulerFromRotation(euler[E_EULER], rotationtype: type, & Float: alpha, & Float: beta, & Float: gamma) {
    /// <summary>Gets the euler angle from the rotation array</summary>
    /// <param name="euler">Euler angle array [<ref name="E_EULER"/>]</param>
    /// <param name="type">Dest rotation type, unused</param>
    /// <param name="alpha">First dest euler angle</param>
    /// <param name="beta">Second dest euler angle</param>
    /// <param name="gamma">Third dest euler angle</param>

    #pragma unused type

    alpha = euler[E_EULER_ALPHA];
    beta  = euler[E_EULER_BETA];
    gamma = euler[E_EULER_GAMMA];

    debug("GetEulerFromRotation(%f, %f, %f)", alpha, beta, gamma);
}