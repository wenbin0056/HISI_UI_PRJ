
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

extern SubPageItemDes sa_SubPageItemDes[MAX_SUBPAGE];
static HI_S32 s_current_subpage = SUBPAGE_PHOTO_MODE_PARAM;
static HI_S32 s_current_subpage_1 = SUBPAGE_PHOTO_PARAM_IMAGE_MODE;



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

#define MAX_ITEM 18
static HI_BOOL bIsRunning = HI_TRUE;
static HI_S32 s_s32Item_0 = -1;
static HI_S32 s_s32Item_1 = -1;
static HI_S32 s_isRefresh_2 = HI_FALSE;
#define LEN 16
#define BASE 13
#define LEN_1 16
extern HI_BOOL g_pageChangeEnd;
static HI_S32 s_UpdateView2Flag = 0;
static HI_BOOL s_bIsRunning_1 = HI_FALSE;
static HI_BOOL s_bIsRunning_2 = HI_FALSE;
static HI_S32 s_s32CurrentNum1 = 0;
static HI_S32 s_s32CurrentNum2 = 0;
static HI_BOOL s_firstEntry = HI_FALSE;
static HI_BOOL s_firstEntry_1 = HI_FALSE;
//static HI_S32 s_s32Filter = 0;
static HI_S32 s_s32OnShowWaitTime = 0;
static HI_BOOL s_userinput_flag2 = HI_FALSE;
#define WAIT_TIME_COUNT 10


static HI_S32 ScrollView_1_10[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_1_10,
    FEIYU_FAST_SET_scrollview_1_10_1,
    FEIYU_FAST_SET_scrollview_1_10_2,
    FEIYU_FAST_SET_scrollview_1_10_3,
    FEIYU_FAST_SET_scrollview_1_10_4,
    FEIYU_FAST_SET_scrollview_1_10_5,    
    FEIYU_FAST_SET_scrollview_1_10_6,
    FEIYU_FAST_SET_scrollview_1_10_7,
    FEIYU_FAST_SET_scrollview_1_10_8,
    FEIYU_FAST_SET_scrollview_1_10_9,
    FEIYU_FAST_SET_scrollview_1_10_10,    
};


static HI_S32 ScrollView_1_11[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_1_11,
    FEIYU_FAST_SET_scrollview_1_11_1,
    FEIYU_FAST_SET_scrollview_1_11_2,
    FEIYU_FAST_SET_scrollview_1_11_3,
    FEIYU_FAST_SET_scrollview_1_11_4,
    FEIYU_FAST_SET_scrollview_1_11_5,    
    FEIYU_FAST_SET_scrollview_1_11_6,
    FEIYU_FAST_SET_scrollview_1_11_7,
    FEIYU_FAST_SET_scrollview_1_11_8,
    FEIYU_FAST_SET_scrollview_1_11_9,
    FEIYU_FAST_SET_scrollview_1_11_10,    
    FEIYU_FAST_SET_scrollview_1_11_11,           
};


static HI_S32 ScrollView_1_12[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_1_12,
    FEIYU_FAST_SET_scrollview_1_12_1,
    FEIYU_FAST_SET_scrollview_1_12_2,
    FEIYU_FAST_SET_scrollview_1_12_3,
    FEIYU_FAST_SET_scrollview_1_12_4,
    FEIYU_FAST_SET_scrollview_1_12_5,    
    FEIYU_FAST_SET_scrollview_1_12_6,
    FEIYU_FAST_SET_scrollview_1_12_7,
    FEIYU_FAST_SET_scrollview_1_12_8,
    FEIYU_FAST_SET_scrollview_1_12_9,
    FEIYU_FAST_SET_scrollview_1_12_10,    
    FEIYU_FAST_SET_scrollview_1_12_11,           
	FEIYU_FAST_SET_scrollview_1_12_12,             	
};

static HI_S32 ScrollView_1_13[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_1_13,
    FEIYU_FAST_SET_scrollview_1_13_1,
    FEIYU_FAST_SET_scrollview_1_13_2,
    FEIYU_FAST_SET_scrollview_1_13_3,
    FEIYU_FAST_SET_scrollview_1_13_4,
    FEIYU_FAST_SET_scrollview_1_13_5,    
    FEIYU_FAST_SET_scrollview_1_13_6,
    FEIYU_FAST_SET_scrollview_1_13_7,
    FEIYU_FAST_SET_scrollview_1_13_8,
    FEIYU_FAST_SET_scrollview_1_13_9,
    FEIYU_FAST_SET_scrollview_1_13_10,    
    FEIYU_FAST_SET_scrollview_1_13_11,           
	FEIYU_FAST_SET_scrollview_1_13_12,           
	FEIYU_FAST_SET_scrollview_1_13_13,           	
};

static HI_S32 ScrollView_1_14[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_1_14,
    FEIYU_FAST_SET_scrollview_1_14_1,
    FEIYU_FAST_SET_scrollview_1_14_2,
    FEIYU_FAST_SET_scrollview_1_14_3,
    FEIYU_FAST_SET_scrollview_1_14_4,
    FEIYU_FAST_SET_scrollview_1_14_5,    
    FEIYU_FAST_SET_scrollview_1_14_6,
    FEIYU_FAST_SET_scrollview_1_14_7,
    FEIYU_FAST_SET_scrollview_1_14_8,
    FEIYU_FAST_SET_scrollview_1_14_9,
    FEIYU_FAST_SET_scrollview_1_14_10,    
    FEIYU_FAST_SET_scrollview_1_14_11,           
	FEIYU_FAST_SET_scrollview_1_14_12,           
	FEIYU_FAST_SET_scrollview_1_14_13,           	
	FEIYU_FAST_SET_scrollview_1_14_14,       
};

static HI_S32 ScrollView_1_15[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_1_15,
    FEIYU_FAST_SET_scrollview_1_15_1,
    FEIYU_FAST_SET_scrollview_1_15_2,
    FEIYU_FAST_SET_scrollview_1_15_3,
    FEIYU_FAST_SET_scrollview_1_15_4,
    FEIYU_FAST_SET_scrollview_1_15_5,    
    FEIYU_FAST_SET_scrollview_1_15_6,
    FEIYU_FAST_SET_scrollview_1_15_7,
    FEIYU_FAST_SET_scrollview_1_15_8,
    FEIYU_FAST_SET_scrollview_1_15_9,
    FEIYU_FAST_SET_scrollview_1_15_10,    
    FEIYU_FAST_SET_scrollview_1_15_11,           
	FEIYU_FAST_SET_scrollview_1_15_12,           
	FEIYU_FAST_SET_scrollview_1_15_13,           	
	FEIYU_FAST_SET_scrollview_1_15_14,       
	FEIYU_FAST_SET_scrollview_1_15_15,       	
};


static HI_S32 ScrollView_1_16[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_1_16,
    FEIYU_FAST_SET_scrollview_1_16_1,
    FEIYU_FAST_SET_scrollview_1_16_2,
    FEIYU_FAST_SET_scrollview_1_16_3,
    FEIYU_FAST_SET_scrollview_1_16_4,
    FEIYU_FAST_SET_scrollview_1_16_5,    
    FEIYU_FAST_SET_scrollview_1_16_6,
    FEIYU_FAST_SET_scrollview_1_16_7,
    FEIYU_FAST_SET_scrollview_1_16_8,
    FEIYU_FAST_SET_scrollview_1_16_9,
    FEIYU_FAST_SET_scrollview_1_16_10,    
    FEIYU_FAST_SET_scrollview_1_16_11,
    FEIYU_FAST_SET_scrollview_1_16_12,
    FEIYU_FAST_SET_scrollview_1_16_13,
    FEIYU_FAST_SET_scrollview_1_16_14,            
	FEIYU_FAST_SET_scrollview_1_16_15,            
	FEIYU_FAST_SET_scrollview_1_16_16,            
};




static HI_S32 *View_1[MAX_ITEM] = 
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ScrollView_1_10,    
    ScrollView_1_11,
    ScrollView_1_12,
    ScrollView_1_13,
    ScrollView_1_14,
    ScrollView_1_15,
    ScrollView_1_16,
    NULL,
    NULL    
};

