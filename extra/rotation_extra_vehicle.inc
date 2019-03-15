#if defined _inc_rotation_extra_vehicle_
	#endinput
#else
    #define _inc_rotation_extra_vehicle_
#endif

#include "rotation_extra_object"

stock bool: GetVehicleRotation(vehicleid, rotation[E_ROTATION]) {
    /// <summary>Get the vehicle rotation</summary>
    /// <export>vehicle</export>
    /// <param name="vehicleid">Vehicleid to get rotation from</param>
    /// <param name="rotation">Dest rotation array [<ref name="E_ROTATION"/>]</param>
    /// <returns>true for success otherwise false</returns>

    new Float: w;
    new Float: x;
    new Float: y;
    new Float: z;

    if(GetVehicleRotationQuat(vehicleid, w, x, y, z)) {
        SetRotation(rotation, rtype_quaternion, w, x, y, z);

        return true;
    }
    return false; 
}

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

    if(GetVehiclePos(vehicleid, X, Y, Z)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        GetVehicleRotation(vehicleid, parentRotation);
        SetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);
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

    if(GetVehiclePos(vehicleid, attachX, attachY, attachZ)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        GetVehicleRotation(vehicleid, parentRotation);
        SetRotation(attachRotation, rtype_euler_samp, RotX, RotY, RotZ);

        GetAttachedOffset(attachX, attachY, attachZ, parentRotation, X, Y, Z, attachRotation, attachX, attachY, attachZ, attachRotation);
        GetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);

        return true;
    }
    return false;
}

stock bool: AttachObjectToVehicleEx(vehicleid, objectid) {
    /// <summary>Attaches the objectid to vehicleid without changing position</summary>
    /// <export>vehicle</export>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <param name="objectid">Objectid to attach</param>
    /// <returns>true for success otherwise false</returns>

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

    if(GetVehiclePos(vehicleid, X, Y, Z)) {
        new rotation[E_ROTATION];

        GetVehicleRotation(vehicleid, rotation);
        RotatePoint(rotation, X, Y, Z, X + offsetX, Y + offsetY, Z + offsetZ, X, Y, Z);

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