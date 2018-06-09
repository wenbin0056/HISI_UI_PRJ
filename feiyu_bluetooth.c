
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
#include "hi_product_devmng_ext.h"
#include "hi_type.h"
#include "hi_mapi_sys.h"

#include "feiyu_UIAnimationTool.h"
#include "feiyu_dv_cmd_proc.h"
#include "feiyu_UICommon_FunTool.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

#if 0
#define MAX_BUFF_LEN    56

typedef struct  BLUETOOTHMAC_S
{
	HI_CHAR Text[MAX_BUFF_LEN];
    
}BLUETOOTHMAC;

static BLUETOOTHMAC s_BluetoothMac[5] = {
    "01:01:01:01:01:01",
    "01:01:01:01:01:02",
    "01:01:01:01:01:03",
    "01:01:01:01:01:04",
};
#endif

typedef unsigned char   UINT8;
#define BD_NAME_LEN     248
#define BD_ADDR_LEN     6
typedef UINT8 BD_NAME[BD_NAME_LEN + 1];         /* Device name */
typedef UINT8 BD_ADDR[BD_ADDR_LEN];

typedef struct fy_ble_s
{
	BD_NAME name;
	BD_ADDR bd_addr;
}FEIYU_BLE_S;

typedef struct fy_ble_buf_s{
	FEIYU_BLE_S feiyu_ble_disc_list[5];
	unsigned char num;
	unsigned char select;
	unsigned char status;//1 is ok,0 is not
						//bit0---disc status 
						//bit1---select status
						//bit2---connect status
}FEIYU_BLE_BUF_S;
extern volatile FEIYU_BLE_BUF_S feiyu_ble_buffer;
static HI_S32 initBluetoothMac();
static HI_S32 bleStartRet = 0;
extern HI_BOOL s_bBluetoothState;
static HI_S32 s_bCurrentPic = 0;
static HI_BOOL s_bluetooth_state = HI_FALSE;
static HI_BOOL s_bluetooth_is_connect = HI_FALSE;
static HI_BOOL s_isHandle = HI_FALSE;
static HI_BOOL s_isdisk_stop = HI_FALSE;
typedef enum hiPDT_TIMEDTASK_TYPE_E
{
    PDT_TIMEDTASK_TYPE_AUTOOFF = 0,
    PDT_TIMEDTASK_TYPE_DORMANT = 1,
    PDT_TIMEDTASK_TYPE_BUTT
} HI_PDT_TIMEDTASK_TYPE_E;

extern void feiyu_ble_disc_stop();
extern void feiyu_ble_disc_star();
extern HI_S32 HI_PDT_TimedTask_SetEnable(HI_PDT_TIMEDTASK_TYPE_E enType, HI_BOOL bEnable);

HI_S32 FEIYU_BLUETOOTH_onevent(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGD("wParam: %d, lParam = %x\n", wParam, lParam);
    if(BLUETOOTH_OPEN == wParam)
    {
        extern int feiyu_ble_start(void);
        s_isHandle = HI_TRUE;
        bleStartRet = feiyu_ble_start(); 
        if(bleStartRet != 0)
        {
            MLOGE("start ERR\n");
            s_bluetooth_state = HI_FALSE;
        }
        s_isHandle = HI_FALSE;
        s_bluetooth_state = HI_TRUE;        
    }
    return HIGV_PROC_GOON;    
}

