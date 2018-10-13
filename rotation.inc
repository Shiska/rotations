#if defined _inc_rotation
	#undef _inc_rotation
#endif

#if defined _rotation_included
	#endinput
#endif
#define _rotation_included

/*
* rotation by Nero_3D (C) 2016
* 
*/

#include <a_samp>

// Reference
// https://en.wikipedia.org/wiki/Euler_Angles#Rotation_matrix
// https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
// https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation
// http://www.euclideanspace.com/maths/geometry/rotations/index.htm
// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

// Right to left - absolute Rotation (active)
// Left to right - relative Rotation (passive)

enum eulermode {
    // Proper / Classic Euler angles
    euler_xzx,
    euler_xyx,
    euler_yxy,
    euler_yzy,
    euler_zyz,
    euler_zxz,
    // Tait-Bryan angles
    euler_xzy,
    euler_xyz,
    euler_yxz,
    euler_yzx,
    euler_zyx,
    euler_zxy // sa-mp // yaw pitch roll
}

const eulermode: euler_samp = euler_zxy;

/*
native
native RotMatrixMatrixAroundRelPoint(Float: matrix1[4][4], Float: oX, Float: oY, Float: oZ, Float: matrix2[4][4])
native RotMatrixQuatAroundRelPoint(Float: matrix[4][4], Float: oX, Float: oY, Float: oZ, Float: w, Float: x, Float: y, Float: z)
native RotMatrixEulerAroundRelPoint(Float: matrix[4][4], Float: oX, Float: oY, Float: oZ, Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp)
native RotMatrixAxisAroundRelPoint(Float: matrix1[4][4], Float: oX, Float: oY, Float: oZ, Float: angle, Float: aX, Float: aY, Float: aZ)
native RotMatrixMatrixAroundPoint(Float: matrix1[4][4], Float: x, Float: y, Float: z, Float: matrix2[4][4])
native RotMatrixQuatAroundPoint(Float: matrix[4][4], Float: x, Float: y, Float: z, Float: w, Float: qX, Float: qY, Float: qZ)
native RotMatrixEulerAroundPoint(Float: matrix[4][4], Float: x, Float: y, Float: z, Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp)
native RotMatrixAxisAroundPoint(Float: matrix1[4][4], Float: x, Float: y, Float: z, Float: angle, Float: aX, Float: aY, Float: aZ)
native TranslateMatrix(Float: matrix[4][4], Float: x, Float: y, Float: z)
native RotateMatrixWithMatrix(Float: matrix1[4][4], Float: matrix2[4][4])
native RotateMatrixWithQuat(Float: matrix[4][4], Float: w, Float: x, Float: y, Float: z)
native RotateMatrixWithEuler(Float: matrix[4][4], Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp)
native RotateMatrixWithAxisAngle(Float: matrix[4][4], Float: angle, Float: aX, Float: aY, Float: aZ)
native GetTranslationMatrix(Float: matrix[4][4], Float: x, Float: y, Float: z)

native GetRotationMatrixFromQuat(Float: matrix[4][4], Float: w, Float: x, Float: y, Float: z)
native GetRotationMatrixFromEuler(Float: matrix[4][4], Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp)
native GetRotationMatrixFromAxisAngle(Float: matrix[4][4], Float: angle, Float: aX, Float: aY, Float: aZ)
native GetQuatFromMatrix(Float: matrix[4][4], & Float: w, & Float: x, & Float: y, & Float: z)
native GetQuatFromEuler(Float: rX, Float: rY, Float: rZ, & Float: w, & Float: x, & Float: y, & Float: z, eulermode: mode = euler_samp)
native GetQuatFromAxisAngle(Float: angle, Float: aX, Float: aY, Float: aZ, & Float: w, & Float: x, & Float: y, & Float: z)
native GetEulerFromMatrix(Float: matrix[4][4], & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp)
native GetEulerFromQuat(Float: w, Float: x, Float: y, Float: z, & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp)
native GetEulerFromEuler(Float: oX, Float: oY, Float: oZ, eulermode: omode, & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp)
native GetEulerFromAxisAngle(Float: angle, Float: aX, Float: aY, Float: aZ, & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp)
native GetAxisAngleFromMatrix(Float: matrix[4][4], & Float: angle, & Float: aX, & Float: aY, & Float: aZ)
native GetAxisAngleFromQuat(Float: w, Float: x, Float: y, Float: z, & Float: angle, & Float: aX, & Float: aY, & Float: aZ)
native GetAxisAngleFromEuler(Float: rX, Float: rY, Float: rZ, & Float: angle, & Float: aX, & Float: aY, & Float: aZ, eulermode: mode = euler_samp)
native MatrixRotate(Float: matrix[4][4], Float: oX, Float: oY, Float: oZ, Float: oT, & Float: X, & Float: Y, & Float: Z)
native QuatRotate(Float: w, Float: x, Float: y, Float: z, Float: oX, Float: oY, Float: oZ, & Float: X, & Float: Y, & Float: Z)
native EulerRotate(Float: rX, Float: rY, Float: rZ, Float: oX, Float: oY, Float: oZ, & Float: X, & Float: Y, & Float: Z, eulermode: mode = euler_samp)
native AxisAngleRotate(Float: angle, Float: aX, Float: aY, Float: aZ, Float: oX, Float: oY, Float: oZ, & Float: X, & Float: Y, & Float: Z)

native GetVehicleObjectPositionWorld(vehicleid, Float: att_X, Float: att_Y, Float: att_Z, Float: att_rotX, Float: att_rotY, Float: att_rotZ, &Float: X, &Float: Y, &Float: Z, &Float: rotX, &Float: rotY, &Float: rotZ)
native GetVehicleObjectPositionOffset(vehicleid, Float: X, Float: Y, Float: Z, Float: rotX, Float: rotY, Float: rotZ, &Float: att_X, &Float: att_Y, &Float: att_Z, &Float: att_rotX, &Float: att_rotY, &Float: att_rotZ)
native
*/

