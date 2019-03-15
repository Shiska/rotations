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
    // TODO: test
    ReverseRotation(parentRotation, rotation);
    RotatePoint(rotation, parentX, parentY, parentZ, X, Y, Z, attachX, attachY, attachZ); // calculate position without rotation

    attachX -= parentX; // sub parent position, so we get the offsets
    attachY -= parentY;
    attachZ -= parentZ;

    CombineRotation(rotation, worldRotation, attachRotation); // combine worldRotation and reversed parentRotation
}