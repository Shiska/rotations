#if defined _inc_rotation_extra_player_
	#endinput
#else
    #define _inc_rotation_extra_player_
#endif

#include "rotation_extra_object"

stock bool: GetPlayerRotation(playerid, rotation[E_ROTATION]) {
    /// <summary>Get the player rotation</summary>
    /// <export>player</export>
    /// <param name="playerid">Playerid to get rotation from</param>
    /// <param name="rotation">Dest rotation array [<ref name="E_ROTATION"/>]</param>
    /// <remarks>Only gets the z angle</remarks>
    /// <returns>true for success otherwise false</returns>

    new Float: rZ;

    if(GetPlayerFacingAngle(playerid, rZ)) {
        new Float: zero;

        SetRotation(rotation, rtype_euler_samp, zero, zero, rZ);

        return true;
    }
    return false; 
}

stock bool: SetPlayerRotation(playerid, rotation[E_ROTATION]) {
    /// <summary>Set the player rotation</summary>
    /// <export>player</export>
    /// <param name="playerid">Playerid to set rotation</param>
    /// <param name="rotation">Source rotation array [<ref name="E_ROTATION"/>]</param>
    /// <remarks>Only sets the z angle</remarks>
    /// <returns>true for success otherwise false</returns>

    new Float: rX;
    new Float: rY;
    new Float: rZ;

    GetRotation(rotation, rtype_euler_samp, rX, rY, rZ);

    return bool: SetPlayerFacingAngle(playerid, rZ);
}

stock bool: GetPlayerAttachedPos(playerid, Float: attachX, Float: attachY, Float: attachZ, Float: attachRotX, Float: attachRotY, Float: attachRotZ, & Float: X, & Float: Y, & Float: Z, & Float: RotX, & Float: RotY, & Float: RotZ) {
    /// <summary>Get the position for the given attach offset</summary>
    /// <export>player</export>
    /// <param name="playerid">Target playerid</param>
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

    if(GetPlayerPos(playerid, X, Y, Z)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        GetPlayerRotation(playerid, parentRotation);
        SetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);
        GetAttachedPos(X, Y, Z, parentRotation, attachX, attachY, attachZ, attachRotation, X, Y, Z, parentRotation);
        GetRotation(parentRotation, rtype_euler_samp, RotX, RotY, RotZ);

        return true;
    }
    return false;
}

stock bool: GetPlayerAttachedOffset(playerid, Float: X, Float: Y, Float: Z, Float: RotX, Float: RotY, Float: RotZ, & Float: attachX, & Float: attachY, & Float: attachZ, & Float: attachRotX, & Float: attachRotY, & Float: attachRotZ) {
    /// <summary>Get the attach offset for the given position</summary>
    /// <export>player</export>
    /// <param name="playerid">Target playerid</param>
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

    if(GetPlayerPos(playerid, attachX, attachY, attachZ)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        GetPlayerRotation(playerid, parentRotation);
        SetRotation(attachRotation, rtype_euler_samp, RotX, RotY, RotZ);
        GetAttachedOffset(attachX, attachY, attachZ, parentRotation, X, Y, Z, attachRotation, attachX, attachY, attachZ, attachRotation);
        GetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);

        return true;
    }
    return false;
}

stock bool: AttachObjectToPlayerEx(objectid, playerid) {
    /// <summary>Attaches the objectid to playerid without changing position or rotation</summary>
    /// <export>player</export>
    /// <param name="objectid">Objectid to attach</param>
    /// <param name="playerid">Target playerid</param>
    /// <returns>true for success otherwise false</returns>

    new Float: X;
    new Float: Y;
    new Float: Z;

    if(GetObjectPos(objectid, X, Y, Z)) {
        new Float: rX;
        new Float: rY;
        new Float: rZ;

        GetObjectRot(objectid, rX, rY, rZ);

        if(GetPlayerAttachedOffset(playerid, X, Y, Z, rX, rY, rZ, X, Y, Z, rX, rY, rZ)) {
            AttachObjectToPlayer(objectid, playerid, X, Y, Z, rX, rY, rZ);

            return true;
        }
    }
    return false;
}