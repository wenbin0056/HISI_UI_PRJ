#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
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
#include "hi_product_param_ext.h"
#include "hi_flash.h"
#include "hi_product_comm.h"
#include "hi_cycle.h"
#include "hi_math.h"
#include "hi_mapi_comm_define.h"
#include "hi_product_workmode_ext.h"
#include "hi_sys_mng.h"
#include "hi_wifi_mng.h"
#include "hi_product_media_ext.h"
#include "hi_gv_scrollview.h"
#include "hi_gv_parser.h"
#include "higv_language.h"
#include "hi_wifi_mng.h"
#include "hi_product_param_ext.h"
#include "hi_gv_gesture.h"
#include "hi_gv_input.h"
#include "hi_product_rawdump.h"
#include "hi_type.h"
#include "hi_mapi_sys.h"

#include "feiyu_UIAnimationTool.h"
#include "feiyu_dv_cmd_proc.h"
#include "feiyu_UICommon_FunTool.h"
#include "hi_type.h"

#include "hi_go.h"
#include "hi_gv.h"
#include "higv_cextfile.h"
#include "higv_language.h"
#include "hi_gv_parser.h"
#include "higv_cextfile.h"
#include "hi_gv_wheelview.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*  __cplusplus  */
#define Month_1 ID_STR_1
#define Month_2 ID_STR_2
#define Month_3 ID_STR_3
#define Month_4 ID_STR_4
#define Month_5 ID_STR_5
#define Month_6 ID_STR_6
#define Month_7 ID_STR_7
#define Month_8 ID_STR_8
#define Month_9 ID_STR_9
#define Month_10 ID_STR_10
#define Month_11 ID_STR_11
#define Month_12 ID_STR_12
#define Day_1 ID_STR_1
#define Day_2 ID_STR_2
#define Day_3 ID_STR_3
#define Day_4 ID_STR_4
#define Day_5 ID_STR_5
#define Day_6 ID_STR_6
#define Day_7 ID_STR_7
#define Day_8 ID_STR_8
#define Day_9 ID_STR_9
#define Day_10 ID_STR_10
#define Day_11 ID_STR_11
#define Day_12 ID_STR_12
#define Day_13 ID_STR_13
#define Day_14 ID_STR_14
#define Day_15 ID_STR_15
#define Day_16 ID_STR_16
#define Day_17 ID_STR_17
#define Day_18 ID_STR_18
#define Day_19 ID_STR_19
#define Day_20 ID_STR_20
#define Day_21 ID_STR_21
#define Day_22 ID_STR_22
#define Day_23 ID_STR_23
#define Day_24 ID_STR_24
#define Day_25 ID_STR_25
#define Day_26 ID_STR_26
#define Day_27 ID_STR_27
#define Day_28 ID_STR_28
#define Day_29 ID_STR_29
#define Day_30 ID_STR_30
#define Day_31 ID_STR_31
#define Year_1 ID_STR_2017
#define Year_2 ID_STR_2018
#define Year_3 ID_STR_2019
#define Year_4 ID_STR_2020
#define Year_5 ID_STR_2021
#define Year_6 ID_STR_2022
#define Year_7 ID_STR_2023
#define Year_8 ID_STR_2024
#define Year_9 ID_STR_2025
#define Year_10 ID_STR_2026
#define Year_11 ID_STR_2027
#define Year_12 ID_STR_2028
#define Hour_0 ID_STR_0
#define Hour_1 ID_STR_1
#define Hour_2 ID_STR_2
#define Hour_3 ID_STR_3
#define Hour_4 ID_STR_4
#define Hour_5 ID_STR_5
#define Hour_6 ID_STR_6
#define Hour_7 ID_STR_7
#define Hour_8 ID_STR_8
#define Hour_9 ID_STR_9
#define Hour_10 ID_STR_10
#define Hour_11 ID_STR_11
#define Hour_12 ID_STR_12
#define Hour_13 ID_STR_13
#define Hour_14 ID_STR_14
#define Hour_15 ID_STR_15
#define Hour_16 ID_STR_16
#define Hour_17 ID_STR_17
#define Hour_18 ID_STR_18
#define Hour_19 ID_STR_19
#define Hour_20 ID_STR_20
#define Hour_21 ID_STR_21
#define Hour_22 ID_STR_22
#define Hour_23 ID_STR_23
#define MINUTE_0 ID_STR_0
#define MINUTE_1 ID_STR_1
#define MINUTE_2 ID_STR_2
#define MINUTE_3 ID_STR_3
#define MINUTE_4 ID_STR_4
#define MINUTE_5 ID_STR_5
#define MINUTE_6 ID_STR_6
#define MINUTE_7 ID_STR_7
#define MINUTE_8 ID_STR_8
#define MINUTE_9 ID_STR_9
#define MINUTE_10 ID_STR_10
#define MINUTE_11 ID_STR_11
#define MINUTE_12 ID_STR_12
#define MINUTE_13 ID_STR_13
#define MINUTE_14 ID_STR_14
#define MINUTE_15 ID_STR_15
#define MINUTE_16 ID_STR_16
#define MINUTE_17 ID_STR_17
#define MINUTE_18 ID_STR_18
#define MINUTE_19 ID_STR_19
#define MINUTE_20 ID_STR_20
#define MINUTE_21 ID_STR_21
#define MINUTE_22 ID_STR_22
#define MINUTE_23 ID_STR_23
#define MINUTE_24 ID_STR_24
#define MINUTE_25 ID_STR_25
#define MINUTE_26 ID_STR_26
#define MINUTE_27 ID_STR_27
#define MINUTE_28 ID_STR_28
#define MINUTE_29 ID_STR_29
#define MINUTE_30 ID_STR_30
#define MINUTE_31 ID_STR_31
#define MINUTE_32 ID_STR_32
#define MINUTE_33 ID_STR_33
#define MINUTE_34 ID_STR_34
#define MINUTE_35 ID_STR_35
#define MINUTE_36 ID_STR_36
#define MINUTE_37 ID_STR_37
#define MINUTE_38 ID_STR_38
#define MINUTE_39 ID_STR_39
#define MINUTE_40 ID_STR_40
#define MINUTE_41 ID_STR_41
#define MINUTE_42 ID_STR_42
#define MINUTE_43 ID_STR_43
#define MINUTE_44 ID_STR_44
#define MINUTE_45 ID_STR_45
#define MINUTE_46 ID_STR_46
#define MINUTE_47 ID_STR_47
#define MINUTE_48 ID_STR_48
#define MINUTE_49 ID_STR_49
#define MINUTE_50 ID_STR_50
#define MINUTE_51 ID_STR_51
#define MINUTE_52 ID_STR_52
#define MINUTE_53 ID_STR_53
#define MINUTE_54 ID_STR_54
#define MINUTE_55 ID_STR_55
#define MINUTE_56 ID_STR_56
#define MINUTE_57 ID_STR_57
#define MINUTE_58 ID_STR_58
#define MINUTE_59 ID_STR_59


