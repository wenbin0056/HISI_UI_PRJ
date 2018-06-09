


/**
*
* @file      feiyu_general_function.c
* @brief     general_function ui
* @author    wenbin<wenbin@feiyu-tech.com>
* @date      2017.09.16
*/

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
#include "feiyu_dv_cmd_proc.h"
#include "feiyu_UICommon_FunTool.h"
#include "fy_user_param.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */
//-----------------------------------------------------------//
#define UI_HIGHLIGHT_TIME       200*1000

 typedef struct GeneralFunctionState_S{
  	HI_BOOL ShutDownState;
 	HI_S32 wifiState;
	HI_BOOL LockState;
	HI_S32 VoiceState;
}GeneralFunctionState;

static HI_BOOL bongesturetap = HI_FALSE;
static HI_S32 s_bCurrentPic = 0;
static HI_BOOL s_bWifiStart = HI_FALSE;
static HI_BOOL isHideGeneralFunction = HI_TRUE;
static HI_BOOL s_bWifiShow = HI_FALSE;
extern HI_S32 s_s32LanguagrType;
extern HI_BOOL g_pageChangeEnd;
extern USER_PARA_S g_stUserPara;
extern USER_PARA_S* g_pstUserPara;

static GeneralFunctionState s_GeneralFunctionState;
static void refreshButtonState();
static void alerViewCallbackProc(HI_HANDLE hWidget);

static HI_HANDLE animation_RectGroupbox_YHandle = 0;
static HI_HANDLE animation_ArcGroupbox_YHandle = 0;
static void animMoveY_ArcGroupBoxUpdate_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_ArcGroupBoxStart_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_ArcGroupBoxEnd_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_RectGroupBoxUpdate_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_RectGroupBoxStart_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_RectGroupBoxEnd_handle(HIGV_LISTENER_S* listener, void* data);
static void moveY_ArcGroupbox(HIGV_CORD fromValue,HIGV_CORD toValue);
static void moveY_RectGroupbox(HIGV_CORD fromValue,HIGV_CORD toValue);
static HI_VOID doHideGeneralFunction();

//-----------------------------------------------------------//
static HI_VOID UI_ShowHomePage()
{
    HI_S32 s32Ret = HI_SUCCESS;
    s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
	s32Ret |= HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_UPDATE_HOME_PAGE, 0);
    s32Ret |= HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION);
    
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
}

//-----------------------------------------------------------//
HI_S32 FEIYU_GENERAL_FUNCTION_BUTTON_BACK_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	UI_ShowHomePage();
	
    return HIGV_PROC_GOON;
}

static HI_S32 s_lock_flag = 0;

static HI_VOID doShowLock(void *arg)
{
    pthread_detach(pthread_self());
    //HI_GV_Widget_SetSkin(FEIYU_MODE_SET_BUTTON_EXIT1, HIGV_SKIN_HIGHLIGHT, global_settings_back_active);
    if(s_GeneralFunctionState.LockState == HI_TRUE)
    { 
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_LOCK_LABEL, HIGV_SKIN_NORMAL, background_skin_gray);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_LOCK_LABEL, HIGV_SKIN_ACITVE, background_skin_gray); 
    }
    else
    {       
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_LOCK_LABEL, HIGV_SKIN_NORMAL, background_skin);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_LOCK_LABEL, HIGV_SKIN_ACITVE, background_skin);
	}

    HI_GV_Widget_Highlight(FEIYU_GENERAL_FUNCTION_BUTTON_LOCK, HI_TRUE);

    HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);
    usleep(UI_HIGHLIGHT_TIME);
    HI_GV_Widget_Highlight(FEIYU_GENERAL_FUNCTION_BUTTON_LOCK, HI_FALSE);        
    
    HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);

    s_lock_flag = 1;

}
static HI_VOID doLock(void *arg)
{
    pthread_detach(pthread_self());
    while(s_lock_flag == 0)
    {
        usleep(1000);
    }
    s_lock_flag = 0;
    
    printf("FEIYU_GENERAL_FUNCTION_BUTTON_LOCK_ongesturetap\n");
    // show lock view
    HI_S32 s32Ret = HI_SUCCESS;	
	g_pageChangeEnd = HI_TRUE;
	s32Ret = HI_GV_PARSER_LoadViewById(FEIYU_UNLOCK_PAGE_CLICK);	
	s32Ret |= HI_GV_Widget_Show(FEIYU_UNLOCK_PAGE_CLICK);
	s32Ret |= HI_GV_Widget_Active(FEIYU_UNLOCK_PAGE_CLICK);
    s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION);
	if (HI_SUCCESS != s32Ret)
	{
		MLOGE("HIGV Error: %#x\n", s32Ret);
	}
    
	FeiYuCmdExecute(CMD_LOCK_DV, 0, NULL);
	s_GeneralFunctionState.LockState = HI_TRUE;    
}

