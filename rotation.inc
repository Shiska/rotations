#if defined _inc_rotation
	#endinput
#endif

#define _inc_rotation

/**
  * <summary>
  *   Allows conversion between different rotations.
  * </summary>
  *
  * <section>Contributer</section>
  *
  * <ul>
  *   <li>Nero_3D</li>
  * </ul>
  *
  * <section>History</section>
  *
  * <ul>
  *   <li>
  *     <subsection>1.0 (14.08.2016)</subsection>
  *     <ul>
  *       <li>Release</li>
  *     </ul>
  *   </li>
  *   <li>
  *     <subsection>1.1 (07.01.2017)</subsection>
  *     <ul>
  *       <li>euler_default renamed to euler_samp</li>
  *       <li>Translation function added for Rotation Matrix</li>
  *       <li>additional translate parameter 'oT' in MatrixRotate</li>
  *     </ul>
  *   </li>
  *   <li>
  *     <subsection>1.2 (11.08.2017)</subsection>
  *     <ul>
  *       <li>GetVehicleObjectPositionWorld and GetVehicleObjectPositionWorld added</li>
  *     </ul>
  *   </li>
  *   <li>
  *     <subsection>2.0 (xx.xx.2019)</subsection>
  *     <ul>
  *       <li>Functions rewritten with rotation array, incompatible with version 1.x</li>
  *     </ul>
  *   </li>
  * </ul>
  *
  * <section>Reference</section>
  *
  * <ul>
  *   <li><a target="_blank" href="https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix" /></li>
  *   <li><a target="_blank" href="https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation" /></li>
  *   <li><a target="_blank" href="https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation" /></li>
  *   <li><a target="_blank" href="http://www.euclideanspace.com/maths/geometry/rotations/index.htm" /></li>
  *   <li><a target="_blank" href="https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles" /></li>
  * </ul>
  *//** */

static enum E_AANGLE {
    Float: E_AANGLE_ANGLE,
    Float: E_AANGLE_X,
    Float: E_AANGLE_Y,
    Float: E_AANGLE_Z
}
/// <summary>Axis angle enum (angle, x, y, z)</summary>

static enum E_EULER {
    Float: E_EULER_ALPHA,
    Float: E_EULER_BETA,
    Float: E_EULER_GAMMA
}
/// <summary>Euler angle enum (alpha, beta, gamma)</summary>

static enum E_QUAT {
    Float: E_QUAT_W,
    Float: E_QUAT_X,
    Float: E_QUAT_Y,
    Float: E_QUAT_Z
}
/// <summary>Quaternion enum (w, x, y, z)</summary>

static enum E_RMATRIX {
    Float: E_RMATRIX_00, Float: E_RMATRIX_01, Float: E_RMATRIX_02,
    Float: E_RMATRIX_10, Float: E_RMATRIX_11, Float: E_RMATRIX_12,
    Float: E_RMATRIX_20, Float: E_RMATRIX_21, Float: E_RMATRIX_22
}
/// <summary>Rotation matrix enum (3 rows, 4 coloums)</summary>

enum rotationtype {
    rtype_axis_angle,
    rtype_euler_xzx, // Proper / Classic Euler angles
    rtype_euler_xyx, // Euler angle have three angles a(lpha), b(eta) and g(amma)
    rtype_euler_yxy, // there are two ways to calculate the final position
    rtype_euler_yzy, // Right to left - global frame-of-reference
    rtype_euler_zyz, // Left to right - Local frame-of-reference
    rtype_euler_zxz, // (A * B * C)^-1 = (C^-1 * B^-1 * A^-1)
    rtype_euler_xzy, // Tait-Bryan angles
    rtype_euler_xyz,
    rtype_euler_yxz,
    rtype_euler_yzx,
    rtype_euler_zyx,
    rtype_euler_zxy, // yaw pitch roll (for vehicle / object facing y direction)
    rtype_euler_samp, // object rX rY rZ, pitch roll yaw, same as rtype_euler_zxy with swapped order
    rtype_quaternion,
    rtype_rotation_matrix
}
/// <summary>All supported rotation types</summary>
/// <export />

static stock __rotationtype[rotationtype]; // necessary for report file, it doesn't detect "tag only" usage

// get largest rotation type, obviously it will be E_RMATRIX
const __largestType = _: E_AANGLE;

#if _: E_EULER > __largestType
    #undef __largestType

    const __largestType = _: E_EULER;
#endif

#if _: E_QUAT > __largestType
    #error test
    #undef __largestType

    const __largestType = _: E_QUAT;
#endif

#if _: E_RMATRIX > __largestType
    #undef __largestType

    const __largestType = _: E_RMATRIX;
#endif

enum E_ROTATION { // TODO: maybe use pointers and operators, afaik operators can't handle arrays
    rotation: E_ROTATION_BASE, // needs to be the first entry
    rotationtype: E_ROTATION_TYPE,
    E_ROTATION_DATA[__largestType]
}
/// <summary>Rotation array</summary>
/// <export />

#undef __largestType

const E_ROTATION_DATA_OFFSET = _: E_ROTATION_DATA << 2;

#if 0
    #define debug printf
#else
    #define debug(%0\10
#endif

static stock SetRotationFromAxisAngle(aangle[E_AANGLE], rotationtype: type, & Float: angle, & Float: x, & Float: y, & Float: z) {
    /// <summary>Set the rotation array from axis angle</summary>
    /// <param name="aangle">Axis angle array [<ref name="E_AANGLE"/>]</param>
    /// <param name="type">Source rotation type, unused</param>
    /// <param name="angle">Source axis angle rotation</param>
    /// <param name="x">Source axis angle unit vector x</param>
    /// <param name="y">Source axis angle unit vector y</param>
    /// <param name="z">Source axis angle unit vector z</param>
    /// <remarks>Used internally</remarks>

    debug("SetRotationFromAxisAngle(%f, %f, %f, %f)", angle, x, y, z);

    #pragma unused type

    aangle[E_AANGLE_ANGLE] = angle;
    aangle[E_AANGLE_X] = x;
    aangle[E_AANGLE_Y] = y;
    aangle[E_AANGLE_Z] = z;
}