#define MONTH_NUM (12)
#define DAY_NUM   (31)
#define YEAR_NUM  (12)
#define HOUR_NUM  (24)
#define MINUTE_NUM  (60)


extern HI_S32	HI_GV_WheelView_SetSelItem(HI_HANDLE hWheelview, HI_U32 Item);
extern HI_S32 HI_GV_WheelView_SetUpImage(HI_HANDLE hWheelview, HI_HANDLE hRes);

extern HI_S32 HI_GV_WheelView_SetUpImage(HI_HANDLE hWheelview, HI_HANDLE hRes);
extern HI_S32 HI_GV_WheelView_SetDownImage(HI_HANDLE hWheelview, HI_HANDLE hRes);

static HI_CHAR aszTimeStr[100];
static HI_S32 s_year = 2017;
static HI_S32 s_mounth = 01;
static HI_S32 s_day = 01;
static HI_S32 s_hour = 00;
static HI_S32 s_min = 00;




HI_S32 WHEELVIEW_WIN_IMAGE1_ontouchaction(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
   // (HI_VOID)HI_GV_Widget_Show(MAIN_WIN);
   // (HI_VOID)HI_GV_Widget_Active(MAIN_WIN);
  //  (HI_VOID)HI_GV_Widget_Hide(WHEELVIEW_WIN);

    return HIGV_PROC_GOON;
}