HI_S32 FEIYU_GENERAL_FUNCTION_BUTTON_LOCK_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = 0;
    pthread_t threadid = 0;
	if(bongesturetap == HI_FALSE)
	{
		bongesturetap = HI_TRUE;
	    s32Ret = pthread_create(&threadid, NULL, (void *)doShowLock, NULL);
	    if(0 != s32Ret)
	    {
	        MLOGE("pthread_create changeWorkMode Failed, s32Ret=%#x\n", s32Ret);
	    }          
	    s32Ret = pthread_create(&threadid, NULL, (void *)doLock, &hWidget);
	    if(0 != s32Ret)
	    {
	        MLOGE("pthread_create changeWorkMode Failed, s32Ret=%#x\n", s32Ret);
	    }  
	}

    return HIGV_PROC_GOON;
}

static HI_S32 s_shutdown_flag = 0;

static HI_VOID doShowShutDown(void *arg)
{
    pthread_detach(pthread_self());
    HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_SHUT_DOWN, HIGV_SKIN_HIGHLIGHT, FAST_OPTION_Shutdown_select);

    HI_GV_Widget_Highlight(FEIYU_GENERAL_FUNCTION_BUTTON_SHUT_DOWN, HI_TRUE);

    HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);
    usleep(UI_HIGHLIGHT_TIME);
    HI_GV_Widget_Highlight(FEIYU_GENERAL_FUNCTION_BUTTON_SHUT_DOWN, HI_FALSE);        
    
    HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);

    s_shutdown_flag = 1;

}
static HI_VOID doShutDown(void *arg)
{
    pthread_detach(pthread_self());
    while(s_shutdown_flag == 0)
    {
        usleep(1000);
    }
    s_shutdown_flag = 0;
	g_pageChangeEnd = HI_TRUE;
    FeiYuCmdExecute(CMD_SHUT_DOWN, 0, NULL);
    //showAlertViewwithText(ID_STR_GENERAL_SHUTDOWN_INFO,alerViewCallbackProc);
}

HI_S32 FEIYU_GENERAL_FUNCTION_BUTTON_SHUT_DOWN_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = 0;
    pthread_t threadid = 0;
	if(s_bWifiStart == HI_FALSE)
	{
	    s32Ret = pthread_create(&threadid, NULL, (void *)doShowShutDown, NULL);
	    if(0 != s32Ret)
	    {
	        MLOGE("pthread_create changeWorkMode Failed, s32Ret=%#x\n", s32Ret);
	    }          
	    s32Ret = pthread_create(&threadid, NULL, (void *)doShutDown, &hWidget);
	    if(0 != s32Ret)
	    {
	        MLOGE("pthread_create changeWorkMode Failed, s32Ret=%#x\n", s32Ret);
	    }  
	}
	else
	{
		if(HI_FALSE == HI_GV_Widget_IsShow(TIPS_WINDOW))
		{
			s_bWifiStart = HI_FALSE;
			UI_ShowHomePage();
		}
	}
    
    return HIGV_PROC_GOON;
}

static HI_S32 s_voice_flag = 0;