static HI_S32 ScrollView_2_5[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_2_5,
    FEIYU_FAST_SET_scrollview_2_5_1,
    FEIYU_FAST_SET_scrollview_2_5_2,
    FEIYU_FAST_SET_scrollview_2_5_3,
    FEIYU_FAST_SET_scrollview_2_5_4,
    FEIYU_FAST_SET_scrollview_2_5_5,    
};


static HI_S32 ScrollView_2_6[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_2_6,
    FEIYU_FAST_SET_scrollview_2_6_1,
    FEIYU_FAST_SET_scrollview_2_6_2,
    FEIYU_FAST_SET_scrollview_2_6_3,
    FEIYU_FAST_SET_scrollview_2_6_4,
    FEIYU_FAST_SET_scrollview_2_6_5,    
    FEIYU_FAST_SET_scrollview_2_6_6,
    
};


static HI_S32 ScrollView_2_7[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_2_7,
    FEIYU_FAST_SET_scrollview_2_7_1,
    FEIYU_FAST_SET_scrollview_2_7_2,
    FEIYU_FAST_SET_scrollview_2_7_3,
    FEIYU_FAST_SET_scrollview_2_7_4,
    FEIYU_FAST_SET_scrollview_2_7_5,    
    FEIYU_FAST_SET_scrollview_2_7_6,
    FEIYU_FAST_SET_scrollview_2_7_7,    
};

static HI_S32 ScrollView_2_8[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_2_8,
    FEIYU_FAST_SET_scrollview_2_8_1,
    FEIYU_FAST_SET_scrollview_2_8_2,
    FEIYU_FAST_SET_scrollview_2_8_3,
    FEIYU_FAST_SET_scrollview_2_8_4,
    FEIYU_FAST_SET_scrollview_2_8_5,    
    FEIYU_FAST_SET_scrollview_2_8_6,
	FEIYU_FAST_SET_scrollview_2_8_7,
	FEIYU_FAST_SET_scrollview_2_8_8,
};
static HI_S32 ScrollView_2_9[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_2_9,
    FEIYU_FAST_SET_scrollview_2_9_1,
    FEIYU_FAST_SET_scrollview_2_9_2,
    FEIYU_FAST_SET_scrollview_2_9_3,
    FEIYU_FAST_SET_scrollview_2_9_4,
    FEIYU_FAST_SET_scrollview_2_9_5,    
    FEIYU_FAST_SET_scrollview_2_9_6,
	FEIYU_FAST_SET_scrollview_2_9_7,
	FEIYU_FAST_SET_scrollview_2_9_8,
	FEIYU_FAST_SET_scrollview_2_9_9,
};

static HI_S32 ScrollView_2_10[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_2_10,
    FEIYU_FAST_SET_scrollview_2_10_1,
    FEIYU_FAST_SET_scrollview_2_10_2,
    FEIYU_FAST_SET_scrollview_2_10_3,
    FEIYU_FAST_SET_scrollview_2_10_4,
    FEIYU_FAST_SET_scrollview_2_10_5,    
    FEIYU_FAST_SET_scrollview_2_10_6,
    FEIYU_FAST_SET_scrollview_2_10_7,
    FEIYU_FAST_SET_scrollview_2_10_8,
    FEIYU_FAST_SET_scrollview_2_10_9,
    FEIYU_FAST_SET_scrollview_2_10_10,    


};
static HI_S32 ScrollView_2_11[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_2_11,
    FEIYU_FAST_SET_scrollview_2_11_1,
    FEIYU_FAST_SET_scrollview_2_11_2,
    FEIYU_FAST_SET_scrollview_2_11_3,
    FEIYU_FAST_SET_scrollview_2_11_4,
    FEIYU_FAST_SET_scrollview_2_11_5,    
    FEIYU_FAST_SET_scrollview_2_11_6,
    FEIYU_FAST_SET_scrollview_2_11_7,
    FEIYU_FAST_SET_scrollview_2_11_8,
    FEIYU_FAST_SET_scrollview_2_11_9,
    FEIYU_FAST_SET_scrollview_2_11_10,    
	FEIYU_FAST_SET_scrollview_2_11_11, 


};
static HI_S32 ScrollView_2_13[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_2_13,
    FEIYU_FAST_SET_scrollview_2_13_1,
    FEIYU_FAST_SET_scrollview_2_13_2,
    FEIYU_FAST_SET_scrollview_2_13_3,
    FEIYU_FAST_SET_scrollview_2_13_4,
    FEIYU_FAST_SET_scrollview_2_13_5,    
    FEIYU_FAST_SET_scrollview_2_13_6,
    FEIYU_FAST_SET_scrollview_2_13_7,
    FEIYU_FAST_SET_scrollview_2_13_8,
    FEIYU_FAST_SET_scrollview_2_13_9,
    FEIYU_FAST_SET_scrollview_2_13_10,    
    FEIYU_FAST_SET_scrollview_2_13_11,
    FEIYU_FAST_SET_scrollview_2_13_12,
    FEIYU_FAST_SET_scrollview_2_13_13,

};

static HI_S32 ScrollView_2_14[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_2_14,
    FEIYU_FAST_SET_scrollview_2_14_1,
    FEIYU_FAST_SET_scrollview_2_14_2,
    FEIYU_FAST_SET_scrollview_2_14_3,
    FEIYU_FAST_SET_scrollview_2_14_4,
    FEIYU_FAST_SET_scrollview_2_14_5,    
    FEIYU_FAST_SET_scrollview_2_14_6,
    FEIYU_FAST_SET_scrollview_2_14_7,
    FEIYU_FAST_SET_scrollview_2_14_8,
    FEIYU_FAST_SET_scrollview_2_14_9,
    FEIYU_FAST_SET_scrollview_2_14_10,    
    FEIYU_FAST_SET_scrollview_2_14_11,
    FEIYU_FAST_SET_scrollview_2_14_12,
    FEIYU_FAST_SET_scrollview_2_14_13,
    FEIYU_FAST_SET_scrollview_2_14_14,

};

static HI_S32 ScrollView_2_15[MAX_ITEM] = 
{
    FEIYU_FAST_SET_scrollview_2_15,
    FEIYU_FAST_SET_scrollview_2_15_1,
    FEIYU_FAST_SET_scrollview_2_15_2,
    FEIYU_FAST_SET_scrollview_2_15_3,
    FEIYU_FAST_SET_scrollview_2_15_4,
    FEIYU_FAST_SET_scrollview_2_15_5,    
    FEIYU_FAST_SET_scrollview_2_15_6,
    FEIYU_FAST_SET_scrollview_2_15_7,
    FEIYU_FAST_SET_scrollview_2_15_8,
    FEIYU_FAST_SET_scrollview_2_15_9,
    FEIYU_FAST_SET_scrollview_2_15_10,    
    FEIYU_FAST_SET_scrollview_2_15_11,
    FEIYU_FAST_SET_scrollview_2_15_12,
    FEIYU_FAST_SET_scrollview_2_15_13,
    FEIYU_FAST_SET_scrollview_2_15_14,
	FEIYU_FAST_SET_scrollview_2_15_15,

};


static HI_S32 *View_2[MAX_ITEM] = 
{
    NULL,
    NULL,
    NULL,
    NULL,
    ScrollView_2_5,
    ScrollView_2_6,
    ScrollView_2_7,
    ScrollView_2_8,
    ScrollView_2_9,
    ScrollView_2_10,
    ScrollView_2_11,
    NULL,
    ScrollView_2_13,    
    ScrollView_2_14,
    ScrollView_2_15,    
    NULL,
    NULL,        
    NULL    
};

static HI_S32 updateUI(HI_S32 flag );


static HI_VOID UI_ShowHomePage()
{
	HI_S32 s32Ret = HI_SUCCESS;
	
	s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
	s32Ret |= HI_GV_Widget_MoveToTop(HOME_PAGE);        
	s32Ret |= HI_GV_Widget_Hide(FEIYU_FAST_SET);
	s32Ret |= HI_GV_Widget_Hide(FAST_SET_LABEL);
	s32Ret |= HI_GV_Widget_Hide(FAST_SET_LABEL_1);            

	if (HI_SUCCESS != s32Ret)
	{
		MLOGE("HIGV Error: %#x\n", s32Ret);
	}  
}

