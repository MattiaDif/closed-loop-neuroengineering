#ifndef RTW_HEADER_HardThreshold_h_
#define RTW_HEADER_HardThreshold_h_
#include <stddef.h>
#include <string.h>
#include "rtw_modelmap.h"
#ifndef HardThreshold_COMMON_INCLUDES_
#define HardThreshold_COMMON_INCLUDES_
#include <stdlib.h>
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simtarget/slSimTgtSigstreamRTW.h"
#include "simtarget/slSimTgtSlioCoreRTW.h"
#include "simtarget/slSimTgtSlioClientsRTW.h"
#include "simtarget/slSimTgtSlioSdiRTW.h"
#include "rtwtypes.h"
#include "sl_fileio_rtw.h"
#include "simtarget/slSimTgtSlFileioRTW.h"
#include "sigstream_rtw.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "raccel.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "rt_logging.h"
#include "dt_info.h"
#include "ext_work.h"
#endif
#include "HardThreshold_types.h"
#include "multiword_types.h"
#include "mwmathutil.h"
#include "rt_defines.h"
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#define MODEL_NAME HardThreshold
#define NSAMPLE_TIMES (3) 
#define NINPUTS (0)       
#define NOUTPUTS (0)     
#define NBLOCKIO (7) 
#define NUM_ZC_EVENTS (0) 
#ifndef NCSTATES
#define NCSTATES (0)   
#elif NCSTATES != 0
#error Invalid specification of NCSTATES defined in compiler command
#endif
#ifndef rtmGetDataMapInfo
#define rtmGetDataMapInfo(rtm) (*rt_dataMapInfoPtr)
#endif
#ifndef rtmSetDataMapInfo
#define rtmSetDataMapInfo(rtm, val) (rt_dataMapInfoPtr = &val)
#endif
#ifndef IN_RACCEL_MAIN
#endif
typedef struct { real_T l1roa33str ; real_T htyuumrruo ; real_T mmcebxfnqk ;
uint32_T hf2aykmen1 ; boolean_T jwpromv5g4 ; boolean_T ma2is0pfqi ; boolean_T
nml21z2krk ; } B ; typedef struct { a24a4xbsq3 lhfowgpsft ; a24a4xbsq3
fz00ynzxl5 ; iw15x2zilv nk1f51vvzs ; struct { void * PrevTimePtr ; }
ij10nfstjj ; struct { void * PrevTimePtr ; } k34mbqgurr ; struct { void *
LoggedData [ 5 ] ; } bpiitaefte ; struct { void * AQHandles ; void * SlioLTF
; } ctp1gugaiy ; uint32_T iog2t1qii5 ; boolean_T fapvev05kd ; boolean_T
h3n5p0se4d ; boolean_T edrqueilvh ; boolean_T mtwbokvwd3 ; } DW ; typedef
struct { rtwCAPI_ModelMappingInfo mmi ; } DataMapInfo ; struct P_ { real_T th
; real_T HDLCounter_CountStep ; real_T Refractoryperiod_const ; uint32_T
Max_value_const ; real_T Gain_Gain ; real_T Gain_Gain_lorz3laygo ; real_T
Constant_Value ; int64_T Switch_wrap_Threshold ; uint64_T
Switch_wrap_Threshold_ny2jtjg2k1 ; uint64_T Mod_value_Value ; uint64_T
Mod_value_Value_hctwj2wdhl ; uint32_T Count_reg_InitialCondition ; uint32_T
From_value_Value ; uint32_T Init_value_Value ; uint32_T Step_value_Value ;
uint32_T const_load_val_Value ; boolean_T Delay_InitialCondition ; boolean_T
Memory1_InitialCondition ; boolean_T const_dir_Value ; boolean_T
Free_running_Value ; boolean_T const_enb_Value ; boolean_T const_load_Value ;
} ; extern const char * RT_MEMORY_ALLOCATION_ERROR ; extern B rtB ; extern DW
rtDW ; extern P rtP ; extern const rtwCAPI_ModelMappingStaticInfo *
HardThreshold_GetCAPIStaticMap ( void ) ; extern SimStruct * const rtS ;
extern const int_T gblNumToFiles ; extern const int_T gblNumFrFiles ; extern
const int_T gblNumFrWksBlocks ; extern rtInportTUtable * gblInportTUtables ;
extern const char * gblInportFileName ; extern const int_T
gblNumRootInportBlks ; extern const int_T gblNumModelInputs ; extern const
int_T gblInportDataTypeIdx [ ] ; extern const int_T gblInportDims [ ] ;
extern const int_T gblInportComplex [ ] ; extern const int_T
gblInportInterpoFlag [ ] ; extern const int_T gblInportContinuous [ ] ;
extern const int_T gblParameterTuningTid ; extern DataMapInfo *
rt_dataMapInfoPtr ; extern rtwCAPI_ModelMappingInfo * rt_modelMapInfoPtr ;
void MdlOutputs ( int_T tid ) ; void MdlOutputsParameterSampleTime ( int_T
tid ) ; void MdlUpdate ( int_T tid ) ; void MdlTerminate ( void ) ; void
MdlInitializeSizes ( void ) ; void MdlInitializeSampleTimes ( void ) ;
SimStruct * raccel_register_model ( void ) ;
#endif