static HI_VOID doShowVoice(void *arg)
{
    pthread_detach(pthread_self());
    //HI_S32 s32Ret = HI_SUCCESS;

    while(s_voice_flag == 0)
    {
        usleep(1000);
    }
    s_voice_flag = 0;

    //HI_GV_Widget_SetSkin(FEIYU_MODE_SET_BUTTON_EXIT1, HIGV_SKIN_HIGHLIGHT, global_settings_back_active);
    HI_S32 state = 0;
    FeiYuCmdExecute(CMD_GET_PROMPT_VOICE_STATE, 0, &state);
    if(PROMPT_VOICE_OFF != state)
    {
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_VOICE_LABEL, HIGV_SKIN_NORMAL, background_skin);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_VOICE_LABEL, HIGV_SKIN_ACITVE, background_skin);
        HI_GV_Widget_Highlight(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HI_TRUE);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HIGV_SKIN_NORMAL, FAST_OPTION_Voice_select);        
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HIGV_SKIN_ACITVE, FAST_OPTION_Voice_select);
        HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);
    }
    else
    {
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_VOICE_LABEL, HIGV_SKIN_NORMAL, background_skin_gray);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_VOICE_LABEL, HIGV_SKIN_ACITVE, background_skin_gray);
        HI_GV_Widget_Highlight(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HI_FALSE); 
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HIGV_SKIN_NORMAL, FAST_OPTION_Voice_noselect);        
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HIGV_SKIN_ACITVE, FAST_OPTION_Voice_noselect);
        HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);        
    }

	g_pageChangeEnd = HI_TRUE;
    usleep(UI_HIGHLIGHT_TIME);
    UI_ShowHomePage();
}
static HI_VOID doVoice(void *arg)
{
    pthread_detach(pthread_self());
	if(PROMPT_VOICE_OFF != s_GeneralFunctionState.VoiceState)
	{
        HI_S32 s32Ret = HI_SUCCESS;
		s32Ret =  FeiYuCmdExecute(CMD_SET_PROMPT_VOICE_STATE, PROMPT_VOICE_OFF, NULL);
        //show hud when setting failed!
        if(HI_SUCCESS != s32Ret)
        {
            showHudWithTextAndTime(ID_STR_GENERAL_VOICE_ERROR,1.5);
        }else
        {
            //back to home when setting succeed
            s_GeneralFunctionState.VoiceState = PROMPT_VOICE_OFF;
        }		       
	}
	else
	{
        
        HI_S32 s32Ret = HI_SUCCESS;
		s32Ret  = FeiYuCmdExecute(CMD_SET_PROMPT_VOICE_STATE, PROMPT_VOICE_MIDDLE, NULL);
        printf("s32Ret=====%d\n",s32Ret);
         //show hud when setting failed!
        if(HI_SUCCESS != s32Ret)
        {
            showHudWithTextAndTime(ID_STR_GENERAL_VOICE_ERROR,1.5);
        }else
        {
            //back to home when setting succeed
            s_GeneralFunctionState.VoiceState = PROMPT_VOICE_MIDDLE;
        }
	}
	HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);
	usleep(UI_HIGHLIGHT_TIME);
    s_voice_flag = 1;

}

HI_S32 FEIYU_GENERAL_FUNCTION_BUTTON_VOICE_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = 0;
    pthread_t threadid = 0; 
	if(bongesturetap == HI_FALSE)
	{
		bongesturetap = HI_TRUE;
	    s32Ret = pthread_create(&threadid, NULL, (void *)doVoice, &hWidget);
	    if(0 != s32Ret)
	    {
	        MLOGE("pthread_create changeWorkMode Failed, s32Ret=%#x\n", s32Ret);
	    } 
	    s32Ret = pthread_create(&threadid, NULL, (void *)doShowVoice, NULL);
	    if(0 != s32Ret)
	    {
	        MLOGE("pthread_create changeWorkMode Failed, s32Ret=%#x\n", s32Ret);
	    }  
	}
	
    return HIGV_PROC_GOON;
}

static HI_S32 s_wifi_flag = 0;

static HI_VOID doShowWifi(void *arg)
{
    pthread_detach(pthread_self());
    if(s_GeneralFunctionState.wifiState == TURN_WIFI_ON)
    { 
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LABEL, HIGV_SKIN_NORMAL, background_skin_gray);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LABEL, HIGV_SKIN_ACITVE, background_skin_gray);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_WIFI, HIGV_SKIN_NORMAL, FAST_OPTION_Wifi_noselect);        
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_WIFI, HIGV_SKIN_ACITVE, FAST_OPTION_Wifi_noselect);
    }
    else
    {       
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LABEL, HIGV_SKIN_NORMAL, background_skin);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LABEL, HIGV_SKIN_ACITVE, background_skin);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_WIFI, HIGV_SKIN_NORMAL, FAST_OPTION_Wifi_select);        
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_WIFI, HIGV_SKIN_ACITVE, FAST_OPTION_Wifi_select);
	}
    HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);
	
    usleep(UI_HIGHLIGHT_TIME);
    s_wifi_flag = 1;
}
static HI_VOID doWifi(void *arg)
{
    pthread_detach(pthread_self());
    while(s_wifi_flag == 0)
    {
        usleep(1000);
    }
    s_wifi_flag = 0;

    HI_S32 s32Ret = HI_SUCCESS;
	if(TURN_WIFI_ON == s_GeneralFunctionState.wifiState)
	{
		s32Ret = FeiYuCmdExecute(CMD_SET_WIFI_STATE,TURN_WIFI_OFF, NULL);
        if(HI_SUCCESS != s32Ret)
        {
            showHudWithTextAndTime(ID_STR_GENERAL_ERROR,1.5);
        }
        else
        {
            s_GeneralFunctionState.wifiState = TURN_WIFI_OFF;
            UI_ShowHomePage();
        }
	}
	else
	{
		s_bWifiStart = HI_TRUE;
		isHideGeneralFunction = HI_TRUE;
		moveY_ArcGroupbox(0,-230);
		
		s32Ret = FeiYuCmdExecute(CMD_SET_WIFI_STATE,TURN_WIFI_ON, NULL);
        if(HI_SUCCESS != s32Ret)
        {
            showHudWithTextAndTime(ID_STR_GENERAL_ERROR,1.5);
        }
        else
        {
            s_GeneralFunctionState.wifiState = TURN_WIFI_ON;
            //UI_ShowHomePage();
        }
	}    

}