static // unroll variables for speed
    Float: m100, Float: m101, Float: m102, Float: m103,
    Float: m110, Float: m111, Float: m112, Float: m113,
    Float: m120, Float: m121, Float: m122, Float: m123,
    Float: m200, Float: m201, Float: m202, Float: m203,
    Float: m210, Float: m211, Float: m212, Float: m213,
    Float: m220, Float: m221, Float: m222, Float: m223
;

stock RotMatrixMatrixAroundRelPoint(Float: matrix1[4][4], Float: oX, Float: oY, Float: oZ, Float: matrix2[4][4]) {
    new
        Float: X,
        Float: Y,
        Float: Z
    ;
    MatrixRotate(matrix1, oX, oY, oZ, 1.0, X, Y, Z);
    RotMatrixMatrixAroundPoint(matrix1, X, Y, Z, matrix2);
}

stock RotMatrixQuatAroundRelPoint(Float: matrix[4][4], Float: oX, Float: oY, Float: oZ, Float: w, Float: x, Float: y, Float: z) {
    new
        Float: X,
        Float: Y,
        Float: Z
    ;
    MatrixRotate(matrix, oX, oY, oZ, 1.0, X, Y, Z);
    RotMatrixQuatAroundPoint(matrix, X, Y, Z, w, x, y, z);
}

stock RotMatrixEulerAroundRelPoint(Float: matrix[4][4], Float: oX, Float: oY, Float: oZ, Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp) {
    new
        Float: X,
        Float: Y,
        Float: Z
    ;
    MatrixRotate(matrix, oX, oY, oZ, 1.0, X, Y, Z);
    RotMatrixEulerAroundPoint(matrix, X, Y, Z, rX, rY, rZ, mode);
}

stock RotMatrixAxisAroundRelPoint(Float: matrix[4][4], Float: oX, Float: oY, Float: oZ, Float: angle, Float: aX, Float: aY, Float: aZ) {
    new
        Float: X,
        Float: Y,
        Float: Z
    ;
    MatrixRotate(matrix, oX, oY, oZ, 1.0, X, Y, Z);
    RotMatrixAxisAroundPoint(matrix, X, Y, Z, angle, aX, aY, aZ);
}

stock RotMatrixMatrixAroundPoint(Float: matrix1[4][4], Float: x, Float: y, Float: z, Float: matrix2[4][4]) {
    TranslateMatrix(matrix1, -x, -y, -z);
    RotateMatrixWithMatrix(matrix1, matrix2);
    TranslateMatrix(matrix1, x, y, z);
}

stock RotMatrixQuatAroundPoint(Float: matrix[4][4], Float: x, Float: y, Float: z, Float: w, Float: qX, Float: qY, Float: qZ) {
    TranslateMatrix(matrix, -x, -y, -z);
    RotateMatrixWithQuat(matrix, w, qX, qY, qZ);
    TranslateMatrix(matrix, x, y, z);
}

stock RotMatrixEulerAroundPoint(Float: matrix[4][4], Float: x, Float: y, Float: z, Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp) {
    TranslateMatrix(matrix, -x, -y, -z);
    RotateMatrixWithEuler(matrix, rX, rY, rZ, mode);
    TranslateMatrix(matrix, x, y, z);
}

stock RotMatrixAxisAroundPoint(Float: matrix1[4][4], Float: x, Float: y, Float: z, Float: angle, Float: aX, Float: aY, Float: aZ) {
    TranslateMatrix(matrix, -x, -y, -z);
    RotateMatrixWithAxisAngle(matrix, angle, aX, aY, aZ);
    TranslateMatrix(matrix, x, y, z);
}

stock TranslateMatrix(Float: matrix[4][4], Float: x, Float: y, Float: z) {
    matrix[0][3] += x; // impressive!
    matrix[1][3] += y;
    matrix[2][3] += z;
}

stock RotateMatrixWithMatrix(Float: matrix1[4][4], Float: matrix2[4][4]) { // matrix1 = matrix2 * matrix1
    // fill variables
    #emit load.s.pri matrix1
    #emit add.c 16
    #emit const.alt m200 // right
    #emit movs 48
    #emit load.s.pri matrix2
    #emit add.c 16
    #emit const.alt m100 // left
    #emit movs 48
    // optimsized for third line 0.0, 0.0, 0.0, 1.0
    matrix1[0][0] = m100 * m200 + m101 * m210 + m102 * m220;
    matrix1[0][1] = m100 * m201 + m101 * m211 + m102 * m221;
    matrix1[0][2] = m100 * m202 + m101 * m212 + m102 * m222;
    matrix1[0][3] = m100 * m203 + m101 * m213 + m102 * m223 + m103;
    matrix1[1][0] = m110 * m200 + m111 * m210 + m112 * m220;
    matrix1[1][1] = m110 * m201 + m111 * m211 + m112 * m221;
    matrix1[1][2] = m110 * m202 + m111 * m212 + m112 * m222;
    matrix1[1][3] = m110 * m203 + m111 * m213 + m112 * m223 + m113;
    matrix1[2][0] = m120 * m200 + m121 * m210 + m122 * m220;
    matrix1[2][1] = m120 * m201 + m121 * m211 + m122 * m221;
    matrix1[2][2] = m120 * m202 + m121 * m212 + m122 * m222;
    matrix1[2][3] = m120 * m203 + m121 * m213 + m122 * m223 + m123;
}

stock RotateMatrixWithQuat(Float: matrix[4][4], Float: w, Float: x, Float: y, Float: z) {
    new
        Float: tmp[4][4]
    ;
    GetRotationMatrixFromQuat(tmp, w, x, y, z);
    RotateMatrixWithMatrix(matrix, tmp);
}

stock RotateMatrixWithEuler(Float: matrix[4][4], Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp) {
    new
        Float: tmp[4][4]
    ;
    GetRotationMatrixFromEuler(tmp, rX, rY, rZ, mode);
    RotateMatrixWithMatrix(matrix, tmp);
}

stock RotateMatrixWithAxisAngle(Float: matrix[4][4], Float: angle, Float: aX, Float: aY, Float: aZ) {
    new
        Float: tmp[4][4]
    ;
    GetRotationMatrixFromAxisAngle(tmp, angle, aX, aY, aZ);
    RotateMatrixWithMatrix(matrix, tmp);
}

