typedef void (__stdcall *TCommandHandle)(char *,short);

#define GEP_API __declspec(dllimport) short __stdcall

#ifdef __cplusplus
extern "C" {
#endif

GEP_API GT_AbptStp(unsigned short axis);
GEP_API GT_AlarmOff(unsigned short axis);
GEP_API GT_AlarmOn(unsigned short axis);
GEP_API GT_AuStpOff(unsigned short axis);
GEP_API GT_AuStpOn(unsigned short axis);
GEP_API GT_AxisOff(unsigned short axis);
GEP_API GT_AxisOn(unsigned short axis);
GEP_API GT_CaptHome(unsigned short axis);
GEP_API GT_CaptIndex(unsigned short axis);
GEP_API GT_CaptProb(void);
GEP_API GT_Close(void);
GEP_API GT_ClrEncPos(unsigned short axis);
GEP_API GT_ClrSts(unsigned short axis);
GEP_API GT_DrvRst(unsigned short axis);
GEP_API GT_EncOff(unsigned short axis);
GEP_API GT_EncOn(unsigned short axis);
GEP_API GT_EncPos(unsigned short axis,long *pos);
GEP_API GT_EncSns(unsigned short sense);
GEP_API GT_EncVel(unsigned short axis,double *vel);
GEP_API GT_ExInpt(unsigned short *value);
GEP_API GT_ExOpt(unsigned short value);
GEP_API GT_ExOptBit(unsigned short bit,unsigned short value);
GEP_API GT_GetAcc(unsigned short axis,double *acc);
GEP_API GT_GetAdc(unsigned short channel,short *voltage);
GEP_API GT_GetAtlErr(unsigned short axis,long *error);
GEP_API GT_GetAtlPos(unsigned short axis,long *pos);
GEP_API GT_GetAtlVel(unsigned short axis,double *vel);
GEP_API GT_GetCapt(unsigned short axis,long *pos);
GEP_API GT_GetClock(unsigned long *value);
GEP_API GT_GetCmdSts(unsigned short *status);
GEP_API GT_GetHomeSwt(unsigned short *home);
GEP_API GT_GetExOpt(unsigned short *value);
GEP_API GT_GetJerk(unsigned short axis,double *jerk);
GEP_API GT_GetLmtSwt(unsigned short *limit);
GEP_API GT_GetMAcc(unsigned short axis,double *macc);
GEP_API GT_GetMode(unsigned short axis,unsigned short *mode);
GEP_API GT_GetPos(unsigned short axis,long *pos);
GEP_API GT_GetPosErr(unsigned short axis,unsigned short *error);
GEP_API GT_GetPrfPos(unsigned short axis,long *pos);
GEP_API GT_GetPrfVel(unsigned short axis,double *vel);
GEP_API GT_GetRatio(unsigned short axis,double *ratio);
GEP_API GT_GetSts(unsigned short axis,unsigned long *status);
GEP_API GT_GetVel(unsigned short axis,double *vel);
GEP_API GT_HardRst(void);
GEP_API GT_HomeSns(unsigned short sense);
GEP_API GT_HookCommand(TCommandHandle CommandHandle);
GEP_API GT_IndexSns(unsigned short sense);
GEP_API GT_LmtSns(unsigned short sense);
GEP_API GT_LmtsOff(unsigned short axis);
GEP_API GT_LmtsOn(unsigned short axis);
GEP_API GT_MltiUpdt(unsigned short mask);
GEP_API GT_Open(unsigned long address=65535);
GEP_API GT_PrflH(unsigned short axis,unsigned short master);
GEP_API GT_PrflS(unsigned short axis);
GEP_API GT_PrflT(unsigned short axis);
GEP_API GT_PrflV(unsigned short axis);
GEP_API GT_ProbStopOff(void);
GEP_API GT_ProbStopOn(void);
GEP_API GT_Reset(void);
GEP_API GT_RstSts(unsigned short axis,unsigned short mask);
GEP_API GT_SetAcc(unsigned short axis,double acc);
GEP_API GT_SetAtlPos(unsigned short axis,long pos);
GEP_API GT_SetJerk(unsigned short axis,double jerk);
GEP_API GT_SetMAcc(unsigned short axis,double macc);
GEP_API GT_SetPos(unsigned short axis,long pos);
GEP_API GT_SetPosErr(unsigned short axis,unsigned short error);
GEP_API GT_SetRatio(unsigned short axis,double ratio);
GEP_API GT_SetVel(unsigned short axis,double vel);
GEP_API GT_Smooth(unsigned short axis,double smooth);
GEP_API GT_SmthStp(unsigned short axis);
GEP_API GT_StepDir(unsigned short axis);
GEP_API GT_StepPulse(unsigned short axis);
GEP_API GT_SwitchtoCardNo(unsigned short card);
GEP_API GT_Update(unsigned short axis);
GEP_API GT_ZeroPos(unsigned short axis);

#ifdef __cplusplus
}
#endif