#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "higv_cextfile.h"
#include "ui_common.h"
#include "uiconstants.h"
#include "hi_gv_scrollview.h"
#include "hi_gv_parser.h"
#include "higv_language.h"
#include "hi_wifi_mng.h"
#include "hi_product_param_ext.h"
#include "hi_gv_gesture.h"
#include "hi_gv_input.h"
#include "hi_product_rawdump.h"
#include "hi_product_playback_ext.h"
#include "hi_product_err.h"
#include "hi_product_photo_ext.h"
#include "hi_product_timedtask.h"

#include "feiyu_dv_cmd_proc.h"
#include "feiyu_UICommon_FunTool.h"
#include "feiyu_UIAnimationTool.h"
#include "feiyu_ui_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*  __cplusplus  */
#define PDT_CHECK_EXPR_GOTO_ERROR_0(expr) \
        do{ \
            if (!(expr)){   \
                MLOGE(" expr[%s] false\n", #expr); \
                return HIGV_PROC_GOON;  \
            }   \
        }while(0)


HI_U32 g_u32ShowRemainSnapCount = 0;
HI_U32 g_count = 0;
extern HI_PDT_PARAM_CFG_S* g_pstPDTCfg;
//extern HI_BOOL g_bIsMenuShow;
static HI_BOOL isHiddenBottomBar = HI_FALSE;
static HI_BOOL isRecording = HI_FALSE;
HI_BOOL g_bHideCountDown = HI_FALSE;
extern HI_U32 g_filelist_flag;
extern HI_BOOL g_isFromHTTPShouldReport;
extern HI_BOOL g_bIsDvLocking;
static HI_BOOL s_bFlag = HI_TRUE;
static HI_BOOL s_bIsShowing = HI_FALSE;
static HI_BOOL s_bShowFilelistAnim = HI_FALSE;
static HI_BOOL s_bIsTopBottomAnim = HI_FALSE;
static HI_BOOL s_bScroll = HI_FALSE;
static HI_BOOL s_bRecHideBar = HI_FALSE;
HI_BOOL g_pageChangeEnd = HI_TRUE;
extern USER_PARA_S* g_pstUserPara;
HI_BOOL bRecordStart = HI_FALSE;

HI_VOID UI_ShowFileListPage();

#ifdef CFG_HJD
HI_S32 zoom_level = 0;
static HI_PDT_WORKMODE_S s_stWorkMode = {0};
static HI_PDT_VIDEO_MODE_E s_enVideoMode;
static HI_S32 g_startY = 0, g_startX =0;	
extern HI_PDT_WORKMODE_S g_stCurrentWorkMode;
#endif

/************************Animation API************************/
static HI_HANDLE animation_TopGroupbox_YHandle = 0;
static HI_HANDLE animation_BottomGroupbox_YHandle = 0;
static HI_HANDLE animation_GeneralFunction_YHandle = 0;
static void animMoveY_TopGroupBoxUpdate_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_TopGroupBoxStart_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_TopGroupBoxEnd_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_BottomGroupBoxUpdate_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_BottomGroupBoxStart_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_BottomGroupBoxEnd_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_GeneralFun_Update_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_GeneralFun_Start_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_GeneralFun_End_handle(HIGV_LISTENER_S* listener, void* data);
static void moveY_TopGroupbox(HIGV_CORD fromValue,HIGV_CORD toValue);
static void moveY_BottomGroupbox(HIGV_CORD fromValue,HIGV_CORD toValue);
static void moveY_GeneralFunction(HIGV_CORD fromValue,HIGV_CORD toValue,HI_U32 animTime);
static HI_VOID doHideBar();
/************************Animation API************************/

HI_VOID gku_zoom_control(HI_U32 option);

HI_PDT_WORKMODE_S g_stSavedWorkMode;

static HI_VOID UI_ShowFeiYuUnlockPage()
{
	HI_S32 s32Ret = HI_SUCCESS;
	s32Ret = HI_GV_PARSER_LoadViewById(FEIYU_UNLOCK_PAGE_CLICK);
	s32Ret |= HI_GV_Widget_Show(FEIYU_UNLOCK_PAGE_CLICK);
	s32Ret |= HI_GV_Widget_Active(FEIYU_UNLOCK_PAGE_CLICK);
	s32Ret |= HI_GV_Widget_Hide(HOME_PAGE);

	if (HI_SUCCESS != s32Ret)
	{
		MLOGE("HIGV Error: %#x\n", s32Ret);
	}
}


static HI_VOID UI_ShowFeiYuGeneralFunction()
{
	HI_S32 s32Ret = HI_SUCCESS;
    s32Ret = HI_GV_PARSER_LoadViewById(FEIYU_GENERAL_FUNCTION);
    s32Ret |= HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION);
	s32Ret |= HI_GV_Widget_Active(FEIYU_GENERAL_FUNCTION);
	s32Ret |= HI_GV_Widget_Hide(HOME_PAGE);
    // s32Ret |= HI_GV_Widget_Active(FEIYU_GENERAL_FUNCTION);
    //extern void FASTOPTION_AnimmoveYItem(HIGV_CORD fromValue,HIGV_CORD toValue);
    //FASTOPTION_AnimmoveYItem(-319, 1);

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
}


static HI_VOID UI_ShowFeiYuPhotoAlbum()
{
	UI_ShowFileListPage();

    HI_S32 s32Ret = HI_SUCCESS;

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }	
}

static HI_VOID UI_ShowFeiYuModeSet()
{
    s_bIsShowing = HI_TRUE;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_PDT_WORKSTATE_S stWorkState;
    s32Ret = HI_PDT_EventProc_GetState(&stWorkState);
    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetState failed\n");
    }    
    if(PDT_CMD_START == stWorkState.enCmd)
    {
        return ;
    }
    s32Ret = HI_PDT_WorkMode_GetWorkMode(&g_stSavedWorkMode);

    if (HI_SUCCESS != s32Ret)
    {
        g_stSavedWorkMode.enWorkMode = PDT_WORKMODE_RECORD;
        g_stSavedWorkMode.enRecMode = PDT_RECMODE_NORMAL;
    }

    s32Ret = HI_GV_PARSER_LoadViewById(FEIYU_MODE_SET);
    s32Ret |= HI_GV_Widget_Show(FEIYU_MODE_SET);
    s32Ret |= HI_GV_Widget_Active(FEIYU_MODE_SET);
	s32Ret |= HI_GV_Widget_Hide(HOME_PAGE);

    extern void ModeSetAnimmoveXItem(HIGV_CORD fromValue,HIGV_CORD toValue);
    ModeSetAnimmoveXItem(320, 0);   


    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }

}
static HI_VOID UI_ShowFeiYuGlobalSettings()
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_PDT_WORKSTATE_S stWorkState;
    s32Ret = HI_PDT_EventProc_GetState(&stWorkState);
    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetState failed\n");
    }    
    if(PDT_CMD_START == stWorkState.enCmd)
    {
        return ;
    }

    s32Ret |= HI_GV_PARSER_LoadViewById(FEIYU_GLOBAL_SETTINGS);
    s32Ret |= HI_GV_Widget_Show(FEIYU_GLOBAL_SETTINGS);
    s32Ret |= HI_GV_Widget_Active(FEIYU_GLOBAL_SETTINGS);
    //s32Ret |= HI_GV_ScrollBar_SetStatus(GLOBAL_SETTING_SET_GROUPBOX_item_scrollbar, HI_TRUE);
    s32Ret |= HI_GV_Widget_Hide(HOME_PAGE);

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
	MLOGD("\n");
}

static HI_VOID UI_ShowFeiyuFastSet()
{
    HI_S32 s32Ret = HI_SUCCESS;
    //extern HI_BOOL s_bAnimRunning;
    //s_bAnimRunning = HI_TRUE;
    if(HI_FALSE == isHiddenBottomBar)
    {
        doHideBar();                    
    }
    s32Ret |= HI_GV_PARSER_LoadViewById(FEIYU_FAST_SET);
    s32Ret |= HI_GV_Widget_Show(FEIYU_FAST_SET);
    s32Ret |= HI_GV_Widget_Active(FEIYU_FAST_SET);
    
    extern void FastSetAnimmoveXItem(HIGV_CORD fromValue,HIGV_CORD toValue);
    FastSetAnimmoveXItem(480, 0); 


    //s32Ret |= HI_GV_Widget_Hide(HOME_PAGE);
    //s32Ret |= HI_GV_Widget_Hide(FEIYU_ZOOM_PAGE);    
    
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
	MLOGD("\n");
}


static HI_VOID FeiYuChangeWorkMode()
{
	HI_S32 s32Ret = HI_SUCCESS;

	HI_S32 s32CurrentWorkMode = 0;
	s32Ret = FeiYuCmdExecute(CMD_GET_CURRENT_WORK_MODE, 0, &s32CurrentWorkMode);
	if(HI_SUCCESS != s32Ret)
	{
		MLOGE("err \n");
	}

	switch(s32CurrentWorkMode)
	{
		case PHOTO:
		case PHOTO_TIMER:
		case PHOTO_DELAY:
			FeiYuCmdExecute(CMD_SET_WORKMODE_MULIT_PHOTO, 0xff, 0);
			break;
		case MULTI_PHOTO:
			FeiYuCmdExecute(CMD_SET_WORKMODE_VIDEO, 0xff, 0);
			break;
		case LIVE:
			FeiYuCmdExecute(CMD_SET_WORKMODE_VIDEO, 0xff, 0);
			break;
		case VIDEO_SNAP:
		case VIDEO:
		case VIDEO_LAPSE:
		case VIDEO_SLOW:
		case VIDEO_LOOP:
			FeiYuCmdExecute(CMD_SET_WORKMODE_PHOTO, 0xff, 0);
			break;
		default:
		break;
	}

}

HI_VOID UI_ShowFileListPage()
{	
    s_bIsShowing = HI_TRUE;
    #define SHOW_VIDEO  2
    g_filelist_flag = SHOW_VIDEO;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_PDT_WORKSTATE_S stWorkState;
    s32Ret = HI_PDT_EventProc_GetState(&stWorkState);
    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetState failed\n");
    }    
    if(PDT_CMD_START == stWorkState.enCmd)
    {
        return ;
    }
    s32Ret = HI_PDT_WorkMode_GetWorkMode(&g_stSavedWorkMode);

    if (HI_SUCCESS != s32Ret)
    {
        g_stSavedWorkMode.enWorkMode = PDT_WORKMODE_RECORD;
        g_stSavedWorkMode.enRecMode = PDT_RECMODE_NORMAL;
    }

    s32Ret = HI_GV_PARSER_LoadViewById(FILELIST_PAGE);
    s32Ret |= HI_GV_Widget_Show(FILELIST_PAGE);
    s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE);
    s32Ret |= HI_GV_Widget_Paint(FILELIST_PAGE, 0);
    
    if(s_bShowFilelistAnim)
    {
        extern void FilelistAnimmoveYItem(HIGV_CORD fromValue,HIGV_CORD toValue);
        FilelistAnimmoveYItem(-479.9, 0.1);        
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(HOME_PAGE);
        //s32Ret |= HI_GV_Widget_Hide(FEIYU_ZOOM_PAGE);
    }


    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
}

