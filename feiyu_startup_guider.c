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

#include "feiyu_dv_cmd_proc.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */
/*
0.mode
1.globalset
2.filelist
3.left
4.right
5.down
6.DOWNLOAD APP
7.LANGUAGE
8.WELECOM

*/
/*
7.LANGUAGE
0.mode
1.globalset
2.filelist
3.left
4.right
5.down
6.DOWNLOAD APP

8.WELECOM

*/

static HI_U8 s_currentStep = 0;
extern USER_PARA_S* g_pstUserPara;

HI_S32 FEIYU_STARTUP_GUIDER_ongesturefling(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
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
            changePage();
        }

    }

}

HI_S32 FEIYU_STARTUP_GUIDER_OnKeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{

    HI_S32 s32Ret = HI_SUCCESS;

    if(8 == s_currentStep)
    {
        HI_PDT_WORKMODE_S stWorkMode = {};
        HI_PDT_WorkMode_GetWorkMode(&stWorkMode);
        if(PDT_WORKMODE_PLAYBACK!=stWorkMode.enWorkMode)
        {
            s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
            s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER);
            s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
        }
        else
        {
            s32Ret |= HI_GV_Widget_Show(FILELIST_PAGE);
            s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER);            
            s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE);
        }             
    }

    if (HIGV_KEY_F2 == wParam || HIGV_KEY_F1 == wParam) //mode key
    {
        changePage();
    }
    else
    {
        MLOGE("KEY CODE %#x not processed\n", wParam);
    }

    return HIGV_PROC_GOON;
}


HI_VOID showGlobalset(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FEIYU_STARTUP_GUIDER_select_globalsetting);               
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_set);       
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_set_pic);               
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_select_globalsetting);               
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_set);       
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_set_pic);                 
    }
    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);

     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_VOID showModeSelect(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_mode_pic);               
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_mode);       
        s32Ret |= HI_GV_Widget_Show(FEIYU_STARTUP_GUIDER_select_mode);     
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_select_mode);
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_mode_pic);                       
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_mode);                               

    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);

     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_VOID showFilelist(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_file_1);               
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_file_2);       
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_file_3);     
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_file_4);          
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_file_1);               
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_file_2);       
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_file_3);     
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_file_4);           
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);

     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_VOID showleft(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_SetSkin(FFEIYU_guider_left, HIGV_SKIN_NORMAL, guider_left);
        s32Ret |= HI_GV_Widget_SetTextByID(FFEIYU_guider_left_1, ID_STR_SLIDE_LEFT);        
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_left);               
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_left_1);       
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left_1, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left_1, 0);        
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_left);               
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_left_1);      
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left_1, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left_1, 0);         
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);

     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }      
}

HI_VOID showRigth(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_SetSkin(FFEIYU_guider_left, HIGV_SKIN_NORMAL, guider_right);
        s32Ret |= HI_GV_Widget_SetTextByID(FFEIYU_guider_left_1, ID_STR_SLIDE_RIGHT);            
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_left);               
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_left_1);       
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left_1, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left_1, 0);         
    }
    else
    {
        s32Ret |= HI_GV_Widget_SetSkin(FFEIYU_guider_left, HIGV_SKIN_NORMAL, guider_right);
        s32Ret |= HI_GV_Widget_SetTextByID(FFEIYU_guider_left_1, ID_STR_SLIDE_RIGHT);            
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_left);               
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_left_1);       
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left_1, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left_1, 0);         
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);

     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }      
}

HI_VOID showDown(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_SetSkin(FFEIYU_guider_left, HIGV_SKIN_NORMAL, guider_down);
        s32Ret |= HI_GV_Widget_SetTextByID(FFEIYU_guider_left_1, ID_STR_SLIDE_DOWN);            
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_left);               
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_left_1);   
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left_1, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left_1, 0);         
    }
    else
    {
        s32Ret |= HI_GV_Widget_SetSkin(FFEIYU_guider_left, HIGV_SKIN_NORMAL, guider_down);
        s32Ret |= HI_GV_Widget_SetTextByID(FFEIYU_guider_left_1, ID_STR_SLIDE_DOWN);            
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_left);               
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_left_1);   
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left, 0);
        s32Ret |= HI_GV_Widget_Update(FFEIYU_guider_left_1, 0);
        s32Ret |= HI_GV_Widget_Refresh(FFEIYU_guider_left_1, 0);         
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);

     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }      
}

