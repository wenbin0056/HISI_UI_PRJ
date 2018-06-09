#include "higv_cextfile.h"
#include "feiyu_dv_cmd_proc.h"
#include "fy_user_param.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

static HI_BOOL s_bCurrentPic = 0;
static HI_BOOL s_bSwitchOn = HI_FALSE;
extern HI_S32 s_s32LanguagrType;
extern USER_PARA_S g_stUserPara;

HI_S32 WIFI_INFO_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HIGV_GESTURE_EVENT_S gestureEvent;
    HI_S32 x, y;
    memset(&gestureEvent, 0x0, sizeof(gestureEvent));
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    x = gestureEvent.gesture.tap.pointer.x;
    y = gestureEvent.gesture.tap.pointer.y;

    if((x >= 190)&&(x <= 290)&&(y >= 267)&&(y <= 302))
	{
		HI_GV_Msg_SendAsync(WIFI_INFO_back_1, HIGV_MSG_GESTURE_TAP, 0, 0);
        return HIGV_PROC_STOP;
	}
	else if((x >= 350)&&(x <= 480)&&(y >= 0)&&(y <= 80))
	{
		HI_GV_Msg_SendAsync(WIFI_INFO_button_switch, HIGV_MSG_GESTURE_TAP, 0, 0);
        return HIGV_PROC_STOP;
	}                      

	return HIGV_PROC_GOON;
}

HI_S32 WIFI_INFO_onkeydown(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 WIFI_INFO_onshow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_GV_Widget_Hide(WIFI_LOAD);
    HI_GV_Widget_Hide(WIFI_LOAD_1);

    s_bCurrentPic = 0;
    s_bSwitchOn = HI_WIFIMNG_GetState();

	if(s_s32LanguagrType == LANGUAGE_CHINESE)
	{
		if(g_stUserPara.commPara.u8WifiNameIsSet == 1)
		{
			HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiSSID_value_CHINESE, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID);
    		HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiPASSWD_value_CHINESE, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD);
		}
		else
		{
			HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiSSID_value_CHINESE, "");
    		HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiPASSWD_value_CHINESE, "");
		}
		HI_GV_Widget_Hide(WIFI_INFO_WifiSSID);
		HI_GV_Widget_Hide(WIFI_INFO_scrollbar_WifiSSID_value);
		HI_GV_Widget_Hide(WIFI_INFO_WifiPASSWD);
		HI_GV_Widget_Hide(WIFI_INFO_scrollbar_WifiPASSWD_value);
		HI_GV_Widget_Show(WIFI_INFO_WifiSSID_CHINESE);
		HI_GV_Widget_Show(WIFI_INFO_WifiPASSWD_CHINESE);
		HI_GV_Widget_Show(WIFI_INFO_scrollbar_WifiSSID_value_CHINESE);
		HI_GV_Widget_Show(WIFI_INFO_scrollbar_WifiPASSWD_value_CHINESE);
		HI_GV_Widget_Paint(WIFI_INFO_scrollbar_WifiSSID_value_CHINESE, 0);
		HI_GV_Widget_Paint(WIFI_INFO_scrollbar_WifiPASSWD_value_CHINESE, 0);
	}
    else
    {
        if(g_stUserPara.commPara.u8WifiNameIsSet == 1)
		{
			HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiSSID_value, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID);
			HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiPASSWD_value, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD);
		}
		else
		{
			HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiSSID_value, "");
    		HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiPASSWD_value, "");
		}
    	HI_GV_Widget_Hide(WIFI_INFO_WifiSSID_CHINESE);
		HI_GV_Widget_Hide(WIFI_INFO_WifiPASSWD_CHINESE);
		HI_GV_Widget_Hide(WIFI_INFO_scrollbar_WifiSSID_value_CHINESE);
		HI_GV_Widget_Hide(WIFI_INFO_scrollbar_WifiPASSWD_value_CHINESE);
		HI_GV_Widget_Show(WIFI_INFO_WifiSSID);
		HI_GV_Widget_Show(WIFI_INFO_WifiPASSWD);
		HI_GV_Widget_Show(WIFI_INFO_scrollbar_WifiSSID_value);
		HI_GV_Widget_Show(WIFI_INFO_scrollbar_WifiPASSWD_value);
		HI_GV_Widget_Paint(WIFI_INFO_scrollbar_WifiSSID_value, 0);
		HI_GV_Widget_Paint(WIFI_INFO_scrollbar_WifiPASSWD_value, 0);
    }

    HI_GV_Timer_Create(WIFI_INFO, WIFI_INTO_TIMER, WIFI_INFO_INTERVAL);
    HI_GV_Timer_Start(WIFI_INFO, WIFI_INTO_TIMER);
    HI_GV_Timer_Create(WIFI_INFO, WIFI_INTO_CHECK_WIFI_TIMER, WIFI_INFO_CHECK_WIFI_INTERVAL);
    HI_GV_Timer_Start(WIFI_INFO, WIFI_INTO_CHECK_WIFI_TIMER);
    return HIGV_PROC_GOON;
}

HI_S32 WIFI_INFO_button_switch_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    FeiYuCmdExecute(CMD_SET_WIFI_STATE,TURN_WIFI_ON, NULL);
    s_bSwitchOn = !s_bSwitchOn;
	return HIGV_PROC_GOON;    
}

