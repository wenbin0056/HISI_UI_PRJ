#include "higv_cextfile.h"
#include "add_ddb.h"
#include "ui_common.h"
#include "hi_gv_input.h"
#include "hi_product_playback_ext.h"
#include "hi_product_filemng_ext.h"
#include "hi_liteplayer.h"
#include "hi_product_media_ext.h"
#include <unistd.h>
#include "hi_usb_mng.h"
#include "feiyu_UICommon_FunTool.h"
#include "jpeglib.h"
#include "feiyu_dv_cmd_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*  __cplusplus  */

#define EVENT_REFRESH_UI 0
#define EVENT_PLAY_END   1
#define EVENT_PLAY_ERROR 2
#define EVENT_FILELIST_PAGE 3
#ifdef CFG_HJD
#define EVENT_TMP_REFRESH_UI 4
#endif

#ifndef PATH_MAX
#define PATH_MAX 256
#endif

#define SHOW_PHOTO  1
#define SHOW_VIDEO  2

static HI_S32 scrollflag = 0;
HI_BOOL s_bplaypause = HI_FALSE;
HI_BOOL s_bplaypausetask = HI_FALSE;
static volatile HI_BOOL s_bIsTracking = HI_FALSE;
static HI_BOOL s_bIsUIShowing = HI_TRUE;
HI_U32 g_u32PlayTime = 0;

HI_S32 g_s32VolumePercent = 100;
HI_BOOL s_bIsMute = HI_FALSE;
extern HI_U32 g_oldu32CurGroupIndex;
extern HI_S32 g_filelist_flag;

//player relative data
extern HI_U32 g_u32CurGroupIndex ;//grid id equal group id
static HI_U32 s_u32GroupCnt = 0;
static HI_PDT_FILEMNG_GROUP_INFO_S s_stCurGroupInfo;
static HI_U32 s_u32GroupFileCnt = 1;//file num in a group.at least 1 .
static HI_U32 s_u32FileIndex = 1;//file index in a group, default is 1 .
static HI_CHAR s_aszCurFilePath[256];
static HI_FILE_INFO_S s_stCurFileInfo;
extern HI_BOOL g_bRecOrSnap;	//Rec-HI_TRUE  Snap--HI_FALSE
extern HI_BOOL g_bLockSate;
	
HI_U32 g_u32FileType = PLAY_FILE_VIDEO;

HI_U32 g_u32PlayerFileTime = 0;

static HI_HANDLE s_PlayBackBtnIds[] =
{
    PLAYBACK_PAGE_BUTTON_BACK,
	PLAYBACK_PAGE_BUTTON_VOLUME,
    PLAYBACK_PAGE_BUTTON_DEL,
	//PLAYBACK_PAGE_BUTTON_PREV,
    PLAYBACK_PAGE_BUTTON_PLAY,
	//PLAYBACK_PAGE_BUTTON_NEXT,
};
// static HI_HANDLE s_activeBtn = PLAYBACK_PAGE_BUTTON_BACK;
static HI_BOOL s_isPlayingFlag = HI_FALSE;
extern HI_PDT_WORKMODE_S g_stSavedWorkMode;

#ifdef WENBIN
#define MY_TRACK_BAR
#ifdef MY_TRACK_BAR

#include "hi_gv_graphiccontext.h"
HI_HANDLE s_ImgHandle = 0; 
HI_HANDLE s_WinGC = 0; 
HI_S32 g_total_playtime = 100;
HI_S32 g_current_playtime = 0;

static void calc_rect(HI_RECT *pRect)
{
	float total = g_u32PlayerFileTime;
	float current = g_u32PlayTime;
	
	pRect->x = 0;
	pRect->y = 0;
	pRect->w = (HI_S32)((480/total)*current);
	pRect->h = 30;
}

HI_S32 TrackBarPaintProc(HI_HANDLE hWidget, HI_U32 wParam, HI_U32  lParam) 
{
	//HI_S32 Ret;
	HI_RECT Rect;
	calc_rect(&Rect);
	HI_GV_GraphicContext_Begin(s_WinGC); 
	HI_GV_GraphicContext_DrawImage(s_WinGC, &Rect, s_ImgHandle, NULL, NULL); 

	/*Rect.x= 200;
	Rect.y= 12;
	Rect.w= 150;	
	Rect.h= 20; 
	HI_CHAR buff[32] = {0};

	HI_U32 interval_time = g_u32PlayerFileTime -g_u32PlayTime;
	HI_S32 hour_val = interval_time/(60*60*1000);
	HI_S32 min_val = (interval_time - (60*60*1000)*hour_val)/(60*1000);
	HI_S32 sec_val = (interval_time - (60*60*1000)*hour_val - (60*1000)*min_val)/1000;
	snprintf(buff, 32,"%02d:%02d:%02d", hour_val, min_val, sec_val);
	HI_GV_GraphicContext_SetFgColor(s_WinGC, 0xFFFFFFFF); //0xFFFFF000
	HI_GV_GraphicContext_DrawText(s_WinGC, buff, &Rect,  0); //0//HIGV_ALIGN_VCENTER
	//HI_GV_GraphicContext_SetFgColor(s_WinGC, 0xFFFFFF00); //0xFFFFF000*/

	//HI_GV_GraphicContext_DrawLine(s_WinGC, 0, 5, 150, 5); 
	//HI_GV_GraphicContext_DrawLine(s_WinGC, 5, 10, 150, 10); 
	//HI_GV_GraphicContext_DrawLine(s_WinGC, 10, 15, 150, 15); 
	HI_GV_GraphicContext_End(s_WinGC); 
	
	return HI_SUCCESS; 
} 

#endif
#endif

#ifdef CFG_HJD

HI_S32 g_s32Width = 0;
HI_S32 g_s32Height = 0;

/**
    path[input]: file path 
    height width[output]: file info
*/
static int getJpgHeader(char * path, int *width, int *height)
{
	/* 
	 *  1. Assigning and initializing a JPEG unzip object 
	 */
	struct jpeg_decompress_struct dinfo; //define a JPEG unzip object 
	struct jpeg_error_mgr jerr; //define an error variable
	
	dinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&dinfo); //init the JPEG unzip object 

	/*
	 *  2. Specify the image file to be uncompressed
	 */
	FILE *infile;
	infile = fopen(path, "r");
	if (infile == NULL)
	{
		perror("fopen error");
		return -1;
	}

	jpeg_stdio_src(&dinfo, infile); //Specify the file to unzip for the unzip object dinfo

	/*
	 *  3. Call jpeg_read_header () to get the image information
	 */
	jpeg_read_header(&dinfo, TRUE);

	MLOGD("-- FILE INFO -- width:%d, height:%d\n", dinfo.image_width, dinfo.image_height);
	*width = dinfo.image_width;
	*height = dinfo.image_height;

	jpeg_destroy_decompress(&dinfo); // Release resources
	fclose(infile);
	
	return 0;
}
#endif


