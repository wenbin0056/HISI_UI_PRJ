#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/prctl.h>

#include "higv_cextfile.h"
#include "ui_common.h"
#include "hi_gv.h"
#include "hi_gv_parser.h"
#include "higv_language.h"
#include "hi_product_timedtask.h"

#include "add_ddb.h"
#include "hi_product_storagemng.h"
#include "hi_product_param_ext.h"
#include "hi_wifi_mng.h"
#include <sys/time.h>
#include "hi_gv_input.h"
#include "hi_sys_mng.h"
#include "hi_mapi_venc.h"
#include "hi_mapi_aenc.h"
#include "hi_product_err.h"
#include "hi_mapi_vproc.h"
#include "hi_product_media_ext.h"
#include "hi_math.h"
#include "hi_mapi_comm_define.h"
#include "hi_filemgr_errcode.h"
#include "hi_product_msgproc_filemng.h"
#include "hi_product_playback_ext.h"
#include "feiyu_dv_cmd_proc.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern HI_BOOL g_bWaitIconShow;
extern HI_BOOL g_bWifiIconShow;
extern HI_BOOL g_bBatCapacityShow;
extern HI_BOOL bUSBExist;
extern HI_BOOL bBatteryExist;
extern HI_BOOL bUIShowStart;
extern HI_BOOL bRecordStart;
extern HI_BOOL g_pageChangeEnd;
extern HI_BOOL g_bSet_MenuLoaded;
extern HI_BOOL g_bNormalFormat;
extern HI_HANDLE g_diskMngHandle ;
extern HI_PDT_PARAM_CFG_S* g_pstPDTCfg;
static HI_BOOL s_bShowDvActionUI = HI_FALSE;
static HI_S32 s_s32Capacity = 100;
	
HI_BOOL UI_IsRecordingOrSnapping(HI_PDT_WORKSTATE_S* pstWorkState)
{
    if(HI_NULL == pstWorkState)
    {
        return HI_FALSE;
    }
    switch(pstWorkState->enCmd)
    {
        case PDT_CMD_START:
            return HI_TRUE;
            break;

        default:
            return HI_FALSE;
            break;
    }
}