HI_S32 WIFI_INFO_back_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("###\n");

	if(HI_FALSE == HI_GV_Widget_IsShow(TIPS_WINDOW))
	{
		HI_GV_Msg_SendAsync(FEIYU_GLOBAL_SETTINGS, HIGV_MSG_EVENT, UPDATE_WIFI_STATE, 0);
	    HI_GV_Widget_Hide(WIFI_INFO);
	    HI_GV_Widget_Paint(WIFI_INFO, 0);     
	}
	
    return HIGV_PROC_GOON;    
}

HI_S32 WIFI_INFO_onhide(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	HI_GV_Timer_Stop(WIFI_INFO, WIFI_INTO_CHECK_WIFI_TIMER);
	HI_GV_Timer_Destory(WIFI_INFO, WIFI_INTO_CHECK_WIFI_TIMER);
	HI_GV_Timer_Stop(WIFI_INFO, WIFI_INTO_TIMER);
	HI_GV_Timer_Destory(WIFI_INFO, WIFI_INTO_TIMER);
	return HIGV_PROC_GOON;	
}

HI_S32 WIFI_INFO_ontimer(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	if(wParam == WIFI_INTO_CHECK_WIFI_TIMER)
	{
		if(g_stUserPara.commPara.u8WifiNameIsSet == 1)
		{
			if(s_s32LanguagrType == LANGUAGE_CHINESE)
			{
				HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiSSID_value_CHINESE, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID);
				HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiPASSWD_value_CHINESE, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD);
				HI_GV_Widget_Paint(WIFI_INFO_scrollbar_WifiSSID_value_CHINESE, 0);
				HI_GV_Widget_Paint(WIFI_INFO_scrollbar_WifiPASSWD_value_CHINESE, 0);
			}
			else
			{
				HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiSSID_value, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiSSID);
				HI_GV_Widget_SetText(WIFI_INFO_scrollbar_WifiPASSWD_value, g_pstPDTCfg->stDevMngParam.stWiFiCfg.aszWiFiPWD);
				HI_GV_Widget_Paint(WIFI_INFO_scrollbar_WifiSSID_value, 0);
				HI_GV_Widget_Paint(WIFI_INFO_scrollbar_WifiPASSWD_value, 0);
			}
		}
		else
		{
			HI_GV_Timer_Reset(WIFI_INFO, WIFI_INTO_CHECK_WIFI_TIMER);
		}
	}
	else if(wParam == WIFI_INTO_TIMER)
	{
	    HI_BOOL bAPState = HI_FALSE;

	    bAPState = HI_WIFIMNG_GetState();
	    if(bAPState)
	        HI_GV_Widget_SetSkin(WIFI_INFO_button_switch, HIGV_SKIN_NORMAL, skin_wifi_on); 
	    else
	        HI_GV_Widget_SetSkin(WIFI_INFO_button_switch, HIGV_SKIN_NORMAL, skin_wifi_off); 
	    
		HI_U32 u32Value = -1;
		himd(0x12040098, &u32Value);
	    if(!bAPState)
	    {
	        HI_GV_Widget_Hide(WIFI_LOAD);
	        HI_GV_Widget_Hide(WIFI_LOAD_1);        
	    }
	    if( HI_TRUE == bAPState && !u32Value)
	    {
	    	HI_GV_Widget_Hide(WIFI_LOAD);
	        HI_GV_Widget_Show(WIFI_LOAD_1);
	    }
	    
	    if((bAPState && u32Value))
	    {
	        HI_GV_Widget_Show(WIFI_LOAD);
	        HI_GV_Widget_Hide(WIFI_LOAD_1);

	        switch(s_bCurrentPic)
	        {
	        case 0:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_1);
	        break;
	        case 1:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_2);            
	        break;
	        case 2:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_3);            
	        break;
	        case 3:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_4);            
	        break;
	        case 4:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_5);            
	        break;
	        case 5:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_6);            
	        break;
	        case 6:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_7);            
	        break;
	        case 7:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_8);            
	        break;            
	        case 8:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_9);            
	        break;            
	        case 9:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_10);            
	        break;            
	        case 10:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_11);            
	        break;            
	        case 11:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_12);            
	        break;            
	        case 12:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_13);            
	        break;            
	        case 13:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_14);            
	        break;            
	        case 14:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_15);            
	        break;                        
	        case 15:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_16);            
	        break;                        
	        case 16:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_17);            
	        break;                        
	        case 17:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_18);            
	        break;                          
	        default:
	        HI_GV_Widget_SetSkin(WIFI_LOAD, HIGV_SKIN_NORMAL, skin_wifi_load_1);            
	        break;
	        }
	        s_bCurrentPic++;
	        if(s_bCurrentPic > 17)
	        s_bCurrentPic = 0;    
	    }

	    
	    HI_GV_Widget_Refresh(WIFI_LOAD, 0);
	    HI_GV_Widget_Update(WIFI_LOAD, 0);  
	    HI_GV_Widget_Refresh(WIFI_INFO, 0);
	    HI_GV_Widget_Update(WIFI_INFO, 0);      
	    HI_GV_Timer_Reset(WIFI_INFO, WIFI_INTO_TIMER);
	}
    
    return HIGV_PROC_GOON;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