stock GetTranslationMatrix(Float: matrix[4][4], Float: x, Float: y, Float: z) {
    matrix[0][0] = 1.0;
    matrix[0][1] = 0.0;
    matrix[0][2] = 0.0;
    matrix[0][3] = x;
    matrix[1][0] = 0.0;
    matrix[1][1] = 1.0;
    matrix[1][2] = 0.0;
    matrix[1][3] = y;
    matrix[2][0] = 0.0;
    matrix[2][1] = 0.0;
    matrix[2][2] = 1.0;
    matrix[2][3] = z;
    matrix[3][0] = 0.0;
    matrix[3][1] = 0.0;
    matrix[3][2] = 0.0;
    matrix[3][3] = 1.0;
}

stock GetRotationMatrixFromQuat(Float: matrix[4][4], Float: w, Float: x, Float: y, Float: z) {
    new
        Float: x2 = x * x,
        Float: y2 = y * y,
        Float: z2 = z * z,
        Float: w2 = w * w,
        Float: xy = 2.0 * x * y,
        Float: xz = 2.0 * x * z,
        Float: yz = 2.0 * y * z,
        Float: wx = 2.0 * w * x,
        Float: wy = 2.0 * w * y,
        Float: wz = 2.0 * w * z;

    // rotation
    matrix[0][0] = w2 + x2 - y2 - z2;
    matrix[1][0] = xy - wz;
    matrix[2][0] = xz + wy;
    matrix[0][1] = xy + wz;
    matrix[1][1] = w2 - x2 + y2 - z2;
    matrix[2][1] = yz - wx;
    matrix[0][2] = xz - wy;
    matrix[1][2] = yz + wx;
    matrix[2][2] = w2 - x2 - y2 + z2;
    // translation
    matrix[0][3] = 0.0;
    matrix[1][3] = 0.0;
    matrix[2][3] = 0.0;
    matrix[3][0] = 0.0;
    matrix[3][1] = 0.0;
    matrix[3][2] = 0.0;
    matrix[3][3] = 1.0;
}