HI_S32 FEIYU_GENERAL_FUNCTION_BUTTON_WIFI_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = 0;
    pthread_t threadid = 0;
	if(bongesturetap == HI_FALSE)
	{
		bongesturetap = HI_TRUE;
	    s32Ret = pthread_create(&threadid, NULL, (void *)doShowWifi, NULL);
	    if(0 != s32Ret)
	    {
	        MLOGE("pthread_create changeWorkMode Failed, s32Ret=%#x\n", s32Ret);
	    }          
	    s32Ret = pthread_create(&threadid, NULL, (void *)doWifi, &hWidget);
	    if(0 != s32Ret)
	    {
	        MLOGE("pthread_create changeWorkMode Failed, s32Ret=%#x\n", s32Ret);
	    }  
	}
    
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_GENERAL_FUNCTION_ongesturefling(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;

    MLOGD("\n");

    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetState failed\n");
        return HIGV_PROC_STOP;
    }

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
    MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);

	if(HI_FALSE == isHideGeneralFunction)
	{
		if (abs(startX - endX) > abs(startY - endY))// left or right
		{
			if (startX > endX)// to left
			{
				
			}
			else // to right
			{

			}
		}
		else //up or down
		{
			if (startY > endY)//to up
			{
				if(g_pageChangeEnd == HI_TRUE)
				{
					g_pageChangeEnd = HI_FALSE;
					doHideGeneralFunction();
				}
			}
			else  //down
			{


			}
		}
	}

    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_GENERAL_FUNCTION_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HIGV_GESTURE_EVENT_S gestureEvent;
    HI_S32 x, y;
    memset(&gestureEvent, 0x0, sizeof(gestureEvent));
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    x = gestureEvent.gesture.tap.pointer.x;
    y = gestureEvent.gesture.tap.pointer.y;

    if((x >= 330)&&(x <= 410)&&(y >= 60)&&(y <= 140))
	{
		HI_GV_Msg_SendAsync(FEIYU_GENERAL_FUNCTION_BUTTON_WIFI, HIGV_MSG_GESTURE_TAP, 0, 0);
        return HIGV_PROC_STOP;
	}
	else if((x >= 70)&&(x <= 150)&&(y >= 60)&&(y <= 140))
	{
		HI_GV_Msg_SendAsync(FEIYU_GENERAL_FUNCTION_BUTTON_LOCK, HIGV_MSG_GESTURE_TAP, 0, 0);
        return HIGV_PROC_STOP;
	} 
	else if((x >= 200)&&(x <= 280)&&(y >= 80)&&(y <= 160))
	{
		HI_GV_Msg_SendAsync(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HIGV_MSG_GESTURE_TAP, 0, 0);
        return HIGV_PROC_STOP;
	}
	else if((x >= 200)&&(x <= 280)&&(y >= 230)&&(y <= 320))
	{
		HI_GV_Msg_SendAsync(FEIYU_GENERAL_FUNCTION_BUTTON_SHUT_DOWN, HIGV_MSG_GESTURE_TAP, 0, 0);
        return HIGV_PROC_STOP;
	}
    return HIGV_PROC_GOON;
}

HI_S32 Feiyu_General_Function_OnKeyDown(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    if (HIGV_KEY_F2 == wParam) //mode key
    {
        hideAlertView();
        UI_ShowHomePage();
    }
    else if (HIGV_KEY_F1 == wParam) //photo key
    {
        hideAlertView();
		HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
        UI_ShowHomePage();
    }
    else
    {
        MLOGE("KEY CODE %#x not processed\n", wParam);
    }

    return HIGV_PROC_GOON;
}


HI_S32 Feiyu_General_Function_OnShow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	HI_S32 wifiState = 0, voiceState = 0;

	isHideGeneralFunction = HI_TRUE;
	bongesturetap = HI_FALSE;

	if(HI_TRUE == isHideGeneralFunction)
	{
		HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_INFO_GROUPBOX);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_SHUT_DOWN, HIGV_SKIN_NORMAL, FAST_OPTION_Shutdown_noselect);
		HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_BUTTON_SHUT_DOWN, 0);
		doHideGeneralFunction();
	}
	    
	FeiYuCmdExecute(CMD_GET_WIFI_STATE, 0, &wifiState);
	FeiYuCmdExecute(CMD_GET_PROMPT_VOICE_STATE, 0, &voiceState);
	s_GeneralFunctionState.wifiState = wifiState;
	s_GeneralFunctionState.VoiceState = voiceState;
	s_GeneralFunctionState.LockState = HI_FALSE; 
	MLOGD("~~~~~wifiState:%d,VoiceState:%d\n", wifiState, voiceState);
	refreshButtonState();
    
    return HIGV_PROC_GOON;
}

