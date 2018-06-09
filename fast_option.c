
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
extern "C"{
#endif
#endif /*  __cplusplus  */

HI_S32 FAST_OPTION_Lock_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;	
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
    
    return HIGV_PROC_GOON;
}

HI_S32 FAST_OPTION_OnGestureFling(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
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
    if(startX < (320 - 80))
    {
        MLOGE("startX=%d\n",startX);
        return HIGV_PROC_GOON;
    }
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
            HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
        }
        else  //down
        {

        }
    }

    return HIGV_PROC_GOON;    
}

HI_S32 FAST_OPTION_OnGestureTap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FAST_OPTION_OnKeyDown(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FAST_OPTION_OnShow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FAST_OPTION_Shutdown_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    FeiYuCmdExecute(CMD_SHUT_DOWN, 0, NULL);
    
    return HIGV_PROC_GOON;
}

HI_S32 FAST_OPTION_Voice_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 state = 0;
    HI_S32 s32Ret = HI_SUCCESS;    
    FeiYuCmdExecute(CMD_GET_PROMPT_VOICE_STATE, 0, &state);
    if(PROMPT_VOICE_OFF != state)
    {
        s32Ret  = FeiYuCmdExecute(CMD_SET_PROMPT_VOICE_STATE, PROMPT_VOICE_OFF, NULL);    
    }
    else
    {
        s32Ret  = FeiYuCmdExecute(CMD_SET_PROMPT_VOICE_STATE, PROMPT_VOICE_MIDDLE, NULL);        
    }
    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("failed\n");
    }

    return HIGV_PROC_GOON;
}

HI_S32 FAST_OPTION_Wifi_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    FeiYuCmdExecute(CMD_SET_WIFI_STATE,TURN_WIFI_ON, NULL);  
    
    return HIGV_PROC_GOON;
}


static HI_HANDLE s_ItemHandle = 0;

static void Item_Update_handle(HIGV_LISTENER_S* listener, void* data)
{
    HI_RECT rect = {0};
    HI_GV_Widget_GetRect(FAST_OPTION,&rect);
    HI_GV_Widget_Move(FAST_OPTION, rect.x,HI_GV_TweenAnimGetTweenValue(s_ItemHandle, 0));
    MLOGE("LEFT:    x=%f, y=%d\n",HI_GV_TweenAnimGetTweenValue(s_ItemHandle, 0), rect.y);

    HI_GV_Widget_Update(FAST_OPTION, HI_NULL);
    HI_GV_Widget_Refresh(FAST_OPTION,0);
    
}

static void Item_Start_handle(HIGV_LISTENER_S* listener, void* data)
{
   // MLOGE(">>\n");
}

static void Item_End_handle(HIGV_LISTENER_S* listener, void* data)
{
   // MLOGE(">>\n");
    HI_S32 s32Ret = HI_SUCCESS;
   s32Ret |= HI_GV_Widget_Hide(HOME_PAGE);
   s32Ret |= HI_GV_Widget_Hide(FEIYU_ZOOM_PAGE);

    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("err, s32Ret=%d\n",s32Ret);
    }

}


HI_VOID FASTOPTION_AnimmoveYItem(HIGV_CORD fromValue,HIGV_CORD toValue)
{
    s_ItemHandle = HI_GV_TweenAnimCreate();
    
    HI_GV_TweenAnimSetDuration(s_ItemHandle,350);

    HI_GV_TweenAnimAddTween(s_ItemHandle,HIGV_TWEEN_TRANSITION_LINEAR,HIGV_TWEEN_EASE_OUT,fromValue,toValue);

    HI_GV_TweenAnimSetExecListener(s_ItemHandle, Item_Update_handle);
        
    HI_GV_TweenAnimSetStartedListener(s_ItemHandle, Item_Start_handle);


    HI_GV_TweenAnimSetFinishedListener(s_ItemHandle, Item_End_handle);


    HI_GV_TweenAnimStart(s_ItemHandle);    
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

