#if defined _inc_rotation_misc
	#undef _inc_rotation_misc
#endif

#if defined _rotation_misc_included
	#endinput
#endif
#define _rotation_misc_included

#include "rotation.inc"

// unfinished or random functions

// misc - Get real / offset position of attached vehicle objects

stock GetVehicleObjectPositionWorld(vehicleid,
    Float: att_X, Float: att_Y, Float: att_Z, Float: att_rotX, Float: att_rotY, Float: att_rotZ,
    &Float: X, &Float: Y, &Float: Z, &Float: rotX, &Float: rotY, &Float: rotZ
    ) {
    if(GetVehiclePos(vehicleid, X, Y, Z)) {
        new
            Float: w,
            Float: x,
            Float: y,
            Float: z,
            Float: matrix1[4][4],
            Float: matrix2[4][4];

        GetVehicleRotationQuat(vehicleid, w, x, y, z);

        GetRotationMatrixFromEuler(matrix1, att_rotX, att_rotY, att_rotZ);
        GetRotationMatrixFromQuat(matrix2, w, x, y, z);

        MatrixRotate(matrix2, att_X, att_Y, att_Z, 0.0, X, Y, Z);

        RotateMatrixWithMatrix(matrix1, matrix2);
        GetEulerFromMatrix(matrix1, rotX, rotY, rotZ);

        return true;
    }
    return false;
}

stock GetVehicleObjectPositionOffset(vehicleid,
    Float: X, Float: Y, Float: Z, Float: rotX, Float: rotY, Float: rotZ,
    &Float: att_X, &Float: att_Y, &Float: att_Z, &Float: att_rotX, &Float: att_rotY, &Float: att_rotZ
    ) {
    if(GetVehiclePos(vehicleid, att_X, att_Y, att_Z)) {
        new
            Float: w,
            Float: x,
            Float: y,
            Float: z,
            Float: matrix1[4][4],
            Float: matrix2[4][4];

        GetVehicleRotationQuat(vehicleid, w, x, y, z);

        GetRotationMatrixFromEuler(matrix1, rotX, rotY, rotZ);
        GetRotationMatrixFromQuat(matrix2, -w, x, y, z);

        X -= att_X;
        Y -= att_Y;
        Z -= att_Z;

        att_X = matrix2[0][0] * X + matrix2[0][1] * Y + matrix2[0][2] * Z;
        att_Y = matrix2[1][0] * X + matrix2[1][1] * Y + matrix2[1][2] * Z;
        att_Z = matrix2[2][0] * X + matrix2[2][1] * Y + matrix2[2][2] * Z;

        RotateMatrixWithMatrix(matrix1, matrix2);
        GetEulerFromMatrix(matrix1, att_rotX, att_rotY, att_rotZ);

        return true;
    }
    return false;
}

stock MatrixMulMatrix(Float: A[][], Float: B[][], Float: C[][], a1 = sizeof A, a2 = sizeof A[], b1 = sizeof B, b2 = sizeof B[], c1 = sizeof C, c2 = sizeof C[]) {
    if(a1 == c1 && a2 == b1 && b2 == c2) {
        for(a1 = 0; a1 < c1; ++a1) {
            for(b2 = 0; b2 < c2; ++b2) {
                for(C[a1][b2] = Float: a2 = 0; a2 < b1; ++a2) {
                    C[a1][b2] += A[a1][a2] * B[a2][b2];
                }
            }
        }
    }
}


stock GetRotMatX(Float: matrix[][], Float: angle) {
    matrix[0][0] = 1.0;
    matrix[0][1] = 0.0;
    matrix[0][2] = 0.0;
    matrix[1][0] = 0.0;
    matrix[2][0] = 0.0;
    matrix[2][2] = matrix[1][1] = floatcos(angle, degrees);
    matrix[1][2] = -(matrix[2][1] = floatsin(angle, degrees));
}

stock GetRotMatY(Float: matrix[][], Float: angle) {
    matrix[1][1] = 1.0;
    matrix[0][1] = 0.0;
    matrix[1][0] = 0.0;
    matrix[1][2] = 0.0;
    matrix[2][1] = 0.0;
    matrix[2][2] = matrix[0][0] = floatcos(angle, degrees);
    matrix[2][0] = -(matrix[0][2] = floatsin(angle, degrees));
}

stock GetRotMatZ(Float: matrix[][], Float: angle) {
    matrix[2][2] = 1.0;
    matrix[0][2] = 0.0;
    matrix[1][2] = 0.0;
    matrix[2][0] = 0.0;
    matrix[2][1] = 0.0;
    matrix[1][1] = matrix[0][0] = floatcos(angle, degrees);
    matrix[0][1] = -(matrix[1][0] = floatsin(angle, degrees));
}

stock MatrixSub(Float: A[][], Float: B[][], Float: C[][], a1 = sizeof A, a2 = sizeof A[], b1 = sizeof B, b2 = sizeof B[], c1 = sizeof C, c2 = sizeof C[]) {
    if(a1 == b1 && b1 == c1 && a2 == b2 && b2 == c2) {
        for(a1 = 0; a1 < b1; ++a1) {
            for(a2 = 0; a2 < b2; ++a2) {
                C[a1][a2] = A[a1][a2] - B[a1][a2];
            }
        }
    }
}

stock GetRotFromFlight(Float: pitch, Float: roll, Float: yaw, & Float: rX, & Float: rY, & Float: rZ) {
    new
        Float: cosY = floatcos(roll, degrees),
        Float: sinX = floatsin(pitch, degrees);

    rX = cosY * sinX;

    if(rX > 0.9999) {
        rX = 90.0;
        rY = 0.0;
        rZ = yaw;
    } else if(rX < -0.9999) {
        rX = -90.0;
        rY = 0.0;
        rZ = yaw;
    } else {
        new
            Float: cosX = floatcos(pitch, degrees),
            Float: cosZ = floatcos(yaw, degrees),
            Float: sinY = floatsin(roll, degrees),
            Float: sinZ = floatsin(yaw, degrees);

        rX = asin(rX);
        rY = atan2(sinY, cosY * cosX);
        rZ = atan2(cosX * sinZ - cosZ * sinY * sinX, cosZ * cosX + sinZ * sinY * sinX);
    }
}