static stock SetRotationFromEuler(euler[E_EULER], rotationtype: type, & Float: alpha, & Float: beta, & Float: gamma) {
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

static stock SetRotationFromQuat(quat[E_QUAT], rotationtype: type, & Float: w, & Float: x, & Float: y, & Float: z) {
    /// <summary>Set the rotation array from quaternion</summary>
    /// <param name="quat">Quaternion array [<ref name="E_QUAT"/>]</param>
    /// <param name="type">Source rotation type, unused</param>
    /// <param name="w">Source quaternion scalar part</param>
    /// <param name="x">Source quaternion imaginary part x</param>
    /// <param name="y">Source quaternion imaginary part y</param>
    /// <param name="z">Source quaternion imaginary part z</param>
    /// <remarks>Used internally</remarks>

    debug("SetRotationFromQuat(%f, %f, %f, %f)", w, x, y, z);

    #pragma unused type

    quat[E_QUAT_W] = w;
    quat[E_QUAT_X] = x;
    quat[E_QUAT_Y] = y;
    quat[E_QUAT_Z] = z;
}

static stock SetRotationFromMatrix(rmatrix[E_RMATRIX], rotationtype: type, Float: matrix[][]) {
    /// <summary>Set the rotation array from rotation matrix</summary>
    /// <param name="rmatrix">Rotation matrix array [<ref name="E_RMATRIX"/>]</param>
    /// <param name="type">Source rotation type, unused</param>
    /// <param name="matrix">Source matrix</param>
    /// <remarks>Used internally</remarks>
 
    debug("SetRotationFromMatrix(...)");

    #pragma unused type

    rmatrix[E_RMATRIX_00] = matrix[0][0];
    rmatrix[E_RMATRIX_01] = matrix[0][1];
    rmatrix[E_RMATRIX_02] = matrix[0][2];

    rmatrix[E_RMATRIX_10] = matrix[1][0];
    rmatrix[E_RMATRIX_11] = matrix[1][1];
    rmatrix[E_RMATRIX_12] = matrix[1][2];

    rmatrix[E_RMATRIX_20] = matrix[2][0];
    rmatrix[E_RMATRIX_21] = matrix[2][1];
    rmatrix[E_RMATRIX_22] = matrix[2][2];
}

// native SetRotation(rotation[E_ROTATION], rotationtype: type, Float: ...);
stock SetRotation(rotation[E_ROTATION], rotationtype: type, Float: ...) {
    /// <summary>Set the rotation array from the given rotation</summary>
    /// <export />
    /// <param name="rotation">Rotation array [<ref name="E_ROTATION"/>]</param>
    /// <param name="type">Source rotation type</param>
    /// <param name="...">Source rotation</param>

    debug("SetRotation(%d)", _: type);
    // store address in E_ROTATION_BASE
    #emit load.s.pri rotation
    #emit sref.s.pri rotation
    // store rotation type
    rotation[E_ROTATION_TYPE] = type;
    // move rotation to data section
    #emit load.s.pri rotation
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri rotation

    switch(type) {
        case rtype_axis_angle:{
            #emit const.pri SetRotationFromAxisAngle
        }
        case rtype_quaternion: {
            #emit const.pri SetRotationFromQuat
        }
        case rtype_rotation_matrix: {
            #emit const.pri SetRotationFromMatrix
        }
        default: { // Euler
            #emit const.pri SetRotationFromEuler
        }
    } {}
    #emit add.c 4
    #emit sctrl 6
}

static stock ConvertEulerToAxisAngle(euler[E_EULER], rotationtype: type[2], aangle[E_AANGLE]) {
    /// <summary>Convert the given euler angle into axis angle</summary>
    /// <param name="euler">Source euler angle [<ref name="E_EULER"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="aangle">Dest axis angle [<ref name="E_AANGLE"/>]</param>
    /// <remarks>Used internally</remarks>
    /// <remarks>Not optimized, uses <ref name="ConvertEulerToQuat"/> and <ref name="ConvertQuatToAxisAngle"/> for the conversion</remarks>

    new quat[E_QUAT];

    type[1] = rtype_quaternion;

    ConvertEulerToQuat(euler, type, quat);
    ConvertQuatToAxisAngle(quat, rotationtype: {rtype_quaternion, rtype_axis_angle}, aangle);

    debug("ConvertEulerToAxisAngle(%f, %f, %f, %f)", aangle[E_AANGLE_ANGLE], aangle[E_AANGLE_X], aangle[E_AANGLE_Y], aangle[E_AANGLE_Z]);
}

static stock ConvertQuatToAxisAngle(quat[E_QUAT], rotationtype: type[2], aangle[E_AANGLE]) {
    /// <summary>Convert the given quaternion into axis angle</summary>
    /// <param name="quat">Source quaternion [<ref name="E_QUAT"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="aangle">Dest axis angle [<ref name="E_AANGLE"/>]</param>
    /// <remarks>Used internally</remarks>

    #pragma unused type

    new Float: size = VectorSize(quat[E_QUAT_X], quat[E_QUAT_Y], quat[E_QUAT_Z]); // w = floatsqroot(1.0 - w * w); // w = floatsin(angle, degrees); // |sin(angle)|

    if(size == 0.0) { // no rotation
        aangle[E_AANGLE_ANGLE] =
        aangle[E_AANGLE_X] =
        aangle[E_AANGLE_Y] = 0.0;
        aangle[E_AANGLE_Z] = 1.0;
    } else {
        aangle[E_AANGLE_ANGLE] = 2.0 * acos(-quat[E_QUAT_W]);
        aangle[E_AANGLE_X] = quat[E_QUAT_X] / size;
        aangle[E_AANGLE_Y] = quat[E_QUAT_Y] / size;
        aangle[E_AANGLE_Z] = quat[E_QUAT_Z] / size;
    }
    debug("ConvertQuatToAxisAngle(%f, %f, %f, %f)", aangle[E_AANGLE_ANGLE], aangle[E_AANGLE_X], aangle[E_AANGLE_Y], aangle[E_AANGLE_Z]);
}

static stock ConvertMatrixToAxisAngle(rmatrix[E_RMATRIX], rotationtype: type[2], aangle[E_AANGLE]) {
    /// <summary>Convert the given rotation matrix into axis angle</summary>
    /// <param name="rmatrix">Source rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="aangle">Dest axis angle [<ref name="E_AANGLE"/>]</param>
    /// <remarks>Used internally</remarks>

    #pragma unused type

    new Float: aX = rmatrix[E_RMATRIX_21] - rmatrix[E_RMATRIX_12]; // does have untriggered singularities
    new Float: aY = rmatrix[E_RMATRIX_02] - rmatrix[E_RMATRIX_20];
    new Float: aZ = rmatrix[E_RMATRIX_10] - rmatrix[E_RMATRIX_01];
    new Float: size = VectorSize(aX, aY, aZ);

    aangle[E_AANGLE_ANGLE] = acos((rmatrix[E_RMATRIX_00] + rmatrix[E_RMATRIX_11] + rmatrix[E_RMATRIX_22] - 1.0) / 2.0);
    aangle[E_AANGLE_X] = aX / size;
    aangle[E_AANGLE_Y] = aY / size;
    aangle[E_AANGLE_Z] = aZ / size;

    debug("ConvertMatrixToAxisAngle(%f, %f, %f, %f)", aangle[E_AANGLE_ANGLE], aangle[E_AANGLE_X], aangle[E_AANGLE_Y], aangle[E_AANGLE_Z]);
}

#define EULER_SINGULARITY_THRESHOLD 0.9999
#define EULER_SINGULARITY_THRESHOLD_H 0.49995 // EULER_SINGULARITY_THRESHOLD / 2.0 - preprocessor can't handle floats :/

static stock ConvertAxisAngleToEuler(aangle[E_AANGLE], rotationtype: type[2], euler[E_EULER]) {
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

static stock ConvertEulerToEuler(src[E_EULER], rotationtype: type[2], dest[E_EULER]) {
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

static stock ConvertQuatToEuler(quat[E_QUAT], rotationtype: type[2], euler[E_EULER]) {
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
                euler[E_EULER_ALPHA]  = atan2(w * y - x * z, x * y + w * z);
                euler[E_EULER_BETA] = 90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else if(m10 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA]  = atan2(x * z - w * y, x * y + w * z);
                euler[E_EULER_BETA] = -90.0;
                euler[E_EULER_GAMMA] = 0.0;
            } else {
                euler[E_EULER_BETA] = asin(2.0 * m10);

                m10 = 0.5 - z * z;

                euler[E_EULER_ALPHA]  = -atan2(x * z + w * y, m10 - y * y);
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
                euler[E_EULER_BETA] = 90.0;
                euler[E_EULER_GAMMA]  = 0.0;
            } else if(m21 < -EULER_SINGULARITY_THRESHOLD_H) {
                euler[E_EULER_ALPHA] = atan2(w * y - x * z, y * z + w * x);
                euler[E_EULER_BETA] = -90.0;
                euler[E_EULER_GAMMA]  = 0.0;
            } else {
                euler[E_EULER_BETA] = asin(2.0 * m21);

                m21 = 0.5 - x * x;

                euler[E_EULER_ALPHA] = -atan2(x * y + w * z, m21 - z * z);
                euler[E_EULER_GAMMA]  = -atan2(x * z + w * y, m21 - y * y);
            }
        }
        case rtype_euler_samp: {
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

static stock ConvertMatrixToEuler(rmatrix[E_RMATRIX], rotationtype: type[2], euler[E_EULER]) {
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
        case rtype_euler_samp: {
            new Float: m21 = rmatrix[E_RMATRIX_21];

            if(m21 > EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = 90.0;
                euler[E_EULER_BETA]  = 0.0;
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]);
            } else if(m21 < -EULER_SINGULARITY_THRESHOLD) {
                euler[E_EULER_ALPHA] = -90.0;
                euler[E_EULER_BETA]  = 0.0;
                euler[E_EULER_GAMMA] = atan2(rmatrix[E_RMATRIX_10], rmatrix[E_RMATRIX_00]);
            } else {
                euler[E_EULER_GAMMA] = -atan2(rmatrix[E_RMATRIX_01], rmatrix[E_RMATRIX_11]);
                euler[E_EULER_BETA]  = -atan2(rmatrix[E_RMATRIX_20], rmatrix[E_RMATRIX_22]);
                euler[E_EULER_ALPHA] = asin(m21); // order swapped to prevent overwrite
            }
        }
    }
    debug("ConvertMatrixToEuler(%f, %f, %f)", euler[E_EULER_ALPHA], euler[E_EULER_BETA], euler[E_EULER_GAMMA]);
}

