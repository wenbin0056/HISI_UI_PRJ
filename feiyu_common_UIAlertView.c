#include "higv_cextfile.h"
#include "feiyu_UICommon_FunTool.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

HI_S32 Feiyu_Common_Uialertview_OnShow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_COMMON_UIALERTVIEW_CONFIRMBUTTON_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    alertViewConfirmButtonOnClick(hWidget,wParam,lParam);
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_COMMON_UIALERTVIEW_CANCELBUTTON_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    alertViewCancelButtonOnClick(hWidget,wParam,lParam);
    return HIGV_PROC_GOON;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

