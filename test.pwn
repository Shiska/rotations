#define RUN_TESTS

#include <streamer>
#include <YSI\y_testing>

#include "rotation"
#include "rotation_misc"
#include "rotation_extra"

forward bool: CheckOrPrintAxisAngle(comment[], Float: a1, Float: x1, Float: y1, Float: z1, Float: a2, Float: x2, Float: y2, Float: z2);
forward bool: CheckOrPrintEuler(comment[], Float: x1, Float: y1, Float: z1, Float: x2, Float: y2, Float: z2);
forward bool: CheckOrPrintQuat(comment[], Float: w1, Float: x1, Float: y1, Float: z1, Float: w2, Float: x2, Float: y2, Float: z2);
forward bool: CheckOrPrintMatrix(comment[], Float: matrix1[3][3], Float: matrix2[3][3]);

Test:Compile() { // all functions which should call all subfunctions
    new Float: matrix[4][4];
    new rotation[E_ROTATION];
    new Float: w, Float: x, Float: y, Float: z;
    // rotation.inc
    SetRotation(rotation, rtype_axis_angle, w, x, y, z);        // SetRotationFromAxisAngle
    SetRotation(rotation, rtype_euler_samp, x, y, z);           // SetRotationFromEuler
    SetRotation(rotation, rtype_quaternion, w, x, y, z);        // SetRotationFromQuat
    SetRotation(rotation, rtype_rotation_matrix, matrix);       // SetRotationFromMatrix

    ConvertRotation(rotation, rtype_euler_samp, rotation);      // ConvertMatrixToEuler
    ConvertRotation(rotation, rtype_quaternion, rotation);      // ConvertEulerToQuat
    ConvertRotation(rotation, rtype_axis_angle, rotation);      // ConvertQuatToAxisAngle

    GetRotation(rotation, rtype_axis_angle, w, x, y, z);        // GetAxisAngleFromRotation
    RotatePoint(rotation, x, y, z, x, y, z, x, y, z);           // RotateAxisAngle
    ReverseRotation(rotation, rotation);                        // ReverseAxisAngle

    ConvertRotation(rotation, rtype_rotation_matrix, rotation); // ConvertAxisAngleToMatrix
    ConvertRotation(rotation, rtype_quaternion, rotation);      // ConvertMatrixToQuat
    ConvertRotation(rotation, rtype_euler_samp, rotation);      // ConvertQuatToEuler

    GetRotation(rotation, rtype_euler_samp, x, y, z);           // GetEulerFromRotation
    ReverseRotation(rotation, rotation);                        // ReverseEuler

    ConvertRotation(rotation, rtype_axis_angle, rotation);      // ConvertEulerToAxisAngle
    ConvertRotation(rotation, rtype_quaternion, rotation);      // ConvertAxisAngleToQuat

    GetRotation(rotation, rtype_quaternion, w, x, y, z);        // GetQuatFromRotation
    RotatePoint(rotation, x, y, z, x, y, z, x, y, z);           // RotateQuat
    CombineRotation(rotation, rotation, rotation);              // CombineQuat
    ReverseRotation(rotation, rotation);                        // ReverseQuat

    ConvertRotation(rotation, rtype_rotation_matrix, rotation); // ConvertQuatToMatrix
    ConvertRotation(rotation, rtype_axis_angle, rotation);      // ConvertMatrixToAxisAngle
    ConvertRotation(rotation, rtype_euler_samp, rotation);      // ConvertAxisAngleToEuler
    ConvertRotation(rotation, rtype_euler_xyx, rotation);       // ConvertEulerToEuler
    ConvertRotation(rotation, rtype_rotation_matrix, rotation); // ConvertEulerToMatrix

    GetRotation(rotation, rtype_rotation_matrix, matrix);       // GetMatrixFromRotation
    RotatePoint(rotation, x, y, z, x, y, z, x, y, z);           // RotateMatrix
    CombineRotation(rotation, rotation, rotation);              // CombineMatrix
    ReverseRotation(rotation, rotation);                        // ReverseMatrix
    // rotation_misc.inc
    QuatNormalise(w, x, y, z);
    QuatScale(w, x, y, z, 5.0);
    QuatMul(w, x, y, z, w, x, y, z, w, x, y, z);
    QuatAdd(w, x, y, z, w, x, y, z, w, x, y, z);

    MatrixMul(matrix, matrix, matrix);
    MatrixAdd(matrix, matrix, matrix);
    MatrixSub(matrix, matrix, matrix);

    RotationMatrixX(matrix, w);
    RotationMatrixY(matrix, w);
    RotationMatrixZ(matrix, w);
    ScalerMatrix(matrix, w, x, y, z);
    TranslationMatrix(matrix, x, y, z);
    // rotation_extra.inc
    GetAttachedPos(x, y, z, rotation, x, y, z, rotation, x, y, z, rotation);
    GetAttachedOffset(x, y, z, rotation, x, y, z, rotation, x, y, z, rotation);

    GetObjectRotation(0, rotation);
    SetObjectRotation(0, rotation);
    GetObjectAttachedPos(0, x, y, z, x, y, z, x, y, z, x, y, z);
    GetObjectAttachedOffset(0, x, y, z, x, y, z, x, y, z, x, y, z);
    GetPlayerRotation(0, rotation);
    SetPlayerRotation(0, rotation);
    GetPlayerAttachedPos(0, x, y, z, x, y, z, x, y, z, x, y, z);
    GetPlayerAttachedOffset(0, x, y, z, x, y, z, x, y, z, x, y, z);
    GetVehicleRotation(0, rotation);
    SetVehicleRotation(0, rotation);
    GetVehicleAttachedPos(0, x, y, z, x, y, z, x, y, z, x, y, z);
    GetVehicleAttachedOffset(0, x, y, z, x, y, z, x, y, z, x, y, z);

    AttachObjectToObjectEx(0, 0);
    AttachObjectToPlayerEx(0, 0);
    AttachObjectToVehicleEx(0, 0);

    GetVehicleRelativePos(0, x, y, z, x, y, z);
    GetVehicleForwardVector(0, x, y, z);
    GetVehicleRightVector(0, x, y, z);
    GetVehicleUpVector(0, x, y, z);

    GetDynamicObjectRotation(0, rotation);
    SetDynamicObjectRotation(0, rotation);
    GetDynamicObjectAttachedPos(0, x, y, z, x, y, z, x, y, z, x, y, z);
    GetDynamicObjectAttachedOffset(0, x, y, z, x, y, z, x, y, z, x, y, z);

    DetachDynamicObject(0);

    AttachDynamicObjectToObjectEx(0, 0);
    AttachDynamicObjectToPlayerEx(0, 0);
    AttachDynamicObjectToVehicleEx(0, 0);
}