static HI_S32 UpdateMonthData()
{
    HI_S32 Ret;
    HI_U32 u32Index = 0;
    HI_HANDLE hDDB = 0;
    HIGV_DBROW_S stDbRow;
    HI_U32 StrSet[MONTH_NUM]
        = {Month_1, Month_2, Month_3, Month_4, Month_5, Month_6, Month_7, Month_8, Month_9, Month_10, Month_11, Month_12};

    /** Get DDB (default data base) handle*/
    Ret = HI_GV_ADM_GetDDBHandle(ADM_MONTH, &hDDB);

    if (HI_SUCCESS != Ret)
    {
        return -1;
    }

    /** Disable DDB change*/
    HI_GV_DDB_EnableDataChange(hDDB, HI_FALSE);

    /** Clear DDB*/
    HI_GV_DDB_Clear(hDDB);

    /** Add data*/
    for (u32Index = 0; u32Index < MONTH_NUM; u32Index++ )
    {
        memset(&stDbRow, 0x00, sizeof(stDbRow));
        stDbRow.pData = &StrSet[u32Index];
        stDbRow.Size  = sizeof(HI_U32);
        HI_GV_DDB_Append(hDDB, &stDbRow);
    }

    /** Enable DDB change*/
    HI_GV_DDB_EnableDataChange(hDDB, HI_TRUE);

    HI_GV_Widget_SyncDB(WHEELVIEW_MONTH);

    return HI_SUCCESS;
}

static HI_S32 UpdateDayData()
{
    HI_S32 Ret;
    HI_U32 u32Index = 0;
    HI_HANDLE hDDB = 0;
    HIGV_DBROW_S stDbRow;
    HI_U32 StrSet[DAY_NUM] = {Day_1, Day_2, Day_3, Day_4, Day_5, Day_6, Day_7, Day_8, Day_9, Day_10,
                              Day_11, Day_12, Day_13, Day_14, Day_15, Day_16, Day_17, Day_18, Day_19, Day_20,
                              Day_21, Day_22, Day_23, Day_24, Day_25, Day_26, Day_27, Day_28, Day_29, Day_30,
                              Day_31
                             };

    /** Get DDB (default data base) handle*/
    Ret = HI_GV_ADM_GetDDBHandle(ADM_DAY, &hDDB);

    if (HI_SUCCESS != Ret)
    {
        return -1;
    }

    /** Disable DDB change*/
    HI_GV_DDB_EnableDataChange(hDDB, HI_FALSE);

    /** Clear DDB*/
    HI_GV_DDB_Clear(hDDB);

    /** Add data*/
    for (u32Index = 0; u32Index < DAY_NUM; u32Index++)
    {
        memset(&stDbRow, 0x00, sizeof(stDbRow));
        stDbRow.pData = &StrSet[u32Index];
        stDbRow.Size  = sizeof(HI_U32);
        HI_GV_DDB_Append(hDDB, &stDbRow);
    }

    /** Enable DDB change*/
    HI_GV_DDB_EnableDataChange(hDDB, HI_TRUE);

    HI_GV_Widget_SyncDB(WHEELVIEW_DAY);

    return HI_SUCCESS;
}