stock GetRotationMatrixFromEuler(Float: matrix[4][4], Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp) {
    new // could be done with matrix multiplication but would be slower
        Float: cosX = floatcos(rX, degrees),
        Float: cosY = floatcos(rY, degrees),
        Float: cosZ = floatcos(rZ, degrees),
        Float: sinX = floatsin(rX, degrees),
        Float: sinY = floatsin(rY, degrees),
        Float: sinZ = floatsin(rZ, degrees);

    switch(mode) { // Proper Euler angles - 1(rX), 2(rY), 3(rZ)
        case euler_xzx: {
            matrix[0][0] = cosY;
            matrix[0][1] = -cosZ * sinY;
            matrix[0][2] = sinY * sinZ;
            matrix[1][0] = cosX * sinY;
            matrix[1][1] = cosX * cosY * cosZ - sinX * sinZ;
            matrix[1][2] = -cosZ * sinX - cosX * cosY * sinZ;
            matrix[2][0] = sinX * sinY;
            matrix[2][1] = cosX * sinZ + cosY * cosZ * sinX;
            matrix[2][2] = cosX * cosZ - cosY * sinX * sinZ;
        }
        case euler_xyx: {
            matrix[0][0] = cosY;
            matrix[0][1] = sinY * sinZ;
            matrix[0][2] = cosZ * sinY;
            matrix[1][0] = sinX * sinY;
            matrix[1][1] = cosX * cosZ - cosY * sinX * sinZ;
            matrix[1][2] = -cosX * sinZ - cosY * cosZ * sinX;
            matrix[2][0] = -cosX * sinY;
            matrix[2][1] = cosZ * sinX + cosX * cosY * sinZ;
            matrix[2][2] = cosX * cosY * cosZ - sinX * sinZ;
        }
        case euler_yxy: {
            matrix[0][0] = cosX * cosZ - cosY * sinX * sinZ;
            matrix[0][1] = sinX * sinY;
            matrix[0][2] = cosX * sinZ + cosY * cosZ * sinX;
            matrix[1][0] = sinY * sinZ;
            matrix[1][1] = cosY;
            matrix[1][2] = -cosZ * sinY;
            matrix[2][0] = -cosZ * sinX - cosX * cosY * sinZ;
            matrix[2][1] = cosX * sinY;
            matrix[2][2] = cosX * cosY * cosZ - sinX * sinZ;
        }
        case euler_yzy: {
            matrix[0][0] = cosX * cosY * cosZ - sinX * sinZ;
            matrix[0][1] = -cosX * sinY;
            matrix[0][2] = cosZ * sinX + cosX * cosY * sinZ;
            matrix[1][0] = cosZ * sinY;
            matrix[1][1] = cosY;
            matrix[1][2] = sinY * sinZ;
            matrix[2][0] = -cosX * sinZ - cosY * cosZ * sinX;
            matrix[2][1] = sinX * sinY;
            matrix[2][2] = cosX * cosZ - cosY * sinX * sinZ;
        }
        case euler_zyz: {
            matrix[0][0] = cosX * cosY * cosZ - sinX * sinZ;
            matrix[0][1] = -cosZ * sinX - cosX * cosY * sinZ;
            matrix[0][2] = cosX * sinY;
            matrix[1][0] = cosX * sinZ + cosY * cosZ * sinX;
            matrix[1][1] = cosX * cosZ - cosY * sinX * sinZ;
            matrix[1][2] = sinX * sinY;
            matrix[2][0] = -cosZ * sinY;
            matrix[2][1] = sinY * sinZ;
            matrix[2][2] = cosY;
        }
        case euler_zxz: {
            matrix[0][0] = cosX * cosZ - cosY * sinX * sinZ;
            matrix[0][1] = -cosX * sinZ - cosY * cosZ * sinX;
            matrix[0][2] = sinX * sinY;
            matrix[1][0] = cosZ * sinX + cosX * cosY * sinZ;
            matrix[1][1] = cosX * cosY * cosZ - sinX * sinZ;
            matrix[1][2] = -cosX * sinY;
            matrix[2][0] = sinY * sinZ;
            matrix[2][1] = cosZ * sinY;
            matrix[2][2] = cosY;
        } // Tait-Bryan angles - X(rX), Y(rY), Z(rZ)
        case euler_xzy: {
            matrix[0][0] = cosZ * cosY;
            matrix[0][1] = -sinZ;
            matrix[0][2] = cosZ * sinY;
            matrix[1][0] = sinX * sinY + cosX * cosY * sinZ;
            matrix[1][1] = cosX * cosZ;
            matrix[1][2] = cosX * sinZ * sinY - cosY * sinX;
            matrix[2][0] = cosY * sinX * sinZ - cosX * sinY;
            matrix[2][1] = cosZ * sinX;
            matrix[2][2] = cosX * cosY + sinX * sinZ * sinY;
        }
        case euler_xyz: {
            matrix[0][0] = cosY * cosZ;
            matrix[0][1] = -cosY * sinZ;
            matrix[0][2] = sinY;
            matrix[1][0] = cosX * sinZ + cosZ * sinX * sinY;
            matrix[1][1] = cosX * cosZ - sinX * sinY * sinZ;
            matrix[1][2] = -cosY * sinX;
            matrix[2][0] = sinX * sinZ - cosX * cosZ * sinY;
            matrix[2][1] = cosZ * sinX + cosX * sinY * sinZ;
            matrix[2][2] = cosX * cosY;
        }
        case euler_yxz: {
            matrix[0][0] = cosY * cosZ + sinY * sinX * sinZ;
            matrix[0][1] = cosZ * sinY * sinX - cosY * sinZ;
            matrix[0][2] = cosX * sinY;
            matrix[1][0] = cosX * sinZ;
            matrix[1][1] = cosX * cosZ;
            matrix[1][2] = -sinX;
            matrix[2][0] = cosY * sinX * sinZ - cosZ * sinY;
            matrix[2][1] = cosY * cosZ * sinX + sinY * sinZ;
            matrix[2][2] = cosY * cosX;
        }
        case euler_yzx: {
            matrix[0][0] = cosY * cosZ;
            matrix[0][1] = sinY * sinX - cosY * cosX * sinZ;
            matrix[0][2] = cosX * sinY + cosY * sinZ * sinX;
            matrix[1][0] = sinZ;
            matrix[1][1] = cosZ * cosX;
            matrix[1][2] = -cosZ * sinX;
            matrix[2][0] = -cosZ * sinY;
            matrix[2][1] = cosY * sinX + cosX * sinY * sinZ;
            matrix[2][2] = cosY * cosX - sinY * sinZ * sinX;
        }
        case euler_zyx: { // pitch roll yaw
            matrix[0][0] = cosZ * cosY;
            matrix[0][1] = cosZ * sinY * sinX - cosX * sinZ;
            matrix[0][2] = sinZ * sinX + cosZ * cosX * sinY;
            matrix[1][0] = cosY * sinZ;
            matrix[1][1] = cosZ * cosX + sinZ * sinY * sinX;
            matrix[1][2] = cosX * sinZ * sinY - cosZ * sinX;
            matrix[2][0] = -sinY;
            matrix[2][1] = cosY * sinX;
            matrix[2][2] = cosY * cosX;
        }
        case euler_zxy: { // sa-mp
            matrix[0][0] = cosZ * cosY - sinZ * sinX * sinY;
            matrix[0][1] = -sinZ * cosX;
            matrix[0][2] = cosZ * sinY + sinZ * sinX * cosY;
            matrix[1][0] = sinZ * cosY + cosZ * sinX * sinY;
            matrix[1][1] = cosZ * cosX;
            matrix[1][2] = sinZ * sinY - cosZ * sinX * cosY;
            matrix[2][0] = -cosX * sinY;
            matrix[2][1] = sinX;
            matrix[2][2] = cosX * cosY;
        }
    }
    matrix[0][3] = 0.0;
    matrix[1][3] = 0.0;
    matrix[2][3] = 0.0;
    matrix[3][0] = 0.0;
    matrix[3][1] = 0.0;
    matrix[3][2] = 0.0;
    matrix[3][3] = 1.0;
}

stock GetRotationMatrixFromAxisAngle(Float: matrix[4][4], Float: angle, Float: aX, Float: aY, Float: aZ) {
    new
        Float: cos = floatcos(angle, degrees),
        Float: sin = floatsin(angle, degrees),
        Float: t = 1.0 - cos;

    matrix[0][0] = t * aX * aX + cos;
    matrix[1][1] = t * aY * aY + cos;
    matrix[2][2] = t * aZ * aZ + cos;

    cos = t * aX * aY;

    matrix[0][1] = cos - sin * aZ;
    matrix[1][0] = cos + sin * aZ;

    cos = t * aX * aZ;

    matrix[0][2] = cos + sin * aY;
    matrix[2][0] = cos - sin * aY;

    cos = t * aY * aZ;

    matrix[1][2] = cos - sin * aX;
    matrix[2][1] = cos + sin * aX;

    matrix[0][3] = 0.0;
    matrix[1][3] = 0.0;
    matrix[2][3] = 0.0;
    matrix[3][0] = 0.0;
    matrix[3][1] = 0.0;
    matrix[3][2] = 0.0;
    matrix[3][3] = 1.0;
}

// can result in NaN (1.0 + matrix[0][0] + matrix[1][1] + matrix[2][2]) could be <= 0.0
stock GetQuatFromMatrix(Float: matrix[4][4], & Float: w, & Float: x, & Float: y, & Float: z) {
    #emit load.s.pri matrix
    #emit add.c 16
    #emit const.alt m100
    #emit movs 44

    z = -4.0 * (w = floatsqroot(1.0 + m100 + m111 + m122) / 2.0);

    x = (m121 - m112) / z;
    y = (m102 - m120) / z;
    z = (m110 - m101) / z;

    #emit retn // neccessary to ensure what no variable gets removed because it isn't used
    #emit const.pri m103
    #emit const.pri m113
    // #emit const.pri m123 - not needed thus only 44 instead of 48 bytes to copy
}