HI_S32 FEIYU_FAST_SET_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_FAST_SET_OnHide(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    bIsRunning = HI_FALSE;
    s_firstEntry = HI_FALSE;
    s_firstEntry_1 = HI_FALSE;    
    s_s32OnShowWaitTime = 0;

    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_FAST_SET_OnHide_ontimer(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    //return HIGV_PROC_GOON;
    s_s32OnShowWaitTime++;
    if(s_s32OnShowWaitTime > 100)
        s_s32OnShowWaitTime = 100;

    HI_S32 tempx = 0, tempy = 0;
    static HI_S32 s_pretempy1 = 0;
    static HI_S32 s_pretempy2 = 0;    
    
    HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy);    
    if(s_pretempy1 != tempy)
    {
        HI_GV_ScrollView_SetViewCoordinate((View_1[s_s32Item_0])[0], tempx, (s_s32CurrentNum1-2)*64);
        
        s_pretempy1 = tempy;
        
        
    }
    HI_GV_Widget_Paint((View_1[s_s32Item_0])[0], 0);
    HI_GV_ScrollView_GetViewCoordinate((View_2[s_s32Item_1])[0], &tempx, &tempy);    
    
    if(s_pretempy2 != tempy)
    {
        HI_GV_ScrollView_SetViewCoordinate((View_2[s_s32Item_1])[0], tempx, (s_s32CurrentNum2-2)*64);  
        
        s_pretempy2 = tempy;
        
               
    }
    //HI_GV_Widget_Paint((View_2[s_s32Item_1])[0], 0); 

    HI_GV_Widget_Paint(FEIYU_FAST_SET, 0);     
    HI_GV_Timer_Reset(FEIYU_FAST_SET, FASTSET_PAGE_TIMER);

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


static HI_VOID doMove(void *arg)
{
    HIGV_CORD tempx =0, tempy = 0;
    HI_S32 moveNum = 0;
    HI_S32 i = 0;
    HI_S32 s32ValX = 0;
    HI_DOUBLE movediff = 0;
    static HIGV_CORD s_tempy = -1;
    static HIGV_CORD s_tempy_1 = -1;
    static HI_S32 s_flag = 0;
    HI_S32 s32Ret = 0;
    HI_HANDLE skinId = background_skin;
    static HI_U16 preu16CmdID = 0;
    static HI_U16 preCmdParam = 0;     
    static HI_S32 s_counter = 0;    
    usleep(1000);

    while(bIsRunning)
    {
        if(s_s32Item_0 > 0)
        {
            HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &tempy);
            tempy += 32;
            if(s_tempy != tempy)
            {
                //MLOGE("tempy=%d\n",tempy);
                s_tempy = tempy;
                s_flag = 0;
            }
            else
            {
                s_flag++;
                moveNum = tempy / 64;
                if(!s_firstEntry)
                {
                    //if(s_flag > 2)
                    //usleep(50*1000);
                    goto page2;
                }
                else
                {
                    s_firstEntry = HI_FALSE;
                     HI_GV_ScrollView_SetViewCoordinate((View_1[s_s32Item_0])[0], tempx, 2*64);                    
                }
            }
            
            moveNum = tempy / 64;
            movediff = tempy - moveNum*64 - 32;
            //MLOGE("movediff=%d")
            for(i = moveNum -1; i < moveNum +6; i++)
            {
                if(i < 0)
                    continue;
                
                s32ValX = (HI_S32)(LEN * (movediff/64));
                
                if( (moveNum -1)== i )
                {
                    s32ValX += LEN * 3;
                    skinId = background_skin_fast;
                }
                else if((moveNum )== i )
                {
                    s32ValX += LEN * 2;
                    skinId = background_skin_fast;                    
                }            
                else if((moveNum +1)== i )
                {
                    s32ValX += LEN * 1;
                    skinId = background_skin_fast;                    
                }
                else if((moveNum +2)== i )
                {
                    skinId = background_skin; 
                    #if 1
                    if(SUBPAGE_PHOTOTIMER_MODE_PARAM == s_current_subpage||
                        SUBPAGE_PHOTODELAY_MODE_PARAM == s_current_subpage ||
                        SUBPAGE_VIDEOSNAP_MODE_PARAM == s_current_subpage ||
                        SUBPAGE_VIDEOLOOP_MODE_PARAM == s_current_subpage ||
                        SUBPAGE_VIDEOLAPSE_MODE_PARAM == s_current_subpage ||
                        SUBPAGE_MULTIPHOTO_MODE_PARAM == s_current_subpage)
                    {
                        s_current_subpage_1 = s_current_subpage + i + 1;
                    }
                    else
                    #endif
                    {
                        s_current_subpage_1 = s_current_subpage + i;   

                    }
                    s_s32CurrentNum1 = moveNum +2;
                    //MLOGE("==========================================>>>i=%d\n",i);


                }
                else if((moveNum +3)== i )
                {
                    s32ValX = LEN - s32ValX;
                    skinId = background_skin_fast;                                        
                }       
                else if((moveNum +4)== i )
                {
                    s32ValX = 2*LEN - s32ValX;
                    skinId = background_skin_fast;                                        
                }  
                else if((moveNum +5)== i )
                {
                    s32ValX = 3*LEN - s32ValX;
                    skinId = background_skin_fast;                                        
                }                  
                
                if((View_1[s_s32Item_0])[i+1] > 0)
                {
                    s32ValX = abs(s32ValX);                
                    //MLOGE("i=%d,s32ValX=%d\n",i,s32ValX);
                    s32ValX += BASE;
                    s32Ret |=HI_GV_Widget_SetSkin((View_1[s_s32Item_0])[i+1], HIGV_SKIN_NORMAL,skinId);                                            
                    HI_GV_Widget_SetMargin((View_1[s_s32Item_0])[i+1], s32ValX, 0, 0, 0);                
                    
                }


            }
            //MLOGE("#######################\n");
            HI_GV_Widget_Paint((View_1[s_s32Item_0])[0], 0);
            //updateUI(0);
            static HI_S32 pre_s_current_subpage_1 = 0;
            if(pre_s_current_subpage_1 == 0)
                pre_s_current_subpage_1 = s_current_subpage_1;

            if(pre_s_current_subpage_1 != s_current_subpage_1)
            {
                s_isRefresh_2 = HI_TRUE;
                //bIsRunning = HI_FALSE;
                s_UpdateView2Flag++;
                preu16CmdID = 0;
                preCmdParam = 0;
                s_counter = 0;
                s_userinput_flag2 = HI_FALSE;
                HI_GV_Msg_SendAsync(FEIYU_FAST_SET, HIGV_MSG_EVENT, UPDATE_FAST_SET, 0);   
                pre_s_current_subpage_1 = s_current_subpage_1;
                //goto end;
            }
            
        }
        if(s_UpdateView2Flag != 0)
        {
            usleep(100);
            continue;
        }
        
        //usleep(10*1000);            
        
page2:

        if(s_s32Item_1 > 0)
        {
            HI_GV_ScrollView_GetViewCoordinate((View_2[s_s32Item_1])[0], &tempx, &tempy);
            tempy += 32;
            //MLOGE("===================tempy=%d\n",tempy);            
            if(s_tempy_1 != tempy)
            {
                //MLOGE("===================tempy=%d\n",tempy);
                s_tempy_1 = tempy;
            }
            else
            {
                if(s_firstEntry_1)
                {
                    s_firstEntry_1 = HI_FALSE;
                    HI_S32 retVal = 0;
                    FeiYuCmdExecute(CMD_GET_PHOTO_SCENE, 0, &retVal); 
                    if((SUBPAGE_PHOTO_PARAM_EXPOSURE == s_current_subpage_1 )&& 
                    (retVal == PHOTO_SCENE_HDR || retVal == PHOTO_SCENE_AUTO ||retVal == PHOTO_SCENE_NORMAL))
                    {
                    }                    
                    else
                    {
                        HI_S32 s32GetCmd = sa_SubPageItemDes[s_current_subpage_1].getCmdid;
                        HI_S32 s32State = 0;
                        FeiYuCmdExecute(s32GetCmd, 0, &s32State);
                        i = 0;

                        do{
                            if( sa_SubPageItemDes[s_current_subpage_1].FeiyuCmdParam[i] == s32State)
                                break;
                            i++;
                        }while(sa_SubPageItemDes[s_current_subpage_1].FeiyuCmdParam[i] > 0);
                        
                        if((View_2[s_s32Item_1])[i+1+2] > 0)
                        {
                            MLOGE("~~~~~~i =%d, s_s32Item_1=%d\n",i,s_s32Item_1);
                            s32Ret |=HI_GV_Widget_SetSkin((View_2[s_s32Item_1])[i+1+2], HIGV_SKIN_NORMAL,background_skin); 
                        }
                        HI_S32 setTempy = 0;
                        if(i >= 1)
                            setTempy = 64*(i);
                        else
                            setTempy = 0;
                        //HI_GV_ScrollView_SetViewCoordinate((View_2[s32ItemNum -1])[0], 0, (i+2)*64);           
                        HI_S32 * p = NULL;
                        p = View_2[s_s32Item_1]; 
                        do
                        {
                            HI_GV_ScrollView_SetViewCoordinate(p[0], tempx, setTempy);  
                            HI_GV_ScrollView_GetViewCoordinate(p[0], &tempx, &tempy);
                            HI_GV_Widget_Refresh(p[0], 0);
                            HI_GV_Widget_Update(p[0], 0);
                            s32Ret |= HI_GV_Widget_Paint(p[0], 0);                
                            MLOGE("--------->>>tempy=%d\n",tempy);
                        }while((i++ < 200) && (tempy != setTempy));  
                    }
                    s_tempy_1 = 0;
                    goto end;
                }
                else
                {
                    usleep(50*1000);
                    if(s_counter++ > 5)
                    {
                        s_counter = 201;
                        goto end;                    
                    }
                    else
                    {
                        preu16CmdID = 0;
                        preCmdParam = 0;                     
                    }
                }

            }          

            moveNum = tempy / 64;
            movediff = tempy - moveNum*64 - 32;      
            for(i = moveNum -1; i < moveNum +6; i++)
            {
                if(i < 0)
                    continue;
                
                s32ValX = (HI_S32)(LEN_1 * (movediff/64));
                
                if( (moveNum -1)== i )
                {
                    s32ValX += LEN_1 * 3;
                    skinId = background_skin_fast;
                }
                else if((moveNum )== i )
                {
                    s32ValX += LEN_1 * 2;
                    skinId = background_skin_fast;                    
                }            
                else if((moveNum +1)== i )
                {
                    s32ValX += LEN_1 * 1;
                    skinId = background_skin_fast;                    
                }
                else if((moveNum +2)== i )
                {
                    skinId = background_skin;                         
                    HI_U16 u16CmdID = 0;
                    HI_U16 u16CmdParam = 0;            
                    u16CmdID = sa_SubPageItemDes[s_current_subpage_1].setCmdid;
                    u16CmdParam = sa_SubPageItemDes[s_current_subpage_1].FeiyuCmdParam[i-2];
   
                    //if(!s_isRefresh_2)
                    {
                        if(u16CmdID > 0 && u16CmdParam > 0)
                        {
                            if(preCmdParam != u16CmdParam)
                            {
                                //MLOGE("preCmdParam=%d, u16CmdParam=%d\n",preCmdParam,u16CmdParam);
                                if(preu16CmdID != 0 && preCmdParam != 0)
                                {
                                    MLOGE("==============================================================\n");
                                    MLOGE("u16CmdID=%d,u16CmdParam=%d,PHOTO_WB_AUTO=%d\n",u16CmdID,u16CmdParam,PHOTO_WB_AUTO);                                        
                                    if(s_userinput_flag2)
                                    {
                                        FeiYuCmdExecute(u16CmdID, u16CmdParam, NULL);  
                                        //s_userinput_flag2 = HI_FALSE;
                                    }
                                    else
                                    {
                                        MLOGE("drop cmd!!!\n");   
                                    }
                                    MLOGE("==============================================================\n");                                    
                                }
                                preu16CmdID = u16CmdID;
                                preCmdParam = u16CmdParam;
                            }
                        }
                    }
                    s_s32CurrentNum2 = moveNum +2;
                }
                else if((moveNum +3)== i )
                {
                    s32ValX = LEN_1 - s32ValX;
                    skinId = background_skin_fast;                                        
                }       
                else if((moveNum +4)== i )
                {
                    s32ValX = 2*LEN_1 - s32ValX;
                    skinId = background_skin_fast;                                        
                }  
                else if((moveNum +5)== i )
                {
                    s32ValX = 3*LEN_1 - s32ValX;
                    skinId = background_skin_fast;                                        
                }                  
                
                if((View_2[s_s32Item_1])[i+1] > 0)
                {
                    s32ValX = abs(s32ValX);
                    //MLOGE("i=%d,s32ValX=%d\n",i,s32ValX);
                    //s32ValX += BASE;
                    s32Ret |=HI_GV_Widget_SetSkin((View_2[s_s32Item_1])[i+1], HIGV_SKIN_NORMAL,skinId);                                            
                    HI_GV_Widget_SetMargin((View_2[s_s32Item_1])[i+1], s32ValX, 0, 0, 0);                
                    
                }
                //HI_GV_Widget_Update((View_2[s_s32Item_1])[0], 0);
               // HI_GV_Widget_Refresh((View_2[s_s32Item_1])[0], 0);
                HI_GV_Widget_Paint((View_2[s_s32Item_1])[0], 0);

            }            
        }
end:
        HI_GV_Widget_Paint(FEIYU_FAST_SET, 0); 
        usleep(1000);
    }
}

