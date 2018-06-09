#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/prctl.h>

#include "higv_cextfile.h"
#include "ui_common.h"

#ifndef CFG_HJD
static HI_S32 zoom_level = 0;
static HI_S32 scrollflag = 0;
static HI_PDT_WORKMODE_S g_stWorkMode = {0};
HI_S32 g_startY = 0;

static HI_VOID UI_ShowHomePage()
{
    HI_S32 s32Ret = HI_SUCCESS;
	
    s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
	s32Ret |= HI_GV_Widget_Hide(FEIYU_ZOOM_PAGE);

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
}

HI_S32 FEIYU_ZOOM_PAGE_Ongesturescroll (HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 endY;
    HI_S32 dy = 0, distance = 0, dmove = 0;
	static HI_S32 tmp_level = 0; 
    HI_S32 s32ModeSkinID = 0;
	
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    endY = gestureEvent.gesture.scroll.end.y;	
	MLOGD(">>>>>>>>>>>> endY:%d\n", endY);

	if(((290 == g_startY) && (290 <= endY))||((30 == g_startY)&&(30 >= endY)))
	{
		goto End;
	}

	if(0 == scrollflag)
	{
		tmp_level = zoom_level;
		scrollflag = 1;
		MLOGD("tmp_level:%d\n", tmp_level);
	}

	dmove = g_startY - endY;
	distance = (g_startY - endY) > 0 ?  g_startY : (320 - g_startY);

	if(distance > 200)
		dy = tmp_level + (dmove/16);
	else if(distance > 150)
		dy = tmp_level + (dmove/14);
	else if(distance > 100)
		dy = tmp_level + (dmove/12);
	else if(distance > 50)
		dy = tmp_level + (dmove/8);	
	if(dy > 7)
		dy = 7;
	if(dy < 0)
		dy = 0;
	
	extern HI_VOID gku_zoom_control(HI_U32 option);
    gku_zoom_control(dy);

	switch(dy)
	{
		case 0:
			s32ModeSkinID = feiyu_zoom_0;
			zoom_level = 0;
			break;
		case 1:
			s32ModeSkinID = feiyu_zoom_1;
			zoom_level = 1;
			break;
		case 2:
			s32ModeSkinID = feiyu_zoom_2;
			zoom_level = 2;
			break;
		case 3:
			s32ModeSkinID = feiyu_zoom_3;
			zoom_level = 3;
			break;
		case 4:
			s32ModeSkinID = feiyu_zoom_4;
			zoom_level = 4;
			break;
		case 5:
			s32ModeSkinID = feiyu_zoom_5;
			zoom_level = 5;
			break;
		case 6:
			s32ModeSkinID = feiyu_zoom_6;
			zoom_level = 6;
			break;
		case 7:
			s32ModeSkinID = feiyu_zoom_7;
			zoom_level = 7;
			break;

	}

	s32Ret |= HI_GV_Widget_Paint(FEIYU_ZOOM_PAGE, 0);
    s32Ret |= HI_GV_Widget_SetSkin(FEIYU_ZOOM_LABEL, HIGV_SKIN_NORMAL, s32ModeSkinID);
	s32Ret |= HI_GV_Widget_Paint(FEIYU_ZOOM_LABEL, 0);
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,ret=%d\n",s32Ret);
    } 
	
End:

    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_ZOOM_PAGE_OnShow(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	MLOGD("FEIYU_ZOOM_PAGE_OnShow\n");	
	HI_PDT_WORKMODE_S stWorkMode = {0};

    HI_PDT_WorkMode_GetWorkMode(&stWorkMode);
	if (memcmp(&g_stWorkMode, &stWorkMode, sizeof(HI_PDT_WORKMODE_S)))
	{
		zoom_level = 0;
		memcpy(&g_stWorkMode, &stWorkMode, sizeof(HI_PDT_WORKMODE_S));
		HI_GV_Widget_SetSkin(FEIYU_ZOOM_LABEL, HIGV_SKIN_NORMAL, feiyu_zoom_0);
	}
	if(HI_GV_Widget_IsShow(FEIYU_ZOOM_LABEL))
	{
		HI_GV_Widget_Hide(FEIYU_ZOOM_LABEL);
	}

	return HIGV_PROC_GOON;
}

HI_S32 FEIYU_ZOOM_PAGE_OnHide(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	return HIGV_PROC_GOON;
}

HI_S32 FEIYU_ZOOM_PAGE_Ongesturelongtap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	UI_ShowHomePage();
	return HIGV_PROC_GOON;
}

HI_S32 FEIYU_ZOOM_PAGE_Ontouchaction(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
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
			if(point_x < 30)
				point_x = 30;
			if(point_x > 424)
				point_x = 424;
			if(point_y < 30)
				point_y = 30;
			if(point_y > 290)
				point_y = 290;
			g_startY = point_y;
			HI_GV_Widget_Show(FEIYU_ZOOM_LABEL);
			HI_GV_Widget_Move(FEIYU_ZOOM_LABEL, point_x - 30 , point_y - 30);
			HI_GV_Widget_Update(FEIYU_ZOOM_LABEL, HI_NULL);

            break;
        }

        case HIGV_TOUCH_END:
        {
			scrollflag = 0;
			MLOGD(">>>>>>>>>> HIGV_TOUCH_END\n");
			HI_GV_Widget_Hide(FEIYU_ZOOM_LABEL);
			UI_ShowHomePage();
            break;
        }

        default:
        	break;
    }

    return HIGV_PROC_GOON;	
}
#else

HI_S32 FEIYU_ZOOM_PAGE_Ongesturescroll (HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_ZOOM_PAGE_OnShow(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	return HIGV_PROC_GOON;
}

HI_S32 FEIYU_ZOOM_PAGE_OnHide(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	return HIGV_PROC_GOON;
}

HI_S32 FEIYU_ZOOM_PAGE_Ongesturelongtap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	return HIGV_PROC_GOON;
}

HI_S32 FEIYU_ZOOM_PAGE_Ontouchaction(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;	
}

#endif
