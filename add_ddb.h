#ifndef __ADD_DDB_H__
#define __ADD_DDB_H__

#include "hi_type.h"
//#include "hi_go.h"
#include "hi_gv.h"
#include "higv_cextfile.h"
#include "hi_gv_parser.h"
#include "hi_gv_adm.h"
#include "hi_gv_ddb.h"
#include "hi_gv_emulator.h"
#include "higv_language.h"
#include "hi_product_workmode_ext.h"



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define INVALID_HANDLE 0x0
#define MAXITEMNUM 20
#define VIDEOSETNUM	11
#define RESOLUTIONNUM	7
#define FIELDANGNUM	8
#define CODEDFORMATNUM	3
#define RECODRLAPSETIME	8
#define RECORDMODENUM	    4
#define PHOTOSETNUM	5
#define PHOTORESOLUTIONNUM 3
#define BURSTSETNUM	4
#define TIMERSETNUM	7
#define LAPSESETNUM	7
#define CAPTURESETNUM	3
#define COMMONSETNUM	10
#define INFOSETNUM	3
#define STARTACTIONNUM		4
#define STARTMODENUM		11
#define VIDEOSTANDARDNUM		3
#define SCREENLUMINANCENUM	4
#define LANGUAGESETNUM		3
#define AUTOSHUTDOWNNUM		8
#define AUTODORMANTNUM		5
#define SDSETNUM		4
#define WIFISETNUM		2
#define RFSETNUM		2
#define WATERSCENESETNUM		2
#define RECDISNUM	    5

#define INITDISP		0
#define SETNEW		1


#define AUDIOSET	0
#define FLIPSET	    1
#define LOOPSET		2
#define LEDSET		3
#define BUZZSET		4
#define OSDSET		5
#define DANALESET   6
#define CARRECORD   7
#define DISSET      8
#define WBSET       9
#define SAVEPCM     10

#define MAXITEMNAME 16


/** datamodel_file_list   corresponding  struct */
typedef struct  hiFileListRow_S
{
    HI_CHAR thumbnail[256];  //thumbnail file absolute path
	HI_CHAR grpType[8];     //"normmp4"  eg.
    HI_CHAR duration[16];   //video duration or burst file count
    HI_U32  selected;       //selected : 1,  not selected : 0
    HI_CHAR rectime[16];
}FileListRow_S;


typedef struct  hiSubModeSet_S
{
	HI_CHAR Text[28];
    HI_RESID Img;
    HI_U32 Type;
    HI_U32 Value;
}SubModeSet_S;

typedef struct  hiVideoSet_S
{
	HI_U32  NameId;
	HI_CHAR  Value[16];
    //HI_RESID Img;                              /**Picture ID */
}VideoSet_S;
typedef struct  hiResolution_S
{
	HI_CHAR  Resolution[20];
	HI_CHAR  Num[1];
	HI_RESID Img;                    /**Picture ID */
}Resolution_S;
typedef struct  hiFieldAng_S
{
	HI_CHAR  FieldAng[10];
	HI_CHAR  Num[1];
	HI_RESID Img;                    /**Picture ID */
}FieldAng_S;
typedef struct  hiPhotoSet_S
{
	HI_U32  NameId;
	//HI_RESID Img;                              /**Picture ID */
	HI_CHAR  Value[10];
}PhotoSet_S;
typedef struct  hiPhotoResolution_S
{
	HI_CHAR  Resolution[10];
	HI_CHAR  Num[1];
	HI_RESID Img;                    /**Picture ID */
}PhotoResolution_S;
typedef struct  hiBurstSet_S
{
	HI_CHAR  BurstSet[10];
	HI_CHAR  Num[1]; 
	HI_RESID Img;                    /**Picture ID */
}BurstSet_S;
typedef struct  hiTimerSet_S
{
	HI_CHAR  TimerSet[10];
	HI_CHAR  Num[1];
	HI_RESID Img;                    /**Picture ID */
}TimerSet_S;
typedef struct  hiLapseSet_S
{
	HI_CHAR  LapseSet[10];
	HI_CHAR  Num[1];
	HI_RESID Img;                    /**Picture ID */
}LapseSet_S;
typedef struct  hiCaptureSet_S
{
	HI_U32  NameId;
	//HI_RESID Img;                              /**Picture ID */
	HI_CHAR  Value[10];
}CaptureSet_S;


typedef struct  hiVideoStandard_S
{
	HI_CHAR  VideoStandard[10];
	HI_CHAR  Num[1];
	HI_RESID Img;                    /**Picture ID */
}VideoStandard_S;

typedef struct  hiLanguageSet_S
{
	HI_CHAR  LanguageSet[10];
	HI_CHAR  Num[1];
	HI_RESID Img;                    /**Picture ID */
}LanguageSet_S;

typedef struct  hiSdSet_S
{
	HI_U32  NameId;
	//HI_RESID Img;                              /**Picture ID */
}SdSet_S;
typedef struct  hiWifiSet_S
{
	HI_U32  NameId;
	HI_CHAR  Num[1];
	HI_RESID Img;                    /**Picture ID */
}WifiSet_S;

typedef struct  hiWaterSceneSet_S
{
	HI_U32  NameId;
	HI_CHAR  Num[1];
	HI_RESID Img;                    /**Picture ID */
}WaterSceneSet_S;

HI_S32 HI_ADB_InitSubMode(HI_PDT_WORKMODE_S stWorkMode);
extern HI_S32 HI_ADB_SubMode_ManageVideo(HI_U32 u32LineNum);
extern HI_S32 HI_ADB_SubMode_ManagePhoto(HI_U32 u32LineNum);
extern HI_S32 HI_ADB_SubMode_ManageMultiPhoto(HI_U32 u32LineNum);

extern HI_S32 HI_ADB_InitVideo();
extern HI_S32 HI_ADB_InitVideoStandard();
extern HI_S32 HI_ADB_InitWifi();

extern HI_S32 HI_ADB_SetVideo(HI_CHAR item);
extern HI_S32 HI_ADB_SetVideoStandard(/*PRODUCT_VIDEO_SYSTEM_E enMode*/);
extern HI_S32 HI_ADB_SetWifi(HI_BOOL bAPState);
extern HI_S32 HI_ADB_InitRecMode();
extern HI_S32 HI_ADB_InitCoded();
extern HI_S32 HI_ADB_SetCoded( /*PRODUCT_VIDEO_FORMAT_E enCodedFormat*/ );
extern HI_S32 HI_ADB_InitDis();
extern HI_S32 HI_ADB_SetDis(HI_BOOL bDisEnable/*,PRODUCT_DIS_MOTION_TYPE_E
eMotionType*/);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