HI_VOID showDownApp(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FEIYU_STARTUP_GUIDER_download_app);               
    }
    else
    {     
        s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_download_app);               
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);

     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }      
}

HI_VOID showWelcom(HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;   

    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_welcom);               
    }
    else
    {     
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_welcom);               
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);

     
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }      
}


HI_VOID showSelectLanguage(HI_BOOL bFlag)
{
    showModeSelect(HI_FALSE);
    showGlobalset(HI_FALSE);
    HI_S32 s32Ret = HI_SUCCESS;    
    s32Ret |= HI_GV_Widget_Highlight(FEIYU_guider_button_1, HI_FALSE);
    s32Ret |= HI_GV_Widget_Highlight(FEIYU_guider_button_2, HI_FALSE);
    s32Ret |= HI_GV_Widget_Highlight(FEIYU_guider_button_3, HI_FALSE);
    s32Ret |= HI_GV_Widget_Highlight(FEIYU_guider_button_4, HI_FALSE);
    s32Ret |= HI_GV_Widget_Highlight(FEIYU_guider_button_5, HI_FALSE);
    s32Ret |= HI_GV_Widget_Highlight(FEIYU_guider_button_6, HI_FALSE);    
    s32Ret |= HI_GV_Widget_Highlight(FEIYU_guider_button_7, HI_FALSE);    
    if(bFlag)
    {
        s32Ret |= HI_GV_Widget_Show(FEIYU_guider_scrollview);               
        s32Ret |= HI_GV_Widget_Show(FEIYU_guider_scrollbar);       
        s32Ret |= HI_GV_Widget_Show(FFEIYU_guider_scrollbar_select_language);                       
        
    }
    else
    {
        s32Ret |= HI_GV_Widget_Hide(FEIYU_guider_scrollview);                       
        s32Ret |= HI_GV_Widget_Hide(FEIYU_guider_scrollbar);                               
        s32Ret |= HI_GV_Widget_Hide(FFEIYU_guider_scrollbar_select_language);                               
    }

    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }     
}
HI_S32 FEIYU_guider_button_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("\n");
    showSelectLanguage(HI_TRUE);
    switch(hWidget)
    {
        case FEIYU_guider_button_1:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_CHINESE,NULL);
            
            break;
        case FEIYU_guider_button_2:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_ENGLISH,NULL);            
            break;
        case FEIYU_guider_button_3:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_FRENCH,NULL);            
            break;
        case FEIYU_guider_button_4:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_GERMAN,NULL);            
            break;
        case FEIYU_guider_button_5:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_RUSSIAN,NULL);            
            break;
        case FEIYU_guider_button_6:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_SPANISH,NULL);            
            break;
        case FEIYU_guider_button_7:
            FeiYuCmdExecute(CMD_SET_LANGUAGE_TYPE,LANGUAGE_ARABIC,NULL);            
            break;
        default:
            break;            
    }
    HI_GV_Widget_Highlight(hWidget, HI_TRUE);

    return HIGV_PROC_GOON;    
}




HI_S32 FEIYU_STARTUP_GUIDER_last_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    if(8 == s_currentStep)
    {
        s_currentStep = 7;
        s32Ret |= HI_GV_Widget_Show(ID_STR_SELECT_LANGUAGE);
        s32Ret |= HI_GV_Widget_Show(FEIYU_STARTUP_GUIDER_next);
        s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_last); 
        s32Ret |= HI_GV_Widget_Hide(ID_STR_GUIDER_OK);       
        return HIGV_PROC_STOP;
    }
    return HIGV_PROC_GOON; 
}