static HI_S32 UI_GetModeIcon(HI_PDT_WORKMODE_S stWorkMode)
{
    HI_S32 s32ModeSkinID = 0;
    if (PDT_WORKMODE_RECORD == stWorkMode.enWorkMode)
    {
    	if(PDT_RECMODE_NORMAL == stWorkMode.enRecMode)
    	{
    		s32ModeSkinID = home_page_mode_video;
    	}
		else if(PDT_RECMODE_LOOP == stWorkMode.enRecMode)
		{
			s32ModeSkinID = home_page_mode_video_loop;
		}
		else if(PDT_RECMODE_LAPSE == stWorkMode.enRecMode)
		{
			s32ModeSkinID = home_page_mode_lapse;
		}
		else if(PDT_RECMODE_RECSNAP == stWorkMode.enRecMode)
		{
			s32ModeSkinID = home_page_mode_video_snap;
		}
		else if(PDT_RECMODE_SLOW == stWorkMode.enRecMode)
		{
			s32ModeSkinID = home_page_mode_video_slow;
		}

    }
    else if (PDT_WORKMODE_MULTIPHOTO == stWorkMode.enWorkMode)
    {
    	if(PDT_MULTIPHOTOMODE_LAPSE == stWorkMode.enMultiPhotoMode)
    	{
		    s32ModeSkinID = home_page_mode_photo_delay;    		
    	}
		else
		{
		    s32ModeSkinID = home_page_mode_mulit_photo;
		}

    }
    else if (PDT_WORKMODE_PHOTO == stWorkMode.enWorkMode)
    {
        if ( PDT_PHOTOMODE_SINGLE == stWorkMode.enPhotoMode)
        {
            s32ModeSkinID = home_page_mode_photo;
        }
		else if(PDT_PHOTOMODE_DELAY == stWorkMode.enPhotoMode)
		{
			s32ModeSkinID = home_page_mode_photo_timer;
		}
        else
        {
            s32ModeSkinID = home_page_mode_photo;
        }
    }
    else
    {
        s32ModeSkinID = -1;
    }


    return s32ModeSkinID;
}


static HI_VOID UI_SetModeButtonSkin(HI_PDT_WORKMODE_S stWorkMode)
{
    HI_S32 s32ModeSkinID = 0;
    s32ModeSkinID = UI_GetModeIcon(stWorkMode);

    HI_GV_Widget_Show(HOME_PAGE_BUTTON_MODE);
    if(s32ModeSkinID > 0)
        HI_GV_Widget_SetSkin(HOME_PAGE_BUTTON_MODE, HIGV_SKIN_NORMAL, s32ModeSkinID);
}
static HI_S32 UI_GetModeCfg(HI_PDT_WORKMODE_S stWorkMode, HI_U32* pu32ConfigValue, HI_PDT_ITEM_VALUESET_S* pstValueSet)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32ConfigValue = 0;
    HI_PDT_ITEM_VALUESET_S stValueSet = {};
    if((pu32ConfigValue == NULL) || (pstValueSet == NULL))
    {
        MLOGE("UI_GetModeCfg NULL pointer.\n");
        return HI_FAILURE;
    }
    if (PDT_WORKMODE_RECORD == stWorkMode.enWorkMode)
    {
        if (PDT_RECMODE_RECSNAP == stWorkMode.enRecMode)
        {
            s32Ret |= HI_PDT_WorkMode_GetRecConf(stWorkMode.enRecMode, PDT_RECMODE_CONF_RECSNAP_MODE, (HI_VOID*)(&u32ConfigValue));
            s32Ret |= HI_PDT_WorkMode_GetRecItemValues(stWorkMode.enRecMode, PDT_RECMODE_CONF_RECSNAP_MODE, &stValueSet);
        }
        else
        {
            s32Ret |= HI_PDT_WorkMode_GetRecConf(stWorkMode.enRecMode, PDT_RECMODE_CONF_VIDEOMODE, (HI_VOID*)(&u32ConfigValue));
            s32Ret |= HI_PDT_WorkMode_GetRecItemValues(stWorkMode.enRecMode, PDT_RECMODE_CONF_VIDEOMODE, &stValueSet);
        }
    }
    else if (PDT_WORKMODE_MULTIPHOTO == stWorkMode.enWorkMode)
    {
        if (PDT_MULTIPHOTOMODE_BURST == stWorkMode.enMultiPhotoMode)
        {
            s32Ret |= HI_PDT_WorkMode_GetMultiPhotoConf(PDT_MULTIPHOTOMODE_BURST, PDT_MULTIPHOTOMODE_CONF_IMAGEMODE, (HI_VOID*)(&u32ConfigValue));
            s32Ret |= HI_PDT_WorkMode_GetMultiPhotoItemValues(PDT_MULTIPHOTOMODE_BURST, PDT_MULTIPHOTOMODE_CONF_IMAGEMODE, &stValueSet);
        }
        else if (PDT_MULTIPHOTOMODE_LAPSE == stWorkMode.enMultiPhotoMode)
        {
            s32Ret |= HI_PDT_WorkMode_GetMultiPhotoConf(PDT_MULTIPHOTOMODE_LAPSE, PDT_MULTIPHOTOMODE_CONF_IMAGEMODE, (HI_VOID*)(&u32ConfigValue));
            s32Ret |= HI_PDT_WorkMode_GetMultiPhotoItemValues(PDT_MULTIPHOTOMODE_LAPSE, PDT_MULTIPHOTOMODE_CONF_IMAGEMODE, &stValueSet);
        }
        else if (PDT_MULTIPHOTOMODE_CONTINUOUS == stWorkMode.enMultiPhotoMode)
        {
            s32Ret |= HI_PDT_WorkMode_GetMultiPhotoConf(PDT_MULTIPHOTOMODE_CONTINUOUS, PDT_MULTIPHOTOMODE_CONF_IMAGEMODE, (HI_VOID*)(&u32ConfigValue));
            s32Ret |= HI_PDT_WorkMode_GetMultiPhotoItemValues(PDT_MULTIPHOTOMODE_CONTINUOUS, PDT_MULTIPHOTOMODE_CONF_IMAGEMODE, &stValueSet);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (PDT_WORKMODE_PHOTO == stWorkMode.enWorkMode)
    {
        if (PDT_PHOTOMODE_SINGLE == stWorkMode.enPhotoMode)
        {
            s32Ret |= HI_PDT_WorkMode_GetPhotoConf(PDT_PHOTOMODE_SINGLE, PDT_PHOTOMODE_CONF_IMAGEMODE, (HI_VOID*)(&u32ConfigValue));
            s32Ret |= HI_PDT_WorkMode_GetPhotoItemValues(PDT_PHOTOMODE_SINGLE, PDT_PHOTOMODE_CONF_IMAGEMODE, &stValueSet);
        }
        else if (PDT_PHOTOMODE_DELAY == stWorkMode.enPhotoMode)
        {
            s32Ret |= HI_PDT_WorkMode_GetPhotoConf(PDT_PHOTOMODE_DELAY, PDT_PHOTOMODE_CONF_IMAGEMODE, (HI_VOID*)(&u32ConfigValue));
            s32Ret |= HI_PDT_WorkMode_GetPhotoItemValues(PDT_PHOTOMODE_DELAY, PDT_PHOTOMODE_CONF_IMAGEMODE, &stValueSet);
        }
        else
        {
            return HI_FAILURE;
        }
    }else
    {
        return HI_FAILURE-1;
    }

    *pu32ConfigValue = u32ConfigValue;
    memcpy(pstValueSet, &stValueSet, sizeof(stValueSet));
    return s32Ret;
}

static HI_VOID UI_SetLoopIcon(HI_PDT_WORKMODE_S stWorkMode)
{
    #if 0
    if (PDT_WORKMODE_RECORD == stWorkMode.enWorkMode && PDT_RECMODE_LOOP == stWorkMode.enRecMode)
    {
        HI_GV_Widget_Show(HOME_PAGE_BUTTON_LOOP);
    }
    else
    {
        HI_GV_Widget_Hide(HOME_PAGE_BUTTON_LOOP);
    }
    #endif
    HI_GV_Widget_Hide(HOME_PAGE_BUTTON_LOOP);
}
static HI_VOID UI_SetCountDownIcon(HI_PDT_WORKSTATE_S stWorkState)
{
    HI_CHAR buffer[3] = {0};

    if (PDT_WORKMODE_PHOTO == stWorkState.stWorkMode.enWorkMode
        && PDT_PHOTOMODE_DELAY == stWorkState.stWorkMode.enPhotoMode
        && PDT_CMD_START == stWorkState.enCmd && stWorkState.s32PastTime >= 0)
    {
        snprintf(buffer, sizeof(buffer), "%02d", stWorkState.s32PastTime);
        if(0 == stWorkState.s32PastTime)
            return;
        HI_GV_Widget_SetText(HOME_PAGE_LABEL_COUNTDOWN, buffer);
        if(HI_FALSE == g_bHideCountDown)
            HI_GV_Widget_Show(HOME_PAGE_LABEL_COUNTDOWN);
    }
    else
    {
        HI_GV_Widget_Hide(HOME_PAGE_LABEL_COUNTDOWN);
    }
}
static HI_VOID UI_SetRedDotIcon(HI_PDT_WORKSTATE_S stWorkState)
{
    #ifdef CFG_TIME_MEASURE_ON
    static HI_BOOL b_rec_flag = HI_FALSE;
    static HI_BOOL bFirst = HI_TRUE;
    #endif
    if (PDT_WORKMODE_RECORD == stWorkState.stWorkMode.enWorkMode
        && PDT_CMD_START == stWorkState.enCmd)
    {
    	//HI_GV_Widget_Hide(HOME_PAGE_LABEL_SD);
		HI_GV_Widget_Hide(HOME_PAGE_GROUPBOX1_BUTTON_PHOTO_ABLUM);
        HI_GV_Widget_Hide(HOME_PAGE_BUTTON_WIFI);
		//HI_GV_Widget_Hide(HOME_PAGE_LABEL_BATTERY);
		HI_GV_Widget_Hide(HOME_PAGE_BUTTON_MODE);
		//HI_GV_Widget_Hide(HOME_PAGE_GROUPBOX2_BUTTON_GLOBAL_SETTINGS);

        HI_GV_Widget_Show(HOME_PAGE_LABEL_RECORDING);
        HI_GV_Widget_Show(HOME_PAGE_LABEL_REMAINING_1);
        HI_GV_Widget_Hide(HOME_PAGE_LABEL_REMAINING);

        #ifdef CFG_TIME_MEASURE_ON
        if(HI_FALSE == b_rec_flag)
        {
            TIME_STAMP;
            if(!bFirst)
            {
                HI_Print_Time_Stamp();
                HI_Time_Stamp_Clean();
            }
            bFirst = HI_FALSE;
        }
        b_rec_flag = HI_TRUE;
        #endif
        HI_PDT_TimedTask_Reset(PDT_TIMEDTASK_TYPE_AUTOOFF);
    }
    else
    {
        HI_GV_Widget_Hide(HOME_PAGE_LABEL_RECORDING);
		HI_GV_Widget_Show(HOME_PAGE_LABEL_SD);
		HI_GV_Widget_Show(HOME_PAGE_GROUPBOX1_BUTTON_PHOTO_ABLUM);
		HI_GV_Widget_Show(HOME_PAGE_LABEL_BATTERY);
		HI_GV_Widget_Show(HOME_PAGE_BUTTON_MODE);
		HI_GV_Widget_Show(HOME_PAGE_GROUPBOX2_BUTTON_GLOBAL_SETTINGS);
        HI_GV_Widget_Hide(HOME_PAGE_LABEL_REMAINING_1);
        HI_GV_Widget_Show(HOME_PAGE_LABEL_REMAINING);  
		if((isHiddenBottomBar == HI_TRUE)&&(s_bRecHideBar == HI_TRUE))
		{
			s_bRecHideBar = HI_FALSE;
			doHideBar();
		}
		
        #ifdef CFG_TIME_MEASURE_ON
        if(HI_TRUE == b_rec_flag)
        {
            TIME_STAMP;
            if(!bFirst)
            {
                HI_Print_Time_Stamp();
                HI_Time_Stamp_Clean();
            }
            bFirst = HI_FALSE;
        }
        b_rec_flag = HI_FALSE;
        #endif
    }
}


#if 0
static HI_VOID UI_SetButtomCenterLabel(HI_PDT_WORKMODE_S stWorkMode, HI_PDT_WORKSTATE_S stWorkState)
{
    HI_CHAR buffer[32] = {0};
    DV_REC_TIME stPastTime;
    HI_U32 u32FileCount = 0;

    if (PDT_WORKMODE_RECORD == stWorkMode.enWorkMode)
    {
        HI_PDT_FileMng_GetVideoFileCount(&u32FileCount);
        snprintf(buffer, sizeof(buffer), "%04u", u32FileCount);
    }
    else if (PDT_MULTIPHOTOMODE_LAPSE == stWorkMode.enMultiPhotoMode
             && PDT_CMD_START_LAPSE == stWorkState.enCmd)
    {
        UI_ConvertTimerFormat(stWorkState.s32PastTime, &stPastTime);

        snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
                 stPastTime.s32Hour, stPastTime.s32Minute, stPastTime.s32Second);
    }
	//HI_GV_Widget_SetText(HOME_PAGE_LABEL_TIME, buffer);
	HI_GV_Widget_SetText(HOME_PAGE_LABEL_TIME, "");
}
#endif
static HI_VOID UI_SetVideoTimeLabel(HI_PDT_WORKMODE_S stWorkMode, HI_PDT_WORKSTATE_S stWorkState)
{
    HI_CHAR buffer[32] = {0};
    DV_REC_TIME stPastTime;
    HI_S32 s32PastTime = 0;
    DV_REC_TIME stRemainTime;
    HI_U32 u32RemainRecordTime = 0;
    HI_U32 u32RemainSnapCount = 0;
    HI_U32 u32FileCount = 0;
    
    if(PDT_WORKMODE_PLAYBACK == stWorkMode.enWorkMode)
    {
        return;
    }
    
    if (PDT_WORKMODE_RECORD == stWorkMode.enWorkMode)
    {
        s32PastTime = 0;

        if (PDT_CMD_START == stWorkState.enCmd)
        {
            s32PastTime = stWorkState.s32PastTime;
        }

        u32RemainRecordTime = 0;
        UI_GetRemainRecordTime(&u32RemainRecordTime);

        UI_ConvertTimerFormat(s32PastTime, &stPastTime);
        UI_ConvertTimerFormat(u32RemainRecordTime, &stRemainTime);

        // snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d/%02d:%02d:%02d",
        //          stPastTime.s32Hour, stPastTime.s32Minute, stPastTime.s32Second,
        //          stRemainTime.s32Hour, stRemainTime.s32Minute, stRemainTime.s32Second);
        
		// snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
        //          stRemainTime.s32Hour, stRemainTime.s32Minute, stRemainTime.s32Second);
        if(PDT_WORKMODE_STARTED == HI_PDT_WorkMode_GetState())
        {
            isRecording = HI_TRUE;
        }
        else
        {
            isRecording = HI_FALSE;
        }
        if(isRecording == HI_TRUE)
        {
            snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
                    stPastTime.s32Hour, stPastTime.s32Minute, stPastTime.s32Second);
            static HI_U8 s_count = 0;
            if( PDT_WORKMODE_STOPPED == HI_PDT_WorkMode_GetState())
            {
                if(s_count++ > 3)
                {
                    isRecording = HI_FALSE; 
                    s_count = 0;
                }
            }
            else
            {
                s_count = 0;
            }
        }
        else
        {
            snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
                 stRemainTime.s32Hour, stRemainTime.s32Minute, stRemainTime.s32Second);
        }
            

    }
    else
    {
        u32RemainSnapCount = 0;
        static HI_BOOL bInit = HI_TRUE;
        static HI_U32 u32Num = 100;
        if(bInit || ((g_count%u32Num) == 0))
        {
            UI_GetRemainSnapCount(&u32RemainSnapCount);
            g_u32ShowRemainSnapCount = u32RemainSnapCount;
            
            if(u32RemainSnapCount < 10)
            {
                u32Num = 1;
            }
            else if(u32RemainSnapCount < 50)
            {
                u32Num = 10;
            }
            else if(u32RemainSnapCount < 100)
            {
                u32Num = 50;
            }
            else
            {
                u32Num = 100;
            }
            bInit = HI_FALSE;
        }

        u32RemainSnapCount = g_u32ShowRemainSnapCount;

        u32FileCount = 0;
        HI_PDT_FileMng_GetImageFileCount(&u32FileCount);

        //snprintf(buffer, sizeof(buffer), "%04u/%04u", u32FileCount, u32RemainSnapCount);
        snprintf(buffer, sizeof(buffer), "%04u", u32RemainSnapCount);
    }
    HI_GV_Widget_SetText(HOME_PAGE_LABEL_REMAINING, buffer);
    HI_GV_Widget_SetText(HOME_PAGE_LABEL_REMAINING_1, buffer);    
}