/**
    input: group info, file id
    output: file info, file path
*/
static HI_S32 updateFileInfo()
{
    HI_S32 s32Ret = 0;
    HI_CHAR aszGroupPath[256] = {0};
    s32Ret = HI_PDT_FileMng_GetGroupFileInfo(&s_stCurGroupInfo, s_u32FileIndex, &s_stCurFileInfo);
    if (s32Ret != HI_SUCCESS )
    {
        MLOGE("HI_PDT_FileMng_GetGroupFileInfo failed\n");
        return HI_FAILURE;
    }

    s32Ret = HI_PDT_FileMng_GetGroupPath(&s_stCurGroupInfo, aszGroupPath, 256);

    if (s32Ret != HI_SUCCESS)
    {
        MLOGE("GetGroupPath failed\n");
        return HI_FAILURE;
    }

    snprintf(s_aszCurFilePath, 256, "%s/%s", aszGroupPath, s_stCurFileInfo.aszFileName);
    printf("=========================================== \n");
	printf("CurFilePath:%s\n", s_aszCurFilePath);
    printf("filename:%s, filenum:%d, file id:%d, group index:%d, group id:%d ,group count:%d \n",
        s_stCurFileInfo.aszFileName, s_u32GroupFileCnt, s_u32FileIndex,
        g_u32CurGroupIndex, s_stCurGroupInfo.stGroupInfo.u32GrpID, s_u32GroupCnt);
    printf("=========================================== \n");


    return HI_SUCCESS;
}
/**
    input: group id
    output: group info
*/
static HI_S32 updateGroupInfo()
{
    HI_S32 s32Ret = 0;
    //basic info is grid id
    //get group count
    s32Ret = HI_PDT_FileMng_GetGroupCount(&s_u32GroupCnt);

    if (s32Ret)
    {
        MLOGE("HI_PDT_FileMng_GetGroupCount \n");
        return HI_FAILURE;
    }

    if (s_u32GroupCnt == 0)
    {
        MLOGD("s_u32GroupCnt is 0 \n");
        return HI_FAILURE;
    }

    if (g_u32CurGroupIndex > s_u32GroupCnt)
    {
        //if delete the last group, this will work to adjust the index to proper value.
        g_u32CurGroupIndex = s_u32GroupCnt;
    }

    MLOGD("g_u32CurGroupIndex:%d, s_u32GroupCnt:%d \n", g_u32CurGroupIndex, s_u32GroupCnt);
    //update group info
    s32Ret = HI_PDT_FileMng_GetGroupInfo(&s_stCurGroupInfo, g_u32CurGroupIndex);

    if (s32Ret != HI_SUCCESS)
    {
        MLOGE("HI_PDT_FileMng_GetGroupInfo failed\n");
        return HI_FAILURE;
    }

    MLOGD("s_stCurGroupInfo group id:%u \n", s_stCurGroupInfo.stGroupInfo.u32GrpID);
    //update file nums in current group
    s32Ret = HI_PDT_FileMng_GetGroupFileCount(&s_stCurGroupInfo, &s_u32GroupFileCnt);

    if (s32Ret != HI_SUCCESS || 0 == s_u32GroupFileCnt)
    {
        MLOGE("HI_PDT_FileMng_GetGroupFileCount failed, file count:%d \n", s_u32GroupFileCnt );
        return HI_FAILURE;
    }
    s_u32FileIndex = 1;//when change group, always change this to 1 .
    s32Ret = updateFileInfo();
    if (s32Ret != HI_SUCCESS )
    {
        MLOGE("updateFileInfo failed \n ");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}


static HI_S32 changeToNextGroup()
{
    //g_oldu32CurGroupIndex = 1;
    HI_S32 s32Ret = 0;
    //change to next group
    g_u32CurGroupIndex += 1;

    if (g_u32CurGroupIndex > s_u32GroupCnt)
    {
        g_u32CurGroupIndex = 1;
    }
    s32Ret = updateGroupInfo();
    if (s32Ret != HI_SUCCESS )
    {
        MLOGE("updateGroupInfo failed\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static HI_S32 changeToPrevGroup()
{
    //g_oldu32CurGroupIndex = 1;
    HI_S32 s32Ret = 0;
    //change to next group
    g_u32CurGroupIndex -= 1;

    if (g_u32CurGroupIndex < 1)
    {
        g_u32CurGroupIndex = s_u32GroupCnt;
    }
    s32Ret = updateGroupInfo();
    if (s32Ret != HI_SUCCESS )
    {
        MLOGE("updateGroupInfo failed\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static HI_S32 changeToNextFile()
{
    HI_S32 s32Ret = 0;
    //change to next group
    s_u32FileIndex += 1;

    s32Ret = updateFileInfo();
    if (s32Ret != HI_SUCCESS )
    {
        MLOGE("updateFileInfo failed\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static HI_S32 changeToPrevFile()
{
    HI_S32 s32Ret = 0;
    //change to next group
    s_u32FileIndex -= 1;

    s32Ret = updateFileInfo();
    if (s32Ret != HI_SUCCESS )
    {
        MLOGE("updateFileInfo failed\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static HI_S32 sendEventRefreshUI()
{
    HI_GV_Msg_SendAsync(PLAYBACK_PAGE, HIGV_MSG_EVENT, EVENT_REFRESH_UI, HI_TRUE);
    return HI_SUCCESS;
}

#ifdef CFG_HJD
static HI_S32 sendEventTmpRefreshUI()
{
    HI_GV_Msg_SendAsync(PLAYBACK_PAGE, HIGV_MSG_EVENT, EVENT_TMP_REFRESH_UI, HI_TRUE);
    return HI_SUCCESS;
}

HI_S32 handleEventTmpRefreshUI()
{
    HI_S32 s32Ret = 0;
    HI_LITEPLAYER_STATE_E enPlayState = HI_LITEPLAYER_STATE_IDLE;
    HI_CHAR stRecTimeText[32] = {0};
	HI_CHAR stRecTotalTimeText[32] = {0};
	HI_CHAR volText[5] = {0};

	if(g_u32FileType == PLAY_FILE_VIDEO)
	{
	    snprintf(stRecTimeText, 32, "%02d:%02d:%02d",
			UI_MicrosecondToSecond(g_u32PlayTime)/60/60,
	        UI_MicrosecondToSecond(g_u32PlayTime)/60,
	        UI_MicrosecondToSecond(g_u32PlayTime)%60);
		snprintf(stRecTotalTimeText, 32, "%02d:%02d:%02d",
			UI_MicrosecondToSecond(g_u32PlayerFileTime)/60/60,
	        UI_MicrosecondToSecond(g_u32PlayerFileTime)/60,
	        UI_MicrosecondToSecond(g_u32PlayerFileTime)%60);
		HI_GV_Widget_SetText(PLAYBACK_PAGE_LABEL_PLAY_TIME, stRecTimeText);	
		HI_GV_Widget_SetText(PLAYBACK_PAGE_LABEL_TOTAL_TIME, stRecTotalTimeText);
		HI_GV_Widget_Paint(PLAYBACK_PAGE_LABEL_PLAY_TIME, 0);
		HI_GV_Widget_Paint(PLAYBACK_PAGE_LABEL_TOTAL_TIME, 0);
	}

	HI_GV_Widget_Hide(PLAYBACK_PAGE_LABEL_PLAY_FILE_COUNT);
	HI_GV_Widget_Hide(PLAYBACK_PAGE_LABEL_PLAY_FILE_SIZE);

    if( !s_bIsTracking )
	{
	    HI_GV_Track_SetRange(PLAYBACK_PAGE_TRACKBAR_PLAY, 0, g_u32PlayerFileTime);
		HI_GV_Track_SetCurVal(PLAYBACK_PAGE_TRACKBAR_PLAY, g_u32PlayTime);
    }

	HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_PLAY, HIGV_SKIN_NORMAL, n_play_back_play_icon);
	HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_PLAY, HIGV_SKIN_ACITVE, n_play_back_play_icon);

    HI_GV_Widget_Paint(PLAYBACK_PAGE_BUTTON_PLAY, 0);
	HI_GV_Widget_Paint(PLAYBACK_PAGE, 0);

    return HI_SUCCESS;
}
#endif

static HI_S32 PLAYBACK_playFile()
{
    HI_S32 s32Ret = 0;
    HI_LITEPLAYER_STATE_E enPlayState = HI_LITEPLAYER_STATE_IDLE;

    //get the file time info to show
    //if file is video file ,show time and play button, else show nothing
    if ((strstr(s_aszCurFilePath, FILE_SUFFIX_LRV) != HI_NULL) || (strstr(s_aszCurFilePath, FILE_SUFFIX_MP4) != HI_NULL))
    {
		g_u32FileType = PLAY_FILE_VIDEO;
		g_bRecOrSnap = HI_TRUE;
        //MLOGD("file info: %s time:%d\n", s_stCurFileInfo.aszFileName, s_stCurFileInfo.u32Duration_Sec);
    }
    else
    {
		g_u32FileType = PLAY_FILE_IMAGE;
		g_bRecOrSnap = HI_FALSE;
		#ifdef CFG_HJD
		getJpgHeader(s_aszCurFilePath, &g_s32Width, &g_s32Height);
		#endif
    }

    s32Ret = HI_PDT_Playback_GetPlayStatus(&enPlayState);
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("GetPlayState Failed\n");
    }

    if (HI_LITEPLAYER_STATE_PLAY == enPlayState)
    {
        MLOGD("HI_PDT_Playback_Pause\n");
        HI_PDT_Playback_Pause();
		s_isPlayingFlag = HI_FALSE;
    }
    else if (HI_LITEPLAYER_STATE_PAUSE == enPlayState)
    {
        MLOGD("HI_PDT_Playback_Resume\n");
		if(s_bplaypausetask == HI_TRUE)
		{
			s_bplaypause = HI_FALSE;
			s_bIsMute = HI_FALSE;
			s_bplaypausetask = HI_FALSE;
			HI_PDT_Playback_UnMute();
		}
        HI_PDT_Playback_Resume();
		s_isPlayingFlag = HI_TRUE;
    }
    else
    {
        MLOGD("start play");
	    g_u32PlayTime = 0;
   	    HI_CHAR asztmpfilePath[PATH_MAX] = {0};
	    HI_CHAR aszPlayfilePath[PATH_MAX] = {0};
//	    HI_LITEPLAYER_STATE_E enPlayState = HI_LITEPLAYER_STATE_IDLE;

   	    snprintf(asztmpfilePath, PATH_MAX, "%s", s_aszCurFilePath);

	    MLOGD("asztmpfilePath:%s \ng_u32FileType:%d\n",asztmpfilePath, g_u32FileType);
   	    asztmpfilePath[strlen(asztmpfilePath) - 4] = '\0';

	   	if(g_u32FileType == PLAY_FILE_VIDEO)
	   	{
	   		#if 1/*play LRV file */
			    snprintf(aszPlayfilePath, PATH_MAX, "%s.LRV", asztmpfilePath);
			#else/*play THM file */
			    snprintf(aszPlayfilePath, PATH_MAX, "%s.THM", asztmpfilePath);
			#endif
	   	}
		else if(g_u32FileType == PLAY_FILE_IMAGE)
		{
			/*play JPG file */
	    		snprintf(aszPlayfilePath, PATH_MAX, "%s", s_aszCurFilePath);
		}

	    /*Clear VO buf*/
        HI_PDT_Media_ClearVo();

		/*Set Aspect Ratio*/
		HI_PDT_VO_AR_E enAR = HI_PDT_VO_AR_BUTT;
        s32Ret = PDT_Playback_GetFileAspectRatio(s_aszCurFilePath,&enAR);

        printf("enAR :%d\n",enAR);
        if((HI_PDT_VO_AR_4_3 == enAR)||(HI_PDT_VO_AR_16_9 == enAR))
        {
        	HI_PDT_Media_SetVoAspect(enAR);
        }
        else
        {
        	MLOGE("wrong aspect ratio enum value:%d\n",enAR);
            //return HI_FAILURE;
        }

		/*Play file*/
    	HI_PDT_Playback_Play(aszPlayfilePath);

		if(s_bIsMute)
		{
			HI_PDT_Playback_Mute();
		}
		else
		{
			if(g_u32FileType == PLAY_FILE_VIDEO)
			{
				HI_PDT_Playback_SetVolume(g_s32VolumePercent);
			}
		}
		s_isPlayingFlag = HI_TRUE;
    }

	if(s_bplaypause == HI_TRUE)
	{		
		sendEventTmpRefreshUI();			
	}	
	else	
	{	
		sendEventRefreshUI();	
	}

    return HI_SUCCESS;
}

static HI_S32 showPlayerUI()
{
    s_bIsUIShowing = HI_TRUE;
    sendEventRefreshUI();

    //reset hide ui's timer
    HI_GV_Timer_Reset(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
    HI_GV_Timer_Start(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
	return HI_SUCCESS;
}

static HI_S32 showPlayerUI1()
{
    s_bIsUIShowing = (s_bIsUIShowing==HI_TRUE)?HI_FALSE:HI_TRUE;
	if(s_bplaypause == HI_TRUE)	
	{
		sendEventTmpRefreshUI();
	}
	else
	{	
		sendEventRefreshUI();
	}
    //sendEventRefreshUI();

    //reset hide ui's timer
    HI_GV_Timer_Reset(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
    HI_GV_Timer_Start(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
	return HI_SUCCESS;
}


HI_S32 PlayerEventProc(HI_HANDLE hPlayer, HI_LITEPLAYER_EVENT_E enEvent, void* pData)
{
    if (hPlayer == 0)
    {
        MLOGD("the handle is error\n");
        return HI_SUCCESS;
    }

    switch (enEvent)
    {
        case HI_LITEPLAYER_EVENT_STATE_CHANGED:
            MLOGD("HI_LITEPLAYER_EVENT_STATE_CHANGED:%d\n", *(HI_LITEPLAYER_STATE_E*)pData);
			s_bIsTracking = HI_FALSE;
			if(s_bplaypause == HI_TRUE)			
			{		
				sendEventTmpRefreshUI();
			}	
			else
			{
				sendEventRefreshUI();
			}
            break;

        case HI_LITEPLAYER_EVENT_EOF:
            HI_GV_Msg_SendAsync(PLAYBACK_PAGE, HIGV_MSG_EVENT, EVENT_PLAY_END, HI_TRUE);
            break;
        case HI_LITEPLAYER_EVENT_PROGRESS:
			MLOGD("HI_LITEPLAYER_EVENT_PROGRESS:%d\n", *((HI_U32 *)pData));
            g_u32PlayTime = *((HI_U32 *)pData);
			if(s_bplaypause == HI_TRUE)			
			{				
				s_bplaypausetask = HI_TRUE;	
				HI_GV_Msg_SendAsync(PLAYBACK_PAGE_BUTTON_PLAY, HIGV_MSG_GESTURE_TAP, 0, 0);
			}
			else
			{
				sendEventRefreshUI();
			}
            break;
        case HI_LITEPLAYER_EVENT_ERROR:
            HI_GV_Msg_SendAsync(PLAYBACK_PAGE, HIGV_MSG_EVENT, EVENT_PLAY_ERROR, HI_TRUE);
            break;

        case HI_LITEPLAYER_EVENT_SEEK_END:			
			MLOGD("HI_LITEPLAYER_EVENT_SEEK_END:%d\n", *((HI_U32 *)pData));
			if(s_bplaypause == HI_TRUE)			
			{
				HI_GV_Msg_SendAsync(PLAYBACK_PAGE_BUTTON_PLAY, HIGV_MSG_GESTURE_TAP, 0, 0);			
			}
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

HI_S32 handleEventPlayEnd()
{
    HI_PDT_Playback_Seek(0);
    g_u32PlayTime = g_u32PlayerFileTime;
	
    sendEventRefreshUI();

    return HI_SUCCESS;
}

HI_S32 handleEventPlayError()
{
    HI_S32 s32Ret = 0;
    MLOGE("can not play this file:%s\n", s_aszCurFilePath);
    // to do, notify user on UI
    s32Ret = HI_PDT_Playback_Stop();
	if(s32Ret != HI_SUCCESS)
	{
         MLOGE("Stop Liteplayer Error\n");
		 return s32Ret;
	}
	
    g_u32PlayTime = g_u32PlayerFileTime;
	
    sendEventRefreshUI();
    return HI_SUCCESS;
}

HI_S32 handleEventRefreshUI()
{
    HI_S32 s32Ret = 0;
	HI_S32 s32Width, s32Height;
    HI_LITEPLAYER_STATE_E enPlayState = HI_LITEPLAYER_STATE_IDLE;
    HI_CHAR stRecTimeText[32] = {0};
	HI_CHAR stRecTotalTimeText[32] = {0};
	HI_CHAR volText[5] = {0};

//	snprintf(stMakeDate, 32, "%02d/%02d/%02d",
//	   2017,8,26);
//    HI_GV_Widget_SetText(LAYBACK_PAGE_LABEL_MAKE_DATE, stMakeDate);

//	HI_GV_Widget_Paint(LAYBACK_PAGE_LABEL_MAKE_DATE, 0);
	#ifndef WENBIN
	if(g_u32FileType == PLAY_FILE_VIDEO)
	{
	    snprintf(stRecTimeText, 32, "%02d:%02d/%02d:%02d",
	        UI_MicrosecondToSecond(g_u32PlayTime)/60,
	        UI_MicrosecondToSecond(g_u32PlayTime)%60,
	        UI_MicrosecondToSecond(g_u32PlayerFileTime)/60,
	        UI_MicrosecondToSecond(g_u32PlayerFileTime)%60);
        #ifdef WENBIN
        HI_CHAR stRecTimeText1[32] = {0};
        snprintf(stRecTimeText1, 32, "%02d/%02d", s_u32FileIndex, s_u32GroupFileCnt);
        HI_GV_Widget_SetText(PLAYBACK_PAGE_LABEL_PLAY_FILE_COUNT, stRecTimeText1);
        #endif
	}
	else if(g_u32FileType == PLAY_FILE_IMAGE)
	{
	    snprintf(stRecTimeText, 32, "%02d/%02d", s_u32FileIndex, s_u32GroupFileCnt);
        HI_GV_Widget_Hide(PLAYBACK_PAGE_LABEL_PLAY_FILE_COUNT);
	}
    HI_GV_Widget_SetText(PLAYBACK_PAGE_LABEL_PLAY_TIME, stRecTimeText);
	HI_GV_Widget_Paint(PLAYBACK_PAGE_LABEL_PLAY_TIME, 0);
	#else
	if(g_u32FileType == PLAY_FILE_VIDEO)
	{
	    snprintf(stRecTimeText, 32, "%02d:%02d:%02d",
			UI_MicrosecondToSecond(g_u32PlayTime)/60/60,
	        UI_MicrosecondToSecond(g_u32PlayTime)/60,
	        UI_MicrosecondToSecond(g_u32PlayTime)%60);
		snprintf(stRecTotalTimeText, 32, "%02d:%02d:%02d",
			UI_MicrosecondToSecond(g_u32PlayerFileTime)/60/60,
	        UI_MicrosecondToSecond(g_u32PlayerFileTime)/60,
	        UI_MicrosecondToSecond(g_u32PlayerFileTime)%60);
		HI_GV_Widget_SetText(PLAYBACK_PAGE_LABEL_PLAY_TIME, stRecTimeText);	
		HI_GV_Widget_SetText(PLAYBACK_PAGE_LABEL_TOTAL_TIME, stRecTotalTimeText);
		HI_GV_Widget_Paint(PLAYBACK_PAGE_LABEL_PLAY_TIME, 0);
		HI_GV_Widget_Paint(PLAYBACK_PAGE_LABEL_TOTAL_TIME, 0);
	}
	else if(g_u32FileType == PLAY_FILE_IMAGE)
	{
	    snprintf(stRecTimeText, 32, "%03d/%03d", s_u32FileIndex, s_u32GroupFileCnt);
		HI_GV_Widget_SetText(PLAYBACK_PAGE_LABEL_PLAY_FILE_COUNT, stRecTimeText);
		#ifdef CFG_HJD
		if((g_s32Width == 2560)&&(g_s32Height == 1920))
			snprintf(stRecTotalTimeText, 32, "%s", "5M");
		else if((g_s32Width == 3840)&&(g_s32Height == 2160))
			snprintf(stRecTotalTimeText, 32, "%s", "8M");
		else if((g_s32Width == 4000)&&(g_s32Height == 3000))
			snprintf(stRecTotalTimeText, 32, "%s", "12M");	
		#endif
		HI_GV_Widget_SetText(PLAYBACK_PAGE_LABEL_PLAY_FILE_SIZE, stRecTotalTimeText);
	}	
	#endif

	snprintf(volText, 5, "%d%%", g_s32VolumePercent);
	//HI_GV_Widget_SetText(PLAYBACK_PAGE_BUTTON_VOLUME, volText);
//    MLOGE("g_s32VolumePercent=%d\n",g_s32VolumePercent);
    s_bIsMute = (g_s32VolumePercent == 0)?HI_TRUE:HI_FALSE;
	if(g_s32VolumePercent == 0)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_0);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_0);
	}
    else if(g_s32VolumePercent < 10)
    {
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_1);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_1);        
    }
	else if(g_s32VolumePercent < 20)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_2);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_2);
	}
	else if(g_s32VolumePercent < 30)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_3);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_3);
	}
	else if(g_s32VolumePercent < 40)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_4);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_4);
	}
	else if(g_s32VolumePercent < 50)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_5);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_5);
	}
	else if(g_s32VolumePercent < 60)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_6);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_6);
	}
	else if(g_s32VolumePercent < 70)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_7);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_7);
	}
	else if(g_s32VolumePercent < 80)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_8);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_8);
	}
	else if(g_s32VolumePercent < 90)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_9);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_9);
	}
	else if(g_s32VolumePercent < 100)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_10);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_10);
	}
    else if(g_s32VolumePercent == 100)
    {
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, n_icon_volume_11);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_ACITVE, n_icon_volume_11);        
    }
	HI_GV_Widget_Paint(PLAYBACK_PAGE_BUTTON_VOLUME, 0);

	if(!s_bIsUIShowing)
    {
        HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_BACK);
        HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_DEL);
        //HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_PREV);
        HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_PLAY);
        //HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_NEXT);
        HI_GV_Widget_Hide(PLAYBACK_PAGE_TRACKBAR_PLAY);
		HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_VOLUME);
		HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_LOCK);
		HI_GV_Widget_Hide(PLAYBACK_PAGE_GROUPBOX_TOP_BACKGROUND);
		HI_GV_Widget_Hide(PLAYBACK_PAGE_GROUPBOX_BOTTOM_BACKGROUND);
        // Feiyu_ShowAllBottomBarViews(HI_FALSE);
		return HI_SUCCESS;
	}
	HI_GV_Widget_Show(PLAYBACK_PAGE_GROUPBOX_TOP_BACKGROUND);
	HI_GV_Widget_Show(PLAYBACK_PAGE_GROUPBOX_BOTTOM_BACKGROUND);
    // Feiyu_ShowAllBottomBarViews(HI_TRUE);
	HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_BACK);
    HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_DEL);
    //HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_PREV);
    //HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_NEXT);

    if(g_u32FileType == PLAY_FILE_VIDEO)
    {
		HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_PLAY);
		if(s_isPlayingFlag)
		{
            //hide delete button 
            HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_DEL);
            //hide back button
            HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_BACK);
		}
		else
		{
		    //hide TRACKBAR
			//HI_GV_Widget_Hide(PLAYBACK_PAGE_TRACKBAR_PLAY);
            //show date label
			//HI_GV_Widget_Show(LAYBACK_PAGE_LABEL_MAKE_DATE);

            //show delete button
            HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_DEL);
            //show back button
            HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_BACK);

            //HI_GV_Widget_Show(PLAYBACK_PAGE_LABEL_PLAY_FILE_COUNT);
		}
		HI_GV_Widget_Hide(PLAYBACK_PAGE_LABEL_PLAY_FILE_COUNT);
		HI_GV_Widget_Hide(PLAYBACK_PAGE_LABEL_PLAY_FILE_SIZE);
		//show TRACKBAR
		HI_GV_Widget_Show(PLAYBACK_PAGE_TRACKBAR_PLAY);
		//show play time
		HI_GV_Widget_Show(PLAYBACK_PAGE_LABEL_PLAY_TIME);
		//show total time
		HI_GV_Widget_Show(PLAYBACK_PAGE_LABEL_TOTAL_TIME);
		HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_VOLUME);
		HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_LOCK);
    }
	else
	{
        //show delete button and image
        HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_DEL);
        // HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_DEL_IMAGE);
        //show back button and image
        HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_BACK);		
		HI_GV_Widget_Show(PLAYBACK_PAGE_LABEL_PLAY_FILE_COUNT);
		HI_GV_Widget_Show(PLAYBACK_PAGE_LABEL_PLAY_FILE_SIZE);
		HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_PLAY);
		HI_GV_Widget_Hide(PLAYBACK_PAGE_LABEL_PLAY_TIME);
		HI_GV_Widget_Hide(PLAYBACK_PAGE_LABEL_TOTAL_TIME);
        HI_GV_Widget_Hide(PLAYBACK_PAGE_TRACKBAR_PLAY);
		HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_VOLUME);
		HI_GV_Widget_Hide(PLAYBACK_PAGE_BUTTON_LOCK);
        // HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_BACK_IMAGE);
	}
    #if 0
	// if any button active, if no ,active back button
	HI_U32 i=0;
	for(i=0; i<sizeof(s_PlayBackBtnIds)/sizeof(HI_HANDLE); i++)
	{
		if(HI_GV_Widget_IsActive(s_PlayBackBtnIds[i]))
		{
			break;
		}
	}
	if(i>=sizeof(s_PlayBackBtnIds)/sizeof(HI_HANDLE))
	{
		HI_GV_Widget_Active(PLAYBACK_PAGE_BUTTON_BACK);
	}
    #else
    HI_GV_Widget_Active(PLAYBACK_PAGE_BUTTON_BACK);
    #endif

    if( !s_bIsTracking )
	{
		//MLOGD(">> g_u32PlayerFileTime:%d, g_u32PlayTime:%d\n", g_u32PlayerFileTime, g_u32PlayTime);
	    HI_GV_Track_SetRange(PLAYBACK_PAGE_TRACKBAR_PLAY, 0, g_u32PlayerFileTime);
	    HI_GV_Track_SetCurVal(PLAYBACK_PAGE_TRACKBAR_PLAY, g_u32PlayTime);
    }
    s32Ret = HI_PDT_Playback_GetPlayStatus(&enPlayState);
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE(" GetPlayState Failed\n");
    }

	if ((HI_LITEPLAYER_STATE_PLAY == enPlayState)
	    || (HI_LITEPLAYER_STATE_PREPARED== enPlayState))
    {
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_PLAY, HIGV_SKIN_NORMAL, n_play_back_pause_icon);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_PLAY, HIGV_SKIN_ACITVE, n_play_back_pause_icon);
    }
    else
    {
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_PLAY, HIGV_SKIN_NORMAL, n_play_back_play_icon);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_PLAY, HIGV_SKIN_ACITVE, n_play_back_play_icon);
        //hide TRACKBAR
	    //HI_GV_Widget_Hide(PLAYBACK_PAGE_TRACKBAR_PLAY);
        //show date label
        //HI_GV_Widget_Show(LAYBACK_PAGE_LABEL_MAKE_DATE);
        //show delete button
        HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_DEL);
        //show back button
        HI_GV_Widget_Show(PLAYBACK_PAGE_BUTTON_BACK);
    }
	HI_GV_Widget_Paint(PLAYBACK_PAGE_BUTTON_VOLUME, 0);

    HI_GV_Widget_Paint(PLAYBACK_PAGE_BUTTON_PLAY, 0);
	HI_GV_Widget_Paint(PLAYBACK_PAGE, 0);

    return HI_SUCCESS;
}