HI_S32 Feiyu_General_Function_OnHide(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	if(s_bWifiShow == HI_TRUE)
	{
		s_bWifiShow = HI_FALSE;
		HI_GV_Timer_Stop(FEIYU_GENERAL_FUNCTION, FEIYU_GENERAL_FUNCTION_WIFI_TIMER);
		HI_GV_Timer_Destory(FEIYU_GENERAL_FUNCTION, FEIYU_GENERAL_FUNCTION_WIFI_TIMER);
		HI_GV_Timer_Stop(FEIYU_GENERAL_FUNCTION, FEIYU_GENERAL_FUNCTION_CHECK_WIFI_TIMER);
		HI_GV_Timer_Destory(FEIYU_GENERAL_FUNCTION, FEIYU_GENERAL_FUNCTION_CHECK_WIFI_TIMER);
	}
	return HIGV_PROC_GOON;
}

static void refreshButtonState()
{
    if(s_GeneralFunctionState.wifiState == TURN_WIFI_ON)
    {
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_WIFI, HIGV_SKIN_NORMAL, FAST_OPTION_Wifi_select);    
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_WIFI, HIGV_SKIN_ACITVE, FAST_OPTION_Wifi_select);    
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LABEL, HIGV_SKIN_NORMAL, background_skin);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LABEL, HIGV_SKIN_ACITVE, background_skin); 
    }
    else
    {
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_WIFI, HIGV_SKIN_NORMAL, FAST_OPTION_Wifi_noselect);        
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_WIFI, HIGV_SKIN_ACITVE, FAST_OPTION_Wifi_noselect);        
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LABEL, HIGV_SKIN_NORMAL, background_skin_gray);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LABEL, HIGV_SKIN_ACITVE, background_skin_gray);
	}
    if(s_GeneralFunctionState.VoiceState != PROMPT_VOICE_OFF)
    {
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HIGV_SKIN_NORMAL, FAST_OPTION_Voice_select);        
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HIGV_SKIN_ACITVE, FAST_OPTION_Voice_select);        
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_VOICE_LABEL, HIGV_SKIN_NORMAL, background_skin);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_VOICE_LABEL, HIGV_SKIN_ACITVE, background_skin); 
	}
    else
    {
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HIGV_SKIN_NORMAL, FAST_OPTION_Voice_noselect);        
    	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, HIGV_SKIN_ACITVE, FAST_OPTION_Voice_noselect);        
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_VOICE_LABEL, HIGV_SKIN_NORMAL, background_skin_gray);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_VOICE_LABEL, HIGV_SKIN_ACITVE, background_skin_gray); 
	}
	if(s_GeneralFunctionState.LockState == HI_TRUE)
	{
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_LOCK, HIGV_SKIN_NORMAL, FAST_OPTION_Lock_select);        
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_LOCK, HIGV_SKIN_ACITVE, FAST_OPTION_Lock_select);        
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_LOCK_LABEL, HIGV_SKIN_NORMAL, background_skin);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_LOCK_LABEL, HIGV_SKIN_ACITVE, background_skin); 
	}
	else
	{
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_LOCK, HIGV_SKIN_NORMAL, FAST_OPTION_Lock_noselect);        
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_LOCK, HIGV_SKIN_ACITVE, FAST_OPTION_Lock_noselect);        
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_LOCK_LABEL, HIGV_SKIN_NORMAL, background_skin_gray);
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_LOCK_LABEL, HIGV_SKIN_ACITVE, background_skin_gray);
	}
	HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_BUTTON_WIFI, 0);
	HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_BUTTON_VOICE, 0);
	HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_BUTTON_LOCK, 0);
    HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);
}

/**  alertView callback  */
static void alerViewCallbackProc(HI_HANDLE hWidget)
{
    HI_S32 s32Ret = HI_SUCCESS;
    switch(hWidget)
    {
        case FEIYU_COMMON_UIALERTVIEW_CONFIRMBUTTON:
        {
            //highlight the  button
            extern HI_VOID feiyuBackLed(HI_S16 light);
            feiyuBackLed(g_pstUserPara->commPara.u8Backlight);//0~1023
		    //himm(0x12150008,0x00);
            s32Ret = FeiYuCmdExecute(CMD_SHUT_DOWN, 0, NULL);
            //if not cmd succeed, show hud to notify user!
            if(HI_SUCCESS != s32Ret)
            {
                //show hud
                showHudWithTextAndTime(ID_STR_GENERAL_ERROR,1.5);
                //unhighlight the button
            }
            break;
        }
        case FEIYU_COMMON_UIALERTVIEW_CANCELBUTTON:
        {
            hideAlertView();
            break;
        }
        default:
        {
            break;
        }
    }
}