static HI_BOOL isValidItem(HI_S32 i)
{
    HI_S32 temp = 0;    
    HI_BOOL s32Ret = HI_TRUE;
    //HI_S32 strID = sa_SubPageItemDes[s_current_subpage].strDesID[i];

    if(SUBPAGE_PHOTO_PARAM_EXPOSURE == s_current_subpage_1 ||
        SUBPAGE_PHOTO_PARAM_SHUTTER == s_current_subpage_1 ||
        SUBPAGE_PHOTO_PARAM_ISO == s_current_subpage_1 ||
        SUBPAGE_PHOTO_PARAM_WB == s_current_subpage_1)
    {
        FeiYuCmdExecute(CMD_GET_PHOTO_SCENE, 0, &temp);    
        if(temp == PHOTO_SCENE_HDR ||
            temp == PHOTO_SCENE_AUTO ||
            temp == PHOTO_SCENE_NORMAL)
        {
            if(i > 0)
            {
                //MLOGE("err i=%d\n",i);
                s32Ret = HI_FALSE;
            }
        }
    }
    
    if(SUBPAGE_PHOTODELAY_PARAM_SHUTTER == s_current_subpage_1)
    {
        FeiYuCmdExecute(CMD_GET_PHOTODELAY_DELAYTIME, 0, &temp);  
        MLOGE("temp=%d, MULITPHOTO_LAPSE_INTERVALTIME_0X5S=%d\n",temp,MULITPHOTO_LAPSE_INTERVALTIME_0X5S);
        if(MULITPHOTO_LAPSE_INTERVALTIME_0X5S == temp || 
           MULITPHOTO_LAPSE_INTERVALTIME_1S == temp || 
           MULITPHOTO_LAPSE_INTERVALTIME_2S == temp)
        {
            if(i > 0)
            {
                s32Ret = HI_FALSE;
            }
        }
        else if(MULITPHOTO_LAPSE_INTERVALTIME_3S == temp || 
            MULITPHOTO_LAPSE_INTERVALTIME_5S == temp)
        {
            if(i > 1)
            {
                s32Ret = HI_FALSE;
            }                
        }
        else if(MULITPHOTO_LAPSE_INTERVALTIME_10S == temp)
        {
            if(i > 2)
            {
                s32Ret = HI_FALSE;
            }  
        }
        else if(MULITPHOTO_LAPSE_INTERVALTIME_30S == temp)
        {
            if(i > 5)
            {
                s32Ret = HI_FALSE;
            }
        }
            
    }        

    return s32Ret;
}

