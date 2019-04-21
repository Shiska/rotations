/**
  * <summary>
  *     Misc functions which are not necessary for the main include but I will leave them here if anyone needs them
  * </summary>
  *//** */

#if defined _inc_rotation_misc_
	#endinput
#else
    #define _inc_rotation_misc_
#endif

#if defined DEBUG
    #define debug printf
#else
    #define debug(%0\10
#endif

#include "rotation_misc\rotation_misc_matrix"
#include "rotation_misc\rotation_misc_quaternion"

#undef debug