stock GetQuatFromEuler(Float: rX, Float: rY, Float: rZ, & Float: w, & Float: x, & Float: y, & Float: z, eulermode: mode = euler_samp) {
    rX /= 2.0; // could be done with multiplication but would be slower
    rY /= 2.0;
    rZ /= 2.0;

    new	Float: cosX = floatcos(rX, degrees),
        Float: cosY = floatcos(rY, degrees),
        Float: cosZ = floatcos(rZ, degrees),
        Float: sinX = floatsin(rX, degrees),
        Float: sinY = floatsin(rY, degrees),
        Float: sinZ = floatsin(rZ, degrees);

    switch(mode) {
        case euler_xzx: {
            w = sinX * cosY * sinZ - cosX * cosY * cosZ;
            x = cosX * cosY * sinZ + sinX * cosY * cosZ;
            y = cosX * sinY * sinZ - sinX * sinY * cosZ;
            z = cosX * sinY * cosZ + sinX * sinY * sinZ;		
        }
        case euler_xyx: {
            w = sinX * cosY * sinZ - cosX * cosY * cosZ;
            x = cosX * cosY * sinZ + sinX * cosY * cosZ;
            y = cosX * sinY * cosZ + sinX * sinY * sinZ;
            z = sinX * sinY * cosZ - cosX * sinY * sinZ;
        }
        case euler_yxy: {
            w = sinX * cosY * sinZ - cosX * cosY * cosZ;
            x = cosX * sinY * cosZ + sinX * sinY * sinZ;
            y = cosX * cosY * sinZ + sinX * cosY * cosZ;
            z = cosX * sinY * sinZ - sinX * sinY * cosZ;
        }
        case euler_yzy: {
            w = sinX * cosY * sinZ - cosX * cosY * cosZ;
            x = sinX * sinY * cosZ - cosX * sinY * sinZ;
            y = cosX * cosY * sinZ + sinX * cosY * cosZ;
            z = cosX * sinY * cosZ + sinX * sinY * sinZ;
        }
        case euler_zyz: {
            w = sinX * cosY * sinZ - cosX * cosY * cosZ;
            x = cosX * sinY * sinZ - sinX * sinY * cosZ;
            y = cosX * sinY * cosZ + sinX * sinY * sinZ;
            z = cosX * cosY * sinZ + sinX * cosY * cosZ;
        }
        case euler_zxz: {
            w = sinX * cosY * sinZ - cosX * cosY * cosZ;
            x = cosX * sinY * cosZ + sinX * sinY * sinZ;
            y = sinX * sinY * cosZ - cosX * sinY * sinZ;
            z = cosX * cosY * sinZ + sinX * cosY * cosZ;
        }
        case euler_xzy: {
            w = cosX * cosY * cosZ + sinX * sinY * sinZ;
            x = cosX * sinY * sinZ - sinX * cosY * cosZ;
            y = sinX * cosY * sinZ - cosX * sinY * cosZ;
            z = -(cosX * cosY * sinZ + sinX * sinY * cosZ);
        }
        case euler_xyz: {
            w = sinX * sinY * sinZ - cosX * cosY * cosZ;
            x = sinX * cosY * cosZ + cosX * sinY * sinZ;
            y = cosX * sinY * cosZ - sinX * cosY * sinZ;
            z = cosX * cosY * sinZ + sinX * sinY * cosZ;
        }
        case euler_yxz: {
            w = cosX * cosY * cosZ + sinX * sinY * sinZ;
            x = -(sinX * cosY * cosZ + cosX * sinY * sinZ);
            y = sinX * cosY * sinZ - cosX * sinY * cosZ;
            z = sinX * sinY * cosZ - cosX * cosY * sinZ;
        }
        case euler_yzx: {
            w = sinX * sinY * sinZ - cosX * cosY * cosZ;
            x = sinX * cosY * cosZ + cosX * sinY * sinZ;
            y = cosX * sinY * cosZ + sinX * cosY * sinZ;
            z = cosX * cosY * sinZ - sinX * sinY * cosZ;
        }
        case euler_zyx: {
            w = cosX * cosY * cosZ + sinX * sinY * sinZ;
            x = cosX * sinY * sinZ - sinX * cosY * cosZ;
            y = -(cosX * sinY * cosZ + sinX * cosY * sinZ);
            z = sinX * sinY * cosZ - cosX * cosY * sinZ;
        }
        case euler_zxy: {
            w = sinX * sinY * sinZ - cosX * cosY * cosZ;
            x = sinX * cosY * cosZ - cosX * sinY * sinZ;
            y = cosX * sinY * cosZ + sinX * cosY * sinZ;
            z = cosX * cosY * sinZ + sinX * sinY * cosZ;
        }
    }
}

stock GetQuatFromAxisAngle(Float: angle, Float: aX, Float: aY, Float: aZ, & Float: w, & Float: x, & Float: y, & Float: z) {
    angle /= -2.0;

    w = floatsin(angle, degrees) / VectorSize(aX, aY, aZ);

    x = aX * w;
    y = aY * w;
    z = aZ * w;
    w = floatcos(angle, degrees);
}

