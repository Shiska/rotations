#if defined _inc_rotation_rotation_matrix_
	#endinput
#else
	#define _inc_rotation_rotation_matrix_
#endif

enum E_RMATRIX {
    Float: E_RMATRIX_00, Float: E_RMATRIX_01, Float: E_RMATRIX_02,
    Float: E_RMATRIX_10, Float: E_RMATRIX_11, Float: E_RMATRIX_12,
    Float: E_RMATRIX_20, Float: E_RMATRIX_21, Float: E_RMATRIX_22
}
/// <summary>Rotation matrix enum (3 rows, 4 coloums)</summary>
/// <export>rmatrix</export>

#if !defined E_AANGLE
    #include "rotation_axis_angle"
#elseif !defined E_EULER
    #include "rotation_euler"
#elseif !defined E_QUAT
    #include "rotation_quaternion"
#endif

static // unroll variables for speed
    Float: m100, Float: m101, Float: m102,
    Float: m110, Float: m111, Float: m112,
    Float: m120, Float: m121, Float: m122,
    Float: m200, Float: m201, Float: m202,
    Float: m210, Float: m211, Float: m212,
    Float: m220, Float: m221, Float: m222
;

stock NormalizeMatrix(rmatrix[E_RMATRIX]) {
    /// <summary>Normalizes rotation matrix with Householder reflections</summary>
    /// <export>rmatrix</export>
    /// <param name="rmatrix">Rotation matrix [<ref name="E_RMATRIX"/>]</param>
    // https://en.wikipedia.org/wiki/QR_decomposition

    debug("NormalizeMatrix(...)");

    new // using Householder reflections for the first column
        Q[E_RMATRIX],
        Float: uX = rmatrix[E_RMATRIX_00],
        Float: uY = rmatrix[E_RMATRIX_10],
        Float: uZ = rmatrix[E_RMATRIX_20],
        Float: norm = VectorSize(uX, uY, uZ),
        bool: swap = (uX >= 0.0)
    ;
    if(swap) { // swapping orientation to avoid loss of significance in floating point arithmetic
        uX += norm;
    } else {
        uX -= norm;
    }
    norm = VectorSize(uX, uY, uZ);

    if(norm == 0.0) {
        uX = -1.0;
        uY = uZ = 0.0;
    } else {
        uX /= norm;
        uY /= norm;
        uZ /= norm;
    }
    Q[E_RMATRIX_00] = 1.0 - 2.0 * uX * uX;
    Q[E_RMATRIX_11] = 1.0 - 2.0 * uY * uY;
    Q[E_RMATRIX_22] = 1.0 - 2.0 * uZ * uZ;
    // The calculation look similar to the quaternion to matrix conversion
    // well it uses a hyperplane like quaternion so I guess it is somehow related
    Q[E_RMATRIX_01] = Q[E_RMATRIX_10] = -2.0 * uX * uY;
    Q[E_RMATRIX_02] = Q[E_RMATRIX_20] = -2.0 * uX * uZ;
    Q[E_RMATRIX_12] = Q[E_RMATRIX_21] = -2.0 * uY * uZ;

    CombineMatrix(Q, rmatrix, rmatrix); // the householder matrix is unitary, no need to transpose it

    if(swap) { // fix sign, so the orientation stays the same
        Q[E_RMATRIX_00] = -Q[E_RMATRIX_00];
        Q[E_RMATRIX_10] = -Q[E_RMATRIX_10];
        Q[E_RMATRIX_20] = -Q[E_RMATRIX_20];
    } // same thing with second column
    uX = rmatrix[E_RMATRIX_11],
    uY = rmatrix[E_RMATRIX_21],
    norm = VectorSize(uX, uY, 0.0);
    swap = (uX >= 0.0);

    if(swap) {
        uX += norm;
    } else {
        uX -= norm;
    }
    norm = VectorSize(uX, uY, 0.0);

    uX /= norm;
    uY /= norm;

    rmatrix[E_RMATRIX_00] = 1.0;
    rmatrix[E_RMATRIX_01] = rmatrix[E_RMATRIX_02] = rmatrix[E_RMATRIX_10] = rmatrix[E_RMATRIX_20] = 0.0;

    rmatrix[E_RMATRIX_11] = 1 - 2 * uX * uX;
    rmatrix[E_RMATRIX_22] = 1 - 2 * uY * uY;

    rmatrix[E_RMATRIX_12] = rmatrix[E_RMATRIX_21] = -2 * uX * uY;

    CombineMatrix(Q, rmatrix, rmatrix);

    if(swap) {
        rmatrix[E_RMATRIX_01] = -rmatrix[E_RMATRIX_01];
        rmatrix[E_RMATRIX_11] = -rmatrix[E_RMATRIX_11];
        rmatrix[E_RMATRIX_21] = -rmatrix[E_RMATRIX_21];
    }
}

