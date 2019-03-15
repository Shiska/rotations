#if !defined _streamer_included
	#endinput  // https://github.com/samp-incognito/samp-streamer-plugin not included
#endif

#if defined _inc_rotation_extra_dynamic_
	#endinput
#else
    #define _inc_rotation_extra_dynamic_
#endif

#include "rotation_extra_object"
#include "rotation_extra_player"
#include "rotation_extra_vehicle"

stock bool: GetDynamicObjectRotation(objectid, rotation[E_ROTATION]) {
    /// <summary>Get the object rotation</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Objectid to get rotation from</param>
    /// <param name="rotation">Dest rotation array [<ref name="E_ROTATION"/>]</param>
    /// <returns>true for success otherwise false</returns>

    new Float: rX;
    new Float: rY;
    new Float: rZ;

    if(GetDynamicObjectRot(objectid, rX, rY, rZ)) {
        SetRotation(rotation, rtype_euler_samp, rX, rY, rZ);

        return true;
    }
    return false; 
}

stock bool: SetDynamicObjectRotation(objectid, rotation[E_ROTATION]) {
    /// <summary>Set the object rotation</summary>
    /// <export>object</export>
    /// <param name="objectid">Objectid to get rotation</param>
    /// <param name="rotation">Source rotation array [<ref name="E_ROTATION"/>]</param>
    /// <returns>true for success otherwise false</returns>

    new Float: rX;
    new Float: rY;
    new Float: rZ;

    GetRotation(rotation, rtype_euler_samp, rX, rY, rZ);

    return bool: SetDynamicObjectRot(objectid, rX, rY, rZ);
}

stock bool: GetDynamicObjectAttachedPos(objectid, Float: attachX, Float: attachY, Float: attachZ, Float: attachRotX, Float: attachRotY, Float: attachRotZ, & Float: X, & Float: Y, & Float: Z, & Float: RotX, & Float: RotY, & Float: RotZ) {
    /// <summary>Get the position for the given attach offset</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Target objectid</param>
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

    if(GetDynamicObjectPos(objectid, X, Y, Z)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        GetDynamicObjectRotation(objectid, parentRotation);
        SetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);
        GetAttachedPos(X, Y, Z, parentRotation, attachX, attachY, attachZ, attachRotation, X, Y, Z, parentRotation);
        GetRotation(parentRotation, rtype_euler_samp, RotX, RotY, RotZ);

        return true;
    }
    return false;
}

stock bool: GetDynamicObjectAttachedOffset(objectid, Float: X, Float: Y, Float: Z, Float: RotX, Float: RotY, Float: RotZ, & Float: attachX, & Float: attachY, & Float: attachZ, & Float: attachRotX, & Float: attachRotY, & Float: attachRotZ) {
    /// <summary>Get the attach offset for the given position</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Target objectid</param>
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

    if(GetDynamicObjectPos(objectid, attachX, attachY, attachZ)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        GetDynamicObjectRotation(objectid, parentRotation);
        SetRotation(attachRotation, rtype_euler_samp, RotX, RotY, RotZ);
        GetAttachedOffset(attachX, attachY, attachZ, parentRotation, X, Y, Z, attachRotation, attachX, attachY, attachZ, attachRotation);
        GetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);

        return true;
    }
    return false;
}

