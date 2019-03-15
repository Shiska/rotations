/**
  * <summary>
  *   Allows conversion between different rotations
  * </summary>
  *
  * <section>Contributer</section>
  *
  * <ul>
  *   <li>Nero_3D</li>
  * </ul>
  *
  * <section>History</section>
  *
  * <ul>
  *   <li>
  *     <subsection>1.0 (14.08.2016)</subsection>
  *     <ul>
  *       <li>Release</li>
  *     </ul>
  *   </li>
  *   <li>
  *     <subsection>1.1 (07.01.2017)</subsection>
  *     <ul>
  *       <li>euler_default renamed to euler_samp</li>
  *       <li>Translation function added for Rotation Matrix</li>
  *       <li>additional translate parameter 'oT' in MatrixRotate</li>
  *     </ul>
  *   </li>
  *   <li>
  *     <subsection>1.2 (11.08.2017)</subsection>
  *     <ul>
  *       <li>GetVehicleObjectPositionWorld and GetVehicleObjectPositionWorld added</li>
  *     </ul>
  *   </li>
  *   <li>
  *     <subsection>2.0 (xx.xx.2019) - incompatible with version 1.x</subsection>
  *     <ul>
  *       <li>All functions rewritten with arrays instead of multiple variables</li>
  *       <li>Set, get, reverse, normalize function added</li>
  *       <li>Documentation added and report files generated</li>
  *     </ul>
  *   </li>
  * </ul>
  *
  * <section>Reference</section>
  *
  * <ul>
  *   <li><a target="_blank" href="https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix"/></li>
  *   <li><a target="_blank" href="https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation"/></li>
  *   <li><a target="_blank" href="https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation"/></li>
  *   <li><a target="_blank" href="http://www.euclideanspace.com/maths/geometry/rotations/index.htm"/></li>
  *   <li><a target="_blank" href="https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles"/></li>
  * </ul>
  *//** */

#if defined _inc_rotation_
	#endinput
#else
    #define _inc_rotation_
#endif

#if defined DEBUG
    #define debug printf
#else
    #define debug(%0\10
#endif

enum rotationtype {
    rtype_axis_angle,
    rtype_euler_xzx, // Proper / Classic Euler angles
    rtype_euler_xyx, // Euler angle have three angles a(lpha), b(eta) and g(amma)
    rtype_euler_yxy, // there are two ways to calculate the final position
    rtype_euler_yzy, // Right to left - global frame-of-reference
    rtype_euler_zyz, // Left to right - Local frame-of-reference
    rtype_euler_zxz,
    rtype_euler_xzy, // Tait-Bryan angles
    rtype_euler_xyz,
    rtype_euler_re_xyz = rtype_euler_xyz, // rtype_euler_xyz is already in fitting order, so we hook it
    rtype_euler_yxz, // the angles are applied in order, also in this case yxz it is, y = alpha, x = beta and z = gamma
    rtype_euler_yzx,
    rtype_euler_zyx,
    rtype_euler_zxy, // yaw pitch roll (for vehicle / object facing y direction)
    rtype_euler_re_xzy, // Tait-Brayan angles rearranged, so alpha = x rotation, beta = y rotation and gamma = z rotation
    rtype_euler_re_yxz, // just for easier usage / samp notation but with additional overheat
    rtype_euler_re_yzx,
    rtype_euler_re_zyx,
    rtype_euler_re_zxy,
    rtype_euler_samp = rtype_euler_re_zxy, // object rX rY rZ, pitch roll yaw, same as rtype_euler_zxy with swapped order
    rtype_quaternion,
    rtype_rotation_matrix
}
/// <summary>All supported rotation types</summary>
/// <export/>

static stock __rotationtype[rotationtype]; // necessary for report file, it doesn't detect "tag only" usage

#include "rotation\rotation_axis_angle"
#include "rotation\rotation_euler"
#include "rotation\rotation_matrix"
#include "rotation\rotation_quaternion"

// get largest rotation type, obviously it will be E_RMATRIX
const __largestType = _: E_AANGLE;

#if _: E_EULER > __largestType
    #undef __largestType

    const __largestType = _: E_EULER;
#endif

#if _: E_QUAT > __largestType
    #error test
    #undef __largestType

    const __largestType = _: E_QUAT;
#endif

#if _: E_RMATRIX > __largestType
    #undef __largestType

    const __largestType = _: E_RMATRIX;
#endif

enum E_ROTATION {
    // rotation: E_ROTATION_BASE, // needs to be the first entry, for operator, not implemented but possible
    rotationtype: E_ROTATION_TYPE,
    E_ROTATION_DATA[__largestType]
}
/// <summary>Rotation array</summary>
/// <export/>

