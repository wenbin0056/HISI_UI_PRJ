
/**
*
* @file      feiyu_unlock_page.c
* @brief     feiyu unlock page
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

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

//-------------------------------------------------------------//
extern HI_S32 unlock_dv_go();

//-------------------------------------------------------------//
#if 0
static void  JumpToHomePage()
{
	HI_S32 s32Ret = HI_SUCCESS;
//	s32Ret = HI_GV_PARSER_LoadViewById(HOME_PAGE);
	s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
	s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
	s32Ret |= HI_GV_Widget_Hide(FEIYU_UNLOCK_PAGE);

	if (HI_SUCCESS != s32Ret)
	{
		MLOGE("HIGV Error: %#x\n", s32Ret);
	}	
}
#endif


//-------------------------------------------------------------//
HI_S32 UNLOCK_PAGE_OnKeyDown(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 UNLOCK_PAGE_on_kickback(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{


    return HIGV_PROC_GOON;
}

HI_S32 UNLOCK_PAGE_on_move(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{

    return HIGV_PROC_GOON;
}


HI_S32 UNLOCK_PAGE_on_unlock(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	HI_GV_Widget_Hide(FEIYU_UNLOCK_PAGE);
	//unlock_dv_go();

    return HIGV_PROC_GOON;
}
HI_S32 UNLOCK_PAGE_OnShow(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_GV_Timer_Create(FEIYU_UNLOCK_PAGE, UNLOCK_PAGE_TIMER, UNLOCK_PAGE_TIMER_INTERVAL);
    HI_GV_Timer_Start(FEIYU_UNLOCK_PAGE, UNLOCK_PAGE_TIMER);
    return HIGV_PROC_GOON;
}

HI_S32 UNLOCK_PAGE_OnHide(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_GV_Timer_Stop(FEIYU_UNLOCK_PAGE, UNLOCK_PAGE_TIMER);
    HI_GV_Timer_Destory(FEIYU_UNLOCK_PAGE, UNLOCK_PAGE_TIMER);
    return HIGV_PROC_GOON;
}



HI_S32 UNLOCK_PAGE_OnTimer(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{

    HI_GV_Widget_Paint(FEIYU_UNLOCK_PAGE,0);
    HI_GV_Timer_Reset(FEIYU_UNLOCK_PAGE, UNLOCK_PAGE_TIMER);
    
    return HIGV_PROC_GOON;
}

HI_S32 UNLOCK_PAGE_OnEvent(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    if(0 == wParam)
    {
        if(HI_FALSE == HI_GV_Widget_IsShow(FEIYU_UNLOCK_PAGE))
        {
            s32Ret |= HI_GV_Widget_Show(FEIYU_UNLOCK_PAGE);
            s32Ret |= HI_GV_Widget_MoveToTop(FEIYU_UNLOCK_PAGE);            
        }

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