#ifdef CFG_LEON
void AdjustDayMax()
{
    HI_S32 Ret;
    HI_U32 pRowCount;
    static HI_HANDLE hDDB = NULL;
    HIGV_DBROW_S stDbRow;
    HI_U32 StrSet[DAY_NUM] = {Day_29, Day_30, Day_31};
    HI_BOOL leap_year = (!(s_year % 4) && (s_year % 100));
    if(!hDDB)
    {
    	if(HI_SUCCESS != (Ret = HI_GV_ADM_GetDDBHandle(ADM_DAY, &hDDB)));
        return -1;
    } 

    HI_GV_DDB_EnableDataChange(hDDB, HI_FALSE);

    HI_GV_DDB_GetRowCount(hDDB, &pRowCount);
    
    if(s_mounth == 1 || s_mounth == 3 || s_mounth == 5\
            || s_mounth == 7 || s_mounth == 8 || s_mounth == 10\
            || s_mounth == 12)
    {
        switch(pRowCount)
        {
            case 28:
                stDbRow.pData = &StrSet[0];
                stDbRow.Size  = sizeof(HI_U32);
                HI_GV_DDB_Append(hDDB, &stDbRow);
            case 29:
                stDbRow.pData = &StrSet[1];
                stDbRow.Size  = sizeof(HI_U32);
                HI_GV_DDB_Append(hDDB, &stDbRow);
            case 30:
                stDbRow.pData = &StrSet[2];
                stDbRow.Size  = sizeof(HI_U32);
                HI_GV_DDB_Append(hDDB, &stDbRow);
        }
    }
    else if(s_mounth == 2)    
    {
        if(leap_year)
        {
            if(s_day > 29)
            {
               s_day = 29;
               HI_GV_WheelView_SetSelItem(WHEELVIEW_DAY, s_day-1);
            }
        }
        else
        {
            if(s_day > 28)
            {
               s_day = 28;
               HI_GV_WheelView_SetSelItem(WHEELVIEW_DAY, s_day-1);
            }
        }
        switch(pRowCount)
        {
            case 31:
                HI_GV_DDB_Del(hDDB, 31 - 1);
            case 30:
                HI_GV_DDB_Del(hDDB, 30 - 1);
            case 29:
                if(!leap_year)
                    HI_GV_DDB_Del(hDDB, 29 - 1);
                break;
            case 28:
                if(leap_year)
                {
                    stDbRow.pData = &StrSet[0];
                    stDbRow.Size  = sizeof(HI_U32);
                    HI_GV_DDB_Append(hDDB, &stDbRow);
                }
        }
    }
    else
    {
        if(s_day > 30)
            s_day = 30;
        HI_GV_WheelView_SetSelItem(WHEELVIEW_DAY, s_day-1);
        switch(pRowCount)
        {
            case 31:
                HI_GV_DDB_Del(hDDB, 31 - 1);
                break;
            case 28:
                stDbRow.pData = &StrSet[0];
                stDbRow.Size  = sizeof(HI_U32);
                HI_GV_DDB_Append(hDDB, &stDbRow);
            case 29:
                stDbRow.pData = &StrSet[1];
                stDbRow.Size  = sizeof(HI_U32);
                HI_GV_DDB_Append(hDDB, &stDbRow);
        }
    }
    
    HI_GV_DDB_EnableDataChange(hDDB, HI_TRUE);

    HI_GV_Widget_SyncDB(WHEELVIEW_DAY);

}
#endif

static HI_S32 UpdateMinuteData()
{
    HI_S32 Ret;
    HI_U32 u32Index = 0;
    HI_HANDLE hDDB = 0;
    HIGV_DBROW_S stDbRow;
    HI_U32 StrSet[MINUTE_NUM] = {
        MINUTE_0,MINUTE_1,MINUTE_2,MINUTE_3,MINUTE_4,MINUTE_5,MINUTE_6,MINUTE_7,MINUTE_8,MINUTE_9,
        MINUTE_10,MINUTE_11,MINUTE_12,MINUTE_13,MINUTE_14,MINUTE_15,MINUTE_16,MINUTE_17,MINUTE_18,MINUTE_19,
        MINUTE_20,MINUTE_21,MINUTE_22,MINUTE_23,MINUTE_24,MINUTE_25,MINUTE_26,MINUTE_27,MINUTE_28,MINUTE_29,
        MINUTE_30,MINUTE_31,MINUTE_32,MINUTE_33,MINUTE_34,MINUTE_35,MINUTE_36,MINUTE_37,MINUTE_38,MINUTE_39,
        MINUTE_40,MINUTE_41,MINUTE_42,MINUTE_43,MINUTE_44,MINUTE_45,MINUTE_46,MINUTE_47,MINUTE_48,MINUTE_49,
        MINUTE_50,MINUTE_51,MINUTE_52,MINUTE_53,MINUTE_54,MINUTE_55,MINUTE_56,MINUTE_57,MINUTE_58,MINUTE_59,
    };
    /** Get DDB (default data base) handle*/
    Ret = HI_GV_ADM_GetDDBHandle(ADM_MINUTE, &hDDB);

    if (HI_SUCCESS != Ret)
    {
        return -1;
    }

    /** Disable DDB change*/
    HI_GV_DDB_EnableDataChange(hDDB, HI_FALSE);

    /** Clear DDB*/
    HI_GV_DDB_Clear(hDDB);

    /** Add data*/
    for (u32Index = 0; u32Index < MINUTE_NUM; u32Index++)
    {
        memset(&stDbRow, 0x00, sizeof(stDbRow));
        stDbRow.pData = &StrSet[u32Index];
        stDbRow.Size  = sizeof(HI_U32);
        HI_GV_DDB_Append(hDDB, &stDbRow);
    }

    /** Enable DDB change*/
    HI_GV_DDB_EnableDataChange(hDDB, HI_TRUE);

    HI_GV_Widget_SyncDB(WHEELVIEW_MINUTE);

    return HI_SUCCESS;
}