#undef __largestType

const E_ROTATION_DATA_OFFSET = _: E_ROTATION_DATA << 2;

// native SetRotation(rotation[E_ROTATION], rotationtype: type, Float: ...);
stock SetRotation(rotation[E_ROTATION], rotationtype: type, Float: ...) {
    /// <summary>Set the rotation array from the given rotation</summary>
    /// <export/>
    /// <param name="rotation">Rotation array [<ref name="E_ROTATION"/>]</param>
    /// <param name="type">Source rotation type</param>
    /// <param name="...">Source rotation</param>
    /// <example>
    ///     SetRotation(rotation, rtype_axis_angle, w, x, y, z);<br/>
    ///     SetRotation(rotation, rtype_euler_samp, x, y, z);<br/>
    ///     SetRotation(rotation, rtype_quaternion, w, x, y, z);<br/>
    ///     SetRotation(rotation, rtype_rotation_matrix, matrix);
    /// </example>

    debug("SetRotation(%d)", _: type);
    // store address in E_ROTATION_BASE
    #emit load.s.pri rotation
    #emit sref.s.pri rotation
    // store rotation type
    rotation[E_ROTATION_TYPE] = type;
    // move rotation to data section
    #emit load.s.pri rotation
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri rotation

    switch(type) {
        case rtype_axis_angle:{
            #emit const.pri SetRotationFromAxisAngle
        }
        case rtype_quaternion: {
            #emit const.pri SetRotationFromQuat
        }
        case rtype_rotation_matrix: {
            #emit const.pri SetRotationFromMatrix
        }
        default: { // Euler
            #emit const.pri SetRotationFromEuler
        }
    } {}
    #emit add.c 4
    #emit sctrl 6
}

// native ConvertRotation(src[E_ROTATION], rotationtype: type, dest[E_ROTATION]);
stock ConvertRotation(src[E_ROTATION], rotationtype: type, dest[E_ROTATION]) {
    /// <summary>Convert the given rotation matrix into the target rotation</summary>
    /// <export/>
    /// <param name="src">Source rotation array</param>
    /// <param name="type">Dest rotation type</param>
    /// <param name="dest">Dest rotation array</param>

    debug("ConvertRotation(%d)", _: type);

    if(src[E_ROTATION_TYPE] == type) {
        dest = src;
    } else {
        static rotationtype: types[2];
        // stores src and dest type beforehand, it could be overwritten if src and dest are the same array
        types[0] = src[E_ROTATION_TYPE];
        types[1] = dest[E_ROTATION_TYPE] = type;
        // find corresponding function
        switch(type) {
            case rtype_axis_angle: {
                switch(types[0]) {
                    case rtype_quaternion: {
                        #emit push.c ConvertQuatToAxisAngle
                    }
                    case rtype_rotation_matrix: {
                        #emit push.c ConvertMatrixToAxisAngle
                    }
                    default: { // Euler
                        #emit push.c ConvertEulerToAxisAngle
                    }
                }
            }
            case rtype_quaternion: {
                switch(types[0]) {
                    case rtype_axis_angle: {
                        #emit push.c ConvertAxisAngleToQuat
                    }
                    case rtype_rotation_matrix: {
                        #emit push.c ConvertMatrixToQuat
                    }
                    default: { // Euler
                        #emit push.c ConvertEulerToQuat
                    }
                }
            }
            case rtype_rotation_matrix: {
                switch(types[0]) {
                    case rtype_axis_angle: {
                        #emit push.c ConvertAxisAngleToMatrix
                    }
                    case rtype_quaternion: {
                        #emit push.c ConvertQuatToMatrix
                    }
                    default: { // Euler
                        #emit push.c ConvertEulerToMatrix
                    }
                }
            }
            default: { // Euler
                switch(types[0]) {
                    case rtype_axis_angle: {
                        #emit push.c ConvertAxisAngleToEuler
                    }
                    case rtype_quaternion: {
                        #emit push.c ConvertQuatToEuler
                    }
                    case rtype_rotation_matrix: {
                        #emit push.c ConvertMatrixToEuler
                    }
                    default: { // Euler
                        #emit push.c ConvertEulerToEuler
                    }
                }
            }
        } {}
        // move src to data section
        #emit load.s.pri src
        #emit add.c E_ROTATION_DATA_OFFSET
        #emit stor.s.pri src
        // replace type address
        #emit const.pri types
        #emit stor.s.pri type
        // move dest to data section
        #emit load.s.pri dest
        #emit add.c E_ROTATION_DATA_OFFSET
        #emit stor.s.pri dest
        // load function from switch
        #emit pop.pri
        #emit add.c 4
        #emit sctrl 6
    }
}

// native CombineRotation(left[E_ROTATION], right[E_ROTATION], dest[E_ROTATION]);
stock CombineRotation(left[E_ROTATION], right[E_ROTATION], dest[E_ROTATION]) {
    /// <summary>Combines two rotations, left * right = dest</summary>
    /// <export/>
    /// <param name="left">Left rotation [<ref name="E_AANGLE"/>]</param>
    /// <param name="right">Right rotation [<ref name="E_AANGLE"/>]</param>
    /// <param name="dest">Dest rotation [<ref name="E_AANGLE"/>]</param>
    /// <remarks>If <paramref name="left"/> or <paramref name="right"/> is a rotation matrix than <paramref name="left"/>, <paramref name="right"/> and <paramref name="dest"/> will be converted to rotation matrices, in all other cases <paramref name="left"/>, <paramref name="right"/> and <paramref name="dest"/> will be quaternion</remarks>
    /// <remarks>[second Rotation] * [first Rotation] = global frame of reference - global coordinate system / axis stay the same for the second rotation</remarks>
    /// <remarks>[first Rotation] * [second Rotation] = local frame-of-reference - coordinate system / axis rotates with first rotation</remarks>

    debug("CombineRotation(...)");

    if(left[E_ROTATION_TYPE] == rtype_rotation_matrix) {
        if(right[E_ROTATION_TYPE] != rtype_rotation_matrix) {
            ConvertRotation(right, rtype_rotation_matrix, right);
        } {}
        dest[E_ROTATION_TYPE] = rtype_rotation_matrix;

        #emit const.alt CombineMatrix
    } else if(right[E_ROTATION_TYPE] == rtype_rotation_matrix) {
        ConvertRotation(left, rtype_rotation_matrix, left);

        dest[E_ROTATION_TYPE] = rtype_rotation_matrix;

        #emit const.alt CombineMatrix
    } else {
        ConvertRotation(left, rtype_quaternion, left);
        ConvertRotation(right, rtype_quaternion, right);
        // you can't combine axis angle or euler angles, so we convert everything else to quaternion
        dest[E_ROTATION_TYPE] = rtype_quaternion;

        #emit const.alt CombineQuat
    } {}
    #emit load.s.pri left
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri left

    #emit load.s.pri right
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri right

    #emit load.s.pri dest
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri dest

    #emit move.pri
    #emit add.c 4
    #emit sctrl 6
}

// native GetRotation(rotation[E_ROTATION], rotationtype: type, Float: ...);
stock GetRotation(rotation[E_ROTATION], rotationtype: type, Float: ...) {
    /// <summary>Get the rotation from the given rotation array</summary>
    /// <export/>
    /// <param name="rotation">Rotation array [<ref name="E_ROTATION"/>]</param>
    /// <param name="type">Dest rotation type</param>
    /// <param name="...">Dest rotation</param>

    debug("GetRotation(%d)", _: type);

    static tmp[E_ROTATION];

    ConvertRotation(rotation, type, tmp);
    // replace rotation array with converted one
    #emit const.pri tmp
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri rotation

    switch(type) {
        case rtype_axis_angle:{
            #emit const.pri GetAxisAngleFromRotation
        }
        case rtype_quaternion: {
            #emit const.pri GetQuatFromRotation
        }
        case rtype_rotation_matrix: {
            #emit const.pri GetMatrixFromRotation
        }
        default: { // Euler
            #emit const.pri GetEulerFromRotation
        }
    } {}
    #emit add.c 4
    #emit sctrl 6
}

// native RotatePoint(rotation[E_ROTATION], Float: cX, Float: cY, Float: cZ, Float: pX, Float: pY, Float: pZ, & Float: oX, & Float: oY, & Float: oZ);
stock RotatePoint(rotation[E_ROTATION], Float: cX, Float: cY, Float: cZ, Float: pX, Float: pY, Float: pZ, & Float: oX, & Float: oY, & Float: oZ) {
    /// <summary>Rotates the given point (pX, pY, pZ) around origin (cX, cY, cZ) and stores the output position (oX, oY, oZ)</summary>
    /// <export/>
    /// <param name="rotation">Rotation array [<ref name="E_ROTATION"/>]</param>
    /// <param name="cX">Rotation origin x</param>
    /// <param name="cY">Rotation origin y</param>
    /// <param name="cZ">Rotation origin z</param>
    /// <param name="pX">Point x</param>
    /// <param name="pY">Point y</param>
    /// <param name="pZ">Point z</param>
    /// <param name="oX">Output position x</param>
    /// <param name="oY">Output position y</param>
    /// <param name="oZ">Output position z</param>

    debug("RotatePoint(%f, %f, %f)", pX, pY, pZ);

    pX -= (oX = cX);
    pY -= (oY = cY);
    pZ -= (oZ = cZ);

    switch(rotation[E_ROTATION_TYPE]) {
        case rtype_axis_angle: {
            #emit const.alt RotateAxisAngle
        }
        case rtype_quaternion: {
            #emit const.alt RotateQuat
        }
        case rtype_rotation_matrix: {
            #emit const.alt RotateMatrix
        }
        default: { // Euler
            ConvertRotation(rotation, rtype_quaternion, rotation);
            // saddly I couldn't find a way to rotate euler angles without a step in between, is there even a way?
            // usually they are converted to rotation matrixes or quaternion and than rotated
            #emit const.alt RotateQuat
        }
    } {}
    #emit load.s.pri rotation
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri rotation

    #emit move.pri
    #emit add.c 4
    #emit sctrl 6
}

// native ReverseRotation(src[E_ROTATION], dest[E_ROTATION]);
stock ReverseRotation(src[E_ROTATION], dest[E_ROTATION]) {
    /// <summary>Stores the reverse rotation in dest</summary>
    /// <export/>
    /// <param name="src">Source rotation [<ref name="E_ROTATION"/>]</param>
    /// <param name="dest">Dest rotation [<ref name="E_ROTATION"/>]</param>

    debug("ReverseRotation(...)");

    switch(src[E_ROTATION_TYPE]) {
        case rtype_axis_angle: {
            dest[E_ROTATION_TYPE] = rtype_axis_angle;

            #emit const.alt ReverseAxisAngle
        }
        case rtype_quaternion: {
            dest[E_ROTATION_TYPE] = rtype_quaternion;

            #emit const.alt ReverseQuat
        }
        case rtype_rotation_matrix: {
            dest[E_ROTATION_TYPE] = rtype_rotation_matrix;

            #emit const.alt ReverseMatrix
        }
        default: { // Euler
            switch(src[E_ROTATION_TYPE]) { // basically order of rotation
                case rtype_euler_xzx: dest[E_ROTATION_TYPE] = rtype_euler_xzx;
                case rtype_euler_xyx: dest[E_ROTATION_TYPE] = rtype_euler_xyx;
                case rtype_euler_yxy: dest[E_ROTATION_TYPE] = rtype_euler_yxy;
                case rtype_euler_yzy: dest[E_ROTATION_TYPE] = rtype_euler_yzy;
                case rtype_euler_zyz: dest[E_ROTATION_TYPE] = rtype_euler_zyz;
                case rtype_euler_zxz: dest[E_ROTATION_TYPE] = rtype_euler_zxz;
                case rtype_euler_xzy: dest[E_ROTATION_TYPE] = rtype_euler_yzx;
                case rtype_euler_xyz: dest[E_ROTATION_TYPE] = rtype_euler_zyx;
                case rtype_euler_yxz: dest[E_ROTATION_TYPE] = rtype_euler_zxy;
                case rtype_euler_yzx: dest[E_ROTATION_TYPE] = rtype_euler_xzy;
                case rtype_euler_zyx: dest[E_ROTATION_TYPE] = rtype_euler_xyz;
                case rtype_euler_zxy: dest[E_ROTATION_TYPE] = rtype_euler_yxz;
                case rtype_euler_re_xzy: dest[E_ROTATION_TYPE] = rtype_euler_re_yzx;
                case rtype_euler_re_yxz: dest[E_ROTATION_TYPE] = rtype_euler_re_zxy;
                case rtype_euler_re_yzx: dest[E_ROTATION_TYPE] = rtype_euler_re_xzy;
                case rtype_euler_re_zyx: dest[E_ROTATION_TYPE] = rtype_euler_re_xyz;
                case rtype_euler_re_zxy: dest[E_ROTATION_TYPE] = rtype_euler_re_yxz;
            } {} // and reverse angles
            #emit const.alt ReverseEuler
        }
    } {}
    #emit load.s.pri src
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri src

    #emit load.s.pri dest
    #emit add.c E_ROTATION_DATA_OFFSET
    #emit stor.s.pri dest

    #emit move.pri
    #emit add.c 4
    #emit sctrl 6
}

#undef E_ROTATION_DATA_OFFSET
#undef debug