#undef EULER_SINGULARITY_THRESHOLD_H
#undef EULER_SINGULARITY_THRESHOLD

static stock ConvertAxisAngleToQuat(aangle[E_AANGLE], rotationtype: type[2], quat[E_QUAT]) {
    /// <summary>Convert the given axis angle into quaternion</summary>
    /// <param name="aangle">Source axis angle [<ref name="E_AANGLE"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="quat">Dest quaternion [<ref name="E_QUAT"/>]</param>
    /// <remarks>Used internally</remarks>

    #pragma unused type

    new Float: size = VectorSize(aangle[E_AANGLE_X], aangle[E_AANGLE_Y], aangle[E_AANGLE_Z]);

    if(size == 0.0) { // no rotation
        quat[E_QUAT_W] = 1.0;
        quat[E_QUAT_X] =
        quat[E_QUAT_Y] =
        quat[E_QUAT_Z] = 0.0;
    } else {
        new Float: angle = aangle[E_AANGLE_ANGLE] / -2.0;

        size = floatsin(angle, degrees) / size;

        quat[E_QUAT_W] = floatcos(angle, degrees);
        quat[E_QUAT_X] = aangle[E_AANGLE_X] * size;
        quat[E_QUAT_Y] = aangle[E_AANGLE_Y] * size;
        quat[E_QUAT_Z] = aangle[E_AANGLE_Z] * size;
    }
    debug("ConvertAxisAngleToQuat(%f, %f, %f, %f)", quat[E_QUAT_W], quat[E_QUAT_X], quat[E_QUAT_Y], quat[E_QUAT_Z]);
}

static stock ConvertEulerToQuat(euler[E_EULER], rotationtype: type[2], quat[E_QUAT]) {
    /// <summary>Convert the given euler angle into quaternion</summary>
    /// <param name="euler">Source euler angle [<ref name="E_EULER"/>]</param>
    /// <param name="type">Rotation type</param>
    /// <param name="quat">Dest quaternion [<ref name="E_QUAT"/>]</param>
    /// <remarks>Used internally</remarks>

    // could be done with matrix multiplication but would be slower
    new Float: a = euler[E_EULER_ALPHA] * 0.5;
    new Float: b = euler[E_EULER_BETA]  * 0.5;
    new Float: g = euler[E_EULER_GAMMA] * 0.5;

    new	Float: cosA = floatcos(a, degrees);
    new Float: cosB = floatcos(b, degrees);
    new Float: cosG = floatcos(g, degrees);
    new Float: sinA = floatsin(a, degrees);
    new Float: sinB = floatsin(b, degrees);
    new Float: sinG = floatsin(g, degrees);

    switch(type[0]) {
        case rtype_euler_xzx: {
            quat[E_QUAT_W] = sinA * cosB * sinG - cosA * cosB * cosG;
            quat[E_QUAT_X] = cosA * cosB * sinG + sinA * cosB * cosG;
            quat[E_QUAT_Y] = cosA * sinB * sinG - sinA * sinB * cosG;
            quat[E_QUAT_Z] = cosA * sinB * cosG + sinA * sinB * sinG;		
        }
        case rtype_euler_xyx: {
            quat[E_QUAT_W] = sinA * cosB * sinG - cosA * cosB * cosG;
            quat[E_QUAT_X] = cosA * cosB * sinG + sinA * cosB * cosG;
            quat[E_QUAT_Y] = cosA * sinB * cosG + sinA * sinB * sinG;
            quat[E_QUAT_Z] = sinA * sinB * cosG - cosA * sinB * sinG;
        }
        case rtype_euler_yxy: {
            quat[E_QUAT_W] = sinA * cosB * sinG - cosA * cosB * cosG;
            quat[E_QUAT_X] = cosA * sinB * cosG + sinA * sinB * sinG;
            quat[E_QUAT_Y] = cosA * cosB * sinG + sinA * cosB * cosG;
            quat[E_QUAT_Z] = cosA * sinB * sinG - sinA * sinB * cosG;
        }
        case rtype_euler_yzy: {
            quat[E_QUAT_W] = sinA * cosB * sinG - cosA * cosB * cosG;
            quat[E_QUAT_X] = sinA * sinB * cosG - cosA * sinB * sinG;
            quat[E_QUAT_Y] = cosA * cosB * sinG + sinA * cosB * cosG;
            quat[E_QUAT_Z] = cosA * sinB * cosG + sinA * sinB * sinG;
        }
        case rtype_euler_zyz: {
            quat[E_QUAT_W] = sinA * cosB * sinG - cosA * cosB * cosG;
            quat[E_QUAT_X] = cosA * sinB * sinG - sinA * sinB * cosG;
            quat[E_QUAT_Y] = cosA * sinB * cosG + sinA * sinB * sinG;
            quat[E_QUAT_Z] = cosA * cosB * sinG + sinA * cosB * cosG;
        }
        case rtype_euler_zxz: {
            quat[E_QUAT_W] = sinA * cosB * sinG - cosA * cosB * cosG;
            quat[E_QUAT_X] = cosA * sinB * cosG + sinA * sinB * sinG;
            quat[E_QUAT_Y] = sinA * sinB * cosG - cosA * sinB * sinG;
            quat[E_QUAT_Z] = cosA * cosB * sinG + sinA * cosB * cosG;
        }
        case rtype_euler_xzy: {
            quat[E_QUAT_W] = cosA * cosG * cosB + sinA * sinG * sinB;
            quat[E_QUAT_X] = cosA * sinG * sinB - sinA * cosG * cosB;
            quat[E_QUAT_Y] = sinA * cosG * sinB - cosA * sinG * cosB;
            quat[E_QUAT_Z] = -(cosA * cosG * sinB + sinA * sinG * cosB);
        }
        case rtype_euler_xyz: {
            quat[E_QUAT_W] = sinA * sinB * sinG - cosA * cosB * cosG;
            quat[E_QUAT_X] = sinA * cosB * cosG + cosA * sinB * sinG;
            quat[E_QUAT_Y] = cosA * sinB * cosG - sinA * cosB * sinG;
            quat[E_QUAT_Z] = cosA * cosB * sinG + sinA * sinB * cosG;
        }
        case rtype_euler_yxz: {
            quat[E_QUAT_W] = cosB * cosA * cosG + sinB * sinA * sinG;
            quat[E_QUAT_X] = -(sinB * cosA * cosG + cosB * sinA * sinG);
            quat[E_QUAT_Y] = sinB * cosA * sinG - cosB * sinA * cosG;
            quat[E_QUAT_Z] = sinB * sinA * cosG - cosB * cosA * sinG;
        }
        case rtype_euler_yzx: {
            quat[E_QUAT_W] = sinG * sinA * sinB - cosG * cosA * cosB;
            quat[E_QUAT_X] = sinG * cosA * cosB + cosG * sinA * sinB;
            quat[E_QUAT_Y] = cosG * sinA * cosB + sinG * cosA * sinB;
            quat[E_QUAT_Z] = cosG * cosA * sinB - sinG * sinA * cosB;
        }
        case rtype_euler_zyx: {
            quat[E_QUAT_W] = cosG * cosB * cosA + sinG * sinB * sinA;
            quat[E_QUAT_X] = cosG * sinB * sinA - sinG * cosB * cosA;
            quat[E_QUAT_Y] = -(cosG * sinB * cosA + sinG * cosB * sinA);
            quat[E_QUAT_Z] = sinG * sinB * cosA - cosG * cosB * sinA;
        }
        case rtype_euler_zxy: {
            quat[E_QUAT_W] = sinB * sinG * sinA - cosB * cosG * cosA;
            quat[E_QUAT_X] = sinB * cosG * cosA - cosB * sinG * sinA;
            quat[E_QUAT_Y] = cosB * sinG * cosA + sinB * cosG * sinA;
            quat[E_QUAT_Z] = cosB * cosG * sinA + sinB * sinG * cosA;
        }
        case rtype_euler_samp: {
            quat[E_QUAT_W] = sinA * sinB * sinG - cosA * cosB * cosG;
            quat[E_QUAT_X] = sinA * cosB * cosG - cosA * sinB * sinG;
            quat[E_QUAT_Y] = cosA * sinB * cosG + sinA * cosB * sinG;
            quat[E_QUAT_Z] = cosA * cosB * sinG + sinA * sinB * cosG;
        }
    }
    debug("ConvertEulerToQuat(%f, %f, %f, %f)", quat[E_QUAT_W], quat[E_QUAT_X], quat[E_QUAT_Y], quat[E_QUAT_Z]);
}