static HI_S32 UpdateHourData()
{
    HI_S32 Ret;
    HI_U32 u32Index = 0;
    HI_HANDLE hDDB = 0;
    HIGV_DBROW_S stDbRow;
    HI_U32 StrSet[HOUR_NUM] = {
        Hour_0,Hour_1,Hour_2,Hour_3,Hour_4,Hour_5,Hour_6,Hour_7,Hour_8,Hour_9,
        Hour_10,Hour_11,Hour_12,Hour_13,Hour_14,Hour_15,Hour_16,Hour_17,Hour_18,Hour_19,
        Hour_20,Hour_21,Hour_22,Hour_23
    };
    /** Get DDB (default data base) handle*/
    Ret = HI_GV_ADM_GetDDBHandle(ADM_HOUR, &hDDB);

    if (HI_SUCCESS != Ret)
    {
        return -1;
    }

    /** Disable DDB change*/
    HI_GV_DDB_EnableDataChange(hDDB, HI_FALSE);

    /** Clear DDB*/
    HI_GV_DDB_Clear(hDDB);

    /** Add data*/
    for (u32Index = 0; u32Index < HOUR_NUM; u32Index++)
    {
        memset(&stDbRow, 0x00, sizeof(stDbRow));
        stDbRow.pData = &StrSet[u32Index];
        stDbRow.Size  = sizeof(HI_U32);
        HI_GV_DDB_Append(hDDB, &stDbRow);
    }

    /** Enable DDB change*/
    HI_GV_DDB_EnableDataChange(hDDB, HI_TRUE);

    HI_GV_Widget_SyncDB(WHEELVIEW_HOUR);

    return HI_SUCCESS;
}

static HI_S32 UpdateYearData()
{
    HI_S32 Ret;
    HI_U32 u32Index = 0;
    HI_HANDLE hDDB = 0;
    HIGV_DBROW_S stDbRow;
    HI_U32 StrSet[YEAR_NUM] = {Year_1, Year_2, Year_3, Year_4, Year_5, Year_6, Year_7, Year_8, Year_9, Year_10, Year_11, Year_12};

    /** Get DDB (default data base) handle*/
    Ret = HI_GV_ADM_GetDDBHandle(ADM_YEAR, &hDDB);

    if (HI_SUCCESS != Ret)
    {
        return -1;
    }

    /** Disable DDB change*/
    HI_GV_DDB_EnableDataChange(hDDB, HI_FALSE);

    /** Clear DDB*/
    HI_GV_DDB_Clear(hDDB);

    /** Add data*/
    for (u32Index = 0; u32Index < YEAR_NUM; u32Index++)
    {
        memset(&stDbRow, 0x00, sizeof(stDbRow));
        stDbRow.pData = &StrSet[u32Index];
        stDbRow.Size  = sizeof(HI_U32);
        HI_GV_DDB_Append(hDDB, &stDbRow);
    }

    /** Enable DDB change*/
    HI_GV_DDB_EnableDataChange(hDDB, HI_TRUE);

    HI_GV_Widget_SyncDB(WHEELVIEW_YEAR);

    return HI_SUCCESS;
}