HI_BOOL isValidDisLcd(HI_S32 currentPage)
{
    HI_S32 state = 0;
    if(SUBPAGE_VIDEO_DIS == currentPage ||
       SUBPAGE_VIDEOLAPSE_DIS ==  currentPage ||
       SUBPAGE_VIDEOSLOW_DIS == currentPage || 
       SUBPAGE_VIDEOLOOP_DIS == currentPage ||
       SUBPAGE_VIDEOSNAP_DIS == currentPage)
    {    
        FeiYuCmdExecute(CMD_GET_VIDEO_LDC, 0, &state);   
        if(VIDEO_LDC_ON == state)
        {
            return HI_FALSE;
        }
    }
    else if(SUBPAGE_VIDEO_LDC == currentPage ||
            SUBPAGE_VIDEOLAPSE_LDC ==  currentPage ||
            SUBPAGE_VIDEOSLOW_LDC == currentPage || 
            SUBPAGE_VIDEOLOOP_LDC == currentPage ||
            SUBPAGE_VIDEOSNAP_LDC == currentPage)

    {
        FeiYuCmdExecute(CMD_GET_VIDEO_DIS, 0, &state);    
        if(VIDEO_DIS_ON == state)
        {
            return HI_FALSE;
        }        
    }

    return HI_TRUE;
}

