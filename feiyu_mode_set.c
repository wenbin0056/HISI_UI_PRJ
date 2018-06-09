
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
#include "fy_user_param.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

extern HI_BOOL g_pageChangeEnd;
extern SubPageItemDes sa_SubPageItemDes[MAX_SUBPAGE];
static HI_S32 s_current_subpage = SUBPAGE_PHOTO_PARAM_IMAGE_MODE;
static HI_S32 s_current_subpage_1 = -1;
static HI_BOOL s_bRunning = HI_FALSE;
static HI_BOOL s_bRunning1 = HI_FALSE;
static HI_S32 s_s32Item_0 = -1;
static HI_S32 s_s32Item_1 = -1;
static HI_S32 s_s32FlingLeftAdd = 0;
static HI_S32 s_s32FlingRigthAdd = 0;
static HI_BOOL s_timer_flag = HI_FALSE;
static HI_HANDLE s_s32Modehandle = 0;
static HI_S32 s_s32WhichView = 0;
static HI_BOOL s_bIsRunning_1 = HI_FALSE;
static HI_BOOL s_bIsRunning_2 = HI_FALSE;
static HI_BOOL s_bIsRunning_3 = HI_FALSE;
static HI_BOOL s_bIsFirstEntry_1 = HI_FALSE;
static HI_BOOL s_bIsFirstEntry_2 = HI_FALSE;
static HI_BOOL s_bSroll = HI_FALSE;
static HI_S32 s_s32CurrentWorkMode = 0;
static HI_S32 new_ItemNum = 0;
static HI_S32 s_s32Item = 0;
static HI_S32 s_s32SelectItem1 = -1;
static HI_S32 s_s32SelectItem2 = -1;
static HI_S32 prePage1tempX = -1;
static HI_S32 prePage2tempX = -1;
HI_S32 g_startY = 0;

#define MAX_ITEM 18
static HI_S32 ScrollView_1_5[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_1_5,
    FEIYU_MODE_SET_scrollview_1_5_1,
    FEIYU_MODE_SET_scrollview_1_5_2,
    FEIYU_MODE_SET_scrollview_1_5_3,
    FEIYU_MODE_SET_scrollview_1_5_4,
    FEIYU_MODE_SET_scrollview_1_5_5,    
};

static HI_S32 ScrollView_1_7[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_1_7,
    FEIYU_MODE_SET_scrollview_1_7_1,
    FEIYU_MODE_SET_scrollview_1_7_2,
    FEIYU_MODE_SET_scrollview_1_7_3,
    FEIYU_MODE_SET_scrollview_1_7_4,
    FEIYU_MODE_SET_scrollview_1_7_5,    
    FEIYU_MODE_SET_scrollview_1_7_6,
    FEIYU_MODE_SET_scrollview_1_7_7,
};


static HI_S32 ScrollView_1_8[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_1_8,
    FEIYU_MODE_SET_scrollview_1_8_1,
    FEIYU_MODE_SET_scrollview_1_8_2,
    FEIYU_MODE_SET_scrollview_1_8_3,
    FEIYU_MODE_SET_scrollview_1_8_4,
    FEIYU_MODE_SET_scrollview_1_8_5,    
    FEIYU_MODE_SET_scrollview_1_8_6,
    FEIYU_MODE_SET_scrollview_1_8_7,
    FEIYU_MODE_SET_scrollview_1_8_8, 
};


static HI_S32 ScrollView_1_10[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_1_10,
    FEIYU_MODE_SET_scrollview_1_10_1,
    FEIYU_MODE_SET_scrollview_1_10_2,
    FEIYU_MODE_SET_scrollview_1_10_3,
    FEIYU_MODE_SET_scrollview_1_10_4,
    FEIYU_MODE_SET_scrollview_1_10_5,    
    FEIYU_MODE_SET_scrollview_1_10_6,
    FEIYU_MODE_SET_scrollview_1_10_7,
    FEIYU_MODE_SET_scrollview_1_10_8,
    FEIYU_MODE_SET_scrollview_1_10_9,
    FEIYU_MODE_SET_scrollview_1_10_10,    
};

static HI_S32 ScrollView_1_11[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_1_11,
    FEIYU_MODE_SET_scrollview_1_11_1,
    FEIYU_MODE_SET_scrollview_1_11_2,
    FEIYU_MODE_SET_scrollview_1_11_3,
    FEIYU_MODE_SET_scrollview_1_11_4,
    FEIYU_MODE_SET_scrollview_1_11_5,    
    FEIYU_MODE_SET_scrollview_1_11_6,
    FEIYU_MODE_SET_scrollview_1_11_7,
    FEIYU_MODE_SET_scrollview_1_11_8,
    FEIYU_MODE_SET_scrollview_1_11_9,
    FEIYU_MODE_SET_scrollview_1_11_10,    
    FEIYU_MODE_SET_scrollview_1_11_11,           
};

static HI_S32 ScrollView_1_14[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_1_14,
    FEIYU_MODE_SET_scrollview_1_14_1,
    FEIYU_MODE_SET_scrollview_1_14_2,
    FEIYU_MODE_SET_scrollview_1_14_3,
    FEIYU_MODE_SET_scrollview_1_14_4,
    FEIYU_MODE_SET_scrollview_1_14_5,    
    FEIYU_MODE_SET_scrollview_1_14_6,
    FEIYU_MODE_SET_scrollview_1_14_7,
    FEIYU_MODE_SET_scrollview_1_14_8,
    FEIYU_MODE_SET_scrollview_1_14_9,
    FEIYU_MODE_SET_scrollview_1_14_10,    
    FEIYU_MODE_SET_scrollview_1_14_11,
    FEIYU_MODE_SET_scrollview_1_14_12,
    FEIYU_MODE_SET_scrollview_1_14_13,
    FEIYU_MODE_SET_scrollview_1_14_14,            
};

static HI_S32 ScrollView_1_18[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_1_18,
    FEIYU_MODE_SET_scrollview_1_18_1,
    FEIYU_MODE_SET_scrollview_1_18_2,
    FEIYU_MODE_SET_scrollview_1_18_3,
    FEIYU_MODE_SET_scrollview_1_18_4,
    FEIYU_MODE_SET_scrollview_1_18_5,    
    FEIYU_MODE_SET_scrollview_1_18_6,
    FEIYU_MODE_SET_scrollview_1_18_7,
    FEIYU_MODE_SET_scrollview_1_18_8,
    FEIYU_MODE_SET_scrollview_1_18_9,
    FEIYU_MODE_SET_scrollview_1_18_10,    
    FEIYU_MODE_SET_scrollview_1_18_11,
    FEIYU_MODE_SET_scrollview_1_18_12,
    FEIYU_MODE_SET_scrollview_1_18_13,
    FEIYU_MODE_SET_scrollview_1_18_14,
    FEIYU_MODE_SET_scrollview_1_18_15,    
    FEIYU_MODE_SET_scrollview_1_18_16,  
    FEIYU_MODE_SET_scrollview_1_18_17,      
    FEIYU_MODE_SET_scrollview_1_18_18,              
};


static HI_S32 *View_1[MAX_ITEM] = 
{
    NULL,
    NULL,
    NULL,
    NULL,
    ScrollView_1_5,
    NULL,
    ScrollView_1_7,
    ScrollView_1_8,
    NULL,
    ScrollView_1_10,    
    ScrollView_1_11,
    NULL,
    NULL,
    ScrollView_1_14,
    NULL,
    NULL,
    NULL,
    ScrollView_1_18    
};

static HI_S32 ScrollView_2_5[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_2_5,
    FEIYU_MODE_SET_scrollview_2_5_1,
    FEIYU_MODE_SET_scrollview_2_5_2, 
    FEIYU_MODE_SET_scrollview_2_5_3,
    FEIYU_MODE_SET_scrollview_2_5_4,
    FEIYU_MODE_SET_scrollview_2_5_5,
};

static HI_S32 ScrollView_2_8[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_2_8,
    FEIYU_MODE_SET_scrollview_2_8_1,
    FEIYU_MODE_SET_scrollview_2_8_2, 
    FEIYU_MODE_SET_scrollview_2_8_3,
    FEIYU_MODE_SET_scrollview_2_8_4,
    FEIYU_MODE_SET_scrollview_2_8_5,
    FEIYU_MODE_SET_scrollview_2_8_6,
    FEIYU_MODE_SET_scrollview_2_8_7, 
    FEIYU_MODE_SET_scrollview_2_8_8,
};

static HI_S32 ScrollView_2_9[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_2_9,
    FEIYU_MODE_SET_scrollview_2_9_1,
    FEIYU_MODE_SET_scrollview_2_9_2, 
    FEIYU_MODE_SET_scrollview_2_9_3,
    FEIYU_MODE_SET_scrollview_2_9_4,
    FEIYU_MODE_SET_scrollview_2_9_5,
    FEIYU_MODE_SET_scrollview_2_9_6,
    FEIYU_MODE_SET_scrollview_2_9_7, 
    FEIYU_MODE_SET_scrollview_2_9_8,
    FEIYU_MODE_SET_scrollview_2_9_9,
};

static HI_S32 ScrollView_2_10[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_2_10,
    FEIYU_MODE_SET_scrollview_2_10_1,
    FEIYU_MODE_SET_scrollview_2_10_2, 
    FEIYU_MODE_SET_scrollview_2_10_3,
    FEIYU_MODE_SET_scrollview_2_10_4,
    FEIYU_MODE_SET_scrollview_2_10_5,
    FEIYU_MODE_SET_scrollview_2_10_6,
    FEIYU_MODE_SET_scrollview_2_10_7, 
    FEIYU_MODE_SET_scrollview_2_10_8,
    FEIYU_MODE_SET_scrollview_2_10_9,
    FEIYU_MODE_SET_scrollview_2_10_10,
};


