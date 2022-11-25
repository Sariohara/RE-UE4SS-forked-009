#define NOMINMAX

#include <Windows.h>

#ifdef TEXT
#undef TEXT
#endif

#include <cwctype>
#include <limits>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <format>

#include <UE4SSProgram.hpp>
#include <build_number.hpp>
#include <Signatures.hpp>
#include <Helpers/ASM.hpp>
#include <Helpers/String.hpp>
#include <Unreal/TypeChecker.hpp>
#include <Helpers/Format.hpp>
#include <Helpers/Integer.hpp>
#include <UnrealDef.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <GUI/GUI.hpp>
#include <GUI/LiveView.hpp>
#include <GUI/ConsoleOutputDevice.hpp>
#include <Timer/ScopedTimer.hpp>
#include <Timer/FunctionTimer.hpp>
#include <SigScanner/SinglePassSigScanner.hpp>
#include <Mod.hpp>
#include <LuaType/LuaUObject.hpp>
#include <LuaType/LuaCustomProperty.hpp>
#include <LuaLibrary.hpp>
#include <SDKGenerator/Generator.hpp>
#include <SDKGenerator/UEHeaderGenerator.hpp>
#include <ExceptionHandling.hpp>
#include <ObjectDumper/ObjectToString.hpp>
#include <IniParser/Ini.hpp>
#include <Unreal/GameplayStatics.hpp>
#include <Unreal/World.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/Searcher/ObjectSearcher.hpp>
#include <Unreal/UPackage.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/UObjectArray.hpp>

//#include <LuaBindings/States/MainState/Main.hpp>
//#include <LuaBindings/LuaSetup.hpp>

// LuaLate params: LuaState, ClassOrFreeFunction, FullyQualifiedClassOrFunctionName, ImportIntoFullyQualifiedScope (optional)
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::GlobalClassTest, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::MyFirstClass, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC2::MyFirstClass, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC2::RC3::RC4::MyFirstClass, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FOutputDevice, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FFrame, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FString, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FName, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::UObjectBase, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::UObjectBaseUtility, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::UObject, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::UStruct, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::UClass, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::UField, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::UWorld, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::UScriptStruct, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FVector, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FRotator, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FQuat, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FObjectInstancingGraph, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::TMulticastScriptDelegate, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FFeedbackContext, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::TScriptDelegate, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::ITargetPlatform, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FOutParmRec, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::UFunction, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FGuid, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FScriptArray, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FTransform, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FUObjectItem, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FWeakObjectPtr, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FText, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::UEnum, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::ObjectSearcher, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FField, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FFieldVariant, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FFieldClass, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FFieldClassVariant, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FObjectPropertyBase, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FObjectProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaAddBaseToClass(::RC::Unreal::TFObjectPropertyBase, ::RC::Unreal::FObjectPropertyBase)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FLazyObjectProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FSoftObjectProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FWeakObjectProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FClassProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FSoftClassProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FByteProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FInt8Property, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FInt16Property, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FIntProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FUInt16Property, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FUInt32Property, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FUInt64Property, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FFloatProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FDoubleProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaAddBaseToClass(::RC::Unreal::TProperty_Numeric, ::RC::Unreal::FNumericProperty)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FArrayProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FBoolProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FDelegateProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FEnumProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FFieldPathProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FInterfaceProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FMapProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FMulticastDelegateProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FMulticastInlineDelegateProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FMulticastSparseDelegateProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FNameProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FNumericProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FSetProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FStrProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FStructProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FTextProperty, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::UnrealRuntimeTypes::ArrayTest, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::UnrealRuntimeTypes::LuaUScriptStruct, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::Version, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::GlobalFunctionTest, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::GetWorldTest, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::GetMyFirstClass, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::GetMyFirstClass2, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::GetMyFirstClass3, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::Unreal::UObjectGlobals::FindFirstOf, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::Unreal::UObjectGlobals::ForEachUObject, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::Unreal::UObjectGlobals::FindAllOf, ::RC::lua_FindAllOf_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::RegisterKeyBind, ::RC::lua_RegisterKeyBind_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::warn, ::RC::lua_warn_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::print, ::RC::lua_print_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */

// There is no non-templated 'StaticFindObject' function and that should change at some point.
// For now we're using 'FindObject' instead which is what the templated 'StaticFindObject' variants wrap.
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::Unreal::UObjectGlobals::FindObject, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */

// LuaMemberFunctionRedirector params: Class, FunctionName, FullyQualifiedLuaWrapper
// Injecting a completely new member function into a struct.
/** CUSTOM_ATTRIBUTE[LuaMemberFunctionRedirector(::RC::Unreal::UObjectBase, MyTestFunc, ::RC::UObjectBase_memberr_function_wrapper_MyTestFunc)] */

// Redirecting an existing member function in a struct to your own wrapper.
// Primarily used for injecting custom code before or after the original function.
// The code for the original function won't be generated so you'll have to rewrite it or copy before you override the original.
/** CUSTOM_ATTRIBUTE[LuaMemberFunctionRedirector(::RC::Unreal::UObjectBase, GetNamePrivate, ::RC::UObjectBase_member_function_wrapper_GetNamePrivate)] */
/** CUSTOM_ATTRIBUTE[LuaMemberFunctionRedirector(::RC::Unreal::UObjectBase, Cast, ::RC::UObjectBase_member_function_wrapper_Cast)] */
/** CUSTOM_ATTRIBUTE[LuaMemberFunctionRedirector(::RC::Unreal::FField, CastField, ::RC::FField_member_function_wrapper_CastField)] */
/** CUSTOM_ATTRIBUTE[LuaMemberFunctionRedirector(::RC::Unreal::UEnum, ForEachName, ::RC::UEnum_member_function_wrapper_ForEachName)] */
/** CUSTOM_ATTRIBUTE[LuaMemberFunctionRedirector(::RC::UnrealRuntimeTypes::ArrayTest, GetElementAtIndex, ::RC::ArrayTest_member_function_wrapper_GetElementAtIndex)] */
/** CUSTOM_ATTRIBUTE[LuaMemberFunctionRedirector(::RC::UnrealRuntimeTypes::ArrayTest, ForEach, ::RC::ArrayTest_member_function_wrapper_ForEach)] */
/** CUSTOM_ATTRIBUTE[LuaStaticMemberFunctionRedirector(::RC::Unreal::UClass, StaticClass, ::RC::UClass_member_function_wrapper_StaticClass)] */