static stock ConvertMatrixToQuat(rmatrix[E_RMATRIX], rotationtype: type[2], quat[E_QUAT]) {
    /// <summary>Convert the given rotation matrix into quaternion</summary>
    /// <param name="rmatrix">Source rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="quat">Dest quaternion [<ref name="E_QUAT"/>]</param>
    /// <remarks>Used internally</remarks>

    #pragma unused type

    new Float: m00 = rmatrix[E_RMATRIX_00];
    new Float: m11 = rmatrix[E_RMATRIX_11];
    new Float: m22 = rmatrix[E_RMATRIX_22];
    new Float: trace = m00 + m11 + m22;
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
    if(trace > 0.0) { // calculate quat with base w
        trace = floatsqroot(1.0 + trace) * 2.0;
        // order swapped to prevent overwrite
        quat[E_QUAT_Z] = (rmatrix[E_RMATRIX_01] - rmatrix[E_RMATRIX_10]) / trace;
        quat[E_QUAT_Y] = (rmatrix[E_RMATRIX_20] - rmatrix[E_RMATRIX_02]) / trace;
        quat[E_QUAT_X] = (rmatrix[E_RMATRIX_12] - rmatrix[E_RMATRIX_21]) / trace;
        quat[E_QUAT_W] = 0.25 * trace;
    } else if(m00 > m11 && m00 > m22) { // m00 biggest, calculate quat with base x
        trace = floatsqroot(1.0 + m00 - m11 - m22) * 2.0;

        m22 = rmatrix[E_RMATRIX_02];
        // order swapped to prevent overwrite
        quat[E_QUAT_W] = (rmatrix[E_RMATRIX_12] - rmatrix[E_RMATRIX_21]) / trace;
        quat[E_QUAT_Y] = (rmatrix[E_RMATRIX_01] + rmatrix[E_RMATRIX_10]) / trace;
        quat[E_QUAT_Z] = (m22 + rmatrix[E_RMATRIX_20]) / trace;
        quat[E_QUAT_X] = 0.25 * trace;
    } else if(m11 > m22) { // m11 biggest, calculate quat with base y
        trace = floatsqroot(1.0 + m11 - m00 - m22) * 2.0;

        quat[E_QUAT_W] = (rmatrix[E_RMATRIX_20] - rmatrix[E_RMATRIX_02]) / trace;
        quat[E_QUAT_X] = (rmatrix[E_RMATRIX_01] + rmatrix[E_RMATRIX_10]) / trace;
        quat[E_QUAT_Y] = 0.25 * trace;
        quat[E_QUAT_Z] = (rmatrix[E_RMATRIX_12] + rmatrix[E_RMATRIX_21]) / trace;
    } else { // m22 biggest, calculate quat with base z
        trace = floatsqroot(1.0 + m22 - m00 - m11) * 2.0;

        quat[E_QUAT_W] = (rmatrix[E_RMATRIX_01] - rmatrix[E_RMATRIX_10]) / trace;
        quat[E_QUAT_X] = (rmatrix[E_RMATRIX_02] + rmatrix[E_RMATRIX_20]) / trace;
        quat[E_QUAT_Y] = (rmatrix[E_RMATRIX_12] + rmatrix[E_RMATRIX_21]) / trace;
        quat[E_QUAT_Z] = 0.25 * trace;
    }
    debug("ConvertMatrixToQuat(%f, %f, %f, %f)", quat[E_QUAT_W], quat[E_QUAT_X], quat[E_QUAT_Y], quat[E_QUAT_Z]);
}

static stock ConvertAxisAngleToMatrix(aangle[E_AANGLE], rotationtype: type[2], rmatrix[E_RMATRIX]) {
    /// <summary>Convert the given axis angle into a rotation matrix</summary>
    /// <param name="aangle">Source axis angle [<ref name="E_AANGLE"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="rmatrix">Dest rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <remarks>Used internally</remarks>

    debug("ConvertAxisAngleToMatrix(...)");

    #pragma unused type
    
    new Float: angle = aangle[E_AANGLE_ANGLE];
    new Float: x = aangle[E_AANGLE_X];
    new Float: y = aangle[E_AANGLE_Y];
    new Float: z = aangle[E_AANGLE_Z];

    new Float: cos = floatcos(angle, degrees);
    new Float: sin = floatsin(angle, degrees);
    new Float: t = 1.0 - cos;

    rmatrix[E_RMATRIX_00] = t * x * x + cos;
    rmatrix[E_RMATRIX_11] = t * y * y + cos;
    rmatrix[E_RMATRIX_22] = t * z * z + cos;

    cos = t * x * y;

    rmatrix[E_RMATRIX_01] = cos - sin * z;
    rmatrix[E_RMATRIX_10] = cos + sin * z;

    cos = t * x * z;

    rmatrix[E_RMATRIX_02] = cos + sin * y;
    rmatrix[E_RMATRIX_20] = cos - sin * y;

    cos = t * y * z;

    rmatrix[E_RMATRIX_12] = cos - sin * x;
    rmatrix[E_RMATRIX_21] = cos + sin * x;
}

