#if defined _inc_rotation_extra_vehicle_
	#endinput
#else
    #define _inc_rotation_extra_vehicle_
#endif

#include "rotation_extra_object"

stock bool: GetVehicleAttachedPos(vehicleid, Float: attachX, Float: attachY, Float: attachZ, Float: attachRotX, Float: attachRotY, Float: attachRotZ, & Float: X, & Float: Y, & Float: Z, & Float: RotX, & Float: RotY, & Float: RotZ) {
    /// <summary>Get the position for the given attach offset</summary>
    /// <export>vehicle</export>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <param name="attachX">Attach offset x</param>
    /// <param name="attachY">Attach offset y</param>
    /// <param name="attachZ">Attach offset z</param>
    /// <param name="attachRotX">Attach rotation x</param>
    /// <param name="attachRotY">Attach rotation y</param>
    /// <param name="attachRotZ">Attach rotation z</param>
    /// <param name="X">Global position x</param>
    /// <param name="Y">Global position y</param>
    /// <param name="Z">Global position z</param>
    /// <param name="RotX">Global rotation x</param>
    /// <param name="RotY">Global rotation y</param>
    /// <param name="RotZ">Global rotation z</param>
    /// <returns>true for success otherwise false</returns>

    debug("GetVehicleAttachedPos(%d, %f, %f, %f, %f, %f, %f, ...)", vehicleid, attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ);

    if(GetVehiclePos(vehicleid, X, Y, Z)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        SetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);
        GetVehicleRotationQuat(vehicleid, attachRotZ, RotX, RotY, RotZ);
        SetRotation(parentRotation, rtype_quaternion, attachRotZ, RotX, RotY, RotZ);
        GetAttachedPos(X, Y, Z, parentRotation, attachX, attachY, attachZ, attachRotation, X, Y, Z, parentRotation);
        GetRotation(parentRotation, rtype_euler_samp, RotX, RotY, RotZ);

        return true;
    }
    return false;
}

stock bool: GetVehicleAttachedOffset(vehicleid, Float: X, Float: Y, Float: Z, Float: RotX, Float: RotY, Float: RotZ, & Float: attachX, & Float: attachY, & Float: attachZ, & Float: attachRotX, & Float: attachRotY, & Float: attachRotZ) {
    /// <summary>Get the attach offset for the given position</summary>
    /// <export>vehicle</export>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <param name="X">Global position x</param>
    /// <param name="Y">Global position y</param>
    /// <param name="Z">Global position z</param>
    /// <param name="RotX">Global rotation x</param>
    /// <param name="RotY">Global rotation y</param>
    /// <param name="RotZ">Global rotation z</param>
    /// <param name="attachX">Attach offset x</param>
    /// <param name="attachY">Attach offset y</param>
    /// <param name="attachZ">Attach offset z</param>
    /// <param name="attachRotX">Attach rotation x</param>
    /// <param name="attachRotY">Attach rotation y</param>
    /// <param name="attachRotZ">Attach rotation z</param>
    /// <returns>true for success otherwise false</returns>

    debug("GetVehicleAttachedOffset(%d, %f, %f, %f, %f, %f, %f, ...)", vehicleid, X, Y, Z, RotX, RotY, RotZ);

    if(GetVehiclePos(vehicleid, attachX, attachY, attachZ)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        SetRotation(attachRotation, rtype_euler_samp, RotX, RotY, RotZ);
        GetVehicleRotationQuat(vehicleid, attachRotZ, RotX, RotY, RotZ);
        SetRotation(parentRotation, rtype_quaternion, attachRotZ, RotX, RotY, RotZ);
        GetAttachedOffset(attachX, attachY, attachZ, parentRotation, X, Y, Z, attachRotation, attachX, attachY, attachZ, attachRotation);
        GetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);

        return true;
    }
    return false;
}

stock bool: AttachObjectToVehicleEx(objectid, vehicleid) {
    /// <summary>Attaches the objectid to vehicleid without changing position or rotation</summary>
    /// <export>vehicle</export>
    /// <param name="objectid">Objectid to attach</param>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <returns>true for success otherwise false</returns>

    debug("AttachObjectToVehicleEx(%d, %d)", objectid, vehicleid);

    new Float: X;
    new Float: Y;
    new Float: Z;

    if(GetObjectPos(objectid, X, Y, Z)) {
        new Float: rX;
        new Float: rY;
        new Float: rZ;

        GetObjectRot(objectid, rX, rY, rZ);

        if(GetVehicleAttachedOffset(vehicleid, X, Y, Z, rX, rY, rZ, X, Y, Z, rX, rY, rZ)) {
            AttachObjectToVehicle(objectid, vehicleid, X, Y, Z, rX, rY, rZ);

            return true;
        }
    }
    return false;
}