// LuaAddMetamethod params: FullyQualifiedClass, MetamethodName, FullyQualifiedLuaWrapper
// Adding metamethods to a struct.
// It doesn't account for future inheritance, meaning that if you add a metamethod to 'Derived', it won't get triggered from 'Base'.
// It accounts for past inheritance, meaning that if you add a metamethod to 'Base', it will get triggered from 'Derived' if 'Derived' doesn't have its own.
/** CUSTOM_ATTRIBUTE[LuaAddMetamethod(::RC::Unreal::UObjectBase, __index, ::RC::UObjectBase_metamethod_wrapper_Index)] */
/** CUSTOM_ATTRIBUTE[LuaAddMetamethod(::RC::Unreal::UObjectBase, __newindex, ::RC::UObjectBase_metamethod_wrapper_NewIndex)] */
/** CUSTOM_ATTRIBUTE[LuaAddMetamethod(::RC::Unreal::UFunction, __call, ::RC::UFunction_metamethod_wrapper_Call)] */
/** CUSTOM_ATTRIBUTE[LuaAddMetamethod(::RC::UnrealRuntimeTypes::ArrayTest, __gc, ::RC::ArrayTest_metamethod_wrapper_GC)] */
/** CUSTOM_ATTRIBUTE[LuaAddMetamethod(::RC::UnrealRuntimeTypes::LuaUScriptStruct, __index, ::RC::LuaUScriptStruct_metamethod_wrapper_Index)] */
/** CUSTOM_ATTRIBUTE[LuaAddMetamethod(::RC::UnrealRuntimeTypes::LuaUScriptStruct, __newindex, ::RC::LuaUScriptStruct_metamethod_wrapper_NewIndex)] */
/** CUSTOM_ATTRIBUTE[LuaAddMetamethod(::RC::UnrealRuntimeTypes::LuaUScriptStruct, __gc, ::RC::LuaUScriptStruct_metamethod_wrapper_GC)] */

/** CUSTOM_ATTRIBUTE[LuaMapTemplateClass(::RC::Unreal::TArray, ::RC::UnrealRuntimeTypes::Array)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::GetArrayTest, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::GetArrayTest2, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::GetArrayTest3, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */

// Low-level memory
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::Unreal::FMemory, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Class, ::RC::MemoryItem, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::AllocateMemory, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::FreeMemory, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */

/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::ReadBytes, ::RC::lua_Test_ReadBytes_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::ReadUInt8, ::RC::lua_Test_ReadUInt8_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::ReadUInt16, ::RC::lua_Test_ReadUInt16_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::ReadUInt32, ::RC::lua_Test_ReadUInt32_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::ReadUInt64, ::RC::lua_Test_ReadUInt64_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::ReadInt8, ::RC::lua_Test_ReadInt8_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::ReadInt16, ::RC::lua_Test_ReadInt16_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::ReadInt32, ::RC::lua_Test_ReadInt32_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::ReadInt64, ::RC::lua_Test_ReadInt64_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::WriteBytes, ::RC::lua_Test_WriteBytes_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::WriteUInt8, ::RC::lua_Test_WriteUInt8_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::WriteUInt16, ::RC::lua_Test_WriteUInt16_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::WriteUInt32, ::RC::lua_Test_WriteUInt32_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::WriteUInt64, ::RC::lua_Test_WriteUInt64_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::WriteInt8, ::RC::lua_Test_WriteInt8_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::WriteInt16, ::RC::lua_Test_WriteInt16_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::WriteInt32, ::RC::lua_Test_WriteInt32_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::WriteInt64, ::RC::lua_Test_WriteInt64_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */

// CE aliases
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::readBytes, ::RC::lua_Test_ReadBytes_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::readByte, ::RC::lua_Test_ReadUInt8_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::readSmallInteger, ::RC::lua_Test_ReadUInt16_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::readInteger, ::RC::lua_Test_ReadUInt32_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::readQword, ::RC::lua_Test_ReadUInt64_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::readPointer, ::RC::lua_Test_ReadUInt64_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::writeBytes, ::RC::lua_Test_WriteBytes_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::writeByte, ::RC::lua_Test_WriteUInt8_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::writeSmallInteger, ::RC::lua_Test_WriteUInt16_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::writeInteger, ::RC::lua_Test_WriteUInt32_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::writeQword, ::RC::lua_Test_WriteUInt64_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::writePointer, ::RC::lua_Test_WriteUInt64_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */

/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::Test_GetUnsignedMemorySetup, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::Test_GetSignedMemorySetup, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::Test_GetPlayerControllerVTablePointer, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::Test_Get_UObject_Nullptr, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */

/** CUSTOM_ATTRIBUTE[LuaLate(Enum, ::RC::LoopAction, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Enum, ::RC::Unreal::EObjectFlags, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Enum, ::RC::Input::Key, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(Enum, ::RC::Input::ModifierKey, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */

// UE4SS 1.3 compatibility
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::LuaBackCompat::StaticFindObject, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(CustomFreeFunction, ::RC::LuaBackCompat::RegisterHook, ::RC::LuaBackCompat::lua_RegisterHook_wrapper, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaLate(FreeFunction, ::RC::LuaBackCompat::NotifyOnNewObject, ::), LuaStateTypes(MainState, AsyncState, AOBState)] */
/** CUSTOM_ATTRIBUTE[LuaMemberFunctionRedirector(::RC::Unreal::UObjectBase, IsA, ::RC::LuaBackCompat::lua_UObjectBase_IsA_wrapper)] */

auto GlobalFunctionTest() -> GlobalClassTest
{
    printf_s("GlobalFunctionTest\n");
    return GlobalClassTest{};
}

namespace RC
{
    auto GetPlayerControllerTest2() -> Unreal::UObjectBase*
    {
        return UObjectGlobals::FindFirstOf(STR("PlayerController"));
    }
    auto GetPlayerControllerTest3() -> Unreal::UObjectBaseUtility*
    {
        return UObjectGlobals::FindFirstOf(STR("PlayerController"));
    }
    auto GetMyFirstClass() -> MyFirstClass
    {
        return MyFirstClass{};
    }
    auto GetMyFirstClass2() -> ::RC2::MyFirstClass
    {
        return ::RC2::MyFirstClass{};
    }
    auto GetMyFirstClass3() -> ::RC2::RC3::RC4::MyFirstClass
    {
        return ::RC2::RC3::RC4::MyFirstClass{};
    }

    // Commented out because this system (turn off hotkeys when in-game console is open) it doesn't work properly.
    /*
    struct RC_UE_API FUEDeathListener : public Unreal::FUObjectCreateListener
    {
        static FUEDeathListener UEDeathListener;

        void NotifyUObjectCreated(const Unreal::UObjectBase* object, int32_t index) override {}
        void OnUObjectArrayShutdown() override
        {
            UE4SSProgram::unreal_is_shutting_down = true;
            Unreal::UObjectArray::RemoveUObjectCreateListener(this);
        }
    };
    FUEDeathListener FUEDeathListener::UEDeathListener{};

    auto get_player_controller() -> UObject*
    {
        std::vector<Unreal::UObject*> player_controllers{};
        UObjectGlobals::FindAllOf(STR("PlayerController"), player_controllers);
        if (!player_controllers.empty())
        {
            return player_controllers.back();
        }
        else
        {
            return nullptr;
        }
    }
    //*/