static stock ConvertEulerToMatrix(euler[E_EULER], rotationtype: type[2], rmatrix[E_RMATRIX]) {
    /// <summary>Convert the given euler angle into a rotation matrix</summary>
    /// <param name="euler">Source euler angle [<ref name="E_EULER"/>]</param>
    /// <param name="type">Rotation type</param>
    /// <param name="rmatrix">Dest rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <remarks>Used internally</remarks>

    debug("ConvertEulerToMatrix(...)");

    new Float: a = euler[E_EULER_ALPHA];
    new Float: b = euler[E_EULER_BETA];
    new Float: g = euler[E_EULER_GAMMA];
    // could be done with matrix multiplication but would be slower
    new Float: cosA = floatcos(a, degrees);
    new Float: cosB = floatcos(b, degrees);
    new Float: cosG = floatcos(g, degrees);
    new Float: sinA = floatsin(a, degrees);
    new Float: sinB = floatsin(b, degrees);
    new Float: sinG = floatsin(g, degrees);

    switch(type[0]) {
        case rtype_euler_xzx: {
            rmatrix[E_RMATRIX_00] = cosB;
            rmatrix[E_RMATRIX_01] = -cosG * sinB;
            rmatrix[E_RMATRIX_02] = sinB * sinG;
            rmatrix[E_RMATRIX_10] = cosA * sinB;
            rmatrix[E_RMATRIX_11] = cosA * cosB * cosG - sinA * sinG;
            rmatrix[E_RMATRIX_12] = -cosG * sinA - cosA * cosB * sinG;
            rmatrix[E_RMATRIX_20] = sinA * sinB;
            rmatrix[E_RMATRIX_21] = cosA * sinG + cosB * cosG * sinA;
            rmatrix[E_RMATRIX_22] = cosA * cosG - cosB * sinA * sinG;
        }
        case rtype_euler_xyx: {
            rmatrix[E_RMATRIX_00] = cosB;
            rmatrix[E_RMATRIX_01] = sinB * sinG;
            rmatrix[E_RMATRIX_02] = cosG * sinB;
            rmatrix[E_RMATRIX_10] = sinA * sinB;
            rmatrix[E_RMATRIX_11] = cosA * cosG - cosB * sinA * sinG;
            rmatrix[E_RMATRIX_12] = -cosA * sinG - cosB * cosG * sinA;
            rmatrix[E_RMATRIX_20] = -cosA * sinB;
            rmatrix[E_RMATRIX_21] = cosG * sinA + cosA * cosB * sinG;
            rmatrix[E_RMATRIX_22] = cosA * cosB * cosG - sinA * sinG;
        }
        case rtype_euler_yxy: {
            rmatrix[E_RMATRIX_00] = cosA * cosG - cosB * sinA * sinG;
            rmatrix[E_RMATRIX_01] = sinA * sinB;
            rmatrix[E_RMATRIX_02] = cosA * sinG + cosB * cosG * sinA;
            rmatrix[E_RMATRIX_10] = sinB * sinG;
            rmatrix[E_RMATRIX_11] = cosB;
            rmatrix[E_RMATRIX_12] = -cosG * sinB;
            rmatrix[E_RMATRIX_20] = -cosG * sinA - cosA * cosB * sinG;
            rmatrix[E_RMATRIX_21] = cosA * sinB;
            rmatrix[E_RMATRIX_22] = cosA * cosB * cosG - sinA * sinG;
        }
        case rtype_euler_yzy: {
            rmatrix[E_RMATRIX_00] = cosA * cosB * cosG - sinA * sinG;
            rmatrix[E_RMATRIX_01] = -cosA * sinB;
            rmatrix[E_RMATRIX_02] = cosG * sinA + cosA * cosB * sinG;
            rmatrix[E_RMATRIX_10] = cosG * sinB;
            rmatrix[E_RMATRIX_11] = cosB;
            rmatrix[E_RMATRIX_12] = sinB * sinG;
            rmatrix[E_RMATRIX_20] = -cosA * sinG - cosB * cosG * sinA;
            rmatrix[E_RMATRIX_21] = sinA * sinB;
            rmatrix[E_RMATRIX_22] = cosA * cosG - cosB * sinA * sinG;
        }
        case rtype_euler_zyz: {
            rmatrix[E_RMATRIX_00] = cosA * cosB * cosG - sinA * sinG;
            rmatrix[E_RMATRIX_01] = -cosG * sinA - cosA * cosB * sinG;
            rmatrix[E_RMATRIX_02] = cosA * sinB;
            rmatrix[E_RMATRIX_10] = cosA * sinG + cosB * cosG * sinA;
            rmatrix[E_RMATRIX_11] = cosA * cosG - cosB * sinA * sinG;
            rmatrix[E_RMATRIX_12] = sinA * sinB;
            rmatrix[E_RMATRIX_20] = -cosG * sinB;
            rmatrix[E_RMATRIX_21] = sinB * sinG;
            rmatrix[E_RMATRIX_22] = cosB;
        }
        case rtype_euler_zxz: {
            rmatrix[E_RMATRIX_00] = cosA * cosG - cosB * sinA * sinG;
            rmatrix[E_RMATRIX_01] = -cosA * sinG - cosB * cosG * sinA;
            rmatrix[E_RMATRIX_02] = sinA * sinB;
            rmatrix[E_RMATRIX_10] = cosG * sinA + cosA * cosB * sinG;
            rmatrix[E_RMATRIX_11] = cosA * cosB * cosG - sinA * sinG;
            rmatrix[E_RMATRIX_12] = -cosA * sinB;
            rmatrix[E_RMATRIX_20] = sinB * sinG;
            rmatrix[E_RMATRIX_21] = cosG * sinB;
            rmatrix[E_RMATRIX_22] = cosB;
        }
        case rtype_euler_xzy: {
            rmatrix[E_RMATRIX_00] = cosB * cosG;
            rmatrix[E_RMATRIX_01] = -sinB;
            rmatrix[E_RMATRIX_02] = cosB * sinG;
            rmatrix[E_RMATRIX_10] = sinA * sinG + cosA * cosG * sinB;
            rmatrix[E_RMATRIX_11] = cosA * cosB;
            rmatrix[E_RMATRIX_12] = cosA * sinB * sinG - cosG * sinA;
            rmatrix[E_RMATRIX_20] = cosG * sinA * sinB - cosA * sinG;
            rmatrix[E_RMATRIX_21] = cosB * sinA;
            rmatrix[E_RMATRIX_22] = cosA * cosG + sinA * sinB * sinG;
        }
        case rtype_euler_xyz: {
            rmatrix[E_RMATRIX_00] = cosB * cosG;
            rmatrix[E_RMATRIX_01] = -cosB * sinG;
            rmatrix[E_RMATRIX_02] = sinB;
            rmatrix[E_RMATRIX_10] = cosA * sinG + cosG * sinA * sinB;
            rmatrix[E_RMATRIX_11] = cosA * cosG - sinA * sinB * sinG;
            rmatrix[E_RMATRIX_12] = -cosB * sinA;
            rmatrix[E_RMATRIX_20] = sinA * sinG - cosA * cosG * sinB;
            rmatrix[E_RMATRIX_21] = cosG * sinA + cosA * sinB * sinG;
            rmatrix[E_RMATRIX_22] = cosA * cosB;
        }
        case rtype_euler_yxz: {
            rmatrix[E_RMATRIX_00] = cosA * cosG + sinA * sinB * sinG;
            rmatrix[E_RMATRIX_01] = cosG * sinA * sinB - cosA * sinG;
            rmatrix[E_RMATRIX_02] = cosB * sinA;
            rmatrix[E_RMATRIX_10] = cosB * sinG;
            rmatrix[E_RMATRIX_11] = cosB * cosG;
            rmatrix[E_RMATRIX_12] = -sinB;
            rmatrix[E_RMATRIX_20] = cosA * sinB * sinG - cosG * sinA;
            rmatrix[E_RMATRIX_21] = cosA * cosG * sinB + sinA * sinG;
            rmatrix[E_RMATRIX_22] = cosA * cosB;
        }
        case rtype_euler_yzx: {
            rmatrix[E_RMATRIX_00] = cosA * cosB;
            rmatrix[E_RMATRIX_01] = sinA * sinG - cosA * cosG * sinB;
            rmatrix[E_RMATRIX_02] = cosG * sinA + cosA * sinB * sinG;
            rmatrix[E_RMATRIX_10] = sinB;
            rmatrix[E_RMATRIX_11] = cosB * cosG;
            rmatrix[E_RMATRIX_12] = -cosB * sinG;
            rmatrix[E_RMATRIX_20] = -cosB * sinA;
            rmatrix[E_RMATRIX_21] = cosA * sinG + cosG * sinA * sinB;
            rmatrix[E_RMATRIX_22] = cosA * cosG - sinA * sinB * sinG;
        }
        case rtype_euler_zyx: {
            rmatrix[E_RMATRIX_00] = cosA * cosB;
            rmatrix[E_RMATRIX_01] = cosA * sinB * sinG - cosG * sinA;
            rmatrix[E_RMATRIX_02] = sinA * sinG + cosA * cosG * sinB;
            rmatrix[E_RMATRIX_10] = cosB * sinA;
            rmatrix[E_RMATRIX_11] = cosA * cosG + sinA * sinB * sinG;
            rmatrix[E_RMATRIX_12] = cosG * sinA * sinB - cosA * sinG;
            rmatrix[E_RMATRIX_20] = -sinB;
            rmatrix[E_RMATRIX_21] = cosB * sinG;
            rmatrix[E_RMATRIX_22] = cosB * cosG;
        }
        case rtype_euler_zxy: {
            rmatrix[E_RMATRIX_00] = cosA * cosG - sinA * sinB * sinG;
            rmatrix[E_RMATRIX_01] = -sinA * cosB;
            rmatrix[E_RMATRIX_02] = cosA * sinG + sinA * sinB * cosG;
            rmatrix[E_RMATRIX_10] = sinA * cosG + cosA * sinB * sinG;
            rmatrix[E_RMATRIX_11] = cosA * cosB;
            rmatrix[E_RMATRIX_12] = sinA * sinG - cosA * sinB * cosG;
            rmatrix[E_RMATRIX_20] = -cosB * sinG;
            rmatrix[E_RMATRIX_21] = sinB;
            rmatrix[E_RMATRIX_22] = cosB * cosG;
        }
        case rtype_euler_samp: {
            rmatrix[E_RMATRIX_00] = cosG * cosB - sinG * sinA * sinB;
            rmatrix[E_RMATRIX_01] = -sinG * cosA;
            rmatrix[E_RMATRIX_02] = cosG * sinB + sinG * sinA * cosB;
            rmatrix[E_RMATRIX_10] = sinG * cosB + cosG * sinA * sinB;
            rmatrix[E_RMATRIX_11] = cosG * cosA;
            rmatrix[E_RMATRIX_12] = sinG * sinB - cosG * sinA * cosB;
            rmatrix[E_RMATRIX_20] = -cosA * sinB;
            rmatrix[E_RMATRIX_21] = sinA;
            rmatrix[E_RMATRIX_22] = cosA * cosB;
        }
    }
}