static HI_S32 ScrollView_2_11[MAX_ITEM] = 
{
    FEIYU_MODE_SET_scrollview_2_11,
    FEIYU_MODE_SET_scrollview_2_11_1,
    FEIYU_MODE_SET_scrollview_2_11_2, 
    FEIYU_MODE_SET_scrollview_2_11_3,
    FEIYU_MODE_SET_scrollview_2_11_4,
    FEIYU_MODE_SET_scrollview_2_11_5,
    FEIYU_MODE_SET_scrollview_2_11_6,
    FEIYU_MODE_SET_scrollview_2_11_7, 
    FEIYU_MODE_SET_scrollview_2_11_8,
    FEIYU_MODE_SET_scrollview_2_11_9,
    FEIYU_MODE_SET_scrollview_2_11_10,
    FEIYU_MODE_SET_scrollview_2_11_11,
};



static HI_S32 *View_2[MAX_ITEM] = 
{
    NULL,
    NULL,
    NULL,
    NULL,
    ScrollView_2_5,
    NULL,
    NULL,
    ScrollView_2_8,
    ScrollView_2_9,
    ScrollView_2_10,
    ScrollView_2_11,
    NULL,
    NULL,    
    NULL,
    NULL,    
    NULL,
    NULL,        
    NULL    
};
static HI_BOOL config_flag = HI_FALSE;
static HI_BOOL positionChange = HI_FALSE;


#define MAX_BUFF_COUNT 40
static HI_S32 s_cmdlist_flag = 0;
typedef struct {
    HI_S32 s32CmdId;
    HI_S32 s32CmdParam;
    HI_BOOL isUse;
}CMD_T;

typedef struct {
    CMD_T cmdBuff[MAX_BUFF_COUNT+1];
    HI_S32 cmdNum;
    HI_S32 flag;
    pthread_mutex_t cmd_mutex;
    
}CMD_LIST_T;

static CMD_LIST_T cmdList;

static HI_VOID deinitCmdList()
{
    MUTEX_DESTROY(cmdList.cmd_mutex);
}

static HI_VOID initCmdList()
{
    memset(&cmdList, 0, sizeof(CMD_LIST_T));
    cmdList.cmdNum = 0;
    MUTEX_INIT_LOCK(cmdList.cmd_mutex);
    
}
static HI_S32 getCmdNum()
{
    return cmdList.cmdNum;
}

static HI_S32 popCmd(CMD_T *pstCmd)
{
    //MLOGE("###\n");

    if(!pstCmd)
    {
        MLOGE("ERR\n");
        return -1;
    }
    
    MUTEX_LOCK(cmdList.cmd_mutex);
    //MLOGE("Entry\n");    
    if(cmdList.cmdNum == 0)
    {

        MUTEX_UNLOCK(cmdList.cmd_mutex);  
        //MLOGE("Exit\n");           
        return -1;
    }
    HI_S32 i = 0;
    for(i = 0; i < MAX_BUFF_COUNT; i++)
    {
        if(cmdList.cmdBuff[i].isUse)
        {
            memcpy(pstCmd, &(cmdList.cmdBuff[i]), sizeof(CMD_T));
            cmdList.cmdBuff[i].isUse = HI_FALSE;
            cmdList.cmdNum--;
            break;
        }
    }
    //MLOGE("Exit\n");   
    MUTEX_UNLOCK(cmdList.cmd_mutex);
    
    //MLOGE("#################=====================>>>>>>>>,s32CmdId=%d,  s32CmdParam=%d \n",pstCmd->s32CmdId, pstCmd->s32CmdParam);

    return 0;
}

static HI_S32 pushCmd(CMD_T stCmd)
{
    //MLOGE("#################=====================>>>>>>>>,s32CmdId=%d,  s32CmdParam=%d \n",stCmd.s32CmdId, stCmd.s32CmdParam);
    if(cmdList.flag < 10)
    {
        //MLOGE("exit..., drop cmd.\n");
        //return -1;
    }
    MUTEX_LOCK(cmdList.cmd_mutex);
    //MLOGE("Entry\n");
    if(cmdList.cmdNum == MAX_BUFF_COUNT)
    {
        MLOGE("===========cmd buff full\n");
        MUTEX_UNLOCK(cmdList.cmd_mutex);
        MLOGE("Exit\n");    
        return -1;
    }
    HI_S32 i = 0;
    for(i = 0; i < MAX_BUFF_COUNT; i++)
    {
        if(cmdList.cmdBuff[i].isUse)
        {
           if(cmdList.cmdBuff[i].s32CmdId == stCmd.s32CmdId )
           {
                s_cmdlist_flag = 0;
                MLOGE("===>>>s32CmdId=%d,s32CmdParam=%d==>>new:%d\n",cmdList.cmdBuff[i].s32CmdId,cmdList.cmdBuff[i].s32CmdParam,stCmd.s32CmdParam);           
                cmdList.cmdBuff[i].s32CmdParam = stCmd.s32CmdParam;
                //MLOGE("Update===>>>s32CmdId=%d,s32CmdParam=%d\n",cmdList.cmdBuff[i].s32CmdId,cmdList.cmdBuff[i].s32CmdParam);
                MUTEX_UNLOCK(cmdList.cmd_mutex);
                //MLOGE("Exit\n");
                return 0;
           }
        }
    }
    for(i = 0; i < MAX_BUFF_COUNT; i++)
    {    
        if(!cmdList.cmdBuff[i].isUse)
        {
            s_cmdlist_flag = 0;
            memcpy(&(cmdList.cmdBuff[i]), &stCmd, sizeof(CMD_T));
            cmdList.cmdBuff[i].isUse = HI_TRUE;
            cmdList.cmdNum++;
            break;
        }  
    }
    MUTEX_UNLOCK(cmdList.cmd_mutex);  
    //MLOGE("Exit\n");    
    return 0;
}

static HI_S32 hideAllWin()
{
    HI_S32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    
    HI_GV_Widget_Hide(FEIYU_MODE_SET);
    HI_GV_Widget_Hide(MODE_SET_LABLE);
    HI_GV_Widget_Hide(MODE_SET_LABLE_1);
    HI_GV_Widget_Hide(MODE_SET_LABLE_2);
    
	if (s32Ret != HI_SUCCESS)
	{
        MLOGE("Error:%#x\n", s32Ret);
	}    
    return s32Ret;
}

static HI_S32 FeiyujumpToHomePage()
{	
	HI_S32 s32Ret = HI_SUCCESS;

	g_pageChangeEnd = HI_TRUE;
	HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_UPDATE_HOME_PAGE, 0);
    HI_GV_Widget_Show(HOME_PAGE);
    HI_GV_Widget_Active(HOME_PAGE);
    hideAllWin();

	if (s32Ret != HI_SUCCESS)
	{
        MLOGE("Error:%#x\n", s32Ret);
	}
	return s32Ret;
}

HI_S32 FEIYU_MODE_SET_BUTTON_EXIT_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}


HI_S32 Feiyu_Mode_Set_OnHide(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    deinitCmdList();

    s_bRunning = HI_FALSE;
    s_bRunning1 = HI_FALSE;
    HI_GV_Timer_Stop(FEIYU_MODE_SET, MODE_SET_TIMER);
    HI_GV_Timer_Destory(FEIYU_MODE_SET, MODE_SET_TIMER);

    return HIGV_PROC_GOON;
}

HI_S32 Feiyu_Mode_Set_OnKeydown(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    if (HIGV_KEY_F2 == wParam) //mode key
    {
        FeiyujumpToHomePage();
    }
    else if (HIGV_KEY_F1 == wParam) //photo key
    {
		HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
        FeiyujumpToHomePage();
    }
    else
    {
        MLOGE("KEY CODE %#x not processed\n", wParam);
    }

    return HIGV_PROC_GOON;
}
static HI_S32 hideAll()
{
    HI_S32 i = 0;
    for(i = 0; i < MAX_ITEM; i++)
    {
        if( NULL != View_1[i])
        {
            if(HI_GV_Widget_IsShow((View_1[i])[0]))
                HI_GV_Widget_Hide((View_1[i])[0]);
        }
    }
    for(i = 0; i < MAX_ITEM; i++)
    {
        if( NULL != View_2[i])
        {    
            if(HI_GV_Widget_IsShow((View_2[i])[0]))
                HI_GV_Widget_Hide((View_2[i])[0]);
        }
    }    
}
static HI_S32 s_NewWorkMode = -1;
static HI_S32 refreshModeLabel(HI_S32 s32WorkMode)
{
    
    //MLOGE("############s32WorkMode=%d,VIDEO_LOOP=%d\n",s32WorkMode,VIDEO_LOOP);
    s_NewWorkMode = s32WorkMode;
    //return 0;
    HI_S32 wighet_id[9][2] = {
        {FEIYU_MODE_SET_scrollview_NP_1,FEIYU_MODE_SET_scrollview_NP_2},
        {FEIYU_MODE_SET_scrollview_TP_1, FEIYU_MODE_SET_scrollview_TP_2},
        {FEIYU_MODE_SET_scrollview_DP_1, FEIYU_MODE_SET_scrollview_DP_2},        
        {FEIYU_MODE_SET_scrollview_MP_1, FEIYU_MODE_SET_scrollview_MP_2},        
        {FEIYU_MODE_SET_scrollview_NV_1, FEIYU_MODE_SET_scrollview_NV_2},                
        {FEIYU_MODE_SET_scrollview_DV_1, FEIYU_MODE_SET_scrollview_DV_2},                        
        {FEIYU_MODE_SET_scrollview_SV_1, FEIYU_MODE_SET_scrollview_SV_2},
        {FEIYU_MODE_SET_scrollview_LV_1, FEIYU_MODE_SET_scrollview_LV_2},        
        {FEIYU_MODE_SET_scrollview_VP_1, FEIYU_MODE_SET_scrollview_VP_2},        
        };
    HI_S32 skin_id[9][2] = {
        {skin_mode_np,skin_mode_np_g},
        {skin_mode_tp,skin_mode_tp_g},
        {skin_mode_dp,skin_mode_dp_g},
        {skin_mode_mp,skin_mode_mp_g},
        {skin_mode_nv,skin_mode_nv_g},
        {skin_mode_dv,skin_mode_dv_g},
        {skin_mode_sv,skin_mode_sv_g},        
        {skin_mode_lv,skin_mode_lv_g},   
        {skin_mode_vp,skin_mode_vp_g},           
        
        };
    HI_S32 workmode_id[9] = {
        PHOTO,
        PHOTO_TIMER,
        PHOTO_DELAY,
        MULTI_PHOTO,
        VIDEO,
        VIDEO_LAPSE,
        VIDEO_SLOW,
        VIDEO_LOOP,
        VIDEO_SNAP,        
        };
   HI_S32 i = 0; 
   for(i = 0; i < 9; i++)
   {   
        if(s32WorkMode == workmode_id[i])
        {
            HI_GV_Widget_Highlight(wighet_id[i][0], HI_TRUE);
            HI_GV_Widget_Highlight(wighet_id[i][1], HI_TRUE);            
            #if 0
            HI_GV_Widget_SetSkin(wighet_id[i][0],HIGV_SKIN_NORMAL,skin_id[i][0]);            
            HI_GV_Widget_SetSkin(wighet_id[i][1],HIGV_SKIN_NORMAL,background_skin);                        
            HI_GV_Widget_SetSkin(wighet_id[i][0],HIGV_SKIN_ACITVE,skin_id[i][0]);            
            HI_GV_Widget_SetSkin(wighet_id[i][1],HIGV_SKIN_ACITVE,background_skin);       
            #endif
        }
        else
        {
            HI_GV_Widget_Highlight(wighet_id[i][0], HI_FALSE);
            HI_GV_Widget_Highlight(wighet_id[i][1], HI_FALSE);         
            #if 0
            HI_GV_Widget_SetSkin(wighet_id[i][0],HIGV_SKIN_NORMAL,skin_id[i][1]);
            HI_GV_Widget_SetSkin(wighet_id[i][1],HIGV_SKIN_NORMAL,background_skin_gray);
            HI_GV_Widget_SetSkin(wighet_id[i][0],HIGV_SKIN_ACITVE,skin_id[i][1]);            
            HI_GV_Widget_SetSkin(wighet_id[i][1],HIGV_SKIN_ACITVE,background_skin_gray);
            #endif
        }
   }

    HI_GV_Widget_Paint(FEIYU_MODE_SET_scrollview, 0);
    HI_GV_Widget_Paint(FEIYU_MODE_SET, 0);

    return 0;
}