HI_S32 WheelViewOnShowListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    UpdateMonthData();
    UpdateDayData();
    UpdateYearData();
    UpdateHourData();
    UpdateMinuteData();

    s32Ret |= HI_RTCMNG_GetTime(aszTimeStr);
    MLOGE("aszTimeStr=%s\n",aszTimeStr);
    HI_CHAR tmp[8] = {0};
    struct tm stTime;


    memset(&stTime, 0, sizeof(struct tm));

    strncpy(tmp, &aszTimeStr[4], 2);
    stTime.tm_mon = atoi(tmp) - 1;

    strncpy(tmp, &aszTimeStr[6], 2);
    stTime.tm_mday = atoi(tmp);

    strncpy(tmp, &aszTimeStr[8], 2);
    stTime.tm_hour = atoi(tmp);

    strncpy(tmp, &aszTimeStr[10], 2);
    stTime.tm_min = atoi(tmp);

    strncpy(tmp, &aszTimeStr[12], 2);
    stTime.tm_sec = atoi(tmp);

    strncpy(tmp, &aszTimeStr[0], 4);
    stTime.tm_year = atoi(tmp)-1900;

    MLOGD("%d-%d-%d  %d:%d\n", stTime.tm_year+1900, stTime.tm_mon+1, stTime.tm_mday, stTime.tm_hour, stTime.tm_min);
    s_year = stTime.tm_year + 1900;
    s_mounth = stTime.tm_mon + 1;
    s_day = stTime.tm_mday;
    s_hour = stTime.tm_hour;
    s_min = stTime.tm_min;

    MLOGE("%d-%d-%d  %d:%d\n",s_year, s_mounth, s_day, s_hour,s_min);

    HI_GV_WheelView_SetSelItem(WHEELVIEW_YEAR, s_year-2017);        
    HI_GV_WheelView_SetSelItem(WHEELVIEW_MONTH, s_mounth-1);
    HI_GV_WheelView_SetSelItem(WHEELVIEW_DAY, s_day-1);
    HI_GV_WheelView_SetSelItem(WHEELVIEW_HOUR, s_hour);
    HI_GV_WheelView_SetSelItem(WHEELVIEW_MINUTE, s_min);

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }

#ifdef CFG_LEON
    AdjustDayMax();
#endif

    return HIGV_PROC_GOON;
}

HI_S32 OnDayFocusSelectListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //printf("[Func: %s, Line: %d] wParam: %u\n\n", __FUNCTION__, __LINE__, wParam);
    s_day = 1 + wParam;

    return HIGV_PROC_GOON;
}

HI_S32 OnMonthFocusSelectListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //printf("[Func: %s, Line: %d] wParam: %u\n\n", __FUNCTION__, __LINE__, wParam);
    s_mounth = 1 + wParam;
    #ifdef CFG_LEON
    AdjustDayMax();
    #endif
    return HIGV_PROC_GOON;
}

HI_S32 OnYearFocusSelectListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //printf("[Func: %s, Line: %d] wParam: %u\n\n", __FUNCTION__, __LINE__, wParam);
    s_year = 2017 + wParam;
    #ifdef CFG_LEON
    AdjustDayMax();
    #endif
    return HIGV_PROC_GOON;
}
HI_S32 OnHourFocusSelectListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //printf("[Func: %s, Line: %d] wParam: %u\n\n", __FUNCTION__, __LINE__, wParam);
    s_hour = 0 + wParam;

    return HIGV_PROC_GOON;
}
HI_S32 OnMinuteFocusSelectListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //printf("[Func: %s, Line: %d] wParam: %u\n\n", __FUNCTION__, __LINE__, wParam);
    s_min = 0 + wParam;

    return HIGV_PROC_GOON;
}

HI_S32 OnMonthListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;

    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));

    type = touchEvent.last.type;

    if (HIGV_TOUCH_START == type)
    {
        HI_GV_WheelView_SetUpImage(WHEELVIEW_MONTH, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_MONTH, background_skin);
        HI_GV_WheelView_SetUpImage(WHEELVIEW_DAY, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_DAY, background_skin);
        HI_GV_WheelView_SetUpImage(WHEELVIEW_YEAR, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_YEAR, background_skin);

        HI_GV_Widget_Paint(WHEELVIEW_MONTH, 0);
        HI_GV_Widget_Paint(WHEELVIEW_DAY, 0);
        HI_GV_Widget_Paint(WHEELVIEW_YEAR, 0);
    }

    return HIGV_PROC_GOON;
}