HI_S32 PLAYBACK_PAGE_onevent(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    switch(wParam)
    {
        case EVENT_PLAY_END:
            handleEventPlayEnd();
            break;
        case EVENT_REFRESH_UI:
            handleEventRefreshUI();
            break;
		#ifdef CFG_HJD
		case EVENT_TMP_REFRESH_UI:
			handleEventTmpRefreshUI();
			break;
		#endif
        case EVENT_PLAY_ERROR:
            handleEventPlayError();
            break;
        case EVENT_FILELIST_PAGE:
        {
            HI_GV_PARSER_LoadViewById(FILELIST_PAGE);
            HI_GV_Widget_Show(FILELIST_PAGE);
            HI_GV_Widget_Hide(PLAYBACK_PAGE);
            break;
        }
        default:
            MLOGE("unhandled event: %u !\n", wParam);
            break;
    }

    return HIGV_PROC_GOON;
}

static void setWorkModeThread(void * param)
{
    printf("setWorkModeThread\n");
    pthread_detach(pthread_self());
    HI_PDT_WORKMODE_S * workMode = (HI_PDT_WORKMODE_S*)param;
    HI_PDT_WorkMode_SetWorkMode(workMode);
    hideHudLoading();
    return;
}

HI_VOID PLAY_BACK_PAGE_exit()
{
    //stop timer
    HI_GV_Timer_Stop(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
    HI_PDT_Playback_Stop();

    HI_S32 s32Ret = HI_SUCCESS;
    HI_LITEPLAYER_STATE_E enPlayState = HI_LITEPLAYER_STATE_IDLE;
    showHudLoading();
    
	pthread_t pid;
    pthread_create(&pid, NULL, (void *)setWorkModeThread, (void *)&g_stSavedWorkMode);
    s32Ret = HI_PDT_Playback_GetPlayStatus(&enPlayState);
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("GetPlayState Failed\n");
    }

    if (HI_LITEPLAYER_STATE_PLAY == enPlayState)
    {
        MLOGD("HI_PDT_Playback_Pause\n");
        HI_PDT_Playback_Pause();
    }
    s32Ret |= HI_GV_Widget_Show(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
    s32Ret |= HI_GV_Widget_Hide(PLAYBACK_PAGE);
    if (HI_SUCCESS!= s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);

    }

}
HI_S32 PLAYBACK_PAGE_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("***,wParam=%d,HIGV_KEY_F2=%d\n",wParam,HIGV_KEY_F2);
    HI_S32 s32Ret = HI_SUCCESS;

    showPlayerUI();

    if (HIGV_KEY_F2 == wParam) //mode key
    {
        s32Ret |= HI_GV_PARSER_LoadViewById(FILELIST_PAGE);
        s32Ret |= HI_GV_Widget_Show(FILELIST_PAGE);
        s32Ret |= HI_GV_Widget_Hide(PLAYBACK_PAGE);
    }
    else if (HIGV_KEY_F1 == wParam) //photo key
    {
        PLAYBACK_playFile();
    }
    else if(HIGV_KEY_F3 == wParam)
    {
        PLAY_BACK_PAGE_exit();
    }
    else
    {
        MLOGE("KEY CODE %#x not processed\n", wParam);
    }


    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }


    return HIGV_PROC_STOP;

}