HI_S32 Feiyu_General_Function_WIFI_INFO_onshow()
{
    s_bCurrentPic = 0;
	s_bWifiShow = HI_TRUE;

	HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_INFO_GROUPBOX);
	HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_LINE);
	HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_LOAD_1);
	HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_LOAD);
	HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_BUTTON_SHUT_DOWN, HIGV_SKIN_NORMAL, FAST_OPTION_Wifi_quit);
	if(s_s32LanguagrType == LANGUAGE_CHINESE)
	{
		if(g_stUserPara.commPara.u8WifiNameIsSet == 1)
		{
			HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value_CHINESE, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID);
    		HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value_CHINESE, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD);
		}
		else
		{
			HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value_CHINESE, "");
    		HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value_CHINESE, "");
		}
		HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID);
		HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD);
		HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value);
		HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value);
		HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_CHINESE);
		HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_CHINESE);
		HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value_CHINESE);
		HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value_CHINESE);
		HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value_CHINESE, 0);
		HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value_CHINESE, 0);
	}
    else
    {
    	if(g_stUserPara.commPara.u8WifiNameIsSet == 1)
		{
			HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID);
			HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD);
		}
		else
		{
			HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value, "");
    		HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value, "");
		}
    	HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_CHINESE);
		HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_CHINESE);
		HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value_CHINESE);
		HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value_CHINESE);
		HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID);
		HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD);
		HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value);
		HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value);
		HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value, 0);
		HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value, 0);
    }
	HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_BUTTON_SHUT_DOWN, 0);
	HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_WIFI_INFO_GROUPBOX, 0);
	HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);
	
    HI_GV_Timer_Create(FEIYU_GENERAL_FUNCTION, FEIYU_GENERAL_FUNCTION_WIFI_TIMER, FEIYU_GENERAL_FUNCTION_WIFI_INFO_INTERVAL);
    HI_GV_Timer_Start(FEIYU_GENERAL_FUNCTION, FEIYU_GENERAL_FUNCTION_WIFI_TIMER);
	HI_GV_Timer_Create(FEIYU_GENERAL_FUNCTION, FEIYU_GENERAL_FUNCTION_CHECK_WIFI_TIMER, FEIYU_GENERAL_FUNCTION_CHECK_WIFI_INTERVAL);
    HI_GV_Timer_Start(FEIYU_GENERAL_FUNCTION, FEIYU_GENERAL_FUNCTION_CHECK_WIFI_TIMER);

    return HIGV_PROC_GOON;
}


HI_S32 FEIYU_GENERAL_FUNCTION_WIFI_INFO_ontimer(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{  
	HI_BOOL bAPState = HI_FALSE;
	HI_U32 u32Value = -1;
	if(wParam == FEIYU_GENERAL_FUNCTION_CHECK_WIFI_TIMER)
	{
		if(g_stUserPara.commPara.u8WifiNameIsSet == 1)
		{
			if(s_s32LanguagrType == LANGUAGE_CHINESE)
			{
				HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value_CHINESE, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID);
				HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value_CHINESE, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD);
				HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value_CHINESE, 0);
				HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value_CHINESE, 0);
			}
			else
			{
				HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID);
				HI_GV_Widget_SetText(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD);
				HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiSSID_value, 0);
				HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_WIFI_INFO_WifiPASSWD_value, 0);
			}
		}
		else
		{
			HI_GV_Timer_Reset(FEIYU_GENERAL_FUNCTION, FEIYU_GENERAL_FUNCTION_CHECK_WIFI_TIMER);
		}

	}
	else if(wParam == FEIYU_GENERAL_FUNCTION_WIFI_TIMER)
	{  
		bAPState = HI_WIFIMNG_GetState();   
		himd(0x12040098, &u32Value);
	    if(!bAPState)
	    {
	        HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_LOAD);     
			HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_LOAD_1); 
	    }
	    if(HI_TRUE == bAPState && !u32Value)
	    {
	    	HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_LOAD);
			HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_LOAD_1);
			usleep(5000 * 1000);
			HI_GV_Msg_SendAsync(FEIYU_GENERAL_FUNCTION_BUTTON_SHUT_DOWN, HIGV_MSG_GESTURE_TAP, 0, 0);
			return HIGV_PROC_STOP;
	    }
		if((bAPState && u32Value))
		{
			HI_GV_Widget_Show(FEIYU_GENERAL_FUNCTION_WIFI_LOAD);
			HI_GV_Widget_Hide(FEIYU_GENERAL_FUNCTION_WIFI_LOAD_1);
		    switch(s_bCurrentPic)
		    {
		        case 0:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_1);
		            break;
		        case 1:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_2);            
		            break;
		        case 2:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_3);            
		            break;
		        case 3:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_4);            
		            break;
		        case 4:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_5);            
		            break;
		        case 5:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_6);            
		            break;
		        case 6:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_7);            
		            break;
		        case 7:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_8);            
		            break;            
		        case 8:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_9);            
		            break;            
		        case 9:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_10);            
		            break;            
		        case 10:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_11);            
		            break;            
		        case 11:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_12);            
		            break;            
		        case 12:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_13);            
		            break;            
		        case 13:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_14);            
		            break;            
		        case 14:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_15);            
		            break;                        
		        case 15:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_16);            
		            break;                        
		        case 16:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_17);            
		            break;                        
		        case 17:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_18);            
		            break;                          
		        default:
		            HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_1);            
		            break;
		    }
		    s_bCurrentPic++;
		    if(s_bCurrentPic > 17)
		        s_bCurrentPic = 0;
		}
		HI_GV_Widget_Refresh(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, 0);
	    HI_GV_Widget_Update(FEIYU_GENERAL_FUNCTION_WIFI_LOAD, 0);  
	    HI_GV_Widget_Refresh(FEIYU_GENERAL_FUNCTION, 0);
	    HI_GV_Widget_Update(FEIYU_GENERAL_FUNCTION, 0);      
	    HI_GV_Timer_Reset(FEIYU_GENERAL_FUNCTION, FEIYU_GENERAL_FUNCTION_WIFI_TIMER);
    }
    return HIGV_PROC_GOON;
}