static HI_VOID UI_SetModeLabel(HI_PDT_WORKMODE_S stWorkMode)
{
    HI_U32 u32ConfigValue = 0;
    HI_PDT_ITEM_VALUESET_S stValueSet = {};
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CHAR buffer[32] = {0};
    HI_U32 i= 0;
    s32Ret = UI_GetModeCfg(stWorkMode, &u32ConfigValue, &stValueSet);

    if (HI_SUCCESS == s32Ret)
    {
        for (i = 0; i < stValueSet.u32Num; i++)
        {
            if (stValueSet.stValues[i].u32Value == u32ConfigValue)
            {
                snprintf(buffer, sizeof(buffer), "%s", stValueSet.stValues[i].aszDesc);
                break;
            }
        }

        if( 0 == strcmp(buffer, "720P240"))
        {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s", "720P60");
        }

        HI_GV_Widget_SetText(HOME_PAGE_LABEL_CONFIG, buffer);
    }else if((HI_FAILURE -1) == s32Ret)
    {
        MLOGE("wait workmode...\n");
    }
    else
    {
        HI_GV_Widget_SetText(HOME_PAGE_LABEL_CONFIG, "");
        MLOGE("Get Mode Config Info Failed\n");
    }

}
#if 0
static HI_VOID HOME_PAGE_Hide()
{
//	HI_GV_Widget_Hide(HOME_PAGE);
	HI_GV_Widget_Hide(HOME_PAGE_GROUPBOX1);
	HI_GV_Widget_Hide(HOME_PAGE_GROUPBOX2);
}
#endif
static HI_VOID HOME_PAGE_Show()
{
	HI_GV_Widget_Show(HOME_PAGE_GROUPBOX1);
	HI_GV_Widget_Show(HOME_PAGE_GROUPBOX2);

	//HI_GV_Widget_Show(HOME_PAGE);
}