Test:ConvertRotation() { // check all conversion functions, although occasionally some test fail due to floating point inaccuracy 
    new comment[32];
    new src[E_ROTATION];
    new dest[E_ROTATION];
    new rotationtype: type;

    new Float: src_angle = random(18000) / 100.0; 
    new Float: src_w = (random(200) - 100) / 100.0;  
    new Float: src_x = (random(200) - 100) / 100.0;
    new Float: src_y = (random(200) - 100) / 100.0;
    new Float: src_z = (random(200) - 100) / 100.0;
    new Float: src_a = random(36000) / 100.0; 
    new Float: src_b = random(36000) / 100.0;
    new Float: src_g = random(36000) / 100.0;
    new Float: src_matrix[3][3];
    new Float: dest_angle;
    new Float: dest_w;
    new Float: dest_x;
    new Float: dest_y;
    new Float: dest_z;
    new Float: dest_a;
    new Float: dest_b;
    new Float: dest_g;
    new Float: dest_matrix[3][3];

    for(new i, j; i < sizeof src_matrix; ++i) {
        for(j = 0; j < sizeof src_matrix[]; ++j) {
            src_matrix[i][j] = (random(200) - 100) / 100.0; 
        }
    }
    SetRotation(src, rtype_axis_angle, src_angle, src_x, src_y, src_z);
    ConvertRotation(src, rtype_quaternion, src); 
    GetRotation(src, rtype_axis_angle, src_angle, src_x, src_y, src_z);

    for(type = rotationtype: 0, comment = "rtype_axis_angle_"; type < rotationtype; ++type) {
        if(type != rtype_axis_angle) {
            dest = src;
    
            valstr(comment[17], _: type);
            ConvertRotation(dest, type, dest);
            GetRotation(dest, rtype_axis_angle, dest_angle, dest_x, dest_y, dest_z);
            ASSERT(CheckOrPrintAxisAngle(comment, src_angle, src_x, src_y, src_z, dest_angle, dest_x, dest_y, dest_z));
        }
    }
    for(new rotationtype: euler = rtype_euler_xzx; euler <= rtype_euler_samp; ++euler) {
        SetRotation(src, euler, src_a, src_b, src_g);
        ConvertRotation(src, rtype_quaternion, src); 
        GetRotation(src, euler, src_a, src_b, src_g);

        format(comment, sizeof comment, "rtype_euler_%02d_", _: euler);

        for(type = rotationtype: 0; type < rotationtype; ++type) {
            if(type != euler) {
                dest = src;

                valstr(comment[15], _: type);
                ConvertRotation(dest, type, dest);
                GetRotation(dest, euler, dest_a, dest_b, dest_g);
                ASSERT(CheckOrPrintEuler(comment, src_a, src_b, src_g, dest_a, dest_b, dest_g));
            }
        }
    }
    SetRotation(src, rtype_quaternion, src_w, src_x, src_y, src_z);
    ConvertRotation(src, rtype_axis_angle, src); 
    GetRotation(src, rtype_quaternion, src_w, src_x, src_y, src_z);

    for(type = rotationtype: 0, comment = "rtype_quaternion_"; type < rotationtype; ++type) {
        if(type != rtype_quaternion) {
            dest = src;

            valstr(comment[17], _: type);
            ConvertRotation(dest, type, dest);
            GetRotation(dest, rtype_quaternion, dest_w, dest_x, dest_y, dest_z);
            ASSERT(CheckOrPrintQuat(comment, src_w, src_x, src_y, src_z, dest_w, dest_x, dest_y, dest_z));
        }
    }
    SetRotation(src, rtype_rotation_matrix, src_matrix);
    ConvertRotation(src, rtype_axis_angle, src);
    GetRotation(src, rtype_rotation_matrix, src_matrix);

    for(type = rotationtype: 0, comment = "rtype_rotation_matrix_"; type < rotationtype; ++type) {
        if(type != rtype_rotation_matrix) {
            dest = src;

            valstr(comment[22], _: type);
            ConvertRotation(dest, type, dest);
            GetRotation(dest, rtype_rotation_matrix, dest_matrix);
            ASSERT(CheckOrPrintMatrix(comment, src_matrix, dest_matrix));
        }
    }
}

