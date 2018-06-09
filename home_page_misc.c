#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/prctl.h>
#include "feiyu_dv_cmd_proc.h"

#include "higv_cextfile.h"
#include "ui_common.h"

extern HI_PDT_WORKMODE_S g_stCurrentWorkMode;
extern HI_S32 zoom_level;
static HI_S32 scrollflag = 0;
static HI_S32 g_startY = 0;

typedef struct HomePageMiscMode_S{
  	HI_BOOL ZoomMode;
	HI_BOOL MetryMode;
    HI_RECT Rect;
}HomePageMiscMode;

static HomePageMiscMode s_HomePageMiscMode;
static HI_S32 zoom_tmp_level;
static HI_BOOL bModeStart = HI_FALSE;

extern HI_S32 userMetry(HI_S32 x, HI_S32 y);

HI_S32 HOME_PAGE_MISC_OnShow(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	MLOGD("HOME_PAGE_MISC_OnShow\n");	

	if(HI_GV_Widget_IsShow(HOME_PAGE_MISC_ZOOM_LABEL))
	{
		HI_GV_Widget_Hide(HOME_PAGE_MISC_ZOOM_LABEL);
	}
	
	if(HI_GV_Widget_IsShow(HOME_PAGE_MISC_METRY_LABEL))
	{
		HI_GV_Widget_Hide(HOME_PAGE_MISC_METRY_LABEL);
	}

	zoom_tmp_level = zoom_level;
	s_HomePageMiscMode.ZoomMode = HI_TRUE;
	s_HomePageMiscMode.MetryMode = HI_FALSE;
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_ZOOM_MODE, HIGV_SKIN_NORMAL, background_skin_blue);
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_ZOOM_MODE, HIGV_SKIN_ACITVE, background_skin_blue);
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_METRY_MODE, HIGV_SKIN_NORMAL, background_skin);
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_METRY_MODE, HIGV_SKIN_ACITVE, background_skin);
	return HIGV_PROC_GOON;
}