static HI_S32 updateUI(HI_S32 flag)
{
    HI_S32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32ItemNum = 0;
    HI_S32 * p = NULL;
    HIGV_CORD tempx = 0;
    HIGV_CORD temp_tempx = 0;
    HIGV_CORD tempy = 0;
    HIGV_CORD temp_tempy = 0;
    HI_S32 s32CurrentWorkMode = 0;  
    
    HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, &tempx, &tempy);
    FeiYuCmdExecute(CMD_GET_CURRENT_WORK_MODE, 0, &s32CurrentWorkMode);
    
    if(1 ==flag)
        s32CurrentWorkMode = s_s32CurrentWorkMode;
    
	switch(s32CurrentWorkMode)
	{
		case PHOTO:
            s_current_subpage = SUBPAGE_PHOTO_PARAM_IMAGE_MODE;
            s_current_subpage_1 = -1;
            tempx = 0;
            break;
		case PHOTO_TIMER:
            s_current_subpage = SUBPAGE_PHOTOTIMER_PARAM_IMAGE_MODE;
            s_current_subpage_1 = SUBPAGE_PHOTOTIMER_PARAM_DELAY_TIME;  
            tempx = 142;
            break;
		case PHOTO_DELAY:
            s_current_subpage = SUBPAGE_PHOTODELAY_PARAM_IMAGE_MODE;
            s_current_subpage_1 = SUBPAGE_PHOTODELAY_PARAM_DELAY_TIME;  
            tempx = 142*2;
            break;
		case MULTI_PHOTO:
            s_current_subpage = SUBPAGE_MULTIPHOTO_BURST_IMAGE_MODE;
            s_current_subpage_1 = SUBPAGE_MULTIPHOTO_BURST_BURSTTYPE;  
            tempx = 142*3;
            break;
		case VIDEO_SNAP:
            s_current_subpage = SUBPAGE_VIDEOSNAP_MODE;
            s_current_subpage_1 = SUBPAGE_VIDEOSNAP_LAPSEINTERVAL;
            tempx = 142*8;
            break;
		case VIDEO:
            s_current_subpage = SUBPAGE_VIDEO_MODE;
            s_current_subpage_1 = -1;   
            tempx = 142*4;
            break;
		case VIDEO_LAPSE:
            s_current_subpage = SUBPAGE_VIDEOLAPSE_MODE;
            s_current_subpage_1 = SUBPAGE_VIDEOLAPSE_LAPSEINTERVAL;  
            tempx = 142*5;
            break;
		case VIDEO_SLOW:
            s_current_subpage = SUBPAGE_VIDEOSLOW_MODE;
            s_current_subpage_1 = -1;      
            tempx = 142*6;
            break;
		case VIDEO_LOOP:
            s_current_subpage = SUBPAGE_VIDEOLOOP_MODE;
            s_current_subpage_1 = SUBPAGE_VIDEOLOOP_LOOPTYPE;     
            tempx = 142*7;
            break;
		default:
            s_current_subpage = SUBPAGE_PHOTO_MODE_PARAM;
            s_current_subpage_1 = -1;            
            break;
	}     

    
    if(0 == flag)
    {
        refreshModeLabel(s32CurrentWorkMode);
        i = 0;
        do{        

            HI_GV_ScrollView_SetViewCoordinate(FEIYU_MODE_SET_scrollview, tempx, tempy);            
            HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, &temp_tempx, &temp_tempy);            
            HI_GV_Widget_Refresh(FEIYU_MODE_SET_scrollview, 0);               
            HI_GV_Widget_Update(FEIYU_MODE_SET_scrollview, 0);
            MLOGE("=====>>>temp_tempx=%d\n",temp_tempx);
        }while(i++ < 100 && temp_tempx != tempx);
    }

    hideAll();

    if(s_current_subpage > 0)
    {
        for(i = 0; i < MAX_ITEM_NUM; i++)
        {
            if(sa_SubPageItemDes[s_current_subpage].strDesID[i] == 0)
            {
                break;
            }
            s32ItemNum++;        
        }
        s32ItemNum += 2;           
        MLOGE("==================>>>s32ItemNum=%d\n",s32ItemNum);  
        s_s32Item_0 = s32ItemNum -1;
        p = View_1[s32ItemNum -1]; 
        if( p != NULL)
        {
            for(i = 0; i < s32ItemNum; i++)
            {
                if(0 == i)
                    s32Ret |=HI_GV_Widget_SetText(p[i+1], " ");
                HI_S32 s32StrID = sa_SubPageItemDes[s_current_subpage].strDesID[i];
                if(s32StrID > 0)
                {
                    s32Ret |=HI_GV_Widget_SetTextByID(p[i+2], s32StrID);
                }
            }         
            s32Ret |= HI_GV_Widget_Show(p[0]);
            s32Ret |= HI_GV_Widget_MoveToTop(p[0]);

            HI_S32 s32GetCmd = sa_SubPageItemDes[s_current_subpage].getCmdid;
            HI_S32 s32State = 0;
            FeiYuCmdExecute(s32GetCmd, 0, &s32State);
            i = 0;

            do{
                if( sa_SubPageItemDes[s_current_subpage].FeiyuCmdParam[i] == s32State)
                    break;
                i++;
            }while(sa_SubPageItemDes[s_current_subpage].FeiyuCmdParam[i] > 0);
            //MLOGE("===============================================================>>>>>>i=%d\n", i);
            HI_S32 setTempy = i*40;
            HI_S32 j = 0;
            //i = 0;
            HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy);            
            do{
            HI_GV_ScrollView_SetViewCoordinate((View_1[s_s32Item_0])[0], tempx, setTempy); 
            HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy);            
            HI_GV_Widget_Update((View_1[s_s32Item_0])[0], 0);
            HI_GV_Widget_Refresh((View_1[s_s32Item_0])[0], 0);  
            MLOGE("location err\n");
            }while(j++ < 200 && tempy!= setTempy);
            HI_S32 s32Item = 0;
            
            s32Item = i;
            for(i = 0; i < MAX_ITEM; i ++)
            {
                if((View_1[s_s32Item_0])[i+1] == 0)
                    break;
               HI_GV_Widget_SetSkin((View_1[s_s32Item_0])[i+1], HIGV_SKIN_NORMAL, background_skin_mode);                         
            }                
            HI_GV_Widget_SetSkin((View_1[s_s32Item_0])[s32Item+2], HIGV_SKIN_NORMAL, background_skin); 
            //MLOGE("===============s32Item=%d\n",s32Item);
            HI_GV_Widget_Update((View_1[s_s32Item_0])[s32Item+2], 0);
            HI_GV_Widget_Refresh((View_1[s_s32Item_0])[s32Item+2], 0);              
            s32Ret |= HI_GV_Widget_Paint(p[0], 0);              
        }
        else
        {
            MLOGE("null \n");
            s_s32Item_0 = -1;
        }        
    }
    else
    {
        s_s32Item_0 = -1;
    }
    
    s32ItemNum = 0;
    if(s_current_subpage_1 > 0)
    {
        s32Ret |=HI_GV_Widget_Hide(FEIYU_MODE_SET_label_3);
        for(i = 0; i < MAX_ITEM_NUM; i++)
        {
            if(sa_SubPageItemDes[s_current_subpage_1].strDesID[i] == 0)
            {
                break;
            }
            s32ItemNum++;        
        }
        s32ItemNum += 2;   
        s_s32Item_1 = s32ItemNum -1;
        MLOGE("s32ItemNum=%d\n",s32ItemNum);
        p = View_2[s32ItemNum -1]; 
        if( p != NULL)
        {
            for(i = 0; i < s32ItemNum; i++)
            {
                if(0 == i)
                    s32Ret |=HI_GV_Widget_SetText(p[i+1], " ");
                HI_S32 s32StrID = sa_SubPageItemDes[s_current_subpage_1].strDesID[i];
                if(s32StrID > 0)
                {
                    s32Ret |=HI_GV_Widget_SetTextByID(p[i+2], s32StrID);
                }
            }   
            s32Ret |= HI_GV_Widget_Show(p[0]);
            s32Ret |= HI_GV_Widget_MoveToTop(p[0]);
            HI_S32 s32GetCmd = sa_SubPageItemDes[s_current_subpage_1].getCmdid;
            HI_S32 s32State = 0;
            FeiYuCmdExecute(s32GetCmd, 0, &s32State);
            i = 0;

            do{
                if( sa_SubPageItemDes[s_current_subpage_1].FeiyuCmdParam[i] == s32State)
                    break;
                i++;
            }while(sa_SubPageItemDes[s_current_subpage_1].FeiyuCmdParam[i] > 0);
            HI_GV_ScrollView_SetViewCoordinate((View_2[s_s32Item_1])[0], 0, i*40);                

            HI_S32 s32Item = 0;
            s32Item = i;
            for(i = 0; i < MAX_ITEM; i ++)
            {
                if((View_2[s_s32Item_1])[i+1] == 0)
                    break;
                HI_GV_Widget_SetSkin((View_2[s_s32Item_1])[i+1], HIGV_SKIN_NORMAL, background_skin_mode);                         
            }
            HI_GV_Widget_SetSkin((View_2[s_s32Item_1])[s32Item+2], HIGV_SKIN_NORMAL, background_skin);                                                   
            s32Ret |= HI_GV_Widget_Paint(p[0], 0);
        }  
        else
        {
            MLOGE("null \n");
            s_s32Item_1 = -1;
        }
    }
    else
    {
        for(i ==0; i < MAX_ITEM; i++)
        {
            if( NULL!=View_2[i])
            {
                if(HI_GV_Widget_IsShow((View_2[i])[0]))
                {
                    s32Ret |= HI_GV_Widget_Hide((View_2[i])[0]);
                }
            }
            s32Ret |=HI_GV_Widget_Show(FEIYU_MODE_SET_label_3);
        }
        s_s32Item_1 = -1;
    }

 
    if(0 != s32Ret)
    {
        MLOGE("ERR, s32Ret=%d\n");
    }

    positionChange = HI_TRUE;

    return 0;
}

