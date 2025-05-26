#include "StdAfx.h"
#include "CameraSetDataMgr.h"

CCameraSetDataMgr::CCameraSetDataMgr(void)
{
    if(!connectDB())
	{
		m_bIsConnect = false;
	}
	else
	{
		m_bIsConnect = true;
	}
}

CCameraSetDataMgr::~CCameraSetDataMgr(void)
{
    if( m_bIsConnect)
	{
		m_adoConnection.Close();
	}
}

bool CCameraSetDataMgr::GetData()
{
    if( !m_bIsConnect)
		return false;

    CString	sSql("select * from T_CameraCfg");

    m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoRecordSet.Open(sSql);
	m_adoConnection.BeginTrans();

    m_adoRecordSet.GetCollect("HorBlankVal",HorBlankVal);
    m_adoRecordSet.GetCollect("VerBlankVal",VerBlankVal);
    m_adoRecordSet.GetCollect("SnapMode",SnapMode);
    m_adoRecordSet.GetCollect("TriggerPolarity",TriggerPolarity);
    m_adoRecordSet.GetCollect("StrobePolarity",StrobePolarity);
    m_adoRecordSet.GetCollect("SnapSpeed",SnapSpeed);
    m_adoRecordSet.GetCollect("EnableVideoMirror",EnableVideoMirror);
    m_adoRecordSet.GetCollect("CamAdcBits",CamAdcBits);
    m_adoRecordSet.GetCollect("BlkLevCal",BlkLevCal);
    m_adoRecordSet.GetCollect("BlkLevCalRedVal",BlkLevCalRedVal);
    m_adoRecordSet.GetCollect("BlkLevCalGreen1Val",BlkLevCalGreen1Val);
    m_adoRecordSet.GetCollect("BlkLevCalGreen2Val",BlkLevCalGreen2Val);
    m_adoRecordSet.GetCollect("BlkLevCalBlueVal",BlkLevCalBlueVal);
    m_adoRecordSet.GetCollect("BrightNessVal",BrightNessVal);
    m_adoRecordSet.GetCollect("GainRedVal",GainRedVal);
    m_adoRecordSet.GetCollect("GainGreen1Val",GainGreen1Val);
    m_adoRecordSet.GetCollect("GainGreen2Val",GainGreen2Val);
    m_adoRecordSet.GetCollect("GainBlueVal",GainBlueVal);
    m_adoRecordSet.GetCollect("GainCameraVal",GainCameraVal);
    m_adoRecordSet.GetCollect("Exposure",Exposure);
    m_adoRecordSet.GetCollect("ShutterSpeed",ShutterSpeed);
    m_adoRecordSet.GetCollect("ShuterUnit",ShuterUnit);

    m_adoRecordSet.Close();

	return true;
}

bool CCameraSetDataMgr::SaveData()
{
    if( !m_bIsConnect)
        return false;

    m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
    m_adoConnection.BeginTrans();

    m_adoRecordSet.Open("select * from T_CameraCfg");
    
    m_adoRecordSet.PutCollect("HorBlankVal",HorBlankVal);
    m_adoRecordSet.PutCollect("VerBlankVal",VerBlankVal);
    m_adoRecordSet.PutCollect("SnapMode",SnapMode);
    m_adoRecordSet.PutCollect("TriggerPolarity",TriggerPolarity);
    m_adoRecordSet.PutCollect("StrobePolarity",StrobePolarity);
    m_adoRecordSet.PutCollect("SnapSpeed",SnapSpeed);
    m_adoRecordSet.PutCollect("EnableVideoMirror",EnableVideoMirror);
    m_adoRecordSet.PutCollect("CamAdcBits",CamAdcBits);
    m_adoRecordSet.PutCollect("BlkLevCal",BlkLevCal);
    m_adoRecordSet.PutCollect("BlkLevCalRedVal",BlkLevCalRedVal);
    m_adoRecordSet.PutCollect("BlkLevCalGreen1Val",BlkLevCalGreen1Val);
    m_adoRecordSet.PutCollect("BlkLevCalGreen2Val",BlkLevCalGreen2Val);
    m_adoRecordSet.PutCollect("BlkLevCalBlueVal",BlkLevCalBlueVal);
    m_adoRecordSet.PutCollect("BrightNessVal",BrightNessVal);
    m_adoRecordSet.PutCollect("GainRedVal",GainRedVal);
    m_adoRecordSet.PutCollect("GainGreen1Val",GainGreen1Val);
    m_adoRecordSet.PutCollect("GainGreen2Val",GainGreen2Val);
    m_adoRecordSet.PutCollect("GainBlueVal",GainBlueVal);
    m_adoRecordSet.PutCollect("GainCameraVal",GainCameraVal);
    m_adoRecordSet.PutCollect("Exposure",Exposure);
    m_adoRecordSet.PutCollect("ShutterSpeed",ShutterSpeed);
    m_adoRecordSet.PutCollect("ShuterUnit",ShuterUnit);

    CString sError;
    if(FALSE == m_adoRecordSet.Update(sError))
	{
		m_adoConnection.RollbackTrans();
		m_adoRecordSet.Close();
		m_adoConnection.Close();
		return false;
	}
	m_adoRecordSet.Close();
	m_adoConnection.CommitTrans();

    return true;
}
