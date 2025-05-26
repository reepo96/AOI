#pragma once

#ifndef _PCBSTATE_H
#define _PCBSTATE_H

//��¼PCB��״̬

class CPCBState
{
public:
    typedef enum StateType{S_NONE/*û��*/,S_ACCEPT/*����*/,S_CLAMP/*�н�*/,S_UP/*����*/,S_DOWN/*����*/,S_LOOSE/*��*/,S_OUT/*�ͳ�*/} STATETYPE;
    
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

    //����PCB
    bool Accept(bool bSthowMsg=true);

    //�н�
    bool Clamp(bool bSthowMsg=true);

	//���� 
    bool Up(bool bSthowMsg=true);

	//����  
    bool Down(bool bSthowMsg=true);

	bool Loose(bool bSthowMsg=true);

	bool SendOut(bool bSthowMsg=true);
};

#endif