static HI_S32 updateUI(HI_S32 flag )
{
    //MLOGE("####\n");
    HI_S32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32ItemNum = 0;
    HI_S32 * p = NULL;
    HIGV_CORD tempx = 0;
    HIGV_CORD tempy = 0;

    hideAll();
    
    //if(0 == flag)
    {
        if(s_current_subpage >= 0)
        {
            for(i = 0; i < MAX_ITEM_NUM; i++)
            {
                if(sa_SubPageItemDes[s_current_subpage].strDesID[i] == 0)
                {
                    break;
                }
                if(ID_STR_GLOBALSET_PHOTO_IMAGEMODE == sa_SubPageItemDes[s_current_subpage].strDesID[i] ||
                   ID_STR_GLOBALSET_PHOTOTIMER_DELAY_TIME == sa_SubPageItemDes[s_current_subpage].strDesID[i]||
                   ID_STR_GLOBALSET_PHOTODELAY_DELAY_TIME == sa_SubPageItemDes[s_current_subpage].strDesID[i]||
                   ID_STR_GLOBALSET_PHOTODELAY_IMAGE_MODE == sa_SubPageItemDes[s_current_subpage].strDesID[i] ||
                   ID_STR_GLOBALSET_VIDEOSNAP_LAPSEINTERVAL == sa_SubPageItemDes[s_current_subpage].strDesID[i] ||
                   ID_STR_GLOBALSET_VIDEOSNAP_MODE == sa_SubPageItemDes[s_current_subpage].strDesID[i] ||
                   ID_STR_GLOBALSET_VIDEO_LOOPTYPE == sa_SubPageItemDes[s_current_subpage].strDesID[i] ||
                   ID_STR_GLOBALSET_VIDEOLOOP_MODE == sa_SubPageItemDes[s_current_subpage].strDesID[i] ||
                   ID_STR_GLOBALSET_VIDEOSLOW_MODE== sa_SubPageItemDes[s_current_subpage].strDesID[i] ||
                   ID_STR_GLOBALSET_VIDEOLAPSE_LAPSEINTERVAL ==sa_SubPageItemDes[s_current_subpage].strDesID[i] ||
                   ID_STR_GLOBALSET_VIDEOLAPSE_MODE == sa_SubPageItemDes[s_current_subpage].strDesID[i] ||
                   ID_STR_GLOBALSET_VIDEO_MODE == sa_SubPageItemDes[s_current_subpage].strDesID[i] ||
                   ID_STR_GLOBALSET_MULTIPHOTO_BURST_BURSTTYPE == sa_SubPageItemDes[s_current_subpage].strDesID[i] || 
                   ID_STR_GLOBALSET_MULTIPHOTO_BURST_IMAGEMODE ==sa_SubPageItemDes[s_current_subpage].strDesID[i] 
                   )
                {
                    continue;
                }            
                s32ItemNum++;        
            }
            s32ItemNum += 4;           
            MLOGE("==================>>>s32ItemNum=%d\n",s32ItemNum);  
            s_s32Item_0 = s32ItemNum -1;
            p = View_1[s32ItemNum -1]; 
            HI_S32 j = 0;
            if( p != NULL)
            {
                for(i = 0; i < s32ItemNum; i++)
                {
                    if((0 == i) || (1 == i))
                        s32Ret |=HI_GV_Widget_SetText(p[i+1], " ");
                    HI_S32 s32StrID = sa_SubPageItemDes[s_current_subpage].strDesID[i];
                    if(ID_STR_GLOBALSET_PHOTO_IMAGEMODE == s32StrID||
                       ID_STR_GLOBALSET_PHOTOTIMER_DELAY_TIME == s32StrID ||
                       ID_STR_GLOBALSET_PHOTODELAY_DELAY_TIME == s32StrID ||
                       ID_STR_GLOBALSET_PHOTODELAY_IMAGE_MODE == s32StrID ||
                       ID_STR_GLOBALSET_VIDEOSNAP_LAPSEINTERVAL == s32StrID ||
                       ID_STR_GLOBALSET_VIDEOSNAP_MODE ==  s32StrID ||
                       ID_STR_GLOBALSET_VIDEO_LOOPTYPE == s32StrID ||
                       ID_STR_GLOBALSET_VIDEOLOOP_MODE == s32StrID ||
                       ID_STR_GLOBALSET_VIDEOSLOW_MODE == s32StrID ||
                       ID_STR_GLOBALSET_VIDEOLAPSE_LAPSEINTERVAL == s32StrID ||
                       ID_STR_GLOBALSET_VIDEOLAPSE_MODE == s32StrID ||
                       ID_STR_GLOBALSET_VIDEO_MODE == s32StrID ||
                       ID_STR_GLOBALSET_MULTIPHOTO_BURST_BURSTTYPE == s32StrID ||
                       ID_STR_GLOBALSET_MULTIPHOTO_BURST_IMAGEMODE == s32StrID
                       
                       )
                    {
                        continue;
                    }
                    if(s32StrID > 0)
                    {
                        s32Ret |=HI_GV_Widget_SetTextByID(p[j+3], s32StrID);
                    }
                    j++;
                }         
                s32Ret |= HI_GV_Widget_Show(p[0]);
                s32Ret |= HI_GV_Widget_MoveToTop(p[0]);

                HI_GV_ScrollView_GetViewCoordinate(p[0], &tempx, &tempy);
                i = 0;
                if(0 == flag)
                {
                    do
                    {
                        HI_S32 j = 0;
                        for(j =0; j < 9; j++)
                        {
                        HI_GV_ScrollView_SetViewCoordinate(p[0], tempx, 64*2); 
                        HI_GV_ScrollView_GetViewCoordinate(p[0], &tempx, &tempy);
                        HI_GV_Widget_Refresh(p[0], 0);
                        HI_GV_Widget_Update(p[0], 0);
                        HI_GV_Widget_Paint(p[0], 0);   
                        HI_GV_Widget_Refresh(FEIYU_FAST_SET, 0); 
                        HI_GV_Widget_Update(FEIYU_FAST_SET, 0); 
                        HI_GV_Widget_Paint(FEIYU_FAST_SET, 0); 
                        MLOGE("111111111111111111111111111--------->>>tempy=%d\n",tempy);
                        }
                    }while((i++ < 200) && (tempy != (64*2)));
                }
              
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
        MLOGE("~s_current_subpage < 0\n");
    }
    }    
    s32ItemNum = 0;


    if(s_current_subpage_1 > 0)
    {
        for(i = 0; i < MAX_ITEM_NUM; i++)
        {
            if(sa_SubPageItemDes[s_current_subpage_1].strDesID[i] == 0)
            {
                break;
            }
            if( HI_FALSE == isValidItem(i))
            {
                continue;
            }
            s32ItemNum++;        
        }
        if(HI_FALSE == isValidDisLcd(s_current_subpage_1))
        {
            s32ItemNum = 1;
        }
        s32ItemNum += 4;   
        s_s32Item_1 = s32ItemNum -1;
        MLOGE("----s32ItemNum=%d\n",s32ItemNum);
        p = View_2[s32ItemNum -1]; 
        if( p != NULL)
        {
            HI_S32 retVal = 0;
            FeiYuCmdExecute(CMD_GET_PHOTO_SCENE, 0, &retVal);        
            if(HI_FALSE == isValidDisLcd(s_current_subpage_1))
            {
                s32Ret |=HI_GV_Widget_SetText(p[0+1], " ");
                s32Ret |=HI_GV_Widget_SetText(p[1+1], " ");
                s32Ret |=HI_GV_Widget_SetTextByID(p[2+1], ID_STR_GLOBALSET_SWITCH_OFF);                
                s32Ret |=HI_GV_Widget_SetText(p[3+1], " ");
                s32Ret |=HI_GV_Widget_SetText(p[4+1], " ");
                s32Ret |=HI_GV_Widget_SetSkin(p[0+1], HIGV_SKIN_NORMAL,background_skin_fast);                 
                s32Ret |=HI_GV_Widget_SetSkin(p[1+1], HIGV_SKIN_NORMAL,background_skin_fast);
                s32Ret |=HI_GV_Widget_SetSkin(p[2+1], HIGV_SKIN_NORMAL,background_skin);
                s32Ret |=HI_GV_Widget_SetSkin(p[3+1], HIGV_SKIN_NORMAL,background_skin_fast);
                s32Ret |=HI_GV_Widget_SetSkin(p[4+1], HIGV_SKIN_NORMAL,background_skin_fast);       
                s32Ret |= HI_GV_Widget_Paint(p[0], 0);
                s32Ret |= HI_GV_Widget_Paint(FEIYU_FAST_SET, 0);                
            }
            else if((SUBPAGE_PHOTO_PARAM_EXPOSURE == s_current_subpage_1 )&& 
            (retVal == PHOTO_SCENE_HDR || retVal == PHOTO_SCENE_AUTO ||retVal == PHOTO_SCENE_NORMAL))
            {
                MLOGE("#####-----SUBPAGE_PHOTO_PARAM_SHUTTER\n");
                s32Ret |=HI_GV_Widget_SetText(p[0+1], " ");
                s32Ret |=HI_GV_Widget_SetText(p[1+1], " ");
                s32Ret |=HI_GV_Widget_SetTextByID(p[2+1], ID_STR_GLOBALSET_PHOTO_EXPOSURE_0);                
                s32Ret |=HI_GV_Widget_SetText(p[3+1], " ");
                s32Ret |=HI_GV_Widget_SetText(p[4+1], " ");
                s32Ret |=HI_GV_Widget_SetSkin(p[0+1], HIGV_SKIN_NORMAL,background_skin_fast);                 
                s32Ret |=HI_GV_Widget_SetSkin(p[1+1], HIGV_SKIN_NORMAL,background_skin_fast);
                s32Ret |=HI_GV_Widget_SetSkin(p[2+1], HIGV_SKIN_NORMAL,background_skin);
                s32Ret |=HI_GV_Widget_SetSkin(p[3+1], HIGV_SKIN_NORMAL,background_skin_fast);
                s32Ret |=HI_GV_Widget_SetSkin(p[4+1], HIGV_SKIN_NORMAL,background_skin_fast);       
                s32Ret |= HI_GV_Widget_Paint(p[0], 0);
                s32Ret |= HI_GV_Widget_Paint(FEIYU_FAST_SET, 0);

            }
            else
            {
                s32Ret |=HI_GV_Widget_SetText(p[0+1], " ");
                s32Ret |=HI_GV_Widget_SetText(p[1+1], " ");            
                s32Ret |=HI_GV_Widget_SetText(p[s32ItemNum -1], " ");                        
                s32Ret |=HI_GV_Widget_SetText(p[s32ItemNum -2], " ");                                    
                for(i = 0; i < s32ItemNum; i++)
                {
                    if(HI_FALSE == isValidItem(i))
                        continue;
                    HI_S32 s32StrID = sa_SubPageItemDes[s_current_subpage_1].strDesID[i];
                    if(s32StrID > 0)
                    {
                        s32Ret |=HI_GV_Widget_SetTextByID(p[i+3], s32StrID);
                    }

                    s32Ret |=HI_GV_Widget_SetSkin(p[i+3], HIGV_SKIN_NORMAL,background_skin_fast);                 
                }   
                if(5 == s32ItemNum)
                {
                    s32Ret |=HI_GV_Widget_SetSkin(p[2+1], HIGV_SKIN_NORMAL,background_skin);
                }
            }
            
            s32Ret |= HI_GV_Widget_Show(p[0]);
            s32Ret |= HI_GV_Widget_MoveToTop(p[0]);

            if(HI_FALSE == isValidDisLcd(s_current_subpage_1))
            {
                
            }
            else if((SUBPAGE_PHOTO_PARAM_SHUTTER == s_current_subpage_1 || SUBPAGE_PHOTO_PARAM_ISO == s_current_subpage_1 ||
            SUBPAGE_PHOTO_PARAM_WB == s_current_subpage_1 || SUBPAGE_PHOTO_PARAM_EXPOSURE == s_current_subpage_1)&& 
            (retVal == PHOTO_SCENE_HDR || retVal == PHOTO_SCENE_AUTO ||retVal == PHOTO_SCENE_NORMAL))
            {
            }
            else
            {
                HI_S32 s32GetCmd = sa_SubPageItemDes[s_current_subpage_1].getCmdid;
                HI_S32 s32State = 0;
                FeiYuCmdExecute(s32GetCmd, 0, &s32State);
                i = 0;
            
                do{
                    if( sa_SubPageItemDes[s_current_subpage_1].FeiyuCmdParam[i] == s32State)
                        break;
                    i++;
                }while(sa_SubPageItemDes[s_current_subpage_1].FeiyuCmdParam[i] > 0);
                
                if((View_2[s_s32Item_1])[i+1+2] > 0)
                {
                    //MLOGE("~~~~~~i =%d, s_s32Item_1=%d\n",i,s_s32Item_1);
                    s32Ret |=HI_GV_Widget_SetSkin((View_2[s_s32Item_1])[i+1+2], HIGV_SKIN_NORMAL,background_skin); 
                }
                HI_S32 setTempy = 0;
                if(i >= 1)
                    setTempy = 64*(i);
                else
                    setTempy = 0;
                //HI_GV_ScrollView_SetViewCoordinate((View_2[s32ItemNum -1])[0], 0, (i+2)*64);           
                
                do
                {
                    HI_S32 j = 0;
                    for(j =0; j < 9; j++)
                    {
                    HI_GV_ScrollView_SetViewCoordinate(p[0], tempx, setTempy);  
                    HI_GV_ScrollView_GetViewCoordinate(p[0], &tempx, &tempy);
                    HI_GV_Widget_Refresh(p[0], 0);
                    HI_GV_Widget_Update(p[0], 0);
                    s32Ret |= HI_GV_Widget_Paint(p[0], 0);                
                    //MLOGE("--------->>>tempy=%d\n",tempy);
                    }
                }while((i++ < 200) && (tempy != setTempy));     

                //MLOGE("#############--------->>>tempy=%d,setTempy=%d\n",tempy,setTempy);            
                s32Ret |= HI_GV_Widget_Paint(p[0], 0);
            }
        }  
        else
        {
            MLOGE("null \n");
            s_s32Item_1 = -1;
        }
    }
    else
    {

    }

 
    if(0 != s32Ret)
    {
        MLOGE("ERR, s32Ret=%d\n");
    }

    HI_GV_Widget_Paint(FEIYU_FAST_SET, 0);
    s_isRefresh_2 = HI_FALSE;

    if(HI_FALSE == bIsRunning)
    {
        pthread_t threadid = 0;
        bIsRunning = HI_TRUE;        
        s32Ret = pthread_create(&threadid, NULL, (void *)doMove, NULL);
        if(0 != s32Ret)
        {
            MLOGE("pthread_create, s32Ret=%#x\n", s32Ret);
        }   

    }

    return ;
}
static HI_S32 InitViewParam()
{
    HI_S32 i = 0;
    HI_S32 s32Ret = 0;
    #define STEP    64
    #define INTERVAL 1000
    #define FLINGPARAM 0.2
    #define SCROLLPARAM 0.7
    
    for(i = 0; i < MAX_ITEM; i++)
    {
        if( NULL != View_1[i])
        {

            s32Ret |= HI_GV_ScrollView_SetStep((View_1[i])[0], STEP);
            s32Ret |= HI_GV_ScrollView_SetInterval((View_1[i])[0], INTERVAL);
            s32Ret |= HI_GV_ScrollView_SetFlingParam((View_1[i])[0], FLINGPARAM);
            s32Ret |= HI_GV_ScrollView_SetScrollParam((View_1[i])[0], SCROLLPARAM);
        }
    }
    for(i = 0; i < MAX_ITEM; i++)
    {
        if( NULL != View_2[i])
        {
            s32Ret |= HI_GV_ScrollView_SetStep((View_2[i])[0], STEP);
            s32Ret |= HI_GV_ScrollView_SetInterval((View_2[i])[0], INTERVAL);
            s32Ret |= HI_GV_ScrollView_SetFlingParam((View_2[i])[0], FLINGPARAM);
            s32Ret |= HI_GV_ScrollView_SetScrollParam((View_2[i])[0], SCROLLPARAM);
        }
    }     
    if(0 != s32Ret)
    {
        MLOGE("ERR , s32Ret=%d\n",s32Ret);
    }

}
HI_S32 FEIYU_FAST_SET_OnShow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32CurrentWorkMode = 0;  
    HIGV_CORD tempx = 0;
    HIGV_CORD tempy = 0;
    HI_S32 s32Ret = 0;
    s_s32OnShowWaitTime = 0;
    HI_GV_Widget_Hide(FAST_SET_LABEL);
    HI_GV_Widget_Hide(FAST_SET_LABEL_1);
    s_firstEntry = HI_TRUE;
    s_firstEntry_1 = HI_TRUE;    
    s_userinput_flag2 = HI_FALSE;


    InitViewParam();

    FeiYuCmdExecute(CMD_GET_CURRENT_WORK_MODE, 0, &s32CurrentWorkMode);
    switch(s32CurrentWorkMode)
    {
        case PHOTO:
            s_current_subpage = SUBPAGE_PHOTO_MODE_PARAM;
            s_current_subpage_1 = SUBPAGE_PHOTO_PARAM_EXPOSURE;
            tempx = 0;
            break;
        case PHOTO_TIMER:
            s_current_subpage = SUBPAGE_PHOTOTIMER_MODE_PARAM;
            s_current_subpage_1 = SUBPAGE_PHOTOTIMER_PARAM_SHUTTER;  
            tempx = 142;
            break;
        case PHOTO_DELAY:
            s_current_subpage = SUBPAGE_PHOTODELAY_MODE_PARAM;
            s_current_subpage_1 = SUBPAGE_PHOTODELAY_PARAM_SHUTTER;  
            tempx = 142*2;
            break;
        case MULTI_PHOTO:
            s_current_subpage = SUBPAGE_MULTIPHOTO_MODE_PARAM;
            s_current_subpage_1 = SUBPAGE_MULTIPHOTO_BURST_ISO;  
            tempx = 142*3;
            break;
        case VIDEO_SNAP:
            s_current_subpage = SUBPAGE_VIDEOSNAP_MODE_PARAM;
            s_current_subpage_1 = SUBPAGE_VIDEOSNAP_DIS;
            tempx = 142*8;
            break;
        case VIDEO:
            s_current_subpage = SUBPAGE_VIDEO_MODE_PARAM;
            s_current_subpage_1 = SUBPAGE_VIDEO_DIS;   
            tempx = 142*4;
            break;
        case VIDEO_LAPSE:
            s_current_subpage = SUBPAGE_VIDEOLAPSE_MODE_PARAM;
            s_current_subpage_1 = SUBPAGE_VIDEOLAPSE_DIS;  
            tempx = 142*5;
            break;
        case VIDEO_SLOW:
            s_current_subpage = SUBPAGE_VIDEOSLOW_MODE_PARAM;
            s_current_subpage_1 = SUBPAGE_VIDEOSLOW_DIS;      
            tempx = 142*6;
            break;
        case VIDEO_LOOP:
            s_current_subpage = SUBPAGE_VIDEOLOOP_MODE_PARAM;
            s_current_subpage_1 = SUBPAGE_VIDEOLOOP_DIS;     
            tempx = 142*7;
            break;
        default:
            s_current_subpage = SUBPAGE_PHOTO_MODE_PARAM;
            s_current_subpage_1 = SUBPAGE_PHOTO_PARAM_EXPOSURE;            
            break;
    }     

    updateUI(0);

    HI_GV_Widget_Show(FAST_SET_LABEL);
    HI_GV_Widget_Show(FAST_SET_LABEL_1);
    HI_GV_Widget_MoveToTop(FAST_SET_LABEL);
    HI_GV_Widget_MoveToTop(FAST_SET_LABEL_1);    
    HI_GV_Timer_Create(FEIYU_FAST_SET, FASTSET_PAGE_TIMER, FASTSET_PAGE_TIMER_INTERVAL);
    HI_GV_Timer_Start(FEIYU_FAST_SET, FASTSET_PAGE_TIMER);    
    bIsRunning = HI_TRUE;

    pthread_t threadid = 0;

    s32Ret = pthread_create(&threadid, NULL, (void *)doMove, NULL);
    if(0 != s32Ret)
    {
        MLOGE("pthread_create, s32Ret=%#x\n", s32Ret);
    }     
    return HIGV_PROC_GOON;
}

