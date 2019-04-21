#if defined _inc_rotation_misc_quaternion_
    #endinput
#else
    #define _inc_rotation_misc_quaternion_
#endif

stock QuatNormalise(& Float: w, & Float: x, & Float: y, & Float: z) {
    /// <summary>Normalizes quaternion</summary>
    /// <export>quat</export>
    /// <param name="w">Quaternion scalar part</param>
    /// <param name="x">Quaternion imaginary part x</param>
    /// <param name="y">Quaternion imaginary part y</param>
    /// <param name="z">Quaternion imaginary part z</param>

    debug("QuatNormalise(%f, %f, %f, %f)", w, x, y, z);

    new Float: size = VectorSize(w, VectorSize(x, y, z), 0.0);

    w /= size;
    x /= size;
    y /= size;
    z /= size;
}

stock QuatScale(& Float: w, & Float: x, & Float: y, & Float: z, Float: s) {
    /// <summary>Scales quaternion by s</summary>
    /// <export>quat</export>
    /// <param name="w">Quaternion scalar part</param>
    /// <param name="x">Quaternion imaginary part x</param>
    /// <param name="y">Quaternion imaginary part y</param>
    /// <param name="z">Quaternion imaginary part z</param>
    /// <param name="s">Scale factor</param>

    debug("QuatScale(%f, %f, %f, %f)", w, x, y, z);

    w *= s;
    x *= s;
    y *= s;
    z *= s;
}

stock QuatMul(Float: w1, Float: x1, Float: y1, Float: z1, Float: w2, Float: x2, Float: y2, Float: z2, & Float: w, & Float: x, & Float: y, & Float: z) {
    /// <summary>Multiplies two quaternion</summary>
    /// <export>quat</export>
    /// <param name="w1">First quaternion scalar part</param>
    /// <param name="x1">First quaternion imaginary part x</param>
    /// <param name="y1">First quaternion imaginary part y</param>
    /// <param name="z1">First quaternion imaginary part z</param>
    /// <param name="w2">Second quaternion scalar part</param>
    /// <param name="x2">Second quaternion imaginary part x</param>
    /// <param name="y2">Second quaternion imaginary part y</param>
    /// <param name="z2">Second quaternion imaginary part z</param>

    debug("QuatMul(%f, %f, %f, %f, %f, %f, %f, %f)", w1, x1, y1, z1, w2, x2, y2, z2);

    // v1v2 = (v1 x v2) - (v1 . v2) | (w1, v1)(w2, v2) = (w1 * w2 - v1 . v2), (w1 * v2) + (w2 * v1) + (v1 x v2)
    w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
    x = w1 * x2 + w2 * x1 + y1 * z2 - z1 * y2;
    y = w1 * y2 + w2 * y1 + z1 * x2 - x1 * z2;
    z = w1 * z2 + w2 * z1 + x1 * y2 - y1 * x2;
}

stock QuatAdd(Float: w1, Float: x1, Float: y1, Float: z1, Float: w2, Float: x2, Float: y2, Float: z2, & Float: w3, & Float: x3, & Float: y3, & Float: z3) {
    /// <summary>Adds two quaternion together</summary>
    /// <export>quat</export>
    /// <param name="w1">First quaternion scalar part</param>
    /// <param name="x1">First quaternion imaginary part x</param>
    /// <param name="y1">First quaternion imaginary part y</param>
    /// <param name="z1">First quaternion imaginary part z</param>
    /// <param name="w2">Second quaternion scalar part</param>
    /// <param name="x2">Second quaternion imaginary part x</param>
    /// <param name="y2">Second quaternion imaginary part y</param>
    /// <param name="z2">Second quaternion imaginary part z</param>

    debug("QuatAdd(%f, %f, %f, %f, %f, %f, %f, %f)", w1, x1, y1, z1, w2, x2, y2, z2);

    w3 = w1 + w2;
    x3 = x1 + x2;
    y3 = y1 + y2;
    z3 = z1 + z2;
}