static stock ConvertQuatToMatrix(quat[E_QUAT], rotationtype: type[2], rmatrix[E_RMATRIX]) {
    /// <summary>Convert the given quaternion into a rotation matrix</summary>
    /// <param name="quat">Source quaternion [<ref name="E_QUAT"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="rmatrix">Dest rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <remarks>Used internally</remarks>

    debug("ConvertQuatToMatrix(...)");

    #pragma unused type

    new Float: w = quat[E_QUAT_W];
    new Float: x = quat[E_QUAT_X];
    new Float: y = quat[E_QUAT_Y];
    new Float: z = quat[E_QUAT_Z];

    new Float: w2 = w * w;
    new Float: x2 = x * x;
    new Float: y2 = y * y;
    new Float: z2 = z * z;
    new Float: wx = 2.0 * w * x;
    new Float: wy = 2.0 * w * y;
    new Float: wz = 2.0 * w * z;
    new Float: xy = 2.0 * x * y;
    new Float: xz = 2.0 * x * z;
    new Float: yz = 2.0 * y * z;

    rmatrix[E_RMATRIX_00] = w2 + x2 - y2 - z2;
    rmatrix[E_RMATRIX_10] = xy - wz;
    rmatrix[E_RMATRIX_20] = xz + wy;
    rmatrix[E_RMATRIX_01] = xy + wz;
    rmatrix[E_RMATRIX_11] = w2 - x2 + y2 - z2;
    rmatrix[E_RMATRIX_21] = yz - wx;
    rmatrix[E_RMATRIX_02] = xz - wy;
    rmatrix[E_RMATRIX_12] = yz + wx;
    rmatrix[E_RMATRIX_22] = w2 - x2 - y2 + z2;
}

// native ConvertRotation(src[E_ROTATION], rotationtype: type, dest[E_ROTATION]);
stock ConvertRotation(src[E_ROTATION], rotationtype: type, dest[E_ROTATION]) {
    /// <summary>Convert the given rotation matrix into the target rotation</summary>
    /// <export />
    /// <param name="src">Source rotation array</param>
    /// <param name="type">Dest rotation type</param>
    /// <param name="dest">Dest rotation array</param>

    debug("ConvertRotation(%d)", _: type);

    if(src[E_ROTATION_TYPE] == type) {
        dest = src;
    } else {
        static rotationtype: types[2];
        // stores src and dest type beforehand, it could be overwritten if src and dest are the same array
        types[0] = src[E_ROTATION_TYPE];
        types[1] = dest[E_ROTATION_TYPE] = type;
        // find corresponding function
        switch(type) {
            case rtype_axis_angle: {
                switch(types[0]) {
                    case rtype_quaternion: {
                        #emit push.c ConvertQuatToAxisAngle
                    }
                    case rtype_rotation_matrix: {
                        #emit push.c ConvertMatrixToAxisAngle
                    }
                    default: { // Euler
                        #emit push.c ConvertEulerToAxisAngle
                    }
                }
            }
            case rtype_quaternion: {
                switch(types[0]) {
                    case rtype_axis_angle: {
                        #emit push.c ConvertAxisAngleToQuat
                    }
                    case rtype_rotation_matrix: {
                        #emit push.c ConvertMatrixToQuat
                    }
                    default: { // Euler
                        #emit push.c ConvertEulerToQuat
                    }
                }
            }
            case rtype_rotation_matrix: {
                switch(types[0]) {
                    case rtype_axis_angle: {
                        #emit push.c ConvertAxisAngleToMatrix
                    }
                    case rtype_quaternion: {
                        #emit push.c ConvertQuatToMatrix
                    }
                    default: { // Euler
                        #emit push.c ConvertEulerToMatrix
                    }
                }
            }
            default: { // Euler
                switch(types[0]) {
                    case rtype_axis_angle: {
                        #emit push.c ConvertAxisAngleToEuler
                    }
                    case rtype_quaternion: {
                        #emit push.c ConvertQuatToEuler
                    }
                    case rtype_rotation_matrix: {
                        #emit push.c ConvertMatrixToEuler
                    }
                    default: { // Euler
                        #emit push.c ConvertEulerToEuler
                    }
                }
            }
        } {}
        // move src to data section
        #emit load.s.pri src
        #emit add.c E_ROTATION_DATA_OFFSET
        #emit stor.s.pri src
        // replace type address
        #emit const.pri types
        #emit stor.s.pri type
        // move dest to data section
        #emit load.s.pri dest
        #emit add.c E_ROTATION_DATA_OFFSET
        #emit stor.s.pri dest
        // load function from switch
        #emit pop.pri
        #emit add.c 4
        #emit sctrl 6
    }
}

static stock CombineQuat(left[E_QUAT], right[E_QUAT], dest[E_QUAT]) {
    /// <summary>Combines two quaternion</summary>
    /// <param name="left">Left quaternion [<ref name="E_QUAT"/>]</param>
    /// <param name="right">Right quaternion [<ref name="E_QUAT"/>]</param>
    /// <param name="dest">Dest quaternion [<ref name="E_QUAT"/>]</param>

    static // unroll variables for speed
        Float: w1, Float: x1, Float: y1, Float: z1,
        Float: w2, Float: x2, Float: y2, Float: z2
    ; // fill variables, but reverse the order because this code works with conjugated quaternion due to GetVehicleRotationQuat
    #emit load.s.pri right
    #emit const.alt w1
    #emit movs 16
    #emit load.s.pri left
    #emit const.alt w2
    #emit movs 16
    // quat multiplication, left * right = dest for but we use conjugated quaternion, so (dest)^-1 = (left * right)^-1 = (right)^-1 * (left)^-1
    dest[E_QUAT_W] = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
    dest[E_QUAT_X] = w1 * x2 + w2 * x1 + y1 * z2 - z1 * y2;
    dest[E_QUAT_Y] = w1 * y2 + w2 * y1 + z1 * x2 - x1 * z2;
    dest[E_QUAT_Z] = w1 * z2 + w2 * z1 + x1 * y2 - y1 * x2;
}