stock GetEulerFromMatrix(Float: matrix[4][4], & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp) {
    switch(mode) {
        case euler_xzx: {
            rY = matrix[0][0];

            if(rY > 0.9999) {
                rX = atan2(matrix[2][1], matrix[1][1]); // -12, 22
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(matrix[2][1], matrix[1][1]);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(matrix[2][0], matrix[1][0]);
                rY = acos(rY);
                rZ = atan2(matrix[0][2], -matrix[0][1]);
            }
        }
        case euler_xyx: {
            rY = matrix[0][0];

            if(rY > 0.9999) {
                rX = atan2(matrix[2][1], matrix[1][1]); // -12, 22
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(matrix[2][1], matrix[1][1]);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(matrix[1][0], -matrix[2][0]);
                rY = acos(rY);
                rZ = atan2(matrix[0][1], matrix[0][2]);
            }
        }
        case euler_yxy: {
            rY = matrix[1][1];

            if(rY > 0.9999) {
                rX = atan2(matrix[0][2], matrix[2][2]); // -20, 00
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(matrix[0][2], matrix[2][2]);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(matrix[0][1], matrix[2][1]);
                rY = acos(rY);
                rZ = atan2(matrix[1][0], -matrix[1][2]);
            }
        }
        case euler_yzy: {
            rY = matrix[1][1];

            if(rY > 0.9999) {
                rX = atan2(matrix[0][2], matrix[2][2]); // -20, 00
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(matrix[0][2], matrix[2][2]);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(matrix[2][1], -matrix[0][1]);
                rY = acos(rY);
                rZ = atan2(matrix[1][2], matrix[1][0]);
            }
        }
        case euler_zyz: {
            rY = matrix[2][2];

            if(rY > 0.9999) {
                rX = atan2(matrix[1][0], matrix[0][0]); // -01, 11
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(matrix[1][0], matrix[0][0]);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(matrix[1][2], matrix[0][2]);
                rY = acos(rY);
                rZ = atan2(matrix[2][1], -matrix[2][0]);
            }
        }
        case euler_zxz: {
            rY = matrix[2][2];

            if(rY > 0.9999) {
                rX = atan2(matrix[1][0], matrix[0][0]); // -01, 11
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(matrix[1][0], matrix[0][0]);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(matrix[0][2], -matrix[1][2]);
                rY = acos(rY);
                rZ = atan2(matrix[2][0], matrix[2][1]);
            }
        }
        case euler_xzy: {
            rZ = matrix[0][1];

            if(rZ < -0.9999) {
                rX = atan2(matrix[2][0], matrix[1][0]); // -12, 22
                rY = 0.0;
                rZ = 90.0;
            } else if(rZ > 0.9999) {
                rX = atan2(matrix[2][0], matrix[1][0]);
                rY = 0.0;
                rZ = -90.0;
            } else {
                rX = atan2(matrix[2][1], matrix[1][1]);
                rY = atan2(matrix[0][2], matrix[0][0]);
                rZ = -asin(rZ);
            }
        }
        case euler_xyz: {
            rY = matrix[0][2];

            if(rY > 0.9999) {
                rX = atan2(matrix[2][1], matrix[1][1]); // 10, -20
                rY = 90.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(matrix[2][1], matrix[1][1]);
                rY = -90.0;
                rZ = 0.0;
            } else {
                rX = -atan2(matrix[1][2], matrix[2][2]);
                rY = asin(rY);
                rZ = -atan2(matrix[0][1], matrix[0][0]);
            }
        }
        case euler_yxz: {
            rX = matrix[1][2];

            if(rX < -0.9999) {
                rX = 90.0;
                rY = atan2(matrix[0][1], matrix[2][1]); // -20, 00
                rZ = 0.0;
            } else if(rX > 0.9999) {
                rX = -90.0;
                rY = atan2(matrix[0][1], matrix[2][1]);
                rZ = 0.0;
            } else {
                rX = -asin(rX);
                rY = atan2(matrix[0][2], matrix[2][2]);
                rZ = atan2(matrix[1][0], matrix[1][1]);
            }
        }
        case euler_yzx: {
            rZ = matrix[1][0];

            if(rZ > 0.9999) {
                rX = 0.0;
                rY = atan2(matrix[0][2], matrix[2][2]); // 21, -01
                rZ = 90.0;
            } else if(rZ < -0.9999) {
                rX = 0.0;
                rY = atan2(matrix[0][2], matrix[2][2]);
                rZ = -90.0;
            } else {
                rX = -atan2(matrix[1][2], matrix[1][1]);
                rY = -atan2(matrix[2][0], matrix[0][0]);
                rZ = asin(rZ);
            }
        }
        case euler_zyx: { // pitch roll yaw
            rY = matrix[2][0];

            if(rY < -0.9999) {
                rX = 0.0;
                rY = 90.0;
                rZ = atan2(matrix[1][2], matrix[0][2]); // -01, 11
            } else if(rY > 0.9999) {
                rX = 0.0;
                rY = -90.0;
                rZ = atan2(matrix[1][2], matrix[0][2]);
            } else {
                rX = atan2(matrix[2][1], matrix[2][2]);
                rY = -asin(rY);
                rZ = atan2(matrix[1][0], matrix[0][0]);
            }
        }
        case euler_zxy: { // sa-mp
            rX = matrix[2][1]; // singularitie at rX = +- 90.0

            if(rX > 0.9999) { // ~ 89.2 degrees
                rX = 90.0;
                rY = 0.0;
                rZ = atan2(matrix[1][0], matrix[0][0]); // 02, -12
            } else if(rX < -0.9999) {
                rX = -90.0;
                rY = 0.0;
                rZ = atan2(matrix[1][0], matrix[0][0]);
            } else { // it does work without the singularities but it gets inaccurate
                rX = asin(rX);
                rY = -atan2(matrix[2][0], matrix[2][2]);
                rZ = -atan2(matrix[0][1], matrix[1][1]);
            }
        }
    }
}

