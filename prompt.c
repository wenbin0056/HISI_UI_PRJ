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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*  __cplusplus  */

extern HI_BOOL g_bNormalFormat;

HI_HANDLE g_s32ActWindow = INVALID_HANDLE;
HI_BOOL g_bNormalFormat = HI_FALSE;//HI_TRUE: user enter FORMAT PAGE by menu, HI_FALSE: jump to format page by exception


HI_S32 ALARM_WINDOW_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if ((HIGV_KEY_F1 == wParam) || (HIGV_KEY_F2 == wParam) || (HIGV_KEY_F3 == wParam))
    {
        if(HI_GV_Widget_IsShow(ALARM_WINDOW))
        {
            s32Ret |= HI_GV_Widget_Hide(ALARM_WINDOW);
            s32Ret |= HI_GV_Widget_Active(g_s32ActWindow);
        }

        if (s32Ret != HI_SUCCESS)
        {
            MLOGE("Error:%#x\n", s32Ret);
        }
    }

    return HIGV_PROC_STOP;
}
HI_S32 SD_FORMAT_onshow(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_GV_Widget_SetTextByID(SD_FORMAT_ASK_BUTTON_format, ID_STR_FORMAT);

    return HIGV_PROC_GOON;
}

HI_S32 SD_FORMAT_ASK_BUTTON_cancel_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    if (HIGV_KEY_F2 == wParam)
    {
        s32Ret |= HI_GV_Widget_Active(SD_FORMAT_ASK_BUTTON_format);
    }
    else if ((HIGV_KEY_F1 == wParam) || (HIGV_KEY_F3 == wParam))
    {
        if (g_bNormalFormat == HI_FALSE)
        {
            s32Ret |= HI_GV_Widget_Active(g_s32ActWindow);
            s32Ret |= HI_GV_Widget_Hide(SD_FORMAT);
            HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_BACK_HOME, 0);
        }
        else
        {
            //s32Ret |= HI_GV_Widget_Active(SD_SET);
            s32Ret |= HI_GV_Widget_Hide(SD_FORMAT);
        }
    }

    if (s32Ret != HI_SUCCESS)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_STOP;
}

HI_S32 SD_FORMAT_ASK_BUTTON_format_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (HIGV_KEY_F2 == wParam)
    {
        s32Ret |= HI_GV_Widget_Active(SD_FORMAT_ASK_BUTTON_cancel);
    }
    else if (HIGV_KEY_F1 == wParam)
    {
        s32Ret |= HI_GV_Widget_Show(SD_FORMATING);
        s32Ret |= HI_GV_Widget_Active(SD_FORMATING);
        s32Ret |= HI_GV_Widget_Hide(SD_FORMAT);
    }
    else if (HIGV_KEY_F3 == wParam)
    {
        if (g_bNormalFormat == HI_FALSE)
        {
            s32Ret |= HI_GV_Widget_Active(g_s32ActWindow);
            s32Ret |= HI_GV_Widget_Hide(SD_FORMAT);
        }
        else
        {
            //s32Ret |= HI_GV_Widget_Active(SD_SET);
            s32Ret |= HI_GV_Widget_Hide(SD_FORMAT);
        }
    }

    if (s32Ret != HI_SUCCESS)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_STOP;
}

static void * ShowSuccess(void* pVoid)
{
	HI_S32 s32Ret = HI_SUCCESS;
	
	s32Ret |= HI_GV_PARSER_LoadViewById(SD_FORMAT_SUCCESS);
	s32Ret |= HI_GV_Widget_Show(SD_FORMAT_SUCCESS);
	usleep(1000 * 500);
	s32Ret |= HI_GV_Widget_Hide(SD_FORMAT_SUCCESS);
	
    if (s32Ret != HI_SUCCESS)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

	
	return NULL;
}

HI_S32 SD_FORMATING_onrefresh(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret |= HI_PDT_Storage_Format(NULL);

    if (s32Ret == HI_SUCCESS)
    {
        if (g_bNormalFormat == HI_FALSE)
        {
            s32Ret |= HI_GV_Widget_Active(g_s32ActWindow);
            s32Ret |= HI_GV_Widget_Hide(SD_FORMATING);
			pthread_t threadid = 0;
		    s32Ret = pthread_create(&threadid, NULL, (void *)ShowSuccess, NULL);
			if(0 != s32Ret)
		    {
		    	MLOGE("pthread_create ui_app_start Failed, s32Ret=%#x\n", s32Ret);
		        return HI_FAILURE;
		    }
            //s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
            //s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
        }
        else
        {
            //s32Ret |= HI_GV_Widget_Active(SD_SET);
            s32Ret |= HI_GV_Widget_Hide(SD_FORMATING);
        }
    }
    else
    {
        s32Ret |= HI_GV_Widget_Show(SD_FORMAT);
        s32Ret |= HI_GV_Widget_Active(SD_FORMAT);
        s32Ret |= HI_GV_Widget_Active(SD_FORMAT_ASK_BUTTON_cancel);
        s32Ret |= HI_GV_Widget_SetTextByID(SD_FORMAT_ASK_BUTTON_format, ID_STR_RETRY);
        s32Ret |= HI_GV_Widget_Paint(SD_FORMAT, 0);
        s32Ret |= HI_GV_Widget_Hide(SD_FORMATING);
    }

    if (s32Ret != HI_SUCCESS)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_STOP;
}

HI_S32 RESET_HARDWARE_WIFI_onrefresh(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	HI_S32 s32Ret = HI_SUCCESS;
	
	hi_usleep(1000 * 1000);
	s32Ret |= HI_GV_Widget_Hide(RESET_HARDWARE_WIFI);

	if (s32Ret != HI_SUCCESS)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

	return HIGV_PROC_STOP;
}

HI_S32 RESET_HARDWARE_BLUETOOTH_onrefresh(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	HI_S32 s32Ret = HI_SUCCESS;
	
	hi_usleep(1000 * 1000);
	s32Ret |= HI_GV_Widget_Hide(RESET_HARDWARE_BLUETOOTH);

	if (s32Ret != HI_SUCCESS)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

	return HIGV_PROC_STOP;
}


HI_S32 ALARM_WINDOW_onevent(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bIsShow = HI_FALSE;
    static HI_HANDLE s32ActWindow = INVALID_HANDLE;
    HI_U32 s32AlarmInfoId;

    HI_PDT_EVENT_E enEvt = (HI_PDT_EVENT_E)wParam;
    //MLOGD("EVENT: %#X\n", enEvt);

    if (enEvt == PDT_EVT_SD_MOUNT_ON)
    {
        bIsShow = HI_GV_Widget_IsShow(ALARM_WINDOW);

        if (bIsShow)
        {
            s32Ret |= HI_GV_Widget_GetTextID(ALARM_WINDOW_label_info, &s32AlarmInfoId);

            if (ID_STR_ALARM_SDNOEXIST == s32AlarmInfoId)
            {
                s32Ret |= HI_GV_Widget_Hide(ALARM_WINDOW);
                s32Ret |= HI_GV_Widget_Active(g_s32ActWindow);
            }
        }
    }
    else if( enEvt == PDT_EVT_SD_CHECKEND )
    {
        bIsShow = HI_GV_Widget_IsShow(ALARM_WINDOW);

        if (bIsShow)
        {
            s32Ret |= HI_GV_Widget_GetTextID(ALARM_WINDOW_label_info, &s32AlarmInfoId);

            if (ID_STR_SD_CHECK == s32AlarmInfoId)
            {
                s32Ret |= HI_GV_Widget_Hide(ALARM_WINDOW);
                s32Ret |= HI_GV_Widget_Active(g_s32ActWindow);
            }
        }
    }
    else if (enEvt == PDT_EVT_ALARM_HIDE)
    {
        bIsShow = HI_GV_Widget_IsShow(ALARM_WINDOW);

        if (bIsShow)
        {
            s32Ret |= HI_GV_Widget_Hide(ALARM_WINDOW);
            s32Ret |= HI_GV_Widget_Active(g_s32ActWindow);
        }
    }
    else
    {
        HI_UI_ALARM_TYPE_E  enAlarmType = (HI_UI_ALARM_TYPE_E)lParam;

        switch (enAlarmType)
        {
            case ALARM_GETSDSTATE_FAIL:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_GETSDSTATE_FAIL);
                break;

            case ALARM_GETFILEINFO_FAIL:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_GETFILEINFO_FAIL);
                break;

            case ALARM_SD_NOEXIST:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_SDNOEXIST);
                break;
			case ALARM_SD_NOMEDIAFILES:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_NOMEDIAFILES);
                break;

            case ALARM_SD_ERR:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_SDERR);
                break;

            case ALARM_SD_FULL:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_SDFULL);
                break;

            case ALARM_REC_SPACEFULL:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_RECORDSPACE_FULL);
                break;

            case ALARM_REC_ERR:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_REC_ERR);
                break;

            case ALARM_SNAP_SPACEFULL:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_SNAPSPACE_FULL);
                break;

            case ALARM_SNAP_ERR:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_SNAP_ERR);
                break;

            case ALARM_REC_LOOPFAIL:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_REC_LOOPFAIL);
                break;

            case ALARM_USB_CONNECTED:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_USB_CONNECTED);
                //s32Ret |= HI_GV_Widget_Show(PREVIEW_PAGE_GROUPBOX_STATE_lable_usb);
                break;

            case ALARM_USB_DISCONNECTED:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_USB_DISCONNECTED);
                //s32Ret |= HI_GV_Widget_Hide(PREVIEW_PAGE_GROUPBOX_STATE_lable_usb);
                break;

            case ALARM_CHIPTEMP:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_CHIPTEMP);
                break;

            case ALARM_CHIPTEMP_SHUTDOWN:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_CHIPTEMP_SHUTDOWN);
                break;

            case ALARM_PWRTEMP:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_PWRTEMP);
                break;

            case ALARM_PWRTEMP_SHUTDOWN:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_PWRTEMP_SHUTDOWN);
                break;

            case ALARM_PWRCAPACITY:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_PWRCAPACITY);
                break;

            case ALARM_PWRCAPACITY_SHUTDOWN:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_PWRCAPACITY_SHUTDOWN);
                break;

            case ALARM_GETCHANNELSTATE_FAIL:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_GETCHANNELSTATE_FAIL);
                break;

            case ALARM_RECCHANNEL_BUSY:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_RECCHANNEL_BUSY);
                break;

            case ALARM_STARTCHANNEL_FAIL:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_STARTCHANNEL_FAIL);
                break;

            case ALARM_STOPCHANNEL_FAIL:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_STOPCHANNEL_FAIL);
                break;

            case ALARM_SNAPPARAM_ERR:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_SNAPPARAM_ERR);
                break;

            case ALARM_NO_MORE_FILE:
                s32Ret |= HI_GV_Widget_SetText(ALARM_WINDOW_label_info, "No file left");
                break;

            case ALARM_DOING_FSCK:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_SD_CHECK);
                break;

            case ALARM_SD_PREPARING:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_ALARM_SD_PREPARING);
                break;

            case ALARM_DISON_LDCNEEDOFF:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_DISON_LDCNEEDOFF);
                break;

            case ALARM_LDCON_DISNEEDOFF:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_LDCON_DISNEEDOFF);
                break;
            case ALARM_SD_SPEEDLOW:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_SD_SPEEDLOW);
                break;
            case ALARM_SD_NEED_FORMAT:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_SD_NEED_FORMAT);
                break;
			case ALARM_AC_FULL:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_AC_FULL);
                break;
            case ALARM_DO_NOT_PULL_OUT_BATTERY:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_NOT_PULL_OUT_BATTERY);
                break;                
            case ALARM_DO_NOT_PULL_OUT_AC:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_NOT_PULL_OUT_AC);
                break;                
            case ALARM_LOW_POWER:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_LOW_POWER);
                break;             
            case ALARM_START_UPWARE:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_START_UPWARE);
                break;       
            case ALARM_CHECK_FILE_ERR:
                s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_FILE_ERR);
                break; 	
			case ALARM_PHOTO_DISABLE_ZOOM:
			    s32Ret |= HI_GV_Widget_SetTextByID(ALARM_WINDOW_label_info, ID_STR_PHOTO_MODE_DISABLE_ZOOM);
                break; 		
            default:
                break;
        }

        s32Ret = HI_GV_Widget_GetActiveWidget(HIGV_WND_MANAGER_HANDLE, &s32ActWindow);
        if(ALARM_WINDOW == s32ActWindow)
        {
//            MLOGD("g_s32ActWindow=%d\n",g_s32ActWindow);
            s32Ret |= HI_GV_Widget_Show(ALARM_WINDOW);
            s32Ret |= HI_GV_Widget_Active(ALARM_WINDOW);
            s32Ret |= HI_GV_Widget_Paint(ALARM_WINDOW, 0);
        }
        else if(SD_FORMAT == s32ActWindow || SD_FORMATING == s32ActWindow)
        {
            MLOGD("g_s32ActWindow=%d\n",g_s32ActWindow);
            if(g_bNormalFormat)
            {
                g_s32ActWindow = FEIYU_GLOBAL_SETTINGS;
            }
            s32Ret |= HI_GV_Widget_Show(ALARM_WINDOW);
            s32Ret |= HI_GV_Widget_Active(ALARM_WINDOW);
            s32Ret |= HI_GV_Widget_Hide(SD_FORMAT);
            s32Ret |= HI_GV_Widget_Hide(SD_FORMATING);
        }
        else
        {
            g_s32ActWindow = s32ActWindow;
            MLOGD("g_s32ActWindow=%d\n",g_s32ActWindow);
            s32Ret |= HI_GV_Widget_Show(ALARM_WINDOW);
            s32Ret |= HI_GV_Widget_Active(ALARM_WINDOW);
        }


    }


    return HIGV_PROC_STOP;
}
HI_S32 USB_CONNECTING_onevent(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (HI_TRUE == lParam)
    {
        s32Ret |= HI_GV_Widget_Show(USB_CONNECTING);
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(USB_CONNECTING);
    }

    return HIGV_PROC_GOON;
}
HI_S32 TIPS_WINDOW_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bAPState = HI_FALSE;

    if (HIGV_KEY_F2 == wParam)
    {
        bAPState = (HI_BOOL)lParam;

        if (bAPState)
        {
            s32Ret |= HI_GV_Widget_SetTextByID(TIPS_WINDOW_label, ID_STR_WIFI_OFF_ERR);
        }
        else
        {
            s32Ret |= HI_GV_Widget_SetTextByID(TIPS_WINDOW_label, ID_STR_WIFI_ON_ERR);
        }

        s32Ret |= HI_GV_Widget_Paint(TIPS_WINDOW, 0);
    }
    else if (HIGV_KEY_F1 == wParam)
    {
        s32Ret |= HI_GV_Widget_Hide(TIPS_WINDOW);
    }

    return HIGV_PROC_GOON;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