stock SetRotationFromMatrix(rmatrix[E_RMATRIX], const rotationtype: type, const Float: matrix[][]) {
    /// <summary>Set the rotation array from rotation matrix</summary>
    /// <export>rmatrix</export>
    /// <param name="rmatrix">Rotation matrix array [<ref name="E_RMATRIX"/>]</param>
    /// <param name="type">Source rotation type, unused</param>
    /// <param name="matrix">Source matrix[row][coloum]</param>
 
    debug("SetRotationFromMatrix(...)");

    #pragma unused type
    // no direct copy into rmatrix because I could be possible that the index of matrix was modified
    rmatrix[E_RMATRIX_00] = matrix[0][0];
    rmatrix[E_RMATRIX_01] = matrix[0][1];
    rmatrix[E_RMATRIX_02] = matrix[0][2];

    rmatrix[E_RMATRIX_10] = matrix[1][0];
    rmatrix[E_RMATRIX_11] = matrix[1][1];
    rmatrix[E_RMATRIX_12] = matrix[1][2];

    rmatrix[E_RMATRIX_20] = matrix[2][0];
    rmatrix[E_RMATRIX_21] = matrix[2][1];
    rmatrix[E_RMATRIX_22] = matrix[2][2];

    NormalizeMatrix(rmatrix);
}