stock GetEulerFromQuat(Float: w, Float: x, Float: y, Float: z, & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp) {
    switch(mode) {
        case euler_xzx: {
            rY = w * w + x * x - y * y - z * z;

            if(rY > 0.9999) {
                rX = atan2(2.0 * (y * z - w * x), w * w - x * x + y * y - z * z);
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(2.0 * (y * z - w * x), w * w - x * x + y * y - z * z);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(x * z + w * y, x * y - w * z);
                rY = acos(rY);
                rZ = atan2(x * z - w * y, -(x * y + w * z));
            }
        }
        case euler_xyx: {
            rY = w * w + x * x - y * y - z * z;

            if(rY > 0.9999) {
                rX = atan2(2.0 * (y * z - w * x), w * w - x * x + y * y - z * z);
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(2.0 * (y * z - w * x), w * w - x * x + y * y - z * z);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(x * y - w * z, -(x * z + w * y));
                rY = acos(rY);
                rZ = atan2(x * y + w * z, x * z - w * y);
            }
        }
        case euler_yxy: {
            rY = w * w - x * x + y * y - z * z;

            if(rY > 0.9999) {
                rX = atan2(2.0 * (x * z - w * y), w * w - x * x - y * y + z * z);
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(2.0 * (x * z - w * y), w * w - x * x - y * y + z * z);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(x * y + w * z, y * z - w * x);
                rY = acos(rY);
                rZ = atan2(x * y - w * z, -(y * z + w * x));
            }
        }
        case euler_yzy: {
            rY = w * w - x * x + y * y - z * z;

            if(rY > 0.9999) {
                rX = atan2(2.0 * (x * z - w * y), w * w - x * x - y * y + z * z);
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(2.0 * (x * z - w * y), w * w - x * x - y * y + z * z);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(y * z - w * x, -(x * y + w * z));
                rY = acos(rY);
                rZ = atan2(y * z + w * x, x * y - w * z);
            }
        }
        case euler_zyz: {
            rY = w * w - x * x - y * y + z * z;

            if(rY > 0.9999) {
                rX = atan2(2.0 * (x * y - w * z), w * w + x * x - y * y - z * z);
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(2.0 * (x * y - w * z), w * w + x * x - y * y - z * z);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(y * z + w * x, x * z - w * y);
                rY = acos(rY);
                rZ = atan2(y * z - w * x, -(x * z + w * y));
            }
        }
        case euler_zxz: {
            rY = w * w - x * x - y * y + z * z;

            if(rY > 0.9999) {
                rX = atan2(2.0 * (x * y - w * z), w * w + x * x - y * y - z * z);
                rY = 0.0;
                rZ = 0.0;
            } else if(rY < -0.9999) {
                rX = atan2(2.0 * (x * y - w * z), w * w + x * x - y * y - z * z);
                rY = 180.0;
                rZ = 0.0;
            } else {
                rX = atan2(x * z - w * y, -(y * z + w * x));
                rY = acos(rY);
                rZ = atan2(x * z + w * y, y * z - w * x);
            }
        }
        case euler_xzy: {
            rZ = x * y + w * z;

            if(rZ < -0.49995) {
                rX = atan2(x * z + w * y, x * y - w * z);
                rY = 0.0;
                rZ = 90.0;
            } else if(rZ > 0.49995) {
                rX = atan2(x * z + w * y, x * y - w * z);
                rY = 0.0;
                rZ = -90.0;
            } else {
                mode = eulermode: (w * w - z * z);
                rY = x * x - y * y;

                rX = atan2(2.0 * (y * z - w * x), Float: mode - rY);
                rY = atan2(2.0 * (x * z - w * y), Float: mode + rY);
                rZ = asin(-2.0 * rZ);
            }
        }
        case euler_xyz: {
            rY = x * z - w * y;

            if(rY > 0.49995) {
                rX = atan2(w * x - y * z, x * z + w * y);
                rY = 90.0;
                rZ = 0.0;
            } else if(rY < -0.49995) {
                rX = atan2(y * z - w * x, x * z + w * y);
                rY = -90.0;
                rZ = 0.0;
            } else {
                mode = eulermode: (w * w - y * y);
                rZ = x * x - z * z;

                rX = atan2(-2.0 * (y * z + w * x), Float: mode - rZ);
                rY = asin(2.0 * rY);
                rZ = atan2(-2.0 * (x * y + w * z), Float: mode + rZ);
            }
        }
        case euler_yxz: {
            rX = y * z + w * x;

            if(rX < -0.49995) {
                rX = 90.0;
                rY = atan2(x * y + w * z, y * z - w * x);
                rZ = 0.0;
            } else if(rX > 0.49995) {
                rX = -90.0;
                rY = atan2(x * y + w * z, y * z - w * x);
                rZ = 0.0;
            } else {
                mode = eulermode: (w * w - x * x);
                rZ = y * y - z * z;

                rX = asin(-2.0 * rX);
                rY = atan2(2.0 * (x * z - w * y), Float: mode - rZ);
                rZ = atan2(2.0 * (x * y - w * z), Float: mode + rZ);
            }
        }
        case euler_yzx: {
            rZ = x * y - w * z;

            if(rZ > 0.49995) {
                rX = 0.0;
                rY = atan2(w * y - x * z, x * y + w * z);
                rZ = 90.0;
            } else if(rZ < -0.49995) {
                rX = 0.0;
                rY = atan2(x * z - w * y, x * y + w * z);
                rZ = -90.0;
            } else {
                mode = eulermode: (w * w - z * z);
                rY = x * x - y * y;

                rX = atan2(-2.0 * (y * z + w * x), Float: mode - rY);
                rY = atan2(-2.0 * (x * z + w * y), Float: mode + rY);
                rZ = asin(2.0 * rZ);
            }
        }
        case euler_zyx: { // pitch roll yaw
            rY = x * z + w * y;

            if(rY < -0.49995) {
                rX = 0.0;
                rY = 90.0;
                rZ = atan2(y * z + w * x, x * z - w * y);
            } else if(rY > 0.49995) {
                rX = 0.0;
                rY = -90.0;
                rZ = atan2(y * z + w * x, x * z - w * y);
            } else {
                mode = eulermode: (w * w - y * y);
                rZ = x * x - z * z;

                rX = atan2(2.0 * (y * z - w * x), Float: mode - rZ);
                rY = asin(-2.0 * rY);
                rZ = atan2(2.0 * (x * y - w * z), Float: mode + rZ);
            }
        }
        case euler_zxy: { // sa-mp
            rX = y * z - w * x;

            if(rX > 0.49995) {
                rY = 0.0;
                rX = 90.0;
                rZ = atan2(w * y - x * z, y * z + w * x);
            } else if(rX < -0.49995) {
                rY = 0.0;
                rX = -90.0;
                rZ = atan2(w * y - x * z, y * z + w * x);
            } else {
                mode = eulermode: (w * w - x * x);
                rZ = y * y - z * z;

                rX = asin(2.0 * rX);
                rY = atan2(-2.0 * (x * z + w * y), Float: mode - rZ);
                rZ = atan2(-2.0 * (x * y + w * z), Float: mode + rZ);
            }
        }
    }
}