HI_S32 OnDayListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;
  //  HI_HANDLE handle;

    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));

    type = touchEvent.last.type;

    if (HIGV_TOUCH_START == type)
    {
        HI_GV_WheelView_SetUpImage(WHEELVIEW_MONTH, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_MONTH, background_skin);
        HI_GV_WheelView_SetUpImage(WHEELVIEW_DAY, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_DAY, background_skin);
        HI_GV_WheelView_SetUpImage(WHEELVIEW_YEAR, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_YEAR, background_skin);

        HI_GV_Widget_Paint(WHEELVIEW_MONTH, 0);
        HI_GV_Widget_Paint(WHEELVIEW_DAY, 0);
        HI_GV_Widget_Paint(WHEELVIEW_YEAR, 0);
    }

    return HIGV_PROC_GOON;
}
HI_S32 OnHourListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;
  //  HI_HANDLE handle;

    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));

    type = touchEvent.last.type;

    if (HIGV_TOUCH_START == type)
    {
        HI_GV_WheelView_SetUpImage(WHEELVIEW_MONTH, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_MONTH, background_skin);
        HI_GV_WheelView_SetUpImage(WHEELVIEW_DAY, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_DAY, background_skin);
        HI_GV_WheelView_SetUpImage(WHEELVIEW_YEAR, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_YEAR, background_skin);

        HI_GV_Widget_Paint(WHEELVIEW_MONTH, 0);
        HI_GV_Widget_Paint(WHEELVIEW_DAY, 0);
        HI_GV_Widget_Paint(WHEELVIEW_YEAR, 0);
    }

    return HIGV_PROC_GOON;
}
HI_S32 OnMinuteListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;
  //  HI_HANDLE handle;

    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));

    type = touchEvent.last.type;

    if (HIGV_TOUCH_START == type)
    {
        HI_GV_WheelView_SetUpImage(WHEELVIEW_MONTH, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_MONTH, background_skin);
        HI_GV_WheelView_SetUpImage(WHEELVIEW_DAY, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_DAY, background_skin);
        HI_GV_WheelView_SetUpImage(WHEELVIEW_YEAR, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_YEAR, background_skin);

        HI_GV_Widget_Paint(WHEELVIEW_MONTH, 0);
        HI_GV_Widget_Paint(WHEELVIEW_DAY, 0);
        HI_GV_Widget_Paint(WHEELVIEW_YEAR, 0);
    }

    return HIGV_PROC_GOON;
}

HI_S32 OnYearListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;

    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));

    type = touchEvent.last.type;

    if (HIGV_TOUCH_START == type)
    {
        HI_GV_WheelView_SetUpImage(WHEELVIEW_MONTH, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_MONTH, background_skin);
        HI_GV_WheelView_SetUpImage(WHEELVIEW_DAY, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_DAY, background_skin);
        HI_GV_WheelView_SetUpImage(WHEELVIEW_YEAR, background_skin);
        HI_GV_WheelView_SetDownImage(WHEELVIEW_YEAR, background_skin);
        HI_GV_Widget_Paint(WHEELVIEW_MONTH, 0);
        HI_GV_Widget_Paint(WHEELVIEW_DAY, 0);
        HI_GV_Widget_Paint(WHEELVIEW_YEAR, 0);
    }

    return HIGV_PROC_GOON;
}

HI_S32 OnMonthScrollListener(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //HI_GV_Msg_SendAsync(WHEELVIEW_DAY, HIGV_MSG_GESTURE_SCROLL, wParam, lParam);

    return HIGV_PROC_GOON;
}


HI_S32 FEIYU_TIME_SET_CONFIRM_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    MLOGE("%d-%d-%d-%d-%d\n",s_year, s_mounth, s_day, s_hour,s_min);
    snprintf(aszTimeStr, 41, "%4d%2d%2d%2d%2d%2d",s_year,s_mounth,s_day,s_hour,s_min,0);    
    s32Ret |= HI_RTCMNG_SetTime(aszTimeStr);
    
    //s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
    //s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Hide(FEIYU_TIME_SET);

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }    
    HI_PDT_WORKSTATE_S stWorkState = {.enCmd = PDT_CMD_BUTT};
    //memset(&stWorkState, 0, sizeof(HI_PDT_WORKSTATE_S));
    extern HI_PDT_WORKMODE_S g_stCurrentWorkMode;
    stWorkState.stWorkMode = g_stCurrentWorkMode;        
    stWorkState.enEvent = PDT_EVT_UPDATE_PARAM;        
    stWorkState.setOpt = 0xff;
    stWorkState.param = 0xff;
    HI_PDT_EventProc_SetState(stWorkState);    
    return HIGV_PROC_GOON;    
}

HI_S32 FEIYU_TIME_SET_CANCEL_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("%d-%d-%d-%d-%d\n",s_year, s_mounth, s_day, s_hour,s_min);

    HI_S32 s32Ret = HI_SUCCESS;
    //s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
    //s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Hide(FEIYU_TIME_SET);

    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
    
    return HIGV_PROC_GOON;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