HI_S32 PLAYBACK_PAGE_BUTTON_BACK_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("***,wParam=%d,HIGV_KEY_F2=%d\n",wParam,HIGV_KEY_F2);
    #if 0
    HI_S32 s32Ret = HI_SUCCESS;

    if (HIGV_KEY_F2 == wParam)
    {
		if(HI_GV_Widget_IsShow(PLAYBACK_PAGE_BUTTON_VOLUME))
		{
        	s32Ret |= ui_active_next_widget(s_PlayBackBtnIds, sizeof(s_PlayBackBtnIds) / sizeof(HI_HANDLE), hWidget);
		}
		else
		{
			//s32Ret |= HI_GV_Widget_Active(PLAYBACK_PAGE_BUTTON_DEL);
		}

    }
    else if (HIGV_KEY_F1 == wParam)
    {
        s32Ret |= HI_GV_PARSER_LoadViewById(FILELIST_PAGE);
        s32Ret |= HI_GV_Widget_Show(FILELIST_PAGE);
        s32Ret |= HI_GV_Widget_Hide(PLAYBACK_PAGE);
    }
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
    #endif

    return HIGV_PROC_GOON;
}



HI_S32 PLAYBACK_PAGE_BUTTON_VOLUME_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	HI_S32 s32Ret = HI_SUCCESS;

	if (HIGV_KEY_F2 == wParam)
	{
		s32Ret |= ui_active_next_widget(s_PlayBackBtnIds, sizeof(s_PlayBackBtnIds) / sizeof(HI_HANDLE), hWidget);
	}
	else if (HIGV_KEY_F1 == wParam)
	{
		if(s_bIsMute)
		{
			//restore current volume
			s32Ret =  HI_PDT_Playback_UnMute();
			if(s32Ret)
			{
				MLOGE("HI_PDT_Playback_UnMute failed \n");
				return HIGV_PROC_GOON;
			}
			s_bIsMute = HI_FALSE;
			//
		}
		else
		{
			//change to mute
			s32Ret =  HI_PDT_Playback_Mute();
			if(s32Ret)
			{
				MLOGE("HI_PDT_Playback_Mute failed \n");
				return HIGV_PROC_GOON;
			}
			s_bIsMute = HI_TRUE;
			//HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_VOLUME, HIGV_SKIN_NORMAL, icon_delete_fo);
			//HI_GV_Widget_SetText(PLAYBACK_PAGE_BUTTON_VOLUME, " ");
		}
		sendEventRefreshUI();
	}

	return HIGV_PROC_GOON;
}

