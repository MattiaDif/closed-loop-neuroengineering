#include "ext_types.h"
static DataTypeInfo rtDataTypeInfoTable [ ] = { { "real_T" , 0 , 8 } , {
"real32_T" , 1 , 4 } , { "int8_T" , 2 , 1 } , { "uint8_T" , 3 , 1 } , {
"int16_T" , 4 , 2 } , { "uint16_T" , 5 , 2 } , { "int32_T" , 6 , 4 } , {
"uint32_T" , 7 , 4 } , { "boolean_T" , 8 , 1 } , { "fcn_call_T" , 9 , 0 } , {
"int_T" , 10 , 4 } , { "pointer_T" , 11 , 8 } , { "action_T" , 12 , 8 } , {
"timer_uint32_pair_T" , 13 , 8 } , { "uint64_T" , 14 , 8 } , { "int64_T" , 15
, 8 } , { "int64_T" , 16 , 8 } , { "iw15x2zilv" , 17 , 56 } , { "a24a4xbsq3"
, 18 , 224 } , { "int64_T" , 19 , 8 } , { "uint64_T" , 20 , 8 } } ; static
uint_T rtDataTypeSizes [ ] = { sizeof ( real_T ) , sizeof ( real32_T ) ,
sizeof ( int8_T ) , sizeof ( uint8_T ) , sizeof ( int16_T ) , sizeof (
uint16_T ) , sizeof ( int32_T ) , sizeof ( uint32_T ) , sizeof ( boolean_T )
, sizeof ( fcn_call_T ) , sizeof ( int_T ) , sizeof ( pointer_T ) , sizeof (
action_T ) , 2 * sizeof ( uint32_T ) , sizeof ( uint64_T ) , sizeof ( int64_T
) , sizeof ( int64_T ) , sizeof ( iw15x2zilv ) , sizeof ( a24a4xbsq3 ) ,
sizeof ( int64_T ) , sizeof ( uint64_T ) } ; static const char_T *
rtDataTypeNames [ ] = { "real_T" , "real32_T" , "int8_T" , "uint8_T" ,
"int16_T" , "uint16_T" , "int32_T" , "uint32_T" , "boolean_T" , "fcn_call_T"
, "int_T" , "pointer_T" , "action_T" , "timer_uint32_pair_T" , "uint64_T" ,
"int64_T" , "int64_T" , "iw15x2zilv" , "a24a4xbsq3" , "int64_T" , "uint64_T"
} ; static DataTypeTransition rtBTransitions [ ] = { { ( char_T * ) ( & rtB .
l1roa33str ) , 0 , 0 , 3 } , { ( char_T * ) ( & rtB . hf2aykmen1 ) , 7 , 0 ,
1 } , { ( char_T * ) ( & rtB . jwpromv5g4 ) , 8 , 0 , 3 } , { ( char_T * ) (
& rtDW . lhfowgpsft ) , 18 , 0 , 2 } , { ( char_T * ) ( & rtDW . nk1f51vvzs )
, 17 , 0 , 1 } , { ( char_T * ) ( & rtDW . ij10nfstjj . PrevTimePtr ) , 11 ,
0 , 8 } , { ( char_T * ) ( & rtDW . iog2t1qii5 ) , 7 , 0 , 1 } , { ( char_T *
) ( & rtDW . fapvev05kd ) , 8 , 0 , 4 } } ; static DataTypeTransitionTable
rtBTransTable = { 8U , rtBTransitions } ; static DataTypeTransition
rtPTransitions [ ] = { { ( char_T * ) ( & rtP . th ) , 0 , 0 , 3 } , { (
char_T * ) ( & rtP . Max_value_const ) , 7 , 0 , 1 } , { ( char_T * ) ( & rtP
. Gain_Gain ) , 0 , 0 , 3 } , { ( char_T * ) ( & rtP . Switch_wrap_Threshold
) , 16 , 0 , 1 } , { ( char_T * ) ( & rtP . Switch_wrap_Threshold_ny2jtjg2k1
) , 14 , 0 , 3 } , { ( char_T * ) ( & rtP . Count_reg_InitialCondition ) , 7
, 0 , 5 } , { ( char_T * ) ( & rtP . Delay_InitialCondition ) , 8 , 0 , 6 } }
; static DataTypeTransitionTable rtPTransTable = { 7U , rtPTransitions } ;