HI_S32 FEIYU_BLUETOOTH_ontimer(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    static HI_BOOL s_firstFlag = HI_TRUE;
    HI_S32 s32Ret = HI_SUCCESS;
    static HI_U8 time_couter = 0;
    if(HI_TRUE == HI_GV_Widget_IsShow(FEIYU_BLUETOOTH_3))
    {
        switch(s_bCurrentPic)
        {
            case 0:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_1);
                break;
            case 1:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_2);            
                break;
            case 2:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_3);            
                break;
            case 3:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_4);            
                break;
            case 4:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_5);            
                break;
            case 5:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_6);            
                break;
            case 6:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_7);            
                break;
            case 7:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_8);            
                break;            
            case 8:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_9);            
                break;            
            case 9:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_10);            
                break;            
            case 10:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_11);            
                break;            
            case 11:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_12);            
                break;            
            case 12:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_13);            
                break;            
            case 13:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_14);            
                break;            
            case 14:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_15);            
                break;                        
            case 15:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_16);            
                break;                        
            case 16:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_17);            
                break;                        
            case 17:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_18);            
                break;                          
            default:
                HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_3, HIGV_SKIN_NORMAL, skin_wifi_load_1);            
                break;
        }
        HI_GV_Widget_Refresh(FEIYU_BLUETOOTH_3, 0);
        HI_GV_Widget_Update(FEIYU_BLUETOOTH_3, 0);  
        HI_GV_Widget_Refresh(FEIYU_BLUETOOTH, 0);
        HI_GV_Widget_Update(FEIYU_BLUETOOTH, 0);      
        HI_GV_Timer_Reset(FEIYU_BLUETOOTH, BLUETOOTH_TIMER);
        s_bCurrentPic++;    
        if(s_bCurrentPic > 17)
            s_bCurrentPic = 1;
    }

    
    //printf("#####\nstatus is 0x%04x\nadd is %x\n#####\n", feiyu_ble_buffer.status, &feiyu_ble_buffer.status);
    //finsh search
    if((feiyu_ble_buffer.status & 0x1) > 0)
    {
        MLOGE("===========finsh search====================\n");
        HI_GV_Widget_Hide(FEIYU_BLUETOOTH_3);  
        HI_GV_Widget_Show(FEIYU_BLUETOOTH_listbox);
        initBluetoothMac(); 
        feiyu_ble_buffer.num = 0;
        s_firstFlag = HI_FALSE;
        feiyu_ble_buffer.status &= (~(0x1 << 0));
        s_isdisk_stop = HI_TRUE;
    }
    
    if(s_isdisk_stop != HI_TRUE && ++time_couter >= 80)
    {
        MLOGE("===========finsh search====================\n");
        HI_GV_Widget_Hide(FEIYU_BLUETOOTH_3);  
        HI_GV_Widget_Show(FEIYU_BLUETOOTH_listbox);
        initBluetoothMac(); 
        feiyu_ble_buffer.num = 0;
        s_firstFlag = HI_FALSE;
        pthread_t threadid = 0;
	    pthread_create(&threadid, NULL, (void *)feiyu_ble_disc_stop, NULL);
        s_isdisk_stop = HI_TRUE;
        time_couter = 0;
    }
    
    //connect ok
    if((feiyu_ble_buffer.status & (0x1 << 2)) > 0)
    {
        MLOGE("===========connect ok====================\n");
        s32Ret = HI_GV_Widget_Hide(FEIYU_BLUETOOTH_3);  
        s32Ret = HI_GV_Widget_Hide(FEIYU_BLUETOOTH_listbox);
        s32Ret = HI_GV_Widget_SetTextByID(FEIYU_BLUETOOTH_8, ID_STR_CONNECT_SUCCESS);            
        feiyu_ble_buffer.status &= (~(0x1 << 2));
        s_bluetooth_is_connect = HI_TRUE;
    } 

    if(bleStartRet != 0)
    {  
        MLOGE("===========bleStartRet err====================\n");
        s32Ret = HI_GV_Widget_Hide(FEIYU_BLUETOOTH_3);  
        s32Ret = HI_GV_Widget_Hide(FEIYU_BLUETOOTH_listbox);      
        s32Ret |= HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_NORMAL, skin_wifi_off);
        s32Ret |= HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_ACITVE, skin_wifi_off);          
        s32Ret = HI_GV_Widget_SetTextByID(FEIYU_BLUETOOTH_8, ID_STR_START_FAILED);
        s32Ret |= HI_GV_Widget_Show(FEIYU_BLUETOOTH_8); 
        s_bluetooth_state = HI_FALSE;
        s_bluetooth_is_connect = HI_FALSE;
    }

    HI_GV_Widget_Paint(FEIYU_BLUETOOTH, 0); 
    HI_GV_Timer_Reset(FEIYU_BLUETOOTH, BLUETOOTH_TIMER); 
    
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("ERR,s32Ret=%d\n",s32Ret);
    }


    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_BLUETOOTH_listbox_oncellselect(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("===========oncellselect====================\n");    

    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_BLUETOOTH_listbox_onkeydown(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("~~~\n");
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_BLUETOOTH_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{ 	
    HIGV_GESTURE_EVENT_S gestureEvent;
    HI_S32 x, y;
    memset(&gestureEvent, 0x0, sizeof(gestureEvent));
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    x = gestureEvent.gesture.tap.pointer.x;
    y = gestureEvent.gesture.tap.pointer.y;
    //userMetry(x, y);
    if((x>= 350) && (x<=480) && (y>=0)&&(y<=120))
    {
        HI_GV_Msg_SendAsync(FEIYU_BLUETOOTH_2, HIGV_MSG_GESTURE_TAP, 0, 0);

        return HIGV_PROC_STOP;
    }
    else if((x >= 350)&&(x <= 450)&&(y >= 260)&&(y <= 310))
	{
		HI_GV_Msg_SendAsync(FEIYU_BLUETOOTH_6, HIGV_MSG_GESTURE_TAP, 0, 0);
        return HIGV_PROC_STOP;
	}
	else if((x >= 30)&&(x <= 130)&&(y >= 260)&&(y <= 310))
	{
		HI_GV_Msg_SendAsync(FEIYU_BLUETOOTH_4, HIGV_MSG_GESTURE_TAP, 0, 0);
        return HIGV_PROC_STOP;
	}                      
    
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_BLUETOOTH_onkeydown(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    return HIGV_PROC_GOON;
}

static HI_S32 initBluetoothMac()
{
	HI_U32 i;
	HI_HANDLE hDDB = INVALID_HANDLE;
	HIGV_DBROW_S DBRow;
	HI_S32 s32Ret;
    HI_U32 u32ItemNum = 0;
    u32ItemNum = 5;

    //Init
	s32Ret = HI_GV_ADM_GetDDBHandle(datamodel_bluetooth_listbox, &hDDB);
	if (HI_SUCCESS != s32Ret)
	{
        MLOGE("get ddb handle failed! s32Ret:%d\n",s32Ret);
		return s32Ret;
	}

    //Add to database
	HI_GV_DDB_EnableDataChange(hDDB, HI_FALSE);
	HI_GV_DDB_Clear(hDDB);
    u32ItemNum = feiyu_ble_buffer.num;
    MLOGE("u32ItemNum=%d\n",u32ItemNum);
    char buff[280] = {0};
	for (i = 0; i < u32ItemNum; i++)
	{
	    memset(buff, 0, sizeof(buff));
        sprintf(buff, "%x:%x:%x:%x:%x:%x",  \
            feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[0], \
            feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[1], \
            feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[2], \
            feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[3], \
            feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[4], \
            feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[5]);
        snprintf(buff+ strlen(buff), BD_NAME_LEN + 1, "%s ", feiyu_ble_buffer.feiyu_ble_disc_list[i].name);
        MLOGE("buff = %s\n",buff);        
		DBRow.pData = buff;
        /*
        MLOGE("--->>%x\n",feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[0]);
        MLOGE("--->>%x\n",feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[1]);
        MLOGE("--->>%x\n",feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[2]);
        MLOGE("--->>%x\n",feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[3]);
        MLOGE("--->>%x\n",feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[4]);
        MLOGE("--->>%x\n",feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[5]);
//        MLOGE("--->>%x\n",feiyu_ble_buffer.feiyu_ble_disc_list[i].bd_addr[6]);
*/
		DBRow.Size = sizeof(buff);
		s32Ret = HI_GV_DDB_Append(hDDB, &DBRow);
		if (HI_SUCCESS != s32Ret)
		{
            MLOGE("DDB_Append failed!\n");
		}
	}
	HI_GV_DDB_EnableDataChange(hDDB, HI_TRUE);

	s32Ret = HI_GV_Widget_SyncDB(FEIYU_BLUETOOTH_listbox);
	if(HI_SUCCESS != s32Ret)
	{
        MLOGE("Error: %#x\n", s32Ret);
	}

}
static void initBluetoothState()
{
    s_bluetooth_state = HI_FALSE;
    extern int feiyu_ble_close(void);
    feiyu_ble_close();

}
static HI_BOOL getBluetoothState()
{
    return s_bluetooth_state;
}

static void  doOpenBluetooth(void *arg)
{
    pthread_detach(pthread_self());
    s_isHandle = HI_TRUE;
    extern int feiyu_ble_start(void);
    bleStartRet = feiyu_ble_start(); 
    if(bleStartRet != 0)
    {
        MLOGE("start ERR\n");
        s_bluetooth_state = HI_FALSE;
    }
    s_bluetooth_state = HI_TRUE;
    s_isHandle = HI_FALSE;
    s_isdisk_stop = HI_FALSE;
    MLOGE("TURN ON\n");
}

static void openBluetooth()
{
#if 1
    if(s_isHandle)
    {
        return ;
    }
    HI_S32 s32Ret = 0;
    pthread_t threadid = 0;
	s32Ret = pthread_create(&threadid, NULL, (void *)doOpenBluetooth, NULL);
	if(0 != s32Ret)
	{
		MLOGE("pthread_create doShowPhotoUI Failed, s32Ret=%#x\n", s32Ret);
    } 
#endif  
    //HI_GV_Msg_SendAsync(FEIYU_BLUETOOTH, HIGV_MSG_EVENT, BLUETOOTH_OPEN, 0);
}

static void closeBluetooth()
{
    extern int feiyu_ble_close(void);
    feiyu_ble_close();
    s_bluetooth_state = HI_FALSE;    
    s_bluetooth_is_connect = HI_FALSE;
}



HI_S32 FEIYU_BLUETOOTH_onshow(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
	//turn on
	bleStartRet = 0;
    s_isHandle = HI_FALSE;
    if(s_bluetooth_is_connect)
    {
        s32Ret |= HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_NORMAL, skin_wifi_on);
        s32Ret |= HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_ACITVE, skin_wifi_on);   
        s32Ret = HI_GV_Widget_SetTextByID(FEIYU_BLUETOOTH_8, ID_STR_ACONNECT);        
        s32Ret |= HI_GV_Widget_Show(FEIYU_BLUETOOTH_8);                  
        
    }
    else
    {
        initBluetoothState();
        s32Ret |= HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_NORMAL, skin_wifi_off);
        s32Ret |= HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_ACITVE, skin_wifi_off);     
        s32Ret |= HI_GV_Widget_Hide(FEIYU_BLUETOOTH_8);          
    }

    s32Ret |= HI_GV_Widget_Hide(FEIYU_BLUETOOTH_3);    
    s32Ret |= HI_GV_Widget_Hide(FEIYU_BLUETOOTH_listbox);    
    s32Ret |= HI_GV_Widget_Paint(FEIYU_BLUETOOTH, 0);    
    
    MLOGE("s_bluetooth_is_connect=%d\n",s_bluetooth_is_connect);
    
    HI_GV_Timer_Create(FEIYU_BLUETOOTH, BLUETOOTH_TIMER, BLUETOOTH_TIMER_INTERVAL);
    HI_GV_Timer_Start(FEIYU_BLUETOOTH, BLUETOOTH_TIMER);
	if(HI_SUCCESS != s32Ret)
	{
        MLOGE("Error: %#x\n", s32Ret);
	}    
    
    HI_S32 HI_PDT_TimedTask_SetEnable(PDT_TIMEDTASK_TYPE_DORMANT, HI_FALSE);
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_BLUETOOTH_onhide(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_GV_Timer_Stop(FEIYU_BLUETOOTH, BLUETOOTH_TIMER);
    HI_GV_Timer_Destory(FEIYU_BLUETOOTH, BLUETOOTH_TIMER);   
    if(!s_bluetooth_is_connect)
    {
        closeBluetooth();
    }

    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_BLUETOOTH_EXIT_button_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    s32Ret |= HI_GV_Widget_Hide(FEIYU_BLUETOOTH);
	if(HI_SUCCESS != s32Ret)
	{
        MLOGE("Error: %#x\n", s32Ret);
	}    
    HI_S32 HI_PDT_TimedTask_SetEnable(PDT_TIMEDTASK_TYPE_DORMANT, HI_TRUE);

    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_BLUETOOTH_6_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    //connect
    HI_U32 Item = 0; 
    HI_U32 Col = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    if(s_isdisk_stop == HI_TRUE)
    {
        s32Ret = HI_GV_List_GetSelCell(FEIYU_BLUETOOTH_listbox, &Item, &Col);
        if(HI_SUCCESS != s32Ret)
        {
            MLOGE("err~~~\n");
        }
        MLOGE("##############connect~~~,Item=%d\n",Item);
        
        feiyu_ble_buffer.select = Item;
        feiyu_ble_buffer.status |= 0x1 << 1;
        
        s32Ret = HI_GV_Widget_Hide(FEIYU_BLUETOOTH_listbox);
        s32Ret = HI_GV_Widget_SetTextByID(FEIYU_BLUETOOTH_8, ID_STR_CONNECTING);
        s32Ret |= HI_GV_Widget_Show(FEIYU_BLUETOOTH_8);
        s32Ret = HI_GV_Widget_Paint(FEIYU_BLUETOOTH, 0);  
        
        
        if(HI_SUCCESS != s32Ret)
        {
            MLOGE("Error: %#x\n", s32Ret);
        }    

    }
    return HIGV_PROC_GOON;
}