/***********************anmaition handle!********************************/
/**TopGroupBox*/
static void animMoveY_ArcGroupBoxUpdate_handle(HIGV_LISTENER_S* listener, void* data)
{
    HI_RECT rect = {0};
    HI_GV_Widget_GetRect(FEIYU_GENERAL_FUNCTION_ARC_GROUPBOX, &rect);
    HI_GV_Widget_Move(FEIYU_GENERAL_FUNCTION_ARC_GROUPBOX, rect.x, HI_GV_TweenAnimGetTweenValue(animation_ArcGroupbox_YHandle, 0));
    if((HI_FALSE == isHideGeneralFunction)&&(HI_FALSE == s_bWifiStart))
	{
		if(HI_GV_TweenAnimGetTweenValue(animation_RectGroupbox_YHandle, 0) == 230)
		{
			HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION, HIGV_SKIN_NORMAL, general_function_bg_percent_100);
			HI_GV_Widget_Update(FEIYU_GENERAL_FUNCTION, HI_NULL);
		}
		/*else if(HI_GV_TweenAnimGetTweenValue(animation_RectGroupbox_YHandle, 0) > 120)
		{
			MLOGE("general_function_bg_percent_80\n");
			HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION, HIGV_SKIN_NORMAL, general_function_bg_percent_80);
			HI_GV_Widget_Update(FEIYU_GENERAL_FUNCTION, HI_NULL);	
		}	*/	
	}
    //HI_GV_Widget_Update(FEIYU_GENERAL_FUNCTION_ARC_GROUPBOX, HI_NULL);
}

static void animMoveY_ArcGroupBoxStart_handle(HIGV_LISTENER_S* listener, void* data)
{
    MLOGE(">>===animMoveY_ArcGroupBoxStart_handle\n");
	if((HI_FALSE == isHideGeneralFunction)&&(HI_FALSE == s_bWifiStart))
	{
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION, HIGV_SKIN_NORMAL, general_function_bg_percent_80);
		HI_GV_Widget_Update(FEIYU_GENERAL_FUNCTION, HI_NULL);
	}
	if((HI_TRUE == isHideGeneralFunction)&&(HI_FALSE == s_bWifiStart))
	{
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION, HIGV_SKIN_NORMAL, general_function_bg_percent_80);
		HI_GV_Widget_Update(FEIYU_GENERAL_FUNCTION, HI_NULL);
	}
}

static void animMoveY_ArcGroupBoxEnd_handle(HIGV_LISTENER_S* listener, void* data)
{
    MLOGE(">>===animMoveY_ArcGroupBoxEnd_handle-----%d\n",*(int*)data);
	/*if(HI_FALSE == isHideGeneralFunction)
	{
		HI_HANDLE s32ModeSkinID = general_function_arc_bg_after;
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_ARC_GROUPBOX, HIGV_SKIN_NORMAL, s32ModeSkinID);
		HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION_ARC_GROUPBOX, 0);
	}*/
	g_pageChangeEnd = HI_TRUE;
	if((HI_TRUE == isHideGeneralFunction)&&(HI_FALSE == s_bWifiStart))
	{
		UI_ShowHomePage();
	}
	if((HI_TRUE == isHideGeneralFunction)&&(HI_TRUE == s_bWifiStart))
	{
		Feiyu_General_Function_WIFI_INFO_onshow();
	}
}