static stock CombineMatrix(left[E_RMATRIX], right[E_RMATRIX], dest[E_RMATRIX]) {
    /// <summary>Combines two rotation matrices</summary>
    /// <param name="left">Left rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="right">Right rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="dest">Dest rotation matrix [<ref name="E_RMATRIX"/>]</param>

    static // unroll variables for speed
        Float: m100, Float: m101, Float: m102,
        Float: m110, Float: m111, Float: m112,
        Float: m120, Float: m121, Float: m122,
        Float: m200, Float: m201, Float: m202,
        Float: m210, Float: m211, Float: m212,
        Float: m220, Float: m221, Float: m222
    ; // fill variables
    #emit load.s.pri left
    #emit const.alt m100
    #emit movs 36
    #emit load.s.pri right
    #emit const.alt m200
    #emit movs 36
    // row 0, left * right = dest
    dest[E_RMATRIX_00] = m100 * m200 + m101 * m210 + m102 * m220;
    dest[E_RMATRIX_01] = m100 * m201 + m101 * m211 + m102 * m221;
    dest[E_RMATRIX_02] = m100 * m202 + m101 * m212 + m102 * m222;
    // row 1
    dest[E_RMATRIX_10] = m110 * m200 + m111 * m210 + m112 * m220;
    dest[E_RMATRIX_11] = m110 * m201 + m111 * m211 + m112 * m221;
    dest[E_RMATRIX_12] = m110 * m202 + m111 * m212 + m112 * m222;
    // row 2
    dest[E_RMATRIX_20] = m120 * m200 + m121 * m210 + m122 * m220;
    dest[E_RMATRIX_21] = m120 * m201 + m121 * m211 + m122 * m221;
    dest[E_RMATRIX_22] = m120 * m202 + m121 * m212 + m122 * m222;
}

// native CombineRotation(left[E_ROTATION], right[E_ROTATION], dest[E_ROTATION]);
stock CombineRotation(left[E_ROTATION], right[E_ROTATION], dest[E_ROTATION]) {
    /// <summary>Combines two rotations, left * right = dest</summary>
    /// <export/>
    /// <param name="left">Left rotation [<ref name="E_AANGLE"/>]</param>
    /// <param name="right">Right rotation [<ref name="E_AANGLE"/>]</param>
    /// <param name="dest">Dest rotation [<ref name="E_AANGLE"/>]</param>
    /// <remarks>If <paramref name="left"/> or <paramref name="right"/> is a rotation matrix than <paramref name="left"/>, <paramref name="right"/> and <paramref name="dest"/> will be converted to rotation matrices, in all other cases <paramref name="left"/>, <paramref name="right"/> and <paramref name="dest"/> will be quaternion</remarks>
    /// <remarks>[second Rotation] * [first Rotation] = global frame of reference - global coordinate system / axis stay the same for the second rotation</remarks>
    /// <remarks>[first Rotation] * [second Rotation] = local frame-of-reference - coordinate system / axis rotates with first rotation</remarks>

    if(left[E_ROTATION_TYPE] == rtype_rotation_matrix) {
        if(right[E_ROTATION_TYPE] != rtype_rotation_matrix) {
            ConvertRotation(right, rtype_rotation_matrix, right);
        } {}
        #emit const.alt CombineMatrix
    } else if(right[E_ROTATION_TYPE] == rtype_rotation_matrix) {
        ConvertRotation(left, rtype_rotation_matrix, left);

        #emit const.alt CombineMatrix
    } else {
        ConvertRotation(left, rtype_quaternion, left);
        ConvertRotation(right, rtype_quaternion, right);
        // you can't combine axis angle or euler angles
        #emit const.alt CombineQuat
    } {}
    #emit load.s.pri left
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri left

    #emit load.s.pri right
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri right

    #emit load.s.pri dest
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri dest

    #emit move.pri
    #emit add.c 4
    #emit sctrl 6
}

static stock GetAxisAngleFromRotation(aangle[E_AANGLE], rotationtype: type, & Float: angle, & Float: x, & Float: y, & Float: z) {
    /// <summary>Get the axis angle from the rotation array</summary>
    /// <param name="aangle">Axis angle array [<ref name="E_AANGLE"/>]</param>
    /// <param name="type">Dest rotation type, unused</param>
    /// <param name="angle">Dest axis angle rotation</param>
    /// <param name="x">Dest axis angle unit vector x</param>
    /// <param name="y">Dest axis angle unit vector y</param>
    /// <param name="z">Dest axis angle unit vector z</param>

    #pragma unused type

    angle = aangle[E_AANGLE_ANGLE];
    x = aangle[E_AANGLE_X];
    y = aangle[E_AANGLE_Y];
    z = aangle[E_AANGLE_Z];

    debug("GetAxisAngleFromRotation(%f, %f, %f, %f)", angle, x, y, z);
}