static HI_S32 UI_SetLabel()
{
    extern SubPageItemDes sa_SubPageItemDes[MAX_SUBPAGE];
    HI_S32 s32CurrentWorkMode = 0;
    HI_S32 s32LanguageType = 0;
    HI_S32 s32RetVal = 0;
    HI_S32 S32subPage = -1;
    HI_S32 i = 0, j = 0;
    FeiYuCmdExecute(CMD_GET_CURRENT_WORK_MODE, 0, &s32CurrentWorkMode);
    FeiYuCmdExecute(CMD_GET_LANGUAGE_TYPE, 0, &s32LanguageType);
    if(LANGUAGE_CHINESE == s32LanguageType || LANGUAGE_ENGLISH == s32LanguageType)
    {
    	switch(s32CurrentWorkMode)
    	{
    		case PHOTO:
                HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_2, ID_STR_FEIYU_MODE_PHOTO);
                break;
    		case PHOTO_TIMER:
                HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_2, ID_STR_FEIYU_MODE_PHOTO_TIMER);
                S32subPage = SUBPAGE_PHOTOTIMER_PARAM_DELAY_TIME;
                break;
    		case PHOTO_DELAY:
                HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_2, ID_STR_FEIYU_MODE_PHOTO_DELAY);
                S32subPage = SUBPAGE_PHOTODELAY_PARAM_DELAY_TIME;
                break;
    		case MULTI_PHOTO:
                HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_2, ID_STR_FEIYU_MODE_MULTI_PHOTO);
                S32subPage = SUBPAGE_MULTIPHOTO_BURST_BURSTTYPE;
                break;
    		case VIDEO_SNAP:
                HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_2, ID_STR_FEIYU_MODE_VIDEO_SNAP);
                S32subPage = SUBPAGE_VIDEOSNAP_LAPSEINTERVAL;
                break;
    		case VIDEO:
                HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_2, ID_STR_FEIYU_MODE_VIDEO);
                break;
    		case VIDEO_LAPSE:
                HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_2, ID_STR_FEIYU_MODE_VIDEO_LAPSE);
                S32subPage = SUBPAGE_VIDEOLAPSE_LAPSEINTERVAL;            
                break;
    		case VIDEO_SLOW:
                HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_2, ID_STR_FEIYU_MODE_VIDEO_SLOW);
                break;
    		case VIDEO_LOOP:
                HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_2, ID_STR_FEIYU_MODE_VIDEO_LOOP);
                S32subPage = SUBPAGE_VIDEOLOOP_LOOPTYPE;
                break;
    		default:
                HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_2, ID_STR_FEIYU_MODE_PHOTO);

                break;
    	}           
    }
    else
    {
    	switch(s32CurrentWorkMode)
    	{
    		case PHOTO:
                HI_GV_Widget_SetText(HOME_PAGE_MODE_LABEL_2, "PHOTO");
                break;
    		case PHOTO_TIMER:
                HI_GV_Widget_SetText(HOME_PAGE_MODE_LABEL_2, "PHOTO TIMER");
                S32subPage = SUBPAGE_PHOTOTIMER_PARAM_DELAY_TIME;
                break;
    		case PHOTO_DELAY:
                HI_GV_Widget_SetText(HOME_PAGE_MODE_LABEL_2, "PHOTO DELAY");
                S32subPage = SUBPAGE_PHOTODELAY_PARAM_DELAY_TIME;
                break;
    		case MULTI_PHOTO:
                HI_GV_Widget_SetText(HOME_PAGE_MODE_LABEL_2, "MULTI PHOTO");
                S32subPage = SUBPAGE_MULTIPHOTO_BURST_BURSTTYPE;
                break;
    		case VIDEO_SNAP:
                HI_GV_Widget_SetText(HOME_PAGE_MODE_LABEL_2, "VIDEO SNAP");
                S32subPage = SUBPAGE_VIDEOSNAP_LAPSEINTERVAL;
                break;
    		case VIDEO:
                HI_GV_Widget_SetText(HOME_PAGE_MODE_LABEL_2, "VIDEO");
                break;
    		case VIDEO_LAPSE:
                HI_GV_Widget_SetText(HOME_PAGE_MODE_LABEL_2, "VIDEO LAPSE");
                S32subPage = SUBPAGE_VIDEOLAPSE_LAPSEINTERVAL;            
                break;
    		case VIDEO_SLOW:
                HI_GV_Widget_SetText(HOME_PAGE_MODE_LABEL_2, "VIDEO SLOW");
                break;
    		case VIDEO_LOOP:
                HI_GV_Widget_SetText(HOME_PAGE_MODE_LABEL_2, "VIDEO LOOP");
                S32subPage = SUBPAGE_VIDEOLOOP_LOOPTYPE;
                break;
    		default:
                HI_GV_Widget_SetText(HOME_PAGE_MODE_LABEL_2, "PHOTO");

                break;
    	}          
    }
  
    if(S32subPage > 0)
    {
        HI_GV_Widget_Hide(HOME_PAGE_MODE_LABEL_interval_no);
        HI_GV_Widget_Show(HOME_PAGE_MODE_LABEL_interval);
        FeiYuCmdExecute(sa_SubPageItemDes[S32subPage].getCmdid, 0 , &s32RetVal);
        for(i = 0; i < MAX_ITEM_NUM; i++)
        {
            if(s32RetVal == sa_SubPageItemDes[S32subPage].FeiyuCmdParam[i])
            {
                j = i;
                break;
            }
        }
        HI_GV_Widget_SetTextByID(HOME_PAGE_MODE_LABEL_interval, sa_SubPageItemDes[S32subPage].strDesID[j]);
    }
    else
    {
        HI_GV_Widget_Show(HOME_PAGE_MODE_LABEL_interval_no);
        HI_GV_Widget_Hide(HOME_PAGE_MODE_LABEL_interval);
    }
       
    return 0;
}

HI_VOID HOME_PAGE_Refresh()
{
    if(HI_TRUE == s_bIsTopBottomAnim || HI_TRUE == s_bIsShowing)
    {
        return ;
    }
    HI_S32 s32Ret = HI_SUCCESS;

    HI_PDT_WORKSTATE_S stWorkState;
    HI_PDT_WORKMODE_S stWorkMode;
    HI_PDT_STORAGE_STATE_E enStorageState = PDT_STORAGE_STATE_DEVOUT;


    //prepare required data
    s32Ret = HI_PDT_EventProc_GetState(&stWorkState);

    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetState failed\n");
        return;
    }

    s32Ret = HI_PDT_WorkMode_GetWorkMode(&stWorkMode);

    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetWorkMode failed\n");
        return;
    }

    s32Ret = HI_PDT_Storage_GetState(g_pstPDTCfg->stStorageMngInfo.aszMountPath, &enStorageState);

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("GetStorageState failed\n");
        return;
    }

    //sd info
    UI_SetSDControl(HOME_PAGE_LABEL_SD, HOME_PAGE_BUTTON_AVAILABLE_PERCENT);

    //loop icon
    UI_SetLoopIcon(stWorkMode);

    //countdown circle
    UI_SetCountDownIcon(stWorkState);

	//bluetooth icon
	UI_SetBluetoothControl(HOME_PAGE_BUTTON_BLUETOOTH);

    //wifi icon
    UI_SetWifiControl(HOME_PAGE_BUTTON_WIFI);

    //battery icon
    UI_SetBatteryControl(HOME_PAGE_LABEL_BATTERY);
	
	//battery capacity
	UI_ShowBatteryCapacity();
	
    //mode icon
    UI_SetModeButtonSkin(stWorkMode);

    //recording flag, red circle
    UI_SetRedDotIcon(stWorkState);

    //set mode label
    UI_SetLabel();

    //bottom center label
    // UI_SetButtomCenterLabel(stWorkMode, stWorkState);
    //set Video Time label
    UI_SetVideoTimeLabel(stWorkMode, stWorkState);

    //mode config info(resolution, snap config)
    UI_SetModeLabel(stWorkMode);

    HI_GV_Widget_Paint(HOME_PAGE, 0);
}

void * NoticeBiBiBi(void* pVoid)
{
    pthread_detach(pthread_self());
    HI_S32 s32Count = 0;
    extern HI_VOID UpdateSubPageItemDes();
    UpdateSubPageItemDes();    
    usleep(500*1000);
    for(s32Count = 0; s32Count<3; s32Count++)
    {
        HI_U16 u16Audio = 0;
        extern HI_S32 HI_MAPI_AO_test(HI_U16 index, HI_U16 vol);
        HI_MAPI_AO_test(0, g_pstUserPara->commPara.u8PromtVoice);        
        usleep(10);
    }
	return NULL;
}

HI_VOID HI_WorkMode_Param_Compare()
{
	HI_PDT_VIDEO_MODE_E enVideoMode = {PDT_VIDEO_MODE_BUTT};
	HI_BOOL bChangeFlag = HI_FALSE;

	if (memcmp(&s_stWorkMode, &g_stCurrentWorkMode, sizeof(HI_PDT_WORKMODE_S)))
	{
		zoom_level = 0;
		bChangeFlag = HI_TRUE;
		memcpy(&s_stWorkMode, &g_stCurrentWorkMode, sizeof(HI_PDT_WORKMODE_S));
	    gku_zoom_control(zoom_level);
	}

	switch(s_stWorkMode.enWorkMode)
	{
		case PDT_WORKMODE_RECORD:
		{
			switch(s_stWorkMode.enRecMode)
			{
				case  PDT_RECMODE_NORMAL :
					enVideoMode = g_pstPDTCfg->stWorkModeParam.stRecModeCfg.stNormalAttr.stRecCommAttr.enVideoMode;
					break;
				case  PDT_RECMODE_LOOP :
					enVideoMode = g_pstPDTCfg->stWorkModeParam.stRecModeCfg.stLoopAttr.stRecCommAttr.enVideoMode;
					break;
		    	case  PDT_RECMODE_LAPSE :
					enVideoMode = g_pstPDTCfg->stWorkModeParam.stRecModeCfg.stLapseAttr.stRecCommAttr.enVideoMode;
					break;
			    case  PDT_RECMODE_RECSNAP:
					enVideoMode = g_pstPDTCfg->stWorkModeParam.stRecModeCfg.stRecSnapAttr.stRecCommAttr.enVideoMode;
					break;
		    	case  PDT_RECMODE_SLOW:
					enVideoMode = g_pstPDTCfg->stWorkModeParam.stRecModeCfg.stSlowAttr.stRecCommAttr.enVideoMode;
					break;
			}
			break;
		}
		case PDT_WORKMODE_PHOTO:
		{
			switch(s_stWorkMode.enPhotoMode)
			{
				case  PDT_PHOTOMODE_SINGLE :
					enVideoMode = g_pstPDTCfg->stWorkModeParam.stPhotoModeCfg.stSingleAttr.stCommAttr.enVideoMode;
					break;
				case  PDT_PHOTOMODE_DELAY :
					enVideoMode = g_pstPDTCfg->stWorkModeParam.stPhotoModeCfg.stDelayAttr.stCommAttr.enVideoMode;
					break;
			}
			break;
		}
		case PDT_WORKMODE_MULTIPHOTO:
		{
			switch(s_stWorkMode.enMultiPhotoMode)
			{
				case  PDT_MULTIPHOTOMODE_BURST :
					enVideoMode = g_pstPDTCfg->stWorkModeParam.stMultiPhotoModeCfg.stBurstAttr.stCommAttr.enVideoMode;
					break;
				case  PDT_MULTIPHOTOMODE_LAPSE:
					enVideoMode = g_pstPDTCfg->stWorkModeParam.stMultiPhotoModeCfg.stLapseAttr.stCommAttr.enVideoMode;
					break;
				case  PDT_MULTIPHOTOMODE_CONTINUOUS:
					enVideoMode = g_pstPDTCfg->stWorkModeParam.stMultiPhotoModeCfg.stContinuousAttr.stCommAttr.enVideoMode;
					break;
			}
			break;
		}
	}
	if(bChangeFlag == HI_FALSE)
	{
		if (memcmp(&s_enVideoMode, &enVideoMode, sizeof(HI_PDT_VIDEO_MODE_E)))
		{
			zoom_level = 0;
			memcpy(&s_enVideoMode, &enVideoMode, sizeof(HI_PDT_VIDEO_MODE_E));
			gku_zoom_control(zoom_level);
		}
	}
	else
	{
		bChangeFlag = HI_FALSE;
		memcpy(&s_enVideoMode, &enVideoMode, sizeof(HI_PDT_VIDEO_MODE_E));
	}

}

