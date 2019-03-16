/**
  * <summary>
  *   Extra / additional functions which apply rotations
  * </summary>
  *//** */

#if defined _inc_rotation_extra_
	#endinput
#else
    #define _inc_rotation_extra_
#endif

#include "rotation"

#if defined DEBUG
    #define debug printf
#else
    #define debug(%0\10
#endif

#include "extra\rotation_extra_object"
#include "extra\rotation_extra_player"
#include "extra\rotation_extra_vehicle"
#include "extra\rotation_extra_dynamic"

#undef debug