stock bool: AttachPlayerObjectToVehicleEx(playerid, objectid, vehicleid) {
    /// <summary>Attaches the objectid to vehicleid without changing position or rotation</summary>
    /// <export>vehicle</export>
    /// <param name="playerid">Playerid who owns the object</param>
    /// <param name="objectid">Objectid to attach</param>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <returns>true for success otherwise false</returns>

    debug("AttachPlayerObjectToVehicleEx(%d, %d)", objectid, vehicleid);

    new Float: X;
    new Float: Y;
    new Float: Z;

    if(GetPlayerObjectPos(playerid, objectid, X, Y, Z)) {
        new Float: rX;
        new Float: rY;
        new Float: rZ;

        GetPlayerObjectRot(playerid, objectid, rX, rY, rZ);

        if(GetVehicleAttachedOffset(vehicleid, X, Y, Z, rX, rY, rZ, X, Y, Z, rX, rY, rZ)) {
            AttachPlayerObjectToVehicle(playerid, objectid, vehicleid, X, Y, Z, rX, rY, rZ);

            return true;
        }
    }
    return false;
}

stock bool: GetVehicleRelativePos(vehicleid, Float: offsetX, Float: offsetY, Float: offsetZ, & Float: X, & Float: Y, & Float: Z) {
    /// <summary>Gets the relative position to the given vehicle</summary>
    /// <export>vehicle</export>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <param name="offsetX">Relative position x</param>
    /// <param name="offsetY">Relative position y</param>
    /// <param name="offsetZ">Relative position z</param>
    /// <param name="X">Global position x</param>
    /// <param name="Y">Global position y</param>
    /// <param name="Z">Global position z</param>
    /// <returns>true for success otherwise false</returns>

    debug("GetVehicleRelativePos(%d, %f, %f, %f, ...)", vehicleid, offsetX, offsetY, offsetZ);

    if(GetVehiclePos(vehicleid, X, Y, Z)) {
        new quat[E_QUAT];

        GetVehicleRotationQuat(vehicleid, quat[E_QUAT_W], quat[E_QUAT_X], quat[E_QUAT_Y], quat[E_QUAT_Z]);
        RotateQuat(quat, X, Y, Z, X + offsetX, Y + offsetY, Z + offsetZ, X, Y, Z);

        return true;
    }
    return false;
}

stock bool: GetVehicleForwardVector(vehicleid, & Float: X, & Float: Y, & Float: Z) {
    /// <summary>Gets the vehicle forward vector</summary>
    /// <export>vehicle</export>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <param name="X">Forward vector x</param>
    /// <param name="Y">Forward vector y</param>
    /// <param name="Z">Forward vector z</param>
    /// <returns>true for success otherwise false</returns>

    debug("GetVehicleForwardVector(%d, ...)", vehicleid);

    new Float: w;
    new Float: x;
    new Float: y;
    new Float: z;

    if(GetVehicleRotationQuat(vehicleid, w, x, y, z)) {
        X = 2.0 * (y * x + w * z);
        Y = 1.0 - 2.0 * (x * x + z * z);
        Z = 2.0 * (y * z - w * x);

        return true;
    }
    return false;
}

stock bool: GetVehicleRightVector(vehicleid, & Float: X, & Float: Y, & Float: Z) {
    /// <summary>Gets the vehicle forward vector</summary>
    /// <export>vehicle</export>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <param name="X">Right vector x</param>
    /// <param name="Y">Right vector y</param>
    /// <param name="Z">Right vector z</param>
    /// <returns>true for success otherwise false</returns>

    debug("GetVehicleRightVector(%d, ...)", vehicleid);

    new Float: w;
    new Float: x;
    new Float: y;
    new Float: z;

    if(GetVehicleRotationQuat(vehicleid, w, x, y, z)) {
        X = 1.0 - 2.0 * (y * y + z * z);
        Y = 2.0 * (x * y - z * w);
        Z = 2.0 * (x * z + y * w);

        return true;
    }
    return false;
}

stock bool: GetVehicleUpVector(vehicleid, & Float: X, & Float: Y, & Float: Z) {
    /// <summary>Gets the vehicle forward vector</summary>
    /// <export>vehicle</export>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <param name="X">Up vector x</param>
    /// <param name="Y">Up vector y</param>
    /// <param name="Z">Up vector z</param>
    /// <returns>true for success otherwise false</returns>

    debug("GetVehicleUpVector(%d, ...)", vehicleid);

    new Float: w;
    new Float: x;
    new Float: y;
    new Float: z;

    if(GetVehicleRotationQuat(vehicleid, w, x, y, z)) {
        X = 2.0 * (z * x - y * w);
        Y = 2.0 * (z * y + x * w);
        Z = 1.0 - 2.0 * (x * x + y * y);

        return true;
    }
    return false;
}