HI_S32 HOME_PAGE_OnShow(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    s_bScroll = HI_FALSE;
	g_pageChangeEnd = HI_TRUE;
	g_startX = -256;
	g_startY = -256;

    MLOGD("***\n");
    HI_S32 s32Ret = HI_SUCCESS;
    HI_GV_Timer_Create(HOME_PAGE, PREVIEW_TIMER, PREVIEW_TIMER_INTERVAL);
    HI_GV_Timer_Start(HOME_PAGE, PREVIEW_TIMER);
    HI_GV_Timer_Create(HOME_PAGE, HOME_PAGE_HIDE_TIMER, HOME_PAGE_HIDE_TIMER_INTERVAL);
    HI_GV_Timer_Start(HOME_PAGE, HOME_PAGE_HIDE_TIMER);

	HI_GV_Widget_Hide(HOME_PAGE_BUTTON_AVAILABLE_PERCENT);

	#ifdef CFG_HJD
	HI_WorkMode_Param_Compare();
	if(HI_GV_Widget_IsShow(HOME_PAGE_MISC))
	{
		HI_GV_Widget_Hide(HOME_PAGE_MISC);
	}
	#endif
	
	//HI_GV_Widget_Hide(VERTICAL_TRACKBAR);
	#ifdef CFG_LAO
	HI_GV_Widget_Hide(HOME_PAGE_groupbox_wifi);
	#endif
    HOME_PAGE_Refresh();

    static HI_BOOL bStart = HI_FALSE;

    if (!bStart)
    {
        bStart = HI_TRUE;
        HI_Time_Stamp(__func__, __LINE__, 0);

        HI_U32 u32Boottime = HI_TIME_GetBootTime();

        printf("[##@@ linux boottime(the first ui time): [%dms]\n", u32Boottime / 1000);
    }
    if(HI_TRUE == isHiddenBottomBar)
    {
        doHideBar();                    
    }
    s_bFlag = HI_TRUE;
    s_bIsShowing = HI_FALSE;

    HI_RECT rect = {0};
    HI_GV_Widget_GetRect(FEIYU_FAST_SET,&rect);
    HI_GV_Widget_Move(FEIYU_FAST_SET,480,rect.y);


    return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_OnHide(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGD("***\n");
    HI_GV_Timer_Stop(HOME_PAGE, PREVIEW_TIMER);
    HI_GV_Timer_Destory(HOME_PAGE, PREVIEW_TIMER);
    HI_GV_Timer_Stop(HOME_PAGE, HOME_PAGE_HIDE_TIMER);
    HI_GV_Timer_Destory(HOME_PAGE, HOME_PAGE_HIDE_TIMER);

    return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_OnTimer(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //MLOGE("++++++++++++++wParam=%d��PREVIEW_TIMER=%d��HOME_PAGE_HIDE_TIMER=%d\n",wParam,PREVIEW_TIMER,HOME_PAGE_HIDE_TIMER);
    
    if(PREVIEW_TIMER == wParam)
    {    
        HI_GV_Timer_Reset(HOME_PAGE, PREVIEW_TIMER);

        HOME_PAGE_Refresh();  
        
    }
    else if(HOME_PAGE_HIDE_TIMER == wParam)
    {
        if(!s_bFlag)
        {
        	MLOGD("++++++++++ HOME_PAGE_OnTimer doHideBar()\n");
            //doHideBar();
        }
        else
        {
            s_bFlag = HI_FALSE;
        }
        s_bScroll = HI_TRUE;
    }

    return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_OnEvent(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGD("wParam: %d, lParam = %x\n", wParam, lParam);
    if(UI_EVENT_JUMP_HOME == wParam)
    {
        UI_JumpToHome();
        return HIGV_PROC_GOON;
	}
    else if(UI_EVENT_BACK_HOME == wParam)
    {
        if( 1 == HI_GV_Widget_IsShow(FILELIST_PAGE))
        {
            HI_GV_Msg_SendAsync(FILELIST_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, HI_TRUE);
        }
        else if( 1 == HI_GV_Widget_IsShow(PLAYBACK_PAGE))
        {
            HI_GV_Msg_SendAsync(PLAYBACK_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_F3, HI_TRUE);
        }
        else
        {
            UI_JumpToHome();
        }
        return HIGV_PROC_GOON;
    }
    else if(UI_EVENT_HIDE_COUNTDOWN_LABLE == wParam)
    {
        HI_GV_Widget_Hide(HOME_PAGE_LABEL_COUNTDOWN);
        HI_GV_Widget_Paint(HOME_PAGE, 0);
        g_bHideCountDown = HI_TRUE;
        return HIGV_PROC_GOON;
    }
	else if(UI_EVENT_UPDATE_HOME_PAGE == wParam)
	{
		HOME_PAGE_Refresh();
		return HIGV_PROC_GOON;
	}
	
    UI_StateProcess((HI_PDT_WORKSTATE_S*)lParam);

    return HIGV_PROC_GOON;
}



HI_S32 HOME_PAGE_OnKeyUp(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_PDT_WORKMODE_S stWorkMode = {};

    MLOGD("KeyCode: %#x\n", wParam);

    s32Ret = HI_PDT_WorkMode_GetWorkMode( &stWorkMode );

    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetWorkMode failed\n");
        return HIGV_PROC_GOON;
    }

    if (PDT_WORKMODE_MULTIPHOTO == stWorkMode.enWorkMode
        && PDT_MULTIPHOTOMODE_CONTINUOUS == stWorkMode.enMultiPhotoMode)
    {
        MLOGD("HI_PDT_WorkMode_Stop\n");
        s32Ret = HI_PDT_WorkMode_Stop();

        if (HI_SUCCESS != s32Ret)
        {
            MLOGE("HI_PDT_WorkMode_Stop failed\n");
        }
    }

    return HIGV_PROC_GOON;
}

static HI_BOOL s_s32doingChangWorkMode = HI_FALSE;

static void * doChangeWorkMode(void* pVoid)
{
    s_s32doingChangWorkMode = HI_TRUE;
	(HI_VOID)prctl(PR_SET_NAME, (unsigned long)"doChangeWorkMode", 0, 0, 0);
    if(HI_TRUE == isHiddenBottomBar)
    {
	    doHideBar();
    }	
    FeiYuChangeWorkMode();
    s_s32doingChangWorkMode = HI_FALSE;

	return NULL;
}

static HI_VOID FeiyuModeKeychangeWorkMode()
{
	HI_S32 s32Ret = 0;
    
    if(HI_TRUE == s_s32doingChangWorkMode)
    {
        MLOGE("last thread is running.\n");
        return;
    }
	pthread_t threadid = 0;
    s32Ret = pthread_create(&threadid, NULL, (void *)doChangeWorkMode, NULL);
	if(0 != s32Ret)
    {
    	MLOGE("pthread_create changeWorkMode Failed, s32Ret=%#x\n", s32Ret);
    }else
    {
        pthread_detach(threadid);
    }

}
static void * doWorkModeStart(void* pVoid)
{
    pthread_detach(pthread_self());

    HI_S32 s32Ret = HI_SUCCESS;
    s32Ret = HI_PDT_WorkMode_Start();

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("WorkModeStart Error:%#x\n", s32Ret);
        UI_ProcessErrorCode(s32Ret);
    }    

    return NULL;
}

#ifdef CFG_LAO
#include "fy_user_param.h"
VPSS_CROP_INFO_WORKMODE g_stCropInfoALL;

VPSS_CROP_INFO_S g_stCropInfo[ZOOM_SIZE][ZOOM_MAX]={};
HI_VOID gku_zoom_control(HI_U32 option)
{

	HI_U32 s8ZoomValue = option;
	HI_U32 u8ZoomSizeCur = ZOOM_SIZE;
	HI_PDT_WORKMODE_S stWorkMode;
	HI_PDT_VIDEO_MODE_E enVideoMode ;
	HI_PDT_WorkMode_GetWorkMode(&stWorkMode);
	printf("HI_PDT_WorkMode_GetWorkMode:%d\n", stWorkMode.enWorkMode);
	
	if(stWorkMode.enWorkMode == PDT_WORKMODE_RECORD)
	{
					switch((int)stWorkMode.enRecMode)
					{
						case  PDT_RECMODE_NORMAL :
							enVideoMode = g_pstPDTCfg->stWorkModeParam.stRecModeCfg.stNormalAttr.stRecCommAttr.enVideoMode;
							break;
				    	case  PDT_RECMODE_LOOP :
							enVideoMode = g_pstPDTCfg->stWorkModeParam.stRecModeCfg.stLoopAttr.stRecCommAttr.enVideoMode;
							break;
				    	case  PDT_RECMODE_LAPSE :
							enVideoMode = g_pstPDTCfg->stWorkModeParam.stRecModeCfg.stLapseAttr.stRecCommAttr.enVideoMode;
							break;
				    	case  PDT_RECMODE_RECSNAP:
							enVideoMode = g_pstPDTCfg->stWorkModeParam.stRecModeCfg.stRecSnapAttr.stRecCommAttr.enVideoMode;
							break;
				    	case  PDT_RECMODE_SLOW:
							enVideoMode = g_pstPDTCfg->stWorkModeParam.stRecModeCfg.stSlowAttr.stRecCommAttr.enVideoMode;
							break;
					}
					
					
					switch(enVideoMode)
					{	
						case PDT_VIDEO_MODE_4K30_4_3_REC:
						case PDT_VIDEO_MODE_4K30_4_3_REC_SNAP:
							u8ZoomSizeCur = 4;
							break;
						case PDT_VIDEO_MODE_4K2K_30_REC:
						case PDT_VIDEO_MODE_4K2K_30_REC_SNAP:
							u8ZoomSizeCur = 3;
							break;
						case PDT_VIDEO_MODE_2K7_30_4_3_REC:
						case PDT_VIDEO_MODE_2K7_30_4_3_REC_SNAP:
							u8ZoomSizeCur = 5;
							break;
						case PDT_VIDEO_MODE_2_7K_30_REC:
						case PDT_VIDEO_MODE_2_7K_30_REC_SNAP:
							u8ZoomSizeCur = 2;
							break;
						case PDT_VIDEO_MODE_1440P_60_4_3_REC:
						case PDT_VIDEO_MODE_1440P_60_4_3_REC_SNAP:
						case PDT_VIDEO_MODE_1440P_30_4_3_REC:
						case PDT_VIDEO_MODE_1440P_30_4_3_REC_SNAP:
							u8ZoomSizeCur = 6;
							break;
						case PDT_VIDEO_MODE_1440P_30_REC:
						case PDT_VIDEO_MODE_1440P_30_REC_SNAP:
						case PDT_VIDEO_MODE_1440P_60_REC:
						case PDT_VIDEO_MODE_1440P_60_REC_SNAP:
							u8ZoomSizeCur = 1;
							break;
						case PDT_VIDEO_MODE_1080P_60_4_3_REC:
						case PDT_VIDEO_MODE_1080P_60_4_3_REC_SNAP:
				    	case PDT_VIDEO_MODE_1080P_30_4_3_REC:
						case PDT_VIDEO_MODE_1080P_30_4_3_REC_SNAP:
							u8ZoomSizeCur = 7;
							break;
						case PDT_VIDEO_MODE_1080P_30_REC:
						case PDT_VIDEO_MODE_1080P_30_REC_SNAP:
						case PDT_VIDEO_MODE_1080P_60_REC:
						case PDT_VIDEO_MODE_1080P_60_REC_SNAP:
						case PDT_VIDEO_MODE_1080P_120_REC:
						case PDT_VIDEO_MODE_720P_240_REC:
						case PDT_VIDEO_MODE_720P_120_REC:
							u8ZoomSizeCur = 0;
							break;
						default :
							u8ZoomSizeCur = ZOOM_SIZE;
							s8ZoomValue = 0;
							break;
					}
					if ((u8ZoomSizeCur > ZOOM_SIZE)||(s8ZoomValue > ZOOM_MAX))
						return ;
					g_stCropInfoALL.mode = 1;
					g_stCropInfoALL.CROP_INF_MAX = g_stCropInfo[u8ZoomSizeCur][s8ZoomValue];
				
		}else
		{
					if(stWorkMode.enWorkMode == PDT_WORKMODE_PHOTO)
					{
							switch((int)stWorkMode.enPhotoMode)
							{
								case  PDT_PHOTOMODE_SINGLE :
									enVideoMode = g_pstPDTCfg->stWorkModeParam.stPhotoModeCfg.stSingleAttr.stCommAttr.enVideoMode;
									break;
						    	case  PDT_PHOTOMODE_DELAY :
									enVideoMode = g_pstPDTCfg->stWorkModeParam.stPhotoModeCfg.stDelayAttr.stCommAttr.enVideoMode;
									break;
							}
					}else if(stWorkMode.enWorkMode == PDT_WORKMODE_MULTIPHOTO)
					{
							switch((int)stWorkMode.enMultiPhotoMode)
							{
						    	case  PDT_MULTIPHOTOMODE_BURST :
									enVideoMode = g_pstPDTCfg->stWorkModeParam.stMultiPhotoModeCfg.stBurstAttr.stCommAttr.enVideoMode;
									break;
						    	case  PDT_MULTIPHOTOMODE_LAPSE:
									enVideoMode = g_pstPDTCfg->stWorkModeParam.stMultiPhotoModeCfg.stLapseAttr.stCommAttr.enVideoMode;
									break;
						    	case  PDT_MULTIPHOTOMODE_CONTINUOUS:
									enVideoMode = g_pstPDTCfg->stWorkModeParam.stMultiPhotoModeCfg.stContinuousAttr.stCommAttr.enVideoMode;
									break;
							}
					}
						
					switch(enVideoMode)
					{	
						case PDT_VIDEO_MODE_5M_SNAP:
   						case PDT_VIDEO_MODE_5M_MULTISNAP:
							u8ZoomSizeCur = 10;
							break;
						case PDT_VIDEO_MODE_4K2K_SNAP:
						case PDT_VIDEO_MODE_4K2K_MULTISNAP:
							u8ZoomSizeCur = 8;
							break;
						case PDT_VIDEO_MODE_12M_SNAP:
						case PDT_VIDEO_MODE_12M_MULTISNAP:
							u8ZoomSizeCur = 12;
							break;
						default :
							u8ZoomSizeCur = ZOOM_SIZE;
							s8ZoomValue = 0;
							break;
					}


					if (((u8ZoomSizeCur+1) > ZOOM_SIZE)||((s8ZoomValue+1) > ZOOM_MAX))
						return ;

					g_stCropInfoALL.mode = 0;
					g_stCropInfoALL.CROP_INF_MAX = g_stCropInfo[u8ZoomSizeCur][s8ZoomValue];
					g_stCropInfoALL.CROP_INF_MIN = g_stCropInfo[u8ZoomSizeCur+1][s8ZoomValue+1];
		}
	
		printf("u8ZoomSize:%d---s8ZoomValue:%d---\n", u8ZoomSizeCur, s8ZoomValue);
		HI_PDT_Media_Zoom(g_stCropInfoALL);
		
		return ;
}
		
#endif

HI_S32 HOME_PAGE_OnKeyDown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_PDT_TimedTask_Reset(PDT_TIMEDTASK_TYPE_AUTOOFF);
    printf(">>>>>>>HIGV_KEY_F3==%dHIGV_KEY_F2==%dHIGV_KEY_F1==%d\n",HIGV_KEY_F3,HIGV_KEY_F2,HIGV_KEY_F1);
    printf(">>>>>>wParam===%d\n",wParam);
    if(g_bIsDvLocking)
    {
        HI_GV_Msg_SendAsync(FEIYU_UNLOCK_PAGE_CLICK, HIGV_MSG_EVENT, 0, 0);
        return HIGV_PROC_STOP;
    }
    g_isFromHTTPShouldReport = HI_TRUE;
    if(s_bIsShowing)
    {
        return HIGV_PROC_STOP;
    }
	if(HI_GV_Widget_IsShow(TIPS_WINDOW))
	{
		return HIGV_PROC_STOP;
	}
    HI_S32 s32Ret = HI_SUCCESS;
    HI_PDT_WORKMODE_S  stWorkMode = {};
    HI_UI_STORAGE_TYPE_E enStroageType = UI_STORAGE_TYPE_IMAGE;
    HI_UI_ALARM_TYPE_E   enCheckErrNo;

    MLOGD("KeyCode: %#x\n", wParam);

    s32Ret |= HI_PDT_WorkMode_GetWorkMode(&stWorkMode);

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("Get WorkState or WorkMode Failed:%#x\n", s32Ret);
        return HIGV_PROC_GOON;
    }

    if (HIGV_KEY_F3 == wParam)
    {
        if (PDT_WORKMODE_STOPPED == HI_PDT_WorkMode_GetState())
        {			
			UI_ShowFeiYuGlobalSettings();
        }
    }
    else if (HIGV_KEY_F2 == wParam)
    {
        if (PDT_WORKMODE_STOPPED == HI_PDT_WorkMode_GetState())
        {
            //UI_ShowMenuPage();
            FeiyuModeKeychangeWorkMode();
        }
        else if(PDT_WORKMODE_STARTED == HI_PDT_WorkMode_GetState())
        {
            HI_S32 workmode;
        	FeiYuCmdExecute(CMD_GET_CURRENT_WORK_MODE, 0, &workmode);
            if(VIDEO == workmode)
            {
                HI_BOOL bEnableDebug;
                HI_PDT_DUMP_GetState(&bEnableDebug);
                PDT_CHECK_EXPR_GOTO_ERROR_0(HI_SUCCESS == s32Ret);

                if (bEnableDebug)
                {
                    s32Ret = HI_PDT_DUMP_Snap_StartDump();
                }
                else
                {
                    s32Ret = HI_PDT_Photo_Single();
                }

                PDT_CHECK_EXPR_GOTO_ERROR_0(HI_SUCCESS == s32Ret);
            }
        }
    }
    else if (HIGV_KEY_F1 == wParam)
    {
        if (PDT_WORKMODE_STARTED == HI_PDT_WorkMode_GetState())
        {
            if (PDT_WORKMODE_RECORD == stWorkMode.enWorkMode)
            {
                isRecording = HI_FALSE;
				bRecordStart = HI_FALSE;
                //when stop recording video, show "Writing SD card".
                //This is UI Thread, if we call HI_PDT_WorkMode_Stop() here,
                //showing "Writing SD card" will be delayed.
                s32Ret = HI_GV_Widget_SetTextByID(WRITINGSD_WINDOW_label, ID_STR_WRITINGSD);
                s32Ret |= HI_GV_Widget_Show(WRITINGSD_WINDOW);
                s32Ret |= HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_P1, 0);

                if (s32Ret)
                {
                    MLOGE("HIGV Error %#x\n", s32Ret);
                }
            }
            else if(PDT_WORKMODE_MULTIPHOTO != stWorkMode.enWorkMode || PDT_MULTIPHOTOMODE_BURST != stWorkMode.enMultiPhotoMode)
            {
                s32Ret = HI_PDT_WorkMode_Stop();
                if(HI_ERR_PHOTO_BUSY == s32Ret)
                {
                    MLOGD("Photo is busy. Please wait for stop.\n");
                }
                else if (HI_SUCCESS != s32Ret)
                {
                    MLOGE("HI_PDT_WorkMode_Stop Error:%#x\n", s32Ret);
                }
                else
                {
                    MLOGD("HI_PDT_WorkMode_Stop SUCCESS.\n");
                }
            }
        }
        else
        {
            if (PDT_WORKMODE_RECORD == stWorkMode.enWorkMode)
            {
                isRecording = HI_TRUE;
                enStroageType = UI_STORAGE_TYPE_VIDEO;
				bRecordStart = HI_TRUE;
            }
            else
            {
                enStroageType = UI_STORAGE_TYPE_IMAGE;
            }
            if((HI_FALSE == isHiddenBottomBar)&&(PDT_WORKMODE_RECORD == stWorkMode.enWorkMode))
            {
            	s_bRecHideBar = HI_TRUE;
                doHideBar();                    
            }

            s32Ret = UI_PreCheckStorage(enStroageType, &enCheckErrNo);

            if (HI_SUCCESS != s32Ret)
            {
                UI_AlarmDisplay(enCheckErrNo);
                return HIGV_PROC_GOON;
            }
            
            s32Ret = HI_PDT_WorkMode_Start();

            if (HI_SUCCESS != s32Ret)
            {
                MLOGE("WorkModeStart Error:%#x\n", s32Ret);
                UI_ProcessErrorCode(s32Ret);
            } 	
        }

        HOME_PAGE_Refresh();

    }
    else if (HIGV_KEY_P1 == wParam)
    {
        s32Ret = HI_PDT_WorkMode_Stop();
        MLOGD("WorkMode_Stop out\n");

        if (HI_SUCCESS != s32Ret)
        {
            MLOGE("WorkModeStop Error:%#x\n", s32Ret);
        }

        s32Ret = HI_GV_Widget_Hide(WRITINGSD_WINDOW);

        if (s32Ret)
        {
            MLOGE("HIGV Error %#x\n", s32Ret);
        }
    }
    else
    {
        MLOGE("KEY CODE %#x not processed\n", wParam);
    }

    return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_OnGestureFling(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{

#if 0
    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;

    MLOGD("\n");

    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetState failed\n");
        return HIGV_PROC_STOP;
    }


    if (PDT_WORKMODE_STARTED == HI_PDT_WorkMode_GetState())
    {
        MLOGD("recording or snapping, ignore gesture fling\n");
        return HIGV_PROC_STOP;
    }

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
    MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);

    if (abs(startX - endX) > abs(startY - endY))// left or right
    {
        if (startX > endX)// to left
        {
            //UI_ShowMenuPage();
			//FeiYuChangeWorkMode();
            UI_ShowFeiyuFastSet();
        }
        else // to right
        {
            HI_U32 fileCount = 0;
            HI_PDT_FileMng_GetFileCount(&fileCount);
            if(0 < fileCount)
            {
                UI_ShowFileListPage();
            }
            else
            {
                //show no medis files!
                HI_GV_Msg_SendAsync(ALARM_WINDOW, HIGV_MSG_EVENT, 0, ALARM_SD_NOMEDIAFILES);
            }
        }
    }
    else //up or down
    {
        if (startY > endY)//to up
        {
            // UI_ShowSubModeSet();
        }
        else  //down
        {
            MLOGD("down fling\n");
			UI_ShowFeiYuGeneralFunction();
        }
    }
