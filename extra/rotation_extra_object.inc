#if defined _inc_rotation_extra_object_
	#endinput
#else
    #define _inc_rotation_extra_object_
#endif

stock GetAttachedPos(Float: parentX, Float: parentY, Float: parentZ, parentRotation[E_ROTATION], Float: attachX, Float: attachY, Float: attachZ, attachRotation[E_ROTATION], & Float: X, & Float: Y, & Float: Z, worldRotation[E_ROTATION]) {
    /// <summary>Get the position for the given attach offset</summary>
    /// <export>object</export>
    /// <param name="parentX">Parent position x</param>
    /// <param name="parentY">Parent position y</param>
    /// <param name="parentZ">Parent position z</param>
    /// <param name="parentRotation">Parent rotation [<ref name="E_ROTATION"/>]</param>
    /// <param name="attachX">Attach offset x</param>
    /// <param name="attachY">Attach offset y</param>
    /// <param name="attachZ">Attach offset z</param>
    /// <param name="attachRotation">Attach rotation [<ref name="E_ROTATION"/>]</param>
    /// <param name="X">Global position x</param>
    /// <param name="Y">Global position y</param>
    /// <param name="Z">Global position z</param>
    /// <param name="worldRotation">Global rotation [<ref name="E_ROTATION"/>]</param>
    /// <returns>true for success otherwise false</returns>

    CombineRotation(parentRotation, attachRotation, worldRotation); // TODO: test
    RotatePoint(parentRotation, parentX, parentY, parentZ, parentX + attachX, parentY + attachY, parentZ + attachZ, X, Y, Z);
}

stock GetAttachedOffset(Float: parentX, Float: parentY, Float: parentZ, parentRotation[E_ROTATION], Float: X, Float: Y, Float: Z, worldRotation[E_ROTATION], & Float: attachX, & Float: attachY, & Float: attachZ, attachRotation[E_ROTATION]) {
    /// <summary>Get the attach offset for the given position</summary>
    /// <export>object</export>
    /// <param name="parentX">Parent position x</param>
    /// <param name="parentY">Parent position y</param>
    /// <param name="parentZ">Parent position z</param>
    /// <param name="parentRotation">Parent rotation [<ref name="E_ROTATION"/>]</param>
    /// <param name="X">Global position x</param>
    /// <param name="Y">Global position y</param>
    /// <param name="Z">Global position z</param>
    /// <param name="worldRotation">Global rotation [<ref name="E_ROTATION"/>]</param>
    /// <param name="attachX">Attach offset x</param>
    /// <param name="attachY">Attach offset y</param>
    /// <param name="attachZ">Attach offset z</param>
    /// <param name="attachRotation">Attach rotation [<ref name="E_ROTATION"/>]</param>
    /// <returns>true for success otherwise false</returns>

    new rotation[E_ROTATION];

    ReverseRotation(parentRotation, rotation);
    RotatePoint(rotation, parentX, parentY, parentZ, X, Y, Z, attachX, attachY, attachZ); // calculate position without rotation

    attachX -= parentX; // sub parent position, so we get the offsets
    attachY -= parentY;
    attachZ -= parentZ;

    CombineRotation(rotation, worldRotation, attachRotation); // combine worldRotation and reversed parentRotation
}

stock bool: GetObjectRotation(objectid, rotation[E_ROTATION]) {
    /// <summary>Get the object rotation</summary>
    /// <export>object</export>
    /// <param name="objectid">Objectid to get rotation from</param>
    /// <param name="rotation">Dest rotation array [<ref name="E_ROTATION"/>]</param>
    /// <returns>true for success otherwise false</returns>

    new Float: rX;
    new Float: rY;
    new Float: rZ;

    if(GetObjectRot(objectid, rX, rY, rZ)) {
        SetRotation(rotation, rtype_euler_samp, rX, rY, rZ);

        return true;
    }
    return false; 
}

stock bool: SetObjectRotation(objectid, rotation[E_ROTATION]) {
    /// <summary>Set the object rotation</summary>
    /// <export>object</export>
    /// <param name="objectid">Objectid to set rotation</param>
    /// <param name="rotation">Source rotation array [<ref name="E_ROTATION"/>]</param>
    /// <returns>true for success otherwise false</returns>

    new Float: rX;
    new Float: rY;
    new Float: rZ;

    GetRotation(rotation, rtype_euler_samp, rX, rY, rZ);

    return bool: SetObjectRot(objectid, rX, rY, rZ);
}

stock bool: GetObjectAttachedPos(objectid, Float: attachX, Float: attachY, Float: attachZ, Float: attachRotX, Float: attachRotY, Float: attachRotZ, & Float: X, & Float: Y, & Float: Z, & Float: RotX, & Float: RotY, & Float: RotZ) {
    /// <summary>Get the position for the given attach offset</summary>
    /// <export>object</export>
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

    if(GetObjectPos(objectid, X, Y, Z)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        GetObjectRotation(objectid, parentRotation);
        SetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);
        GetAttachedPos(X, Y, Z, parentRotation, attachX, attachY, attachZ, attachRotation, X, Y, Z, parentRotation);
        GetRotation(parentRotation, rtype_euler_samp, RotX, RotY, RotZ);

        return true;
    }
    return false;
}

stock bool: GetObjectAttachedOffset(objectid, Float: X, Float: Y, Float: Z, Float: RotX, Float: RotY, Float: RotZ, & Float: attachX, & Float: attachY, & Float: attachZ, & Float: attachRotX, & Float: attachRotY, & Float: attachRotZ) {
    /// <summary>Get the attach offset for the given position</summary>
    /// <export>object</export>
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

    if(GetObjectPos(objectid, attachX, attachY, attachZ)) {
        new parentRotation[E_ROTATION];
        new attachRotation[E_ROTATION];

        GetObjectRotation(objectid, parentRotation);
        SetRotation(attachRotation, rtype_euler_samp, RotX, RotY, RotZ);
        GetAttachedOffset(attachX, attachY, attachZ, parentRotation, X, Y, Z, attachRotation, attachX, attachY, attachZ, attachRotation);
        GetRotation(attachRotation, rtype_euler_samp, attachRotX, attachRotY, attachRotZ);

        return true;
    }
    return false;
}

stock bool: AttachObjectToObjectEx(objectid, attachtoid, syncrotation = 1) {
    /// <summary>Attaches the objectid to attachtoid without changing position or rotation</summary>
    /// <export>object</export>
    /// <param name="objectid">Objectid to attach</param>
    /// <param name="attachtoid">Target objectid</param>
    /// <param name="syncrotation">Synchronize rotation</param>
    /// <returns>true for success otherwise false</returns>

    new Float: X;
    new Float: Y;
    new Float: Z;

    if(GetObjectPos(objectid, X, Y, Z)) {
        new Float: rX;
        new Float: rY;
        new Float: rZ;

        GetObjectRot(objectid, rX, rY, rZ);

        if(GetObjectAttachedOffset(attachtoid, X, Y, Z, rX, rY, rZ, X, Y, Z, rX, rY, rZ)) {
            AttachObjectToObject(objectid, attachtoid, X, Y, Z, rX, rY, rZ, syncrotation);

            return true;
        }
    }
    return false;
}