stock GetFlightFromRot(Float: rX, Float: rY, Float: rZ, & Float: pitch, & Float: roll, & Float: yaw) {
    new
        Float: cosX = floatcos(rX, degrees),
        Float: sinY = floatsin(rY, degrees);

    roll = cosX * sinY;

    if(roll < -0.9999) {
        pitch = 0.0;
        roll = 90.0;
        yaw = rZ;
    } else if(roll > 0.9999) {
        pitch = 0.0;
        roll = -90.0;
        yaw = rZ;
    } else {
        new
            Float: cosY = floatcos(rY, degrees),
            Float: cosZ = floatcos(rZ, degrees),
            Float: sinX = floatsin(rX, degrees),
            Float: sinZ = floatsin(rZ, degrees);

        roll = asin(roll);
        pitch = atan2(sinX, cosX * cosY);
        yaw = atan2(sinZ * cosY + cosZ * sinX * sinY, cosZ * cosY - sinZ * sinX * sinY);
    }
}

// (w + xi + yj + zk)(ai + bj + ck)(w - xi - yj - zk)

// ((ai + bj + ck)w + (-a + bk -cj)x + (-ak - b + ci)y + (aj - bi -c)z)(w - xi - yj - zk)

// ((-a*x - b*y - c*z) + (w*a - b*z + c*y)i + (a*z + w*b - c*x)j + (-a*y + b*x + w*c)k) * (w - xi - yj - zk)

// ((-a*x*w - b*y*w - c*z*w) + (w^2*a - b*z*w + c*y*w)i + (a*z*w + w^2*b - c*x*w)j + (-a*y*w + b*x*w + w^2*c)k) +
// ((a*x^2 + b*y*x + c*z*x)i + (w*a*x - b*z*x + c*y*x) + (a*z*x + w*b*x - c*x^2)k + (a*y*x - b*x^2 - w*c*x)j) +
// ((a*x*y + b*y^2 + c*z*y)j + (-w*a*y + b*z*y - c*y^2)k + (a*z*y + w*b*y - c*x*y) + (-a*y^2 + b*x*y + w*c*y)i) +
// ((a*x*z + b*y*z + c*z^2)k + (w*a*z - b*z^2 + c*y*z)j + (-a*z^2 - w*b*z + c*x*z)i + (-a*y*z + b*x*z + w*c*z))

// ((-a*x*w - b*y*w - c*z*w) + (w*a*x - b*z*x + c*y*x) + (a*z*y + w*b*y - c*x*y) + (-a*y*z + b*x*z + w*c*z)) +
// ((w^2*a - b*z*w + c*y*w) + (a*x^2 + b*y*x + c*z*x) + (-a*y^2 + b*x*y + w*c*y) + (-a*z^2 - w*b*z + c*x*z))i +
// ((a*z*w + w^2*b - c*x*w) + (a*y*x - b*x^2 - w*c*x) + (a*x*y + b*y^2 + c*z*y) + (w*a*z - b*z^2 + c*y*z))j +
// ((-a*y*w + b*x*w + w^2*c) + (a*z*x + w*b*x - c*x^2) + (-w*a*y + b*z*y - c*y^2) + (a*x*z + b*y*z + c*z^2))k

// (w^2*a + a*x^2 - a*y^2 - a*z^2 + 2*((c*y*w + c*x*z + b*y*x) - b*z*w))i +
// (w^2*b - b*x^2 + b*y^2 - b*z^2 + 2*((a*z*w + a*y*x + c*z*y) - c*x*w))j +
// (w^2*c - c*x^2 - c*y^2 + c*z^2 + 2*((b*x*w + b*z*y + a*x*z) - a*y*w))k

stock QuatMul(Float: w1, Float: x1, Float: y1, Float: z1, Float: w2, Float: x2, Float: y2, Float: z2, & Float: w, & Float: x, & Float: y, & Float: z) {
    // v1v2 = (v1 x v2) - (v1 . v2) | (w1, v1)(w2, v2) = (w1 * w2 - v1 . v2), (w1 * v2) + (w2 * v1) + (v1 x v2)
    w = (w1*w2 - x1*x2 - y1*y2 - z1*z2);
    x = (w1*x2 + w2*x1 + y1*z2 - z1*y2);
    y = (w1*y2 + w2*y1 + z1*x2 - x1*z2);
    z = (w1*z2 + w2*z1 + x1*y2 - y1*x2);
}

stock CrossProduct(Float: x1, Float: y1, Float: z1, Float: x2, Float: y2, Float: z2, & Float: x, & Float: y, & Float: z) {
    x = y1 * z2 - z1 * y2;
    y = z1 * x2 - x1 * z2;
    z = x1 * y2 - y1 * x2;
}

stock QuatRotateVector(Float: w, Float: i, Float: j, Float: k, Float: X, Float: Y, Float: Z, & Float: x, & Float: y, & Float: z) {
    // q * v * q^-1
    // x = (w*w*X + X*i*i - X*j*j - X*k*k + 2.0*((Z*j*w + Z*i*k + Y*j*i) - Y*k*w));
    // y = (w*w*Y - Y*i*i + Y*j*j - Y*k*k + 2.0*((X*k*w + X*j*i + Z*k*j) - Z*i*w));
    // z = (w*w*Z - Z*i*i - Z*j*j + Z*k*k + 2.0*((Y*i*w + Y*k*j + X*i*k) - X*j*w));
    // after a lot of computing you get to the formula "v1 + 2*(w*(v1 x v2) + v1x(v1 x v2));" but only valid with versors (unit quaternion)
    // x = (X + 2.0*(w*(j*Z - k*Y) + j*(i*Y - j*X) - k*(k*X - i*Z)));
    // y = (Y + 2.0*(w*(k*X - i*Z) + k*(j*Z - k*Y) - i*(i*Y - j*X)));
    // z = (Z + 2.0*(w*(i*Y - j*X) + i*(k*X - i*Z) - j*(j*Z - k*Y)));

    // x = (X + 2.0*(w*(j*Z - k*Y) + i*(j*Y + k*Z) - X*(j*j + k*k)));
    // y = (Y + 2.0*(w*(k*X - i*Z) + k*(j*Z - k*Y) - i*(i*Y - j*X)));
    // z = (Z + 2.0*(w*(i*Y - j*X) + i*(k*X - i*Z) - j*(j*Z - k*Y)));

    // 18 muls, 12 adds
    // CrossProduct(i, j, k, X, Y, Z, x, y, z);
    // CrossProduct(i, j, k, x, y, z, i, j, k);
    // x = (X + 2.0 * (w * x + i));
    // y = (Y + 2.0 * (w * y + j));
    // z = (Z + 2.0 * (w * z + k));

    // 15 muls, 15 adds
    CrossProduct(i, j, k, X, Y, Z, x, y, z);
    CrossProduct(
        i + i,
        j + j,
        k + k,
        x + w * X,
        y + w * Y,
        z + w * Z,
        x, y, z
    );
    x += X;
    y += Y;
    z += Z;
}

