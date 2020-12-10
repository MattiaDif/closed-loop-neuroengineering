#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "HardThreshold_capi_host.h"
#define sizeof(s) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)    
#ifndef SS_INT64
#define SS_INT64  19
#endif
#ifndef SS_UINT64
#define SS_UINT64  20
#endif
#else
#include "builtin_typeid_types.h"
#include "HardThreshold.h"
#include "HardThreshold_capi.h"
#include "HardThreshold_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST                  
#define TARGET_STRING(s)               (NULL)                    
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif
static const rtwCAPI_Signals rtBlockSignals [ ] = { { 0 , 0 , TARGET_STRING (
"HardThreshold/Abs" ) , TARGET_STRING (
"Raw extracellular recording - Channel 29 - ABS" ) , 0 , 0 , 0 , 0 , 0 } , {
1 , 0 , TARGET_STRING (
"HardThreshold/Juxtacellular point process (Ground truth)1" ) , TARGET_STRING
( "Juxtacellular point process (Ground truth)" ) , 0 , 0 , 0 , 0 , 0 } , { 2
, 0 , TARGET_STRING ( "HardThreshold/Compare To Constant/Compare" ) ,
TARGET_STRING ( "" ) , 0 , 1 , 0 , 0 , 0 } , { 3 , 0 , TARGET_STRING (
"HardThreshold/Refractory check/Gain" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 ,
0 , 0 } , { 4 , 0 , TARGET_STRING ( "HardThreshold/Refractory check/Switch" )
, TARGET_STRING ( "" ) , 0 , 1 , 0 , 0 , 0 } , { 5 , 0 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter/Switch_reset"
) , TARGET_STRING ( "" ) , 0 , 2 , 0 , 0 , 0 } , { 6 , 0 , TARGET_STRING (
 "HardThreshold/Refractory check/Time since last event/HDL Counter/Dir_logic/Logical Operator"
) , TARGET_STRING ( "" ) , 0 , 1 , 0 , 0 , 1 } , { 0 , 0 , ( NULL ) , ( NULL
) , 0 , 0 , 0 , 0 , 0 } } ; static const rtwCAPI_BlockParameters
rtBlockParameters [ ] = { { 7 , TARGET_STRING ( "HardThreshold/Gain" ) ,
TARGET_STRING ( "Gain" ) , 0 , 0 , 0 } , { 8 , TARGET_STRING (
"HardThreshold/Edge Detector/Delay" ) , TARGET_STRING ( "InitialCondition" )
, 1 , 0 , 0 } , { 9 , TARGET_STRING (
"HardThreshold/Refractory check/Refractory period" ) , TARGET_STRING (
"const" ) , 0 , 0 , 0 } , { 10 , TARGET_STRING (
"HardThreshold/Refractory check/Constant" ) , TARGET_STRING ( "Value" ) , 0 ,
0 , 0 } , { 11 , TARGET_STRING ( "HardThreshold/Refractory check/Gain" ) ,
TARGET_STRING ( "Gain" ) , 0 , 0 , 0 } , { 12 , TARGET_STRING (
"HardThreshold/Refractory check/Memory1" ) , TARGET_STRING (
"InitialCondition" ) , 1 , 0 , 0 } , { 13 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter" ) ,
TARGET_STRING ( "CountStep" ) , 0 , 0 , 0 } , { 14 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter/Max_value"
) , TARGET_STRING ( "const" ) , 2 , 0 , 0 } , { 15 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter/Free_running"
) , TARGET_STRING ( "Value" ) , 1 , 0 , 0 } , { 16 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter/From_value"
) , TARGET_STRING ( "Value" ) , 2 , 0 , 0 } , { 17 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter/Init_value"
) , TARGET_STRING ( "Value" ) , 2 , 0 , 0 } , { 18 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter/Step_value"
) , TARGET_STRING ( "Value" ) , 2 , 0 , 0 } , { 19 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter/const_dir"
) , TARGET_STRING ( "Value" ) , 1 , 0 , 0 } , { 20 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter/const_enb"
) , TARGET_STRING ( "Value" ) , 1 , 0 , 0 } , { 21 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter/const_load"
) , TARGET_STRING ( "Value" ) , 1 , 0 , 0 } , { 22 , TARGET_STRING (
 "HardThreshold/Refractory check/Time since last event/HDL Counter/const_load_val"
) , TARGET_STRING ( "Value" ) , 2 , 0 , 0 } , { 23 , TARGET_STRING (
"HardThreshold/Refractory check/Time since last event/HDL Counter/Count_reg"
) , TARGET_STRING ( "InitialCondition" ) , 2 , 0 , 0 } , { 24 , TARGET_STRING
(
 "HardThreshold/Refractory check/Time since last event/HDL Counter/Add_wrap/Mod_value"
) , TARGET_STRING ( "Value" ) , 3 , 0 , 1 } , { 25 , TARGET_STRING (
 "HardThreshold/Refractory check/Time since last event/HDL Counter/Add_wrap/Switch_wrap"
) , TARGET_STRING ( "Threshold" ) , 3 , 0 , 1 } , { 26 , TARGET_STRING (
 "HardThreshold/Refractory check/Time since last event/HDL Counter/Sub_wrap/Mod_value"
) , TARGET_STRING ( "Value" ) , 3 , 0 , 1 } , { 27 , TARGET_STRING (
 "HardThreshold/Refractory check/Time since last event/HDL Counter/Sub_wrap/Switch_wrap"
) , TARGET_STRING ( "Threshold" ) , 4 , 0 , 2 } , { 0 , ( NULL ) , ( NULL ) ,
0 , 0 , 0 } } ; static const rtwCAPI_ModelParameters rtModelParameters [ ] =
{ { 28 , TARGET_STRING ( "th" ) , 0 , 0 , 0 } , { 0 , ( NULL ) , 0 , 0 , 0 }
} ;
#ifndef HOST_CAPI_BUILD
static void * rtDataAddrMap [ ] = { & rtB . htyuumrruo , & rtB . l1roa33str ,
& rtB . jwpromv5g4 , & rtB . mmcebxfnqk , & rtB . ma2is0pfqi , & rtB .
hf2aykmen1 , & rtB . nml21z2krk , & rtP . Gain_Gain , & rtP .
Delay_InitialCondition , & rtP . Refractoryperiod_const , & rtP .
Constant_Value , & rtP . Gain_Gain_lorz3laygo , & rtP .
Memory1_InitialCondition , & rtP . HDLCounter_CountStep , & rtP .
Max_value_const , & rtP . Free_running_Value , & rtP . From_value_Value , &
rtP . Init_value_Value , & rtP . Step_value_Value , & rtP . const_dir_Value ,
& rtP . const_enb_Value , & rtP . const_load_Value , & rtP .
const_load_val_Value , & rtP . Count_reg_InitialCondition , & rtP .
Mod_value_Value , & rtP . Switch_wrap_Threshold_ny2jtjg2k1 , & rtP .
Mod_value_Value_hctwj2wdhl , & rtP . Switch_wrap_Threshold , & rtP . th , } ;
static int32_T * rtVarDimsAddrMap [ ] = { ( NULL ) } ;
#endif
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap [ ] = { { "double" ,
"real_T" , 0 , 0 , sizeof ( real_T ) , SS_DOUBLE , 0 , 0 , 0 } , {
"unsigned char" , "boolean_T" , 0 , 0 , sizeof ( boolean_T ) , SS_BOOLEAN , 0
, 0 , 0 } , { "unsigned int" , "uint32_T" , 0 , 0 , sizeof ( uint32_T ) ,
SS_UINT32 , 0 , 0 , 0 } , { "numeric" , "uint64_T" , 0 , 0 , sizeof (
uint64_T ) , SS_UINT64 , 0 , 0 , 0 } , { "numeric" , "int64_T" , 0 , 0 ,
sizeof ( int64_T ) , SS_INT64 , 0 , 0 , 0 } } ;
#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif
static TARGET_CONST rtwCAPI_ElementMap rtElementMap [ ] = { { ( NULL ) , 0 ,
0 , 0 , 0 } , } ; static const rtwCAPI_DimensionMap rtDimensionMap [ ] = { {
rtwCAPI_SCALAR , 0 , 2 , 0 } } ; static const uint_T rtDimensionArray [ ] = {
1 , 1 } ; static const real_T rtcapiStoredFloats [ ] = {
3.3333333333333335E-5 , 0.0 , 1.0 } ; static const rtwCAPI_FixPtMap
rtFixPtMap [ ] = { { ( NULL ) , ( NULL ) , rtwCAPI_FIX_RESERVED , 0 , 0 , 0 }
, { ( const void * ) & rtcapiStoredFloats [ 2 ] , ( const void * ) &
rtcapiStoredFloats [ 1 ] , rtwCAPI_FIX_UNIFORM_SCALING , 33 , 0 , 0 } , { (
const void * ) & rtcapiStoredFloats [ 2 ] , ( const void * ) &
rtcapiStoredFloats [ 1 ] , rtwCAPI_FIX_UNIFORM_SCALING , 33 , 0 , 1 } } ;
static const rtwCAPI_SampleTimeMap rtSampleTimeMap [ ] = { { ( const void * )
& rtcapiStoredFloats [ 0 ] , ( const void * ) & rtcapiStoredFloats [ 1 ] , 1
, 0 } , { ( NULL ) , ( NULL ) , 2 , 0 } } ; static
rtwCAPI_ModelMappingStaticInfo mmiStatic = { { rtBlockSignals , 7 , ( NULL )
, 0 , ( NULL ) , 0 } , { rtBlockParameters , 21 , rtModelParameters , 1 } , {
( NULL ) , 0 } , { rtDataTypeMap , rtDimensionMap , rtFixPtMap , rtElementMap
, rtSampleTimeMap , rtDimensionArray } , "float" , { 1579930992U ,
1493352536U , 4053685715U , 2386267743U } , ( NULL ) , 0 , 0 } ; const
rtwCAPI_ModelMappingStaticInfo * HardThreshold_GetCAPIStaticMap ( void ) {
return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
void HardThreshold_InitializeDataMapInfo ( void ) { rtwCAPI_SetVersion ( ( *
rt_dataMapInfoPtr ) . mmi , 1 ) ; rtwCAPI_SetStaticMap ( ( *
rt_dataMapInfoPtr ) . mmi , & mmiStatic ) ; rtwCAPI_SetLoggingStaticMap ( ( *
rt_dataMapInfoPtr ) . mmi , ( NULL ) ) ; rtwCAPI_SetDataAddressMap ( ( *
rt_dataMapInfoPtr ) . mmi , rtDataAddrMap ) ; rtwCAPI_SetVarDimsAddressMap (
( * rt_dataMapInfoPtr ) . mmi , rtVarDimsAddrMap ) ;
rtwCAPI_SetInstanceLoggingInfo ( ( * rt_dataMapInfoPtr ) . mmi , ( NULL ) ) ;
rtwCAPI_SetChildMMIArray ( ( * rt_dataMapInfoPtr ) . mmi , ( NULL ) ) ;
rtwCAPI_SetChildMMIArrayLen ( ( * rt_dataMapInfoPtr ) . mmi , 0 ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void HardThreshold_host_InitializeDataMapInfo (
HardThreshold_host_DataMapInfo_T * dataMap , const char * path ) {
rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ; rtwCAPI_SetStaticMap ( dataMap ->
mmi , & mmiStatic ) ; rtwCAPI_SetDataAddressMap ( dataMap -> mmi , NULL ) ;
rtwCAPI_SetVarDimsAddressMap ( dataMap -> mmi , NULL ) ; rtwCAPI_SetPath (
dataMap -> mmi , path ) ; rtwCAPI_SetFullPath ( dataMap -> mmi , NULL ) ;
rtwCAPI_SetChildMMIArray ( dataMap -> mmi , ( NULL ) ) ;
rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi , 0 ) ; }
#ifdef __cplusplus
}
#endif
#endif