Test:RotatePoint() {
    new rotation[E_ROTATION];
    new Float: matrix[4][4];
    new comment[16] = "RotatePoint_";
    new Float: pX = (random(5000) - 2500) / 100.0; 
    new Float: pY = (random(5000) - 2500) / 100.0;
    new Float: pZ = (random(5000) - 2500) / 100.0;
    new Float: src_oX;
    new Float: src_oY;
    new Float: src_oZ;
    new Float: dest_oX;
    new Float: dest_oY;
    new Float: dest_oZ;
    // check if all different rotation function result in the same output
    SetRotation(rotation, rtype_euler_samp, random(36000) / 100.0, random(36000) / 100.0, random(36000) / 100.0);
    RotatePoint(rotation, 0.0, 0.0, 0.0, pX, pY, pZ, src_oX, src_oY, src_oZ);

    for(new rotationtype: type; type < rotationtype; ++type) {
        valstr(comment[12], _: type);
        ConvertRotation(rotation, type, rotation);
        RotatePoint(rotation, 0.0, 0.0, 0.0, pX, pY, pZ, dest_oX, dest_oY, dest_oZ);
        ASSERT(CheckOrPrintEuler(comment, src_oX, src_oY, src_oZ, dest_oX, dest_oY, dest_oZ));
    }
    // check if the rotation is actually correct
    SetRotation(rotation, rtype_axis_angle, 180.0, 0.0, 0.0, 1.0);
    RotatePoint(rotation, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, dest_oX, dest_oY, dest_oZ);

    ASSERT(CheckOrPrintEuler("rtype_axis_angle", dest_oX, dest_oY, dest_oZ, -1.0, 0.0, 0.0));

    SetRotation(rotation, rtype_euler_samp, 0.0, 0.0, 180.0);
    RotatePoint(rotation, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, dest_oX, dest_oY, dest_oZ);

    ASSERT(CheckOrPrintEuler("rtype_euler_samp", dest_oX, dest_oY, dest_oZ, -1.0, 2.0, 0.0));

    SetRotation(rotation, rtype_euler_zxy, 180.0, 0.0, 0.0);
    RotatePoint(rotation, 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, dest_oX, dest_oY, dest_oZ);

    ASSERT(CheckOrPrintEuler("rtype_euler_zxy", dest_oX, dest_oY, dest_oZ, -1.0, -2.0, 0.0));

    SetRotation(rotation, rtype_quaternion, 0.0, 0.0, 0.0, 1.0);
    RotatePoint(rotation, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, dest_oX, dest_oY, dest_oZ);

    ASSERT(CheckOrPrintEuler("rtype_quaternion", dest_oX, dest_oY, dest_oZ, -3.0, 0.0, 0.0));

    RotationMatrixZ(matrix, 180.0);
    SetRotation(rotation, rtype_rotation_matrix, matrix);
    RotatePoint(rotation, -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, dest_oX, dest_oY, dest_oZ);

    ASSERT(CheckOrPrintEuler("rtype_rotation_matrix", dest_oX, dest_oY, dest_oZ, -3.0, -2.0, 0.0));
}


