#pragma once

#ifndef _PCBSTATE_H
#define _PCBSTATE_H

//记录PCB板状态

class CPCBState
{
public:
    typedef enum StateType{S_NONE/*没有*/,S_ACCEPT/*接受*/,S_CLAMP/*夹紧*/,S_UP/*升起*/,S_DOWN/*落下*/,S_LOOSE/*松*/,S_OUT/*送出*/} STATETYPE;
    
protected:
    STATETYPE state;

	CString GetStateDesc();

public:
    CPCBState(void);

    ~CPCBState(void);

	void ClearState();

    STATETYPE GetState()
    {
        return state;
    }

    //接受PCB
    bool Accept(bool bSthowMsg=true);

    //夹紧
    bool Clamp(bool bSthowMsg=true);

	//升高 
    bool Up(bool bSthowMsg=true);

	//落下  
    bool Down(bool bSthowMsg=true);

	bool Loose(bool bSthowMsg=true);

	bool SendOut(bool bSthowMsg=true);
};

#endif