HI_S32 FEIYU_BLUETOOTH_2_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{    
    MLOGE("########\n");
    HI_S32 s32Ret = HI_SUCCESS;
    if(s_bluetooth_is_connect)
    {
        HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_NORMAL, skin_wifi_off);
        HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_ACITVE, skin_wifi_off);  
         HI_GV_Widget_Hide(FEIYU_BLUETOOTH_3);  
         HI_GV_Widget_Hide(FEIYU_BLUETOOTH_8);
         s32Ret |= HI_GV_Widget_Hide(FEIYU_BLUETOOTH_listbox);
        s_bluetooth_is_connect = HI_FALSE;
        closeBluetooth();
    }
    else
    {
        if(HI_TRUE == getBluetoothState())
        {
            HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_NORMAL, skin_wifi_off); 
            HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_ACITVE, skin_wifi_off); 
            HI_GV_Widget_Hide(FEIYU_BLUETOOTH_3);            
            HI_GV_Widget_Hide(FEIYU_BLUETOOTH_8);
            s32Ret |= HI_GV_Widget_Hide(FEIYU_BLUETOOTH_listbox);
            closeBluetooth();              
        }
        else
        {
            HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_NORMAL, skin_wifi_on); 
            HI_GV_Widget_SetSkin(FEIYU_BLUETOOTH_2, HIGV_SKIN_ACITVE, skin_wifi_on); 
            HI_GV_Widget_Update(FEIYU_BLUETOOTH_2, 0);
            HI_GV_Widget_Refresh(FEIYU_BLUETOOTH_2, 0);
            HI_GV_Widget_Show(FEIYU_BLUETOOTH_3);            
            openBluetooth();            
        }
    }
    
    s32Ret = HI_GV_Widget_Paint(FEIYU_BLUETOOTH, 0);  


    #if 0
    pthread_t threadid = 0;
	s32Ret = pthread_create(&threadid, NULL, (void *)doOpenBluetooth, NULL);
	if(0 != s32Ret)
	{
		MLOGE("pthread_create doShowPhotoUI Failed, s32Ret=%#x\n", s32Ret);
    }    
    #endif
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("Error: %#x\n", s32Ret);
    }        
    
    return HIGV_PROC_GOON;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