stock GetVehicleRelativePos(vehicleid, & Float: x, & Float: y, & Float: z, Float: oX = 0.0, Float: oY = 0.0, Float: oZ = 0.0) {
    if(GetVehiclePos(vehicleid, x, y, z) && ((_: oX != 0) || (_: oY != 0) || (_: oZ != 0))) {
        new
            Float: qW,
            Float: qX,
            Float: qY,
            Float: qZ
        ;
        GetVehicleRotationQuat(vehicleid, qW, qX, qY, qZ);
        QuatRotateVector(qW, -qX, -qY, -qZ, oX, oY, oZ, qX, qY, qZ);

        x += qX;
        y += qY;
        z += qZ;
        return true;
    }
    return false;
}

stock QuatForwardVector(Float: w, Float: i, Float: j, Float: k, & Float: x, & Float: y, & Float: z) {
    x = 2.0 * (j * i - k * w);
    y = 1.0 - 2.0 * (k * k + i * i);
    z = 2.0 * (i * w + j * k);
}

stock GetVehicleForwardVector(vehicleid, & Float: x, & Float: y, & Float: z) {
    if(GetVehicleRotationQuat(vehicleid, Float: vehicleid, x, y, z)) {
        QuatForwardVector(Float: vehicleid, -x, -y, -z, x, y, z);
        return true;
    }
    return false;
}

stock QuatRightVector(Float: w, Float: i, Float: j, Float: k, & Float: x, & Float: y, & Float: z) {
    x = 1.0 - 2.0 * (j * j + k * k);
    y = 2.0 * (k * w + i * j);
    z = 2.0 * (i * k - j * w);
}

stock GetVehicleRightVector(vehicleid, & Float: x, & Float: y, & Float: z) {
    if(GetVehicleRotationQuat(vehicleid, Float: vehicleid, x, y, z)) {
        QuatRightVector(Float: vehicleid, -x, -y, -z, x, y, z);
        return true;
    }
    return false;
}

stock QuatUpVector(Float: w, Float: i, Float: j, Float: k, & Float: x, & Float: y, & Float: z) {
    x = 2.0 * (j * w + k * i);
    y = 2.0 * (k * j - i * w);
    z = 1.0 - 2.0 * (i * i + j * j);
}

stock GetVehicleUpVector(vehicleid, & Float: x, & Float: y, & Float: z) {
    if(GetVehicleRotationQuat(vehicleid, Float: vehicleid, x, y, z)) {
        QuatUpVector(Float: vehicleid, -x, -y, -z, x, y, z);
        return true;
    }
    return false;
}

stock EulerVectorToQuat(Float: X, Float: Y, Float: Z, Float: angle, & Float: w, & Float: i, & Float: j, & Float: k, anglemode: mode = degrees) {
    if((w = VectorSize(X, Y, Z)) != 1.0) {
        X /= w;
        Y /= w;
        Z /= w;
    }
    angle /= 2.0;

    w = floatsin(angle, mode);
    i = X * w;
    j = Y * w;
    k = Z * w;
    w = floatcos(angle, mode);
}

// stock GetEulerFromEuler(Float: oX, Float: oY, Float: oZ, eulermode: omode, & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp) {
// //	new Float: matrix[3][3]; // 

// //	GetRotationMatrixFromEuler(matrix, oX, oY, oZ, omode);
// //	GetEulerFromMatrix(matrix, rX, rY, rZ, mode);

    // GetQuatFromEuler(oX, oY, oZ, rX, oX, oY, oZ, omode);
    // GetEulerFromQuat(rX, oX, oY, oZ, rX, rY, rZ, mode);

    // #emit retn // tried to unravel these but to much work

    // new
        // Float: cosX = floatcos(oX, degrees),
        // Float: sinY = floatsin(oY, degrees),
        // Float: cosY = floatcos(oY, degrees),
        // Float: cosZ = floatcos(oZ, degrees),
        // Float: sinX = floatsin(oX, degrees),
        // Float: sinZ = floatsin(oZ, degrees);

    // switch(omode) {
// /*
            // matrix[1][1] = cosX * cosY;
            // matrix[1][2] = -sinX;
            // matrix[2][1] = cosY * sinX;
            // matrix[2][2] = cosX;
