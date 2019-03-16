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

    debug("GetDynamicObjectAttachedPos(%d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f)", objectid, attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ, X, Y, Z, RotX, RotY, RotZ);

    if(IsValidDynamicObject(objectid)) { // valid objectid
        new Float: attX;

        if(Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_OFFSET_X, attX)) { // objectid is attached
            new Float: attY;
            new Float: attZ;
            new Float: attRotX;
            new Float: attRotY;
            new Float: attRotZ;

            Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_OFFSET_Y, attY);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_OFFSET_Z, attZ);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_R_X, attRotX);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_R_Y, attRotY);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_R_Z, attRotZ);

            new attachedTo = Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACHED_OBJECT);

            if(attachedTo != INVALID_STREAMER_ID) { // attached to object
                attachedTo = GetDynamicObjectAttachedPos(attachedTo, attX, attY, attZ, attRotX, attRotY, attRotZ, X, Y, Z, RotX, RotY, RotZ);
            } else {
                attachedTo = Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACHED_VEHICLE);

                if(attachedTo != INVALID_VEHICLE_ID) { // attached to vehicle
                    attachedTo = GetVehicleAttachedPos(attachedTo, attX, attY, attZ, attRotX, attRotY, attRotZ, X, Y, Z, RotX, RotY, RotZ);
                } else { // if it wasn't attached to an vehicle or object but it is still an attached object than it must be attached to a player
                    attachedTo = Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACHED_PLAYER);
                    attachedTo = GetPlayerAttachedPos(attachedTo, attX, attY, attZ, attRotX, attRotY, attRotZ, X, Y, Z, RotX, RotY, RotZ);
                }
            }
            new parentRotation[E_ROTATION];
            new attachRotation[E_ROTATION];

            SetRotation(parentRotation, rtype_euler_samp, RotX, RotY, RotZ);
            SetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);
            GetAttachedPos(X, Y, Z, parentRotation, attachX, attachY, attachZ, attachRotation, X, Y, Z, parentRotation);
            GetRotation(parentRotation, rtype_euler_samp, RotX, RotY, RotZ);
        } else {
            new parentRotation[E_ROTATION];
            new attachRotation[E_ROTATION];

            GetDynamicObjectPos(objectid, X, Y, Z); 
            GetDynamicObjectRot(objectid, RotX, RotY, RotZ);
            SetRotation(parentRotation, rtype_euler_samp, RotX, RotY, RotZ);
            SetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);
            GetAttachedPos(X, Y, Z, parentRotation, attachX, attachY, attachZ, attachRotation, X, Y, Z, parentRotation);
            GetRotation(parentRotation, rtype_euler_samp, RotX, RotY, RotZ);
        }
        return true;
    }
    return false;
}

stock GetDynamicObjectPosRot(objectid, & Float: X, & Float: Y, & Float: Z, & Float: RotX, & Float: RotY, & Float: RotZ) {
    /// <summary>Get the object global position and rotation, even if attached</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Target objectid</param>
    /// <param name="X">Global position x</param>
    /// <param name="Y">Global position y</param>
    /// <param name="Z">Global position z</param>
    /// <param name="RotX">Global rotation x</param>
    /// <param name="RotY">Global rotation y</param>
    /// <param name="RotZ">Global rotation z</param>
    /// <returns>true for success otherwise false</returns>

    debug("GetDynamicObjectPosRot(%d, ...)", objectid);

    if(IsValidDynamicObject(objectid)) {
        if(Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_ATTACH_OFFSET_X, X)) { // objectid is attached to something
            return GetDynamicObjectAttachedPos(objectid, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, X, Y, Z, RotX, RotY, RotZ);
        }
        return GetDynamicObjectPos(objectid, X, Y, Z) && GetDynamicObjectRot(objectid, RotX, RotY, RotZ);
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

    debug("GetDynamicObjectAttachedOffset(%d, %f, %f, %f, %f, %f, %f, ...)", objectid, X, Y, Z, RotX, RotY, RotZ);

    if(GetDynamicObjectPosRot(objectid, attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        SetRotation(parentRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);
        SetRotation(attachRotation, rtype_euler_samp, RotX, RotY, RotZ);
        GetAttachedOffset(attachX, attachY, attachZ, parentRotation, X, Y, Z, attachRotation, attachX, attachY, attachZ, attachRotation);
        GetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);

        return true;
    }
    return false;
}

