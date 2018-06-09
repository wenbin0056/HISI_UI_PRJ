#include "higv_cextfile.h"
#include "hi_product_devmng_ext.h"
#include "ui_common.h"
#include "uiconstants.h"
#include "hi_gv_scrollview.h"
#include "hi_gv_parser.h"
#include "higv_language.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

HI_S32 FEIYU_INFO_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
	HI_GV_Widget_Hide(FEIYU_INFO);
    HI_GV_Widget_Paint(FEIYU_INFO, 0);

    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_INFO_onkeydown(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_INFO_onshow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_PDT_DEVINFO_S stSystemInfo;
    
    s32Ret = HI_PDT_DevMng_GetDevInfo( &stSystemInfo );
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR\n");
        return HIGV_PROC_GOON;
    }    
    s32Ret |=HI_GV_Widget_SetTextByID(FEIYU_INFO_ITEM, ID_STR_SWVersion);
    s32Ret |=HI_GV_Widget_SetText(FEIYU_INFO_VAL, stSystemInfo.aszSoftVersion);
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR, RET=%d\n",s32Ret);
    }

    return HIGV_PROC_GOON;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