HI_VOID changePage()
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(0 == s_currentStep)
    {
        s_currentStep = 1;
        s32Ret |= HI_GV_Widget_SetText(FEIYU_STARTUP_GUIDER_num, "2/9");
        showSelectLanguage(HI_FALSE);
        showModeSelect(HI_TRUE);
    }
    else if(1 == s_currentStep)
    {
        s_currentStep = 2;
        s32Ret |= HI_GV_Widget_SetText(FEIYU_STARTUP_GUIDER_num, "3/9");        
        showModeSelect(HI_FALSE);
        showGlobalset(HI_TRUE);

    }
    else if(2 == s_currentStep)
    {
        s_currentStep = 3;
        s32Ret |= HI_GV_Widget_SetText(FEIYU_STARTUP_GUIDER_num, "4/9");           
        showGlobalset(HI_FALSE);
        showFilelist(HI_TRUE);
    }
    else if(3 == s_currentStep)
    {
        s_currentStep = 4;         
        s32Ret |= HI_GV_Widget_SetText(FEIYU_STARTUP_GUIDER_num, "5/9");    
        showFilelist(HI_FALSE);
        showleft(HI_TRUE);           
    }
    else if(4 == s_currentStep)
    {
        s_currentStep = 5;         
        s32Ret |= HI_GV_Widget_SetText(FEIYU_STARTUP_GUIDER_num, "6/9");        
        showleft(HI_FALSE);
        showRigth(HI_TRUE);
    }
    else if(5 == s_currentStep)
    {
        s_currentStep = 6;                 
        s32Ret |= HI_GV_Widget_SetText(FEIYU_STARTUP_GUIDER_num, "7/9");                                        
        showRigth(HI_FALSE);
        showDown(HI_TRUE);            
    }
    else if(6 == s_currentStep)
    {
        s_currentStep = 7;                 
        s32Ret |= HI_GV_Widget_SetText(FEIYU_STARTUP_GUIDER_num, "8/9");                                                
        showDown(HI_FALSE);  
        showDownApp(HI_TRUE);
        
    }
    else if(7 == s_currentStep)
    {
        s_currentStep = 8;   
        s32Ret |= HI_GV_Widget_SetText(FEIYU_STARTUP_GUIDER_num, "9/9");
        showDownApp(HI_FALSE);
        showWelcom(HI_TRUE);
        
    }
    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }        
}

HI_S32 FEIYU_STARTUP_GUIDER_next_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    //changePage();

    return HIGV_PROC_GOON;    
}

HI_S32 FEIYU_STARTUP_GUIDER_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(8 == s_currentStep)
    {
        HI_PDT_WORKMODE_S stWorkMode = {};
        HI_PDT_WorkMode_GetWorkMode(&stWorkMode);
        if(PDT_WORKMODE_PLAYBACK!=stWorkMode.enWorkMode)
        {
            s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
            s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER);
            s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
        }
        else
        {
            s32Ret |= HI_GV_Widget_Show(FILELIST_PAGE);
            s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER);            
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
        if(x > 300 && y < 100)
        {
            changePage();
        }
    }
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }    
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_STARTUP_GUIDER_onhide(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
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
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_STARTUP_GUIDER_onshow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    s_currentStep = 0;
    showSelectLanguage(HI_TRUE);
    showFilelist(HI_FALSE);
    showleft(HI_FALSE);
    showWelcom(HI_FALSE);
    s32Ret |= HI_GV_Widget_Show(FEIYU_STARTUP_GUIDER_select_language);
    s32Ret |= HI_GV_Widget_SetText(FEIYU_STARTUP_GUIDER_num, "1/9");
    s32Ret |= HI_GV_Widget_Show(FEIYU_STARTUP_GUIDER_num);
    s32Ret |= HI_GV_Widget_SetTextByID(FEIYU_STARTUP_GUIDER_next,ID_STR_SKIP);
    s32Ret |= HI_GV_Widget_Show(FEIYU_STARTUP_GUIDER_next);

    s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_select_mode);
    s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_select_language);
    s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_last);
    s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_select_globalsetting);    
    s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_select_filelist);        
    s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_download_app);            
    s32Ret |= HI_GV_Widget_Hide(FEIYU_STARTUP_GUIDER_slide);
    s32Ret |= HI_GV_Widget_Paint(FEIYU_STARTUP_GUIDER, 0);
    if( HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }
    return HIGV_PROC_GOON;
}



HI_S32 GUIDER_LANGUAGE_OnItemListener(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("~~~\n");

    return HIGV_PROC_GOON;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

