#if defined _inc_rotation_quaternion_
	#endinput
#else
	#define _inc_rotation_quaternion_
#endif

enum E_QUAT {
    Float: E_QUAT_W,
    Float: E_QUAT_X,
    Float: E_QUAT_Y,
    Float: E_QUAT_Z
}
/// <summary>Quaternion enum (w, x, y, z)</summary>
/// <export>quat</export>

#if !defined E_AANGLE
    #include "rotation_axis_angle"
#elseif !defined E_EULER
    #include "rotation_euler"
#elseif !defined E_RMATRIX
    #include "rotation_matrix"
#endif

stock NormalizeQuat(quat[E_QUAT]) {
    /// <summary>Normalizes quaternion</summary>
    /// <export>quat</export>
    /// <param name="quat">Quaternion array [<ref name="E_QUAT"/>]</param>

    new Float: size = VectorSize(quat[E_QUAT_W], VectorSize(quat[E_QUAT_X], quat[E_QUAT_Y], quat[E_QUAT_Z]), 0.0);

    quat[E_QUAT_W] /= size;
    quat[E_QUAT_X] /= size;
    quat[E_QUAT_Y] /= size;
    quat[E_QUAT_Z] /= size;

    debug("NormalizeQuat(%f, %f, %f, %f)", quat[E_QUAT_W], quat[E_QUAT_X], quat[E_QUAT_Y], quat[E_QUAT_Z]);
}

stock SetRotationFromQuat(quat[E_QUAT], rotationtype: type, & Float: w, & Float: x, & Float: y, & Float: z) {
    /// <summary>Set the rotation array from quaternion</summary>
    /// <export>quat</export>
    /// <param name="quat">Quaternion array [<ref name="E_QUAT"/>]</param>
    /// <param name="type">Source rotation type, unused</param>
    /// <param name="w">Source quaternion scalar part</param>
    /// <param name="x">Source quaternion imaginary part x</param>
    /// <param name="y">Source quaternion imaginary part y</param>
    /// <param name="z">Source quaternion imaginary part z</param>

    debug("SetRotationFromQuat(%f, %f, %f, %f)", w, x, y, z);

    #pragma unused type

    quat[E_QUAT_W] = w;
    quat[E_QUAT_X] = x;
    quat[E_QUAT_Y] = y;
    quat[E_QUAT_Z] = z;

    NormalizeQuat(quat);
}

stock ConvertAxisAngleToQuat(aangle[E_AANGLE], rotationtype: type[2], quat[E_QUAT]) {
    /// <summary>Convert the given axis angle into quaternion</summary>
    /// <export>quat</export>
    /// <param name="aangle">Source axis angle [<ref name="E_AANGLE"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="quat">Dest quaternion [<ref name="E_QUAT"/>]</param>

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

stock ConvertEulerToQuat(euler[E_EULER], rotationtype: type[2], quat[E_QUAT]) {
    /// <summary>Convert the given euler angle into quaternion</summary>
    /// <export>quat</export>
    /// <param name="euler">Source euler angle [<ref name="E_EULER"/>]</param>
    /// <param name="type">Rotation type</param>
    /// <param name="quat">Dest quaternion [<ref name="E_QUAT"/>]</param>

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
        case rtype_euler_re_xzy: {
            quat[E_QUAT_W] = cosA * cosB * cosG + sinA * sinB * sinG;
            quat[E_QUAT_X] = cosA * sinB * sinG - sinA * cosB * cosG;
            quat[E_QUAT_Y] = sinA * cosB * sinG - cosA * sinB * cosG;
            quat[E_QUAT_Z] = -(cosA * cosB * sinG + sinA * sinB * cosG);
        }
        case rtype_euler_re_yxz: {
            quat[E_QUAT_W] = cosA * cosB * cosG + sinA * sinB * sinG;
            quat[E_QUAT_X] = -(sinA * cosB * cosG + cosA * sinB * sinG);
            quat[E_QUAT_Y] = sinA * cosB * sinG - cosA * sinB * cosG;
            quat[E_QUAT_Z] = sinA * sinB * cosG - cosA * cosB * sinG;
        }
        case rtype_euler_re_yzx: {
            quat[E_QUAT_W] = sinA * sinB * sinG - cosA * cosB * cosG;
            quat[E_QUAT_X] = sinA * cosB * cosG + cosA * sinB * sinG;
            quat[E_QUAT_Y] = cosA * sinB * cosG + sinA * cosB * sinG;
            quat[E_QUAT_Z] = cosA * cosB * sinG - sinA * sinB * cosG;
        }
        case rtype_euler_re_zyx: {
            quat[E_QUAT_W] = cosA * cosB * cosG + sinA * sinB * sinG;
            quat[E_QUAT_X] = cosA * sinB * sinG - sinA * cosB * cosG;
            quat[E_QUAT_Y] = -(cosA * sinB * cosG + sinA * cosB * sinG);
            quat[E_QUAT_Z] = sinA * sinB * cosG - cosA * cosB * sinG;
        }
        case rtype_euler_re_zxy: {
            quat[E_QUAT_W] = sinA * sinB * sinG - cosA * cosB * cosG;
            quat[E_QUAT_X] = sinA * cosB * cosG - cosA * sinB * sinG;
            quat[E_QUAT_Y] = cosA * sinB * cosG + sinA * cosB * sinG;
            quat[E_QUAT_Z] = cosA * cosB * sinG + sinA * sinB * cosG;
        }
    }
    debug("ConvertEulerToQuat(%f, %f, %f, %f)", quat[E_QUAT_W], quat[E_QUAT_X], quat[E_QUAT_Y], quat[E_QUAT_Z]);
}

stock ConvertMatrixToQuat(rmatrix[E_RMATRIX], rotationtype: type[2], quat[E_QUAT]) {
    /// <summary>Convert the given rotation matrix into quaternion</summary>
    /// <export>quat</export>
    /// <param name="rmatrix">Source rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="quat">Dest quaternion [<ref name="E_QUAT"/>]</param>

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

stock CombineQuat(left[E_QUAT], right[E_QUAT], dest[E_QUAT]) {
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

    debug("CombineQuat(%f %f %f %f)", dest[E_QUAT_W], dest[E_QUAT_X], dest[E_QUAT_Y], dest[E_QUAT_Z]);
}

stock GetQuatFromRotation(quat[E_QUAT], rotationtype: type, & Float: w, & Float: x, & Float: y, & Float: z) {
    /// <summary>Get the quaternion from the rotation array</summary>
    /// <export>quat</export>
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

stock RotateQuat(quat[E_QUAT], Float: cX, Float: cY, Float: cZ, Float: pX, Float: pY, Float: pZ, & Float: oX, & Float: oY, & Float: oZ) {
    /// <summary>Rotates the given point (pX, pY, pZ) and add it to the output position (oX, oY, oZ) using quaternion</summary>
    /// <export>quat</export>
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

stock ReverseQuat(src[E_QUAT], dest[E_QUAT]) {
    /// <summary>Stores the conjugated quaternion in dest</summary>
    /// <export>quat</export>
    /// <param name="src">Source quaternion [<ref name="E_QUAT"/>]</param>
    /// <param name="dest">Dest quaternion [<ref name="E_QUAT"/>]</param>

    debug("ReverseQuat(...)");

    dest = src; // similar to axis angle either negate w or x, y, and z
    dest[E_QUAT_W] *= -1.0; // though only in 3d, if I am not mistaken in higher dimension it q = -q isn't valid anymore
    // but conj(q) is always valid, also negate the imaginary part
}