#endif
    return HIGV_PROC_GOON;
}

#ifdef WENBIN
HI_U8 au8Weight1[15][17] = 
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,2,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};  

HI_S32 userMetry(HI_S32 x, HI_S32 y)
{

   HI_S32 s32WeightCenter = 0;
   HI_S32 num = 0;
   HI_S32 i = 0, j = 0, Valx = 0, Valy = 0;
   Valx = 480 - x;
   Valy = 320 - y;
   //Valx = x;
   //Valy = y;

   i = Valx/(480 / 17);
   j = Valy/(320 / 15);
   if(i < 2)
    i = 0;
   else if(i > 14)
    i = 14;

   if(j < 1)
    j = 1;
   else if(j > 13)
    j = 13;

   MLOGE("userMetry,x=%d,y=%d,i=%d,j=%d\n", x,y,i,j);   
   s32WeightCenter = j*17 + i;
   
/*
  1 1 1
1 1 2 1 1
  1 1 1
*/

   for(j = 0; j < 15; j ++)
   {
        for(i = 0; i < 17; i ++)
        {
            num = j*17 + i;
            if(s32WeightCenter == num)
            {
                au8Weight1[j][i] = 2;                
            }
            else if((s32WeightCenter -1 ) == num ||
                    (s32WeightCenter -2 ) == num ||
                    (s32WeightCenter +1 ) == num ||
                    (s32WeightCenter +2 ) == num ||
                    (s32WeightCenter +16 ) == num ||
                    (s32WeightCenter +17 ) == num ||
                    (s32WeightCenter +18 ) == num ||
                    (s32WeightCenter -16 ) == num ||
                    (s32WeightCenter -17 ) == num ||
                    (s32WeightCenter -18 ) == num )
            {
                au8Weight1[j][i] = 1;
            }
            else
            {
                au8Weight1[j][i] = 0;
            }

        }
   }

   CUSTOM_MsgPost(CUSTOM_MSG_CMD_SetMetry,(HI_VOID*)au8Weight1,sizeof(au8Weight1),NULL,0);
}
#endif
HI_S32 HOME_PAGE_OnGestureTap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGD(">>>>>>>>>>>>>>HOME_PAGE_OnGestureTap\n");
  	
    if(g_bIsDvLocking)
    {
        HI_GV_Msg_SendAsync(FEIYU_UNLOCK_PAGE_CLICK, HIGV_MSG_EVENT, 0, 0);
        return HIGV_PROC_STOP;
    }
    if(s_bIsShowing)
    {
        return HIGV_PROC_STOP;
    }
	if(HI_GV_Widget_IsShow(TIPS_WINDOW))
	{
		return HIGV_PROC_STOP;
	}
	if(PDT_WORKMODE_STARTED == HI_PDT_WorkMode_GetState())
		return HIGV_PROC_GOON;
    HIGV_GESTURE_EVENT_S gestureEvent;
    HI_S32 x, y;
    memset(&gestureEvent, 0x0, sizeof(gestureEvent));
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    x = gestureEvent.gesture.tap.pointer.x;
    y = gestureEvent.gesture.tap.pointer.y;
    //userMetry(x, y);

	if((HI_FALSE == isHiddenBottomBar) &&(HI_FALSE == s_bIsTopBottomAnim))
    {
        if((x >= 0)&&(x <= 70)&&(y >= 260)&&(y <= 320))
		{
			HI_GV_Msg_SendAsync(HOME_PAGE_BUTTON_FILELIST, HIGV_MSG_GESTURE_TAP, 0, 0);
            return HIGV_PROC_STOP;
		}
		else if((x >= 420)&&(x <= 480)&&(y >= 260)&&(y <= 320))
		{
			HI_GV_Msg_SendAsync(HOME_PAGE_GROUPBOX2_BUTTON_GLOBAL_SETTINGS, HIGV_MSG_GESTURE_TAP, 0, 0);
            return HIGV_PROC_STOP;
		}
		else
		{
			doHideBar();
		}                       
    }
	else if((HI_TRUE == isHiddenBottomBar) &&(HI_FALSE == s_bIsTopBottomAnim))
	{
		doHideBar();
	}

    return HIGV_PROC_GOON;//HIGV_PROC_STOP
}