static stock GetEulerFromRotation(euler[E_EULER], rotationtype: type, & Float: alpha, & Float: beta, & Float: gamma) {
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

static stock GetQuatFromRotation(quat[E_QUAT], rotationtype: type, & Float: w, & Float: x, & Float: y, & Float: z) {
    /// <summary>Get the quaternion from the rotation array</summary>
    /// <param name="quat">Quaternion array [<ref name="E_QUAT"/>]</param>
    /// <param name="type">Dest rotation type, unused</param>
    /// <param name="w">Dest quaternion scalar part</param>
    /// <param name="x">Dest quaternion imaginary part x</param>
    /// <param name="y">Dest quaternion imaginary part y</param>
    /// <param name="z">Dest quaternion imaginary part z</param>

    #pragma unused type

    w = quat[E_QUAT_W];
    x = quat[E_QUAT_X];
    y = quat[E_QUAT_Y];
    z = quat[E_QUAT_Z];

    debug("GetQuatFromRotation(%f, %f, %f, %f)", w, x, y, z);
}

static stock GetMatrixFromRotation(rmatrix[E_RMATRIX], rotationtype: type, Float: matrix[][]) {
    /// <summary>Get the matrix from the rotation array</summary>
    /// <param name="rmatrix">Rotation matrix array [<ref name="E_RMATRIX"/>]</param>
    /// <param name="type">Dest rotation type, unused</param>
    /// <param name="matrix">Dest matrix</param>

    #pragma unused type

    matrix[0][0] = rmatrix[E_RMATRIX_00];
    matrix[0][1] = rmatrix[E_RMATRIX_01];
    matrix[0][2] = rmatrix[E_RMATRIX_02];

    matrix[1][0] = rmatrix[E_RMATRIX_10];
    matrix[1][1] = rmatrix[E_RMATRIX_11];
    matrix[1][2] = rmatrix[E_RMATRIX_12];

    matrix[2][0] = rmatrix[E_RMATRIX_20];
    matrix[2][1] = rmatrix[E_RMATRIX_21];
    matrix[2][2] = rmatrix[E_RMATRIX_22];

    debug("GetMatrixFromRotation(...)");
}

// native GetRotation(rotation[E_ROTATION], rotationtype: type, Float: ...);
stock GetRotation(rotation[E_ROTATION], rotationtype: type, Float: ...) {
    /// <summary>Get the rotation from the given rotation array</summary>
    /// <export />
    /// <param name="rotation">Rotation array [<ref name="E_ROTATION"/>]</param>
    /// <param name="type">Dest rotation type</param>
    /// <param name="...">Dest rotation</param>

    debug("GetRotation(%d)", _: type);

    static tmp[E_ROTATION];

    ConvertRotation(rotation, type, tmp);
    // replace rotation array with converted one
    #emit const.pri tmp
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri rotation

    switch(type) {
        case rtype_axis_angle:{
            #emit const.pri GetAxisAngleFromRotation
        }
        case rtype_quaternion: {
            #emit const.pri GetQuatFromRotation
        }
        case rtype_rotation_matrix: {
            #emit const.pri GetMatrixFromRotation
        }
        default: { // Euler
            #emit const.pri GetEulerFromRotation
        }
    } {}
    #emit add.c 4
    #emit sctrl 6
}

static stock RotateAxisAngle(aangle[E_AANGLE], Float: cX, Float: cY, Float: cZ, Float: pX, Float: pY, Float: pZ, & Float: oX, & Float: oY, & Float: oZ) {
    /// <summary>Rotates the given point (pX, pY, pZ) and add it to the output position (oX, oY, oZ) using axis angle</summary>
    /// <param name="aangle">Axis angle array [<ref name="E_AANGLE"/>]</param>
    /// <param name="cX">Rotation origin x</param>
    /// <param name="cY">Rotation origin y</param>
    /// <param name="cZ">Rotation origin z</param>
    /// <param name="pX">Point x</param>
    /// <param name="pY">Point y</param>
    /// <param name="pZ">Point z</param>
    /// <param name="oX">Output position x</param>
    /// <param name="oY">Output position y</param>
    /// <param name="oZ">Output position z</param>
    /// <remarks>Used internally</remarks>
    /// <remarks>Uses Rodrigues' rotation formula</remarks>

    #pragma unused cX, cY, cZ

    debug("RotateAxisAngle(%f, %f, %f)", pX, pY, pZ);

    new Float: angle = aangle[E_AANGLE_ANGLE];
    new Float: aX = aangle[E_AANGLE_X];
    new Float: aY = aangle[E_AANGLE_Y];
    new Float: aZ = aangle[E_AANGLE_Z];
    // uses Rodrigues' rotation formula, similar to RotateQuat
    new Float: cos = floatcos(angle, degrees);
    new Float: sin = floatsin(angle, degrees);
    new Float: ts = (1.0 - cos) * (aX * pX + aY * pY + aZ * pZ);
    // u = unit vector = [aX, aY, aZ]
    // v = [rX, rY, rZ]
    // cos * v + sin * (u x v) + (1 - cos) * (u.v) * u
    oX += cos * pX + sin * (aY * pZ - aZ * pY) + ts * aX;
    oY += cos * pY + sin * (aZ * pX - aX * pZ) + ts * aY;
    oZ += cos * pZ + sin * (aX * pY - aY * pX) + ts * aZ;
}

static stock RotateQuat(quat[E_QUAT], Float: cX, Float: cY, Float: cZ, Float: pX, Float: pY, Float: pZ, & Float: oX, & Float: oY, & Float: oZ) {
    /// <summary>Rotates the given point (pX, pY, pZ) and add it to the output position (oX, oY, oZ) using quaternion</summary>
    /// <param name="quat">Quaternion array [<ref name="E_QUAT"/>]</param>
    /// <param name="cX">Rotation origin x</param>
    /// <param name="cY">Rotation origin y</param>
    /// <param name="cZ">Rotation origin z</param>
    /// <param name="pX">Point x</param>
    /// <param name="pY">Point y</param>
    /// <param name="pZ">Point z</param>
    /// <param name="oX">Output position x</param>
    /// <param name="oY">Output position y</param>
    /// <param name="oZ">Output position z</param>
    /// <remarks>Used internally</remarks>

    #pragma unused cX, cY, cZ

    debug("RotateQuat(%f, %f, %f)", pX, pY, pZ);

    new Float: w = quat[E_QUAT_W];
    new Float: x = quat[E_QUAT_X];
    new Float: y = quat[E_QUAT_Y];
    new Float: z = quat[E_QUAT_Z];
    // wikipedia suggest this formula (a x b - cross prudct)
    // v + 2*r x (r x v + w*v) | q = w,r
    // 15 muls, 15 adds, 9 saves, 42 loads
    // new
        // Float: tX = y * pZ - z * pY - w * pX,
        // Float: tY = z * pX - x * pZ - w * pY,
        // Float: tZ = x * pY - y * pX - w * pZ;
    //
    // x += x;
    // y += y;
    // z += z;
    //
    // oX += pX + y * tZ - z * tY;
    // oY += pY + z * tX - x * tZ;
    // oZ += pZ + x * tY - y * tX;
    // ---
    // but I used this one because it more compact (a.b - dot product)
    // 2*((r.v)*r + (0.5 - (|r.r|^2))*v + w * (r x v)) | q = w,r
    // 22 muls, 12 adds, 5 saves, 38 loads
    new Float: dot = x * pX + y * pY + z * pZ;
    new Float: abs = w * w - 0.5;

    oX += 2.0 * (dot * x + abs * pX + w * (z * pY - y * pZ));
    oY += 2.0 * (dot * y + abs * pY + w * (x * pZ - z * pX));
    oZ += 2.0 * (dot * z + abs * pZ + w * (y * pX - x * pY));
}

static stock RotateMatrix(rmatrix[E_RMATRIX], Float: cX, Float: cY, Float: cZ, Float: pX, Float: pY, Float: pZ, & Float: oX, & Float: oY, & Float: oZ) {
    /// <summary>Rotates the given point (pX, pY, pZ) and add it to the output position (oX, oY, oZ) using a rotation matrix</summary>
    /// <param name="rmatrix">Rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="cX">Rotation origin x</param>
    /// <param name="cY">Rotation origin y</param>
    /// <param name="cZ">Rotation origin z</param>
    /// <param name="pX">Point x</param>
    /// <param name="pY">Point y</param>
    /// <param name="pZ">Point z</param>
    /// <param name="oX">Output position x</param>
    /// <param name="oY">Output position y</param>
    /// <param name="oZ">Output position z</param>
    /// <remarks>Used internally</remarks>

    #pragma unused cX, cY, cZ

    debug("RotateMatrix(%f, %f, %f)", pX, pY, pZ);

    // oT = Translation, 0.0 for no translation, 1.0 for translation
    // M * v | v = [oX, oY, oZ]
    oX += pX * rmatrix[E_RMATRIX_00] + pY * rmatrix[E_RMATRIX_01] + pZ * rmatrix[E_RMATRIX_02];
    oY += pX * rmatrix[E_RMATRIX_10] + pY * rmatrix[E_RMATRIX_11] + pZ * rmatrix[E_RMATRIX_12];
    oZ += pX * rmatrix[E_RMATRIX_20] + pY * rmatrix[E_RMATRIX_21] + pZ * rmatrix[E_RMATRIX_22];
}

// native RotatePoint(rotation[E_ROTATION], Float: cX, Float: cY, Float: cZ, Float: pX, Float: pY, Float: pZ, & Float: oX, & Float: oY, & Float: oZ);
stock RotatePoint(rotation[E_ROTATION], Float: cX, Float: cY, Float: cZ, Float: pX, Float: pY, Float: pZ, & Float: oX, & Float: oY, & Float: oZ) {
    /// <summary>Rotates the given point (pX, pY, pZ) around origin (cX, cY, cZ) and stores the output position (oX, oY, oZ)</summary>
    /// <export />
    /// <param name="rotation">Rotation array [<ref name="E_ROTATION"/>]</param>
    /// <param name="cX">Rotation origin x</param>
    /// <param name="cY">Rotation origin y</param>
    /// <param name="cZ">Rotation origin z</param>
    /// <param name="pX">Point x</param>
    /// <param name="pY">Point y</param>
    /// <param name="pZ">Point z</param>
    /// <param name="oX">Output position x</param>
    /// <param name="oY">Output position y</param>
    /// <param name="oZ">Output position z</param>

    debug("RotatePoint(%f, %f, %f)", pX, pY, pZ);

    pX -= (oX = cX);
    pY -= (oY = cY);
    pZ -= (oZ = cZ);

    switch(rotation[E_ROTATION_TYPE]) {
        case rtype_axis_angle: {
            #emit const.alt RotateAxisAngle
        }
        case rtype_quaternion: {
            #emit const.alt RotateQuat
        }
        case rtype_rotation_matrix: {
            #emit const.alt RotateMatrix
        }
        default: { // Euler
            ConvertRotation(rotation, rtype_quaternion, rotation);
            // saddly I couldn't find a way to rotate euler angles without a step in between, is there even a way?
            // usually they are converted to rotation matrixes or quaternion and than rotated
            #emit const.alt RotateQuat
        }
    } {}
    #emit load.s.pri rotation
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri rotation

    #emit move.pri
    #emit add.c 4
    #emit sctrl 6
}

#undef E_ROTATION_DATA_OFFSET
#undef debug