HI_S32 HOME_PAGE_MISC_GROUPBOX_ZOOM_MODE_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{	
	s_HomePageMiscMode.ZoomMode = HI_TRUE;
	s_HomePageMiscMode.MetryMode = HI_FALSE;
    HI_GV_Widget_Hide(HOME_PAGE_MISC_METRY_LABEL);
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_METRY_MODE, HIGV_SKIN_NORMAL, background_skin);
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_METRY_MODE, HIGV_SKIN_ACITVE, background_skin);
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_ZOOM_MODE, HIGV_SKIN_NORMAL, background_skin_blue);
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_ZOOM_MODE, HIGV_SKIN_ACITVE, background_skin_blue);
	HI_GV_Widget_Paint(HOME_PAGE_MISC, 0);
	return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_MISC_GROUPBOX_METRY_MODE_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	s_HomePageMiscMode.MetryMode = HI_TRUE;
	s_HomePageMiscMode.ZoomMode = HI_FALSE;
    
    //HI_GV_Widget_Show(HOME_PAGE_MISC_METRY_LABEL);
    if(s_HomePageMiscMode.Rect.x != 0 && s_HomePageMiscMode.Rect.y != 0)
    {
        //HI_GV_Widget_Move(HOME_PAGE_MISC_METRY_LABEL, s_HomePageMiscMode.Rect.x , s_HomePageMiscMode.Rect.y);
        //HI_GV_Widget_Update(HOME_PAGE_MISC_METRY_LABEL, HI_NULL);        
    }    
    else
    {
        //HI_GV_Widget_Move(HOME_PAGE_MISC_METRY_LABEL, 209, 129);
        //HI_GV_Widget_Update(HOME_PAGE_MISC_METRY_LABEL, HI_NULL);    
    }
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_METRY_MODE, HIGV_SKIN_NORMAL, background_skin_blue);
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_METRY_MODE, HIGV_SKIN_ACITVE, background_skin_blue);
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_ZOOM_MODE, HIGV_SKIN_NORMAL, background_skin);
	HI_GV_Widget_SetSkin(HOME_PAGE_MISC_GROUPBOX_ZOOM_MODE, HIGV_SKIN_ACITVE, background_skin);
	HI_GV_Widget_Paint(HOME_PAGE_MISC, 0);
	return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_MISC_GROUPBOX_OK_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	zoom_level = zoom_tmp_level;
	HI_S32 s32Ret = HI_SUCCESS;
    HI_GV_Widget_GetRect(HOME_PAGE_MISC_METRY_LABEL, &s_HomePageMiscMode.Rect);    
    s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Hide(HOME_PAGE_MISC);
    
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
	return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_MISC_GROUPBOX_CLOSE_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
	extern HI_VOID gku_zoom_control(HI_U32 option);
	gku_zoom_control(zoom_level);
    HI_S32 state = 0;
	s32Ret = get_val_PHOTO_METER(&state);
	s32Ret = set_mode_param_PHOTO_METER(state);
    //FeiYuCmdExecute(CMD_GET_PHOTO_METER, 0, &state);
    //FeiYuCmdExecute(CMD_SET_MODE_PARAM_PHOTO_METER, state, NULL);
    s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Hide(HOME_PAGE_MISC);
    
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
	return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_MISC_OnKeyDown(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	if (HIGV_KEY_F2 == wParam) //mode key
    {
        HI_GV_Msg_SendAsync(HOME_PAGE_MISC_GROUPBOX_OK, HIGV_MSG_GESTURE_TAP, 0, 0);
    }
    else if (HIGV_KEY_F1 == wParam) //photo key
    {
        HI_GV_Msg_SendAsync(HOME_PAGE_MISC_GROUPBOX_OK, HIGV_MSG_GESTURE_TAP, 0, 0);
		HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
    }
    else
    {
        MLOGE("KEY CODE %#x not processed\n", wParam);
    }

    return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_MISC_Ongesturescroll(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 endY;
    HI_S32 cur_level = 0, distance = 0, dmove = 0;
	static HI_S32 tmp_level = 0; 
    HI_S32 s32ModeSkinID = 0;
	
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    endY = gestureEvent.gesture.scroll.end.y;	
	MLOGD(">>>>>>>>>>>> endY:%d\n", endY);

	if((s_HomePageMiscMode.ZoomMode == HI_TRUE)&&(bModeStart == HI_TRUE))
	{
		if(PDT_WORKMODE_RECORD == g_stCurrentWorkMode.enWorkMode)
		{
			if(0 == scrollflag)
			{
				tmp_level = zoom_tmp_level;
				cur_level = zoom_tmp_level;
				scrollflag = 1;
				MLOGD("tmp_level:%d\n", tmp_level);
			}

			dmove = g_startY - endY;
			distance = (g_startY - endY) > 0 ?  g_startY : (320 - g_startY);
				
			if(distance > 200)
				cur_level = tmp_level + (dmove/16);
			else if(distance > 150)
				cur_level= tmp_level + (dmove/14);
			else if(distance > 100)
				cur_level = tmp_level + (dmove/12);
			else if(distance > 50)
				cur_level = tmp_level + (dmove/8);
			else
				cur_level = tmp_level + (dmove/5);
			if(cur_level > 7)
				cur_level = 7;
			if(cur_level< 0)
				cur_level = 0;
			
			extern HI_VOID gku_zoom_control(HI_U32 option);
		    gku_zoom_control(cur_level);

			switch(cur_level)
			{
				case 0:
					s32ModeSkinID = feiyu_zoom_0;
					zoom_tmp_level = 0;
					break;
				case 1:
					s32ModeSkinID = feiyu_zoom_1;
					zoom_tmp_level = 1;
					break;
				case 2:
					s32ModeSkinID = feiyu_zoom_2;
					zoom_tmp_level = 2;
					break;
				case 3:
					s32ModeSkinID = feiyu_zoom_3;
					zoom_tmp_level = 3;
					break;
				case 4:
					s32ModeSkinID = feiyu_zoom_4;
					zoom_tmp_level = 4;
					break;
				case 5:
					s32ModeSkinID = feiyu_zoom_5;
					zoom_tmp_level = 5;
					break;
				case 6:
					s32ModeSkinID = feiyu_zoom_6;
					zoom_tmp_level = 6;
					break;
				case 7:
					s32ModeSkinID = feiyu_zoom_7;
					zoom_tmp_level = 7;
					break;
			}

			s32Ret |= HI_GV_Widget_Paint(HOME_PAGE_MISC, 0);
		    s32Ret |= HI_GV_Widget_SetSkin(HOME_PAGE_MISC_ZOOM_LABEL, HIGV_SKIN_NORMAL, s32ModeSkinID);
			s32Ret |= HI_GV_Widget_Paint(HOME_PAGE_MISC_ZOOM_LABEL, 0);
		    if( HI_SUCCESS != s32Ret)
		    {
		        MLOGE("ERR,ret=%d\n",s32Ret);
		    } 
		}
	}
	
End:
    return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_MISC_OnHide(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Hide(HOME_PAGE_MISC);
    
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
	return HIGV_PROC_GOON;
}

HI_S32 HOME_PAGE_MISC_OnGestureTap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HIGV_GESTURE_EVENT_S gestureEvent;
    HI_S32 x, y;
    memset(&gestureEvent, 0x0, sizeof(gestureEvent));
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    x = gestureEvent.gesture.tap.pointer.x;
    y = gestureEvent.gesture.tap.pointer.y;

    if((x >= 0)&&(x <= 50)&&(y >= 270)&&(y <= 320))
	{
		HI_GV_Msg_SendAsync(HOME_PAGE_MISC_GROUPBOX_CLOSE, HIGV_MSG_GESTURE_TAP, 0, 0);
		return HIGV_PROC_STOP;
	}
	else if((x >= 430)&&(x <= 480)&&(y >= 270)&&(y <= 320))
	{
		HI_GV_Msg_SendAsync(HOME_PAGE_MISC_GROUPBOX_OK, HIGV_MSG_GESTURE_TAP, 0, 0);
		return HIGV_PROC_STOP;
	}
	else if((x > 50)&&(x <= 240)&&(y >= 270)&&(y <= 320))
	{
		HI_GV_Msg_SendAsync(HOME_PAGE_MISC_GROUPBOX_ZOOM_MODE, HIGV_MSG_GESTURE_TAP, 0, 0);
		return HIGV_PROC_STOP;
	} 
	else if((x > 240)&&(x < 430)&&(y >= 270)&&(y <= 320))
	{
		HI_GV_Msg_SendAsync(HOME_PAGE_MISC_GROUPBOX_METRY_MODE, HIGV_MSG_GESTURE_TAP, 0, 0);
		return HIGV_PROC_STOP;
	}
    else
    {
        if(s_HomePageMiscMode.MetryMode)
        {
            HI_GV_Widget_Show(HOME_PAGE_MISC_METRY_LABEL);
            HI_GV_Widget_Move(HOME_PAGE_MISC_METRY_LABEL, x-31 , y-31);
            HI_GV_Widget_Update(HOME_PAGE_MISC_METRY_LABEL, HI_NULL);
            userMetry(x,y);
        }
    }
    return HIGV_PROC_GOON;//HIGV_PROC_STOP
}

HI_S32 HOME_PAGE_MISC_Ontouchaction(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 point_x, point_y;
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;
	HI_S32 s32ModeSkinID = 0;

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
			if((s_HomePageMiscMode.ZoomMode == HI_TRUE)&&(point_y < 270))
			{		
				if(PDT_WORKMODE_RECORD == g_stCurrentWorkMode.enWorkMode)
				{
					bModeStart = HI_TRUE;
					
					if(point_x < 30)
						point_x = 30;
					if(point_x > 424)
						point_x = 424;
					if(point_y < 30)
					{
						point_y = 30;
						g_startY = 0;
					}
					else
					{
						g_startY = point_y;
					}
					MLOGD("~~~~~~~~~~~~~g_startY:%d\n", g_startY);
					switch(zoom_tmp_level)
					{
						case 0: s32ModeSkinID = feiyu_zoom_0; break;
						case 1: s32ModeSkinID = feiyu_zoom_1; break;
						case 2: s32ModeSkinID = feiyu_zoom_2; break;
						case 3: s32ModeSkinID = feiyu_zoom_3; break;
						case 4: s32ModeSkinID = feiyu_zoom_4; break;
						case 5: s32ModeSkinID = feiyu_zoom_5; break;
						case 6: s32ModeSkinID = feiyu_zoom_6; break;
						case 7: s32ModeSkinID = feiyu_zoom_7; break;
						default: s32ModeSkinID = feiyu_zoom_0; break;
					}
					HI_GV_Widget_SetSkin(HOME_PAGE_MISC_ZOOM_LABEL, HIGV_SKIN_NORMAL, s32ModeSkinID);
					HI_GV_Widget_Show(HOME_PAGE_MISC_ZOOM_LABEL);
					HI_GV_Widget_Move(HOME_PAGE_MISC_ZOOM_LABEL, point_x - 30 , point_y - 30);
					HI_GV_Widget_Update(HOME_PAGE_MISC_ZOOM_LABEL, HI_NULL);
				}
				else
				{
					UI_AlarmDisplay(ALARM_PHOTO_DISABLE_ZOOM);
				}
			}
            break;
        }

        case HIGV_TOUCH_END:
        {			
			MLOGD(">>>>>>>>>> HIGV_TOUCH_END\n");
			if((s_HomePageMiscMode.ZoomMode == HI_TRUE)&&(bModeStart == HI_TRUE))
			{			
				scrollflag = 0;
				bModeStart = HI_FALSE;
				HI_GV_Widget_Hide(HOME_PAGE_MISC_ZOOM_LABEL);
				usleep(1000 * 50);
				if(HI_TRUE == HI_GV_Widget_IsShow(HOME_PAGE_MISC_ZOOM_LABEL))
				{
					HI_GV_Widget_Hide(HOME_PAGE_MISC_ZOOM_LABEL);
				}
			}
            break;
        }

        default:
        	break;
    }

    return HIGV_PROC_GOON;
}