HI_S32 HOME_PAGE_OnGestureLongTap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGD("HOME_PAGE_OnGestureLongTap\n");
	if(g_bIsDvLocking)
    {
        HI_GV_Msg_SendAsync(FEIYU_UNLOCK_PAGE_CLICK, HIGV_MSG_EVENT, 0, 0);
        return HIGV_PROC_STOP;
    }
	if(HI_GV_Widget_IsShow(TIPS_WINDOW))
	{
        return HIGV_PROC_STOP;
    }
	
    HI_S32 s32Ret = 0;
    
    s32Ret = HI_GV_PARSER_LoadViewById(HOME_PAGE_MISC);
    s32Ret |= HI_GV_Widget_Show(HOME_PAGE_MISC);
    if (PDT_WORKMODE_STOPPED == HI_PDT_WorkMode_GetState())
    {
        s32Ret |= HI_GV_Widget_Hide(HOME_PAGE);        
    }
    s32Ret |= HI_GV_Widget_Active(HOME_PAGE_MISC);
    s32Ret |= HI_GV_Widget_Paint(HOME_PAGE_MISC, 0);        

    if(0 != s32Ret)
    {
        MLOGE("ERR\n");
    }
    
    return HIGV_PROC_GOON;
}

static HI_VOID doHideBar()
{
    HI_S32 s32Ret = HI_SUCCESS;
    s_bIsTopBottomAnim = HI_TRUE;
    if(HI_FALSE == isHiddenBottomBar)
    {
        //s32Ret |= HI_GV_Widget_Show(FEIYU_ZOOM_PAGE);
        //s32Ret |= HI_GV_Widget_MoveToTop(FEIYU_ZOOM_PAGE);
        //s32Ret |= HI_GV_Widget_Paint(HOME_PAGE, 0);   
        isHiddenBottomBar = HI_TRUE;
        moveY_TopGroupbox(0,-48);
        moveY_BottomGroupbox(260,320);
        extern HI_PDT_WORKMODE_S g_stCurrentWorkMode;
        if(PDT_WORKMODE_RECORD == g_stCurrentWorkMode.enWorkMode)
        {
        	//HI_GV_Widget_Show(VERTICAL_TRACKBAR);
            //HI_GV_Widget_Paint(VERTICAL_TRACKBAR, 0);            
        }
        // addMoveYAnimation(HOME_PAGE_GROUPBOX1,0,-60,200,HIGV_TWEEN_TRANSITION_LINEAR);
        // addMoveYAnimation(HOME_PAGE_GROUPBOX2,260,320,200,HIGV_TWEEN_TRANSITION_LINEAR);
    }else
    {
        //s32Ret |= HI_GV_Widget_Hide(FEIYU_ZOOM_PAGE);
        //s32Ret |= HI_GV_Widget_Paint(HOME_PAGE, 0);  
        isHiddenBottomBar = HI_FALSE;
        moveY_TopGroupbox(-48,0);
        moveY_BottomGroupbox(320,260);
        extern HI_PDT_WORKMODE_S g_stCurrentWorkMode;
        if(PDT_WORKMODE_RECORD == g_stCurrentWorkMode.enWorkMode)
        {        
        	//HI_GV_Widget_Hide(VERTICAL_TRACKBAR);        
            //HI_GV_Widget_Paint(VERTICAL_TRACKBAR, 0);        
        }    
        // addMoveYAnimation(HOME_PAGE_GROUPBOX1,-60,0,200,HIGV_TWEEN_TRANSITION_LINEAR);
        // addMoveYAnimation(HOME_PAGE_GROUPBOX2,320,260,200,HIGV_TWEEN_TRANSITION_LINEAR);
    }
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("WorkModeStop Error:%#x\n", s32Ret);
    }    
}
HI_S32 HOME_PAGE_COVER_LABEL_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    if(s_bIsShowing)
    {
        return HIGV_PROC_GOON;
    }
    
    HI_GV_Timer_Reset(HOME_PAGE, HOME_PAGE_HIDE_TIMER);
    
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("WorkModeStop Error:%#x\n", s32Ret);
    }
    return HIGV_PROC_GOON;
}




HI_S32 HOME_PAGE_OnGestureScroll(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;

    MLOGD("\n");
    if(!s_bScroll)
        return HIGV_PROC_GOON;
    
    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetState failed\n");
        return HIGV_PROC_STOP;
    }
    if(g_bIsDvLocking)
    {
        HI_GV_Msg_SendAsync(FEIYU_UNLOCK_PAGE_CLICK, HIGV_MSG_EVENT, 0, 0);
        return HIGV_PROC_STOP;
    }
    if(s_bIsShowing)
    {
        return HIGV_PROC_STOP;
    }

	if(HI_GV_Widget_IsShow(TIPS_WINDOW))
	{
		return HIGV_PROC_STOP;
	}
    if (PDT_WORKMODE_STARTED == HI_PDT_WorkMode_GetState())
    {
        MLOGD("recording or snapping, ignore gesture fling\n");
        return HIGV_PROC_STOP;
    }

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
	//dy = endY > g_startY ? (endY - g_startY) : (g_startY - endY);
	//dx = endX > g_startX ? (endX - g_startX) : (g_startX - endX);
    MLOGD(">>>>>>>>>dx:%d, dy:%d>>>>>>>>>> endY:%d\n", endX - startX, endY - startY, endY);
    
	if (abs(startX - endX) > abs(startY - endY))// left or right
	{
	    if ((startX > endX)&&(g_startX >= 420)&&(g_startX - endX > 40))// to left
	    {
	      	if(g_pageChangeEnd == HI_TRUE)
	      	{
        		//UI_ShowMenuPage();
				//FeiYuChangeWorkMode();
				g_pageChangeEnd = HI_FALSE;
		        UI_ShowFeiyuFastSet();
	      	}
	    }
	    else if((startX < endX)&&(g_startX <= 60)&&(g_startX >= 0)&&(endX - g_startX > 40))// to right
	    {
	        HI_U32 fileCount = 0;
	        HI_PDT_FileMng_GetFileCount(&fileCount);
	        if((0 < fileCount)&&(g_pageChangeEnd == HI_TRUE))
	        {
	            s_bShowFilelistAnim = HI_TRUE;
				g_pageChangeEnd = HI_FALSE;
	            UI_ShowFileListPage();
	        }
	        else
	        {
	            //show no medis files!
	            HI_GV_Msg_SendAsync(ALARM_WINDOW, HIGV_MSG_EVENT, 0, ALARM_SD_NOMEDIAFILES);
	        }
	    }
	}
	else //up or down
	{
	    if ((startY > endY)&&(g_startY >= 260)&&(g_startY - endY > 40))//to up
	    {
	        //UI_ShowSubModeSet();
	        if(g_pageChangeEnd == HI_TRUE)
	        {
	           	g_pageChangeEnd = HI_FALSE;
	           	UI_ShowFeiYuModeSet();
	        }
        }
	    else if((startY < endY)&&(g_startY <= 50)&&(g_startY >= 0)&&(endY - g_startY > 40)) //down
	    {
	        if(g_pageChangeEnd == HI_TRUE)
	       	{
		        MLOGD("down fling\n");
				g_pageChangeEnd = HI_FALSE;
		        s_bIsShowing = HI_TRUE;
				UI_ShowFeiYuGeneralFunction();
	       	}
	    }
	}

	if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,ret=%d\n",s32Ret);
    }

    return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_OnTouchAction(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	#ifdef CFG_HJD
    HI_S32 point_x, point_y;
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;

    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));

    point_x = touchEvent.last.x;
    point_y = touchEvent.last.y;

    type = touchEvent.last.type;


    switch (type)
    {
        case HIGV_TOUCH_START:
        {
			MLOGD(">>>>>>>>>> HIGV_TOUCH_START\n");
			g_startY = point_y;
			g_startX = point_x;
            break;
        }

        case HIGV_TOUCH_END:
        {
			MLOGD(">>>>>>>>>> HIGV_TOUCH_END\n");
            break;
        }

        default:
        	break;
    }
	#endif
	
    return HIGV_PROC_GOON;
}