Test:CombineRotation() {
    new Float: dest_oX;
    new Float: dest_oY;
    new Float: dest_oZ;
    new rot1[E_ROTATION];
    new rot2[E_ROTATION];
    new rotation[E_ROTATION];

    SetRotation(rot1, rtype_euler_zxz, 0.0, 0.0, 90.0);
    SetRotation(rot2, rtype_euler_zxz, 0.0, 90.0, 0.0);

    CombineRotation(rot2, rot1, rotation); // CombineQuat - global frame of reference = [second Rotation] * [first Rotation]
    RotatePoint(rotation, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, dest_oX, dest_oY, dest_oZ);

    ASSERT(CheckOrPrintEuler("CombineRotation global", dest_oX, dest_oY, dest_oZ, 0.0, 0.0, 1.0));

    ConvertRotation(rot2, rtype_rotation_matrix, rot2);
    CombineRotation(rot1, rot2, rotation); // CombineMatrix - local frame-of-reference = [first Rotation] * [second Rotation]
    RotatePoint(rotation, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, dest_oX, dest_oY, dest_oZ);

    ASSERT(CheckOrPrintEuler("CombineRotation local", dest_oX, dest_oY, dest_oZ, 0.0, 1.0, 0.0));
}

Test:ReverseRotation() {
    new rotation[E_ROTATION];
    new comment[32] = "ReverseRotation_";
    new Float: dest_oX = (random(5000) - 2500) / 100.0; 
    new Float: dest_oY = (random(5000) - 2500) / 100.0;
    new Float: dest_oZ = (random(5000) - 2500) / 100.0;

    SetRotation(rotation, rtype_euler_yxz, dest_oX, dest_oY, dest_oZ);

    for(new rotationtype: type; type < rotationtype; ++type) {
        valstr(comment[16], _: type);

        ConvertRotation(rotation, type, rotation);
        RotatePoint(rotation, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, dest_oX, dest_oY, dest_oZ);
        ReverseRotation(rotation, rotation);
        RotatePoint(rotation, 0.0, 0.0, 0.0, dest_oX, dest_oY, dest_oZ, dest_oX, dest_oY, dest_oZ);

        ASSERT(CheckOrPrintEuler(comment, dest_oX, dest_oY, dest_oZ, 1.0, 0.0, 0.0));
    }
}

#define EPSILON 0.001

