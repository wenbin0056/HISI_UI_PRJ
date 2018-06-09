#include "higv_cextfile.h"
#include "uiconstants.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

HI_S32 Feiyu_Commonui_Hud_OnShow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    
    return HIGV_PROC_GOON;
}
HI_S32 FEIYU_COMMONUI_HUD_OnTimer(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_GV_Widget_Hide(FEIYU_COMMONUI_HUD);
    return HIGV_PROC_GOON;
}


HI_S32 FEIYU_COMMONUI_HUD_OnHide(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_GV_Timer_Stop(FEIYU_COMMONUI_HUD, HUD_HIDDENTIMER);
    HI_GV_Timer_Destory(FEIYU_COMMONUI_HUD, HUD_HIDDENTIMER);
    return HIGV_PROC_GOON;
}


HI_S32 FEIYU_COMMONUI_HUD_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_GV_Widget_Hide(FEIYU_COMMONUI_HUD);
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_COMMONUI_HUD_onevent(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{

    HI_GV_Widget_SetTextByID(FEIYU_COMMONUI_HUD_LABEL1,lParam);
    
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_COMMONUI_HUD_LOADING_OnHide(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_COMMONUI_HUD_LOADING_OnShow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
