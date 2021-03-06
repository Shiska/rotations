#if defined _inc_rotation_axis_angle_
    #endinput
#else
    #define _inc_rotation_axis_angle_
#endif

enum E_AANGLE {
    Float: E_AANGLE_ANGLE,
    Float: E_AANGLE_X,
    Float: E_AANGLE_Y,
    Float: E_AANGLE_Z
}
/// <summary>Axis angle enum (angle, x, y, z)</summary>
/// <export>aangle</export>

#if !defined E_EULER
    #include "rotation_euler"
#elseif !defined E_QUAT
    #include "rotation_quaternion"
#elseif !defined E_RMATRIX
    #include "rotation_matrix"
#endif

stock NormalizeAxisAngle(aangle[E_AANGLE]) {
    /// <summary>Normalizes axis angle</summary>
    /// <export>aangle</export>
    /// <param name="aangle">Axis angle array [<ref name="E_AANGLE"/>]</param>

    new Float: size = VectorSize(aangle[E_AANGLE_X], aangle[E_AANGLE_Y], aangle[E_AANGLE_Z]);
    // normalize vector, angle doesn't matter
    aangle[E_AANGLE_X] /= size;
    aangle[E_AANGLE_Y] /= size;
    aangle[E_AANGLE_Z] /= size;

    debug("NormalizeAxisAngle(...)", aangle[E_AANGLE_ANGLE], aangle[E_AANGLE_X], aangle[E_AANGLE_Y], aangle[E_AANGLE_Z]);
}

#pragma warning disable 238

stock SetRotationFromAxisAngle(aangle[E_AANGLE], const rotationtype: type, const & Float: angle, const & Float: x, const & Float: y, const & Float: z) {
    /// <summary>Set the rotation array from axis angle</summary>
    /// <export>aangle</export>
    /// <param name="aangle">Axis angle array [<ref name="E_AANGLE"/>]</param>
    /// <param name="type">Source rotation type, unused</param>
    /// <param name="angle">Source axis angle rotation</param>
    /// <param name="x">Source axis angle unit vector x</param>
    /// <param name="y">Source axis angle unit vector y</param>
    /// <param name="z">Source axis angle unit vector z</param>

    debug("SetRotationFromAxisAngle(%f, %f, %f, %f)", angle, x, y, z);

    #pragma unused type

    aangle[E_AANGLE_ANGLE] = angle;
    aangle[E_AANGLE_X] = x;
    aangle[E_AANGLE_Y] = y;
    aangle[E_AANGLE_Z] = z;

    NormalizeAxisAngle(aangle);
}

#pragma warning enable 238

stock ConvertEulerToAxisAngle(const euler[E_EULER], const rotationtype: type[2], aangle[E_AANGLE]) {
    /// <summary>Convert the given euler angle into axis angle</summary>
    /// <export>aangle</export>
    /// <param name="euler">Source euler angle [<ref name="E_EULER"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="aangle">Dest axis angle [<ref name="E_AANGLE"/>]</param>
    /// <remarks>Not optimized, uses <ref name="ConvertEulerToQuat"/> and <ref name="ConvertQuatToAxisAngle"/> for the conversion</remarks>

    new quat[E_QUAT];
    new rotationtype: ntype[2];

    ntype[0] = type[0];
    ntype[1] = rtype_quaternion;

    ConvertEulerToQuat(euler, ntype, quat);
    ConvertQuatToAxisAngle(quat, rotationtype: {rtype_quaternion, rtype_axis_angle}, aangle);

    debug("ConvertEulerToAxisAngle(%f, %f, %f, %f)", aangle[E_AANGLE_ANGLE], aangle[E_AANGLE_X], aangle[E_AANGLE_Y], aangle[E_AANGLE_Z]);
}

stock ConvertQuatToAxisAngle(const quat[E_QUAT], const rotationtype: type[2], aangle[E_AANGLE]) {
    /// <summary>Convert the given quaternion into axis angle</summary>
    /// <export>aangle</export>
    /// <param name="quat">Source quaternion [<ref name="E_QUAT"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="aangle">Dest axis angle [<ref name="E_AANGLE"/>]</param>

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

stock ConvertMatrixToAxisAngle(const rmatrix[E_RMATRIX], const rotationtype: type[2], aangle[E_AANGLE]) {
    /// <summary>Convert the given rotation matrix into axis angle</summary>
    /// <export>aangle</export>
    /// <param name="rmatrix">Source rotation matrix [<ref name="E_RMATRIX"/>]</param>
    /// <param name="type">Rotation type, unused</param>
    /// <param name="aangle">Dest axis angle [<ref name="E_AANGLE"/>]</param>

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

stock GetAxisAngleFromRotation(const aangle[E_AANGLE], const rotationtype: type, & Float: angle, & Float: x, & Float: y, & Float: z) {
    /// <summary>Get the axis angle from the rotation array</summary>
    /// <export>aangle</export>
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

stock RotateAxisAngle(const aangle[E_AANGLE], Float: cX, Float: cY, Float: cZ, Float: pX, Float: pY, Float: pZ, & Float: oX, & Float: oY, & Float: oZ) {
    /// <summary>Rotates the given point (pX, pY, pZ) and add it to the output position (oX, oY, oZ) using axis angle</summary>
    /// <export>aangle</export>
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
    /// <remarks>Uses Rodrigues' rotation formula</remarks>

    pX -= cX;
    pY -= cY;
    pZ -= cZ;

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
    oX = cX + cos * pX + sin * (aY * pZ - aZ * pY) + ts * aX;
    oY = cY + cos * pY + sin * (aZ * pX - aX * pZ) + ts * aY;
    oZ = cZ + cos * pZ + sin * (aX * pY - aY * pX) + ts * aZ;
}

stock ReverseAxisAngle(const src[E_AANGLE], dest[E_AANGLE]) {
    /// <summary>Stores the reverse axis angle in dest</summary>
    /// <export>aangle</export>
    /// <param name="src">Source axis angle [<ref name="E_AANGLE"/>]</param>
    /// <param name="dest">Dest axis angle [<ref name="E_AANGLE"/>]</param>

    debug("ReverseAxisAngle(...)");

    dest = src; // just negate rotation angle
    dest[E_AANGLE_ANGLE] *= -1.0; // you could also negate the unit vector instead
}