bool: CheckOrPrintEuler(comment[], Float: x1, Float: y1, Float: z1, Float: x2, Float: y2, Float: z2) {
    new
        Float: x = x1 - x2,
        Float: y = y1 - y2,
        Float: z = z1 - z2
    ;
    while(x >= 360.0) x -= 360.0;
    while(y >= 360.0) y -= 360.0;
    while(z >= 360.0) z -= 360.0;
    while(x <= -EPSILON) x += 360.0;
    while(y <= -EPSILON) y += 360.0;
    while(z <= -EPSILON) z += 360.0;

    return (
        -EPSILON < x < EPSILON && -EPSILON < y < EPSILON && -EPSILON < z < EPSILON
    ) || printf("%s\nvec1 - %4.4f %4.4f %4.4f\nvec2 - %4.4f %4.4f %4.4f\ndiff - %4.4f %4.4f %4.4f", comment, x1, y1, z1, x2, y2, z2, x, y, z);
}

bool: CheckOrPrintMatrix(comment[], Float: matrix1[3][3], Float: matrix2[3][3]) {
    new
        Float: matrix[3][3]
    ;
    MatrixSub(matrix1, matrix2, matrix);

    return (
        -EPSILON < matrix[0][0] < EPSILON && -EPSILON < matrix[0][1] < EPSILON && -EPSILON < matrix[0][2] < EPSILON &&
        -EPSILON < matrix[1][0] < EPSILON && -EPSILON < matrix[1][1] < EPSILON && -EPSILON < matrix[1][2] < EPSILON &&
        -EPSILON < matrix[2][0] < EPSILON && -EPSILON < matrix[2][1] < EPSILON && -EPSILON < matrix[2][2] < EPSILON
    ) || printf("%s\n%4.4f %4.4f %4.4f\n%4.4f %4.4f %4.4f\n%4.4f %4.4f %4.4f", comment,
            matrix[0][0], matrix[0][1], matrix[0][2],
            matrix[1][0], matrix[1][1], matrix[1][2],
            matrix[2][0], matrix[2][1], matrix[2][2]
        )
    ;
}

bool: CheckOrPrintQuat(comment[], Float: w1, Float: x1, Float: y1, Float: z1, Float: w2, Float: x2, Float: y2, Float: z2) {
    new
        Float: w,
        Float: x,
        Float: y,
        Float: z
    ;
    if((w1 * w2) < 0.0) {
        w = w1 + w2;
        x = x1 + x2;
        y = y1 + y2;
        z = z1 + z2;
    } else {
        w = w1 - w2;
        x = x1 - x2;
        y = y1 - y2;
        z = z1 - z2;
    }
    return (
        -EPSILON < w < EPSILON && -EPSILON < x < EPSILON && -EPSILON < y < EPSILON && -EPSILON < z < EPSILON
    ) || printf("%s\nquat1 - %4.4f %4.4f %4.4f %4.4f\nquat2 - %4.4f %4.4f %4.4f %4.4f\n diff - %4.4f %4.4f %4.4f %4.4f", comment, w1, x1, y1, z1, w2, x2, y2, z2, w, x, y, z);
}

bool: CheckOrPrintAxisAngle(comment[], Float: a1, Float: x1, Float: y1, Float: z1, Float: a2, Float: x2, Float: y2, Float: z2) {
    new
        Float: a,
        Float: x,
        Float: y,
        Float: z
    ;
    if((x1 * x2) < 0.0) {
        a = 360.0 - a1 - a2;
        x = x1 + x2;
        y = y1 + y2;
        z = z1 + z2;
    } else {
        a = a1 - a2;
        x = x1 - x2;
        y = y1 - y2;
        z = z1 - z2;
    }
    return (
        -EPSILON < a < EPSILON && -EPSILON < x < EPSILON && -EPSILON < y < EPSILON && -EPSILON < z < EPSILON
    ) || printf("%s\naangle1 - %4.4f %4.4f %4.4f %4.4f\naangle2 - %4.4f %4.4f %4.4f %4.4f\n   diff - %4.4f %4.4f %4.4f %4.4f", comment, a1, x1, y1, z1, a2, x2, y2, z2, a, x, y, z);
}