#include "ext_types.h"
static DataTypeInfo rtDataTypeInfoTable [ ] = { { "real_T" , 0 , 8 } , {
"real32_T" , 1 , 4 } , { "int8_T" , 2 , 1 } , { "uint8_T" , 3 , 1 } , {
"int16_T" , 4 , 2 } , { "uint16_T" , 5 , 2 } , { "int32_T" , 6 , 4 } , {
"uint32_T" , 7 , 4 } , { "boolean_T" , 8 , 1 } , { "fcn_call_T" , 9 , 0 } , {
"int_T" , 10 , 4 } , { "pointer_T" , 11 , 8 } , { "action_T" , 12 , 8 } , {
"timer_uint32_pair_T" , 13 , 8 } , { "uint64_T" , 14 , 8 } , { "int64_T" , 15
, 8 } , { "int64_T" , 16 , 8 } , { "int64_T" , 17 , 8 } , { "uint64_T" , 18 ,
8 } } ; static uint_T rtDataTypeSizes [ ] = { sizeof ( real_T ) , sizeof (
real32_T ) , sizeof ( int8_T ) , sizeof ( uint8_T ) , sizeof ( int16_T ) ,
sizeof ( uint16_T ) , sizeof ( int32_T ) , sizeof ( uint32_T ) , sizeof (
boolean_T ) , sizeof ( fcn_call_T ) , sizeof ( int_T ) , sizeof ( pointer_T )
, sizeof ( action_T ) , 2 * sizeof ( uint32_T ) , sizeof ( uint64_T ) ,
sizeof ( int64_T ) , sizeof ( int64_T ) , sizeof ( int64_T ) , sizeof (
uint64_T ) } ; static const char_T * rtDataTypeNames [ ] = { "real_T" ,
"real32_T" , "int8_T" , "uint8_T" , "int16_T" , "uint16_T" , "int32_T" ,
"uint32_T" , "boolean_T" , "fcn_call_T" , "int_T" , "pointer_T" , "action_T"
, "timer_uint32_pair_T" , "uint64_T" , "int64_T" , "int64_T" , "int64_T" ,
"uint64_T" } ; static DataTypeTransition rtBTransitions [ ] = { { ( char_T *
) ( & rtB . ouzstej054 ) , 0 , 0 , 3 } , { ( char_T * ) ( & rtB . jesi2z0j4g
) , 7 , 0 , 1 } , { ( char_T * ) ( & rtB . jwpromv5g4 ) , 8 , 0 , 4 } , { (
char_T * ) ( & rtDW . a4ipisgwzj ) , 0 , 0 , 1 } , { ( char_T * ) ( & rtDW .
pj5i3u21oj . PrevTimePtr ) , 11 , 0 , 8 } , { ( char_T * ) ( & rtDW .
iqegvrxn5l ) , 7 , 0 , 1 } , { ( char_T * ) ( & rtDW . hwmpquokvc ) , 8 , 0 ,
2 } } ; static DataTypeTransitionTable rtBTransTable = { 7U , rtBTransitions
} ; static DataTypeTransition rtPTransitions [ ] = { { ( char_T * ) ( & rtP .
HDLCounter_CountStep ) , 0 , 0 , 3 } , { ( char_T * ) ( & rtP .
Max_value_const ) , 7 , 0 , 1 } , { ( char_T * ) ( & rtP . Constant_Value ) ,
0 , 0 , 4 } , { ( char_T * ) ( & rtP . Switch_wrap_Threshold ) , 16 , 0 , 1 }
, { ( char_T * ) ( & rtP . Switch_wrap_Threshold_gqqrfmqwsl ) , 14 , 0 , 3 }
, { ( char_T * ) ( & rtP . Count_reg_InitialCondition ) , 7 , 0 , 5 } , { (
char_T * ) ( & rtP . Delay_InitialCondition ) , 8 , 0 , 6 } } ; static
DataTypeTransitionTable rtPTransTable = { 7U , rtPTransitions } ;