HI_S32 FEIYU_MODE_SET_onevent(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32NewWorkMode = 0;
    HIGV_CORD tempx = 0;
    HIGV_CORD tempy = 0; 
    HI_S32 s32Item = 0;
    CMD_T stCmd = {0};    
    if(MODE_SET_EVENT_1 == wParam)
    {
        
        HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, &tempx, &tempy); 
        tempx += 71;        
        s32Item = tempx /142;  
        switch(s32Item)
        {
            case 0:s32NewWorkMode=PHOTO;stCmd.s32CmdParam =CMD_SET_WORKMODE_PHOTO;break;
            case 1:s32NewWorkMode=PHOTO_TIMER;stCmd.s32CmdParam =CMD_SET_WORKMODE_PHOTO_TIMER;break;
            case 2:s32NewWorkMode=PHOTO_DELAY;stCmd.s32CmdParam =CMD_SET_WORKMODE_PHOTO_DELAY;break;
            case 3:s32NewWorkMode=MULTI_PHOTO;stCmd.s32CmdParam =CMD_SET_WORKMODE_MULIT_PHOTO;break;       
            case 4:s32NewWorkMode=VIDEO;stCmd.s32CmdParam =CMD_SET_WORKMODE_VIDEO;break;
            case 5:s32NewWorkMode=VIDEO_LAPSE;stCmd.s32CmdParam =CMD_SET_WORKMODE_VIDEO_LAPSE;break;
            case 6:s32NewWorkMode=VIDEO_SLOW;stCmd.s32CmdParam =CMD_SET_WORKMODE_VIDEO_SLOW;break;
            case 7:s32NewWorkMode=VIDEO_LOOP;stCmd.s32CmdParam =CMD_SET_WORKMODE_VIDEO_LOOP;break;
            case 8:s32NewWorkMode=VIDEO_SNAP;stCmd.s32CmdParam =CMD_SET_WORKMODE_VIDEO_SNAP;break;
            default:break;
        }
        refreshModeLabel(s32NewWorkMode);
        s_s32CurrentWorkMode = s32NewWorkMode;
        s_s32SelectItem1 = -1;
        s_s32SelectItem2 = -1;
        prePage1tempX = -1;
        prePage2tempX = -1;
        updateUI(1);
        MLOGE("pushCmd\n");
        stCmd.s32CmdId = 0x1234;
        pushCmd(stCmd);

	}
    return HIGV_PROC_GOON;
        
}
static HI_S32 handleInput(HIGV_CORD * pVal)
{
    if(!pVal)
    {
        MLOGE("err\n");
        return -1;
    }
    HI_S32 s32Diff = 0;
    HI_S32 s32Num = 0;
    s32Num = (*pVal)/142;
    s32Diff = (*pVal) - (*pVal)/ 142;
    if(s32Diff > 71)
    {
        *pVal = ((s32Num+1)*142);
    }
    return 0;
}



HI_VOID doMove(void *arg)
{
    //return ;
    pthread_detach(pthread_self());
    HIGV_CORD tempx = 0;
    HIGV_CORD tempy = 0;  
    static HIGV_CORD s_tempy = 0;
    static HIGV_CORD s_tempx = 0;    
    HI_S32 s32Item = 0;
    static HI_S32 s_s32Item = 0;
    static HI_S32 s_flag = 0;
    HI_S32 i = 0;
    static HI_S32 s_preCmdID = 0;
    static HI_S32 s_preCmdParam = 0;
    static HI_S32 s_preCmdID1 = 0;
    static HI_S32 s_preCmdParam1 = 0;  
    HI_S32 s32Ret = 0;
    positionChange = HI_TRUE;
    usleep(1000);
    while(s_bRunning)
    {
        #if 1
        if(config_flag || !positionChange)
        {
            usleep(100);
            continue;
        }
        #endif
PAGE_0:
    
        HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, &tempx, &tempy); 
        tempx += 71;
        static HI_S32 preItem = -1;
        if(preItem != tempx /142)
        {
            if(-1 != preItem)
            HI_GV_Msg_SendAsync(FEIYU_MODE_SET, HIGV_MSG_EVENT, MODE_SET_EVENT_1, 0);
            preItem = tempx /142;
        }

PAGE_1:

        if(s_s32Item_0 > 0)
        {
            static HI_S32 s_s32PreItem = 0;
            static HI_S32 s_s32TempY = 0;
            static HI_S32 s_flag_1 = 0;
            HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy);  
            if(prePage1tempX != tempy)
            {
                prePage1tempX = tempy;
            }
            else
            {
                goto PAGE_2;
            }
            //MLOGE("---->tempy=%d\n",tempy);
            if(tempy < 20)
            {
                tempy = 0;
            }
            else if(tempy > 20 && tempy < 60)
            {
                tempy = 40;
            }
            else if(tempy >((s_s32Item_0 )*40 - 20))
            {
                tempy = (s_s32Item_0 )*40;
            }
            else
            {
                tempy += 20;                
            }

            s32Item = tempy/40;
            s_s32SelectItem1 = s32Item;
            HI_U16 u16CmdID = 0;
            HI_U16 u16CmdParam = 0;            
            u16CmdID = sa_SubPageItemDes[s_current_subpage].setCmdid;
            u16CmdParam = sa_SubPageItemDes[s_current_subpage].FeiyuCmdParam[s32Item];
            //if(s_s32PreItem != s32Item)
            {
                //HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy); 
                //tempy+=10;
                //s32Item = tempy/40;
                for(i = 0; i < MAX_ITEM; i ++)
                {
                    if((View_1[s_s32Item_0])[i+1] == 0)
                        break;
                   HI_GV_Widget_SetSkin((View_1[s_s32Item_0])[i+1], HIGV_SKIN_NORMAL, background_skin_mode);                         
                }                
                //MLOGE("===============s32Item=%d\n",s32Item);
                HI_GV_Widget_SetSkin((View_1[s_s32Item_0])[s32Item+2], HIGV_SKIN_NORMAL, background_skin);               
                if(u16CmdID > 0 && u16CmdParam > 0)
                {
                    if(s_preCmdParam != u16CmdParam && s_preCmdParam != 0)
                    {
                        //FeiYuCmdExecute(u16CmdID, u16CmdParam, NULL);
                        CMD_T stCmd;
                        stCmd.s32CmdId = u16CmdID;
                        stCmd.s32CmdParam = u16CmdParam;
                        MLOGE("pushCmd\n");
                        pushCmd(stCmd);
                    }
                    s_preCmdID = u16CmdID;
                    s_preCmdParam = u16CmdParam;

                }
                else
                    MLOGE("------cmd err!-------\n");
                s_s32PreItem = s32Item;
                s_bIsFirstEntry_1 = HI_FALSE;
                

            }
            if(s_s32TempY == tempy)
            {
                s_flag_1++;
            }
            else
            {
                s_flag_1 = 0;
                s_s32TempY = tempy;
            }
            HI_GV_Widget_Paint((View_1[s_s32Item_0])[0], 0);
        }