stock ConvertAxisAngleToMatrix(const aangle[E_AANGLE], const rotationtype: type[2], rmatrix[E_RMATRIX]) {
    /// <summary>Convert the given axis angle into a rotation matrix</summary>
    /// <export>rmatrix</export>
    /// <param name="aangle">Source axis angle [<ref name="E_AANGLE"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="rmatrix">Dest rotation matrix [<ref name="E_RMATRIX"/>]</param>

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

stock ConvertEulerToMatrix(const euler[E_EULER], const rotationtype: type[2], rmatrix[E_RMATRIX]) {
    /// <summary>Convert the given euler angle into a rotation matrix</summary>
    /// <export>rmatrix</export>
    /// <param name="euler">Source euler angle [<ref name="E_EULER"/>]</param>
    /// <param name="type">Rotation type</param>
    /// <param name="rmatrix">Dest rotation matrix [<ref name="E_RMATRIX"/>]</param>

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
        case rtype_euler_re_xzy: {
            rmatrix[E_RMATRIX_00] = cosG * cosB;
            rmatrix[E_RMATRIX_01] = -sinG;
            rmatrix[E_RMATRIX_02] = cosG * sinB;
            rmatrix[E_RMATRIX_10] = sinA * sinB + cosA * cosB * sinG;
            rmatrix[E_RMATRIX_11] = cosA * cosG;
            rmatrix[E_RMATRIX_12] = cosA * sinG * sinB - cosB * sinA;
            rmatrix[E_RMATRIX_20] = cosB * sinA * sinG - cosA * sinB;
            rmatrix[E_RMATRIX_21] = cosG * sinA;
            rmatrix[E_RMATRIX_22] = cosA * cosB + sinA * sinG * sinB;
        }
        case rtype_euler_re_yxz: {
            rmatrix[E_RMATRIX_00] = cosB * cosG + sinB * sinA * sinG;
            rmatrix[E_RMATRIX_01] = cosG * sinB * sinA - cosB * sinG;
            rmatrix[E_RMATRIX_02] = cosA * sinB;
            rmatrix[E_RMATRIX_10] = cosA * sinG;
            rmatrix[E_RMATRIX_11] = cosA * cosG;
            rmatrix[E_RMATRIX_12] = -sinA;
            rmatrix[E_RMATRIX_20] = cosB * sinA * sinG - cosG * sinB;
            rmatrix[E_RMATRIX_21] = cosB * cosG * sinA + sinB * sinG;
            rmatrix[E_RMATRIX_22] = cosB * cosA;
        }
        case rtype_euler_re_yzx: {
            rmatrix[E_RMATRIX_00] = cosB * cosG;
            rmatrix[E_RMATRIX_01] = sinB * sinA - cosB * cosA * sinG;
            rmatrix[E_RMATRIX_02] = cosA * sinB + cosB * sinG * sinA;
            rmatrix[E_RMATRIX_10] = sinG;
            rmatrix[E_RMATRIX_11] = cosG * cosA;
            rmatrix[E_RMATRIX_12] = -cosG * sinA;
            rmatrix[E_RMATRIX_20] = -cosG * sinB;
            rmatrix[E_RMATRIX_21] = cosB * sinA + cosA * sinB * sinG;
            rmatrix[E_RMATRIX_22] = cosB * cosA - sinB * sinG * sinA;
        }
        case rtype_euler_re_zyx: {
            rmatrix[E_RMATRIX_00] = cosG * cosB;
            rmatrix[E_RMATRIX_01] = cosG * sinB * sinA - cosA * sinG;
            rmatrix[E_RMATRIX_02] = sinG * sinA + cosG * cosA * sinB;
            rmatrix[E_RMATRIX_10] = cosB * sinG;
            rmatrix[E_RMATRIX_11] = cosG * cosA + sinG * sinB * sinA;
            rmatrix[E_RMATRIX_12] = cosA * sinG * sinB - cosG * sinA;
            rmatrix[E_RMATRIX_20] = -sinB;
            rmatrix[E_RMATRIX_21] = cosB * sinA;
            rmatrix[E_RMATRIX_22] = cosB * cosA;
        }
        case rtype_euler_re_zxy: {
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

stock ConvertQuatToMatrix(const quat[E_QUAT], const rotationtype: type[2], rmatrix[E_RMATRIX]) {
    /// <summary>Convert the given quaternion into a rotation matrix</summary>
    /// <export>rmatrix</export>
    /// <param name="quat">Source quaternion [<ref name="E_QUAT"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="rmatrix">Dest rotation matrix [<ref name="E_RMATRIX"/>]</param>

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

stock CombineMatrix(const left[E_RMATRIX], const right[E_RMATRIX], dest[E_RMATRIX]) {
    /// <summary>Combines two rotation matrices</summary>
    /// <export>rmatrix</export>
    /// <param name="left">Left rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="right">Right rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="dest">Dest rotation matrix [<ref name="E_RMATRIX"/>]</param>

    debug("CombineMatrix(...)");
    // fill variables
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

stock GetMatrixFromRotation(const rmatrix[E_RMATRIX], const rotationtype: type, Float: matrix[][]) {
    /// <summary>Get the matrix from the rotation array</summary>
    /// <export>rmatrix</export>
    /// <param name="rmatrix">Rotation matrix array [<ref name="E_RMATRIX"/>]</param>
    /// <param name="type">Dest rotation type, unused</param>
    /// <param name="matrix">Dest matrix</param>

    #pragma unused type
    // fill variables
    #emit load.s.pri rmatrix
    #emit const.alt m100
    #emit movs 36
    // no direct copy into matrix because I could be possible that the index of matrix was modified
    matrix[0][0] = m100;
    matrix[0][1] = m101;
    matrix[0][2] = m102;
    matrix[1][0] = m110;
    matrix[1][1] = m111;
    matrix[1][2] = m112;
    matrix[2][0] = m120;
    matrix[2][1] = m121;
    matrix[2][2] = m122;

    debug("GetMatrixFromRotation(...)");
}

stock RotateMatrix(const rmatrix[E_RMATRIX], Float: cX, Float: cY, Float: cZ, Float: pX, Float: pY, Float: pZ, & Float: oX, & Float: oY, & Float: oZ) {
    /// <summary>Rotates the given point (pX, pY, pZ) and add it to the output position (oX, oY, oZ) using a rotation matrix</summary>
    /// <export>rmatrix</export>
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

    pX -= cX;
    pY -= cY;
    pZ -= cZ;

    debug("RotateMatrix(%f, %f, %f)", pX, pY, pZ);
    // fill variables
    #emit load.s.pri rmatrix
    #emit const.alt m100
    #emit movs 36
    // oT = Translation, 0.0 for no translation, 1.0 for translation
    // M * v | v = [oX, oY, oZ]
    oX = cX + pX * m100 + pY * m101 + pZ * m102;
    oY = cY + pX * m110 + pY * m111 + pZ * m112;
    oZ = cZ + pX * m120 + pY * m121 + pZ * m122;
}

stock ReverseMatrix(const src[E_RMATRIX], dest[E_RMATRIX]) {
    /// <summary>Stores the inverse rotation matrix in dest</summary>
    /// <export>rmatrix</export>
    /// <param name="src">Source rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="dest">Dest rotation matrix [<ref name="E_RMATRIX"/>]</param>

    debug("ReverseMatrix(...)");
    // fill variables
    #emit load.s.pri src
    #emit const.alt m100
    #emit movs 36
    // the inverse is just the transpose matrix
    dest[E_RMATRIX_00] = m100;
    dest[E_RMATRIX_01] = m110;
    dest[E_RMATRIX_02] = m120;
    dest[E_RMATRIX_10] = m101;
    dest[E_RMATRIX_11] = m111;
    dest[E_RMATRIX_12] = m121;
    dest[E_RMATRIX_20] = m102;
    dest[E_RMATRIX_21] = m112;
    dest[E_RMATRIX_22] = m122;
}