stock DetachDynamicObject(oldobjectid, & newobjectid) {
    /// <summary>Destroyes the dynamic objectid and recreates it at the correct position</summary>
    /// <export>dynamic</export>
    /// <param name="oldobjectid">Dynamic objectid to destroy</param>
    /// <param name="newobjectid">New objectid if successful</param>
    /// <returns>true for success otherwise false</returns>

    debug("DetachDynamicObject(%d, ...)", oldobjectid);

    new Float: attachX;

    if(Streamer_GetFloatData(STREAMER_TYPE_OBJECT, oldobjectid, E_STREAMER_ATTACH_OFFSET_X, attachX)) { // object is attached to something
        new Float: attachY;
        new Float: attachZ;
        new Float: attachRotX;
        new Float: attachRotY;
        new Float: attachRotZ;

        if(GetDynamicObjectPosRot(oldobjectid, attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ)) {
            new Float: streamdistance;
            new Float: drawdistance;

            Streamer_GetFloatData(STREAMER_TYPE_OBJECT, oldobjectid, E_STREAMER_STREAM_DISTANCE, streamdistance);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT, oldobjectid, E_STREAMER_DRAW_DISTANCE, drawdistance);

            newobjectid = CreateDynamicObject(
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, oldobjectid, E_STREAMER_MODEL_ID),
                attachX, attachY, attachZ, attachRotX, attachRotY, attachRotZ,
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, oldobjectid, E_STREAMER_WORLD_ID),
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, oldobjectid, E_STREAMER_INTERIOR_ID),
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, oldobjectid, E_STREAMER_PLAYER_ID),
                streamdistance, drawdistance,
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, oldobjectid, E_STREAMER_AREA_ID),
                Streamer_GetIntData(STREAMER_TYPE_OBJECT, oldobjectid, E_STREAMER_PRIORITY)
            );
            DestroyDynamicObject(oldobjectid);

            return true;
        }
    }
    return false;
}

stock bool: AttachDynamicObjectToObjectEx(objectid, attachtoid, syncrotation = 1) {
    /// <summary>Attaches the dynamic objectid to attachtoid without changing position</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Dynamic objectid to attach</param>
    /// <param name="attachtoid">Target objectid</param>
    /// <param name="syncrotation">Synchronize rotation</param>
    /// <returns>true for success otherwise false</returns>

    debug("AttachDynamicObjectToObjectEx(%d, %d, %d)", objectid, attachtoid, syncrotation);

    new Float: X;
    new Float: Y;
    new Float: Z;
    new Float: rX;
    new Float: rY;
    new Float: rZ;

    if(GetDynamicObjectPosRot(objectid, X, Y, Z, rX, rY, rZ) && GetDynamicObjectAttachedOffset(attachtoid, X, Y, Z, rX, rY, rZ, X, Y, Z, rX, rY, rZ)) {
        AttachDynamicObjectToObject(objectid, attachtoid, X, Y, Z, rX, rY, rZ, syncrotation);

        return true;
    }
    return false;
}

stock bool: AttachDynamicObjectToPlayerEx(objectid, playerid) {
    /// <summary>Attaches the dynamic objectid to playerid without changing position</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Dynamic objectid to attach</param>
    /// <param name="playerid">Target playerid</param>
    /// <returns>true for success otherwise false</returns>

    debug("AttachDynamicObjectToPlayerEx(%d, %d)", objectid, playerid);

    new Float: X;
    new Float: Y;
    new Float: Z;
    new Float: rX;
    new Float: rY;
    new Float: rZ;

    if(GetDynamicObjectPosRot(objectid, X, Y, Z, rX, rY, rZ) && GetPlayerAttachedOffset(playerid, X, Y, Z, rX, rY, rZ, X, Y, Z, rX, rY, rZ)) {
        AttachDynamicObjectToPlayer(objectid, playerid, X, Y, Z, rX, rY, rZ);

        return true;
    }
    return false;
}

stock bool: AttachDynamicObjectToVehicleEx(objectid, vehicleid) {
    /// <summary>Attaches the dynamic objectid to vehicleid without changing position</summary>
    /// <export>dynamic</export>
    /// <param name="objectid">Dynamic objectid to attach</param>
    /// <param name="vehicleid">Target vehicleid</param>
    /// <returns>true for success otherwise false</returns>

    debug("AttachDynamicObjectToVehicleEx(%d, %d)", objectid, vehicleid);

    new Float: X;
    new Float: Y;
    new Float: Z;
    new Float: rX;
    new Float: rY;
    new Float: rZ;

    if(GetDynamicObjectPosRot(objectid, X, Y, Z, rX, rY, rZ) && GetVehicleAttachedOffset(vehicleid, X, Y, Z, rX, rY, rZ, X, Y, Z, rX, rY, rZ)) {
        AttachDynamicObjectToVehicle(objectid, vehicleid, X, Y, Z, rX, rY, rZ);

        return true;
    }
    return false;
}