PAGE_2:

        if(s_s32Item_1 > 0)
        {
            static HI_S32 s_s32PreItem_1 = 0;
            static HI_S32 s_s32TempY_1 = 0;
            static HI_S32 s_flag_2 = 0;                
            HI_GV_ScrollView_GetViewCoordinate((View_2[s_s32Item_1])[0], &tempx, &tempy);  
            if(prePage2tempX != tempy)
            {
                prePage2tempX = tempy;
            }
            else
            {
                goto END;
            }            
            if(tempy < 20)
            {
                tempy = 0;
            }
            else if(tempy > 20 && tempy < 60)
            {
                tempy = 40;
            }
            else if(tempy >((s_s32Item_1 )*40 - 20))
            {
                tempy = (s_s32Item_1 )*40;
            }
            else
            {
                tempy += 20;                
            }            
           
            s32Item = tempy/40;
            s_s32SelectItem2 = s32Item;

            //if(s_s32PreItem_1 != s32Item)
            {      
                for(i = 0; i < MAX_ITEM; i ++)
                {
                    if((View_2[s_s32Item_1])[i+1] == 0)
                        break;
                    HI_GV_Widget_SetSkin((View_2[s_s32Item_1])[i+1], HIGV_SKIN_NORMAL, background_skin_mode);                         
                }
                HI_GV_Widget_SetSkin((View_2[s_s32Item_1])[s32Item+2], HIGV_SKIN_NORMAL, background_skin);              
                HI_U16 u16CmdID = 0;
                HI_U16 u16CmdParam = 0;            
                u16CmdID = sa_SubPageItemDes[s_current_subpage_1].setCmdid;
                u16CmdParam = sa_SubPageItemDes[s_current_subpage_1].FeiyuCmdParam[s32Item];            
                if(u16CmdID > 0 && u16CmdParam > 0)
                {
                    if(s_preCmdParam1 != u16CmdParam && s_preCmdParam1 != 0)
                    {
                        //FeiYuCmdExecute(u16CmdID, u16CmdParam, NULL);   
                        CMD_T stCmd;
                        stCmd.s32CmdId = u16CmdID;
                        stCmd.s32CmdParam = u16CmdParam;
                        MLOGE("pushCmd\n");
                        pushCmd(stCmd);                        
                    }
                    s_preCmdID1 = u16CmdID;
                    s_preCmdParam1 = u16CmdParam;

                }
                else
                    MLOGE("------cmd err!-------\n");
                s_s32PreItem_1 = s32Item;                
                s_bIsFirstEntry_2 = HI_FALSE;
            }            
            if(s_s32TempY_1 == tempy)
            {
                s_flag_2++;
            }
            else
            {
                s_flag_2 = 0;
                s_s32TempY_1 = tempy;
            }         
            HI_GV_Widget_Paint((View_2[s_s32Item_1])[0], 0);
        }
END:
        HI_GV_Widget_Paint(FEIYU_MODE_SET, 0);
        positionChange = HI_FALSE;
        usleep(100*1000);        
    }


    return ;
}

static HI_VOID doExcuteCmd(void *arg)
{
    pthread_detach(pthread_self());

    CMD_T stCmd = {0};
    HI_S32 s32Ret = 0;

    while(s_bRunning || (getCmdNum() > 0))
    {
        #if 0
        if(s_cmdlist_flag++ < 500)
        {
            usleep(1000);
            continue;
        }
        else
        {
            s_cmdlist_flag = 200;
        }
        #endif
        if(getCmdNum() > 0)
        {
            s32Ret = popCmd(&stCmd);
            if(0 == s32Ret)
            {
                //MLOGE("CMD_SET_MODE_PARAM_VIDEO_VIDEOMODE=%d,VIDEO_MODE_720P_120=%d",CMD_SET_MODE_PARAM_VIDEO_VIDEOMODE,VIDEO_MODE_720P_120);
                MLOGE("[excute]>>>>>>>>,s32CmdId=%d s32CmdParam=%d \n",stCmd.s32CmdId, stCmd.s32CmdParam);
                if(0x1234 == stCmd.s32CmdId)
                {
                    FeiYuCmdExecute(stCmd.s32CmdParam, 0, NULL);
                }
                else
                {
                    FeiYuCmdExecute(stCmd.s32CmdId, stCmd.s32CmdParam, NULL);                    
                }

            }
            usleep(1000);
        }
        else
        {
            usleep(500*1000);
        }
    }
    #if 0
    HI_S32 s32CmdId = sa_SubPageItemDes[t_s_current_subpage].setCmdid;  
    HI_S32 s32CmdParam = sa_SubPageItemDes[t_s_current_subpage].FeiyuCmdParam[s_i-1];  
    if(s32CmdId != 0 && s32CmdParam != 0)
        FeiYuCmdExecute(s32CmdId,s32CmdParam,NULL);
    else
        MLOGE("cmd err!\n");    
    #endif
    
    return NULL;
}

static HI_S32 Feiyu_Hide_User_Guider()
{
	if(HI_GV_Widget_IsShow(mode_set_user_guider_groupbox))
	{
		HI_GV_Widget_Hide(mode_set_user_guider_groupbox);
	}

	return HI_SUCCESS;
}

HI_S32 Feiyu_Mode_Set_OnShow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    //MLOGE("########\n");
    HI_S32 s32Ret = 0;
    extern USER_PARA_S* g_pstUserPara;
	
    initCmdList();
    prePage1tempX = -1;
    prePage2tempX = -1;
    s_bRunning = HI_TRUE;
    s_bIsFirstEntry_1 = HI_TRUE;
    s_bIsFirstEntry_2 = HI_TRUE;    
    s_bSroll = HI_FALSE;
    s_NewWorkMode = -1;
    //HI_GV_ScrollView_SetFlingParam(FEIYU_MODE_SET_scrollview, 0.3);
    s_s32SelectItem1 = -1;
    s_s32SelectItem2 = -1;

	if(1 == g_pstUserPara->commPara.u8FirstEntryModeSet)
	{
		HI_GV_Widget_Show(mode_set_user_guider_groupbox);
		g_pstUserPara->commPara.u8FirstEntryModeSet = 0;
		extern HI_S32 fy_User_SaveParam(HI_U8 enSaveFlag);
	    s32Ret |= fy_User_SaveParam(HI_TRUE);
	    if(HI_SUCCESS != s32Ret)
		{
			MLOGE("err: %d\n", s32Ret);
	    } 
	}
	else
	{
		HI_GV_Widget_Hide(mode_set_user_guider_groupbox);
	}

    updateUI(0);
    HI_GV_Widget_Show(MODE_SET_LABLE);
    HI_GV_Widget_MoveToTop(MODE_SET_LABLE);
    HI_GV_Widget_Show(MODE_SET_LABLE_1);
    HI_GV_Widget_MoveToTop(MODE_SET_LABLE_1);
    HI_GV_Widget_Show(MODE_SET_LABLE_2);
    HI_GV_Widget_MoveToTop(MODE_SET_LABLE_2);    
    HI_GV_Widget_Paint(FEIYU_MODE_SET_scrollview, 0);
    #if 1
    pthread_t threadid = 0;
    s32Ret = pthread_create(&threadid, NULL, (void *)doMove, NULL);
    if(0 != s32Ret)
    {
        MLOGE("pthread_create Failed, s32Ret=%#x\n", s32Ret);
    }    
    #endif
    pthread_t threadid1 = 0;
    s_bRunning1 =HI_TRUE;
    s32Ret = pthread_create(&threadid1, NULL, (void *)doExcuteCmd, NULL);
    if(0 != s32Ret)
    {
        MLOGE("pthread_create, s32Ret=%#x\n", s32Ret);
    }       
    HI_GV_Timer_Create(FEIYU_MODE_SET, MODE_SET_TIMER, MODE_SET_TIMER_INTERVAL);
    HI_GV_Timer_Start(FEIYU_MODE_SET, MODE_SET_TIMER);    
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_SCROLLVIEW1_BUTTON_MULTI_PHOTO_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_SCROLLVIEW1_BUTTON_PHOTO_DELAY_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_SCROLLVIEW1_BUTTON_PHOTO_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_SCROLLVIEW1_BUTTON_PHOTO_TIMER_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_SCROLLVIEW1_BUTTON_VIDEO_LOOP_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_SCROLLVIEW1_BUTTON_VIDEO_SLOW_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_SCROLLVIEW1_BUTTON_VIDEO_SNAP_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_SCROLLVIEW1_VIDEO_LAPSE_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_SCROLLVIEW1_VIDEO_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

static void mode_set_an_handle(HIGV_LISTENER_S* listener, void* data)
{
    //MLOGE("###,s_s32WhichView=%d\n",s_s32WhichView);
    HI_S32 tempx = 0, tempy = 0;
    HI_S32 viewId = 0;
    if(s_s32WhichView == 0)
    {
        HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, tempx, tempy);

        HI_GV_ScrollView_SetViewCoordinate(FEIYU_MODE_SET_scrollview, HI_GV_TweenAnimGetTweenValue(s_s32Modehandle, 0), tempy);
        
        HI_GV_Widget_Update(FEIYU_MODE_SET_scrollview, 0);    
    }
    else if(s_s32WhichView == 1)
    {
        viewId = (View_1[s_s32Item_0])[0];
        HI_GV_ScrollView_GetViewCoordinate(viewId, tempx, tempy);

        HI_GV_ScrollView_SetViewCoordinate(viewId, tempx, HI_GV_TweenAnimGetTweenValue(s_s32Modehandle, 0));
        
        HI_GV_Widget_Update(viewId, 0);            
    }
    else if(s_s32WhichView == 2)
    {
        viewId = (View_2[s_s32Item_1])[0];
        HI_GV_ScrollView_GetViewCoordinate(viewId, tempx, tempy);

        HI_GV_ScrollView_SetViewCoordinate(viewId, tempx, HI_GV_TweenAnimGetTweenValue(s_s32Modehandle, 0));
        
        HI_GV_Widget_Update(viewId, 0);            
        
    }
}





