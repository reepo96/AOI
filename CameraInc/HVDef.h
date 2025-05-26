/*
 *-----------------------------------------------------------------------------
 *       Name				HVDef.h
 *       Purpose			Constant definition
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				
 *       Date          		2004/04/16
 *		 Version			1.01.0002
 *
 *       Description
 *		 This header file defines constant and external struct type
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(HV_HVDEF)         //
#define HV_HVDEF

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif
   	
typedef HANDLE HHV;


typedef enum tagHVTYPE { 	
	HV1300UCTYPE = 0,
	HV2000UCTYPE = 1,
	HV1301UCTYPE = 2,
	HV2001UCTYPE = 3,
	HV3000UCTYPE = 4,
	HV1300UMTYPE = 5,
	HV1302UCTYPE = 6,
	HV2002UCTYPE = 7,
	HV3102UCTYPE = 8,
	HV1302UMTYPE = 9,
	HV1300FCTYPE = 10,
	HV2000FCTYPE = 11,
	HV3100FCTYPE = 12,
	HV1300FMTYPE = 13,
	HV1303UCTYPE = 14,
	HV2003UCTYPE = 15,
	HV3103UCTYPE = 16,
	HV1303UMTYPE = 17
} HVTYPE;


typedef enum tagHV_RESOLUTION { 
	RES_MODE0		= 0,
	RES_MODE1		= 1,
	RES_MODE2		= 2,
	RES_MODE3		= 3	
} HV_RESOLUTION;


typedef enum tagHV_SNAP_MODE { 
	CONTINUATION	= 0,
	TRIGGER			= 1,
	TRIGGER_EDGE	= 2,
	TRIGGER_LEVEL	= 3
} HV_SNAP_MODE;


typedef enum tagHV_POLARITY { 
	LOW	 = 0,
	HIGH = 1
} HV_POLARITY;


typedef enum tagHV_SNAP_SPEED { 
	NORMAL_SPEED	= 0,	
	HIGH_SPEED	    = 1		
} HV_SNAP_SPEED;


typedef enum tagHV_CHANNEL { 
	RED_CHANNEL			= 0x10,
	GREEN_CHANNEL1		= 0x11,
	GREEN_CHANNEL2		= 0x12,
	BLUE_CHANNEL		= 0x13	
} HV_CHANNEL;


typedef enum tagHV_ADC_CONTROL{ 
	ADC_BITS		                = 0,
	ADC_BLKLEVEL_CAL				= 1,
	ADC_BLKLEVEL_CAL_REDCHANNEL		= 0x10,
	ADC_BLKLEVEL_CAL_GREENCHANNEL1	= 0x11,
	ADC_BLKLEVEL_CAL_GREENCHANNEL2	= 0x12,
	ADC_BLKLEVEL_CAL_BLUECHANNEL	= 0x13
} HV_ADC_CONTROL;


typedef enum tagHV_ADC_LEVEL{ 
	ADC_LEVEL0		= 0,
	ADC_LEVEL1		= 1,
	ADC_LEVEL2		= 2,
	ADC_LEVEL3		= 3		
} HV_ADC_LEVEL;


typedef enum tagHV_AEC_CONTROL{ 
	AEC_EXPOSURE_TIME		= 1	
} HV_AEC_CONTROL;



typedef enum tagHV_MIRROR_DIRECTION { 
	VERT_DIR	= 1	
} HV_MIRROR_DIRECTION;


typedef enum tagHV_DEVICE_INFO { 
	DESC_DEVICE_TYPE			 = 0,
	DESC_RESOLUTION				 = 1,
	DESC_DEVICE_MARK			 = 2, 
	DESC_DEVICE_SERIESNUM	     = 3,
	DESC_DEVICE_BLANKSIZE        = 4,
	DESC_DEVICE_CHIPID           = 5,
	DESC_DEVICE_HARDWARE_VERSION = 6
} HV_DEVICE_INFO;



typedef enum tagHV_PERIDEV_CONTROL{ 
	PERIDEV_BICOLOR_LAMP1 = 0,
	PERIDEV_BICOLOR_LAMP2 =	1,
	PERIDEV_IR_EMITTER	  =	2,
	PERIDEV_LED			  =	3,
	PERIDEV_ARRIVE_CLEAR  =	4,
	PERIDEV_LED1		  =	5,
	PERIDEV_LED2		  =	6
} HV_PERIDEV_CONTROL;



typedef enum tagHV_COMMAND_CODE { 
	CMD_RESERVED0			= 0x00,
	CMD_RESERVED1			= 0x01
} HV_COMMAND_CODE;


typedef enum tagHVSTATUS { 
	STATUS_OK							= 0, 
	STATUS_NO_DEVICE_FOUND				= -1,
	STATUS_DEVICE_HANDLE_INVALID		= -2,
	STATUS_HW_DEVICE_TYPE_ERROR			= -3,
	STATUS_HW_INIT_ERROR				= -4,
	STATUS_HW_RESET_ERROR				= -5,
	STATUS_NOT_ENOUGH_SYSTEM_MEMORY		= -6,
	STATUS_HW_IO_ERROR					= -7,
	STATUS_HW_IO_TIMEOUT				= -8,
	STATUS_HW_ACCESS_ERROR				= -9,
	////////////////////////////////////////////
	STATUS_OPEN_DRIVER_FAILED			= -10,
	STATUS_NOT_SUPPORT_INTERFACE		= -11,
	STATUS_PARAMETER_INVALID			= -12,
	STATUS_PARAMETER_OUT_OF_BOUND		= -13,
	STATUS_IN_WORK						= -14,
	STATUS_NOT_OPEN_SNAP				= -15,
	STATUS_NOT_START_SNAP				= -16,
	STATUS_FILE_CREATE_ERROR			= -17,
	STATUS_FILE_INVALID					= -18,
	STATUS_NOT_START_SNAP_INT			= -19,
	STATUS_INTERNAL_ERROR				= -20
} HVSTATUS;




#define HV_SUCCESS(status) ( (status) == STATUS_OK )

#ifdef _DEBUG
#define HV_VERIFY(status) \
		if((status) != STATUS_OK){ \
			::MessageBox(NULL, HVGetErrorString(status), \
						 "Error", MB_ICONWARNING | MB_OK);   \
		}     
#else
#define HV_VERIFY(status) (status)
#endif


#define HV_MESSAGE(status) \
		if((status) != STATUS_OK){ \
		::MessageBox(NULL, HVGetErrorString(status), \
		"Error message", MB_ICONWARNING | MB_OK);   \
		}    



typedef struct tagHV_SANP_INFO {
	HHV	hhv;
	int nDevice;
	int nIndex;
	void *pParam;
} HV_SNAP_INFO;


typedef int (CALLBACK* HV_SNAP_PROC)(HV_SNAP_INFO *pInfo);


typedef struct tagHV_CHECK_PARAM {	
	BYTE byParam1;
	BYTE byParam2;
	BYTE byParam3;
	BYTE byParam4;
} HV_CHECK_PARAM;


/* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif

