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
#include "fy_user_param.h"
#include "hi_product_timedtask.h"

#include "feiyu_dv_cmd_proc.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

/*	PAGE GUIDER
 *
	0.CHOOSE LANGUAGE
	1.WELECOM 
	2.CLICK FILELIST
	3.CLICK GLOBAL SET
	4.LONGTAP METRY ZOOM
	5.SLIDE UP
	6.SLIDE DOWN  
	7.SLIDE LEFT 
	8.SLIDE RIGHT
	9.DOWNLOAD APP
	10.END
 */

static HI_U8 s_currentStep = 0;
extern USER_PARA_S* g_pstUserPara;
static HI_S32 s32ChooseLanguage = -1;

HI_VOID FEIYU_USER_GUIDER_ChangePage();
HI_VOID FEIYU_USER_GUIDER_Back_ChangePage();


HI_S32 FEIYU_USER_GUIDER_ongesturefling(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{

    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;

    MLOGD("\n");
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
    MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);

    if (abs(startX - endX) > abs(startY - endY))// left or right
    {
        if (startX > endX)// to left
        {
            FEIYU_USER_GUIDER_ChangePage();
        }
		else
		{
			FEIYU_USER_GUIDER_Back_ChangePage();
		}
    }

}

HI_S32 FEIYU_USER_GUIDER_OnKeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{

    HI_S32 s32Ret = HI_SUCCESS;

    if(10 == s_currentStep)
    {
        HI_PDT_WORKMODE_S stWorkMode = {};
        HI_PDT_WorkMode_GetWorkMode(&stWorkMode);
        if(PDT_WORKMODE_PLAYBACK!=stWorkMode.enWorkMode)
        {
            s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
            s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER);
            s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
        }
        else
        {
            s32Ret |= HI_GV_Widget_Show(FILELIST_PAGE);
            s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER);            
            s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE);
        }             
    }

    if (HIGV_KEY_F2 == wParam || HIGV_KEY_F1 == wParam) //mode key
    {
        FEIYU_USER_GUIDER_ChangePage();
    }
    else
    {
        MLOGE("KEY CODE %#x not processed\n", wParam);
    }

    return HIGV_PROC_GOON;
}

HI_VOID showSelectLanguagePage(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS; 

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SCROLLVIEW); 
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_LINE); 
		s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_CHOOSE_LANGUAGE);
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_SCROLLVIEW); 
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_LINE);
		s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_CHOOSE_LANGUAGE);
		goto End;
    }

	if(s32ChooseLanguage != -1)
	{
		 s32Ret |= HI_GV_Widget_SetSkin(s32ChooseLanguage, HIGV_SKIN_NORMAL, background_skin_blue);
		 s32Ret |= HI_GV_Widget_SetSkin(s32ChooseLanguage, HIGV_SKIN_ACITVE, background_skin_blue);
	}
	else
	{
		 s32Ret |= HI_GV_Widget_SetTextByID(FEIYU_USER_GUIDER_SKIP, ID_STR_SKIP);
	}
	
End:	
	
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }

	return HIGV_PROC_GOON;
}

HI_S32 FEIYU_USER_GUIDER_BUTTON_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("\n");
	if(HI_TRUE == HI_GV_Widget_IsActive(FEIYU_USER_GUIDER_LABEL_NULL))
		return;

	s32ChooseLanguage = hWidget;
    HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_CHINESE, HIGV_SKIN_NORMAL, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_CHINESE, HIGV_SKIN_ACITVE, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_ENGLISH, HIGV_SKIN_NORMAL, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_ENGLISH, HIGV_SKIN_ACITVE, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_FRENCH, HIGV_SKIN_NORMAL, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_FRENCH, HIGV_SKIN_ACITVE, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_GERMAN, HIGV_SKIN_NORMAL, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_GERMAN, HIGV_SKIN_ACITVE, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_RUSSIAN, HIGV_SKIN_NORMAL, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_RUSSIAN, HIGV_SKIN_ACITVE, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_SPANISH, HIGV_SKIN_NORMAL, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_SPANISH, HIGV_SKIN_ACITVE, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_ARABIC, HIGV_SKIN_NORMAL, background_skin);
	HI_GV_Widget_SetSkin(FEIYU_USER_GUIDER_BUTTON_ARABIC, HIGV_SKIN_ACITVE, background_skin);
    switch(hWidget)
    {
        case FEIYU_USER_GUIDER_BUTTON_CHINESE:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_CHINESE,NULL);            
            break;
        case FEIYU_USER_GUIDER_BUTTON_ENGLISH:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_ENGLISH,NULL);            
            break;
        case FEIYU_USER_GUIDER_BUTTON_FRENCH:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_FRENCH,NULL);            
            break;
        case FEIYU_USER_GUIDER_BUTTON_GERMAN:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_GERMAN,NULL);            
            break;
        case FEIYU_USER_GUIDER_BUTTON_RUSSIAN:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_RUSSIAN,NULL);            
            break;
        case FEIYU_USER_GUIDER_BUTTON_SPANISH:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_SPANISH,NULL);            
            break;
        case FEIYU_USER_GUIDER_BUTTON_ARABIC:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_ARABIC,NULL);            
            break;
        default:
            break;            
    }	
	HI_GV_Widget_SetTextByID(FEIYU_USER_GUIDER_SKIP, ID_STR_NEXT);
	HI_GV_Widget_SetSkin(hWidget, HIGV_SKIN_NORMAL, background_skin_blue);
	HI_GV_Widget_SetSkin(hWidget, HIGV_SKIN_ACITVE, background_skin_blue);
	HI_GV_Widget_Paint(FEIYU_USER_GUIDER_SCROLLVIEW, 0);
    HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);

    return HIGV_PROC_STOP;    
}

HI_VOID showWelcomPage(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
    	if(s_currentStep == 1)
    	{
    		s32Ret |= HI_GV_Widget_SetTextByID(FEIYU_USER_GUIDER_WELCOM, ID_STR_GUIDER_OK);  
        	s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_WELCOM);   
			s32Ret |= HI_GV_Widget_SetTextByID(FEIYU_USER_GUIDER_SKIP, ID_STR_NEXT);
			s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SKIP);
    	}
		else if(s_currentStep == 10)
		{
    		s32Ret |= HI_GV_Widget_SetTextByID(FEIYU_USER_GUIDER_WELCOM, ID_STR_WELCOM);  
        	s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_WELCOM);  
			s32Ret |= HI_GV_Widget_SetTextByID(FEIYU_USER_GUIDER_SKIP, ID_STR_END); 
			s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SKIP);
    	}
    }
    else
    {     
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_WELCOM);               
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
 
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }      
}

HI_VOID showFilelistPage(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
    	s32Ret |= HI_GV_Widget_SetTextByID(FEIYU_USER_GUIDER_CLICK_PROMPT, ID_STR_SELECT_FILELIST); 
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_CLICK_PROMPT);               
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_CLICK_FILELIST_PAGE);               
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_CLICK_PROMPT);               
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_CLICK_FILELIST_PAGE);                 
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);

     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_VOID showGlobalsetPage(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_SetTextByID(FEIYU_USER_GUIDER_CLICK_PROMPT, ID_STR_CLICK_GLOBAL_SET);   
		s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_CLICK_PROMPT);                
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_CLICK_GLOBAL_SET_PAGE);                     
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_CLICK_PROMPT);               
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_CLICK_GLOBAL_SET_PAGE);                       
    }
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
 
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_VOID showMetryZoomPage(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {  
		s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_LONGTAP_PROMPT);               
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_LONGTAP_METRY_ZOOM_PAGE);                    
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_LONGTAP_PROMPT);               
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_LONGTAP_METRY_ZOOM_PAGE);                        
    }
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
  
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_VOID showAppDownPage(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_APP_DOWNLOAD);               
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_ANDROID_CLIENT_LABEL); 
		s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_ANDROID_QR); 
		s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_IPHONE_CLIENT_LABEL); 
		s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_IPHONE_QR); 
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_APP_DOWNLOAD);               
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_ANDROID_CLIENT_LABEL); 
		s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_ANDROID_QR); 
		s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_IPHONE_CLIENT_LABEL); 
		s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_IPHONE_QR);                       
    }
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_VOID showSlideUpPage(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SLIDE_UP_PAGE);               
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SLIDE_UP_LABEL); 
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_SLIDE_UP_PAGE);               
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_SLIDE_UP_LABEL);                       
    }
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_VOID showSlideDownPage(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SLIDE_DOWN_PAGE);               
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SLIDE_DOWN_LABEL); 
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_SLIDE_DOWN_PAGE);               
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_SLIDE_DOWN_LABEL);                       
    }
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}


HI_VOID showSlideLeftPage(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SLIDE_LEFT_PAGE);               
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SLIDE_LEFT_LABEL); 
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_SLIDE_LEFT_PAGE);               
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_SLIDE_LEFT_LABEL);                       
    }
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_VOID showSlideRightPage(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SLIDE_RIGHT_PAGE);               
        s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SLIDE_RIGHT_LABEL); 
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_SLIDE_RIGHT_PAGE);               
        s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER_SLIDE_RIGHT_LABEL);                       
    }
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}



HI_VOID FEIYU_USER_GUIDER_ChangePage()
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(0 == s_currentStep)
    {
        s_currentStep = 1;
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "02/11");
        showSelectLanguagePage(HI_FALSE);
        showWelcomPage(HI_TRUE);
    }
    else if(1 == s_currentStep)
    {
        s_currentStep = 2;
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "03/11");        
        showWelcomPage(HI_FALSE);
        showFilelistPage(HI_TRUE);
    }
    else if(2 == s_currentStep)
    {
        s_currentStep = 3;
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "04/11");           
        showFilelistPage(HI_FALSE);
        showGlobalsetPage(HI_TRUE);
    }
    else if(3 == s_currentStep)
    {
        s_currentStep = 4;         
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "05/11");    
        showGlobalsetPage(HI_FALSE);
        showMetryZoomPage(HI_TRUE);           
    }
    else if(4 == s_currentStep)
    {
        s_currentStep = 5;         
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "06/11");        
        showMetryZoomPage(HI_FALSE);
        showSlideUpPage(HI_TRUE);
    }
    else if(5 == s_currentStep)
    {
        s_currentStep = 6;                 
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "07/11");                                        
        showSlideUpPage(HI_FALSE);
        showSlideDownPage(HI_TRUE);            
    }
    else if(6 == s_currentStep)
    {
        s_currentStep = 7;                 
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "08/11");                                                
        showSlideDownPage(HI_FALSE);  
        showSlideLeftPage(HI_TRUE);
    }
    else if(7 == s_currentStep)
    {
        s_currentStep = 8;   
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "09/11");
        showSlideLeftPage(HI_FALSE);
        showSlideRightPage(HI_TRUE);  
    }
	else if(8 == s_currentStep)
    {
        s_currentStep = 9;   
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "10/11");
        showSlideRightPage(HI_FALSE);
        showAppDownPage(HI_TRUE);  
    }
	else if(9 == s_currentStep)
    {
        s_currentStep = 10;   
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "11/11");
        showAppDownPage(HI_FALSE);
        showWelcomPage(HI_TRUE);  
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_VOID FEIYU_USER_GUIDER_Back_ChangePage()
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(10 == s_currentStep)
    {
        s_currentStep = 9;
		s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "10/11");
		s32Ret |= HI_GV_Widget_SetTextByID(FEIYU_USER_GUIDER_SKIP, ID_STR_NEXT); 
		s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SKIP);
		showWelcomPage(HI_FALSE);
        showAppDownPage(HI_TRUE);
    }
    else if(9 == s_currentStep)
    {
        s_currentStep = 8;
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "09/11");        
        showAppDownPage(HI_FALSE);
        showSlideRightPage(HI_TRUE);
    }
    else if(8 == s_currentStep)
    {
        s_currentStep = 7;
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "08/11");            
        showSlideRightPage(HI_FALSE);
        showSlideLeftPage(HI_TRUE);
    }
    else if(7 == s_currentStep)
    {
        s_currentStep = 6;         
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "07/11");    
        showSlideLeftPage(HI_FALSE);
        showSlideDownPage(HI_TRUE);           
    }
    else if(6 == s_currentStep)
    {
        s_currentStep = 5;         
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "06/11");        
        showSlideDownPage(HI_FALSE);
        showSlideUpPage(HI_TRUE);
    }
    else if(5 == s_currentStep)
    {
        s_currentStep = 4;                 
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "05/11");                                        
        showSlideUpPage(HI_FALSE);
        showMetryZoomPage(HI_TRUE);            
    }
    else if(4 == s_currentStep)
    {
        s_currentStep = 3;                 
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "04/11");                                                
        showMetryZoomPage(HI_FALSE);  
        showGlobalsetPage(HI_TRUE);
    }
    else if(3 == s_currentStep)
    {
        s_currentStep = 2;   
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "03/11");
        showGlobalsetPage(HI_FALSE);
        showFilelistPage(HI_TRUE);  
    }
	else if(2 == s_currentStep)
    {
        s_currentStep = 1;   
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "02/11");
        showFilelistPage(HI_FALSE);
        showWelcomPage(HI_TRUE);  
    }
	else if(1 == s_currentStep)
    {
        s_currentStep = 0;   
        s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "01/11");
        showWelcomPage(HI_FALSE);
        showSelectLanguagePage(HI_TRUE);  
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}


HI_S32 FEIYU_USER_GUIDER_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(10 == s_currentStep)
    {
        HI_PDT_WORKMODE_S stWorkMode = {};
        HI_PDT_WorkMode_GetWorkMode(&stWorkMode);
        if(PDT_WORKMODE_PLAYBACK!=stWorkMode.enWorkMode)
        {
            s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
            s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER);
            s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
        }
        else
        {
            s32Ret |= HI_GV_Widget_Show(FILELIST_PAGE);
            s32Ret |= HI_GV_Widget_Hide(FEIYU_USER_GUIDER);            
            s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE);
        }             
    }
    else
    {
        HIGV_GESTURE_EVENT_S gestureEvent;
        HI_S32 x, y;
        memset(&gestureEvent, 0x0, sizeof(gestureEvent));
        memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
        x = gestureEvent.gesture.tap.pointer.x;
        y = gestureEvent.gesture.tap.pointer.y;  

        if(x > 340 && y > 200)
        {
			HIGV_GESTURE_EVENT_S gestureNewEvent = {0};
			memset(&gestureNewEvent, 0x0, sizeof(gestureNewEvent));
			gestureNewEvent.gesture.fling.start.x = 480;
			gestureNewEvent.gesture.fling.start.y = 0;
			gestureNewEvent.gesture.fling.end.x = 470;
			gestureNewEvent.gesture.fling.end.y = 0;
        	
			HI_GV_Msg_SendAsyncWithData(FEIYU_USER_GUIDER, HIGV_MSG_GESTURE_FLING, 
				&gestureNewEvent, sizeof(HIGV_GESTURE_EVENT_S));
            return HIGV_PROC_STOP;
        }
    }
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }    
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_USER_GUIDER_onhide(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    MLOGD("~~~~\n");
    HI_S32 s32Ret = HI_SUCCESS;
    g_pstUserPara->commPara.u8ShowStartUpGuider = 1;
    extern HI_S32 fy_User_SaveParam(HI_U8 enSaveFlag);
    s32Ret |= fy_User_SaveParam(HI_TRUE);
    if(HI_SUCCESS != s32Ret)
	{
		MLOGE("err: %d\n", s32Ret);
    }    
    HI_PDT_TimedTask_SetEnable(PDT_TIMEDTASK_TYPE_AUTOOFF, g_pstPDTCfg->stDevMngParam.stAutoOffAttr.bEnable);
    HI_PDT_TimedTask_SetEnable(PDT_TIMEDTASK_TYPE_DORMANT, g_pstPDTCfg->stDevMngParam.stDormantAttr.bEnable);        
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_USER_GUIDER_onshow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_PDT_TimedTask_SetEnable(PDT_TIMEDTASK_TYPE_AUTOOFF, HI_FALSE);
    HI_PDT_TimedTask_SetEnable(PDT_TIMEDTASK_TYPE_DORMANT, HI_FALSE);    
    s_currentStep = 0;
    showSelectLanguagePage(HI_TRUE);
	showWelcomPage(HI_FALSE);
    showFilelistPage(HI_FALSE);
	showGlobalsetPage(HI_FALSE);
	showMetryZoomPage(HI_FALSE);
	showAppDownPage(HI_FALSE);
    showSlideLeftPage(HI_FALSE); 
	showSlideRightPage(HI_FALSE);
	showSlideUpPage(HI_FALSE);
	showSlideDownPage(HI_FALSE);
    s32Ret |= HI_GV_Widget_SetText(FEIYU_USER_GUIDER_PAGE, "01/11"); 
    s32Ret |= HI_GV_Widget_SetTextByID(FEIYU_USER_GUIDER_SKIP, ID_STR_SKIP);
    s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_SKIP);
	s32Ret |= HI_GV_Widget_Show(FEIYU_USER_GUIDER_PAGE);
    s32Ret |= HI_GV_Widget_Paint(FEIYU_USER_GUIDER, 0);
	
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }
    return HIGV_PROC_GOON;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