    SettingsManager UE4SSProgram::settings_manager{};

#define OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(StructName) \
    for (const auto&[name, offset] : Unreal::StructName::MemberOffsets) \
    {                                      \
        Output::send(STR(#StructName "::{} = 0x{:X}\n"), name, offset); \
    }

    auto output_all_member_offsets() -> void
    {
        Output::send(STR("\n##### MEMBER OFFSETS START #####\n\n"));
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(UObjectBase);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(UScriptStruct);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(UScriptStruct::ICppStructOps);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FField);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FOutputDevice);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FEnumProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(UStruct);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(UFunction);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(UField);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(UWorld);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(UClass);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(UEnum);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FObjectPropertyBase);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FDelegateProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FMulticastDelegateProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FSetProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FStructProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FArrayProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FMapProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FBoolProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FByteProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FClassProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FSoftClassProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FInterfaceProperty);
        OUTPUT_MEMBER_OFFSETS_FOR_STRUCT(FFieldPathProperty);
        Output::send(STR("\n##### MEMBER OFFSETS END #####\n\n"));
    }

    UE4SSProgram::UE4SSProgram(const std::wstring& moduleFilePath, std::initializer_list<BinaryOptions> options) : MProgram(options)
    {
        TIME_FUNCTION()

        s_program = this;

        try
        {
            setup_paths(moduleFilePath);

            try
            {
                settings_manager.deserialize(m_settings_path_and_file);
            }
            catch (std::exception& e)
            {
                create_emergency_console_for_early_error(std::format(STR("The IniParser failed to parse: {}"), to_wstring(e.what())));
                return;
            }

            // Setup the log file
            auto& file_device = Output::set_default_devices<Output::NewFileDevice>();
            //auto& file_device = Output::get_device<Output::NewFileDevice>();
            file_device.set_file_name_and_path(m_log_directory / m_log_file_name);

            m_simple_console_enabled = settings_manager.Debug.SimpleConsoleEnabled;
            m_debug_console_enabled = settings_manager.Debug.DebugConsoleEnabled;
            m_debug_console_visible = settings_manager.Debug.DebugConsoleVisible;

            create_debug_console();
            setup_output_devices();

            // This is experimental code that's here only for future reference
            /*
            Unreal::UnrealInitializer::SetupUnrealModules();

            constexpr const wchar_t* str_to_find = STR("Allocator: %s");
            void* string_address = SinglePassScanner::string_scan(str_to_find, ScanTarget::Core);
            Output::send(STR("\n\nFound string '{}' at {}\n\n"), std::wstring_view{str_to_find}, string_address);
            //*/

            Output::send(STR("Console created\n"));
            Output::send(STR("UE4SS - v{}.{}.{}{}{} - Build #{}\n"),
                         UE4SS_LIB_VERSION_MAJOR,
                         UE4SS_LIB_VERSION_MINOR,
                         UE4SS_LIB_VERSION_HOTFIX,
                         std::format(L"{}", UE4SS_LIB_VERSION_PRERELEASE == 0 ? L"" : std::format(L" PreRelease #{}", UE4SS_LIB_VERSION_PRERELEASE)),
                         std::format(L"{}", UE4SS_LIB_BETA_STARTED == 0 ? L"" : (UE4SS_LIB_IS_BETA == 0 ? L" Beta #?" : std::format(L" Beta #{}", UE4SS_LIB_VERSION_BETA))),
                         UE4SS_LIB_BUILD_NUMBER);
#ifdef WITH_CASE_PRESERVING_NAME
            Output::send(STR("WITH_CASE_PRESERVING_NAME: Yes\n\n"));
#else
            Output::send(STR("WITH_CASE_PRESERVING_NAME: No\n\n"));
#endif

            setup_mod_directory_path();

            if (m_has_game_specific_config)
            {
                Output::send(STR("Found configuration for game: {}\n"), m_mods_directory.parent_path().filename().c_str());
            }
            else
            {
                Output::send(STR("No specific game configuration found, using default configuration file\n"));
            }

            Output::send(STR("Config: {}\n\n"), m_settings_path_and_file.c_str());
            Output::send(STR("root directory: {}\n"), m_root_directory.c_str());
            Output::send(STR("working directory: {}\n"), m_working_directory.c_str());
            Output::send(STR("game executable directory: {}\n"), m_game_executable_directory.c_str());
            Output::send(STR("mods directory: {}\n"), m_mods_directory.c_str());
            Output::send(STR("log directory: {}\n"), m_log_directory.c_str());
            Output::send(STR("object dumper directory: {}\n\n\n"), m_object_dumper_output_directory.c_str());

            setup_unreal();
            setup_unreal_properties();
            UAssetRegistry::SetMaxMemoryUsageDuringAssetLoading(settings_manager.Memory.MaxMemoryUsageDuringAssetLoading);

            output_all_member_offsets();

            share_lua_functions();

            // Only deal with the event loop thread here if the 'Test' constructor doesn't need to be called
#ifndef RUN_TESTS
            // Program is now fully setup
            // Start event loop
            m_event_loop = std::jthread{&UE4SSProgram::update, this};

            // Wait for thread
            // There's a loop inside the thread that only exits when you hit the 'End' key on the keyboard
            // As long as you don't do that the thread will stay open and accept further inputs
            m_event_loop.join();
#endif
        }
        catch (std::runtime_error& e)
        {
            // Returns to main from here which checks, displays & handles whether to close the program or not
            // If has_error() returns false that means that set_error was not called
            // In that case we need to copy the exception message to the error buffer before we return to main
            if (!m_error_object->has_error())
            {
                copy_error_into_message(e.what());

            }
            return;
        }
    }

    UE4SSProgram::~UE4SSProgram()
    {
        // Shut down the event loop
        m_processing_events = false;

        // It's possible that main() will destroy the default devices (they are static)
        // However it's also possible that this program object is constructed in a context where main() is not gonna immediately exit
        // Because of that and because the default devices are created in the constructor, it's preferred to explicitly close all default devices in the destructor
        Output::close_all_default_devices();
    }

    auto UE4SSProgram::setup_paths(const std::wstring& moduleFilePathString) -> void
    {
        const std::filesystem::path moduleFilePath = std::filesystem::path(moduleFilePathString);
        m_root_directory = moduleFilePath.parent_path().wstring();
        m_module_file_path = moduleFilePath.wstring();

        // The default working directory is the root directory
        // Can be changed by creating a <GameName> directory in the root directory
        // At that point, the working directory will be "root/<GameName>"
        m_working_directory = m_root_directory;

        // Default file to open if there is no game specific config
        m_default_settings_path_and_file = m_root_directory / m_settings_file_name;

        wchar_t exe_path_buffer[1024];
        GetModuleFileNameW(GetModuleHandle(nullptr), exe_path_buffer, 1023);
        std::filesystem::path game_exe_path = exe_path_buffer;
        std::filesystem::path game_directory_path = game_exe_path.parent_path();
        m_working_directory = m_root_directory;
        m_mods_directory = m_working_directory / "Mods";
        m_game_executable_directory = game_directory_path/*game_exe_path.parent_path()*/;
        m_settings_path_and_file = m_root_directory;
        m_game_path_and_exe_name = game_exe_path;
        m_object_dumper_output_directory = m_game_executable_directory;

        bool has_game_specific_config{};
        for (const auto& item : std::filesystem::directory_iterator(m_root_directory))
        {
            if (!item.is_directory()) { continue; }

            if (item.path().filename() == game_directory_path.parent_path().parent_path().parent_path().filename())
            {
                m_has_game_specific_config = true;
                m_working_directory = item.path();
                m_mods_directory = item.path().wstring() + L"\\Mods";
                m_settings_path_and_file = std::move(item.path());
                m_log_directory = m_working_directory;
                m_object_dumper_output_directory = m_working_directory;
                break;
            }
        }

        m_log_directory = m_working_directory;
        m_settings_path_and_file.append(m_settings_file_name);
    }

    auto UE4SSProgram::create_emergency_console_for_early_error(File::StringViewType error_message) -> void
    {
        m_simple_console_enabled = true;
        create_debug_console();
        printf_s("%S\n", error_message.data());
    }

    auto UE4SSProgram::setup_mod_directory_path() -> void
    {
        // Mods folder path, typically '<m_working_directory>\Mods'
        // Can be customized via UE4SS-settings.ini
        if (settings_manager.Overrides.ModsFolderPath.empty())
        {
            m_mods_directory = m_mods_directory.empty() ? m_working_directory / "Mods" : m_mods_directory;
        }
        else
        {
            m_mods_directory = settings_manager.Overrides.ModsFolderPath;
        }
    }

    auto UE4SSProgram::create_debug_console() -> void
    {
        if (m_simple_console_enabled)
        {
            if (AllocConsole())
            {
                FILE* stdin_filename;
                FILE* stdout_filename;
                FILE* stderr_filename;
                freopen_s(&stdin_filename, "CONIN$", "r", stdin);
                freopen_s(&stdout_filename, "CONOUT$", "w", stdout);
                freopen_s(&stderr_filename, "CONOUT$", "w", stderr);
            }

        }
        if (m_debug_console_enabled && m_debug_console_visible)
        {
            m_render_thread = std::jthread{ &GUI::gui_thread, &m_debugging_gui };
        }
    }

    auto UE4SSProgram::load_unreal_offsets_from_file() -> void
    {
        std::filesystem::path file_path = m_working_directory / "MemberVariableLayout.ini";
        if (std::filesystem::exists(file_path))
        {
            auto file = File::open(file_path);
            if (auto file_contents = file.read_all(); !file_contents.empty())
            {
                Ini::Parser parser;
                parser.parse(file_contents);
                file.close();

                // The following code is auto-generated.
#include <MacroSetter.hpp>
            }
        }
    }

    auto UE4SSProgram::setup_unreal() -> void
    {
        // Retrieve offsets from the config file
        const std::wstring offset_overrides_section{L"OffsetOverrides"};
        using MemberOffsets = Unreal::StaticOffsetFinder::MemberOffsets;

        load_unreal_offsets_from_file();

        Unreal::UnrealInitializer::Config config;
        config.CachePath = m_root_directory / "cache";
        config.bInvalidateCacheIfSelfChanged = settings_manager.General.InvalidateCacheIfDLLDiffers;
        config.bEnableCache = true;
        config.NumScanAttemptsNormal = settings_manager.General.MaxScanAttemptsNormal;
        config.NumScanAttemptsModular = settings_manager.General.MaxScanAttemptsModular;
        config.bUseUObjectArrayCache = settings_manager.General.UseUObjectArrayCache;

        // Retrieve from the config file the number of threads to be used for aob scanning
        {
            // The config system only directly supports signed 64-bit integers
            // I'm using '-1' for the default and then only proceeding with using the value from the config file if it's within the
            // range of an unsigned 32-bit integer (which is what the SinglePassScanner uses)
            // The variables for these settings are default initialized with valid values so no need to set them if the config value
            // was either missing or invalid
            int64_t num_threads_for_scanner_from_config = settings_manager.Threads.SigScannerNumThreads;

            // The scanner is expecting a uint32_t so lets make sure we can safely convert to a uint32_t
            if (num_threads_for_scanner_from_config <= std::numeric_limits<uint32_t>::max() && num_threads_for_scanner_from_config >= 1)
            {
                config.NumScanThreads = static_cast<uint32_t>(num_threads_for_scanner_from_config);
            }
        }

        {
            int64_t multithreading_module_size_threshold_from_config = settings_manager.Threads.SigScannerMultithreadingModuleSizeThreshold;

            if (multithreading_module_size_threshold_from_config <= std::numeric_limits<uint32_t>::max() && multithreading_module_size_threshold_from_config >= std::numeric_limits<uint32_t>::min())
            {
                config.MultithreadingModuleSizeThreshold = static_cast<uint32_t>(multithreading_module_size_threshold_from_config);
            }
        }

        // Version override from ini file
        {
            int64_t major_version = settings_manager.EngineVersionOverride.MajorVersion;
            int64_t minor_version = settings_manager.EngineVersionOverride.MinorVersion;

            if (major_version != -1 && minor_version != -1)
            {
                // clang-format off
                if (major_version < std::numeric_limits<uint32_t>::min() ||
                    major_version > std::numeric_limits<uint32_t>::max() ||
                    minor_version < std::numeric_limits<uint32_t>::min() ||
                    minor_version > std::numeric_limits<uint32_t>::max())
                {
                    throw std::runtime_error{
                            "Was unable to override engine version from ini file; The number in the ini file must be in range of a uint32"};
                }
                // clang-format on

                Unreal::Version::Major = static_cast<uint32_t>(major_version);
                Unreal::Version::Minor = static_cast<uint32_t>(minor_version);

                config.ScanOverrides.version_finder = [&]([[maybe_unused]]auto&, Unreal::Signatures::ScanResult&) {};
            }
        }

        // If any Lua scripts are found, add overrides so that the Lua script can perform the aob scan instead of the Unreal API itself
        setup_lua_scan_overrides(m_working_directory, config);

        // Virtual function offset overrides
        TRY([&]() {
            static File::StringType virtual_function_offset_override_file{(m_working_directory / STR("VTableLayout.ini")).wstring()};
            if (std::filesystem::exists(virtual_function_offset_override_file))
            {
                auto file = File::open(virtual_function_offset_override_file, File::OpenFor::Reading, File::OverwriteExistingFile::No, File::CreateIfNonExistent::No);
                Ini::Parser parser;
                parser.parse(file);

                Output::send(STR("Getting ordered lists from ini file\n"));

                auto calculate_virtual_function_offset = []<typename... BaseSizes>(uint32_t current_index, BaseSizes... base_sizes) -> uint32_t {
                    return current_index == 0 ? 0 : (current_index + (base_sizes + ...)) * 8;
                };

                auto retrieve_vtable_layout_from_ini = [&](const File::StringType& section_name, auto callable) -> uint32_t {
                    auto list = parser.get_ordered_list(section_name);
                    uint32_t vtable_size = list.size() - 1;
                    list.for_each([&](uint32_t index, File::StringType& item) {
                        callable(index, item);
                    });
                    return vtable_size;
                };

                Output::send(STR("UObjectBase\n"));
                uint32_t uobjectbase_size = retrieve_vtable_layout_from_ini(STR("UObjectBase"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, 0);
                    Output::send(STR("UObjectBase::{} = 0x{:X}\n"), item, offset);
                    Unreal::UObjectBase::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("UObjectBaseUtility\n"));
                uint32_t uobjectbaseutility_size = retrieve_vtable_layout_from_ini(STR("UObjectBaseUtility"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, uobjectbase_size);
                    Output::send(STR("UObjectBaseUtility::{} = 0x{:X}\n"), item, offset);
                    Unreal::UObjectBaseUtility::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("UObject\n"));
                uint32_t uobject_size = retrieve_vtable_layout_from_ini(STR("UObject"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, uobjectbase_size, uobjectbaseutility_size);
                    Output::send(STR("UObject::{} = 0x{:X}\n"), item, offset);
                    Unreal::UObject::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("UField\n"));
                uint32_t ufield_size = retrieve_vtable_layout_from_ini(STR("UField"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, uobjectbase_size, uobjectbaseutility_size, uobject_size);
                    Output::send(STR("UField::{} = 0x{:X}\n"), item, offset);
                    Unreal::UStruct::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("UScriptStruct::ICppStructOps\n"));
                retrieve_vtable_layout_from_ini(STR("UScriptStruct::ICppStructOps"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, 0);
                    Output::send(STR("UScriptStruct::ICppStructOps::{} = 0x{:X}\n"), item, offset);
                    Unreal::UScriptStruct::ICppStructOps::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("FField\n"));
                uint32_t ffield_size = retrieve_vtable_layout_from_ini(STR("FField"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, 0);
                    Output::send(STR("FField::{} = 0x{:X}\n"), item, offset);
                    Unreal::FField::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("FProperty\n"));
                uint32_t fproperty_size = retrieve_vtable_layout_from_ini(STR("FProperty"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset{};
                    if (Unreal::Version::IsBelow(4, 25))
                    {
                        offset = calculate_virtual_function_offset(index, uobjectbase_size, uobjectbaseutility_size, uobject_size, ufield_size);
                    }
                    else
                    {
                        offset = calculate_virtual_function_offset(index, ffield_size);
                    }
                    Output::send(STR("FProperty::{} = 0x{:X}\n"), item, offset);
                    Unreal::FProperty::VTableLayoutMap.emplace(item, offset);
                });

                // If the engine version is <4.25 then the inheritance is different and we must take that into consideration.
                if (Unreal::Version::IsBelow(4, 25))
                {
                    fproperty_size = uobjectbase_size + uobjectbaseutility_size + uobject_size + ufield_size + fproperty_size;
                }
                else
                {
                    fproperty_size = ffield_size + fproperty_size;
                }

                Output::send(STR("FNumericProperty\n"));
                retrieve_vtable_layout_from_ini(STR("FNumericProperty"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, fproperty_size);
                    Output::send(STR("FNumericProperty::{} = 0x{:X}\n"), item, offset);
                    Unreal::FNumericProperty::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("FMulticastDelegateProperty\n"));
                retrieve_vtable_layout_from_ini(STR("FMulticastDelegateProperty"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, fproperty_size);
                    Output::send(STR("FMulticastDelegateProperty::{} = 0x{:X}\n"), item, offset);
                    Unreal::FMulticastDelegateProperty::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("FObjectPropertyBase\n"));
                retrieve_vtable_layout_from_ini(STR("FObjectPropertyBase"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, fproperty_size);
                    Output::send(STR("FObjectPropertyBase::{} = 0x{:X}\n"), item, offset);
                    Unreal::FObjectPropertyBase::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("UStruct\n"));
                uint32_t ustruct_size = retrieve_vtable_layout_from_ini(STR("UStruct"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, uobjectbase_size, uobjectbaseutility_size, uobject_size, ufield_size);
                    Output::send(STR("UStruct::{} = 0x{:X}\n"), item, offset);
                    Unreal::UField::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("FOutputDevice\n"));
                retrieve_vtable_layout_from_ini(STR("FOutputDevice"), [&](uint32_t index, File::StringType& item) {
                    uint32_t offset = calculate_virtual_function_offset(index, 0);
                    Output::send(STR("FOutputDevice::{} = 0x{:X}\n"), item, offset);
                    Unreal::FOutputDevice::VTableLayoutMap.emplace(item, offset);
                });

                Output::send(STR("FMalloc\n"));
                retrieve_vtable_layout_from_ini(STR("FMalloc"), [&](uint32_t index, File::StringType& item) {
                    // We don't support FExec, so we're manually telling it the size.
                    static constexpr uint32_t fexec_size = 1;
                    uint32_t offset = calculate_virtual_function_offset(index, fexec_size);
                    Output::send(STR("FMalloc::{} = 0x{:X}\n"), item, offset);
                    Unreal::FMalloc::VTableLayoutMap.emplace(item, offset);
                });

                file.close();
            }
        });

        Unreal::UnrealInitializer::Initialize(config);
    }

    auto UE4SSProgram::share_lua_functions() -> void
    {
        m_shared_functions.set_script_variable_int32_function = &LuaLibrary::set_script_variable_int32;
        m_shared_functions.set_script_variable_default_data_function = &LuaLibrary::set_script_variable_default_data;
        m_shared_functions.call_script_function_function = &LuaLibrary::call_script_function;
        m_shared_functions.is_ue4ss_initialized_function = &LuaLibrary::is_ue4ss_initialized;
        Output::send(STR("m_shared_functions: {}\n"), static_cast<void*>(&m_shared_functions));
    }

    static FName g_player_controller_name{};

    auto UE4SSProgram::on_program_start() -> void
    {
        using namespace Unreal;

        // Commented out because this system (turn off hotkeys when in-game console is open) it doesn't work properly.
        /*
        UObjectArray::AddUObjectCreateListener(&FUEDeathListener::UEDeathListener);
        //*/

        if (m_debug_console_enabled)
        {
            if (settings_manager.General.UseUObjectArrayCache)
            {
                m_debugging_gui.get_live_view().set_listeners_allowed(true);
                m_debugging_gui.get_live_view().set_listeners();
            }
            else
            {
                m_debugging_gui.get_live_view().set_listeners_allowed(false);
            }

            m_input_handler.register_keydown_event(Input::Key::O, { Input::ModifierKey::CONTROL }, [&]() {
                TRY([&] {
                    if (!get_debugging_ui().is_open())
                    {
                        if (m_render_thread.joinable())
                        {
                            m_render_thread.request_stop();
                            m_render_thread.join();
                        }
                        m_render_thread = std::jthread{ &GUI::gui_thread, &m_debugging_gui };
                    }
                    });
                });
        }

#ifdef TIME_FUNCTION_MACRO_ENABLED
        m_input_handler.register_keydown_event(Input::Key::Y, { Input::ModifierKey::CONTROL }, [&]() {
            if (FunctionTimerFrame::s_timer_enabled)
            {
                FunctionTimerFrame::stop_profiling();
                FunctionTimerFrame::dump_profile();
                Output::send(STR("Profiler stopped & dumped\n"));
            }
            else
            {
                FunctionTimerFrame::start_profiling();
                Output::send(STR("Profiler started\n"));
            }
        });
#endif

        TRY([&] {
            ObjectDumper::init();

            if (settings_manager.General.EnableHotReloadSystem)
            {
                m_input_handler.register_keydown_event(Input::Key::R, {Input::ModifierKey::CONTROL}, [&]() {
                    TRY([&] { reinstall_mods(); });
                });
            }

            if ((settings_manager.ObjectDumper.LoadAllAssetsBeforeDumpingObjects || settings_manager.CXXHeaderGenerator.LoadAllAssetsBeforeGeneratingCXXHeaders) && Unreal::Version::IsBelow(4, 17))
            {
                Output::send<LogLevel::Warning>(STR("FAssetData not available in <4.17, ignoring 'LoadAllAssetsBeforeDumpingObjects' & 'LoadAllAssetsBeforeGeneratingCXXHeaders'."));
            }

            setup_mods();
            Mod::on_program_start();
            start_mods();
        });

        if (settings_manager.General.EnableDebugKeyBindings)
        {
            m_input_handler.register_keydown_event(Input::Key::NUM_NINE, {Input::ModifierKey::CONTROL}, [&]() {
                generate_uht_compatible_headers();
            });
        }
    }

    auto UE4SSProgram::update() -> void
    {
        on_program_start();

        Output::send(STR("Event loop start\n"));
        for (m_processing_events = true; m_processing_events;)
        {
            if (m_pause_events_processing || UE4SSProgram::unreal_is_shutting_down) { continue; }

            if (!is_queue_empty())
            {
                static constexpr size_t max_events_executed_per_frame = 5;
                size_t num_events_executed{};
                std::lock_guard<std::mutex> guard(m_event_queue_mutex);
                m_queued_events.erase(std::remove_if(m_queued_events.begin(), m_queued_events.end(), [&](Event& event) -> bool {
                    if (num_events_executed >= max_events_executed_per_frame) { return false; }
                    ++num_events_executed;
                    event.callable(event.data);
                    return true;
                }), m_queued_events.end());
            }

            // Commented out because this system (turn off hotkeys when in-game console is open) it doesn't work properly.
            /*
            auto* player_controller = get_player_controller();
            if (player_controller)
            {
                auto** player = player_controller->GetValuePtrByPropertyName<UObject*>(STR("Player"));
                if (player && *player)
                {
                    auto** viewportclient = (*player)->GetValuePtrByPropertyName<UObject*>(STR("ViewportClient"));
                    if (viewportclient && *viewportclient)
                    {
                        auto** console = (*viewportclient)->GetValuePtrByPropertyName<UObject*>(STR("ViewportConsole"));
                        if (console && *console)
                        {
                            auto* console_state = std::bit_cast<FName*>(static_cast<uint8_t*>((*console)->GetValuePtrByPropertyNameInChain(STR("HistoryBuffer"))) + 0x70);
                            m_input_handler.set_allow_input(console_state && *console_state == Unreal::NAME_None);
                        }
                    }
                }
            }
            //*/

            m_input_handler.process_event();
            Mod::update();

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        Output::send(STR("Event loop end\n"));
    }

    auto UE4SSProgram::setup_unreal_properties() -> void
    {
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"ObjectProperty").GetComparisonIndex(), &LuaType::push_objectproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"ClassProperty").GetComparisonIndex(), &LuaType::push_classproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"Int8Property").GetComparisonIndex(), &LuaType::push_int8property);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"Int16Property").GetComparisonIndex(), &LuaType::push_int16property);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"IntProperty").GetComparisonIndex(), &LuaType::push_intproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"Int64Property").GetComparisonIndex(), &LuaType::push_int64property);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"ByteProperty").GetComparisonIndex(), &LuaType::push_byteproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"UInt16Property").GetComparisonIndex(), &LuaType::push_uint16property);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"UInt32Property").GetComparisonIndex(), &LuaType::push_uint32property);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"UInt64Property").GetComparisonIndex(), &LuaType::push_uint64property);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"StructProperty").GetComparisonIndex(), &LuaType::push_structproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"ArrayProperty").GetComparisonIndex(), &LuaType::push_arrayproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"FloatProperty").GetComparisonIndex(), &LuaType::push_floatproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"DoubleProperty").GetComparisonIndex(), &LuaType::push_doubleproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"BoolProperty").GetComparisonIndex(), &LuaType::push_boolproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"EnumProperty").GetComparisonIndex(), &LuaType::push_enumproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"WeakObjectProperty").GetComparisonIndex(), &LuaType::push_weakobjectproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"NameProperty").GetComparisonIndex(), &LuaType::push_nameproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"TextProperty").GetComparisonIndex(), &LuaType::push_textproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"StrProperty").GetComparisonIndex(), &LuaType::push_strproperty);
        LuaType::StaticState::m_property_value_pushers.emplace(FName(L"SoftClassProperty").GetComparisonIndex(), &LuaType::push_softclassproperty);
    }

    auto UE4SSProgram::setup_mods() -> void
    {
        Output::send(STR("Setting up mods...\n"));

        if (!std::filesystem::exists(m_mods_directory))
        {
            set_error("Mods directory doesn't exist, please create it: <%S>", m_mods_directory.c_str());
        }

        for (const auto& sub_directory : std::filesystem::directory_iterator(m_mods_directory))
        {
            std::error_code ec;

            // Ignore all non-directories
            if (!sub_directory.is_directory()) { continue; }
            if (ec.value() != 0) { set_error("is_directory ran into error %d", ec.value()); }

            std::wstring directory_lowercase = sub_directory.path().stem().wstring();
            std::transform(directory_lowercase.begin(), directory_lowercase.end(), directory_lowercase.begin(), std::towlower);

            if (directory_lowercase == L"shared")
            {
                // Do stuff when shared libraries have been implemented
            }
            else
            {
                // Create the mod but don't install it yet
                m_mods.emplace_back(std::make_unique<Mod>(*this, std::move(sub_directory.path().stem().wstring()), std::move(sub_directory.path().wstring())));
            }
        }

        install_mods();
    }

    auto UE4SSProgram::setup_output_devices() -> void
    {
        // Setup DynamicOutput
        if (m_simple_console_enabled)
        {
            m_debug_console_device = &Output::set_default_devices<Output::DebugConsoleDevice>();
            Output::set_default_log_level<LogLevel::Normal>();
            m_debug_console_device->set_formatter([](File::StringViewType string) -> File::StringType {
                return std::format(STR("[{}] {}"), std::format(STR("{:%X}"), std::chrono::system_clock::now()), string);
            });
        }

        if (m_debug_console_enabled)
        {
            m_console_device = &Output::set_default_devices<Output::ConsoleDevice>();
            m_console_device->set_formatter([](File::StringViewType string) -> File::StringType {
                return std::format(STR("[{}] {}"), std::format(STR("{:%X}"), std::chrono::system_clock::now()), string);
            });
        }
    }

    auto UE4SSProgram::install_mods() -> void
    {
        for (auto& mod : m_mods)
        {
            bool mod_name_is_taken = std::find_if(m_mods.begin(), m_mods.end(), [&](auto& elem) {
                return elem->get_name() == mod->get_name();
            }) == m_mods.end();

            if (mod_name_is_taken)
            {
                mod->set_installable(false);
                Output::send(STR("Mod name '{}' is already in use.\n"), mod->get_name());
                continue;
            }

            if (mod->is_installed())
            {
                Output::send(STR("Tried to install a mod that was already installed, Mod: '{}'\n"), mod->get_name());
                continue;
            }

            if (!mod->is_installable())
            {
                Output::send(STR("Was unable to install mod '{}' for unknown reasons. Mod is not installable.\n"), mod->get_name());
                continue;
            }

            mod->set_installed(true);
        }
    }

    auto UE4SSProgram::start_mods() -> void
    {
        // Part #1: Start all mods that are enabled in mods.txt.
        Output::send(STR("Starting mods (from mods.txt load order)...\n"));

        std::wstring enabled_mods_file{m_mods_directory / "mods.txt"};
        if (!std::filesystem::exists(enabled_mods_file))
        {
            Output::send(STR("No mods.txt file found...\n"));
        }
        else
        {
            // 'mods.txt' exists, lets parse it
            std::wifstream mods_stream{enabled_mods_file};

            size_t mod_count{};

            std::wstring current_line;
            while (std::getline(mods_stream, current_line))
            {
                // Don't parse any lines with ';'
                if (current_line.find(L";") != current_line.npos) { continue; }

                // Don't parse if the line is impossibly short (empty lines for example)
                if (current_line.size() <= 4) { continue; }

                // Increasing the mod counter but only after a valid line has been found
                ++mod_count;

                // Remove all spaces
                auto end = std::remove(current_line.begin(), current_line.end(), L' ');
                current_line.erase(end, current_line.end());

                // Parse the line into something that can be converted into proper data
                std::wstring mod_name = explode_by_occurrence(current_line, L':', 1);
                std::wstring mod_enabled = explode_by_occurrence(current_line, L':', ExplodeType::FromEnd);

                Mod* mod = find_mod_by_name(mod_name, IsInstalled::Yes);

                if (mod_enabled == L"1")
                {
                    if (!mod)
                    {
                        Output::send(STR("Tried to start non-existent mod '{}'\n"), mod_name);
                        continue;
                    }

                    Output::send(STR("Starting mod '{}'\n"), mod->get_name().data());
                    mod->start_mod();
                }
                else
                {
                    Output::send(STR("Mod '{}' disabled in mods.txt.\n"), mod_name);
                }
            }
        }

        // Part #2: Start all mods that have enabled.txt present in the mod directory.
        Output::send(STR("Starting mods (from enabled.txt, no defined load order)...\n"));

        for (const auto& mod_directory : std::filesystem::directory_iterator(m_mods_directory))
        {
            std::error_code ec{};

            if (!mod_directory.is_directory(ec)) { continue; }
            if (ec.value() != 0) { set_error("is_directory ran into error %d", ec.value()); }

            if (!std::filesystem::exists(mod_directory.path() / "enabled.txt", ec)) { continue; }
            if (ec.value() != 0) { set_error("exists ran into error %d", ec.value()); }

            auto mod = find_mod_by_name(mod_directory.path().stem().c_str(), IsInstalled::Yes);
            if (!mod)
            {
                Output::send<LogLevel::Warning>(STR("Found a mod with enabled.txt but mod has not been installed properly.\n"));
                continue;
            }
            
            if (mod->is_started()) { continue; }

            Output::send(STR("Mod '{}' has enabled.txt, starting mod.\n"), mod->get_name().data());
            mod->start_mod();
        }
    }

    auto UE4SSProgram::uninstall_mods() -> void
    {
        for (const auto& mod : m_mods)
        {
            // Remove any actions, or we'll get an internal error as the lua ref won't be valid
            mod->clear_delayed_actions();
            mod->uninstall();
        }

        m_mods.clear();
        Mod::m_static_construct_object_lua_callbacks.clear();
        Mod::m_global_command_lua_callbacks.clear();
        Mod::m_custom_command_lua_pre_callbacks.clear();
    }

    auto UE4SSProgram::reinstall_mods() -> void
    {
        Output::send(STR("Re-installing all mods\n"));

        // Stop processing events while stuff isn't properly setup
        m_pause_events_processing = true;

        uninstall_mods();

        // Remove key binds that were set from Lua scripts
        for (auto& input_event : m_input_handler.get_events())
        {
            for (auto&[key, vector_of_key_data] : input_event.key_data)
            {
                std::erase_if(vector_of_key_data, [](Input::KeyData& key_data) -> bool {
                    return key_data.custom_data == 1;
                });
            }
        }

        // Remove all custom properties
        // Uncomment when custom properties are working
        LuaType::LuaCustomProperty::StaticStorage::property_list.clear();

        // Reset the Lua callbacks for the global Lua function 'NotifyOnNewObject'
        Mod::m_static_construct_object_lua_callbacks.clear();

        // Start processing events again as everything is now properly setup
        // Do this before mods are started or else you won't be able to use the hot-reload key bind if there's an error from Lua
        m_pause_events_processing = false;

        setup_mods();
        start_mods();
        Output::send(STR("All mods re-installed\n"));
    }

    auto UE4SSProgram::get_working_directory() -> File::StringViewType
    {
        return m_working_directory.c_str();
    }

    auto UE4SSProgram::get_mods_directory() -> File::StringViewType
    {
        return m_mods_directory.c_str();
    }

    auto UE4SSProgram::generate_uht_compatible_headers() -> void
    {
        Output::send(STR("Generating UHT compatible headers...\n"));

        double generator_duration{};
        {
            ScopedTimer generator_timer{&generator_duration};

            const std::filesystem::path DumpRootDirectory = m_working_directory / "UHTHeaderDump";
            UEGenerator::UEHeaderGenerator HeaderGenerator = UEGenerator::UEHeaderGenerator(DumpRootDirectory);
            HeaderGenerator.dump_native_packages();
        }

        Output::send(STR("Generating UHT compatible headers took {} seconds\n"), generator_duration);
    }

    auto UE4SSProgram::generate_cxx_headers(const std::filesystem::path& output_dir) -> void
    {
        if (settings_manager.CXXHeaderGenerator.LoadAllAssetsBeforeGeneratingCXXHeaders)
        {
            Output::send(STR("Loading all assets...\n"));
            double asset_loading_duration{};
            {
                ScopedTimer loading_timer{&asset_loading_duration};

                UAssetRegistry::LoadAllAssets();
            }
            Output::send(STR("Loading all assets took {} seconds\n"), asset_loading_duration);
        }

        double generator_duration;
        {
            ScopedTimer generator_timer{&generator_duration};

            UEGenerator::CXXGenerator cxx_generator{output_dir};
            cxx_generator.generate();

            Output::send(STR("Unloading all forcefully loaded assets\n"));
        }

        UAssetRegistry::FreeAllForcefullyLoadedAssets();
        Output::send(STR("SDK generated in {} seconds.\n"), generator_duration);
    }

    auto UE4SSProgram::stop_render_thread() -> void
    {
        if (m_render_thread.joinable())
        {
            m_render_thread.request_stop();
            m_render_thread.join();
        }
    }

    auto UE4SSProgram::queue_event(EventCallable callable, void* data) -> void
    {
        if (!can_process_events()) { return; }
        std::lock_guard<std::mutex> guard(m_event_queue_mutex);
        m_queued_events.emplace_back(Event{callable, data});
    }

    auto UE4SSProgram::is_queue_empty() -> bool
    {
        // Not locking here because if the worst that could happen as far as I know is that the event loop processes the event slightly late.
        return m_queued_events.empty();
    }

    auto UE4SSProgram::register_keydown_event(Input::Key key, const Input::EventCallbackCallable& callback, uint8_t custom_data) -> void
    {
        m_input_handler.register_keydown_event(key, callback, custom_data);
    }

    auto UE4SSProgram::register_keydown_event(Input::Key key, const Input::Handler::ModifierKeyArray& modifier_keys, const Input::EventCallbackCallable& callback, uint8_t custom_data) -> void
    {
        m_input_handler.register_keydown_event(key, modifier_keys, callback, custom_data);
    }

    auto UE4SSProgram::find_mod_by_name(std::wstring_view mod_name, IsInstalled is_installed, IsStarted is_started) -> Mod*
    {
        auto mod_exists_with_name = std::find_if(m_mods.begin(), m_mods.end(), [&](auto& elem) -> bool {
            bool found = true;

            if (mod_name != elem->get_name()) { found = false; }
            if (is_installed == IsInstalled::Yes && !elem->is_installable()) { found = false; }
            if (is_started == IsStarted::Yes && !elem->is_started()) { found = false; }

            return found;
        });

        // clang-format off
        if (mod_exists_with_name == m_mods.end())
        {
            return nullptr;
        }
        // clang-format on
        else
        {
            return mod_exists_with_name->get();
        }
    }

    auto UE4SSProgram::find_mod_by_name(std::string_view mod_name, UE4SSProgram::IsInstalled installed_only, IsStarted is_started) -> Mod*
    {
        return find_mod_by_name(to_wstring(mod_name), installed_only, is_started);
    }

    auto UE4SSProgram::get_object_dumper_output_directory() -> const File::StringType
    {
        return m_object_dumper_output_directory.c_str();
    }

    auto UE4SSProgram::dump_all_objects_and_properties(const File::StringType& output_path_and_file_name) -> void
    {
        /*
        Output::send(STR("Test msg with no fmt args, and no optional arg\n"));
        Output::send(STR("Test msg with no fmt args, and one optional arg [Normal]\n"), LogLevel::Normal);
        Output::send(STR("Test msg with no fmt args, and one optional arg [Verbose]\n"), LogLevel::Verbose);
        Output::send(STR("Test msg with one fmt arg [{}], and one optional arg [Warning]\n"), LogLevel::Warning, 33);
        Output::send(STR("Test msg with two fmt args [{}, {}], and one optional arg [Error]\n"), LogLevel::Error, 33, 44);
        //*/

        // Object & Property Dumper -> START
        if (settings_manager.ObjectDumper.LoadAllAssetsBeforeDumpingObjects)
        {
            Output::send(STR("Loading all assets...\n"));
            double asset_loading_duration{};
            {
                ScopedTimer loading_timer{&asset_loading_duration};

                UAssetRegistry::LoadAllAssets();
            }
            Output::send(STR("Loading all assets took {} seconds\n"), asset_loading_duration);
        }

        double dumper_duration{};
        {
            ScopedTimer dumper_timer{&dumper_duration};

            std::unordered_set<FField*> dumped_fields;
            // There will be tons of dumped fields so lets just reserve tons in order to speed things up a bit
            dumped_fields.reserve(100000);

            bool is_below_425 = Unreal::Version::IsBelow(4, 25);

            // The final outputted string shouldn't need be reformatted just to put a new line at the end
            // Instead the object/property implementations should add a new line in the last format that they do
            //
            // Optimizations done:
            // 1. The entire code-base has been changed to use 'wchar_t' instead of 'char'.
            // The effect of this is that there is no need to ever convert between types.
            // There's also no thinking about which type should be used since 'wchar_t' is now the standard for UE4SS.
            // The downside with wchar_t is that all files that get output to will be doubled in size.

            using ObjectDumperOutputDevice = Output::NewFileDevice;
            Output::Targets<ObjectDumperOutputDevice> scoped_dumper_out;
            auto& file_device = scoped_dumper_out.get_device<ObjectDumperOutputDevice>();
            file_device.set_file_name_and_path(output_path_and_file_name);
            file_device.set_formatter([](File::StringViewType string) -> File::StringType {
                return File::StringType{string};
            });

            // Make string & reserve massive amounts of space to hopefully not reach the end of the string and require more
            // dynamic allocations
            std::wstring out_line;
            out_line.reserve(200000000);

            auto dump_xproperty = [&](FProperty* property) -> void {
                auto typed_prop_class = property->GetClass().HashObject();

                if (ObjectDumper::to_string_exists(typed_prop_class))
                {
                    ObjectDumper::get_to_string(typed_prop_class)(property, out_line);
                    out_line.append(L"\n");

                    if (ObjectDumper::to_string_complex_exists(typed_prop_class))
                    {
                        ObjectDumper::get_to_string_complex(typed_prop_class)(property, out_line, [&]([[maybe_unused]]void* prop) {
                            out_line.append(L"\n");
                        });
                    }
                }
                else
                {
                    ObjectDumper::property_to_string(property, out_line);
                    out_line.append(L"\n");
                }
            };

            auto dump_uobject = [&](void* object) -> void {
                UObject* typed_obj = static_cast<UObject*>(object);

                if (is_below_425 && Unreal::TypeChecker::is_property(typed_obj) && !typed_obj->HasAnyFlags(static_cast<EObjectFlags>(EObjectFlags::RF_DefaultSubObject | EObjectFlags::RF_ArchetypeObject)))
                {
                    // We've verified that we're in <4.25 so this cast is safe but should be abstracted at some point
                    dump_xproperty(std::bit_cast<FProperty*>(typed_obj));
                }
                else
                {
                    auto typed_class = typed_obj->GetClassPrivate()->HashObject();
                    if (ObjectDumper::to_string_exists(typed_class))
                    {
                        // Call type-specific implementation to dump UObject
                        // The type is determined at runtime

                        // Dump UObject
                        ObjectDumper::get_to_string(typed_class)(object, out_line);
                        out_line.append(L"\n");

                        if (!is_below_425 && ObjectDumper::to_string_complex_exists(typed_class))
                        {
                            // Dump all properties that are directly owned by this UObject (not its UClass)
                            // UE 4.25+ (properties are part of GUObjectArray in earlier versions)
                            ObjectDumper::get_to_string_complex(typed_class)(object, out_line, [&](void* prop) {
                                if (dumped_fields.contains(static_cast<FField*>(prop))) { return; }

                                dump_xproperty(static_cast<FProperty*>(prop));
                                dumped_fields.emplace(static_cast<FField*>(prop));
                            });
                        }
                    }
                    else
                    {
                        // A type-specific implementation does not exist so lets call the default implementation for UObjects instead
                        ObjectDumper::object_to_string(object, out_line);
                        out_line.append(L"\n");
                    }

                    // If the UClass of the UObject has any properties then dump them
                    // UE 4.25+ (properties are part of GUObjectArray in earlier versions)
                    if (!is_below_425)
                    {
                        if (typed_obj->IsA<UStruct>())
                        {
                            static_cast<UClass*>(typed_obj)->ForEachProperty([&](FProperty* prop) {
                                if (dumped_fields.contains(prop)) { return LoopAction::Continue; }

                                dump_xproperty(prop);
                                dumped_fields.emplace(prop);

                                return LoopAction::Continue;
                            });
                        }
                    }
                }
            };

            Output::send(STR("Dumping all objects & properties in GUObjectArray\n"));
            UObjectGlobals::ForEachUObject([&](void* object, [[maybe_unused]]int32_t chunk_index, [[maybe_unused]]int32_t object_index) {
                dump_uobject(object);
                return LoopAction::Continue;
            });

            // Save to file
            scoped_dumper_out.send(out_line);

            // Reset the dumped_fields set, otherwise no fields will be dumped in subsequent dumps
            dumped_fields.clear();
            Output::send(STR("Done iterating GUObjectArray\n"));
        }

        UAssetRegistry::FreeAllForcefullyLoadedAssets();
        Output::send(STR("Dumping GUObjectArray took {} seconds\n"), dumper_duration);
        // Object & Property Dumper -> END
    }

    auto UE4SSProgram::static_cleanup() -> void
    {
        delete &get_program();

        // Do cleanup of static objects here
        // This function is called right before the DLL detaches from the game
        // Including when the player hits the 'X' button to exit the game
#if TIME_FUNCTION_MACRO_V2 == 0
        FunctionTimerCollection::dump();
#endif
    }
}