HI_S32 OnConfigFocusSelectListener(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 OnConfigListener(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 OnItemFocusSelectListener(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 OnItemListener(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_FAST_SET_onevent(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    if(UPDATE_FAST_SET == wParam)
    {
        updateUI(1);
        s_UpdateView2Flag--;
    }
}
HI_S32 FEIYU_FAST_SET_ongesturescroll(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //return HIGV_PROC_GOON;

    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;

    MLOGD("\n");

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.scroll.start.x;
    startY = gestureEvent.gesture.scroll.start.y;
    endX = gestureEvent.gesture.scroll.end.x;
    endY = gestureEvent.gesture.scroll.end.y;
    //MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);

    if (abs(startX - endX) > abs(startY - endY))// left or right
    {
        if (startX > endX)// to left
        {
              
        }
        else // to right
        {
        	if(g_pageChangeEnd == HI_TRUE)
        	{
				g_pageChangeEnd = HI_FALSE;
	            bIsRunning = HI_FALSE;
				UI_ShowHomePage();
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

        }
    }
    
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_FAST_SET_ongesturefling(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;

    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;

    MLOGD("\n");

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
    //MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);

    if (abs(startX - endX) > abs(startY - endY))// left or right
    {
        if (startX > endX)// to left
        {
              
        }
        else // to right
        {
        	if(g_pageChangeEnd == HI_TRUE)
        	{
				g_pageChangeEnd = HI_FALSE;
	            bIsRunning = HI_FALSE;
				UI_ShowHomePage();
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

        }
    }
    
    return HIGV_PROC_GOON;
}

static HI_HANDLE s_s32Fasthandle = 0;
static HI_S32 s_s32WhichView = 0;
static void fast_set_an_handle(HIGV_LISTENER_S* listener, void* data)
{
//    MLOGE("###\n");
    HI_S32 tempx = 0, tempy = 0;
    HI_S32 viewID = 0;
    if(s_s32WhichView == 0)
    {
        viewID = (View_1[s_s32Item_0])[0];
    }
    else if(s_s32WhichView == 1)
    {
        viewID = (View_2[s_s32Item_1])[0];
    }
    
    HI_GV_ScrollView_GetViewCoordinate(viewID, tempx, tempy);

    HI_GV_ScrollView_SetViewCoordinate(viewID, tempx, HI_GV_TweenAnimGetTweenValue(s_s32Fasthandle, 0));
    
    HI_GV_Widget_Update(viewID, 0);    
}

static void fast_set_an_handle_end(HIGV_LISTENER_S* listener, void* data)
{
    s_bIsRunning_1 = HI_FALSE;
    s_bIsRunning_2 = HI_FALSE;    
}

static HI_S32 moveFrame(float startVal, float endVal)
{
    MLOGE("startVal=%d, endVal=%d\n",startVal,endVal);
    
    s_s32Fasthandle = HI_GV_TweenAnimCreate();
    HI_GV_TweenAnimSetDuration(s_s32Fasthandle,300);

    HI_GV_TweenAnimAddTween(s_s32Fasthandle,HIGV_TWEEN_TRANSITION_LINEAR,HIGV_TWEEN_EASE_OUT,startVal,endVal);

    HI_GV_TweenAnimSetExecListener(s_s32Fasthandle, fast_set_an_handle);
       
    HI_GV_TweenAnimSetStartedListener(s_s32Fasthandle, NULL);

    HI_GV_TweenAnimSetFinishedListener(s_s32Fasthandle, fast_set_an_handle_end);


    HI_GV_TweenAnimStart(s_s32Fasthandle);    
    
    return 0;
}

HI_S32 FEIYU_FAST_SET_OnHide_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	if (HIGV_KEY_F2 == wParam) //mode key
    {
        UI_ShowHomePage();
    }
    else if (HIGV_KEY_F1 == wParam) //photo key
    {
		HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
        UI_ShowHomePage();
    }
    else
    {
        MLOGE("KEY CODE %#x not processed\n", wParam);
    }

    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_FAST_SET_ontouchaction_1(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    if(s_s32OnShowWaitTime < 5)
        return HIGV_PROC_GOON; 
    HI_S32 point_x, point_y;
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;
    HI_S32 tempy =0, tempx =0, afterTempy = 0;
    HI_BOOL config_flag = HI_FALSE;
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
            HI_GV_ScrollView_GetViewCoordinate((View_1[s_s32Item_0])[0], &tempx, &s_tempy);   
            config_flag = HI_FALSE;
            HI_GV_Timer_Stop(FEIYU_FAST_SET, FASTSET_PAGE_TIMER);

            break;
        case HIGV_TOUCH_END:
            HI_GV_Timer_Start(FEIYU_FAST_SET, FASTSET_PAGE_TIMER);            
        case HIGV_TOUCH_MOVE:
            afterTempy = s_tempy - (point_y - baseY);
            config_flag = HI_TRUE;            
            break;
        default:
            break;
    }
    //MLOGE("type=%d,point_y=%d,s_tempy=%d,afterTempy=%d\n",type,point_y,s_tempy,afterTempy);
    if(config_flag)
        HI_GV_ScrollView_SetViewCoordinate((View_1[s_s32Item_0])[0], tempx, afterTempy);

    return HIGV_PROC_GOON; 

}



HI_S32 FEIYU_FAST_SET_ontouchaction_2(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    if(s_s32OnShowWaitTime < 5)
        return HIGV_PROC_GOON; 

    HI_S32 point_x, point_y;
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;
    HI_S32 tempy =0, tempx =0, afterTempy = 0;
    HI_BOOL config_flag = HI_FALSE;
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
            HI_GV_ScrollView_GetViewCoordinate((View_2[s_s32Item_1])[0], &tempx, &s_tempy);   
            config_flag = HI_FALSE;
            HI_GV_Timer_Stop(FEIYU_FAST_SET, FASTSET_PAGE_TIMER);
            s_userinput_flag2 = HI_TRUE;

            break;
        case HIGV_TOUCH_END:
            HI_GV_Timer_Start(FEIYU_FAST_SET, FASTSET_PAGE_TIMER);   
            //s_userinput_flag2 = HI_FALSE;
        case HIGV_TOUCH_MOVE:
            afterTempy = s_tempy - (point_y - baseY);
            config_flag = HI_TRUE;  
            s_userinput_flag2 = HI_TRUE;
            break;
        default:
            break;
    }
    //MLOGE("type=%d,point_y=%d,s_tempy=%d,afterTempy=%d\n",type,point_y,s_tempy,afterTempy);
    if(config_flag)
        HI_GV_ScrollView_SetViewCoordinate((View_2[s_s32Item_1])[0], tempx, afterTempy);

    return HIGV_PROC_GOON; 

}    
HI_S32 FEIYU_FAST_SET_ongesturescroll_1(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON; 
    if(s_s32OnShowWaitTime < WAIT_TIME_COUNT)
        return HIGV_PROC_GOON;
    HI_GV_Timer_Reset(FEIYU_FAST_SET, FASTSET_PAGE_TIMER);

    
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
    HI_S32 static s_tempy = 0;
    if(0 == s_tempy)
        s_tempy = tempy;
    if(s_bIsRunning_1)
        return HIGV_PROC_GOON;  

    s_tempy = s_tempy + ((startY - endY)*7/8);
    if(s_tempy < 0)
        s_tempy = 0;
    
    HI_GV_ScrollView_SetViewCoordinate((View_1[s_s32Item_0])[0], tempx, s_tempy);
    //MLOGE("-------------tempy=%d,s_tempy=%d,dy=%d\n",tempy,s_tempy, ((startY - endY)*7/8));
    
    return HIGV_PROC_GOON;    
}

HI_S32 FEIYU_FAST_SET_ongesturescroll_2(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON; 
    if(s_s32OnShowWaitTime < WAIT_TIME_COUNT)
        return HIGV_PROC_GOON;

    HI_GV_Timer_Reset(FEIYU_FAST_SET, FASTSET_PAGE_TIMER);

    

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
    HI_S32 static s_tempy = 0;
    if(0 == s_tempy)
        s_tempy = tempy;
    if(s_bIsRunning_1)
        return HIGV_PROC_GOON;  

    s_tempy = s_tempy + ((startY - endY)*7/8);
    if(s_tempy < 0)
        s_tempy = 0;
    
    HI_GV_ScrollView_SetViewCoordinate((View_2[s_s32Item_1])[0], tempx, s_tempy);

    
    return HIGV_PROC_GOON;    
}


HI_S32 FEIYU_FAST_SET_OnGestureFling_1(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //return HIGV_PROC_GOON;

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
    if(s_bIsRunning_1)
        return HIGV_PROC_GOON;    
    if (abs(startX - endX) > abs(startY - endY))// left or right
    {
        if (startX > endX)// to left
        {

        }
        else // to right
        {
            if(tempx >= 142)
            {
            
            }            
        }
    }
    else //up or down
    {
        s_s32WhichView = 0;
        if (startY > endY)//to up
        {
            s_bIsRunning_1 = HI_TRUE;
            moveFrame(tempy, tempy + 64);
            //new_ItemNum = (tempx + 142)/142;

        }
        else  //down
        {
            s_bIsRunning_1 = HI_TRUE;        
            moveFrame(tempy, tempy - 64);                
            //new_ItemNum = (tempx - 142)/142;    

        }

    }
    
    return HIGV_PROC_GOON;    
}

HI_S32 FEIYU_FAST_SET_OnGestureFling_2(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
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
    if(s_bIsRunning_2)
        return HIGV_PROC_GOON; 

    if (abs(startX - endX) > abs(startY - endY))// left or right
    {
        if (startX > endX)// to left
        {

        }
        else // to right
        {
            if(tempx >= 142)
            {
            
            }            
        }
    }
    else //up or down
    {
        s_s32WhichView = 1;
        if (startY > endY)//to up
        {
            s_bIsRunning_2 = HI_TRUE;        
            moveFrame(tempy, tempy + 64);
            //new_ItemNum = (tempx + 142)/142;

        }
        else  //down
        {
            s_bIsRunning_2 = HI_TRUE;        
            moveFrame(tempy, tempy - 64);                
            //new_ItemNum = (tempx - 142)/142;    

        }

    }

    return HIGV_PROC_GOON;    
}

static HI_HANDLE s_ItemHandle = 0;

static void Item_Update_handle(HIGV_LISTENER_S* listener, void* data)
{
    HI_RECT rect = {0};
    HI_GV_Widget_GetRect(FEIYU_FAST_SET,&rect);
    HI_GV_Widget_Move(FEIYU_FAST_SET,HI_GV_TweenAnimGetTweenValue(s_ItemHandle, 0),rect.y);
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
    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("err, s32Ret=%d\n",s32Ret);
    }

}

void FastSetAnimmoveXItem(HIGV_CORD fromValue,HIGV_CORD toValue)
{
    s_ItemHandle = HI_GV_TweenAnimCreate();
    HI_GV_TweenAnimSetDuration(s_ItemHandle,300);

    HI_GV_TweenAnimAddTween(s_ItemHandle,HIGV_TWEEN_TRANSITION_LINEAR,HIGV_TWEEN_EASE_IN,fromValue,toValue);

    HI_GV_TweenAnimSetExecListener(s_ItemHandle, Item_Update_handle);
        
    HI_GV_TweenAnimSetStartedListener(s_ItemHandle, Item_Start_handle);


    HI_GV_TweenAnimSetFinishedListener(s_ItemHandle, Item_End_handle);


    HI_GV_TweenAnimStart(s_ItemHandle);    

}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