static void mode_set_an_handle_end(HIGV_LISTENER_S* listener, void* data)
{
    HI_S32 tempx = 0, tempy = 0;
    HI_S32 s32Item = 0;
    HI_S32 i = 0;
    s_timer_flag = HI_FALSE;
    if(0 ==s_s32WhichView )
    {
         updateUI(1);
         s_bIsRunning_1 =HI_FALSE;
    }
    else if(1 == s_s32WhichView)
    {
        HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy); 
        s32Item = tempy/40;
        for(i = 0; i < MAX_ITEM; i ++)
        {
            if((View_1[s_s32Item_0])[i+1] == 0)
                break;
           HI_GV_Widget_SetSkin((View_1[s_s32Item_0])[i+1], HIGV_SKIN_NORMAL, background_skin_mode);                         
        }                
        HI_GV_Widget_SetSkin((View_1[s_s32Item_0])[s32Item+2], HIGV_SKIN_NORMAL, background_skin);   
        MLOGE("11111============>>>>s32Item=%d\n",s32Item);
        HI_U16 u16CmdID = 0;
        HI_U16 u16CmdParam = 0;            
        u16CmdID = sa_SubPageItemDes[s_current_subpage].setCmdid;
        u16CmdParam = sa_SubPageItemDes[s_current_subpage].FeiyuCmdParam[s32Item];  
        FeiYuCmdExecute(u16CmdID, u16CmdParam, NULL);
        s_bIsRunning_2 = HI_FALSE;     
    }
    else if(2 == s_s32WhichView)
    {
        HI_GV_ScrollView_GetViewCoordinate((View_2[s_s32Item_1])[0], &tempx, &tempy);     
        s32Item = tempy/40;        
        for(i = 0; i < MAX_ITEM; i ++)
        {
            if((View_2[s_s32Item_1])[i+1] == 0)
                break;
            HI_GV_Widget_SetSkin((View_2[s_s32Item_1])[i+1], HIGV_SKIN_NORMAL, background_skin_mode);                         
        }
        HI_GV_Widget_SetSkin((View_2[s_s32Item_1])[s32Item+2], HIGV_SKIN_NORMAL, background_skin);                                        
        MLOGE("222============>>>>s32Item=%d\n",s32Item);
        HI_U16 u16CmdID = 0;
        HI_U16 u16CmdParam = 0;            
        u16CmdID = sa_SubPageItemDes[s_current_subpage_1].setCmdid;
        u16CmdParam = sa_SubPageItemDes[s_current_subpage_1].FeiyuCmdParam[s32Item];  
        FeiYuCmdExecute(u16CmdID, u16CmdParam, NULL);       
        s_bIsRunning_3 = HI_FALSE;     
    }

     s_s32Item = new_ItemNum;

    
}

static HI_S32 moveFrame(float startVal, float endVal)
{
    MLOGE("startVal=%f, endVal=%f\n",startVal,endVal);
    HI_S32 i = 0;
    i = abs(startVal - endVal)/142;
    s_s32Modehandle = HI_GV_TweenAnimCreate();
    if(0 == s_s32WhichView)
    {
        HI_GV_TweenAnimSetDuration(s_s32Modehandle,100);
    }
    else
        HI_GV_TweenAnimSetDuration(s_s32Modehandle,200);    

    HI_GV_TweenAnimAddTween(s_s32Modehandle,HIGV_TWEEN_TRANSITION_LINEAR,HIGV_TWEEN_EASE_OUT,startVal,endVal);

    HI_GV_TweenAnimSetExecListener(s_s32Modehandle, mode_set_an_handle);
       
    HI_GV_TweenAnimSetStartedListener(s_s32Modehandle, NULL);

    HI_GV_TweenAnimSetFinishedListener(s_s32Modehandle, mode_set_an_handle_end);


    HI_GV_TweenAnimStart(s_s32Modehandle);    
    
    return 0;
}

#if 0
HI_S32 FEIYU_MODE_SET_OnGestureFling(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
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

        }
        else  //down
        {
            HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
        }
    }

    return HIGV_PROC_GOON;
}
#endif


HI_S32 FEIYU_MODE_SET_OnGestureFling_1(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
    if(!s_bSroll)
    {
        MLOGE("waiting ...\n");
        return HIGV_PROC_GOON;
    }
    
    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32Diff = 0;
    HI_S32 tempy =0, tempx =0, afterTempx = 0;
    HI_S32 i = 0;

    //MLOGD("\n");

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
    //MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);
    HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, &tempx, &tempy);
    if(s_bIsRunning_3 | s_bIsRunning_2 | s_bIsRunning_1)
    {
        MLOGE("waiting an...\n");
        return HIGV_PROC_GOON;
    }
    if (abs(startX - endX) > abs(startY - endY))// left or right
    {
        s_s32WhichView = 0;
        if (startX > endX)// to left
        {
            s_bIsRunning_1 = HI_TRUE;
            moveFrame(tempx, tempx + 142);
            new_ItemNum = (tempx + 142)/142;
        }
        else // to right
        {
            if(tempx >= 142)
            {
                s_bIsRunning_1 = HI_TRUE;            
                moveFrame(tempx, tempx - 142);                
                new_ItemNum = (tempx - 142)/142;                
            }            
        }
    }
    else //up or down
    {
        if (startY > endY)//to up
        {

        }
        else  //down
        {
            HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
        }

    }


    
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_ontouchaction_2(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    if(s_current_subpage < 0)
        return HIGV_PROC_GOON;

    if(cmdList.flag < 5)
         return HIGV_PROC_GOON;

    HI_S32 point_x, point_y;
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;
    HI_S32 tempy =0, tempx =0, afterTempy = 0;
    //HI_BOOL config_flag = HI_FALSE;
    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));
    static HI_S32 baseX = 0;
    static HI_S32 baseY = 0;
    static HI_S32 s_tempy = 0;
    point_x = touchEvent.last.x;
    point_y = touchEvent.last.y;


    type = touchEvent.last.type;
    switch(type)
    {
        case HIGV_TOUCH_START:
            baseX = point_x;
            baseY = point_y;
			Feiyu_Hide_User_Guider();
            HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &s_tempy);   
            config_flag = HI_FALSE;
            HI_GV_Timer_Stop(FEIYU_MODE_SET, MODE_SET_TIMER);

            break;
        case HIGV_TOUCH_END:
            HI_GV_Timer_Start(FEIYU_MODE_SET, MODE_SET_TIMER);
            config_flag = HI_FALSE;
            positionChange = HI_TRUE;
            break;
        case HIGV_TOUCH_MOVE:
            afterTempy = s_tempy - (point_y - baseY);
            config_flag = HI_TRUE;            
            break;
        default:
            break;
    }
    //MLOGE("type=%d,point_y=%d,s_tempy=%d,afterTempy=%d\n",type,point_y,s_tempy,afterTempy);
    if(config_flag)
    {
        HI_GV_ScrollView_SetViewCoordinate((View_1[s_s32Item_0])[0], tempx, afterTempy);
         HI_GV_Widget_Paint((View_1[s_s32Item_0])[0], 0);        
    }

    return HIGV_PROC_GOON;

} 

HI_S32 FEIYU_MODE_SET_ontouchaction_1(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //MLOGE("###\n");
    HI_S32 point_x, point_y;
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;
    HI_S32 tempy =0, tempx =0, afterTempx = 0;
    //HI_BOOL config_flag = HI_FALSE;
    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));
    static HI_S32 baseX = 0;
    static HI_S32 baseY = 0;
    static HI_S32 s_tempx = 0;
    point_x = touchEvent.last.x;
    point_y = touchEvent.last.y;
    type = touchEvent.last.type;
    //MLOGE("type=%d\n",type);
    switch(type)
    {
        case HIGV_TOUCH_START:
            g_startY = point_y;
            baseX = point_x;
            baseY = point_y;     
			Feiyu_Hide_User_Guider();
            HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, &s_tempx, &tempy); 
            config_flag = HI_FALSE;
            HI_GV_Timer_Stop(FEIYU_MODE_SET, MODE_SET_TIMER);
            break;
        case HIGV_TOUCH_END:
            HI_GV_Timer_Start(FEIYU_MODE_SET, MODE_SET_TIMER);  
            config_flag = HI_FALSE;
            positionChange = HI_TRUE;
            break;            
        case HIGV_TOUCH_MOVE:
            afterTempx = s_tempx - (point_x - baseX);
            config_flag = HI_TRUE;      
            break;
        default:
            break;
    }
    
    if(config_flag && g_startY > 40)
    {
        HI_GV_ScrollView_SetViewCoordinate(FEIYU_MODE_SET_scrollview, afterTempx, tempy);    
        HI_GV_Widget_Paint(FEIYU_MODE_SET_scrollview, 0);
    }
    
    return HIGV_PROC_GOON;

}