/**BottomGroupBox*/
static void animMoveY_RectGroupBoxUpdate_handle(HIGV_LISTENER_S* listener, void* data)
{
    HI_RECT rect = {0};
    HI_GV_Widget_GetRect(FEIYU_GENERAL_FUNCTION_RECT_GROUPBOX, &rect);
    HI_GV_Widget_Move(FEIYU_GENERAL_FUNCTION_RECT_GROUPBOX, rect.x, HI_GV_TweenAnimGetTweenValue(animation_RectGroupbox_YHandle, 0));
	
    //HI_GV_Widget_Update(FEIYU_GENERAL_FUNCTION_RECT_GROUPBOX, HI_NULL);
}
static void animMoveY_RectGroupBoxStart_handle(HIGV_LISTENER_S* listener, void* data)
{
    MLOGE(">>===animMoveY_RectGroupBoxStart_handle\n");
}
static void animMoveY_RectGroupBoxEnd_handle(HIGV_LISTENER_S* listener, void* data)
{
    MLOGE(">>===animMoveY_RectGroupBoxEnd_handle-----%d\n",*(int*)data);
	g_pageChangeEnd = HI_TRUE;
    if(HI_FALSE == isHideGeneralFunction)
	{
		HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION, HIGV_SKIN_NORMAL, general_function_bg_percent_100);
		HI_GV_Widget_Paint(FEIYU_GENERAL_FUNCTION, 0);
	}
}

void moveY_ArcGroupbox(HIGV_CORD fromValue,HIGV_CORD toValue)
{
    animation_ArcGroupbox_YHandle = HI_GV_TweenAnimCreate();
    HI_GV_TweenAnimSetDuration(animation_ArcGroupbox_YHandle, 500);

    HI_GV_TweenAnimAddTween(animation_ArcGroupbox_YHandle,HIGV_TWEEN_TRANSITION_LINEAR,HIGV_TWEEN_EASE_OUT,fromValue,toValue);
	
    HI_GV_TweenAnimSetExecListener(animation_ArcGroupbox_YHandle, animMoveY_ArcGroupBoxUpdate_handle);
        
    HI_GV_TweenAnimSetStartedListener(animation_ArcGroupbox_YHandle, animMoveY_ArcGroupBoxStart_handle);

    HI_GV_TweenAnimSetFinishedListener(animation_ArcGroupbox_YHandle, animMoveY_ArcGroupBoxEnd_handle);
        
    HI_GV_TweenAnimStart(animation_ArcGroupbox_YHandle);
}

void moveY_RectGroupbox(HIGV_CORD fromValue,HIGV_CORD toValue)
{
    animation_RectGroupbox_YHandle = HI_GV_TweenAnimCreate();
    HI_GV_TweenAnimSetDuration(animation_RectGroupbox_YHandle, 500);

    HI_GV_TweenAnimAddTween(animation_RectGroupbox_YHandle,HIGV_TWEEN_TRANSITION_LINEAR,HIGV_TWEEN_EASE_OUT,fromValue,toValue);

    HI_GV_TweenAnimSetExecListener(animation_RectGroupbox_YHandle, animMoveY_RectGroupBoxUpdate_handle);
        
    HI_GV_TweenAnimSetStartedListener(animation_RectGroupbox_YHandle, animMoveY_RectGroupBoxStart_handle);

    HI_GV_TweenAnimSetFinishedListener(animation_RectGroupbox_YHandle, animMoveY_RectGroupBoxEnd_handle);

    HI_GV_TweenAnimStart(animation_RectGroupbox_YHandle);
}

/***********************anmaition handle!********************************/

static HI_VOID doHideGeneralFunction()
{
    HI_S32 s32Ret = HI_SUCCESS;
    if(HI_FALSE == isHideGeneralFunction)
    {    
		//HI_HANDLE s32ModeSkinID = background_skin;
		//HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION, HIGV_SKIN_NORMAL, s32ModeSkinID);
		//HI_GV_Widget_SetSkin(FEIYU_GENERAL_FUNCTION_RECT_GROUPBOX, HIGV_SKIN_NORMAL, s32ModeSkinID);
        moveY_ArcGroupbox(0,-230);
        moveY_RectGroupbox(230,320);
        isHideGeneralFunction = HI_TRUE;
    }
	else
    {               
        moveY_ArcGroupbox(-230,0);
        moveY_RectGroupbox(320,230);
        isHideGeneralFunction = HI_FALSE;
    }
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("WorkModeStop Error:%#x\n", s32Ret);
    }    
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

