#include "rt_logging_mmi.h"
#include "HardThreshold_capi.h"
#include <math.h>
#include "HardThreshold.h"
#include "HardThreshold_private.h"
#include "HardThreshold_dt.h"
extern void * CreateDiagnosticAsVoidPtr_wrapper ( const char * id , int nargs
, ... ) ; RTWExtModeInfo * gblRTWExtModeInfo = NULL ; extern boolean_T
gblExtModeStartPktReceived ; void raccelForceExtModeShutdown ( ) { if ( !
gblExtModeStartPktReceived ) { boolean_T stopRequested = false ;
rtExtModeWaitForStartPkt ( gblRTWExtModeInfo , 2 , & stopRequested ) ; }
rtExtModeShutdown ( 2 ) ; }
#include "slsv_diagnostic_codegen_c_api.h"
const int_T gblNumToFiles = 0 ; const int_T gblNumFrFiles = 2 ; const int_T
gblNumFrWksBlocks = 0 ;
#ifdef RSIM_WITH_SOLVER_MULTITASKING
boolean_T gbl_raccel_isMultitasking = 1 ;
#else
boolean_T gbl_raccel_isMultitasking = 0 ;
#endif
boolean_T gbl_raccel_tid01eq = 0 ; int_T gbl_raccel_NumST = 3 ; const char_T
* gbl_raccel_Version = "9.3 (R2020a) 18-Nov-2019" ; void
raccel_setup_MMIStateLog ( SimStruct * S ) {
#ifdef UseMMIDataLogging
rt_FillStateSigInfoFromMMI ( ssGetRTWLogInfo ( S ) , & ssGetErrorStatus ( S )
) ;
#else
UNUSED_PARAMETER ( S ) ;
#endif
} static DataMapInfo rt_dataMapInfo ; DataMapInfo * rt_dataMapInfoPtr = &
rt_dataMapInfo ; rtwCAPI_ModelMappingInfo * rt_modelMapInfoPtr = & (
rt_dataMapInfo . mmi ) ; const char * gblSlvrJacPatternFileName =
"slprj\\raccel\\HardThreshold\\HardThreshold_Jpattern.mat" ; const int_T
gblNumRootInportBlks = 0 ; const int_T gblNumModelInputs = 0 ; extern
rtInportTUtable * gblInportTUtables ; extern const char * gblInportFileName ;
extern void * gblAperiodicPartitionHitTimes ; const int_T
gblInportDataTypeIdx [ ] = { - 1 } ; const int_T gblInportDims [ ] = { - 1 }
; const int_T gblInportComplex [ ] = { - 1 } ; const int_T
gblInportInterpoFlag [ ] = { - 1 } ; const int_T gblInportContinuous [ ] = {
- 1 } ; int_T enableFcnCallFlag [ ] = { 1 , 1 , 1 } ; const char *
raccelLoadInputsAndAperiodicHitTimes ( const char * inportFileName , int *
matFileFormat ) { return rt_RapidReadInportsMatFile ( inportFileName ,
matFileFormat , 1 ) ; } FrFInfo HardThreshold_gblFrFInfo [ 2 ] ;
#include "simstruc.h"
#include "fixedpoint.h"
B rtB ; DW rtDW ; static SimStruct model_S ; SimStruct * const rtS = &
model_S ; void MdlInitialize ( void ) { rtDW . a4ipisgwzj = rtP .
Memory2_InitialCondition ; rtDW . hwmpquokvc = rtP . Delay_InitialCondition ;
rtDW . iqegvrxn5l = rtP . Count_reg_InitialCondition ; rtDW . indimadzdu =
rtP . Memory1_InitialCondition ; } void MdlStart ( void ) { { void * *
slioCatalogueAddr = rt_slioCatalogueAddr ( ) ; void * r2 = ( NULL ) ; void *
* pOSigstreamManagerAddr = ( NULL ) ; const int maxErrorBufferSize = 16384 ;
char errMsgCreatingOSigstreamManager [ 16384 ] ; bool
errorCreatingOSigstreamManager = false ; const char *
errorAddingR2SharedResource = ( NULL ) ; * slioCatalogueAddr =
rtwGetNewSlioCatalogue ( rt_GetMatSigLogSelectorFileName ( ) ) ;
errorAddingR2SharedResource = rtwAddR2SharedResource (
rtwGetPointerFromUniquePtr ( rt_slioCatalogue ( ) ) , 1 ) ; if (
errorAddingR2SharedResource != ( NULL ) ) { rtwTerminateSlioCatalogue (
slioCatalogueAddr ) ; * slioCatalogueAddr = ( NULL ) ; ssSetErrorStatus ( rtS
, errorAddingR2SharedResource ) ; return ; } r2 = rtwGetR2SharedResource (
rtwGetPointerFromUniquePtr ( rt_slioCatalogue ( ) ) ) ;
pOSigstreamManagerAddr = rt_GetOSigstreamManagerAddr ( ) ;
errorCreatingOSigstreamManager = rtwOSigstreamManagerCreateInstance (
rt_GetMatSigLogSelectorFileName ( ) , r2 , pOSigstreamManagerAddr ,
errMsgCreatingOSigstreamManager , maxErrorBufferSize ) ; if (
errorCreatingOSigstreamManager ) { * pOSigstreamManagerAddr = ( NULL ) ;
ssSetErrorStatus ( rtS , errMsgCreatingOSigstreamManager ) ; return ; } } {
bool externalInputIsInDatasetFormat = false ; void * pISigstreamManager =
rt_GetISigstreamManager ( ) ; rtwISigstreamManagerGetInputIsInDatasetFormat (
pISigstreamManager , & externalInputIsInDatasetFormat ) ; if (
externalInputIsInDatasetFormat ) { } } { void * slioCatalogue =
rt_slioCatalogue ( ) ? rtwGetPointerFromUniquePtr ( rt_slioCatalogue ( ) ) :
sdiGetSlioCatalogue ( rt_dataMapInfo . mmi . InstanceMap . fullPath ) ; if (
! slioCatalogue || ! rtwIsLoggingToFile ( slioCatalogue ) ) { { {
sdiSignalSourceInfoU srcInfo ; sdiLabelU loggedName = sdiGetLabelFromChars (
"EC spikes above threshold AND refractory" ) ; sdiLabelU origSigName =
sdiGetLabelFromChars ( "EC spikes above threshold AND refractory" ) ;
sdiLabelU propName = sdiGetLabelFromChars ( "" ) ; sdiLabelU blockPath =
sdiGetLabelFromChars ( "HardThreshold/Refractory check" ) ; sdiLabelU
blockSID = sdiGetLabelFromChars ( "" ) ; sdiLabelU subPath =
sdiGetLabelFromChars ( "" ) ; sdiDims sigDims ; sdiLabelU sigName =
sdiGetLabelFromChars ( "EC spikes above threshold AND refractory" ) ;
sdiAsyncRepoDataTypeHandle hDT = sdiAsyncRepoGetBuiltInDataTypeHandle (
DATA_TYPE_BOOLEAN ) ; { sdiComplexity sigComplexity = REAL ;
sdiSampleTimeContinuity stCont = SAMPLE_TIME_DISCRETE ; int_T sigDimsArray [
1 ] = { 1 } ; sigDims . nDims = 1 ; sigDims . dimensions = sigDimsArray ;
srcInfo . numBlockPathElems = 1 ; srcInfo . fullBlockPath = ( sdiFullBlkPathU
) & blockPath ; srcInfo . SID = ( sdiSignalIDU ) & blockSID ; srcInfo .
subPath = subPath ; srcInfo . portIndex = 0 + 1 ; srcInfo . signalName =
sigName ; srcInfo . sigSourceUUID = 0 ; rtDW . ctp1gugaiy . AQHandles =
sdiAsyncRepoCreateAsyncioQueue ( hDT , & srcInfo , rt_dataMapInfo . mmi .
InstanceMap . fullPath , "c7176de5-a761-45f4-88d9-23ca090540bd" ,
sigComplexity , & sigDims , DIMENSIONS_MODE_FIXED , stCont , "" ) ; if ( rtDW
. ctp1gugaiy . AQHandles ) { sdiSetSignalSampleTimeString ( rtDW . ctp1gugaiy
. AQHandles , "3.33333e-05" , 3.3333333333333335E-5 , ssGetTFinal ( rtS ) ) ;
sdiSetRunStartTime ( rtDW . ctp1gugaiy . AQHandles , ssGetTaskTime ( rtS , 1
) ) ; sdiAsyncRepoSetSignalExportSettings ( rtDW . ctp1gugaiy . AQHandles , 1
, 0 ) ; sdiAsyncRepoSetSignalExportName ( rtDW . ctp1gugaiy . AQHandles ,
loggedName , origSigName , propName ) ; } sdiFreeLabel ( sigName ) ;
sdiFreeLabel ( loggedName ) ; sdiFreeLabel ( origSigName ) ; sdiFreeLabel (
propName ) ; sdiFreeLabel ( blockPath ) ; sdiFreeLabel ( blockSID ) ;
sdiFreeLabel ( subPath ) ; } } } } rtDW . ctp1gugaiy . SlioLTF = ( NULL ) ; {
void * treeVector = ( NULL ) ; void * accessor = ( NULL ) ; const void *
signalDescriptor = ( NULL ) ; void * loggingInterval = ( NULL ) ; char *
datasetName = "tmp_raccel_logsout" ; void * slioCatalogue = rt_slioCatalogue
( ) ? rtwGetPointerFromUniquePtr ( rt_slioCatalogue ( ) ) :
sdiGetSlioCatalogue ( rt_dataMapInfo . mmi . InstanceMap . fullPath ) ; if (
slioCatalogue && rtwIsLoggingToFile ( slioCatalogue ) ) { treeVector =
rtwGetTreeVector ( ) ; { int_T sigDimsArray [ 1 ] = { 1 } ; rtwAddLeafNode (
8 , "EC spikes above threshold AND refractory" , "zoh" , 0 , ( unsigned int *
) sigDimsArray , 1 , "logical" , "" , "3.33333e-05" , 3.3333333333333335E-5 ,
ssGetTFinal ( rtS ) , treeVector ) ; } signalDescriptor =
rtwGetSignalDescriptor ( treeVector , 1 , 1 , 0 , 1 ,
"EC spikes above threshold AND refractory" , "" , rt_dataMapInfo . mmi .
InstanceMap . fullPath , "HardThreshold/Refractory check" , 1 , 0 ,
slioCatalogue , ( NULL ) , 0 , ( NULL ) , 0 ) ; if ( ! rt_slioCatalogue ( ) )
{ sdiSlioIsLoggingSignal ( rt_dataMapInfo . mmi . InstanceMap . fullPath ,
"HardThreshold/Refractory check" , 1 ,
"EC spikes above threshold AND refractory" ) ; } if ( rtwLoggingOverride (
signalDescriptor , slioCatalogue ) ) { if ( ssGetRootSS ( rtS ) -> mdlInfo ->
rtwLogInfo ) { loggingInterval = rtliGetLoggingInterval ( ssGetRootSS ( rtS )
-> mdlInfo -> rtwLogInfo ) ; } else { loggingInterval =
sdiGetLoggingIntervals ( rt_dataMapInfo . mmi . InstanceMap . fullPath ) ;
datasetName = "" ; } accessor = rtwGetAccessor ( signalDescriptor ,
loggingInterval ) ; rtwAddR2Client ( accessor , signalDescriptor ,
slioCatalogue , datasetName , 1 ) ; rtDW . ctp1gugaiy . SlioLTF = accessor ;
} } } } { char fileName [ 509 ] =
 "D:\\File\\Shared\\Google Drive\\IIT - Neuroengineering\\Project\\WP1\\Data\\KampffLab\\2014_11_25_Pair_3_0\\Juxta_sim.mat"
; const char * blockpath = "HardThreshold/Ground truth - Juxtacellular" ; if
( slIsRapidAcceleratorSimulating ( ) ) { rt_RAccelReplaceFromFilename (
blockpath , fileName ) ; } { void * fp = ( NULL ) ; const char * errMsg = (
NULL ) ; errMsg = rtwMatFileLoaderCollectionCreateInstance ( 1 , & fp ) ; if
( errMsg != ( NULL ) ) { ssSetErrorStatus ( rtS , errMsg ) ; return ; } rtDW
. pj5i3u21oj . PrevTimePtr = fp ; { unsigned char groundValue [ ] = { 0U , 0U
, 0U , 0U , 0U , 0U , 0U , 0U } ; const int enumNStrings = 0 ; const char * *
enumStrings = ( NULL ) ; const int32_T * enumValues = ( NULL ) ; int_T
dimensions [ 1 ] = { 1 } ; errMsg = rtwMatFileLoaderCollectionAddElement ( 1
, fp , fileName , "" , 0 , 0 , 0 , 0 , groundValue , "double" , 0 , 1 ,
dimensions , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , enumNStrings , enumStrings ,
enumValues , 1 , 1 , "HardThreshold/Ground truth - Juxtacellular" ) ; if (
errMsg != ( NULL ) ) { ssSetErrorStatus ( rtS , errMsg ) ; return ; } } } } {
char fileName [ 509 ] =
 "D:\\File\\Shared\\Google Drive\\IIT - Neuroengineering\\Project\\WP1\\Data\\KampffLab\\2014_11_25_Pair_3_0\\EC_sim.mat"
; const char * blockpath = "HardThreshold/EC recording - Channel 29" ; if (
slIsRapidAcceleratorSimulating ( ) ) { rt_RAccelReplaceFromFilename (
blockpath , fileName ) ; } { void * fp = ( NULL ) ; const char * errMsg = (
NULL ) ; errMsg = rtwMatFileLoaderCollectionCreateInstance ( 1 , & fp ) ; if
( errMsg != ( NULL ) ) { ssSetErrorStatus ( rtS , errMsg ) ; return ; } rtDW
. fmlytn2sqn . PrevTimePtr = fp ; { unsigned char groundValue [ ] = { 0U , 0U
, 0U , 0U , 0U , 0U , 0U , 0U } ; const int enumNStrings = 0 ; const char * *
enumStrings = ( NULL ) ; const int32_T * enumValues = ( NULL ) ; int_T
dimensions [ 1 ] = { 1 } ; errMsg = rtwMatFileLoaderCollectionAddElement ( 1
, fp , fileName , "" , 0 , 0 , 0 , 0 , groundValue , "double" , 0 , 1 ,
dimensions , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , enumNStrings , enumStrings ,
enumValues , 1 , 1 , "HardThreshold/EC recording - Channel 29" ) ; if (
errMsg != ( NULL ) ) { ssSetErrorStatus ( rtS , errMsg ) ; return ; } } } }
MdlInitialize ( ) ; } void MdlOutputs ( int_T tid ) { uint32_T ewjjbe05be ;
uint64_T eimsxe1usn ; int64_T l1ljakqkec ; int64_T tmp ; if ( ssIsSampleHit (
rtS , 1 , 0 ) ) { { void * fp = ( void * ) rtDW . pj5i3u21oj . PrevTimePtr ;
const char * errMsg = ( NULL ) ; if ( fp != ( NULL ) && ( ssIsMajorTimeStep (
rtS ) || ! 0 ) ) { real_T t = ssGetTaskTime ( rtS , 1 ) ; { void * y = & rtB
. ouzstej054 ; errMsg = rtwMatFileLoaderCollectionGetOutput ( 1 , fp , 0 , t
, ssIsMajorTimeStep ( rtS ) , & y ) ; if ( errMsg != ( NULL ) ) {
ssSetErrorStatus ( rtS , errMsg ) ; return ; } } } } { void * fp = ( void * )
rtDW . fmlytn2sqn . PrevTimePtr ; const char * errMsg = ( NULL ) ; if ( fp !=
( NULL ) && ( ssIsMajorTimeStep ( rtS ) || ! 0 ) ) { real_T t = ssGetTaskTime
( rtS , 1 ) ; { void * y = & rtB . o4ysm25xty ; errMsg =
rtwMatFileLoaderCollectionGetOutput ( 1 , fp , 0 , t , ssIsMajorTimeStep (
rtS ) , & y ) ; if ( errMsg != ( NULL ) ) { ssSetErrorStatus ( rtS , errMsg )
; return ; } } } } rtB . jwpromv5g4 = ( rtB . o4ysm25xty <= rtP .
CompareToConstant_const ) ; rtB . m1q11hgbsi = ( rtB . jwpromv5g4 && ( rtB .
o4ysm25xty - rtDW . a4ipisgwzj >= rtP . Constant_Value ) ) ; rtB . oeany0t1zb
= rtP . Gain_Gain * ( real_T ) rtDW . iqegvrxn5l ; if ( rtB . oeany0t1zb >=
rtP . Refractorycheck_const ) { rtB . lyxlvo5d3m = ( rtB . m1q11hgbsi && ( !
rtDW . hwmpquokvc ) ) ; } else { rtB . lyxlvo5d3m = ( rtP .
Constant_Value_chs5tmnkek != 0.0 ) ; } { if ( ( rtDW . ctp1gugaiy . AQHandles
|| rtDW . ctp1gugaiy . SlioLTF ) && ssGetLogOutput ( rtS ) ) {
sdiSlioSdiWriteSignal ( rtDW . ctp1gugaiy . AQHandles , rtDW . ctp1gugaiy .
SlioLTF , 0 , ssGetTaskTime ( rtS , 1 ) , ( char * ) & rtB . lyxlvo5d3m + 0 )
; } } if ( rtB . ksas4hwhb5 ) { tmp = rtDW . iqegvrxn5l ; l1ljakqkec = rtP .
Step_value_Value ; tmp = ( ( tmp & 4294967296ULL ) != 0ULL ? tmp | -
4294967296LL : tmp & 4294967295LL ) - ( ( l1ljakqkec & 4294967296ULL ) !=
0ULL ? l1ljakqkec | - 4294967296LL : l1ljakqkec & 4294967295LL ) ; l1ljakqkec
= ( tmp & 4294967296ULL ) != 0ULL ? tmp | - 4294967296LL : tmp & 4294967295LL
; if ( l1ljakqkec >= rtP . Switch_wrap_Threshold ) { ewjjbe05be = ( uint32_T
) l1ljakqkec ; } else { tmp = ( int64_T ) rtP . Mod_value_Value_iaowpfgwjx ;
tmp = ( ( l1ljakqkec & 17179869184ULL ) != 0ULL ? l1ljakqkec | -
17179869184LL : l1ljakqkec & 17179869183LL ) + ( ( tmp & 17179869184ULL ) !=
0ULL ? tmp | - 17179869184LL : tmp & 17179869183LL ) ; ewjjbe05be = (
uint32_T ) ( ( tmp & 17179869184ULL ) != 0ULL ? tmp | - 17179869184LL : tmp &
17179869183LL ) ; } } else { eimsxe1usn = ( ( rtP . Step_value_Value &
8589934591ULL ) + ( rtDW . iqegvrxn5l & 8589934591ULL ) ) & 8589934591ULL ;
if ( eimsxe1usn > rtP . Switch_wrap_Threshold_gqqrfmqwsl ) { tmp = ( int64_T
) eimsxe1usn ; l1ljakqkec = ( int64_T ) rtP . Mod_value_Value ; tmp = ( ( tmp
& 17179869184ULL ) != 0ULL ? tmp | - 17179869184LL : tmp & 17179869183LL ) -
( ( l1ljakqkec & 17179869184ULL ) != 0ULL ? l1ljakqkec | - 17179869184LL :
l1ljakqkec & 17179869183LL ) ; ewjjbe05be = ( uint32_T ) ( ( tmp &
17179869184ULL ) != 0ULL ? tmp | - 17179869184LL : tmp & 17179869183LL ) ; }
else { ewjjbe05be = ( uint32_T ) eimsxe1usn ; } } if ( rtDW . indimadzdu ) {
rtB . jesi2z0j4g = rtP . Init_value_Value ; } else if ( rtP .
const_load_Value ) { rtB . jesi2z0j4g = rtP . const_load_val_Value ; } else
if ( rtP . const_enb_Value ) { if ( rtP . Free_running_Value ) { rtB .
jesi2z0j4g = ewjjbe05be ; } else if ( rtDW . iqegvrxn5l == rtP .
Max_value_const ) { rtB . jesi2z0j4g = rtP . From_value_Value ; } else { rtB
. jesi2z0j4g = ewjjbe05be ; } } else { rtB . jesi2z0j4g = rtDW . iqegvrxn5l ;
} } UNUSED_PARAMETER ( tid ) ; } void MdlOutputsTID2 ( int_T tid ) { rtB .
ksas4hwhb5 = ( muDoubleScalarSign ( rtP . HDLCounter_CountStep ) == 1.0 ) ^
rtP . const_dir_Value ; UNUSED_PARAMETER ( tid ) ; } void MdlUpdate ( int_T
tid ) { if ( ssIsSampleHit ( rtS , 1 , 0 ) ) { rtDW . a4ipisgwzj = rtB .
o4ysm25xty ; rtDW . hwmpquokvc = rtB . m1q11hgbsi ; rtDW . iqegvrxn5l = rtB .
jesi2z0j4g ; rtDW . indimadzdu = rtB . lyxlvo5d3m ; } UNUSED_PARAMETER ( tid
) ; } void MdlUpdateTID2 ( int_T tid ) { UNUSED_PARAMETER ( tid ) ; } void
MdlTerminate ( void ) { { const char * errMsg = ( NULL ) ; void * fp = ( void
* ) rtDW . pj5i3u21oj . PrevTimePtr ; if ( fp != ( NULL ) ) { errMsg =
rtwMatFileLoaderCollectionDestroyInstance ( 1 , fp ) ; if ( errMsg != ( NULL
) ) { ssSetErrorStatus ( rtS , errMsg ) ; return ; } } } { const char *
errMsg = ( NULL ) ; void * fp = ( void * ) rtDW . fmlytn2sqn . PrevTimePtr ;
if ( fp != ( NULL ) ) { errMsg = rtwMatFileLoaderCollectionDestroyInstance (
1 , fp ) ; if ( errMsg != ( NULL ) ) { ssSetErrorStatus ( rtS , errMsg ) ;
return ; } } } { if ( rtDW . ctp1gugaiy . AQHandles ) { sdiTerminateStreaming
( & rtDW . ctp1gugaiy . AQHandles ) ; } if ( rtDW . ctp1gugaiy . SlioLTF ) {
rtwDestructAccessorPointer ( rtDW . ctp1gugaiy . SlioLTF ) ; } } if (
rt_slioCatalogue ( ) != ( NULL ) ) { void * * slioCatalogueAddr =
rt_slioCatalogueAddr ( ) ; rtwSaveDatasetsToMatFile (
rtwGetPointerFromUniquePtr ( rt_slioCatalogue ( ) ) ,
rt_GetMatSigstreamLoggingFileName ( ) ) ; rtwTerminateSlioCatalogue (
slioCatalogueAddr ) ; * slioCatalogueAddr = NULL ; } } void
MdlInitializeSizes ( void ) { ssSetNumContStates ( rtS , 0 ) ; ssSetNumY (
rtS , 0 ) ; ssSetNumU ( rtS , 0 ) ; ssSetDirectFeedThrough ( rtS , 0 ) ;
ssSetNumSampleTimes ( rtS , 2 ) ; ssSetNumBlocks ( rtS , 57 ) ;
ssSetNumBlockIO ( rtS , 8 ) ; ssSetNumBlockParams ( rtS , 23 ) ; } void
MdlInitializeSampleTimes ( void ) { ssSetSampleTime ( rtS , 0 , 0.0 ) ;
ssSetSampleTime ( rtS , 1 , 3.3333333333333335E-5 ) ; ssSetOffsetTime ( rtS ,
0 , 1.0 ) ; ssSetOffsetTime ( rtS , 1 , 0.0 ) ; } void raccel_set_checksum (
) { ssSetChecksumVal ( rtS , 0 , 1311056555U ) ; ssSetChecksumVal ( rtS , 1 ,
613106949U ) ; ssSetChecksumVal ( rtS , 2 , 2840415959U ) ; ssSetChecksumVal
( rtS , 3 , 91085993U ) ; }
#if defined(_MSC_VER)
#pragma optimize( "", off )
#endif
SimStruct * raccel_register_model ( void ) { static struct _ssMdlInfo mdlInfo
; ( void ) memset ( ( char * ) rtS , 0 , sizeof ( SimStruct ) ) ; ( void )
memset ( ( char * ) & mdlInfo , 0 , sizeof ( struct _ssMdlInfo ) ) ;
ssSetMdlInfoPtr ( rtS , & mdlInfo ) ; { static time_T mdlPeriod [
NSAMPLE_TIMES ] ; static time_T mdlOffset [ NSAMPLE_TIMES ] ; static time_T
mdlTaskTimes [ NSAMPLE_TIMES ] ; static int_T mdlTsMap [ NSAMPLE_TIMES ] ;
static int_T mdlSampleHits [ NSAMPLE_TIMES ] ; static boolean_T
mdlTNextWasAdjustedPtr [ NSAMPLE_TIMES ] ; static int_T mdlPerTaskSampleHits
[ NSAMPLE_TIMES * NSAMPLE_TIMES ] ; static time_T mdlTimeOfNextSampleHit [
NSAMPLE_TIMES ] ; { int_T i ; for ( i = 0 ; i < NSAMPLE_TIMES ; i ++ ) {
mdlPeriod [ i ] = 0.0 ; mdlOffset [ i ] = 0.0 ; mdlTaskTimes [ i ] = 0.0 ;
mdlTsMap [ i ] = i ; mdlSampleHits [ i ] = 1 ; } } ssSetSampleTimePtr ( rtS ,
& mdlPeriod [ 0 ] ) ; ssSetOffsetTimePtr ( rtS , & mdlOffset [ 0 ] ) ;
ssSetSampleTimeTaskIDPtr ( rtS , & mdlTsMap [ 0 ] ) ; ssSetTPtr ( rtS , &
mdlTaskTimes [ 0 ] ) ; ssSetSampleHitPtr ( rtS , & mdlSampleHits [ 0 ] ) ;
ssSetTNextWasAdjustedPtr ( rtS , & mdlTNextWasAdjustedPtr [ 0 ] ) ;
ssSetPerTaskSampleHitsPtr ( rtS , & mdlPerTaskSampleHits [ 0 ] ) ;
ssSetTimeOfNextSampleHitPtr ( rtS , & mdlTimeOfNextSampleHit [ 0 ] ) ; }
ssSetSolverMode ( rtS , SOLVER_MODE_SINGLETASKING ) ; { ssSetBlockIO ( rtS ,
( ( void * ) & rtB ) ) ; ( void ) memset ( ( ( void * ) & rtB ) , 0 , sizeof
( B ) ) ; } { void * dwork = ( void * ) & rtDW ; ssSetRootDWork ( rtS , dwork
) ; ( void ) memset ( dwork , 0 , sizeof ( DW ) ) ; } { static
DataTypeTransInfo dtInfo ; ( void ) memset ( ( char_T * ) & dtInfo , 0 ,
sizeof ( dtInfo ) ) ; ssSetModelMappingInfo ( rtS , & dtInfo ) ; dtInfo .
numDataTypes = 19 ; dtInfo . dataTypeSizes = & rtDataTypeSizes [ 0 ] ; dtInfo
. dataTypeNames = & rtDataTypeNames [ 0 ] ; dtInfo . BTransTable = &
rtBTransTable ; dtInfo . PTransTable = & rtPTransTable ; dtInfo .
dataTypeInfoTable = rtDataTypeInfoTable ; }
HardThreshold_InitializeDataMapInfo ( ) ; ssSetIsRapidAcceleratorActive ( rtS
, true ) ; ssSetRootSS ( rtS , rtS ) ; ssSetVersion ( rtS ,
SIMSTRUCT_VERSION_LEVEL2 ) ; ssSetModelName ( rtS , "HardThreshold" ) ;
ssSetPath ( rtS , "HardThreshold" ) ; ssSetTStart ( rtS , 0.0 ) ; ssSetTFinal
( rtS , 100.0 ) ; { static RTWLogInfo rt_DataLoggingInfo ; rt_DataLoggingInfo
. loggingInterval = NULL ; ssSetRTWLogInfo ( rtS , & rt_DataLoggingInfo ) ; }
{ { static int_T rt_LoggedStateWidths [ ] = { 1 , 1 } ; static int_T
rt_LoggedStateNumDimensions [ ] = { 1 , 1 } ; static int_T
rt_LoggedStateDimensions [ ] = { 1 , 1 } ; static boolean_T
rt_LoggedStateIsVarDims [ ] = { 0 , 0 } ; static BuiltInDTypeId
rt_LoggedStateDataTypeIds [ ] = { SS_UINT32 , SS_BOOLEAN } ; static int_T
rt_LoggedStateComplexSignals [ ] = { 0 , 0 } ; static RTWPreprocessingFcnPtr
rt_LoggingStatePreprocessingFcnPtrs [ ] = { ( NULL ) , ( NULL ) } ; static
const char_T * rt_LoggedStateLabels [ ] = { "DSTATE" , "DSTATE" } ; static
const char_T * rt_LoggedStateBlockNames [ ] = {
"HardThreshold/Refractory check/Time since last event/HDL Counter/Count_reg"
, "HardThreshold/Edge\nDetector/Delay" } ; static const char_T *
rt_LoggedStateNames [ ] = { "DSTATE" , "DSTATE" } ; static boolean_T
rt_LoggedStateCrossMdlRef [ ] = { 0 , 0 } ; static RTWLogDataTypeConvert
rt_RTWLogDataTypeConvert [ ] = { { 0 , SS_UINT32 , SS_UINT32 , 0 , 0 , 0 ,
1.0 , 0 , 0.0 } , { 0 , SS_BOOLEAN , SS_BOOLEAN , 0 , 0 , 0 , 1.0 , 0 , 0.0 }
} ; static RTWLogSignalInfo rt_LoggedStateSignalInfo = { 2 ,
rt_LoggedStateWidths , rt_LoggedStateNumDimensions , rt_LoggedStateDimensions
, rt_LoggedStateIsVarDims , ( NULL ) , ( NULL ) , rt_LoggedStateDataTypeIds ,
rt_LoggedStateComplexSignals , ( NULL ) , rt_LoggingStatePreprocessingFcnPtrs
, { rt_LoggedStateLabels } , ( NULL ) , ( NULL ) , ( NULL ) , {
rt_LoggedStateBlockNames } , { rt_LoggedStateNames } ,
rt_LoggedStateCrossMdlRef , rt_RTWLogDataTypeConvert } ; static void *
rt_LoggedStateSignalPtrs [ 2 ] ; rtliSetLogXSignalPtrs ( ssGetRTWLogInfo (
rtS ) , ( LogSignalPtrsType ) rt_LoggedStateSignalPtrs ) ;
rtliSetLogXSignalInfo ( ssGetRTWLogInfo ( rtS ) , & rt_LoggedStateSignalInfo
) ; rt_LoggedStateSignalPtrs [ 0 ] = ( void * ) & rtDW . iqegvrxn5l ;
rt_LoggedStateSignalPtrs [ 1 ] = ( void * ) & rtDW . hwmpquokvc ; }
rtliSetLogT ( ssGetRTWLogInfo ( rtS ) , "tout" ) ; rtliSetLogX (
ssGetRTWLogInfo ( rtS ) , "" ) ; rtliSetLogXFinal ( ssGetRTWLogInfo ( rtS ) ,
"" ) ; rtliSetLogVarNameModifier ( ssGetRTWLogInfo ( rtS ) , "none" ) ;
rtliSetLogFormat ( ssGetRTWLogInfo ( rtS ) , 4 ) ; rtliSetLogMaxRows (
ssGetRTWLogInfo ( rtS ) , 0 ) ; rtliSetLogDecimation ( ssGetRTWLogInfo ( rtS
) , 1 ) ; rtliSetLogY ( ssGetRTWLogInfo ( rtS ) , "" ) ;
rtliSetLogYSignalInfo ( ssGetRTWLogInfo ( rtS ) , ( NULL ) ) ;
rtliSetLogYSignalPtrs ( ssGetRTWLogInfo ( rtS ) , ( NULL ) ) ; } { static
ssSolverInfo slvrInfo ; ssSetStepSize ( rtS , 3.3333333333333335E-5 ) ;
ssSetMinStepSize ( rtS , 0.0 ) ; ssSetMaxNumMinSteps ( rtS , - 1 ) ;
ssSetMinStepViolatedError ( rtS , 0 ) ; ssSetMaxStepSize ( rtS ,
3.3333333333333335E-5 ) ; ssSetSolverMaxOrder ( rtS , - 1 ) ;
ssSetSolverRefineFactor ( rtS , 1 ) ; ssSetOutputTimes ( rtS , ( NULL ) ) ;
ssSetNumOutputTimes ( rtS , 0 ) ; ssSetOutputTimesOnly ( rtS , 0 ) ;
ssSetOutputTimesIndex ( rtS , 0 ) ; ssSetZCCacheNeedsReset ( rtS , 0 ) ;
ssSetDerivCacheNeedsReset ( rtS , 0 ) ; ssSetNumNonContDerivSigInfos ( rtS ,
0 ) ; ssSetNonContDerivSigInfos ( rtS , ( NULL ) ) ; ssSetSolverInfo ( rtS ,
& slvrInfo ) ; ssSetSolverName ( rtS , "VariableStepDiscrete" ) ;
ssSetVariableStepSolver ( rtS , 1 ) ; ssSetSolverConsistencyChecking ( rtS ,
0 ) ; ssSetSolverAdaptiveZcDetection ( rtS , 0 ) ;
ssSetSolverRobustResetMethod ( rtS , 0 ) ; ssSetSolverStateProjection ( rtS ,
0 ) ; ssSetSolverMassMatrixType ( rtS , ( ssMatrixType ) 0 ) ;
ssSetSolverMassMatrixNzMax ( rtS , 0 ) ; ssSetModelOutputs ( rtS , MdlOutputs
) ; ssSetModelLogData ( rtS , rt_UpdateTXYLogVars ) ;
ssSetModelLogDataIfInInterval ( rtS , rt_UpdateTXXFYLogVars ) ;
ssSetModelUpdate ( rtS , MdlUpdate ) ; ssSetTNextTid ( rtS , INT_MIN ) ;
ssSetTNext ( rtS , rtMinusInf ) ; ssSetSolverNeedsReset ( rtS ) ;
ssSetNumNonsampledZCs ( rtS , 0 ) ; } ssSetChecksumVal ( rtS , 0 ,
1311056555U ) ; ssSetChecksumVal ( rtS , 1 , 613106949U ) ; ssSetChecksumVal
( rtS , 2 , 2840415959U ) ; ssSetChecksumVal ( rtS , 3 , 91085993U ) ; {
static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE ; static
RTWExtModeInfo rt_ExtModeInfo ; static const sysRanDType * systemRan [ 10 ] ;
gblRTWExtModeInfo = & rt_ExtModeInfo ; ssSetRTWExtModeInfo ( rtS , &
rt_ExtModeInfo ) ; rteiSetSubSystemActiveVectorAddresses ( & rt_ExtModeInfo ,
systemRan ) ; systemRan [ 0 ] = & rtAlwaysEnabled ; systemRan [ 1 ] = &
rtAlwaysEnabled ; systemRan [ 2 ] = & rtAlwaysEnabled ; systemRan [ 3 ] = &
rtAlwaysEnabled ; systemRan [ 4 ] = & rtAlwaysEnabled ; systemRan [ 5 ] = &
rtAlwaysEnabled ; systemRan [ 6 ] = & rtAlwaysEnabled ; systemRan [ 7 ] = &
rtAlwaysEnabled ; systemRan [ 8 ] = & rtAlwaysEnabled ; systemRan [ 9 ] = &
rtAlwaysEnabled ; rteiSetModelMappingInfoPtr ( ssGetRTWExtModeInfo ( rtS ) ,
& ssGetModelMappingInfo ( rtS ) ) ; rteiSetChecksumsPtr ( ssGetRTWExtModeInfo
( rtS ) , ssGetChecksums ( rtS ) ) ; rteiSetTPtr ( ssGetRTWExtModeInfo ( rtS
) , ssGetTPtr ( rtS ) ) ; } return rtS ; }
#if defined(_MSC_VER)
#pragma optimize( "", on )
#endif
const int_T gblParameterTuningTid = 2 ; void MdlOutputsParameterSampleTime (
int_T tid ) { MdlOutputsTID2 ( tid ) ; }