HI_S32 PLAYBACK_PAGE_BUTTON_DEL_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

#ifdef CFG_USB_STORAGE_ON
    HI_USBMNG_STATE_E state;
    HI_USBMNG_GetState(&state);
    if(state == USB_STA_DEVICE_PC_OK)
    {
        MLOGE("USBMNG state: USB_STA_DEVICE_PC_OK, connect to PC, can not start any play task\n");
        s32Ret = HI_FAILURE;
        return s32Ret;
    }
#endif 

    if (HIGV_KEY_F2 == wParam)
    {
        s32Ret |= ui_active_next_widget(s_PlayBackBtnIds, sizeof(s_PlayBackBtnIds) / sizeof(HI_HANDLE), hWidget);
    }
    else if (HIGV_KEY_F1 == wParam)
    {
        s32Ret = HI_PDT_Playback_Stop();
        if(HI_SUCCESS != s32Ret)
        {
            MLOGE("Stop player failed\n");
        }

        if(s_u32GroupFileCnt > 1)
        {
            //delete current file
            s32Ret = HI_PDT_FileMng_DeleteFile(s_aszCurFilePath);
            if(s32Ret)
            {
                MLOGE("HI_PDT_FileMng_DeleteFile fail:%s\n", s_aszCurFilePath);
                return HIGV_PROC_STOP;
            }

            s_u32GroupFileCnt--;
            if(s_u32FileIndex > s_u32GroupFileCnt)
            {
                s_u32FileIndex = s_u32GroupFileCnt;
            }
            s32Ret = updateFileInfo();
            if (s32Ret)
            {
                MLOGE("updateFileInfo fail \n");
                return HIGV_PROC_STOP;
            }
            MLOGD("CUR FILE: %s\n", s_aszCurFilePath);

        }
        else
        {
            //delete current group
            MLOGD("DeleteGroup GroupID:%u\n", s_stCurGroupInfo.stGroupInfo.u32GrpID);
            s32Ret = HI_PDT_FileMng_DeleteGroup(&s_stCurGroupInfo);
            if (s32Ret)
            {
                MLOGE("HI_PDT_FileMng_DeleteGroup \n");
            }

            //update group info
            s32Ret = updateGroupInfo();
            if (s32Ret)
            {
                MLOGE("updateGroupInfo \n");
                if(s_u32GroupCnt == 0)
				{
					MLOGD("There is no more file left \n");
					//HI_PDT_Playback_Stop();
					//UI_AlarmDisplay(ALARM_NO_MORE_FILE);
					s32Ret = HI_GV_Msg_SendAsync(PLAYBACK_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_F3, 0);
					if (s32Ret)
                    {
                        MLOGE("HI_GV_Msg_SendAsync failed %#x\n", s32Ret);
                    }
				}
				return HIGV_PROC_GOON;
            }

        }

        s32Ret = PLAYBACK_playFile();
        if (s32Ret)
        {
            MLOGE("PLAYBACK_playFile failed %#x\n", s32Ret);
        }
    }

    return HIGV_PROC_GOON;
}