HI_S32 FEIYU_MODE_SET_ontouchaction(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //MLOGE("###\n");
    HI_S32 point_x, point_y;
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;
    HI_S32 tempy =0, tempx =0, afterTempy = 0;
    //HI_BOOL config_flag = HI_FALSE;
    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));
    static HI_S32 baseX = 0;
    static HI_S32 baseY = 0;
    static HI_S32 s_tempy = 0;
    point_x = touchEvent.last.x;
    point_y = touchEvent.last.y;
    type = touchEvent.last.type;
    //MLOGE("type=%d\n",type);
    switch(type)
    {
        case HIGV_TOUCH_START:
			g_startY = point_y;
            baseX = point_x;
            baseY = point_y;            
            //HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, &tempx, &s_tempy); 
            Feiyu_Hide_User_Guider();
            config_flag = HI_FALSE;
            HI_GV_Timer_Stop(FEIYU_MODE_SET, MODE_SET_TIMER);

            break;
        case HIGV_TOUCH_END:
            HI_GV_Timer_Start(FEIYU_MODE_SET, MODE_SET_TIMER); 
            config_flag = HI_FALSE;
            break;
        case HIGV_TOUCH_MOVE:
            afterTempy = s_tempy - (point_y - baseY);
            config_flag = HI_TRUE;      
            break;
        default:
            break;
    }
    
    //if(config_flag && (MODE_SET_LABLE == hWidget))
        //HI_GV_ScrollView_SetViewCoordinate(FEIYU_MODE_SET_scrollview, tempx, afterTempy);    
    
    return HIGV_PROC_GOON;

}
HI_S32 FEIYU_MODE_SET_ontouchaction_3(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    if(s_current_subpage_1 < 0)
        return HIGV_PROC_GOON;
    if(cmdList.flag < 5)
         return HIGV_PROC_GOON;    
    HI_S32 point_x, point_y;
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;
    HI_S32 tempy =0, tempx =0, afterTempy = 0;
    //HI_BOOL config_flag = HI_FALSE;
    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));
    static HI_S32 baseX = 0;
    static HI_S32 baseY = 0;
    static HI_S32 s_tempy = 0;
    point_x = touchEvent.last.x;
    point_y = touchEvent.last.y;


    type = touchEvent.last.type;
    switch(type)
    {
        case HIGV_TOUCH_START:
            baseX = point_x;
            baseY = point_y;
			Feiyu_Hide_User_Guider();
            HI_GV_ScrollView_GetViewCoordinate((View_2[s_s32Item_1])[0], &tempx, &s_tempy);   
            config_flag = HI_FALSE;
            HI_GV_Timer_Stop(FEIYU_MODE_SET, MODE_SET_TIMER);

            break;
        case HIGV_TOUCH_END:
            HI_GV_Timer_Start(FEIYU_MODE_SET, MODE_SET_TIMER); 
            config_flag = HI_FALSE;
            positionChange = HI_TRUE;
            break;
        case HIGV_TOUCH_MOVE:
            afterTempy = s_tempy - (point_y - baseY);
            config_flag = HI_TRUE;            
            break;
        default:
            break;
    }
    //MLOGE("type=%d,point_y=%d,s_tempy=%d,afterTempy=%d\n",type,point_y,s_tempy,afterTempy);
    if(config_flag)
    {
        if(afterTempy >= 0)
        HI_GV_ScrollView_SetViewCoordinate((View_2[s_s32Item_1])[0], tempx, afterTempy);
        HI_GV_Widget_Paint((View_2[s_s32Item_1])[0], 0);        
    }

    return HIGV_PROC_GOON;

} 

HI_S32 FEIYU_MODE_SET_ongesturescroll_3(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;

    //MLOGE("###\n");
    if(!s_bSroll)
    {
        MLOGE("waiting ...\n");
        return HIGV_PROC_GOON;
    }

    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32Diff = 0;
    HI_S32 tempy =0, tempx =0, afterTempy = 0;
    HI_S32 i = 0;

    //MLOGD("\n");

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
//    MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);
    HI_GV_ScrollView_GetViewCoordinate((View_2[s_s32Item_1])[0], &tempx, &tempy);
    
    if(s_bIsRunning_3 | s_bIsRunning_2 | s_bIsRunning_1)
    {
        MLOGE("waiting an...\n");
        return HIGV_PROC_GOON;
    }

    HI_S32 s_tempy = 0;
    if(s_tempy == 0)
    {
        HI_GV_ScrollView_GetViewCoordinate((View_2[s_s32Item_1])[0], &tempx, &tempy);  
        s_tempy = tempy;
    }
    
    s_tempy = s_tempy + (startY-endY);
    
    HI_GV_ScrollView_SetViewCoordinate((View_2[s_s32Item_1])[0], tempx, s_tempy);

    return HIGV_PROC_GOON;
}


HI_S32 FEIYU_MODE_SET_ongesturescroll_2(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
    //MLOGE("###\n");
    if(!s_bSroll)
    {
        MLOGE("waiting ...\n");
        return HIGV_PROC_GOON;
    }

    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32Diff = 0;
    HI_S32 tempy =0, tempx =0, afterTempy = 0;
    HI_S32 i = 0;

    //MLOGD("\n");

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
    //MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);
    HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy);
    
    if(s_bIsRunning_3 | s_bIsRunning_2 | s_bIsRunning_1)
    {
        MLOGE("waiting an...\n");
        return HIGV_PROC_GOON;
    }


    HI_S32 s_tempy = 0;
    if(s_tempy == 0)
    {
        HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy);  
        s_tempy = tempy;
    }
    s_tempy = s_tempy + (startY-endY);


    HI_GV_ScrollView_SetViewCoordinate((View_1[s_s32Item_0])[0], tempx, s_tempy);

    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_OnGestureFling_2(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{

    return HIGV_PROC_GOON;

    if(!s_bSroll)
    {
        MLOGE("waiting ...\n");
        return HIGV_PROC_GOON;
    }

    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32Diff = 0;
    HI_S32 tempy =0, tempx =0, afterTempx = 0;
    HI_S32 i = 0;

    //MLOGD("\n");

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
    //MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);
    HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy);
    if(s_bIsRunning_3 | s_bIsRunning_2 | s_bIsRunning_1)
    {
        MLOGE("waiting an...\n");
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
        s_s32WhichView = 1;    
        if (startY > endY)//to up
        {
            s_bIsRunning_2 = HI_TRUE;
            moveFrame(tempy, tempy + 40);
            //new_ItemNum = (tempx + 142)/142;

        }
        else  //down
        {
            if(tempy >= 40)
            {
                s_bIsRunning_2 = HI_TRUE;                                
                moveFrame(tempy, tempy - 40);                
                //new_ItemNum = (tempx - 142)/142;                
            }           
        }

    }


    
    return HIGV_PROC_GOON;
}




HI_S32 FEIYU_MODE_SET_OnGestureFling_3(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;

    if(!s_bSroll)
    {
        MLOGE("waiting ...\n");
        return HIGV_PROC_GOON;
    }

    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32Diff = 0;
    HI_S32 tempy =0, tempx =0, afterTempx = 0;
    HI_S32 i = 0;

    //MLOGD("\n");

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
    //MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);
    HI_GV_ScrollView_GetViewCoordinate((View_2[s_s32Item_1])[0], &tempx, &tempy);
    if(s_bIsRunning_3 | s_bIsRunning_2 | s_bIsRunning_1)
    {
        MLOGE("waiting an...\n");
        return HIGV_PROC_GOON;    
    }
    MLOGE("~~~~~~~~~tempx=%d,tempy=%d\n",tempx,tempy);
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
        s_s32WhichView = 2;        
        if (startY > endY)//to up
        {
            s_bIsRunning_3 = HI_TRUE;           
            moveFrame(tempy, tempy + 40);
            //new_ItemNum = (tempx + 142)/142;

        }
        else  //down
        {
            if(tempy >= 40)
            {
                s_bIsRunning_3 = HI_TRUE;                       
                moveFrame(tempy, tempy - 40);                
                //new_ItemNum = (tempx - 142)/142;                
            }   
        }

    }


    
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_ongesturescroll(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("###\n");
    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32Diff = 0;
    HI_S32 tempy =0, tempx =0, afterTempx = 0;
    HI_S32 i = 0;
	HI_BOOL dMoveFlag = HI_FALSE;
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    startX = gestureEvent.gesture.scroll.start.x;
    startY = gestureEvent.gesture.scroll.start.y;
    endX = gestureEvent.gesture.scroll.end.x;
    endY = gestureEvent.gesture.scroll.end.y;
    if(g_startY < 40)
    {
    	dMoveFlag = (endY - g_startY) > 40 ? HI_TRUE : HI_FALSE;
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


            }
            else if((dMoveFlag == HI_TRUE)&&(startY < endY)) //down
            {
            	if(g_pageChangeEnd == HI_TRUE)
            	{
            		g_pageChangeEnd = HI_FALSE;
                	//HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
                	FeiyujumpToHomePage();
            	}
            }

        }        
    }

    return HIGV_PROC_GOON;
}  

HI_S32 FEIYU_MODE_SET_button_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	HIGV_GESTURE_EVENT_S gestureEvent;
    HI_S32 x, y;
    memset(&gestureEvent, 0x0, sizeof(gestureEvent));
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    x = gestureEvent.gesture.tap.pointer.x;
    y = gestureEvent.gesture.tap.pointer.y;

    if((y>0) && (y <40))
    {
        FeiyujumpToHomePage();        
    }
    
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_MODE_SET_ongesturefling(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;

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

        }
        else  //down
        {
        	if(g_pageChangeEnd == HI_TRUE)
        	{
        		g_pageChangeEnd = HI_FALSE;
            	//HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
            	FeiyujumpToHomePage();
			}
        }

    }
    return HIGV_PROC_GOON;
}


HI_S32 getTempXVal(HI_S32 mode)
{
    HI_S32 tempx = 0;
	switch(mode)
	{
		case PHOTO:
            tempx = 0;
            break;
		case PHOTO_TIMER:
            tempx = 142;
            break;
		case PHOTO_DELAY:
            tempx = 142*2;
            break;
		case MULTI_PHOTO:
            tempx = 142*3;
            break;
		case VIDEO:
            tempx = 142*4;
            break;
		case VIDEO_LAPSE:
            tempx = 142*5;
            break;
		case VIDEO_SLOW:     
            tempx = 142*6;
            break;
		case VIDEO_LOOP:  
            tempx = 142*7;
            break;
		case VIDEO_SNAP:
            tempx = 142*8;    
            break;
		default:          
            break;
	}         
    return tempx;
}