/**
 * @brief  refresh UI, or change wifi state.
 *  event_proc module will send  HIGV_KEY_F1  to  HOME_PAGE_BUTTON_WIFI.
 */
HI_S32 HOME_PAGE_BUTTON_WIFI_OnKeyDown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	MLOGD("HOME_PAGE_BUTTON_WIFI_OnKeyDown\n");
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bAPState = HI_FALSE;
    pthread_t wifiThread = 0;

    if (HIGV_KEY_F2 == wParam)
    {
        bAPState =  HI_WIFIMNG_GetState();

        if (bAPState)
        {
            s32Ret |= HI_GV_Widget_SetTextByID(TIPS_WINDOW_label, ID_STR_WIFI_OFF);
        }
        else
        {
            s32Ret |= HI_GV_Widget_SetTextByID(TIPS_WINDOW_label, ID_STR_WIFI_ON);
        }

        if (HI_TRUE != lParam)
        {
            s32Ret |= HI_GV_Widget_Show(TIPS_WINDOW);
        }

        s32Ret = pthread_create(&wifiThread, 0, Wifi_TurnOnOff, NULL);

        if (HI_SUCCESS == s32Ret)
        {
            pthread_detach(wifiThread);
        }

    }
    else if (HIGV_KEY_F1 == wParam)
    {
        UI_SetWifiControl(HOME_PAGE_BUTTON_WIFI);
    }

    return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_GROUPBOX1_BUTTON_PHOTO_ABLUM_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    //check whether SD Card has files!
    HI_U32 fileCount = 0;
    HI_PDT_FileMng_GetFileCount(&fileCount);
    if(0 < fileCount)
    {
        UI_ShowFileListPage();
    }
    else
    {
        //show no medis files!
        HI_GV_Msg_SendAsync(ALARM_WINDOW, HIGV_MSG_EVENT, 0, ALARM_SD_NOMEDIAFILES);
    }
    return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_BUTTON_MODE_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	UI_ShowFeiYuModeSet();

    return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_GLOBAL_SETTINGS_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	UI_ShowFeiYuGlobalSettings();
	
    return HIGV_PROC_GOON;
}

/***********************anmaition handle!********************************/
/**TopGroupBox*/
static void animMoveY_TopGroupBoxUpdate_handle(HIGV_LISTENER_S* listener, void* data)
{
    HI_RECT rect = {0};
    HI_GV_Widget_GetRect(HOME_PAGE_GROUPBOX1,&rect);
    HI_GV_Widget_Move(HOME_PAGE_GROUPBOX1,rect.x,HI_GV_TweenAnimGetTweenValue(animation_TopGroupbox_YHandle, 0));
    //刷新window
    //HI_GV_Widget_Update(HOME_PAGE_GROUPBOX1, HI_NULL);
}
static void animMoveY_TopGroupBoxStart_handle(HIGV_LISTENER_S* listener, void* data)
{
    MLOGE(">>===animY_start_handle\n");
}

static void animMoveY_TopGroupBoxEnd_handle(HIGV_LISTENER_S* listener, void* data)
{
    MLOGE(">>===animY_end_handle-----%d\n",*(int*)data);
    s_bIsTopBottomAnim = HI_FALSE;
}

/**BottomGroupBox*/
static void animMoveY_BottomGroupBoxUpdate_handle(HIGV_LISTENER_S* listener, void* data)
{
    HI_RECT rect = {0};
    HI_GV_Widget_GetRect(HOME_PAGE_GROUPBOX2,&rect);
    HI_GV_Widget_Move(HOME_PAGE_GROUPBOX2,rect.x,HI_GV_TweenAnimGetTweenValue(animation_BottomGroupbox_YHandle, 0));
    //刷新window
    //HI_GV_Widget_Update(HOME_PAGE_GROUPBOX2, HI_NULL);
}
static void animMoveY_BottomGroupBoxStart_handle(HIGV_LISTENER_S* listener, void* data)
{
    MLOGE(">>===animY_start_handle\n");
}
static void animMoveY_BottomGroupBoxEnd_handle(HIGV_LISTENER_S* listener, void* data)
{
    MLOGE(">>===animY_end_handle-----%d\n",*(int*)data);
    s_bIsTopBottomAnim = HI_FALSE;
}

/**GeneralFunction*/
static void animMoveY_GeneralFun_Update_handle(HIGV_LISTENER_S* listener, void* data)
{
    HI_RECT rect = {0};
    HI_GV_Widget_GetRect(FEIYU_GENERAL_FUNCTION,&rect);
    HI_GV_Widget_Move(FEIYU_GENERAL_FUNCTION,rect.x,HI_GV_TweenAnimGetTweenValue(animation_BottomGroupbox_YHandle, 0));
    //刷新window
    //HI_GV_Widget_Update(FEIYU_GENERAL_FUNCTION, HI_NULL);
}

static void animMoveY_GeneralFun_Start_handle(HIGV_LISTENER_S* listener, void* data)
{
    MLOGE(">>===animY_start_handle\n");
}

static void animMoveY_GeneralFun_End_handle(HIGV_LISTENER_S* listener, void* data)
{
    MLOGE(">>===animY_end_handle-----%d\n",*(int*)data);
}

void moveY_TopGroupbox(HIGV_CORD fromValue,HIGV_CORD toValue)
{
    animation_TopGroupbox_YHandle = HI_GV_TweenAnimCreate();
    HI_GV_TweenAnimSetDuration(animation_TopGroupbox_YHandle,400);

    //设置动画属性值得范围
    HI_GV_TweenAnimAddTween(animation_TopGroupbox_YHandle, HIGV_TWEEN_TRANSITION_LINEAR, HIGV_TWEEN_EASE_OUT, fromValue, toValue);

    //添加动画回调函数
    HI_GV_TweenAnimSetExecListener(animation_TopGroupbox_YHandle, animMoveY_TopGroupBoxUpdate_handle);
        
    //添加动画回调函数
    HI_GV_TweenAnimSetStartedListener(animation_TopGroupbox_YHandle, animMoveY_TopGroupBoxStart_handle);

    //添加动画回调函数
    HI_GV_TweenAnimSetFinishedListener(animation_TopGroupbox_YHandle, animMoveY_TopGroupBoxEnd_handle);

    //开始动�?    
    HI_GV_TweenAnimStart(animation_TopGroupbox_YHandle);
}

void moveY_BottomGroupbox(HIGV_CORD fromValue,HIGV_CORD toValue)
{
    animation_BottomGroupbox_YHandle = HI_GV_TweenAnimCreate();
    HI_GV_TweenAnimSetDuration(animation_BottomGroupbox_YHandle,400);

    //设置动画属性值得范围
    HI_GV_TweenAnimAddTween(animation_BottomGroupbox_YHandle,HIGV_TWEEN_TRANSITION_LINEAR,HIGV_TWEEN_EASE_OUT,fromValue,toValue);

    //添加动画回调函数
    HI_GV_TweenAnimSetExecListener(animation_BottomGroupbox_YHandle, animMoveY_BottomGroupBoxUpdate_handle);
        
    //添加动画回调函数
    HI_GV_TweenAnimSetStartedListener(animation_BottomGroupbox_YHandle, animMoveY_BottomGroupBoxStart_handle);

    //添加动画回调函数
    HI_GV_TweenAnimSetFinishedListener(animation_BottomGroupbox_YHandle, animMoveY_BottomGroupBoxEnd_handle);
        
    //开始动�?    
    HI_GV_TweenAnimStart(animation_BottomGroupbox_YHandle);
}

void moveY_GeneralFunction(HIGV_CORD fromValue,HIGV_CORD toValue,HI_U32 animTime)
{
    return;
    animation_GeneralFunction_YHandle = HI_GV_TweenAnimCreate();
    HI_GV_TweenAnimSetDuration(animation_GeneralFunction_YHandle,animTime);

    //设置动画属性值得范围
    HI_GV_TweenAnimAddTween(animation_GeneralFunction_YHandle,HIGV_TWEEN_TRANSITION_LINEAR,HIGV_TWEEN_EASE_OUT,fromValue,toValue);

    //添加动画回调函数
    HI_GV_TweenAnimSetExecListener(animation_GeneralFunction_YHandle, animMoveY_GeneralFun_Update_handle);
    
    //添加动画回调函数
    HI_GV_TweenAnimSetStartedListener(animation_GeneralFunction_YHandle, animMoveY_GeneralFun_Start_handle);

    //添加动画回调函数
    HI_GV_TweenAnimSetFinishedListener(animation_GeneralFunction_YHandle, animMoveY_GeneralFun_End_handle);
        
    //开始动�?    
    HI_GV_TweenAnimStart(animation_GeneralFunction_YHandle);
}

/***********************anmaition handle!********************************/

HI_S32 HOME_PAGE_BUTTON_FILELIST_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{

    HI_U32 fileCount = 0;
    HI_PDT_FileMng_GetFileCount(&fileCount);
    if(0 < fileCount)
    {
        s_bShowFilelistAnim = HI_FALSE;
        UI_ShowFileListPage();
    }
    else
    {
        //show no medis files!
        HI_GV_Msg_SendAsync(ALARM_WINDOW, HIGV_MSG_EVENT, 0, ALARM_SD_NOMEDIAFILES);
    }    
    return HIGV_PROC_GOON;    
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