// */
        // case euler_xzx: {
            // switch(mode) {
                // case euler_xzx: {
                    // rX = oX;
                    // rY = oZ;
                    // rZ = oZ;
                // }
                // case euler_xyx: {
                    // rX = oX + 90.0;
                    // rY = oY;
                    // rZ = oZ - 90.0;
                // }
                // case euler_yxy: {
                    // rY = cosX * cosY * cosZ - sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = atan2(sinY * sinZ, cosY);
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = -atan2(sinY * sinZ, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(-cosZ * sinY, cosX * sinZ + cosY * cosZ * sinX);
                        // rY = acos(rY);
                        // rZ = atan2(cosX * sinY, cosZ * sinX + cosX * cosY * sinZ);
                    // }
                // }
                // case euler_yzy: {
                    // rY = cosX * cosY * cosZ - sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = atan2(sinY * sinZ, cosY);
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = atan2(sinY * sinZ, -cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(cosX * sinZ + cosY * cosZ * sinX, cosZ * sinY);
                        // rY = acos(rY);
                        // rZ = -atan2(cosZ * sinX + cosX * cosY * sinZ, cosX * sinY);
                    // }
                // }
                // case euler_zyz: {
                    // rY = cosX * cosZ - cosY * sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = atan2(cosX * sinY, cosY);
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = atan2(cosX * sinY, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = -atan2(cosZ * sinX + cosX * cosY * sinZ, sinY * sinZ);
                        // rY = acos(rY);
                        // rZ = atan2(cosX * sinZ + cosY * cosZ * sinX, -sinX * sinY);
                    // }
                // }
                // case euler_zxz: {
                    // rY = cosX * cosZ - cosY * sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = atan2(cosX * sinY, cosY);
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = atan2(cosX * sinY, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(sinY * sinZ, cosZ * sinX + cosX * cosY * sinZ);
                        // rY = acos(rY);
                        // rZ = atan2(sinX * sinY, cosX * sinZ + cosY * cosZ * sinX);
                    // }
                // }
                // case euler_xzy: {
                    // rZ = cosZ * sinY;

                    // if(rZ > 0.9999) {
                        // rX = oX;
                        // rY = 0.0;
                        // rZ = 90.0;
                    // } else if(rZ < -0.9999) {
                        // rX = oX;
                        // rY = 0.0;
                        // rZ = -90.0;
                    // } else {
                        // rX = atan2(cosX * sinZ + cosY * cosZ * sinX, cosX * cosY * cosZ - sinX * sinZ);
                        // rY = atan2(sinY * sinZ, cosY);
                        // rZ = asin(rZ);
                    // }
                // }
                // case euler_xyz: {
                    // rY = sinY * sinZ;

                    // if(rY > 0.9999) {
                        // rX = 90.0 + oX;
                        // rY = 90.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = 90.0 + oX;
                        // rY = -90.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(cosZ * sinX + cosX * cosY * sinZ, cosX * cosZ - cosY * sinX * sinZ);
                        // rY = asin(rY);
                        // rZ = atan2(cosZ * sinY, cosY);
                    // }
                // }
                // case euler_yxz: {
                    // rX = cosZ * sinX + cosX * cosY * sinZ;

                    // if(rX > 0.9999) {
                        // rX = 90.0;
                        // rY = -atan2(cosZ * sinY, cosY);
                        // rZ = 0.0;
                    // } else if(rX < -0.9999) {
                        // rX = -90.0;
                        // rY = atan2(cosZ * sinY, cosY);
                        // rZ = 0.0;
                    // } else {
                        // rX = asin(rX);
                        // rY = atan2(sinY * sinZ, cosX * cosZ - cosY * sinX * sinZ);
                        // rZ = atan2(cosX * sinY, cosX * cosY * cosZ - sinX * sinZ);
                    // }
                // }
                // case euler_yzx: {
                    // rZ = cosX * sinY;

                    // if(rZ > 0.9999) {
                        // rX = 0.0;
                        // rY = oZ;
                        // rZ = 90.0;
                    // } else if(rZ < -0.9999) {
                        // rX = 0.0;
                        // rY = 180.0 - oZ;
                        // rZ = -90.0;
                    // } else {
                        // rX = atan2(cosZ * sinX + cosX * cosY * sinZ, cosX * cosY * cosZ - sinX * sinZ);
                        // rY = -atan2(sinX * sinY, cosY);
                        // rZ = asin(rZ);
                    // }
                // }
                // case euler_zyx: {
                    // rY = sinX * sinY;

                    // if(rY < -0.9999) {
                        // rX = 0.0;
                        // rY = 90.0;
                        // rZ = 90.0 - oZ;
                    // } else if(rY > 0.9999) {
                        // rX = 0.0;
                        // rY = -90.0;
                        // rZ = oZ - 90.0;
                    // } else {
                        // rX = atan2(cosX * sinZ + cosY * cosZ * sinX, cosX * cosZ - cosY * sinX * sinZ);
                        // rY = -asin(rY);
                        // rZ = atan2(cosX * sinY, cosY);
                    // }
                // }
                // case euler_zxy: {
                    // rX = cosX * sinZ + cosY * cosZ * sinX;

                    // if(rX > 0.9999) {
                        // rX = 90.0;
                        // rY = 0.0;
                        // rZ = atan2(cosX * sinY, cosY);
                    // } else if(rX < -0.9999) {
                        // rX = -90.0;
                        // rY = 0.0;
                        // rZ = atan2(cosX * sinY, cosY);
                    // } else {
                        // rX = asin(rX);
                        // rY = -atan2(sinX * sinY, cosX * cosZ - cosY * sinX * sinZ);
                        // rZ = atan2(cosZ * sinY, cosX * cosY * cosZ - sinX * sinZ);
                    // }
                // }
            // }
        // }
// /*
            // matrix[1][1] = cosX;
            // matrix[1][2] = - cosY * sinX;
            // matrix[2][1] = sinX;
            // matrix[2][2] = cosX * cosY;
// */
        // case euler_xyx: {
            // switch(mode) {
                // case euler_xzx: {
                    // rX = oX - 90.0;
                    // rY = oY;
                    // rZ = oZ + 90.0;
                // }
                // case euler_xyx: {
                    // rX = oX;
                    // rY = oY;
                    // rZ = oZ;
                // }
                // case euler_yxy: {
                    // rY = cosX * cosZ - cosY * sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = atan2(cosX * sinY, cosY); // -20, 00
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = atan2(cosX * sinY, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(sinY * sinZ, cosZ * sinX + cosX * cosY * sinZ);
                        // rY = acos(rY);
                        // rZ = atan2(sinX * sinY, cosX * sinZ + cosY * cosZ * sinX);
                    // }
                // }
                // case euler_yzy: {
                    // rY = cosX * cosZ - cosY * sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = atan2(cosX * sinY, cosY);  // -20, 00
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = atan2(cosX * sinY, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(cosZ * sinX + cosX * cosY * sinZ, -sinY * sinZ);
                        // rY = acos(rY);
                        // rZ = -atan2(cosX * sinZ + cosY * cosZ * sinX, sinX * sinY);
                    // }
                // }
                // case euler_zyz: {
                    // rY = cosX * cosY * cosZ - sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = atan2(sinX * sinY, cosY); // -01, 11
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = atan2(sinX * sinY, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = -atan2(cosX * sinZ + cosY * cosZ * sinX, cosZ * sinY);
                        // rY = acos(rY);
                        // rZ = atan2(cosZ * sinX + cosX * cosY * sinZ, cosX * sinY);
                    // }
                // }
                // case euler_zxz: {
                    // rY = cosX * cosY * cosZ - sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = atan2(sinX * sinY, cosY); // -01, 11
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = atan2(sinX * sinY, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(cosZ * sinY, cosX * sinZ + cosY * cosZ * sinX);
                        // rY = acos(rY);
                        // rZ = -atan2(cosX * sinY, cosZ * sinX + cosX * cosY * sinZ);
                    // }
                // }
                // case euler_xzy: {
                    // rZ = sinY * sinZ;

                    // if(rZ < -0.9999) {
                        // rX = oX - 90.0; // -12, 22
                        // rY = 0.0;
                        // rZ = 90.0;
                    // } else if(rZ > 0.9999) {
                        // rX = oX - 90.0;
                        // rY = 0.0;
                        // rZ = -90.0;
                    // } else {
                        // rX = atan2(cosZ * sinX + cosX * cosY * sinZ, cosX * cosZ - cosY * sinX * sinZ);
                        // rY = atan2(cosZ * sinY, cosY);
                        // rZ = -asin(rZ);
                    // }
                // }
                // case euler_xyz: {
                    // rY = cosZ * sinY;

                    // if(rY > 0.9999) {
                        // rX = oX; // 10, -20
                        // rY = 90.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = oX;
                        // rY = -90.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(cosX * sinZ + cosY * cosZ * sinX, cosX * cosY * cosZ - sinX * sinZ);
                        // rY = asin(rY);
                        // rZ = -atan2(sinY * sinZ, cosY);
                    // }
                // }
                // case euler_yxz: {
                    // rX = cosX * sinZ + cosY * cosZ * sinX;

                    // if(rX > 0.9999) {
                        // rX = 90.0;
                        // rY = atan2(cosX * sinY, cosY); // -20, 00
                        // rZ = 0.0;
                    // } else if(rX < -0.9999) {
                        // rX = -90.0;
                        // rY = atan2(cosX * sinY, cosY);
                        // rZ = 0.0;
                    // } else {
                        // rX = asin(rX);
                        // rY = atan2(cosZ * sinY, cosX * cosY * cosZ - sinX * sinZ);
                        // rZ = atan2(sinX * sinY, cosX * cosZ - cosY * sinX * sinZ);
                    // }
                // }
                // case euler_yzx: {
                    // rZ = sinX * sinY;

                    // if(rZ > 0.9999) {
                        // rX = 0.0;
                        // rY = 90.0 + oZ; // 21, -01
                        // rZ = 90.0;
                    // } else if(rZ < -0.9999) {
                        // rX = 0.0;
                        // rY = 90.0 - oZ;
                        // rZ = -90.0;
                    // } else {
                        // rX = atan2(cosX * sinZ + cosY * cosZ * sinX, cosX * cosZ - cosY * sinX * sinZ);
                        // rY = atan2(cosX * sinY, cosY);
                        // rZ = asin(rZ);
                    // }
                // }
                // case euler_zyx: { // pitch roll yaw
                    // rY = cosX * sinY;

                    // if(rY > 0.9999) {
                        // rX = 0.0;
                        // rY = 90.0;
                        // rZ = -oZ; // -01, 11
                    // } else if(rY < -0.9999) {
                        // rX = 0.0;
                        // rY = -90.0;
                        // rZ = oZ;
                    // } else {
                        // rX = atan2(cosZ * sinX + cosX * cosY * sinZ, cosX * cosY * cosZ - sinX * sinZ);
                        // rY = asin(rY);
                        // rZ = atan2(sinX * sinY, cosY);
                    // }
                // }
                // case euler_zxy: { // sa-mp
                    // rX = cosZ * sinX + cosX * cosY * sinZ;

                    // if(rX > 0.9999) {
                        // rX = 90.0;
                        // rY = 0.0;
                        // rZ = atan2(sinX * sinY, cosY); // 02, -12
                    // } else if(rX < -0.9999) {
                        // rX = -90.0;
                        // rY = 0.0;
                        // rZ = atan2(sinX * sinY, cosY);
                    // } else {
                        // rX = asin(rX);
                        // rY = atan2(cosX * sinY, cosX * cosY * cosZ - sinX * sinZ);
                        // rZ = -atan2(sinY * sinZ, cosX * cosZ - cosY * sinX * sinZ);
                    // }
                // }
            // }
        // }
// /*
            // matrix[0][0] = cosX;
            // matrix[0][2] = cosY * sinX;
            // matrix[2][0] = -sinX;
            // matrix[2][2] = cosX * cosY;
// */
        // case euler_yxy: {
            // switch(mode) {
                // case euler_xzx: {
                    // rY = cosX * cosZ - cosY * sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = atan2(cosX * sinY, cosY); // -12, 22
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = atan2(cosX * sinY, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = -atan2(cosZ * sinX + cosX * cosY * sinZ, sinY * sinZ);
                        // rY = acos(rY);
                        // rZ = atan2(cosX * sinZ + cosY * cosZ * sinX, -sinX * sinY);
                    // }
                // }
                // case euler_xyx: {
                    // rY = cosX * cosZ - cosY * sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = atan2(cosX * sinY, cosY); // -12, 22
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = atan2(cosX * sinY, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(sinY * sinZ, cosZ * sinX + cosX * cosY * sinZ);
                        // rY = acos(rY);
                        // rZ = atan2(sinX * sinY, cosX * sinZ + cosY * cosZ * sinX);
                    // }
                // }
                // case euler_yxy: {
                    // rX = oX;
                    // rY = oY;
                    // rZ = oZ;
                // }
                // case euler_yzy: {
                    // rX = oX + 90.0;
                    // rY = oY;
                    // rZ = oZ - 90.0;
                // }
                // case euler_zyz: {
                    // rY = cosX * cosY * cosZ - sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = -atan2(sinX * sinY, cosY); // -01, 11
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = -atan2(sinX * sinY, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = -atan2(cosZ * sinY, cosX * sinZ + cosY * cosZ * sinX);
                        // rY = acos(rY);
                        // rZ = atan2(cosX * sinY, cosZ * sinX + cosX * cosY * sinZ);
                    // }
                // }
                // case euler_zxz: {
                    // rY = cosX * cosY * cosZ - sinX * sinZ;

                    // if(rY > 0.9999) {
                        // rX = -atan2(sinX * sinY, cosY); // -01, 11
                        // rY = 0.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = -atan2(sinX * sinY, cosY);
                        // rY = 180.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(cosX * sinZ + cosY * cosZ * sinX, cosZ * sinY);
                        // rY = acos(rY);
                        // rZ = -atan2(cosZ * sinX + cosX * cosY * sinZ, cosX * sinY);
                    // }
                // }
                // case euler_xzy: {
                    // rZ = sinX * sinY;

                    // if(rZ < -0.9999) {
                        // rX = 90.0 - oZ; // -12, 22
                        // rY = 0.0;
                        // rZ = 90.0;
                    // } else if(rZ > 0.9999) {
                        // rX = 90.0 + oZ;
                        // rY = 0.0;
                        // rZ = -90.0;
                    // } else {
                        // rX = atan2(cosX * sinY, cosY);
                        // rY = atan2(cosX * sinZ + cosY * cosZ * sinX, cosX * cosZ - cosY * sinX * sinZ);
                        // rZ = -asin(rZ);
                    // }
                // }
                // case euler_xyz: {
                    // rY = cosX * sinZ + cosY * cosZ * sinX;

                    // if(rY > 0.9999) {
                        // rX = atan2(cosX * sinY, cosY); // 10, -20
                        // rY = 90.0;
                        // rZ = 0.0;
                    // } else if(rY < -0.9999) {
                        // rX = atan2(cosX * sinY, cosY);
                        // rY = -90.0;
                        // rZ = 0.0;
                    // } else {
                        // rX = atan2(cosZ * sinY, cosX * cosY * cosZ - sinX * sinZ);
                        // rY = asin(rY);
                        // rZ = -atan2(sinX * sinY, cosX * cosZ - cosY * sinX * sinZ);
                    // }
                // }
                // case euler_yxz: {
                    // rX = cosZ * sinY;

                    // if(rX > 0.9999) {
                        // rX = 90.0;
                        // rY = oX; // -20, 00
                        // rZ = 0.0;
                    // } else if(rX < -0.9999) {
                        // rX = -90.0;
                        // rY = oX;
                        // rZ = 0.0;
                    // } else {
                        // rX = asin(rX);
                        // rY = atan2(cosX * sinZ + cosY * cosZ * sinX, cosX * cosY * cosZ - sinX * sinZ);
                        // rZ = atan2(sinY * sinZ, cosY);
                    // }
                // }
                // case euler_yzx: {
                    // rZ = sinY * sinZ;

                    // if(rZ > 0.9999) {
                        // rX = 0.0;
                        // rY = 90.0 + oX; // 21, -01
                        // rZ = 90.0;
                    // } else if(rZ < -0.9999) {
                        // rX = 0.0;
                        // rY = 90.0 + oX;
                        // rZ = -90.0;
                    // } else {
                        // rX = atan2(cosZ * sinY, cosY);
                        // rY = atan2(cosZ * sinX + cosX * cosY * sinZ, cosX * cosZ - cosY * sinX * sinZ);
                        // rZ = asin(rZ);
                    // }
                // }
                // case euler_zyx: { // pitch roll yaw
                    // rY = cosZ * sinX + cosX * cosY * sinZ;

                    // if(rY > 0.9999) {
                        // rX = 0.0;
                        // rY = 90.0;
                        // rZ = -atan2(sinX * sinY, cosY); // -01, 11
                    // } else if(rY < -0.9999) {
                        // rX = 0.0;
                        // rY = -90.0;
                        // rZ = -atan2(sinX * sinY, cosY);
                    // } else {
                        // rX = atan2(cosX * sinY, cosX * cosY * cosZ - sinX * sinZ);
                        // rY = asin(rY);
                        // rZ = atan2(sinY * sinZ, cosX * cosZ - cosY * sinX * sinZ);
                    // }
                // }
                // case euler_zxy: { // sa-mp
                    // rX = cosX * sinY;

                    // if(rX > 0.9999) {
                        // rX = 90.0;
                        // rY = 0.0;
                        // rZ = oZ; // 02, -12
                    // } else if(rX < -0.9999) {
                        // rX = -90.0;
                        // rY = 0.0;
                        // rZ = 180.0 - oZ;
                    // } else {
                        // rX = asin(rX);
                        // rY = atan2(cosZ * sinX + cosX * cosY * sinZ, cosX * cosY * cosZ - sinX * sinZ);
                        // rZ = -atan2(sinX * sinY, cosY);
                    // }
                // }
            // }
        // }
// /*
            // matrix[0][0] = cosX * cosY * cosZ - sinX * sinZ;
            // matrix[0][1] = -cosX * sinY;
            // matrix[0][2] = cosZ * sinX + cosX * cosY * sinZ;
            // matrix[1][0] = cosZ * sinY;
            // matrix[1][1] = cosY;
            // matrix[1][2] = sinY * sinZ;
            // matrix[2][0] = -cosX * sinZ - cosY * cosZ * sinX;
            // matrix[2][1] = sinX * sinY;
            // matrix[2][2] = cosX * cosZ - cosY * sinX * sinZ;
// rZ = 0 rY = 0 / 180
            // matrix[0][0] = cosX * cosY;
            // matrix[0][2] = sinX;
            // matrix[2][0] = - cosY * sinX;
            // matrix[2][2] = cosX;
// */
        // case euler_yzy: {
            // switch(mode) {
                // case euler_xzx: {}
                // case euler_xyx: {}
                // case euler_yxy: {}
                // case euler_yzy: {
                    // rX = oX;
                    // rY = oZ;
                    // rZ = oZ;
                // }
                // case euler_zyz: {}
                // case euler_zxz: {}
                // case euler_xzy: {}
                // case euler_xyz: {}
                // case euler_yxz: {}
                // case euler_yzx: {}
                // case euler_zyx: {}
                // case euler_zxy: {}
            // }
        // }
// /*
            // matrix[0][0] = cosX * cosY * cosZ - sinX * sinZ;
            // matrix[0][1] = -cosZ * sinX - cosX * cosY * sinZ;
            // matrix[0][2] = cosX * sinY;
            // matrix[1][0] = cosX * sinZ + cosY * cosZ * sinX;
            // matrix[1][1] = cosX * cosZ - cosY * sinX * sinZ;
            // matrix[1][2] = sinX * sinY;
            // matrix[2][0] = -cosZ * sinY;
            // matrix[2][1] = sinY * sinZ;
            // matrix[2][2] = cosY;
// rZ = 0 rY = 0 / 180
            // matrix[0][0] = cosX * cosY;
            // matrix[0][1] = -sinX;
            // matrix[1][0] = cosY * sinX;
            // matrix[1][1] = cosX;
// */
        // case euler_zyz: {
            // switch(mode) {
                // case euler_xzx: {}
                // case euler_xyx: {}
                // case euler_yxy: {}
                // case euler_yzy: {}
                // case euler_zyz: {
                    // rX = oX;
                    // rY = oZ;
                    // rZ = oZ;
                // }
                // case euler_zxz: {}
                // case euler_xzy: {}
                // case euler_xyz: {}
                // case euler_yxz: {}
                // case euler_yzx: {}
                // case euler_zyx: {}
                // case euler_zxy: {}
            // }
        // }
// /*
            // matrix[0][0] = cosX * cosZ - cosY * sinX * sinZ;
            // matrix[0][1] = -cosX * sinZ - cosY * cosZ * sinX;
            // matrix[0][2] = sinX * sinY;
            // matrix[1][0] = cosZ * sinX + cosX * cosY * sinZ;
            // matrix[1][1] = cosX * cosY * cosZ - sinX * sinZ;
            // matrix[1][2] = -cosX * sinY;
            // matrix[2][0] = sinY * sinZ;
            // matrix[2][1] = cosZ * sinY;
            // matrix[2][2] = cosY;
// rZ = 0.0 rY = 0 / 180
            // matrix[0][0] = cosX;
            // matrix[0][1] = -cosY * sinX;
            // matrix[1][0] = sinX;
            // matrix[1][1] = cosX * cosY;
// */
        // case euler_zxz: {
            // switch(mode) {
                // case euler_xzx: {}
                // case euler_xyx: {}
                // case euler_yxy: {}
                // case euler_yzy: {}
                // case euler_zyz: {}
                // case euler_zxz: {
                    // rX = oX;
                    // rY = oZ;
                    // rZ = oZ;
                // }
                // case euler_xzy: {}
                // case euler_xyz: {}
                // case euler_yxz: {}
                // case euler_yzx: {}
                // case euler_zyx: {}
                // case euler_zxy: {}
            // }
        // }
// /*
            // matrix[0][0] = cosZ * cosY;
            // matrix[0][1] = -sinZ;
            // matrix[0][2] = cosZ * sinY;
            // matrix[1][0] = sinX * sinY + cosX * cosY * sinZ;
            // matrix[1][1] = cosX * cosZ;
            // matrix[1][2] = cosX * sinZ * sinY - cosY * sinX;
            // matrix[2][0] = cosY * sinX * sinZ - cosX * sinY;
            // matrix[2][1] = cosZ * sinX;
            // matrix[2][2] = cosX * cosY + sinX * sinZ * sinY;
// rY = 0 rZ = 90 / -90
            // matrix[1][0] = cosX * sinZ;
            // matrix[1][2] = -sinX;
            // matrix[2][0] = sinX * sinZ;
            // matrix[2][2] = cosX;
// */
        // case euler_xzy: {
            // switch(mode) {
                // case euler_xzx: {}
                // case euler_xyx: {}
                // case euler_yxy: {}
                // case euler_yzy: {}
                // case euler_zyz: {}
                // case euler_zxz: {}
                // case euler_xzy: {
                    // rX = oX;
                    // rY = oZ;
                    // rZ = oZ;
                // }
                // case euler_xyz: {}
                // case euler_yxz: {}
                // case euler_yzx: {}
                // case euler_zyx: {}
                // case euler_zxy: {}
            // }
        // }
// /*
            // matrix[0][0] = cosY * cosZ;
            // matrix[0][1] = -cosY * sinZ;
            // matrix[0][2] = sinY;
            // matrix[1][0] = cosX * sinZ + cosZ * sinX * sinY;
            // matrix[1][1] = cosX * cosZ - sinX * sinY * sinZ;
            // matrix[1][2] = -cosY * sinX;
            // matrix[2][0] = sinX * sinZ - cosX * cosZ * sinY;
            // matrix[2][1] = cosZ * sinX + cosX * sinY * sinZ;
            // matrix[2][2] = cosX * cosY;
// rZ = 0 rY = 90 / -90
            // matrix[1][0] = sinX * sinY;
            // matrix[1][1] = cosX;
            // matrix[2][0] = -cosX * sinY;
            // matrix[2][1] = sinX;
// */
        // case euler_xyz: {
            // switch(mode) {
                // case euler_xzx: {}
                // case euler_xyx: {}
                // case euler_yxy: {}
                // case euler_yzy: {}
                // case euler_zyz: {}
                // case euler_zxz: {}
                // case euler_xzy: {}
                // case euler_xyz: {
                    // rX = oX;
                    // rY = oZ;
                    // rZ = oZ;
                // }
                // case euler_yxz: {}
                // case euler_yzx: {}
                // case euler_zyx: {}
                // case euler_zxy: {}
            // }
        // }
// /*
            // matrix[0][0] = cosY * cosZ + sinY * sinX * sinZ;
            // matrix[0][1] = cosZ * sinY * sinX - cosY * sinZ;
            // matrix[0][2] = cosX * sinY;
            // matrix[1][0] = cosX * sinZ;
            // matrix[1][1] = cosX * cosZ;
            // matrix[1][2] = -sinX;
            // matrix[2][0] = cosY * sinX * sinZ - cosZ * sinY;
            // matrix[2][1] = cosY * cosZ * sinX + sinY * sinZ;
            // matrix[2][2] = cosY * cosX;
// rZ = 0 rX = 90 / -90
            // matrix[0][0] = cosY;
            // matrix[0][1] = sinY * sinX;
            // matrix[2][0] = -sinY;
            // matrix[2][1] = cosY * sinX;
// */
        // case euler_yxz: {
            // switch(mode) {
                // case euler_xzx: {}
                // case euler_xyx: {}
                // case euler_yxy: {}
                // case euler_yzy: {}
                // case euler_zyz: {}
                // case euler_zxz: {}
                // case euler_xzy: {}
                // case euler_xyz: {}
                // case euler_yxz: {
                    // rX = oX;
                    // rY = oZ;
                    // rZ = oZ;
                // }
                // case euler_yzx: {}
                // case euler_zyx: {}
                // case euler_zxy: {}
            // }
        // }
// /*
            // matrix[0][0] = cosY * cosZ;
            // matrix[0][1] = sinY * sinX - cosY * cosX * sinZ;
            // matrix[0][2] = cosX * sinY + cosY * sinZ * sinX;
            // matrix[1][0] = sinZ;
            // matrix[1][1] = cosZ * cosX;
            // matrix[1][2] = -cosZ * sinX;
            // matrix[2][0] = -cosZ * sinY;
            // matrix[2][1] = cosY * sinX + cosX * sinY * sinZ;
            // matrix[2][2] = cosY * cosX - sinY * sinZ * sinX;
// rX = 0 rZ = 90 / -90
            // matrix[0][1] = -cosY * sinZ;
            // matrix[0][2] = sinY;
            // matrix[2][1] = sinY * sinZ;
            // matrix[2][2] = cosY;
// */
        // case euler_yzx: {
            // switch(mode) {
                // case euler_xzx: {}
                // case euler_xyx: {}
                // case euler_yxy: {}
                // case euler_yzy: {}
                // case euler_zyz: {}
                // case euler_zxz: {}
                // case euler_xzy: {}
                // case euler_xyz: {}
                // case euler_yxz: {}
                // case euler_yzx: {
                    // rX = oX;
                    // rY = oZ;
                    // rZ = oZ;
                // }
                // case euler_zyx: {}
                // case euler_zxy: {}
            // }
        // }
// /*
            // matrix[0][0] = cosZ * cosY;
            // matrix[0][1] = cosZ * sinY * sinX - cosX * sinZ;
            // matrix[0][2] = sinZ * sinX + cosZ * cosX * sinY;
            // matrix[1][0] = cosY * sinZ;
            // matrix[1][1] = cosZ * cosX + sinZ * sinY * sinX;
            // matrix[1][2] = cosX * sinZ * sinY - cosZ * sinX;
            // matrix[2][0] = -sinY;
            // matrix[2][1] = cosY * sinX;
            // matrix[2][2] = cosY * cosX;
// rX = 0 rY = 90 / -90
            // matrix[0][1] = -sinZ;
            // matrix[0][2] = cosZ * sinY;
            // matrix[1][1] = cosZ;
            // matrix[1][2] = sinZ * sinY;
// */
        // case euler_zyx: { // pitch roll yaw
            // switch(mode) {
                // case euler_xzx: {}
                // case euler_xyx: {}
                // case euler_yxy: {}
                // case euler_yzy: {}
                // case euler_zyz: {}
                // case euler_zxz: {}
                // case euler_xzy: {}
                // case euler_xyz: {}
                // case euler_yxz: {}
                // case euler_yzx: {}
                // case euler_zyx: { // pitch roll yaw
                    // rX = oX;
                    // rY = oZ;
                    // rZ = oZ;
                // }
                // case euler_zxy: { // sa-mp
                    // rX = cosY * sinX;

                    // if(rX > 0.9999) {
                        // rX = 90.0;
                        // rY = 0.0;
                        // rZ = oZ;
                    // } else if(rX < -0.9999) {
                        // rX = -90.0;
                        // rY = 0.0;
                        // rZ = oZ;
                    // } else {
                        // rX = asin(rX);
                        // rY = atan2(sinY, cosY * cosX);
                        // rZ = atan2(cosX * sinZ - cosZ * sinY * sinX, cosZ * cosX + sinZ * sinY * sinX);
                    // }
                // }
            // }
        // }
// /*
            // matrix[0][0] = cosZ * cosY - sinZ * sinX * sinY;
            // matrix[0][1] = -sinZ * cosX;
            // matrix[0][2] = cosZ * sinY + sinZ * sinX * cosY;
            // matrix[1][0] = sinZ * cosY + cosZ * sinX * sinY;
            // matrix[1][1] = cosZ * cosX;
            // matrix[1][2] = sinZ * sinY - cosZ * sinX * cosY;
            // matrix[2][0] = -cosX * sinY;
            // matrix[2][1] = sinX;
            // matrix[2][2] = cosX * cosY;
// rY = 0 rX = 90 / -90
            // matrix[0][0] = cosZ;
            // matrix[0][2] = sinZ * sinX;
            // matrix[1][0] = sinZ ;
            // matrix[1][2] = -cosZ * sinX;
// */
        // case euler_zxy: { // sa-mp
            // switch(mode) {
                // case euler_xzx: {}
                // case euler_xyx: {}
                // case euler_yxy: {}
                // case euler_yzy: {}
                // case euler_zyz: {}
                // case euler_zxz: {}
                // case euler_xzy: {}
                // case euler_xyz: {}
                // case euler_yxz: {}
                // case euler_yzx: {}
                // case euler_zyx: { // pitch roll yaw
                    // rY = cosX * sinY;

                    // if(rY < -0.9999) {
                        // rX = 0.0;
                        // rY = 90.0;
                        // rZ = oZ;
                    // } else if(rY > 0.9999) {
                        // rX = 0.0;
                        // rY = -90.0;
                        // rZ = oZ;
                    // } else {
                        // rX = atan2(sinX, cosX * cosY);
                        // rY = asin(rY);
                        // rZ = atan2(sinZ * cosY + cosZ * sinX * sinY, cosZ * cosY - sinZ * sinX * sinY);
                    // }
                // }
                // case euler_zxy: { // sa-mp
                    // rX = oX;
                    // rY = oZ;
                    // rZ = oZ;
                // }
            // }
        // }
    // }
// }