HI_S32 FEIYU_MODE_SET_ontimer(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //return HIGV_PROC_GOON;

    s_bSroll = HI_TRUE;
    cmdList.flag++;
    if(cmdList.flag > 2000)
        cmdList.flag = 2000;

    HI_S32 tempx = 0, tempy = 0;    
    #if 1

    if(s_NewWorkMode != -1)
    {
        HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, &tempx, &tempy);
        //if(tempx != getTempXVal(s_NewWorkMode))
        {
            HI_GV_ScrollView_SetViewCoordinate(FEIYU_MODE_SET_scrollview, getTempXVal(s_NewWorkMode), tempy);
            HI_GV_Widget_Paint(FEIYU_MODE_SET_scrollview, 0);
        }
    }
    //MLOGE("==============================\n");
    
    if(s_current_subpage > 0 && s_s32SelectItem1 >= 0)
    {
        HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy);
        if(tempy != s_s32SelectItem1*40)
        {
            HI_GV_ScrollView_SetViewCoordinate((View_1[s_s32Item_0])[0], tempx, s_s32SelectItem1*40); 
            HI_GV_Widget_Paint((View_1[s_s32Item_0])[0], 0);
        }
    }
    if(s_current_subpage_1 > 0 && s_s32SelectItem2 >= 0)
    {
        HI_GV_ScrollView_GetViewCoordinate((View_2[s_s32Item_1])[0], &tempx, &tempy);
        if(tempy != s_s32SelectItem2*40)
        {
            HI_GV_ScrollView_SetViewCoordinate((View_2[s_s32Item_1])[0], tempx, s_s32SelectItem2*40); 
            HI_GV_Widget_Paint((View_2[s_s32Item_1])[0], 0);
        }
    }
    #endif
    {
        static HI_S32 count = 0;
        if((count++)%10 == 0)
        {
            count = 0;
            HI_GV_Widget_Paint(FEIYU_MODE_SET, 0);
        }
    }
    
    HI_GV_Timer_Reset(FEIYU_MODE_SET, MODE_SET_TIMER);
    
    return HIGV_PROC_GOON;
}

static HI_HANDLE s_ItemHandle = 0;

static void Item_Update_handle(HIGV_LISTENER_S* listener, void* data)
{
    HI_RECT rect = {0};
    HI_GV_Widget_GetRect(FEIYU_MODE_SET,&rect);
    HI_GV_Widget_Move(FEIYU_MODE_SET,rect.x,HI_GV_TweenAnimGetTweenValue(s_ItemHandle, 0));
}

static void Item_Start_handle(HIGV_LISTENER_S* listener, void* data)
{
   // MLOGE(">>\n");
}

static void Item_End_handle(HIGV_LISTENER_S* listener, void* data)
{
   // MLOGE(">>\n");
    HI_S32 s32Ret = HI_SUCCESS;
   
    g_pageChangeEnd = HI_TRUE;
    s32Ret |= HI_GV_Widget_Hide(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Hide(FEIYU_ZOOM_PAGE);
    s32Ret |= HI_GV_Widget_Update(FILELIST_PAGE, 0);
    s32Ret |= HI_GV_Widget_Refresh(FILELIST_PAGE, 0);
        
    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("err, s32Ret=%d\n",s32Ret);
    }

}

void ModeSetAnimmoveXItem(HIGV_CORD fromValue,HIGV_CORD toValue)
{
    s_ItemHandle = HI_GV_TweenAnimCreate();
    HI_GV_TweenAnimSetDuration(s_ItemHandle,300);

    HI_GV_TweenAnimAddTween(s_ItemHandle,HIGV_TWEEN_TRANSITION_LINEAR,HIGV_TWEEN_EASE_IN,fromValue,toValue);

    HI_GV_TweenAnimSetExecListener(s_ItemHandle, Item_Update_handle);
        
    HI_GV_TweenAnimSetStartedListener(s_ItemHandle, Item_Start_handle);


    HI_GV_TweenAnimSetFinishedListener(s_ItemHandle, Item_End_handle);


    HI_GV_TweenAnimStart(s_ItemHandle);    

}
#if 0
HI_S32 FEIYU_MODE_SET_scrollview_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 tempx = 0, tempy = 0;
    HIGV_GESTURE_EVENT_S gestureEvent;
    HI_S32 x, y;
    memset(&gestureEvent, 0x0, sizeof(gestureEvent));
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    x = gestureEvent.gesture.tap.pointer.x;
    y = gestureEvent.gesture.tap.pointer.y;
    //MLOGE("X=%d,y=%d\n",x,y);    
    HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, &tempx, &tempy);    

    HI_S32 s32Diff = 0;
    s32Diff = (tempx%142);
    HI_S32 tempx1 = 0;
    tempx1 =tempx - s32Diff;

    HI_S32 s32EndX = 0;
    if(s_bIsRunning_1)
    {
        MLOGE("waiting an...\n");
        return HIGV_PROC_STOP;
    }
    
    if((x < (160 - s32Diff)))
    {
        s_s32WhichView = 0;
        s_bIsRunning_1 = HI_TRUE;
        s32EndX = tempx1 - 142;
        if(s32EndX < 0)
            s32EndX = 0;
        
        new_ItemNum = s32EndX/142;  
        moveFrame(tempx, s32EndX); 
    }
    else if( (x > (160 - s32Diff)) && (x < (320-s32Diff)))
    {
        s_s32WhichView = 0;
        s_bIsRunning_1 = HI_TRUE;
        
        new_ItemNum = (tempx)/142;   
        moveFrame(tempx, tempx);
    }
    else if((x > (320-s32Diff)) && (x < (480 - s32Diff)))
    {
        s_s32WhichView = 0;
        s_bIsRunning_1 = HI_TRUE;
        s32EndX = tempx1 + 142;
        
        if(s32EndX > 1204)
            s32EndX = 1204;
        new_ItemNum = s32EndX/142;
        moveFrame(tempx, s32EndX);
    }    
    else if(x > (480 - s32Diff))
    {
        s_s32WhichView = 0;
        s_bIsRunning_1 = HI_TRUE;
        s32EndX = tempx1 + 142*2;
        if(s32EndX > 1204)
            s32EndX = 1204;        
        
        new_ItemNum = s32EndX/142;  
        moveFrame(tempx, s32EndX);
    }
    MLOGE("==================>>>>new_ItemNum=%d\n",new_ItemNum);
    return HIGV_PROC_STOP;
}
#endif

static HI_VOID doWorkMode(void *arg)
{
    MLOGE("new_ItemNum=%d\n",new_ItemNum);
     switch(s_s32CurrentWorkMode)
     {
        case PHOTO:FeiYuCmdExecute(CMD_SET_WORKMODE_PHOTO, 0, NULL);break;
        case PHOTO_TIMER:FeiYuCmdExecute(CMD_SET_WORKMODE_PHOTO_TIMER, 0, NULL);break;
        case PHOTO_DELAY:FeiYuCmdExecute(CMD_SET_WORKMODE_PHOTO_DELAY, 0, NULL);break;
        case MULTI_PHOTO:FeiYuCmdExecute(CMD_SET_WORKMODE_MULIT_PHOTO, 0, NULL);break;
        case VIDEO_SNAP:FeiYuCmdExecute(CMD_SET_WORKMODE_VIDEO_SNAP, 0, NULL);break;
        case VIDEO:FeiYuCmdExecute(CMD_SET_WORKMODE_VIDEO, 0, NULL);break;
        case VIDEO_LAPSE:FeiYuCmdExecute(CMD_SET_WORKMODE_VIDEO_LAPSE, 0, NULL);break;
        case VIDEO_SLOW:FeiYuCmdExecute(CMD_SET_WORKMODE_VIDEO_SLOW, 0, NULL);break;
        case VIDEO_LOOP:FeiYuCmdExecute(CMD_SET_WORKMODE_VIDEO_LOOP, 0, NULL);break;
        default: break;
     }
     s_bIsRunning_1 = HI_FALSE;
     s_s32Item = new_ItemNum;

    return 0;
}


HI_S32 FEIYU_MODE_SET_scrollview_button_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
    MLOGE("###");
    HI_S32 tempx = 0, tempx1= 0, tempy= 0;
    
    switch(hWidget)
    {
        case FEIYU_MODE_SET_scrollview_NP_1:
        case FEIYU_MODE_SET_scrollview_NP_2:  
            s_s32CurrentWorkMode = PHOTO;
            tempx = 0;
            break;
        case FEIYU_MODE_SET_scrollview_TP_1:
        case FEIYU_MODE_SET_scrollview_TP_2:  
            s_s32CurrentWorkMode = PHOTO_TIMER;   
            tempx = 142;
            break;
        case FEIYU_MODE_SET_scrollview_DP_1:
        case FEIYU_MODE_SET_scrollview_DP_2:
            s_s32CurrentWorkMode = PHOTO_DELAY;  
            tempx = 142*2;
            break;
        case FEIYU_MODE_SET_scrollview_MP_1:
        case FEIYU_MODE_SET_scrollview_MP_2:
            s_s32CurrentWorkMode = MULTI_PHOTO; 
            tempx = 142*3;
            break;
        case FEIYU_MODE_SET_scrollview_NV_1:
        case FEIYU_MODE_SET_scrollview_NV_2:            
            s_s32CurrentWorkMode = VIDEO;  
            tempx = 142*4;
            break;
        case FEIYU_MODE_SET_scrollview_DV_1:
        case FEIYU_MODE_SET_scrollview_DV_2:
            s_s32CurrentWorkMode = VIDEO_LAPSE; 
            tempx = 142*5;
            break;
        case FEIYU_MODE_SET_scrollview_SV_1:
        case FEIYU_MODE_SET_scrollview_SV_2:
            s_s32CurrentWorkMode = VIDEO_SLOW;  
            tempx = 142*6;
            break;
        case FEIYU_MODE_SET_scrollview_LV_1:
        case FEIYU_MODE_SET_scrollview_LV_2:
            s_s32CurrentWorkMode = VIDEO_LOOP;
            tempx = 142*7;
            break;
        case FEIYU_MODE_SET_scrollview_VP_1:
        case FEIYU_MODE_SET_scrollview_VP_2:
            s_s32CurrentWorkMode = VIDEO_SNAP;
            tempx = 142*8;
            break;


    }
    HI_S32 s32Ret = 0;
    pthread_t threadid = 0;
    s32Ret = pthread_create(&threadid, NULL, (void *)doWorkMode, NULL);
    if(0 != s32Ret)
    {
        MLOGE("pthread_create Failed, s32Ret=%#x\n", s32Ret);
    }     
    refreshModeLabel(s_s32CurrentWorkMode);   
    HI_GV_ScrollView_GetViewCoordinate(FEIYU_MODE_SET_scrollview, &tempx1, &tempy);
    s_s32WhichView = 0;
    s_bIsRunning_1 = HI_TRUE;
    moveFrame(tempx1, tempx);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