stock DetachDynamicObject(objectid) {
    /// <summary>Destroyes the dynamic objectid and recreates it at the correct position</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Dynamic objectid to destroy</param>
    /// <returns>The new objectid for success otherwise INVALID_OBJECT_ID</returns>

    new Float: attachX;

    if(Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_OFFSET_X, attachX)) {
        new Float: attachY;
        new Float: attachZ;
        new Float: attachRotX;
        new Float: attachRotY;
        new Float: attachRotZ;

        Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_OFFSET_Y, attachY);
        Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_OFFSET_Z, attachZ);
        Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_R_X, attachRotX);
        Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_R_Y, attachRotY);
        Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_R_Z, attachRotZ);

        new attachedTo = Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACHED_VEHICLE);

        if(attachedTo) { // attached to vehicle
            attachedTo = GetVehicleAttachedPos(attachedTo, attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ, attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ);
        } else {
            attachedTo = Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACHED_OBJECT);

            if(attachedTo) { // attached to object
                attachedTo = GetDynamicObjectAttachedPos(attachedTo, attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ, attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ);
            } else { // if it wasn't attached to an vehicle or object but it is still an attached object than it must be attached to a player
                attachedTo = Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACHED_PLAYER);
                attachedTo = GetPlayerAttachedPos(attachedTo, attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ, attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ);
            }
        }
        if(attachedTo) {
            new Float: streamdistance;
            new Float: drawdistance;

            Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_STREAM_DISTANCE, streamdistance);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_DRAW_DISTANCE, drawdistance);

            attachedTo = CreateDynamicObject(
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_MODEL_ID),
                attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ,
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_WORLD_ID),
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_INTERIOR_ID),
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_PLAYER_ID),
                streamdistance, drawdistance,
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_AREA_ID),
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_PRIORITY)
            );
            DestroyDynamicObject(objectid);

            return attachedTo;
        }
    }
    return INVALID_OBJECT_ID;
}

stock bool: AttachDynamicObjectToObjectEx(objectid, attachtoid, syncrotation = 1) {
    /// <summary>Attaches the dynamic objectid to attachtoid without changing position</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Dynamic objectid to attach</param>
    /// <param name="attachtoid">Target objectid</param>
    /// <param name="syncrotation">Synchronize rotation</param>
    /// <returns>true for success otherwise false</returns>

    new Float: X;
    new Float: Y;
    new Float: Z;

    if(GetDynamicObjectPos(objectid, X, Y, Z)) {
        new Float: rX;
        new Float: rY;
        new Float: rZ;

        GetDynamicObjectRot(objectid, rX, rY, rZ);

        if(GetDynamicObjectAttachedOffset(attachtoid, X, Y, Z, rX, rY, rZ, X, Y, Z, rX, rY, rZ)) {
            AttachDynamicObjectToObject(objectid, attachtoid, X, Y, Z, rX, rY, rZ, syncrotation);

            return true;
        }
    }
    return false;
}

stock bool: AttachDynamicObjectToPlayerEx(objectid, playerid) {
    /// <summary>Attaches the dynamic objectid to playerid without changing position</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Dynamic objectid to attach</param>
    /// <param name="playerid">Target playerid</param>
    /// <returns>true for success otherwise false</returns>

    new Float: X;
    new Float: Y;
    new Float: Z;

    if(GetDynamicObjectPos(objectid, X, Y, Z)) {
        new Float: rX;
        new Float: rY;
        new Float: rZ;

        GetDynamicObjectRot(objectid, rX, rY, rZ);

        if(GetPlayerAttachedOffset(playerid, X, Y, Z, rX, rY, rZ, X, Y, Z, rX, rY, rZ)) {
            AttachDynamicObjectToPlayer(objectid, playerid, X, Y, Z, rX, rY, rZ);

            return true;
        }
    }
    return false;
}

stock bool: AttachDynamicObjectToVehicleEx(objectid, vehicleid) {
    /// <summary>Attaches the dynamic objectid to vehicleid without changing position</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Dynamic objectid to attach</param>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <returns>true for success otherwise false</returns>

    new Float: X;
    new Float: Y;
    new Float: Z;

    if(GetDynamicObjectPos(objectid, X, Y, Z)) {
        new Float: rX;
        new Float: rY;
        new Float: rZ;

        GetDynamicObjectRot(objectid, rX, rY, rZ);

        if(GetVehicleAttachedOffset(vehicleid, X, Y, Z, rX, rY, rZ, X, Y, Z, rX, rY, rZ)) {
            AttachDynamicObjectToVehicle(objectid, vehicleid, X, Y, Z, rX, rY, rZ);

            return true;
        }
    }
    return false;
}