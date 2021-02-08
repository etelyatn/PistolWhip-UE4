// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

/** when you modify this, please note that this information can be saved with instances
* also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
#define COLLISION_WEAPON_TRACE  ECC_GameTraceChannel1

/** Current game mode type */
UENUM(BlueprintType)
enum class EGameModeType : uint8 
{
	GMT_VR,
    GMT_FP,

    GMT_Invalid
};