HI_S32 UI_CheckSDState(HI_UI_ALARM_TYPE_E *penErrNo)
{
    HI_PDT_STORAGE_STATE_E enState = PDT_STORAGE_STATE_DEVOUT;
    HI_S32 s32Ret = 0;

    //check if exist
    s32Ret =  HI_PDT_Storage_GetState(g_pstPDTCfg->stStorageMngInfo.aszMountPath, &enState);
    if(s32Ret!=HI_SUCCESS || PDT_STORAGE_STATE_MOUNTED != enState)
    {
        //MLOGE("get sd state:%d \n", enState);
        if(PDT_STORAGE_STATE_SDCHECK_ERROR == enState || PDT_STORAGE_STATE_WRITEERR == enState )
        {
            *penErrNo= ALARM_SD_ERR;
        }
        else if(PDT_STORAGE_STATE_UNINITED == enState || PDT_STORAGE_STATE_CLUSTER_NOT_64K == enState || PDT_STORAGE_STATE_NOT_FAT32 == enState || PDT_STORAGE_STATE_MOUNT_FAILED == enState)
        {
            *penErrNo= ALARM_SD_NEED_FORMAT;
        }
        else if(PDT_STORAGE_STATE_SPEEDLOW == enState )
        {
            *penErrNo= ALARM_SD_SPEEDLOW;
        }
        else
        {
            *penErrNo= ALARM_SD_NOEXIST;
        }

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/**
 * Point sd card FULL or NO SD at power on.
 */
HI_S32 UI_PowerOnStorageCheck()
{
    HI_S32 s32Ret = 0;
    HI_U32 u32StroageType = UI_STORAGE_TYPE_BUTT;
    HI_PDT_WORKMODE_S stWorkMode = {};
    HI_UI_ALARM_TYPE_E enCheckErrNo;
	#ifdef CFG_FASTAPP_ON
	static HI_BOOL bFirstTime = HI_TRUE;// to ensure the enc stop only once.
	#endif
    s32Ret = HI_PDT_WorkMode_GetWorkMode(&stWorkMode);

    if (HI_SUCCESS != s32Ret)
    {
        MLOGD("GetWorkMode fail\n");
        return HI_FAILURE;
    }

    switch (stWorkMode.enWorkMode)
    {
        case PDT_WORKMODE_RECORD:
            u32StroageType = UI_STORAGE_TYPE_VIDEO;
            break;

        case PDT_WORKMODE_PHOTO:
        case PDT_WORKMODE_MULTIPHOTO:
            u32StroageType = UI_STORAGE_TYPE_IMAGE;
            break;

        default:
            u32StroageType = UI_STORAGE_TYPE_BUTT;
            break;
    }

    s32Ret = UI_PreCheckStorage(u32StroageType, &enCheckErrNo);

    if (HI_SUCCESS != s32Ret)
    {
        UI_AlarmDisplay(enCheckErrNo);

		//As the sd card not exist, just stop venc to save power.
 #ifdef CFG_FASTAPP_ON

        if ((g_pstPDTCfg->stWorkModeParam.enPoweronAction == PDT_POWERON_ACTION_REC
                && PDT_RECMODE_LAPSE != g_pstPDTCfg->stWorkModeParam.stPoweronWorkMode.enRecMode)
			&& (bFirstTime))
        {
            s32Ret = HI_MAPI_VEnc_Stop(HI_PDT_VENC_HDL_MAINVIDEO); //start before stop to ensure it work normally
            s32Ret |= HI_MAPI_VEnc_Stop(HI_PDT_VENC_HDL_SUBVIDEO);
			if(g_pstPDTCfg->stMediaCfg.stACapCfg[HI_PDT_ACAP_HDL].bEnable)
			{
				s32Ret |= HI_MAPI_AEnc_UnBindACap(HI_PDT_ACAP_HDL, HI_PDT_AENC_HDL);
				s32Ret |= HI_MAPI_AEnc_Stop(HI_PDT_AENC_HDL);
			}
            if (s32Ret)
            {
                 MLOGE("stop enc error:%x \n", s32Ret);
             }
			bFirstTime = HI_FALSE;
         }

 #endif
        return HI_FAILURE;
    }
    else
    {
        if (HI_GV_Widget_IsShow(ALARM_WINDOW))
        {
            s32Ret |= HI_GV_Widget_Hide(ALARM_WINDOW);
        }
    }

    return HI_SUCCESS;
}



/**
 *  Use HI_UI_ALARM_TYPE_E to output error code.
 */
HI_S32 UI_PreCheckStorage(HI_UI_STORAGE_TYPE_E enStorgeType, HI_UI_ALARM_TYPE_E *penErrNo)
{
    HI_S32 s32Ret = 0;

    //check sd card
    s32Ret = UI_CheckSDState(penErrNo);
    if(HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }
    //check sd scan state
    if(!HI_PDT_MsgProc_FileMng_GetScanState())
    {
        MLOGD("Disk scan is not over!\n");
        *penErrNo = ALARM_SD_PREPARING;
        return HI_FAILURE;
    }
    //check if card is full
    if(UI_STORAGE_TYPE_VIDEO == enStorgeType)
    {
        HI_U32 u32RemainTime = 0;
        UI_GetRemainRecordTime(&u32RemainTime);
        if(u32RemainTime <= 0 )
        {
            *penErrNo = ALARM_REC_SPACEFULL;
            return HI_FAILURE;
        }
    }
    else if(UI_STORAGE_TYPE_IMAGE == enStorgeType)
    {
        HI_U32 u32RemainNum = 0;
        UI_GetRemainSnapCount(&u32RemainNum);
        if(u32RemainNum <= 0 )
        {
            *penErrNo = ALARM_SNAP_SPACEFULL;
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ui_active_next_widget(HI_HANDLE ahWidgetArray[], HI_U32 u32ArraySize, HI_HANDLE hWidget)
{
    HI_U32 u32NextIndex = 0;
    HI_U32 u32CurIndex = 0;

    for (u32CurIndex = 0;  u32CurIndex < u32ArraySize; u32CurIndex++)
    {
        if (hWidget == ahWidgetArray[u32CurIndex])
        {
            u32NextIndex = (u32CurIndex + 1) % u32ArraySize;
            HI_GV_Widget_Active( ahWidgetArray[u32NextIndex] );
            return HI_SUCCESS;
        }
    }

    MLOGE("not found the given widget %d, NO widget activated\n", hWidget);
    return HI_FAILURE;
}

HI_U32 UI_MicrosecondToSecond(HI_U32 u32MicroSec)
{
    return u32MicroSec/1000 + (u32MicroSec%1000>500 ? 1 : 0);
}

HI_VOID UI_ConvertTimerFormat(HI_S32 s32Time, DV_REC_TIME* pTime)
{
    HI_S32 s32Temp;

    /*hour*/
    pTime->s32Hour = s32Time / 3600;
    /*min*/
    s32Temp = s32Time % 3600;
    pTime->s32Minute = s32Temp / 60;
    /*sec*/
    s32Temp = s32Time % 60;
    pTime->s32Second = s32Temp;
}

HI_S32 UI_GetRemainSnapCount(HI_U32* pu32RemainNum)
{
    HI_S32 i;

    HI_DISK_SPACE_STATUS_S  stDiskSpace;
    HI_DISK_SPACE_STATUS_BY_TYPE_S stDiskSpaceByType;
    memset(&stDiskSpace, 0, sizeof(HI_DISK_SPACE_STATUS_S));
    memset(&stDiskSpaceByType, 0, sizeof(HI_DISK_SPACE_STATUS_BY_TYPE_S));
	//if disk not mounted, return
    HI_UI_ALARM_TYPE_E enErrNo;
	if(UI_CheckSDState(&enErrNo) != HI_SUCCESS)
	{
		*pu32RemainNum = 0;
		return HI_SUCCESS;
	}
    HI_FILEMGR_GetDiskSpaceStatus(g_diskMngHandle, &stDiskSpace);

    for (i = 0; i < HI_SPACE_TYPE_CNT_MAX; i++)
    {
    	#ifdef CFG_LAO
		if (!strcmp("rec", stDiskSpace.stSpaceStatusByType[i].aszSpaceType))
		#else
        if (!strcmp("snp", stDiskSpace.stSpaceStatusByType[i].aszSpaceType))
		#endif
        {
            //find rec
            memcpy(&stDiskSpaceByType, &(stDiskSpace.stSpaceStatusByType[i]), \
                   sizeof(HI_DISK_SPACE_STATUS_BY_TYPE_S));
            break;
        }
    }

    //get the space of a pic
    HI_U32 u32Width = g_pstPDTCfg->stMediaCfg.stVEncCfg[HI_PDT_VENC_HDL_MAINSNAP].stResolution.u32Width;
    HI_U32 u32Height = g_pstPDTCfg->stMediaCfg.stVEncCfg[HI_PDT_VENC_HDL_MAINSNAP].stResolution.u32Height;


    if (stDiskSpaceByType.u64FreeSpace <= stDiskSpaceByType.u64SpaceLeftThres)
    {
        *pu32RemainNum = 0;
        return HI_SUCCESS;
    }
    if(0==u32Width || 0==u32Height)
    {
        MLOGD("u32Width or u32Height is 0\n");
        *pu32RemainNum = 999;
        return HI_SUCCESS;
    }

    *pu32RemainNum = (HI_U32)((8 * (stDiskSpaceByType.u64FreeSpace - stDiskSpaceByType.u64SpaceLeftThres)) / ((HI_U64)u32Width * u32Height * 3));

    return HI_SUCCESS;
}

HI_S32 UI_GetRemainRecordTime(HI_U32 *pu32RemainTime)
{
    HI_S32 i;

    HI_DISK_SPACE_STATUS_S  stDiskSpace;
    HI_DISK_SPACE_STATUS_BY_TYPE_S stDiskSpaceByType;
    memset(&stDiskSpace, 0, sizeof(HI_DISK_SPACE_STATUS_S));
    memset(&stDiskSpaceByType, 0, sizeof(HI_DISK_SPACE_STATUS_BY_TYPE_S));
    HI_UI_ALARM_TYPE_E enErrNo;
	if(UI_CheckSDState(&enErrNo) != HI_SUCCESS)
	{
		*pu32RemainTime = 0;
		return HI_SUCCESS;
	}
    HI_FILEMGR_GetDiskSpaceStatus(g_diskMngHandle, &stDiskSpace);
    for(i=0; i<HI_SPACE_TYPE_CNT_MAX; i++)
    {
        if(!strcmp("rec", stDiskSpace.stSpaceStatusByType[i].aszSpaceType))
        {
            //find rec
            memcpy(&stDiskSpaceByType, &(stDiskSpace.stSpaceStatusByType[i]), \
            sizeof(HI_DISK_SPACE_STATUS_BY_TYPE_S));
            break;
        }
    }

    HI_U64 u32BitRate = 0;
    HI_U64 u64FreeSpace = stDiskSpaceByType.u64FreeSpace;

    HI_MPP_PAYLOAD_TYPE_E enType = HI_MPP_PAYLOAD_TYPE_H265;
    for(i=0 ;i <HI_PDT_VENC_MAX_CNT; i++)
    {
        if (g_pstPDTCfg->stMediaCfg.stVEncCfg[i].bEnable)
        {
            if(0 == g_pstPDTCfg->stMediaCfg.stVEncCfg[i].u8Type)
            {
                enType = g_pstPDTCfg->stVencCommCfg.stMainVideo.enType;
            }
            else if(1 == g_pstPDTCfg->stMediaCfg.stVEncCfg[i].u8Type)
            {
                enType = g_pstPDTCfg->stVencCommCfg.stSubVideo.enType;
            }
            else if(2 == g_pstPDTCfg->stMediaCfg.stVEncCfg[i].u8Type)
            {
                enType = g_pstPDTCfg->stVencCommCfg.stMainSnap.enType;
            }
            else if(3 == g_pstPDTCfg->stMediaCfg.stVEncCfg[i].u8Type)
            {
                enType = g_pstPDTCfg->stVencCommCfg.stSubSnap.enType;
            }
            else
            {
                MLOGD("u8Type=%d,exceed error,choose main video type\n",g_pstPDTCfg->stMediaCfg.stVEncCfg[i].u8Type);
                enType = g_pstPDTCfg->stVencCommCfg.stMainVideo.enType;
            }

            if (HI_MPP_PAYLOAD_TYPE_H264==enType)
            {
                u32BitRate += g_pstPDTCfg->stMediaCfg.stVEncCfg[i].u32H264Bitrate;
            }
            else
            {
                u32BitRate += g_pstPDTCfg->stMediaCfg.stVEncCfg[i].u32BitRate;
            }
        }
    }
    // add this for accurate the remain time when ui from playback to preview
    HI_PDT_WORKMODE_S pstWorkModeTemp;
    HI_PDT_WorkMode_GetWorkMode(&pstWorkModeTemp);
    if((0 == u32BitRate)&&(PDT_WORKMODE_PLAYBACK == pstWorkModeTemp.enWorkMode))
    {
        extern HI_PDT_WORKMODE_S g_stSavedWorkMode;

        HI_PDT_VIDEO_MODE_E enVideMode;
        HI_PDT_RECMODE_CONF_E enItem = (PDT_RECMODE_RECSNAP != g_stSavedWorkMode.enRecMode)?PDT_RECMODE_CONF_VIDEOMODE : PDT_RECMODE_CONF_RECSNAP_MODE;
        if(HI_SUCCESS == HI_PDT_WorkMode_GetRecConf(g_stSavedWorkMode.enRecMode,enItem,&enVideMode))
        {
            //printf("g_stSavedWorkMode.enRecMode ==%d,enVideMode ==%d\n",g_stSavedWorkMode.enRecMode,enVideMode);
            for(i=0 ;i <HI_PDT_VENC_MAX_CNT; i++)
            {
                if (g_pstPDTCfg->stWorkModeParam.stVideoModeCfg[enVideMode].stVEncCfg[i].bEnable)
                {
                    if(0 == g_pstPDTCfg->stWorkModeParam.stVideoModeCfg[enVideMode].stVEncCfg[i].u8Type)
                    {
                        enType = g_pstPDTCfg->stVencCommCfg.stMainVideo.enType;
                    }
                    else if(1 == g_pstPDTCfg->stWorkModeParam.stVideoModeCfg[enVideMode].stVEncCfg[i].u8Type)
                    {
                        enType = g_pstPDTCfg->stVencCommCfg.stSubVideo.enType;
                    }
                    else if(2 == g_pstPDTCfg->stWorkModeParam.stVideoModeCfg[enVideMode].stVEncCfg[i].u8Type)
                    {
                        enType = g_pstPDTCfg->stVencCommCfg.stMainSnap.enType;
                    }
                    else if(3 == g_pstPDTCfg->stWorkModeParam.stVideoModeCfg[enVideMode].stVEncCfg[i].u8Type)
                    {
                        enType = g_pstPDTCfg->stVencCommCfg.stSubSnap.enType;
                    }
                    else
                    {
                        MLOGD("u8Type=%d,exceed error1,choose main video type\n",g_pstPDTCfg->stMediaCfg.stVEncCfg[i].u8Type);
                        enType = g_pstPDTCfg->stVencCommCfg.stMainVideo.enType;
                    }

                    if (HI_MPP_PAYLOAD_TYPE_H264==enType)
                    {
                        u32BitRate += g_pstPDTCfg->stWorkModeParam.stVideoModeCfg[enVideMode].stVEncCfg[i].u32H264Bitrate;
                    }
                    else
                    {
                        u32BitRate += g_pstPDTCfg->stWorkModeParam.stVideoModeCfg[enVideMode].stVEncCfg[i].u32BitRate;
                    }
                    //printf("venc bitrate is     %u ========111\n\n",(HI_U32)u32BitRate);
                }

            }
        }
    }
    if(0 == u32BitRate)
    {
        printf("venc bitrate is 0 ========\n\n");
        *pu32RemainTime = (HI_U32)u64FreeSpace;
        return HI_SUCCESS;
    }

    if (u64FreeSpace <= stDiskSpaceByType.u64SpaceLeftThres)
    {
        *pu32RemainTime = 0;
    }
    else
    {
        *pu32RemainTime = (HI_U32)((8  * (u64FreeSpace-stDiskSpaceByType.u64SpaceLeftThres))/((HI_U64)(u32BitRate*1024))) + 1;
    }

    return HI_SUCCESS;
}


HI_S32 ui_enwifi(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_GV_Msg_SendAsync(HOME_PAGE_BUTTON_WIFI, HIGV_MSG_KEYDOWN, HIGV_KEY_F2, 0);

    return s32Ret;
}


HI_VOID* Wifi_TurnOnOff(HI_VOID* pData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bAPState = HI_FALSE;

    bAPState = HI_WIFIMNG_GetState();

    if (bAPState)
    {   
        #ifdef WENBIN
        s32Ret = HI_WIFIMNG_SetParam(g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD, WIFI_SHUTDOWN);
        #else
        s32Ret = HI_WIFIMNG_Stop();
        #endif
        MLOGE("WIFI OFF\n");
        if (HI_SUCCESS != s32Ret)
        {
            MLOGE("HI_WIFIMNG_Stop failed!\n");
            s32Ret = HI_GV_Msg_SendAsync(TIPS_WINDOW, HIGV_MSG_KEYDOWN, HIGV_KEY_F2, HI_TRUE);
            hi_usleep(500 * 1000);
            s32Ret = HI_GV_Msg_SendAsync(TIPS_WINDOW, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
        }
        else
        {
			g_bWifiIconShow = HI_FALSE;
            //s32Ret |= HI_GV_Msg_SendAsync(WIFI_SET, HIGV_MSG_EVENT, 0, HI_FALSE);
            while(HI_FALSE != HI_WIFIMNG_GetState())
            {	
            	hi_usleep(500 * 1000);
            }
            s32Ret = HI_GV_Msg_SendAsync(HOME_PAGE_BUTTON_WIFI, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
            s32Ret = HI_GV_Msg_SendAsync(TIPS_WINDOW, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
        }
    }
    else
    {
        #ifdef WENBIN
        s32Ret = HI_WIFIMNG_SetParam(g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD, WIFI_MODE_AP);      
        #else
        s32Ret = HI_WIFIMNG_Start();
        #endif
        MLOGE("WIFI ON\n");
        if (HI_SUCCESS != s32Ret)
        {
            MLOGE("HI_WIFIMNG_Start failed!\n");
            s32Ret = HI_GV_Msg_SendAsync(TIPS_WINDOW, HIGV_MSG_KEYDOWN, HIGV_KEY_F2, HI_FALSE);
            hi_usleep(500 * 1000);
            s32Ret = HI_GV_Msg_SendAsync(TIPS_WINDOW, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
        }
        else
        {
            //s32Ret |= HI_GV_Msg_SendAsync(WIFI_SET, HIGV_MSG_EVENT, 0, HI_TRUE);
			g_bWifiIconShow = HI_TRUE;
            while(HI_TRUE != HI_WIFIMNG_GetState())
            {	
            	hi_usleep(500 * 1000);
            }
            s32Ret = HI_GV_Msg_SendAsync(HOME_PAGE_BUTTON_WIFI, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
            hi_usleep(400 * 1000);
            s32Ret = HI_GV_Msg_SendAsync(TIPS_WINDOW, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
        }
    }

    #ifdef CFG_TIME_MEASURE_ON
    TIME_STAMP;
    HI_Print_Time_Stamp();
    #endif
    #ifdef WENBIN
    extern HI_BOOL g_bWifiConfigBusy;
    g_bWifiConfigBusy = HI_FALSE;
    #endif
    return NULL;
}


HI_S32 UI_HideAllWindows()
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_U32 i = 0;

    for (i = 0; i < 2500; i++) //600 is bigger than the max num of widget.so this can hide all the widget.
    {
        if (HI_GV_Widget_IsShow(i))
        {
            HI_GV_Widget_Hide(i);
        }
    }

    return s32Ret;
}


HI_S32 UI_JumpToHome()
{
    HI_S32 s32Ret = 0;
    HI_PDT_WORKMODE_S stWorkMode = {};
	MLOGD("\n");

    UI_HideAllWindows();

    HI_PDT_WorkMode_GetWorkMode(&stWorkMode);
	if(PDT_WORKMODE_PLAYBACK == stWorkMode.enWorkMode)
	{
		MLOGD("stop player and set workmode\n");
		HI_PDT_Playback_Stop();

		stWorkMode.enWorkMode = PDT_WORKMODE_RECORD;
        stWorkMode.enRecMode = PDT_RECMODE_NORMAL;
        s32Ret |= HI_PDT_WorkMode_SetWorkMode(&stWorkMode);
	}

    s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Update(HOME_PAGE, 0);
    s32Ret |= HI_GV_Widget_Refresh(HOME_PAGE, 0);
    if(HI_TRUE == s_bShowDvActionUI)
    {
        s32Ret |= HI_GV_Widget_Show(FEIYU_TAKE_PHTO);        
        s32Ret |= HI_GV_Widget_MoveToTop(FEIYU_TAKE_PHTO);
        s32Ret |= HI_GV_Widget_Update(FEIYU_TAKE_PHTO, 0);
        s32Ret |= HI_GV_Widget_Refresh(FEIYU_TAKE_PHTO, 0);
        s32Ret |= HI_GV_Widget_Paint(FEIYU_TAKE_PHTO, 0);
    }

	g_pageChangeEnd = HI_TRUE;
    if (s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HI_FAILURE;
}



HI_S32 UI_JumpToFormat()
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE s32ActWindow = INVALID_HANDLE;
    g_bNormalFormat = HI_FALSE;


    s32Ret |= HI_GV_Widget_GetActiveWidget(HIGV_WND_MANAGER_HANDLE, &s32ActWindow);
    if(ALARM_WINDOW == s32ActWindow)
    {
        s32Ret |= HI_GV_Widget_Hide(ALARM_WINDOW);
    }
    else if(s32ActWindow != SD_FORMATING && s32ActWindow != SD_FORMAT)
    {
        extern HI_HANDLE g_s32ActWindow;
        g_s32ActWindow = s32ActWindow;
        MLOGD("g_s32ActWindow=%d\n",g_s32ActWindow);
    }

    s32Ret |= HI_GV_Widget_Show(SD_FORMAT);
    s32Ret |= HI_GV_Widget_Active(SD_FORMAT);
    s32Ret |= HI_GV_Widget_Show(SD_FORMAT_ASK);
    s32Ret |= HI_GV_Widget_Active(SD_FORMAT_ASK_BUTTON_cancel);
    s32Ret |= HI_GV_Widget_Paint(SD_FORMAT, 0);

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("Error %d\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 UI_StateNotify(HI_PDT_WORKSTATE_S* pstWorkState)
{
    //higv only support one thread. IF other threads need to call higv interface, call HI_GV_Msg_SendAsync to send msg to the major UI thread.
    HI_GV_Msg_SendAsyncWithData(HOME_PAGE, HIGV_MSG_EVENT, (HI_VOID *)pstWorkState, sizeof(HI_PDT_WORKSTATE_S));
    return HI_SUCCESS;
}

static HI_BOOL bJumpHomePageByApp = HI_FALSE;
extern HI_PDT_WORKMODE_S g_stCurrentWorkMode;

HI_S32 UI_StateProcess(HI_PDT_WORKSTATE_S* pstWorkState)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bAPState = HI_FALSE;

    MLOGD(" enCmd = %u, event = %u, pasttime= %u\n", pstWorkState->enCmd, pstWorkState->enEvent, pstWorkState->s32PastTime);
    #ifdef WENBIN
    if( PDT_EVT_UPDATE_PARAM == pstWorkState->enEvent)
    {
    	if(HI_GV_Widget_IsShow(groupbox_date))
    	{
    		extern HI_S32 refreshDateMiscPage();
    		refreshDateMiscPage();
    	}
        return HI_SUCCESS;
    }
    #endif
    if((PDT_CMD_START == pstWorkState->enCmd || PDT_CMD_STOP == pstWorkState->enCmd) &&  PDT_EVT_NORMAL_OPT == pstWorkState->enEvent)
    {
        HI_PDT_WORKMODE_S stWorkMode = {};
        HI_PDT_WorkMode_GetWorkMode(&stWorkMode);
        
        if(PDT_WORKMODE_PLAYBACK == stWorkMode.enWorkMode)
        {
            MLOGD("WorkMode PDT_WORKMODE_PLAYBACK, filter out normal start/stop event.\n");
            return HI_SUCCESS;
        }
    }
    
    if(PDT_CMD_START == pstWorkState->enCmd)
    {
        if(HI_FALSE == bJumpHomePageByApp)
        {
        #if 0
            //in case of changeing work state from phone.
            UI_HideAllWindows();
            s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
            s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
            if(HI_TRUE == s_bShowDvActionUI)
            {
                s32Ret |= HI_GV_Widget_Show(FEIYU_TAKE_PHTO);        
                s32Ret |= HI_GV_Widget_MoveToTop(FEIYU_TAKE_PHTO);
                s32Ret |= HI_GV_Widget_Update(FEIYU_TAKE_PHTO, 0);
                s32Ret |= HI_GV_Widget_Refresh(FEIYU_TAKE_PHTO, 0);
                s32Ret |= HI_GV_Widget_Paint(FEIYU_TAKE_PHTO, 0);
            }
        #endif
        }
        else
        {
            bJumpHomePageByApp = HI_FALSE;
        }
    }

    if (PDT_CMD_STOP == pstWorkState->enCmd)
    {
        //refresh countdown number now
        ShowTakePhoto(HI_FALSE);
        HOME_PAGE_Refresh();
    }

    //MLOGD(" enCmd = %u, event = %u, pasttime= %u\n", pstWorkState->enCmd,
    //      pstWorkState->enEvent, pstWorkState->s32PastTime);

    switch (pstWorkState->enEvent)
    {
        case PDT_EVT_REC_FULL:
            //s32Ret |= HI_Product_WarningTone_On();
            s32Ret |= UI_AlarmDisplay(ALARM_REC_SPACEFULL);
            break;

        case PDT_EVT_REC_END:
            //s32Ret |= HI_Product_WarningTone_On();
            //s32Ret |= UI_AlarmDisplay(ALARM_REC_SPACEFULL);
            break;

        case PDT_EVT_REC_ERR:
            s32Ret |= UI_AlarmDisplay(ALARM_REC_ERR);
            break;

        case PDT_EVT_SNAP_FULL:
            //s32Ret |= HI_Product_WarningTone_On();
            s32Ret |= UI_AlarmDisplay(ALARM_SNAP_SPACEFULL);
            break;

        case PDT_EVT_SNAP_ERR:
            s32Ret |= UI_AlarmDisplay(ALARM_SNAP_ERR);
            break;

        case PDT_EVT_SD_NOEXIST:
            if(HI_GV_Widget_IsShow(PLAYBACK_PAGE) && HI_GV_Widget_IsActive(PLAYBACK_PAGE))
            {
                HI_GV_Msg_SendAsync(PLAYBACK_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_F3, 0);
            }
			if(HI_GV_Widget_IsShow(HOME_PAGE_MISC) && (PDT_WORKMODE_RECORD == g_stCurrentWorkMode.enWorkMode) && 
				(bRecordStart == HI_TRUE))
			{
				HI_GV_Msg_SendAsync(HOME_PAGE_MISC_GROUPBOX_CLOSE, HIGV_MSG_GESTURE_TAP, 0, 0);
			}
            s32Ret |= UI_AlarmDisplay(ALARM_SD_NOEXIST);
			if(HI_GV_Widget_IsShow(FILELIST_PAGE) && HI_GV_Widget_IsActive(FILELIST_PAGE))
            {
			    s32Ret |= HI_GV_Msg_SendAsync(FILELIST_PAGE, HIGV_MSG_EVENT, PDT_EVT_SD_NOEXIST, 0);
			}
            break;

        case PDT_EVT_SD_ERR:
        case PDT_EVT_REC_SD_ERR:
        case PDT_EVT_SNAP_SD_ERR:
            s32Ret |= UI_AlarmDisplay(ALARM_SD_ERR);
            break;

        case PDT_EVT_CHIP_TEMP_HIGH:
            //s32Ret |= HI_Product_WarningTone_On();
            s32Ret |= UI_AlarmDisplay(ALARM_CHIPTEMP_SHUTDOWN);
            //sleep(2);
            //s32Ret = HI_Product_ShutDown();
            break;

        case PDT_EVT_PWR_TEMP_HIGH:
            //s32Ret |= HI_Product_WarningTone_On();
            s32Ret |= UI_AlarmDisplay(ALARM_PWRTEMP_SHUTDOWN);
            //sleep(2);
            //s32Ret = HI_Product_ShutDown();
            break;

        case PDT_EVT_PWR_LOW:
            //s32Ret |= HI_Product_WarningTone_On();
            s32Ret |= UI_AlarmDisplay(ALARM_PWRCAPACITY_SHUTDOWN);
            //sleep(2);
            //s32Ret = HI_Product_ShutDown();
            break;

        case PDT_EVT_USB_CONNECTED:
            s32Ret |= HI_GV_Msg_SendAsync(USB_CONNECTING, HIGV_MSG_EVENT, 0, HI_TRUE);
            g_KeyEnable = HI_FALSE;
            bAPState = HI_WIFIMNG_GetState();
            if (bAPState)
            {
                s32Ret |= HI_GV_Msg_SendAsync(HOME_PAGE_BUTTON_WIFI, HIGV_MSG_KEYDOWN, HIGV_KEY_F2, HI_TRUE);
            }
            break;

        case PDT_EVT_SHUT_DOWN:
            break;

        case PDT_EVT_USB_DISCONNECTED:
            s32Ret |= HI_GV_Msg_SendAsync(USB_CONNECTING, HIGV_MSG_EVENT, 0, HI_FALSE);
            g_KeyEnable = HI_TRUE;
            break;

        case PDT_EVT_CHIP_TEMP_ALARM:
            //s32Ret |= HI_Product_WarningTone_On();
            s32Ret |= UI_AlarmDisplay(ALARM_CHIPTEMP);
            break;

        case PDT_EVT_PWR_TEMP_ALARM:
            //s32Ret |= HI_Product_WarningTone_On();
            s32Ret |= UI_AlarmDisplay(ALARM_PWRTEMP);
            break;

        case PDT_EVT_PWR_LOW_ALARM:
            //s32Ret |= HI_Product_WarningTone_On();
            s32Ret |= UI_AlarmDisplay(ALARM_PWRCAPACITY);
            break;

        case PDT_EVT_SD_MOUNT_ON:
            s32Ret |= HI_GV_Msg_SendAsync(ALARM_WINDOW, HIGV_MSG_EVENT, PDT_EVT_SD_MOUNT_ON, 0);
            break;

        case PDT_EVT_AC_ON:
            s32Ret |= HI_GV_Msg_SendAsync(ALARM_WINDOW, HIGV_MSG_EVENT, PDT_EVT_AC_ON, 0);
            break;

        case PDT_EVT_AC_OFF:
            break;

        case PDT_EVT_WIFI_OFF:
            s32Ret |= HI_GV_Msg_SendAsync(HOME_PAGE_BUTTON_WIFI, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
            break;
        case PDT_EVT_SD_CHECK:
            if(!HI_PDT_Storage_IsFormatting())
            {
                MLOGD("-->> PDT_EVT_SD_CHECK ");
                UI_AlarmDisplay(ALARM_DOING_FSCK);
            }
            break;
        case PDT_EVT_SD_CHECKEND:
            if(!HI_PDT_Storage_IsFormatting())
            {
                s32Ret |= HI_GV_Msg_SendAsync(ALARM_WINDOW, HIGV_MSG_EVENT, PDT_EVT_SD_CHECKEND, 0);
            }
            break;
        case  PDT_EVT_SD_SPEEDLOW:
            s32Ret |= UI_AlarmDisplay(ALARM_SD_SPEEDLOW);
            break;
        case PDT_EVT_SD_NEED_FORMAT:
            UI_JumpToFormat();
            break;
        case PDT_EVT_SCAN_END:
			if(HI_GV_Widget_IsShow(FILELIST_PAGE) && HI_GV_Widget_IsActive(FILELIST_PAGE))
            {
                s32Ret |= HI_GV_Msg_SendAsync(FILELIST_PAGE, HIGV_MSG_EVENT, PDT_EVT_SD_MOUNT_ON, 0);
			}
            break;
        case PDT_EVT_WORKMODE_UPDATE:
            break;
        default:
            s32Ret |= HI_GV_Msg_SendAsync(ALARM_WINDOW, HIGV_MSG_EVENT, PDT_EVT_ALARM_HIDE, 0);
            break;
    }

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#ifdef CFG_HJD
HI_S32 UI_ShowBatteryCapacity()
{
	HI_CHAR batteryCapacity[8] = {0};
	if((HI_FALSE == bUIShowStart)||(HI_TRUE == bUSBExist))
	{
		snprintf(batteryCapacity, 8, "");
	}
	else
	{
		if(HI_FALSE == g_bWaitIconShow)
		{
			snprintf(batteryCapacity, 8, "%d%%", s_s32Capacity);
		}
	}

	HI_GV_Widget_SetText(HOME_PAGE_LABEL_BATTERY_CAPACITY, batteryCapacity);
	HI_GV_Widget_Paint(HOME_PAGE_LABEL_BATTERY_CAPACITY, 0);
}

HI_S32 HI_HAL_BAT_GetPlugState()
{
    HI_BOOL bACState = HI_FALSE;
    HI_BOOL bACFullState = HI_FALSE;
	HI_S32 s32Ret = HI_SUCCESS;  
	static HI_U8 s_u8Count1 = 0;
	static HI_U8 s_u8Count2 = 0;
	
	s32Ret = HI_HAL_BAT_GetState(&bACState);
    if(HI_SUCCESS != s32Ret)
    {		
		MLOGE("[Error] func:%s line:%d get data failed.\r\n",__FUNCTION__,__LINE__);
		return HI_FAILURE;
	}

    if (bACState)
    {
    	s32Ret = HAL_Battery_Full_GetState(&bACFullState);
		if(HI_SUCCESS != s32Ret)
		{
			MLOGE("[Error] func:%s line:%d get data failed.\r\n",__FUNCTION__,__LINE__);
			return HI_FAILURE;
		}
		
		if(bACFullState == HI_TRUE)
        {       	
        	s_u8Count2 = 0;
        	if((s_u8Count1++ > 5)&&(HI_TRUE == bBatteryExist))
        	{
        		bBatteryExist = HI_FALSE;
        		s_u8Count1 = 0;
        	}		
        }
        else
        {
        	s_u8Count1 = 0;
			if((s_u8Count2++ > 5)&&(HI_FALSE == bBatteryExist))
			{
				bBatteryExist = HI_TRUE;
				s_u8Count2 = 0;
			}
		}	
	}
	else
	{
		bBatteryExist = HI_TRUE;
	}

	return HI_SUCCESS;
}
#endif

#ifdef CFG_LAO
#include "hi_hal_led.h"
HI_BOOL g_bLowPWRShutDown = HI_FALSE;
HI_BOOL g_bShutDown = HI_FALSE;
extern HI_VOID feiyuAcBreathLed(HI_BOOL flag);
extern HI_VOID feiyuFullAcLed(HI_BOOL flag);

HI_S32 UI_SetBatteryControl(HI_HANDLE hWidget)
{
		HI_S32 s32Ret = HI_SUCCESS;
		HI_BOOL bACFullState = HI_FALSE;
		HI_BOOL bBatteryCharge = HI_FALSE;
		HI_BOOL bACState = HI_FALSE;
		HI_S32 s32BatteryCapacity = 0;
		HI_HANDLE hSkin = battery_100;
		static HI_HANDLE hPreSkin = battery_100;
        static HI_U32 s_u32Count = 0;
        static HI_BOOL s_bfirstFlag = HI_TRUE;
		HI_BOOL bIsShow = HI_FALSE;
		static HI_S32 s_s32LowPWRCount = 0; 
		static HI_BOOL s_bShowFlag = HI_TRUE;

		if(HI_FALSE == bUIShowStart)
		{
			s_bShowFlag = HI_TRUE;
			s32Ret |= HI_GV_Widget_SetSkin(hWidget, HIGV_SKIN_NORMAL, background_skin);
			s32Ret = HI_GV_Widget_Paint(hWidget, 0);
			goto End;
		}
	
		s32Ret = HI_BATMNG_GetInfo(&bACState, &bBatteryCharge, &s32BatteryCapacity);
		s_s32Capacity = s32BatteryCapacity;
		//MLOGD(">>>>>>>>>Battery Capacity:%d\n", s32BatteryCapacity);
			
		if (HI_SUCCESS != s32Ret)
		{
			MLOGE("Error:%#x\n", s32Ret);
			return s32Ret;
		}
		if (g_bLowPWRShutDown)
			return s32Ret;
		HI_HAL_BAT_GetState(&bACState);

		//printf("s32BatteryCapacity is %d \n",s32BatteryCapacity);
		if (bACState)
		{
			hSkin = battery_ac;
			#ifdef CFG_HJD
			HAL_Battery_Full_GetState(&bACFullState);
			if((HI_FALSE == bACFullState)&&(100 == s32BatteryCapacity))      
            {
                if((s_u32Count++ > 20) && (HI_TRUE == s_bfirstFlag))
                {
                    UI_AlarmDisplay(ALARM_AC_FULL); 
                    s_bfirstFlag = HI_FALSE;
                }
            }

			if(HI_TRUE == bUSBExist)     
            {
				s_bShowFlag = HI_TRUE;
				s32Ret |= HI_GV_Widget_SetSkin(hWidget, HIGV_SKIN_NORMAL, battery_usb);
				s32Ret = HI_GV_Widget_Paint(hWidget, 0);
				goto End;
            }
			#endif
		}
		else
		{
		    #if 0
			if (s32BatteryCapacity >= 80)
			{
				hSkin = (hPreSkin>home_page_battery_100&&hPreSkin!=preview_batteryac_skin) ? hPreSkin : home_page_battery_100;
			}
			else if (s32BatteryCapacity >= 50)
			{
				hSkin = (hPreSkin>home_page_battery_80&&hPreSkin!=preview_batteryac_skin) ? hPreSkin : home_page_battery_80;
			}
			else if (s32BatteryCapacity >= 30)
			{
				hSkin = (hPreSkin>home_page_battery_40&&hPreSkin!=preview_batteryac_skin) ? hPreSkin : home_page_battery_40;
			}
			else if (s32BatteryCapacity >= 20)
			{
				hSkin = (hPreSkin>home_page_battery_20&&hPreSkin!=preview_batteryac_skin) ? hPreSkin : home_page_battery_20;
			}
			else if (s32BatteryCapacity >= 4)
			{
				hSkin = (hPreSkin>home_page_battery_0&&hPreSkin!=preview_batteryac_skin) ? hPreSkin : home_page_battery_0;
			}
			else if (s32BatteryCapacity == 0)
			{
				g_bLowPWRShutDown = HI_TRUE;
				
			}
			else
			{
				hSkin = home_page_battery_0;
			}
            #else
			if(s32BatteryCapacity != 0)
				s_s32LowPWRCount = 0;
				
			if(s32BatteryCapacity != 100)
				s_bfirstFlag = HI_TRUE;
			
			if (s32BatteryCapacity > 90)
			{
				hSkin = (hPreSkin>battery_100&&hPreSkin!=battery_ac) ? hPreSkin : battery_100;
			}
			else if (s32BatteryCapacity > 80)
			{
				hSkin = (hPreSkin>battery_90&&hPreSkin!=battery_ac) ? hPreSkin : battery_90;
			}
			else if (s32BatteryCapacity > 70)
			{
				hSkin = (hPreSkin>battery_80&&hPreSkin!=battery_ac) ? hPreSkin : battery_80;
			}
			else if (s32BatteryCapacity > 60)
			{
				hSkin = (hPreSkin>battery_70&&hPreSkin!=battery_ac) ? hPreSkin : battery_70;
			}
			else if (s32BatteryCapacity > 50)
			{
				hSkin = (hPreSkin>battery_60&&hPreSkin!=battery_ac) ? hPreSkin : battery_60;
			}
			else if (s32BatteryCapacity > 40)
			{
				hSkin = (hPreSkin>battery_50&&hPreSkin!=battery_ac) ? hPreSkin : battery_50;
			}            
			else if (s32BatteryCapacity > 30)
			{
				hSkin = (hPreSkin>battery_40&&hPreSkin!=battery_ac) ? hPreSkin : battery_40;
			}
			else if (s32BatteryCapacity > 20)
			{
				hSkin = (hPreSkin>battery_30&&hPreSkin!=battery_ac) ? hPreSkin : battery_30;
			}
			else if (s32BatteryCapacity > 10)
			{
				hSkin = (hPreSkin>battery_20&&hPreSkin!= battery_ac) ? hPreSkin : battery_20;
			}            
			else if (s32BatteryCapacity == 0)
			{
				if(s_s32LowPWRCount++ > 10)
				{
					s_s32LowPWRCount = 0;
					g_bLowPWRShutDown = HI_TRUE;
				}
				
				if(s_s32LowPWRCount > 2)
				{
					hSkin = battery_0;
				}
			}
			else
			{
				hSkin = battery_10;
			}            
            #endif
		}
	
		if ((hSkin != hPreSkin)||(HI_TRUE == s_bShowFlag))
		{
			s32Ret |= HI_GV_Widget_SetSkin( hWidget, HIGV_SKIN_NORMAL, hSkin);
		}
	
		bIsShow = HI_GV_Widget_IsShow(hWidget);
	
		if ((HI_FALSE == bACState) && (HI_FALSE == bBatteryCharge) && (s32BatteryCapacity < 10))
		{
			if (bIsShow)
			{
				s32Ret |= HI_GV_Widget_Hide(hWidget);
			}
			else
			{
				s32Ret |= HI_GV_Widget_Show(hWidget);
			}
		}
		else
		{
			if (!bIsShow)
			{
				s32Ret |= HI_GV_Widget_Show(hWidget);
			}
		}
	
		if ((hSkin != hPreSkin)||(HI_TRUE == s_bShowFlag))
		{
			s_bShowFlag = HI_FALSE;
			s32Ret = HI_GV_Widget_Paint(hWidget, 0);
			hPreSkin = hSkin;
		}
End:	
		if (HI_SUCCESS != s32Ret)
		{
			//UI_ERROR("s32Ret = %#x\n", s32Ret);
		}
	
		return s32Ret;
}

extern HI_S32 GKU_RTCMNG_GetChipTemp();
void * pthread_SetHighTmpControl(void *data)
{
	HI_U32 u32Value = 0;
	prctl(PR_SET_NAME, (unsigned long)"TmpControl", 0, 0, 0);
	sleep(3);
	while(1)
	{
		u32Value = 0;
		u32Value = GKU_RTCMNG_GetChipTemp();
		
		if(u32Value > 95)
		{
			ALARM_WINDOW_onevent(0, PDT_EVT_CHIP_TEMP_HIGH, ALARM_CHIPTEMP_SHUTDOWN);
			sleep(3);

			HI_GV_Widget_Hide(ALARM_WINDOW);

			printf("ALARM_CHIPTEMP_SHUTDOWN HI_SYSMNG_PowerOff~~~~~~~~~~~~~~~\n\n\n");
			HI_GV_Msg_SendAsync(HOME_PAGE_BUTTON_WIFI, HIGV_MSG_KEYDOWN, HIGV_KEY_HOME, 0);

			HI_SYSMNG_PowerOff();
		}
		else if (g_bLowPWRShutDown || g_bShutDown)
		{
		    #if 1
			if (g_bLowPWRShutDown)
			{
				ALARM_WINDOW_onevent(0, PDT_EVT_PWR_LOW_ALARM, ALARM_PWRCAPACITY_SHUTDOWN);
				sleep(3);
				HI_GV_Widget_Hide(ALARM_WINDOW);
			}
			printf("SHUTDOWN HI_SYSMNG_PowerOff~~~~~~~~~~~~~~~\n\n\n");
			//HI_GV_Msg_SendAsync(HOME_PAGE_BUTTON_WIFI, HIGV_MSG_KEYDOWN, HIGV_KEY_HOME, 0);
			
			HI_SYSMNG_PowerOff();
            #endif
		}
		sleep(1);
	}
}
#endif


HI_S32 UI_SetSDControl(HI_HANDLE hStatusWidget, HI_HANDLE hCapacityWidget)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CHAR aszText[8] = {0};
    static HI_CHAR aszPreText[8];
    HI_HANDLE hSDStatusSkin = preview_status_sd_normal_skin;
    static HI_HANDLE hPreSDStatusSkin = preview_status_sd_normal_skin;
    HI_PDT_STORAGE_STATE_E enSDState = PDT_STORAGE_STATE_MOUNTED;
    HI_U64 u64SDTotalSize = 1, u64SDRemainSize = 0;
    HI_U32 u32Percent = 0;
    HI_PDT_STORAGE_DISKATTR_S stDiskAttr = {0};

    s32Ret |= HI_PDT_Storage_GetState(g_pstPDTCfg->stStorageMngInfo.aszMountPath, &enSDState);


    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    switch (enSDState)
    {

        case PDT_STORAGE_STATE_UNINITED:
        case PDT_STORAGE_STATE_UNMOUNTED:
        case PDT_STORAGE_STATE_WRITEPROTECT:
        case PDT_STORAGE_STATE_SDCHECK_ERROR:
        case PDT_STORAGE_STATE_MOUNT_FAILED:
            if(HI_PDT_Storage_IsFormatting())
            {
                hSDStatusSkin = no_sd;
                snprintf(aszText, 8, "%s", "F...");
            }
            else
            {
                hSDStatusSkin = no_sd;
                snprintf(aszText, 8, "%s", "ERR");
            }
            break;

        case PDT_STORAGE_STATE_DEVOUT:
            hSDStatusSkin = no_sd;
            snprintf(aszText, 8, "%s", "NO");
            break;

        case PDT_STORAGE_STATE_MOUNTED:
            s32Ret = HI_PDT_Storage_GetDiskAttr(g_pstPDTCfg->stStorageMngInfo.aszMountPath, &stDiskAttr);

            if (HI_SUCCESS != s32Ret)
            {
                return HI_FAILURE;
            }

            hSDStatusSkin = sd_normal;
            u64SDTotalSize = stDiskAttr.u64TotalSpace;
            u64SDRemainSize = stDiskAttr.u64FreeSpace;
            u32Percent = u64SDRemainSize * 100 / u64SDTotalSize;

            if (u32Percent > 100)
            {
                u32Percent = 100;
            }

            snprintf(aszText, 8, "%u%%", u32Percent);
            break;

        default:
            return HI_FAILURE;
    }


    if ((hSDStatusSkin != hPreSDStatusSkin) || (strncmp(aszText, aszPreText, 8) != 0))
    {
        if(sd_normal !=hSDStatusSkin)
            hSDStatusSkin = no_sd;       
        s32Ret |= HI_GV_Widget_SetSkin(hStatusWidget, HIGV_SKIN_NORMAL, hSDStatusSkin);

        #if 0
        if(hSDStatusSkin != sd_normal)
            //s32Ret |= HI_GV_Widget_SetTextByID(HOME_PAGE_LABEL_SD_LABEL,no_sd);  
        else
            //s32Ret |= HI_GV_Widget_SetText(HOME_PAGE_LABEL_SD_LABEL," "); 
        #endif
        s32Ret |= HI_GV_Widget_SetText(hCapacityWidget, aszText);
        
        hPreSDStatusSkin = hSDStatusSkin;
        memcpy(aszPreText, aszText, 8);
    }

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return s32Ret;
}


#ifdef CFG_LAO
extern int himm(unsigned int addr, unsigned int val);
#endif

HI_S32 UI_SetWifiControl(HI_HANDLE hWidget)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bAPState = HI_FALSE;
	HI_RECT rect = {0};
	#ifdef CFG_LAO
	HI_CHAR bufSsid[32] = {0};
	HI_CHAR bufPwd[32] = {0};
	HI_U32 u32Value = -1;
	s32Ret = himd(0x12040098, &u32Value);
	#endif
    //Get wifi status
    if(HI_TRUE == g_bWaitIconShow)
    {
    	g_bWaitIconShow = HI_FALSE;
    }
    bAPState = HI_WIFIMNG_GetState();
    if (bAPState)
    {
    	if(HI_FALSE == g_bBatCapacityShow)
    	{
    		s32Ret |= HI_GV_Widget_GetRect(hWidget, &rect);
        	s32Ret |= HI_GV_Widget_Show(hWidget);
			s32Ret |= HI_GV_Widget_Move(hWidget, 398, rect.y);
        	s32Ret |= HI_GV_Widget_Update(hWidget, HI_NULL);
    	}
		else
		{
			s32Ret |= HI_GV_Widget_GetRect(hWidget, &rect);
			s32Ret |= HI_GV_Widget_Show(hWidget);
			s32Ret |= HI_GV_Widget_Move(hWidget, 340, rect.y);
        	s32Ret |= HI_GV_Widget_Update(hWidget, HI_NULL);
		}
		#ifndef CFG_LAO
		snprintf(bufSsid, 30, "%s", g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID);
		snprintf(bufPwd, 30, "%s", g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD);
		HI_GV_Widget_SetText(HOME_PAGE_label_wifi_ssid, bufSsid);
	    HI_GV_Widget_SetText(HOME_PAGE_label_wifi_pwd, bufPwd);
		if (!u32Value)
		{
			s32Ret |= HI_GV_Widget_Hide(HOME_PAGE_groupbox_wifi);
		}
		else
		{
			s32Ret |= HI_GV_Widget_Show(HOME_PAGE_groupbox_wifi);
		}
		#endif
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(hWidget);
		#ifndef CFG_LAO
		s32Ret |= HI_GV_Widget_Hide(HOME_PAGE_groupbox_wifi);
		#endif
    }
	#ifndef CFG_LAO
    if(HI_TRUE == HI_GV_Widget_IsShow(HOME_PAGE_LABEL_COUNTDOWN))
    {
        s32Ret |= HI_GV_Widget_Hide(HOME_PAGE_groupbox_wifi);
    }
	#endif
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 UI_SetBluetoothControl(HI_HANDLE hWidget)
{
	HI_S32 s32Ret = HI_SUCCESS;
	HI_S32 s32State = HI_FALSE;
	HI_RECT rect = {0};

	s32Ret = FeiYuCmdExecute(CMD_GET_BLUETOOTH_STATE, 0, &s32State);
	if(TURN_BLUETOOTH_ON == s32State)
	{
		if(HI_TRUE == g_bBatCapacityShow)
		{
			if(HI_TRUE == g_bWifiIconShow)
			{
				s32Ret |= HI_GV_Widget_GetRect(hWidget, &rect);
				s32Ret |= HI_GV_Widget_Show(hWidget);
				s32Ret |= HI_GV_Widget_Move(hWidget, 310, rect.y);
	        	s32Ret |= HI_GV_Widget_Update(hWidget, HI_NULL);
			}
			else
			{
				s32Ret |= HI_GV_Widget_GetRect(hWidget, &rect);
				s32Ret |= HI_GV_Widget_Show(hWidget);
				s32Ret |= HI_GV_Widget_Move(hWidget, 340, rect.y);
	        	s32Ret |= HI_GV_Widget_Update(hWidget, HI_NULL);
			}
		}
		else
		{
			if(HI_TRUE == g_bWifiIconShow)
			{
				s32Ret |= HI_GV_Widget_GetRect(hWidget, &rect);
				s32Ret |= HI_GV_Widget_Show(hWidget);
				s32Ret |= HI_GV_Widget_Move(hWidget, 368, rect.y);
	        	s32Ret |= HI_GV_Widget_Update(hWidget, HI_NULL);
			}
			else
			{
				s32Ret |= HI_GV_Widget_GetRect(hWidget, &rect);
				s32Ret |= HI_GV_Widget_Show(hWidget);
				s32Ret |= HI_GV_Widget_Move(hWidget, 398, rect.y);
	        	s32Ret |= HI_GV_Widget_Update(hWidget, HI_NULL);
			}
		}
	}
	else if(TURN_BLUETOOTH_OFF == s32State)
	{
		s32Ret |= HI_GV_Widget_Hide(hWidget);
	}

	if (HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }
	
	return s32Ret;
}

HI_S32 UI_AlarmDisplay(HI_UI_ALARM_TYPE_E alarmtype )
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret |= HI_GV_Msg_SendAsync(ALARM_WINDOW, HIGV_MSG_EVENT, 0, alarmtype);

    return s32Ret;
}

HI_VOID UI_ProcessErrorCode(HI_S32 s32ErrorCode )
{
    switch(s32ErrorCode)
    {
        case HI_ERR_NO_AVAILABLE_RECORD_SPACE:
            MLOGD("Error %#x processed\n", s32ErrorCode);
            UI_AlarmDisplay(ALARM_REC_SPACEFULL);
            break;
        case HI_ERR_FILEMGR_DISK_NOT_SCANDONE:
            MLOGD("Error %#x processed\n", s32ErrorCode);
            UI_AlarmDisplay(ALARM_SD_NOEXIST);
            break;
        default:
            MLOGD("Error %#x not processed\n", s32ErrorCode);
            break;
    }
}


HI_S32 UI_GetParaValFromBtnIds(HI_HANDLE hWidget, SET_ITEM_S* pstSetItems, HI_U32 num, HI_U32* val)
{
    int i = 0;

    //HI_S32 s32Ret = HI_SUCCESS;
    //i begins from 1 to skip back button
    for (i = 1; i < num; i++)
    {
        if (hWidget == pstSetItems[i].hButton)
        {
            *val = pstSetItems[i].u32ParaVal;
            break;
        }
    }

    if (i == num)
    {
        MLOGE("getParaValFromBtnIds \n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}



HI_S32 UI_SetItemSelected(HI_HANDLE hWidget,  SET_ITEM_S* pstSetItems, HI_U32 num)
{
    //get the label by relative button id(hWidget)
    HI_U32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 tmp = 0;

    for (i = 1; i < num; i++)
    {
        if (pstSetItems[i].hButton == hWidget)
        {
            MLOGD("items: \n");
            MLOGD("hButton:%d, hint:%s \n", pstSetItems[i].hButton, pstSetItems[i].aszHint);
            break;
        }
    }

    if (i >= num)
    {
        MLOGE("UI_RepaintSetPage");
        return HI_FAILURE;
    }

    tmp = i;//save i

    //clean all the SELECT_ICON to ensure only one is marked
    for (i = 1; i < num; i++)
    {
        snprintf(pstSetItems[i].aszValStr, 16, "%s", " ");
        HI_GV_Widget_SetText(pstSetItems[i].hLabel, pstSetItems[i].aszValStr);
    }

    s32Ret |= HI_GV_Widget_SetText(pstSetItems[tmp].hLabel, SELECT_ICON);
    return HI_SUCCESS;
}

HI_S32 ui_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
#ifdef CFG_TIME_MEASURE_ON
    HI_Time_Stamp_Clean();
    TIME_STAMP;
#endif
    if(ALARM_WINDOW == hWidget)
    {
        HIGV_GESTURE_EVENT_S gestureEvent = {};
        HI_S32 x = 0, y = 0;
        memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
        x = gestureEvent.gesture.tap.pointer.x;
        y = gestureEvent.gesture.tap.pointer.y;        
        if(!((x>300) && (x < 480) && (y > 0) && (y < 200)))
        {
            return HIGV_PROC_GOON;
        }
        
    }

    HIGV_INPUTEVENT_S stInputEvent;
    HI_S32 s32Ret = HI_SUCCESS;
    memset(&stInputEvent, 0, sizeof(stInputEvent));
    HI_GV_Widget_Active(hWidget);
    stInputEvent.msg = HIGV_MSG_KEYDOWN;
    stInputEvent.value = HIGV_KEY_F1;
    s32Ret = HI_GV_SendInputEvent(&stInputEvent);

    if (s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_GOON ;
}


HI_S32 UI_GetIdxFromEnum(ENUM_IDX_S* pstEnumIdx, HI_U32 num, HI_U32 enumVal, HI_U32* idx)
{
    HI_U32 i;

    for (i = 0; i < num; i++)
    {
        if (pstEnumIdx[i].enumVal == enumVal)
        {
            *idx = pstEnumIdx[i].u32Idx;
            break;
        }
    }

    return HI_SUCCESS;
}

HI_VOID UI_LabelMapButton(HI_HANDLE aLable[], HI_HANDLE aButton[], HI_HANDLE LabelHandle, HI_HANDLE* pButtonHandle, HI_U32 size)
{
    HI_U32 i = 0;

    for (i = 1; i < size; i++) //begin from 1
    {
        if (aLable[i] == LabelHandle)
        {
            *pButtonHandle = aButton[i];
            break;
        }
    }
}

static HI_VOID setchargeLedState()
{
    HI_BOOL bACState = HI_FALSE;
    HI_BOOL bACFullState = HI_FALSE;
    static HI_U8 s_u8Account = 0;   
    static HI_BOOL s_bfirst = HI_TRUE;

    HI_HAL_BAT_GetState(&bACState);

    if (bACState)
    {
        HAL_Battery_Full_GetState(&bACFullState);
        if(bACFullState == HI_TRUE)
        {         
            feiyuAcBreathLed(HI_FALSE);
            feiyuFullAcLed(HI_TRUE);
            s_bfirst = HI_TRUE;
        }
        else
        {
            if(s_u8Account++ > 10 || s_bfirst)
            {
                feiyuFullAcLed(HI_FALSE);
                feiyuAcBreathLed(HI_TRUE);
                s_u8Account = 0;
                s_bfirst = HI_FALSE;
            }                    
        }

    }
    else
    {
        feiyuFullAcLed(HI_FALSE);
        feiyuAcBreathLed(HI_FALSE);
    }

}

void * UI_MonitorActiveWindow(void* arg)
{
	HI_HANDLE window = 0;
	HI_S32 s32Ret = 0;
	static HI_U32 u32InactiveCount = 0;
    static HI_U8 u8RunCount = 0;

	prctl(PR_SET_NAME, (unsigned long)"ChkActiveWin", 0, 0, 0);
	while(HI_GV_App_IsActive())
	{

		//if there is no active window,  keys will not be responsed.
		s32Ret = HI_GV_Widget_GetActiveWidget(HIGV_WND_MANAGER_HANDLE, &window);
		if(HI_SUCCESS == s32Ret)
		{
			u32InactiveCount = 0;
		}
		else
		{
			u32InactiveCount++;
			MLOGE("No Active Window, count=%u\n", u32InactiveCount);
			if(u32InactiveCount >=3 )
			{
				HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
			}
		}
        if(u8RunCount < 3)
        {
            u8RunCount++;
        }
        else
        {
            setchargeLedState();
        }
		sleep(1);
	}

    return NULL;
}


HI_S32 FEIYU_TAKE_PHTO_OnKeyDown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 state = 0;
    HI_S32 s32Ret = 0;
    FeiYuCmdExecute(CMD_GET_CURRENT_WORK_MODE, 0, &state);        
    if(PHOTO == state)
    {
        FeiYuCmdExecute(CMD_GET_PHOTO_FILETYPE, 0, &state);   
        if(FILETYPE_JPG_RAW == state)
        {
            s32Ret |= HI_GV_Widget_SetTextByID(TAKE_PHOTO_label, ID_STR_SAVE_RAW);
            s32Ret = HI_GV_Widget_Show(FEIYU_TAKE_PHTO);
            s32Ret |= HI_GV_Widget_Show(TAKE_PHOTO_label);
            s32Ret |= HI_GV_Widget_MoveToTop(FEIYU_TAKE_PHTO);
            s32Ret |= HI_GV_Widget_Update(FEIYU_TAKE_PHTO, 0);
            s32Ret |= HI_GV_Widget_Refresh(FEIYU_TAKE_PHTO, 0);
            s32Ret |= HI_GV_Widget_Paint(FEIYU_TAKE_PHTO, 0);
            s32Ret |= HI_GV_Widget_Paint(HOME_PAGE, 0);                   
            if (s32Ret != HI_SUCCESS)
            {
                MLOGE("Error:%#x\n", s32Ret);
            } 
        }
    }
    
    return HIGV_PROC_GOON;
}

HI_S32 ShowWriteRaw()
{
    //HI_GV_Msg_SendAsync(FEIYU_TAKE_PHTO,  HIGV_MSG_KEYDOWN, HIGV_KEY_F2, 0);  
    //HI_GV_Msg_SendSync(FEIYU_TAKE_PHTO,  HIGV_MSG_KEYDOWN, HIGV_KEY_F2, 0, 100);  
    HI_S32 state = 0;
    HI_S32 s32Ret = 0;
    FeiYuCmdExecute(CMD_GET_CURRENT_WORK_MODE, 0, &state);        
    if(PHOTO == state)
    {
        FeiYuCmdExecute(CMD_GET_PHOTO_FILETYPE, 0, &state);   
        if(FILETYPE_JPG_RAW == state)
        {
            s32Ret |= HI_GV_Widget_SetTextByID(TAKE_PHOTO_label, ID_STR_SAVE_RAW);
            s32Ret = HI_GV_Widget_Show(FEIYU_TAKE_PHTO);
            s32Ret |= HI_GV_Widget_Show(TAKE_PHOTO_label);
            s32Ret |= HI_GV_Widget_MoveToTop(FEIYU_TAKE_PHTO);
            s32Ret |= HI_GV_Widget_Update(FEIYU_TAKE_PHTO, 0);
            s32Ret |= HI_GV_Widget_Refresh(FEIYU_TAKE_PHTO, 0);
            s32Ret |= HI_GV_Widget_Paint(FEIYU_TAKE_PHTO, 0);
            s32Ret |= HI_GV_Widget_Paint(HOME_PAGE, 0);                   
            if (s32Ret != HI_SUCCESS)
            {
                MLOGE("Error:%#x\n", s32Ret);
            } 
        }
    }    
}

HI_S32 HideWriteRaw()
{
    HI_S32 state = 0;
    HI_S32 s32Ret = 0;
    FeiYuCmdExecute(CMD_GET_CURRENT_WORK_MODE, 0, &state);        
    if(PHOTO == state)
    {
        FeiYuCmdExecute(CMD_GET_PHOTO_FILETYPE, 0, &state);   
        if(FILETYPE_JPG_RAW == state)
        {
            s32Ret = HI_GV_Widget_Hide(FEIYU_TAKE_PHTO);
            s32Ret |= HI_GV_Widget_Hide(TAKE_PHOTO_label);              
            s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
            s32Ret |= HI_GV_Widget_Paint(HOME_PAGE, 0);
            if (s32Ret != HI_SUCCESS)
            {
                MLOGE("Error:%#x\n", s32Ret);
            } 

        }
    }    
}

HI_S32 ShowTakePhoto(HI_BOOL bFlag)
{
    HI_S32 state = 0;
    HI_S32 s32Ret = 0;
    static HI_BOOL bBoot = HI_TRUE;
    if(bBoot)
    {
        s32Ret = HI_GV_PARSER_LoadViewById(FEIYU_TAKE_PHTO);
        bBoot = HI_FALSE;
    }
    
    FeiYuCmdExecute(CMD_GET_CURRENT_WORK_MODE, 0, &state);        
    if(PHOTO == state)
    {
        HI_BOOL bShowTakePhototUI = HI_TRUE;
        FeiYuCmdExecute(CMD_GET_PHOTO_SCENE, 0, &state);
        switch(state)
        {
            case PHOTO_SCENE_HDR:
                s32Ret |= HI_GV_Widget_SetTextByID(TAKE_PHOTO_label, ID_STR_TAKING_PHOTO_HDR);                                 
                break;
            case PHOTO_SCENE_AUTO:
                s32Ret |= HI_GV_Widget_SetTextByID(TAKE_PHOTO_label, ID_STR_TAKING_PHOTO_AUTO);
                break;            
            case PHOTO_SCENE_NORMAL:
                s32Ret |= HI_GV_Widget_SetTextByID(TAKE_PHOTO_label, ID_STR_TAKING_PHOTO_NORMAL);
                break;               
            case PHOTO_SCENE_FAST:
                FeiYuCmdExecute(CMD_GET_PHOTO_FILETYPE, 0, &state);
                if(FILETYPE_JPG_RAW == state)
                {
                    s32Ret |= HI_GV_Widget_SetTextByID(TAKE_PHOTO_label, ID_STR_SAVE_RAW);
                }
                else
                {
                    bShowTakePhototUI = HI_FALSE;
                }
            default:
                break;
        }
            
        if( HI_TRUE == bShowTakePhototUI)
        {
            if (HI_TRUE == bFlag)
            {
                s32Ret = HI_GV_Widget_Show(FEIYU_TAKE_PHTO);
                s32Ret |= HI_GV_Widget_Show(TAKE_PHOTO_label);
                //s32Ret |= HI_GV_Widget_Active(FEIYU_TAKE_PHTO);
                s32Ret |= HI_GV_Widget_MoveToTop(FEIYU_TAKE_PHTO);
                s32Ret |= HI_GV_Widget_Update(FEIYU_TAKE_PHTO, 0);
                s32Ret |= HI_GV_Widget_Refresh(FEIYU_TAKE_PHTO, 0);
                s32Ret |= HI_GV_Widget_Paint(FEIYU_TAKE_PHTO, 0);
                s32Ret |= HI_GV_Widget_Paint(HOME_PAGE, 0);    
                s_bShowDvActionUI = HI_TRUE;
                if (s32Ret != HI_SUCCESS)
                {
                    MLOGE("Error:%#x\n", s32Ret);
                }                
            }
            else
            {
                s32Ret = HI_GV_Widget_Hide(FEIYU_TAKE_PHTO);
                s32Ret |= HI_GV_Widget_Hide(TAKE_PHOTO_label);
                //s32Ret |= HI_GV_Widget_Update(HOME_PAGE, 0);
                //s32Ret |= HI_GV_Widget_Refresh(HOME_PAGE, 0);
                s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
                s32Ret |= HI_GV_Widget_Paint(HOME_PAGE, 0); 
                s_bShowDvActionUI = HI_FALSE;
                if (s32Ret != HI_SUCCESS)
                {
                    MLOGE("Error:%#x\n", s32Ret);
                }                            
            }
        }
    }
    else if(PHOTO_DELAY == state || MULTI_PHOTO == state)
    {
        s32Ret |= HI_GV_Widget_SetTextByID(TAKE_PHOTO_label, ID_STR_TAKING_PHOTO);
        if (HI_TRUE == bFlag)
        {
            s32Ret = HI_GV_Widget_Show(FEIYU_TAKE_PHTO);
            s32Ret |= HI_GV_Widget_Show(TAKE_PHOTO_label);
            //s32Ret |= HI_GV_Widget_Active(FEIYU_TAKE_PHTO);
            s32Ret |= HI_GV_Widget_MoveToTop(FEIYU_TAKE_PHTO);
            s32Ret |= HI_GV_Widget_Update(FEIYU_TAKE_PHTO, 0);
            s32Ret |= HI_GV_Widget_Refresh(FEIYU_TAKE_PHTO, 0);                
            s32Ret |= HI_GV_Widget_Paint(FEIYU_TAKE_PHTO, 0);
            s32Ret |= HI_GV_Widget_Paint(HOME_PAGE, 0);  
            s_bShowDvActionUI = HI_TRUE;
            if (s32Ret != HI_SUCCESS)
            {
                MLOGE("Error:%#x\n", s32Ret);
            }                
        }
        else
        {
            s32Ret = HI_GV_Widget_Hide(FEIYU_TAKE_PHTO);
            s32Ret |= HI_GV_Widget_Hide(TAKE_PHOTO_label);              
            s32Ret |= HI_GV_Widget_Hide(HOME_PAGE_LABEL_COUNTDOWN);      
            s32Ret |= HI_GV_Widget_Refresh(HOME_PAGE_LABEL_COUNTDOWN, 0);                
            s32Ret |= HI_GV_Widget_Paint(HOME_PAGE_LABEL_COUNTDOWN, 0);            
            s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
            s32Ret |= HI_GV_Widget_Paint(HOME_PAGE, 0);
            s_bShowDvActionUI = HI_FALSE;
            if (s32Ret != HI_SUCCESS)
            {
                MLOGE("Error:%#x\n", s32Ret);
            }                            
        }            
    }
}



HI_VOID feiyuShowUnlockPage()
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_PDT_WORKSTATE_S stWorkState;
    
    s32Ret = HI_PDT_EventProc_GetState(&stWorkState);
    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetState failed\n");
    }
    
    if (PDT_CMD_START != stWorkState.enCmd && (HI_FALSE == HI_GV_Widget_IsShow(FEIYU_STARTUP_GUIDER)))
    {
        HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_BACK_HOME, 0);        
    }
    
    HI_GV_Msg_SendAsync(FEIYU_UNLOCK_PAGE_CLICK, HIGV_MSG_EVENT, 0, 0);
    
}
HI_VOID feiyuJumpHomePage()
{
    HI_PDT_WORKMODE_S stWorkMode = {0};
    HI_PDT_WorkMode_GetWorkMode(&stWorkMode);
    if (PDT_WORKMODE_PLAYBACK != stWorkMode.enWorkMode)
    {
        if(HI_FALSE == HI_GV_Widget_IsShow(HOME_PAGE))
        {
            HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_BACK_HOME, 0);
            #if 1
            while(1)
            {
                //MLOGE("====>>>%d\n",HI_GV_Widget_IsShow(HOME_PAGE));
                if(HI_TRUE == HI_GV_Widget_IsShow(HOME_PAGE))
                {
                    break;
                }
                usleep(100);
            }
            #endif
            bJumpHomePageByApp = HI_TRUE;
        }

    }
    HI_PDT_TimedTask_Reset(PDT_TIMEDTASK_TYPE_AUTOOFF);
}


HI_VOID FeiyuEntryGuiderUI()
{
    MLOGE("***\n");
    HI_S32 s32Ret = HI_SUCCESS;
    s32Ret |= HI_GV_PARSER_LoadViewById(FEIYU_USER_GUIDER);
    s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER);
    s32Ret |= HI_GV_Widget_Active(FEIYU_USER_GUIDER);
	if (s32Ret != HI_SUCCESS)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }     
}


#if 1
static HI_S32 checkUpWareFile()
{
    HI_CHAR buff[64] = {0};
    HI_CHAR buff1[64] = {0};
    HI_S32 ret = 0;
    FILE *fp = NULL;
    fp = fopen("/app/sd/update_pkg_e", "rb+");
    if(!fp)
    {
        MLOGE("ERR\n");
        return -1;
    }
    
    fseek(fp,-32L,2);
    
    ret = fread(buff, sizeof(HI_CHAR), 32, fp);
    if(ret != 32)
    {
        MLOGE("ERR\n");
        fclose(fp);
        return -1;
    }
    buff[32] = '\0';
    fclose(fp);
    truncate("/app/sd/update_pkg_e", 32571392);
    
    system("md5sum /app/sd/update_pkg_e > /app/sd/a.txt");

    int i = 0;
    do{
        ret = access("/app/sd/a.txt",F_OK);
        if(0 == ret)
            break;
        usleep(500*1000);
    }while(i++ < 10);
    if(ret != 0)
    {
        MLOGE("ERR\n");
        return -1;
    }
    fp = fopen("/app/sd/a.txt", "rb+");
    if(!fp)
    {
        MLOGE("ERR\n");
        return -1;        
    }
    ret = fread(buff1, sizeof(HI_CHAR), 32, fp);
    if(ret != 32)
    {
        MLOGE("ERR\n");
        fclose(fp);
        return -1;
    }    
    buff1[32] = '\0';
    system("rm /app/sd/a.txt");    
    fclose(fp);
    MLOGE("buff=%s,buff1=%s\n",buff, buff1);
    if(0 != strcmp(buff, buff1))
    {
        MLOGE("file err!\n");
        return -1;
    }
    rename("/app/sd/update_pkg_e", "/app/sd/update_pkg");
    return 0;        
}

static HI_S32 startUpware()
{
    HI_S32 s32Ret = 0;
    sleep(1);
    s32Ret = checkUpWareFile();
    if(s32Ret < 0)
    {
        MLOGE("checkUpWareFile failed!\N");    
        s32Ret |= UI_AlarmDisplay(ALARM_CHECK_FILE_ERR);
        return -1;
    }
    MLOGE("checkUpWareFile SUCCESS!\N");
    #if 0
	extern int himm(unsigned int addr, unsigned int val);
	//set flag
	himm(0x800FFFE0,0x424F4F54);
	//set flag
	himm(0x800FFFE4,1);
	FeiYuCmdExecute(CMD_REBOOT, 0, NULL);
    #endif
    s32Ret |= UI_AlarmDisplay(ALARM_START_UPWARE);
    sleep(3);
    system("update_enc");

}
HI_VOID doUpware(void *arg)
{
    HI_BOOL bACState = HI_FALSE;
    HI_BOOL bBatteryCharge = HI_FALSE;
    HI_S32 s32BatteryCapacity = 0;
    HI_S32 s32Ret = 0;

    pthread_detach(pthread_self());
    usleep(1000);

    //
    s32Ret = HI_BATMNG_GetInfo(&bACState, &bBatteryCharge, &s32BatteryCapacity);
    HI_HAL_BAT_GetState(&bACState);
    if(bACState)
    {
        s32Ret |= UI_AlarmDisplay(ALARM_DO_NOT_PULL_OUT_AC);

        startUpware();
    }
    else
    {
        if(s32BatteryCapacity < 50)
        {
            s32Ret |= UI_AlarmDisplay(ALARM_LOW_POWER);
        }
        else
        {
            s32Ret |= UI_AlarmDisplay(ALARM_DO_NOT_PULL_OUT_BATTERY);
            startUpware();
        }
    }
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR!!!");
    }
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