stock GetEulerFromEuler(Float: oX, Float: oY, Float: oZ, eulermode: omode, & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp) {
    GetQuatFromEuler(oX, oY, oZ, rX, oX, oY, oZ, omode); // to much work I guess
    GetEulerFromQuat(rX, oX, oY, oZ, rX, rY, rZ, mode);
}

stock GetEulerFromAxisAngle(Float: angle, Float: aX, Float: aY, Float: aZ, & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp) {
    new Float: matrix[4][4]; // leaving that unoptimised, see no use for it

    GetRotationMatrixFromAxisAngle(matrix, angle, aX, aY, aZ);
    GetEulerFromMatrix(matrix, rX, rY, rZ, mode);
}

stock GetAxisAngleFromMatrix(Float: matrix[4][4], & Float: angle, & Float: aX, & Float: aY, & Float: aZ) {
    #emit load.s.pri matrix
    #emit add.c 16
    #emit const.alt m100
    #emit movs 44

    aX = m121 - m112; // does have untriggered singularities
    aY = m102 - m120;
    aZ = m110 - m101;

    angle = VectorSize(aX, aY, aZ);

    aX /= angle;
    aY /= angle;
    aZ /= angle;

    angle = acos((m100 + m111 + m122 - 1.0) / 2.0);

    #emit retn // neccessary to ensure what no variable gets removed because it isn't used
    #emit const.pri m103
    #emit const.pri m113
    // #emit const.pri m123
}

stock GetAxisAngleFromQuat(Float: w, Float: x, Float: y, Float: z, & Float: angle, & Float: aX, & Float: aY, & Float: aZ) {
    angle = 2.0 * acos(-w);
    w = VectorSize(x, y, z); // w = floatsqroot(1.0 - w * w); // w = floatsin(angle, degrees); // |sin(angle)|

    if(w == 0.0) { // no rotation
        aX = 1.0;
        aY = 0.0;
        aZ = 0.0;

        angle = 0.0;
    } else {
        aX = x / w;
        aY = y / w;
        aZ = z / w;
    }
}

stock GetAxisAngleFromEuler(Float: rX, Float: rY, Float: rZ, & Float: angle, & Float: aX, & Float: aY, & Float: aZ, eulermode: mode = euler_samp) {
    GetQuatFromEuler(rX, rY, rZ, angle, aX, aY, aZ, mode);
    GetAxisAngleFromQuat(angle, aX, aY, aZ, angle, aX, aY, aZ);	
}

stock MatrixRotate(Float: matrix[4][4], Float: oX, Float: oY, Float: oZ, Float: oT, & Float: X, & Float: Y, & Float: Z) {
    #emit load.s.pri matrix
    #emit add.c 16
    #emit const.alt m100
    #emit movs 48

    // oT = Translation, 0.0 for no translation, 1.0 for translation
    X += oX * m100 + oY * m101 + oZ * m102 + oT * m103; // M * v | v = [oX, oY, oZ]
    Y += oX * m110 + oY * m111 + oZ * m112 + oT * m113;
    Z += oX * m120 + oY * m121 + oZ * m122 + oT * m123;
}

stock QuatRotate(Float: w, Float: x, Float: y, Float: z, Float: oX, Float: oY, Float: oZ, & Float: X, & Float: Y, & Float: Z) {
    // wikipedia suggest this formula (a x b - cross prudct)
    // v + 2*r x (r x v + w*v) | q = w,r
    // 15 muls, 15 adds, 9 saves, 42 loads
    // new
        // Float: tX = y * oZ - z * oY - w * oX,
        // Float: tY = z * oX - x * oZ - w * oY,
        // Float: tZ = x * oY - y * oX - w * oZ;

    // x += x;
    // y += y;
    // z += z;

    // X += oX + y * tZ - z * tY;
    // Y += oY + z * tX - x * tZ;
    // Z += oZ + x * tY - y * tX;
    // ---
    // but I used this one because it more compact (a.b - dot product)
    // 2*((r.v)*r + (0.5 - (|r.r|^2))*v + w * (r x v)) | q = w,r
    // 22 muls, 12 adds, 5 saves, 38 loads
    new
        Float: dot = x * oX + y * oY + z * oZ,
        Float: abs = w * w - 0.5;

    X += 2.0 * (dot * x + abs * oX + w * (z * oY - y * oZ));
    Y += 2.0 * (dot * y + abs * oY + w * (x * oZ - z * oX));
    Z += 2.0 * (dot * z + abs * oZ + w * (y * oX - x * oY));
}

stock EulerRotate(Float: rX, Float: rY, Float: rZ, Float: oX, Float: oY, Float: oZ, & Float: X, & Float: Y, & Float: Z, eulermode: mode = euler_samp) {
    // saddly I couldn't find a way to rotate euler angles without a step in between, is there even a way?
    GetQuatFromEuler(rX, rY, rZ, Float: mode, rX, rY, rZ, mode);
    QuatRotate(Float: mode, rX, rY, rZ, oX, oY, oZ, X, Y, Z);
}

stock AxisAngleRotate(Float: angle, Float: aX, Float: aY, Float: aZ, Float: oX, Float: oY, Float: oZ, & Float: X, & Float: Y, & Float: Z) {
    new // uses Rodrigues rotation formula, similar to QuatRotate
        Float: cos = floatcos(angle, degrees),
        Float: sin = floatsin(angle, degrees),
        Float: ts = (1.0 - cos) * (aX * oX + aY * oY + aZ * oZ);
    // cos * v + sin * (e x v) + (1 - cos) * (e.v) * e | e = [aX, aY, aZ] v = [oX, oY, oZ]
    X += ts * aX + cos * oX + sin * (aY * oZ - aZ * oY);
    Y += ts * aY + cos * oY + sin * (aZ * oX - aX * oZ);
    Z += ts * aZ + cos * oZ + sin * (aX * oY - aY * oX);
}