HI_S32 Feiyu_DeleteFile()
{
    HI_S32 s32Ret = HI_SUCCESS;
	#ifdef CFG_USB_STORAGE_ON
    HI_USBMNG_STATE_E state;
    HI_USBMNG_GetState(&state);
    if(state == USB_STA_DEVICE_PC_OK)
    {
        MLOGE("USBMNG state: USB_STA_DEVICE_PC_OK, connect to PC, can not start any play task\n");
        s32Ret = HI_FAILURE;
        return s32Ret;
    }
#endif 
    s_isPlayingFlag = HI_FALSE;
    s32Ret = HI_PDT_Playback_Stop();
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("Stop player failed\n");
    }

    if(s_u32GroupFileCnt > 1)
    {
        //delete current file
        s32Ret = HI_PDT_FileMng_DeleteFile(s_aszCurFilePath);
        if(s32Ret) 
        {
            MLOGE("HI_PDT_FileMng_DeleteFile fail:%s\n", s_aszCurFilePath);
            return HIGV_PROC_STOP;
        }

        s_u32GroupFileCnt--;
        if(s_u32FileIndex > s_u32GroupFileCnt)
        {
            s_u32FileIndex = s_u32GroupFileCnt;
        }
        s32Ret = updateFileInfo();
        if (s32Ret)
        {
            MLOGE("updateFileInfo fail \n");
            return HIGV_PROC_STOP;
        }
        MLOGD("CUR FILE: %s\n", s_aszCurFilePath);

    }
    else
    {
        //delete current group
        MLOGD("DeleteGroup GroupID:%u\n", s_stCurGroupInfo.stGroupInfo.u32GrpID);
        s32Ret = HI_PDT_FileMng_DeleteGroup(&s_stCurGroupInfo);
        if (s32Ret)
        {
            MLOGE("HI_PDT_FileMng_DeleteGroup \n");
        }

        //update group info
        s32Ret = updateGroupInfo();
        if (s32Ret)
        {
            MLOGE("updateGroupInfo \n");
            if(s_u32GroupCnt == 0)
            {
                MLOGD("There is no more file left \n");
                //HI_PDT_Playback_Stop();
                //UI_AlarmDisplay(ALARM_NO_MORE_FILE);
                s32Ret = HI_GV_Msg_SendAsync(PLAYBACK_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_F3, 0);
                if (s32Ret)
                {
                    MLOGE("HI_GV_Msg_SendAsync failed %#x\n", s32Ret);
                }
            }
            return HIGV_PROC_GOON;
        }

    }

    s32Ret = PLAYBACK_playFile();
    if (s32Ret)
    {
        MLOGE("PLAYBACK_playFile failed %#x\n", s32Ret);
    }
    return HIGV_PROC_GOON;
}

//delete button gestureTap
HI_S32 PLAYBACK_PAGE_BUTTON_DEL_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    g_oldu32CurGroupIndex = 1;
    s_bIsUIShowing = HI_TRUE;
    return Feiyu_DeleteFile();
}

static HI_S32 LAYBACK_PAGE_CHANGE_PRE()
{
    HI_S32 s32Ret = HI_SUCCESS;

    if((s_u32FileIndex == 1))//current play the first file in group,change to prev group
    {
        changeToPrevGroup();
    }
    else
    {
        changeToPrevFile();
    }
    s32Ret |= HI_PDT_Playback_Stop();
    s32Ret |= PLAYBACK_playFile();

	return s32Ret;
}

static HI_S32 LAYBACK_PAGE_CHANGE_NEXT()
{
    HI_S32 s32Ret = HI_SUCCESS;

	if((s_u32FileIndex >= s_u32GroupFileCnt))//current play the last file in group,change to next group
	{
		changeToNextGroup();
	}
	else
	{
		changeToNextFile();
	}

	s32Ret |= HI_PDT_Playback_Stop();
	s32Ret |= PLAYBACK_playFile();

	return s32Ret;
}

HI_S32 PLAYBACK_PAGE_BUTTON_PREV_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

#ifdef CFG_USB_STORAGE_ON
    HI_USBMNG_STATE_E state;
    HI_USBMNG_GetState(&state);
    if(state == USB_STA_DEVICE_PC_OK)
    {
        MLOGE("USBMNG state: USB_STA_DEVICE_PC_OK, connect to PC, can not start any play task\n");
        s32Ret = HI_FAILURE;
        return s32Ret;
    }
#endif 

    if (HIGV_KEY_F2 == wParam)
    {
		if(HI_GV_Widget_IsShow(PLAYBACK_PAGE_BUTTON_PLAY))
		{
			s32Ret |= ui_active_next_widget(s_PlayBackBtnIds, sizeof(s_PlayBackBtnIds) / sizeof(HI_HANDLE), hWidget);
		}
		else
		{
			//HI_GV_Widget_Active(PLAYBACK_PAGE_BUTTON_NEXT);
		}
    }
    else if (HIGV_KEY_F1 == wParam)
    {
        if((s_u32FileIndex == 1))//current play the first file in group,change to prev group
        {
            changeToPrevGroup();
        }
        else
        {
            changeToPrevFile();
        }
        s32Ret |= HI_PDT_Playback_Stop();
        s32Ret |= PLAYBACK_playFile();

    }

    return HIGV_PROC_GOON;
}

HI_S32 PLAYBACK_PAGE_BUTTON_PLAY_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

#ifdef CFG_USB_STORAGE_ON
    HI_USBMNG_STATE_E state;
    HI_USBMNG_GetState(&state);
    if(state == USB_STA_DEVICE_PC_OK)
    {
        MLOGE("USBMNG state: USB_STA_DEVICE_PC_OK, connect to PC, can not start any play task\n");
        s32Ret = HI_FAILURE;
        return s32Ret;
    }
#endif

    if (HIGV_KEY_F2 == wParam)
    {
        s32Ret |= ui_active_next_widget(s_PlayBackBtnIds, sizeof(s_PlayBackBtnIds) / sizeof(HI_HANDLE), hWidget);
    }
    else if (HIGV_KEY_F1 == wParam)
    {
        PLAYBACK_playFile();
    }

    return HIGV_PROC_GOON;
}

HI_S32 PLAYBACK_PAGE_BUTTON_PLAY_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    s_bIsUIShowing = HI_TRUE;
    printf("PLAYBACK_PAGE_BUTTON_PLAY_ongesturetap\n");
    PLAYBACK_playFile();
    return HIGV_PROC_GOON;
}

HI_S32 PLAYBACK_PAGE_BUTTON_LOCK_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	g_bLockSate = !g_bLockSate;
	MLOGD("g_bLockSate = %s\n", g_bLockSate ? "HI_TRUE" : "HI_FALSE");
	if(g_bLockSate)
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_LOCK, HIGV_SKIN_NORMAL, playback_lock);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_LOCK, HIGV_SKIN_ACITVE, playback_lock);
	}
	else
	{
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_LOCK, HIGV_SKIN_NORMAL, playback_unlock);
		HI_GV_Widget_SetSkin(PLAYBACK_PAGE_BUTTON_LOCK, HIGV_SKIN_ACITVE, playback_unlock);
	}
	HI_GV_Widget_Paint(PLAYBACK_PAGE, 0);
}


#if 0
HI_S32 PLAYBACK_PAGE_BUTTON_NEXT_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

#ifdef CFG_USB_STORAGE_ON
    HI_USBMNG_STATE_E state;
    HI_USBMNG_GetState(&state);
    if(state == USB_STA_DEVICE_PC_OK)
    {
        MLOGE("USBMNG state: USB_STA_DEVICE_PC_OK, connect to PC, can not start any play task\n");
        s32Ret = HI_FAILURE;
        return s32Ret;
    }
#endif 

    if (HIGV_KEY_F2 == wParam)
    {
        s32Ret |= ui_active_next_widget(s_PlayBackBtnIds, sizeof(s_PlayBackBtnIds) / sizeof(HI_HANDLE), hWidget);
    }
    else if (HIGV_KEY_F1 == wParam)
    {
        if((s_u32FileIndex >= s_u32GroupFileCnt))//current play the last file in group,change to next group
        {
            changeToNextGroup();
        }
        else
        {
            changeToNextFile();
        }

        s32Ret |= HI_PDT_Playback_Stop();
        s32Ret |= PLAYBACK_playFile();
    }

    return HIGV_PROC_GOON;
}
#endif


