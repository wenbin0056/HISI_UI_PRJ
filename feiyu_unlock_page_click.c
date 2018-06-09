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


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */
static HI_S32 s_s32TouchCount = 0;
HI_BOOL g_bIsDvLocking = HI_FALSE;

HI_S32 UNLOCK_CLICK_button_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_GV_Timer_Reset(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_PAGE_SHOW_TIMER);
    s_s32TouchCount++;
    if(s_s32TouchCount < 2)
    {
        HI_GV_Timer_Reset(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_CLICK_TIMER);        
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_UNLOCK_PAGE_CLICK);        
        //s32Ret |= HI_GV_Widget_Show(HOME_PAGE);     
        //HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
        g_bIsDvLocking = HI_FALSE;
    }

    return HIGV_PROC_GOON;
}

HI_S32 UNLOCK_PAGE_CLICK_OnEvent(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    if(0 == wParam)
    {
        if(HI_FALSE == HI_GV_Widget_IsShow(FEIYU_UNLOCK_PAGE_CLICK))
        {
            s32Ret |= HI_GV_Widget_Show(FEIYU_UNLOCK_PAGE_CLICK);
            s32Ret |= HI_GV_Widget_MoveToTop(FEIYU_UNLOCK_PAGE_CLICK);            
        }

    }
    
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("err,ret=%d\n",s32Ret);
    }
    return HIGV_PROC_GOON;
    
}

HI_S32 UNLOCK_PAGE_CLICK_OnHide(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	HI_GV_Timer_Stop(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_CLICK_TIMER);
	HI_GV_Timer_Destory(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_CLICK_TIMER);
	HI_GV_Timer_Stop(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_PAGE_SHOW_TIMER);
	HI_GV_Timer_Destory(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_PAGE_SHOW_TIMER);    
    s_s32TouchCount = 0;

    return HIGV_PROC_GOON;
}

HI_S32 UNLOCK_PAGE_CLICK_OnKeyDown(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    if (HIGV_KEY_F2 == wParam || HIGV_KEY_F1 == wParam)
    {    
        HI_GV_Timer_Reset(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_PAGE_SHOW_TIMER);
        s_s32TouchCount++;
        if(s_s32TouchCount < 2)
        {
            HI_GV_Timer_Reset(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_CLICK_TIMER);        
        }
        else
        {
            s32Ret |= HI_GV_Widget_Hide(FEIYU_UNLOCK_PAGE_CLICK);        
            //s32Ret |= HI_GV_Widget_Show(HOME_PAGE);     
            //HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
            g_bIsDvLocking = HI_FALSE;
        }
    }

    return HIGV_PROC_GOON;
}

HI_S32 UNLOCK_PAGE_CLICK_OnShow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	HI_GV_Timer_Create(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_CLICK_TIMER, UNLOCK_CLICK_TIMER_INTERVAL);
    HI_GV_Timer_Start(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_CLICK_TIMER);
	HI_GV_Timer_Create(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_PAGE_SHOW_TIMER, UNLOCK_PAGE_SHOW_TIMER_INTERVAL);
    HI_GV_Timer_Start(FEIYU_UNLOCK_PAGE_CLICK, UNLOCK_PAGE_SHOW_TIMER);
    HI_GV_Widget_Active(FEIYU_UNLOCK_PAGE_CLICK);
    s_s32TouchCount = 0;
    g_bIsDvLocking = HI_TRUE;

    return HIGV_PROC_GOON;
}

HI_S32 UNLOCK_PAGE_CLICK_OnTimer(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    if(UNLOCK_CLICK_TIMER == wParam)
    {
        if(s_s32TouchCount < 2)
        {
            s_s32TouchCount = 0;
        }        
    }
    else if(UNLOCK_PAGE_SHOW_TIMER == wParam)
    {
        //HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
        s32Ret |= HI_GV_Widget_Hide(FEIYU_UNLOCK_PAGE_CLICK);
    }
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("err,ret=%d\n",s32Ret);
    }

    return HIGV_PROC_GOON;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