HI_S32 PLAYBACK_PAGE_ongesturefling(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HIGV_GESTURE_EVENT_S gestureEvent;
    HI_S32 startX, startY, endX, endY;

    //HIGV_INPUTEVENT_S stInputEvent;
    
#ifdef CFG_USB_STORAGE_ON
    HI_USBMNG_STATE_E state;
    HI_USBMNG_GetState(&state);
    if(state == USB_STA_DEVICE_PC_OK)
    {
        MLOGE("USBMNG state: USB_STA_DEVICE_PC_OK, connect to PC, can not start any play task\n");
        return HIGV_PROC_GOON;
    }
#endif 

    //memset(&stInputEvent, 0, sizeof(stInputEvent));
    memset(&gestureEvent, 0x0, sizeof(gestureEvent));
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
    //MLOGD("dx:%d, dy:%d", endX-startX, endY-startY);

    if (abs(startX - endX) > abs(startY - endY))// left or right
    {
    	HI_RECT rect;
		memset(&rect, 0x0, sizeof(rect));
    	HI_GV_Widget_GetRect(PLAYBACK_PAGE_TRACKBAR_PLAY,&rect);
		//MLOGD("x :%d   y:%d  w:%d  h:%d\n", rect.x,rect.y,rect.w,rect.h);
		// when is tracking or gesture position locate trackbar area 
    	if(g_u32FileType == PLAY_FILE_VIDEO && (s_bIsTracking || startY >= (rect.y - 5) || endY >= (rect.y - 5)))
		{
			MLOGD("trackbar is tracking ,fling will not jump to next file\n");
		}
        if (startX > endX)// to left
        {
            if(s_bplaypausetask == HI_TRUE)
			{
				s_bplaypause = HI_FALSE;
				s_bIsMute = HI_FALSE;
				s_bplaypausetask = HI_FALSE;
				HI_PDT_Playback_UnMute();
			}
					
            MLOGD("left fling\n");
			//HI_GV_Widget_Active(PLAYBACK_PAGE_BUTTON_NEXT);
            //ui_ongesturetap(PLAYBACK_PAGE_BUTTON_NEXT, 0, 0);
            LAYBACK_PAGE_CHANGE_NEXT();

        }
        else // to right
        {
            if(s_bplaypausetask == HI_TRUE)
			{
				s_bplaypause = HI_FALSE;
				s_bIsMute = HI_FALSE;
				s_bplaypausetask = HI_FALSE;
				HI_PDT_Playback_UnMute();
			}
					
            MLOGD("right fling\n");
			//HI_GV_Widget_Active(PLAYBACK_PAGE_BUTTON_PREV);
            //ui_ongesturetap(PLAYBACK_PAGE_BUTTON_PREV, 0, 0);
            LAYBACK_PAGE_CHANGE_PRE();
        }
    }
    else //up or down
    {
        if (startY > endY)//to up
        {
            //direction 3
            MLOGD("up fling\n");
        }
        else
        {
            //direction 4
            MLOGD("down fling\n");
        }
    }
    return HIGV_PROC_GOON;
}


HI_S32 PLAYBACK_PAGE_trackbar_ontouchaction(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HIGV_TOUCH_EVENT_S * pstTouchEvent = NULL;
    HI_U32 u32CurValue = 0;
    HI_S32 s32Ret = 0;
	HI_LITEPLAYER_STATE_E enPlayState = HI_LITEPLAYER_STATE_IDLE;

    pstTouchEvent = (HIGV_TOUCH_EVENT_S*)lParam;

    if(NULL == pstTouchEvent)
    {
        MLOGE("Null Pointer\n");
        return HIGV_PROC_GOON;
    }

    switch (pstTouchEvent->last.type)
    {
        case HIGV_TOUCH_END:
			if(HI_FALSE == g_bLockSate)
			{
	            if(s_bIsTracking)
	            {
	                s_bIsTracking = HI_FALSE;
	                HI_GV_Track_GetCurVal(PLAYBACK_PAGE_TRACKBAR_PLAY, &u32CurValue);
					g_u32PlayTime = u32CurValue;
					
					s32Ret = HI_PDT_Playback_Seek(u32CurValue);
	                if(s32Ret)
	                {
	                    MLOGE("HI_PDT_Playback_Seek fail \n");
	                }
					
					s32Ret = HI_PDT_Playback_GetPlayStatus(&enPlayState);
				    if(HI_SUCCESS != s32Ret)
				    {
				        MLOGE("GetPlayState Failed\n");
				    }

					if (HI_LITEPLAYER_STATE_PAUSE == enPlayState)
				    {	    
				    	s_bplaypausetask = HI_FALSE;
	  			    	s_bplaypause = HI_TRUE;
						s_bIsMute = HI_TRUE;
						HI_PDT_Playback_Mute();
				    }
					showPlayerUI1();
	            }
			}
            break;

        case HIGV_TOUCH_START:
			if(HI_FALSE == g_bLockSate)
            	s_bIsTracking = HI_TRUE;
            break;

        default:
            break;
    }

    return HIGV_PROC_GOON;
}

HI_S32 PLAYBACK_PAGE_trackbar_onvaluechange(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGD("onvaluechange = %lld\n", (HI_S64)wParam);

    return HIGV_PROC_GOON;
}

HI_S32 PLAYBACK_PAGE_trackbar_onmouseout(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HIGV_TOUCH_EVENT_S touchEvent;
    memset(&touchEvent, 0x0, sizeof(touchEvent));
    touchEvent.last.id = 0;
    touchEvent.last.type = HIGV_TOUCH_END;
    HI_GV_Msg_SendAsyncWithData(PLAYBACK_PAGE_TRACKBAR_PLAY, HIGV_MSG_TOUCH,
            &touchEvent, sizeof(HIGV_TOUCH_EVENT_S));

    return HIGV_PROC_GOON;
}



HI_S32 PLAYBACK_PAGE_ontimer(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    s_bIsUIShowing = HI_FALSE;
	if(s_bplaypause == HI_TRUE)
		sendEventTmpRefreshUI();
    else
    	sendEventRefreshUI();

    HI_GV_Timer_Reset(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
    HI_GV_Timer_Start(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);

    return HIGV_PROC_GOON;
}

HI_S32 PLAYBACK_PAGE_ongesturescroll(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	#if 1
    HIGV_GESTURE_EVENT_S gestureEvent;
    HI_S32 startX, startY, endX, endY;
	HI_U32 u32CurValue = 0;
	HI_S32 s32Ret = 0;
	static HI_U8 u8Count = 0;
	static HI_U8 u8MoveDir = 0;
	HI_LITEPLAYER_STATE_E enPlayState = HI_LITEPLAYER_STATE_IDLE;

	if(g_u32FileType != PLAY_FILE_VIDEO)
	{
		return HIGV_PROC_GOON;//if file is pic, return.
	}

	if(s_bIsTracking)
	{
		MLOGD("tracking, not set volume\n");
		return HIGV_PROC_GOON;
	}

	if(scrollflag == 0)
	{
		u8MoveDir = (startY - endY)? 2:1;
		scrollflag = 1;
	}
    memset(&gestureEvent, 0x0, sizeof(gestureEvent));
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
	#if 1
    if (abs(startX - endX) > abs(startY - endY))// left or right
    {
        if (startX > endX)// to left
        {
            //MLOGD("left scroll\n");
            //ui_ongesturetap(PLAYBACK_PAGE_BUTTON_NEXT, 0, 0);

        }
        else // to right
        {
            //MLOGD("right scroll\n");
            //ui_ongesturetap(PLAYBACK_PAGE_BUTTON_PREV, 0, 0);
        }
    }
    else //up or down
    {
        if (startY > endY)//to up
        {
            //direction 3
            if(u8MoveDir == 2)
            {
            	u8MoveDir = 1;
				u8Count = 0;
            }
            if(u8Count++ > 5)
            {
            	u8Count = 0;
            	g_s32VolumePercent += 9;
            }		
        }
        else
        {
            //direction 4
            //printf("down scroll\n");
            if(u8MoveDir == 1)
            {
            	u8MoveDir = 2;
				u8Count = 0;
            }
            if(u8Count++ > 5)
            {
            	u8Count = 0;
				if(g_s32VolumePercent == 100)
            		g_s32VolumePercent -= 1;
				else
					g_s32VolumePercent -= 9;
            }		
        }
		if(g_s32VolumePercent >= 100)
		{
			g_s32VolumePercent = 100;
		}
		else if(g_s32VolumePercent <= 0)
		{
			g_s32VolumePercent = 0;
		}
		HI_PDT_Playback_SetVolume(g_s32VolumePercent);
		sendEventRefreshUI();
    }
	#endif
	#endif
    return HIGV_PROC_GOON;
}


HI_S32 PLAYBACK_PAGE_ontouchaction(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 point_x, point_y;
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;

    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));

    point_x = touchEvent.last.x;
    point_y = touchEvent.last.y;

    type = touchEvent.last.type;

	if(point_y >= 290)
		goto End;
    switch (type)
    {
        case HIGV_TOUCH_START:
        {
			HI_GV_Timer_Stop(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
            break;
        }

        case HIGV_TOUCH_END:
        {
			scrollflag = 0;
			HI_GV_Timer_Reset(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
			HI_GV_Timer_Start(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
            break;
        }

        default:
        	break;
    }
	
End:
    return HIGV_PROC_GOON;	
}


HI_S32 PLAYBACK_PAGE_onshow(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    static HI_BOOL bInit = HI_FALSE;
    HI_S32 s32Ret = HI_SUCCESS;
	HI_S32 s32Val = -1;

#ifdef MY_TRACK_BAR
	HI_S32 Ret;
	Ret = HI_GV_GraphicContext_Create(PLAYBACK_PAGE_TRACKBAR_PLAY, &s_WinGC); 
	if(HI_SUCCESS == Ret) 
	{ 
		Ret = HI_GV_GraphicContext_DecodeImg("./res/pic/new_fy_pic/my_track_bar.png", 0,0, &s_ImgHandle); 
		if(HI_SUCCESS != Ret) 
		{ 
			MLOGE("HI_GV_GraphicContext_DecodeImg err!\n");
			HI_GV_GraphicContext_Destroy(s_WinGC);
		} 
	}
	else
	{
		MLOGE("HI_GV_GraphicContext_Create err!\n");
		HI_GV_GraphicContext_Destroy(s_WinGC); 
	}
	HI_GV_Widget_SetMsgProc(PLAYBACK_PAGE_TRACKBAR_PLAY, HIGV_MSG_PAINT, TrackBarPaintProc, HIGV_PROCORDER_BEFORE);//HIGV_PROCORDER_AFTER //HIGV_PROCORDER_BEFORE
#endif

    //start a timer to hide the ui
    if (!bInit)
    {
        s32Ret |= HI_GV_Timer_Create(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER, PLAYBACK_HIDE_UI_TIMER_INTERVAL);

        if (HI_SUCCESS == s32Ret)
        {
            bInit = HI_TRUE;
        }
    }

    if (bInit)
    {
        HI_GV_Timer_Start(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
    }

    s_bIsUIShowing = HI_TRUE;

    s32Ret = updateGroupInfo();

    if (s32Ret)
    {
        MLOGE("updateGroupInfo \n");
        return HIGV_PROC_STOP;
    }
	
    s32Ret = PLAYBACK_playFile();
    if (s32Ret)
    {
        MLOGE("PLAYBACK_playFile Failed %#x \n", s32Ret);
        return HIGV_PROC_STOP;
    }

    return HIGV_PROC_GOON;
}

HI_S32 PLAYBACK_PAGE_onhide(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    //stop timer
    g_bRecOrSnap = HI_FALSE;
    HI_GV_Timer_Stop(PLAYBACK_PAGE, PLAYBACK_HIDE_UI_TIMER);
    HI_PDT_Playback_Stop();
    return HIGV_PROC_GOON;
}


HI_S32 PLAYBACK_PAGE_BUTTON_BACK_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    s_bIsUIShowing = HI_TRUE;
    printf("PLAYBACK_PAGE_BUTTON_BACK_ongesturetap\n");
	// HIGV_INPUTEVENT_S stInputEvent;
	if(HI_NULL != strstr(s_stCurFileInfo.aszFileName, "MP4"))
    	g_filelist_flag = SHOW_VIDEO;
	else if(HI_NULL != strstr(s_stCurFileInfo.aszFileName, "JPG"))
		g_filelist_flag = SHOW_PHOTO;
	HI_S32 s32Ret = HI_SUCCESS;
    s32Ret |= HI_GV_PARSER_LoadViewById(FILELIST_PAGE);
    s32Ret |= HI_GV_Widget_Show(FILELIST_PAGE);
    s32Ret |= HI_GV_Widget_Hide(PLAYBACK_PAGE);
	// memset(&stInputEvent, 0, sizeof(stInputEvent));
	// HI_GV_Widget_Active(hWidget);
	// stInputEvent.msg = HIGV_MSG_KEYDOWN;
	// stInputEvent.value = HIGV_KEY_F1;
	// s32Ret = HI_GV_SendInputEvent(&stInputEvent);

	// if (s32Ret)
	// {
    //     MLOGE("Error:%#x\n", s32Ret);
	// }
	return HIGV_PROC_GOON;
}
//addBySpace
HI_S32 PLAYBACK_PAGE_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    printf("PLAYBACK_PAGE_ongesturetap\n");
    HIGV_GESTURE_EVENT_S gestureEvent;
    HI_S32 x, y;
    HI_S32 s32Ret = HI_SUCCESS;
    memset(&gestureEvent, 0x0, sizeof(gestureEvent));
    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
    x = gestureEvent.gesture.tap.pointer.x;
    y = gestureEvent.gesture.tap.pointer.y;
    //MLOGE("X=%d,y=%d\n",x,y);
    HI_LITEPLAYER_STATE_E enPlayState = HI_LITEPLAYER_STATE_IDLE;
    s32Ret = HI_PDT_Playback_GetPlayStatus(&enPlayState);
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("GetPlayState Failed\n");
    }
    if (HI_LITEPLAYER_STATE_PLAY == enPlayState)
    {
        showPlayerUI1();
        return HIGV_PROC_GOON;
    }    
    if(HI_TRUE == s_bIsUIShowing)
    {
        #define LABEL_W 70
        #define LABEL_H 70
        if((x > (480 -LABEL_W)) && (y < LABEL_H))
        {
        	if(s_bplaypausetask == HI_TRUE)
			{
				s_bplaypause = HI_FALSE;
				s_bIsMute = HI_FALSE;
				s_bplaypausetask = HI_FALSE;
				HI_PDT_Playback_UnMute();
			}
			
            //back
            s_bIsUIShowing = HI_TRUE;
            printf("PLAYBACK_PAGE_BUTTON_BACK_ongesturetap\n");
            // HIGV_INPUTEVENT_S stInputEvent;
			if(HI_NULL != strstr(s_stCurFileInfo.aszFileName, "MP4"))
				g_filelist_flag = SHOW_VIDEO;
			else if(HI_NULL != strstr(s_stCurFileInfo.aszFileName, "JPG"))
				g_filelist_flag = SHOW_PHOTO;
            HI_S32 s32Ret = HI_SUCCESS;
            s32Ret |= HI_GV_PARSER_LoadViewById(FILELIST_PAGE);
            s32Ret |= HI_GV_Widget_Show(FILELIST_PAGE);
            s32Ret |= HI_GV_Widget_Hide(PLAYBACK_PAGE);
            return HIGV_PROC_STOP;            
        }
        else if((x < LABEL_W) && (y < LABEL_H))
        {
            if(s_bplaypausetask == HI_TRUE)
			{
				s_bplaypause = HI_FALSE;
				s_bIsMute = HI_FALSE;
				s_bplaypausetask = HI_FALSE;
				HI_PDT_Playback_UnMute();
			}
					
            //delete
            g_oldu32CurGroupIndex = 1;
            s_bIsUIShowing = HI_TRUE;
            Feiyu_DeleteFile();
            return HIGV_PROC_STOP;            
        }

		if(g_u32FileType == PLAY_FILE_VIDEO)
		{
			if((x <= 270) && (x >= 210) && (y >= 130) && (y <= 190))
			{
				s_bIsUIShowing = HI_TRUE;
				HI_GV_Msg_SendAsync(PLAYBACK_PAGE_BUTTON_PLAY, HIGV_MSG_GESTURE_TAP, 0, 0);
				return HIGV_PROC_STOP;
			}
			else if((x <= 70) && (x >= 0) && (y >= 130) && (y <= 190))
			{
				s_bIsUIShowing = HI_TRUE;
				HI_GV_Msg_SendAsync(PLAYBACK_PAGE_BUTTON_LOCK, HIGV_MSG_GESTURE_TAP, 0, 0);
				return HIGV_PROC_STOP;
			}
		}
    }

    showPlayerUI1();
    
    return HIGV_PROC_GOON;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

