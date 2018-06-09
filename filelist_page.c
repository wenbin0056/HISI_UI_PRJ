#include "higv_cextfile.h"
#include "ui_common.h"
#include "hi_product_filemng_ext.h"
#include "hi_product_workmode_ext.h"
#include "add_ddb.h"
#include "feiyu_UICommon_FunTool.h"
#include <unistd.h>
#include "hi_gv_input.h"
#include "fy_user_param.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*  __cplusplus  */

#define Black_color 0xFF000000

extern HI_PDT_WORKMODE_S g_stSavedWorkMode;
extern HI_BOOL g_pageChangeEnd;

HI_U32 g_u32CurGroupIndex = 0; //count from 1
HI_U32 g_oldu32CurGroupIndex = 0; //count from 1
static HI_S32 g_startX = 0;

static HI_BOOL s_bDelete = HI_FALSE;

static HI_BOOL s_bSelectState = HI_FALSE;

static void setWorkModeThread(void * param);
HI_S32 FILELIST_PAGE_exit();

typedef struct hiGROUP_NODE_S
{
    HI_U32 u32GroupIndex;
    struct hiGROUP_NODE_S* pstNext;
} HI_GROUP_NODE_S;

static HI_GROUP_NODE_S* pstListHead = NULL;


static HI_S32 LIST_PrintAll()
{
    HI_GROUP_NODE_S* pstGridNode = pstListHead;
    printf("LIST_PrintAll: ");

    while (NULL != pstGridNode)
    {
        printf("%u ", pstGridNode->u32GroupIndex);
        pstGridNode = pstGridNode->pstNext;
    }

    printf("\n");
    return HI_SUCCESS;
}

/**
 * @brief  add selected grid index to list, order by descend.
 * @return 0 success
 * @return -1 failure
 */
static HI_S32 LIST_AddGroupIndex(HI_U32 u32GroupIndex)
{
    HI_GROUP_NODE_S* pstLeft = NULL;
    HI_GROUP_NODE_S* pstRight = NULL;
    HI_GROUP_NODE_S* pstGridNode = (HI_GROUP_NODE_S*)malloc(sizeof(HI_GROUP_NODE_S));

    if (NULL == pstGridNode)
    {
        MLOGE("malloc failed\n");
        return HI_FAILURE;
    }

    pstGridNode->pstNext = NULL;
    pstGridNode->u32GroupIndex = u32GroupIndex;

    if (NULL == pstListHead)
    {
        pstListHead = pstGridNode;
    }
    else if (pstGridNode->u32GroupIndex > pstListHead->u32GroupIndex)
    {
        pstGridNode->pstNext = pstListHead;
        pstListHead = pstGridNode;
    }
    else
    {
        //order by descend !
        pstLeft = pstListHead;
        pstRight = pstListHead->pstNext;

        while (NULL != pstRight && pstRight->u32GroupIndex > pstGridNode->u32GroupIndex)
        {
            pstLeft = pstRight;
            pstRight = pstRight->pstNext;
        }

        pstGridNode->pstNext = pstRight;
        pstLeft->pstNext = pstGridNode;
    }

    LIST_PrintAll();
    return HI_SUCCESS;
}

static HI_S32 LIST_FindGroupIndex(HI_U32 u32GroupIndex)
{
    HI_GROUP_NODE_S* pstGridNode = pstListHead;

    while (NULL != pstGridNode)
    {
        if (pstGridNode->u32GroupIndex == u32GroupIndex)
        {
            return HI_SUCCESS;
        }

        pstGridNode = pstGridNode->pstNext;
    }

    return HI_FAILURE;
}

static HI_S32 LIST_DeleteGroupIndex(HI_U32 u32GroupIndex)
{
    HI_GROUP_NODE_S* pstLeft = NULL;
    HI_GROUP_NODE_S* pstRight = NULL;

    if (NULL == pstListHead)
    {
        return HI_SUCCESS;
    }

    if (pstListHead->u32GroupIndex == u32GroupIndex)
    {
        pstLeft = pstListHead;
        pstListHead = pstListHead->pstNext;
        free(pstLeft);
        LIST_PrintAll();
        return HI_SUCCESS;
    }

    pstLeft = pstListHead;
    pstRight = pstListHead->pstNext;

    while (NULL != pstRight)
    {
        if (pstRight->u32GroupIndex == u32GroupIndex)
        {
            pstLeft->pstNext = pstRight->pstNext;
            free(pstRight);
            LIST_PrintAll();
            return HI_SUCCESS;
        }

        pstLeft = pstRight;
        pstRight = pstRight->pstNext;
    }

    return HI_SUCCESS;
}

/**
 * @brief delete all list node and the corresponding group.
 * @return 0 success
 * @return -1 failure
 */
static HI_S32 LIST_DeleteNodeAndGroup()
{
    HI_S32 s32Ret = 0;
    HI_PDT_FILEMNG_GROUP_INFO_S stGroupInfo;

    while (NULL != pstListHead)
    {
        s32Ret = HI_PDT_FileMng_GetGroupInfo(&stGroupInfo, pstListHead->u32GroupIndex);

        if (s32Ret == HI_SUCCESS)
        {
            s32Ret = HI_PDT_FileMng_DeleteGroup(&stGroupInfo);
            if(HI_SUCCESS != s32Ret)
            {
                MLOGE("DeleteGroup [%s, %s, %u] failed\n", stGroupInfo.aszDirName,
                    stGroupInfo.stGroupInfo.aszGrpType, stGroupInfo.stGroupInfo.u32GrpID);
            }
        }
        else
        {
            MLOGE("GetGroupInfo [%u] failed\n", pstListHead->u32GroupIndex);
        }

        LIST_DeleteGroupIndex(pstListHead->u32GroupIndex);
    }

    return HI_SUCCESS;
}

static HI_S32 LIST_Clear()
{
    HI_GROUP_NODE_S* pstGridNode = NULL;

    while (NULL != pstListHead)
    {
        pstGridNode = pstListHead;
        pstListHead = pstListHead->pstNext;
        free(pstGridNode);
    }

    pstListHead = NULL;
    return HI_SUCCESS;
}



/**
 * @brief used by datamodel, registerdatachange
 */
HI_S32 SCROLL_GRID_Register(HI_U32 DBSource, HI_HANDLE hADM)
{
    return HI_SUCCESS;
}

/** used by datamodel, unregisterdatachange */
HI_S32 SCROLL_GRID_Unregister(HI_U32 DBSource, HI_HANDLE hADM)
{
    return HI_SUCCESS;
}
/**
 * @brief used by datamodel, registerdatachange
 */
HI_S32 SCROLL_GRID_Register_1(HI_U32 DBSource, HI_HANDLE hADM)
{
    MLOGE("\n");

    return HI_SUCCESS;
}

/** used by datamodel, unregisterdatachange */
HI_S32 SCROLL_GRID_Unregister_1(HI_U32 DBSource, HI_HANDLE hADM)
{
    MLOGE("\n");
    return HI_SUCCESS;
}

#ifdef WENBIN
#define SHOW_PHOTO  1
#define SHOW_VIDEO  2
#define SHOW_ALL_FILE 3
HI_U32 g_filelist_flag = SHOW_VIDEO;//SHOW_VIDEO;//SHOW_PHOTO;
static HI_U32 s_videoFileNum = 0;
static HI_U32 s_photoFileNum = 0;
static HI_U32 s_totalFileNum = 0;


#endif
static HI_VOID refreashLabel()
{
    HI_CHAR buff[28] = {0};
    HI_S32 ret32 = HI_SUCCESS;
    
    if(SHOW_ALL_FILE ==g_filelist_flag)
    {
        snprintf(buff, 28, ":%d >>", s_totalFileNum);
        ret32 |= HI_GV_Widget_SetTextByID(ID_STR_FILELIST_LABEL, ID_STR_FILELIST_SHOW_ALL);
    }
    else if(SHOW_PHOTO ==g_filelist_flag)
    {
        snprintf(buff, 28, ":%d >>", s_photoFileNum);
        ret32 |= HI_GV_Widget_SetTextByID(ID_STR_FILELIST_LABEL, ID_STR_FILELIST_SHOW_PHOTO);
    }
    else if(SHOW_VIDEO ==g_filelist_flag)
    {
        snprintf(buff, 28, ":%d >>", s_videoFileNum);        
        ret32 |= HI_GV_Widget_SetTextByID(ID_STR_FILELIST_LABEL, ID_STR_FILELIST_SHOW_VIDEO);
    }
          
    ret32 |= HI_GV_Widget_SetText(FILELIST_PAGE_FILE_NUM, buff);
    ret32 |= HI_GV_Widget_Paint(FILELIST_PAGE, 0);
    
    if(HI_SUCCESS != ret32)
    {
        MLOGE("ERR\n");
    }
}
/**
 * @brief used  by datamodel, get filemgr's group count. A grid's data come from a row, which corresponds a group.
 */
HI_S32 SCROLL_GRID_GetRowCount(HI_U32 DBSource, HI_U32* RowCnt)
{
    MLOGE("######\n");
    s_videoFileNum = 0;
    s_photoFileNum = 0;
    s_totalFileNum = 0;
    HI_S32 s32Ret = 0;
    HI_U32 u32GroupCount = 0;

    if(HI_NULL == RowCnt)
    {
        MLOGE("Null Pointer\n");
        return HI_FAILURE;
    }

    s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);

    if (s32Ret != HI_SUCCESS)
    {
        *RowCnt = 0;
        MLOGE("HI_PDT_FileMng_GetGroupCount failed\n");
    }
    else
    {
    #ifdef WENBIN
        HI_U32 u32RowNumber = u32GroupCount;
        HI_U32 u32StartRow = 0;
        HI_U32 u32CurRow = 0;
        HI_U32 i = 0;
        HI_PDT_FILEMNG_GROUP_INFO_S stGroupInfo;
        for ( i = 0; i < u32RowNumber; i++)
        {
            u32CurRow = u32StartRow + i + 1;   //FileMng's index begins from 1, not 0
            s32Ret = HI_PDT_FileMng_GetGroupInfo(&stGroupInfo, u32CurRow);

            if (s32Ret != HI_SUCCESS)
            {
                MLOGE("HI_PDT_FileMng_GetGroupInfo failed\n");
                return HI_FAILURE;
            }
            
            if( 0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "normmp4") ||
                0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "slowmp4") ||
                0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "lpsemp4") ||
                0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "loopmp4"))
            {
                s_videoFileNum++;
                if(SHOW_PHOTO == g_filelist_flag)
                    u32GroupCount--;
            }
            else
            {
                s_photoFileNum++;
                if(SHOW_VIDEO == g_filelist_flag)
                    u32GroupCount--;                    
            }
            s_totalFileNum++;
            
        }
       #endif
        *RowCnt = u32GroupCount;
    }

	
    //refreashLabel();
    if(s_bDelete == HI_FALSE)
    	hideHudLoading();

    return HI_SUCCESS;
}

HI_S32 SCROLL_GRID_GetRowCount_1(HI_U32 DBSource, HI_U32* RowCnt)
{
    MLOGE("###\n");
    *RowCnt = 1;

    return HI_SUCCESS;
}


static void * doshowHudLoading(void* pVoid)
{
    pthread_detach(pthread_self());
    showHudLoading();
    HI_GV_Widget_Paint(FILELIST_PAGE, 0);
    usleep(500*1000);
    //HI_GV_ADM_ClearAllData();
    HI_GV_ADM_Sync(datamodel_file_list);

    return NULL;
}



HI_S32 ID_STR_FILELIST_LABEL_ongesturetap(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    pthread_t pid;;
    pthread_create(&pid, NULL, (void *)doshowHudLoading, NULL);

    //showHudLoading();

    if(SHOW_ALL_FILE ==g_filelist_flag)
    {
        g_filelist_flag = SHOW_PHOTO;
    }else if(SHOW_PHOTO ==g_filelist_flag)
    {
        g_filelist_flag = SHOW_VIDEO;
    }else if(SHOW_VIDEO ==g_filelist_flag)
    {
        g_filelist_flag = SHOW_ALL_FILE;
    }

    HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
    HI_GV_ScrollGrid_SetSelCell(FILELIST_PAGE_grid_files, 0);

//    HI_GV_ADM_Sync(datamodel_file_list);

    return HIGV_PROC_GOON;
}
#ifdef WENBIN
HI_U32 calcRealCurRow(HI_U32 u32CurRow)
{
    HI_S32 s32Ret = 0;
    HI_U32 i = 0;
    HI_PDT_FILEMNG_GROUP_INFO_S stGroupInfo;
    HI_U32 u32GroupCount = 0;
    HI_U32 realu32CurRow = u32CurRow;
    HI_S32 s32TempCount = u32CurRow;

    if(SHOW_ALL_FILE == g_filelist_flag)
    {
        return  u32CurRow;
    }
    
    s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);
    for( i = 0; i < u32GroupCount; i++)
    {
        s32Ret = HI_PDT_FileMng_GetGroupInfo(&stGroupInfo, i+1);
        if (s32Ret != HI_SUCCESS)
        {
            MLOGE("HI_PDT_FileMng_GetGroupInfo failed\n");
            return u32CurRow;
        }

        if( 0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "normmp4") ||
            0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "slowmp4") ||
            0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "lpsemp4") ||
            0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "loopmp4"))
        {
            if(SHOW_PHOTO == g_filelist_flag)
            {
                realu32CurRow++;
            }
            else if(SHOW_VIDEO == g_filelist_flag)
            {
                s32TempCount--;
                if(s32TempCount == 0)
                    break;
            }   
        }            
        else
        {
            if(SHOW_VIDEO == g_filelist_flag) 
            {
                realu32CurRow++;
            }
            else if(SHOW_PHOTO == g_filelist_flag)
            {
                s32TempCount--;
                if(s32TempCount == 0)
                    break;                
            }         
        }

    }
    if(realu32CurRow > u32GroupCount)
    {
        //MLOGE("ERR\n");
        realu32CurRow = u32GroupCount;
    }

    return realu32CurRow;

}

#endif
HI_S32 SCROLL_GRID_GetRowValue_1(HI_U32 DBSource, HI_U32 u32StartRow, HI_U32 u32RowNumber, HI_VOID* pData, HI_U32* pu32RowCount)
{
    MLOGE("###\n");
    FileListRow_S stRowData;
    FileListRow_S* pstRow = NULL;
    
    MLOGD("==============================>>>u32StartRow:%u, u32RowNumber:%u\n", u32StartRow, u32RowNumber);    
    *pu32RowCount = 0;
    pstRow = (FileListRow_S*)pData;
    sprintf(stRowData.thumbnail, "%s", "/app/sd/DCIM/100IVCAM/SING0001.JPG");   
    memcpy(pstRow, &stRowData, sizeof(FileListRow_S));
    
    return HI_SUCCESS;
}

/**
 * @brief  get N row's value.  The struct FileListRow_S is consistent with datamodel(datamodel_file_list).
 * @param DBSource DataBase source
 * @param u32StartNow: start from u32StartNow.
 * @param u32RowNumber: hope to get u32RowNumber rows.
 * @param pu32RowCount:  the final gotten row count
 * @return HI_SUCCESS, HI_FAILURE
 */
HI_S32 SCROLL_GRID_GetRowValue(HI_U32 DBSource, HI_U32 u32StartRow, HI_U32 u32RowNumber, HI_VOID* pData, HI_U32* pu32RowCount)
{
    MLOGE("######\n");

    HI_U32 i = 0;
    HI_S32 s32Ret = 0;
    HI_U32 u32CurRow = 0;
    HI_U32 u32GroupCount = 0;
    HI_U32 u32GroupFileCount = 0;
    HI_CHAR aszGroupPath[256] = {0};
    HI_CHAR* pSuffix = NULL;

    HI_FILE_INFO_S stFileInfo;
    HI_PDT_FILEMNG_GROUP_INFO_S stGroupInfo;
    FileListRow_S stRowData;
    FileListRow_S* pstRow = NULL;

    if(HI_NULL == pData || HI_NULL == pu32RowCount)
    {
        MLOGE("Null Pointer\n");
        return HI_FAILURE;
    }

    MLOGD("==============================>>>u32StartRow:%u, u32RowNumber:%u\n", u32StartRow, u32RowNumber);

    *pu32RowCount = 0;
    pstRow = (FileListRow_S*)pData;

    s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);

    if (s32Ret != HI_SUCCESS)
    {
        MLOGE("HI_PDT_FileMng_GetGroupCount failed\n");
        return HI_FAILURE;
    }

    for ( i = 0; i < u32RowNumber; i++)
    {
        u32CurRow = u32StartRow + i + 1;   //FileMng's index begins from 1, not 0

        if (u32CurRow > u32GroupCount)
        {
            MLOGE("RowNumber(%u) > GroupCount(%u)\n", u32CurRow, u32GroupCount);
            return HI_FAILURE;
        }
        
        #ifdef WENBIN
        u32CurRow = calcRealCurRow(u32CurRow);
        #endif
        //MLOGE("u32CurRow=%d\n",u32CurRow);

        s32Ret = HI_PDT_FileMng_GetGroupInfo(&stGroupInfo, u32CurRow);

        if (s32Ret != HI_SUCCESS)
        {
            MLOGE("HI_PDT_FileMng_GetGroupInfo failed\n");
            return HI_FAILURE;
        }

        s32Ret = HI_PDT_FileMng_GetGroupPath(&stGroupInfo, aszGroupPath, sizeof(aszGroupPath));

        if (s32Ret != HI_SUCCESS)
        {
            MLOGE("HI_PDT_FileMng_GetGroupPath failed\n");
            return HI_FAILURE;
        }

        s32Ret = HI_PDT_FileMng_GetGroupFileCount(&stGroupInfo, &u32GroupFileCount);

        if (s32Ret != HI_SUCCESS || 0 == u32GroupFileCount)
        {
            MLOGE("HI_PDT_FileMng_GetGroupFileCount failed\n");
            return HI_FAILURE;
        }

        s32Ret = HI_PDT_FileMng_GetGroupFileInfo(&stGroupInfo, 1, &stFileInfo);

        if (s32Ret != HI_SUCCESS || 0 == u32GroupFileCount)
        {
            MLOGE("HI_PDT_FileMng_GetGroupFileInfo failed\n");
            return HI_FAILURE;
        }

        snprintf(stRowData.thumbnail, 256, "%s/%s", aszGroupPath, stFileInfo.aszFileName);
        MLOGE("stRowData.thumbnail=%s\n",stRowData.thumbnail);
        snprintf(stRowData.grpType, sizeof(stRowData.grpType), "%s", stGroupInfo.stGroupInfo.aszGrpType);
        // MLOGE("u32GroupFileCount==%d\n",u32GroupFileCount);
        // static int notOneCount = 0;
        //There is a reuse system in HI_GV & Database internal, if we don't set " ", it will cause some UI issues!
		
        if(u32GroupFileCount==1)
        {
            snprintf(stRowData.duration, sizeof(stRowData.duration), "%s", " ");
        }
        else
        {
            // MLOGE(">>>>>>stRowData.grpType=%s>>>>>>>>stRowData.duration====%s\n",stRowData.grpType,stRowData.duration);
            snprintf(stRowData.duration, sizeof(stRowData.duration), "%u", u32GroupFileCount);
            // notOneCount++;
            // MLOGE(">>>>>notOneCount%d\n",notOneCount);
        }
    	

        if (HI_SUCCESS == LIST_FindGroupIndex(u32CurRow))
        {
            stRowData.selected = 1;
        }
        else
        {
            stRowData.selected = 0;
        }

        pSuffix = strstr(stRowData.thumbnail, FILE_SUFFIX_MP4);

        if (NULL != pSuffix)
        {
            snprintf(pSuffix, strlen(FILE_SUFFIX_THM)+1, "%s", FILE_SUFFIX_THM);
        }
#ifdef WENBIN
        if( 0 == strcmp(stRowData.grpType, "normmp4") ||
            0 == strcmp(stRowData.grpType, "slowmp4") ||
            0 == strcmp(stRowData.grpType, "lpsemp4") ||
            0 == strcmp(stRowData.grpType, "loopmp4"))
        {
            if(SHOW_PHOTO == g_filelist_flag)
                  continue;		
			snprintf(stRowData.rectime, 16, "%02d:%02d:%02d",
		        stFileInfo.u32Duration_Sec/60/60,
		        stFileInfo.u32Duration_Sec/60,
		        stFileInfo.u32Duration_Sec%60);
        }
        else
        {
            if(SHOW_VIDEO == g_filelist_flag)   
                continue;
			snprintf(stRowData.rectime, 16, "%s", "");
        }
#endif

        memcpy(pstRow, &stRowData, sizeof(FileListRow_S));
//         MLOGD("%s, %s, %s, %u\n", pstRow->thumbnail, pstRow->grpType, pstRow->duration, pstRow->selected);
        pstRow++;
        (*pu32RowCount)++;

    }
    //hideHudLoading();


    return HI_SUCCESS;
}

/**
 * @brief  given data from row value,  output  TYPE image's path.
 * @param u32GridChild  grid's child's index.
 * @param u32Grid  grid index
 * @param pSrcStr   in XML file, coldbindex="1", so we get a row's 1st column value.
 * @param pDstStr  output TYPE image's path
 * @return HI_SUCCESS, HI_FAILURE
 */
HI_S32 SCROLL_GRID_GetTypeIcon(HI_HANDLE hGridHandle, HI_U32 u32GridChild, HI_U32 u32Grid, const HI_CHAR* pSrcStr, HI_CHAR* pDstStr,
                            HI_U32 u32Length)
{

    if(HI_NULL == pSrcStr || HI_NULL == pDstStr)
    {
        MLOGE("Null Pointer\n");
        return HI_FAILURE;
    }
    // MLOGD("Grid:%u, GridChild:%u, scrStr:%s\n", u32Grid, u32GridChild, pSrcStr);
	
	if ((0 == strcasecmp("normmp4", pSrcStr)) || 
		(0 == strcasecmp("slowmp4", pSrcStr)) ||
		(0 == strcasecmp("lpsemp4", pSrcStr)) ||
		(0 == strcasecmp("loopmp4", pSrcStr))
		)
	{
        // >>>>>>>>0pDstStr====/app/sd/DCIM/100HISDV/NORM0310.THM
        // printf(">>>>>>>>0pDstStr====%s\n",pDstStr);
		snprintf(pDstStr, u32Length, "%s", "./res/pic/new_fy_pic/play_icon_small.png");
	}
	else
	{
        
        // >>>>>>>>1pDstStr====
		snprintf(pDstStr, u32Length, "%s", "");
        // printf(">>>>>>>>1pDstStr====%s\n",pDstStr);
	}

#if 0
    if (0 == strcasecmp("normmp4", pSrcStr))
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/playback/type_icon_video_no.png");
    }
    else if ( 0 == strcasecmp("slowmp4", pSrcStr) )
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/playback/type_icon_video_no.png");
    }
    else if ( 0 == strcasecmp("lpsemp4", pSrcStr) )
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/playback/type_icon_video_no.png");
    }
    else if ( 0 == strcasecmp("loopmp4", pSrcStr) )
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/playback/type_icon_video_no.png");
    }
    else if ( 0 == strcasecmp("single", pSrcStr) )
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/playback/type_icon_camera_no.png");
    }
    else if ( 0 == strcasecmp("delay", pSrcStr) )
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/playback/type_icon_delayed_no.png");
    }
    else if ( 0 == strcasecmp("lapse", pSrcStr) )
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/playback/type_icon_fixed_time_no.png");
    }
    else if ( 0 == strcasecmp("burst", pSrcStr) )
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/playback/type_icon_continuous_no.png");
    }
    else if ( 0 == strcasecmp("continu", pSrcStr) )
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/playback/type_icon_continuous_no.png");
    }
    else
    {
        snprintf(pDstStr, u32Length, "%s", "");
    }
#endif
    return HI_SUCCESS;
}

/**
 * @brief  given data from row value,  output  SELECT state image's path.
 * @param u32GridChild  grid's child's index.
 * @param u32Grid  grid index
 * @param pSrcStr   in XML file, coldbindex="3", so we get a row's 3rd column value.
 * @param pDstStr  output TYPE image's path
 * @return HI_SUCCESS, HI_FAILURE
 */
HI_S32 SCROLL_GRID_GetSelectIcon(HI_HANDLE hGridHandle, HI_U32 u32GridChild, HI_U32 u32Grid, const HI_CHAR* pSrcStr, HI_CHAR* pDstStr,
                         HI_U32 u32Length)
{

    if(HI_NULL == pSrcStr || HI_NULL == pDstStr)
    {
        MLOGE("Null Pointer\n");
        return HI_FAILURE;
    }
    if (HI_FALSE == s_bSelectState)
    {
        snprintf(pDstStr, u32Length, "%s", "");
        return HI_SUCCESS;
    }

    if (0 == strcmp("0", pSrcStr))
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/new_fy_pic/icon_selecting.png");
    }
    else
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/new_fy_pic/icon_selected.png");
    }

    return HI_SUCCESS;
}

/**
 * @brief  given data from row value,  output  FOCUS state image's path.
 * @param u32GridChild  grid's child's index.
 * @param u32Grid  grid index
 * @param pSrcStr  we don't use it.
 * @param pDstStr  output FOCUS box image's path
 * @return HI_SUCCESS, HI_FAILURE
 */
/*HI_S32 SCROLL_GRID_GetFocusIcon(HI_HANDLE hGridHandle, HI_U32 u32GridChild, HI_U32 u32Grid, const HI_CHAR* pSrcStr, HI_CHAR* pDstStr,
                         HI_U32 u32Length)
{
    return HI_SUCCESS;
    if (g_u32CurGroupIndex == u32Grid + 1)
    {
        snprintf(pDstStr, u32Length, "%s", "./res/pic/playback/icon_focus_box.png");
    }
    else
    {
        snprintf(pDstStr, u32Length, "%s", "");
    }
}*/

HI_S32 SCROLL_GRID_GetFrameIcon(HI_HANDLE hGridHandle, HI_U32 u32GridChild, HI_U32 u32Grid, const HI_CHAR* pSrcStr, HI_CHAR* pDstStr,
                         HI_U32 u32Length)
{
	if(HI_NULL == pSrcStr || HI_NULL == pDstStr)
    {
        MLOGE("Null Pointer\n");
        return HI_FAILURE;
    }

    snprintf(pDstStr, u32Length, "%s", "./res/pic/new_fy_pic/image_frame.png");

    return HI_SUCCESS;
}


/**
 * @brief  callback on user click a scrollgrid's grid
 */
HI_S32 FILELIST_PAGE_grid_files_onfocusmove(HI_HANDLE hWidget, HI_U32 u32GridIndex, HI_U32 lParam)
{
    MLOGD("GridIndex = %u, lParam = %u\n", u32GridIndex, lParam);

    return HIGV_PROC_GOON;
}

/**
 * @brief  callback on event
 */

HI_S32 FILELIST_PAGE_onevent(HI_HANDLE hWidget,HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(!HI_GV_Widget_IsShow(FILELIST_PAGE))
    {
        return HIGV_PROC_GOON;
    }
    if(PDT_EVT_SD_MOUNT_ON == wParam)
    {   
        s32Ret |= HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
        s32Ret |= HI_GV_ScrollGrid_SetSelCell(FILELIST_PAGE_grid_files, g_u32CurGroupIndex + 1);
        s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE_grid_files);
        HI_GV_ADM_Sync(datamodel_file_list);		
    }
    else if(PDT_EVT_SD_NOEXIST == wParam)
    {
		HI_GV_ADM_Sync(datamodel_file_list);
        s32Ret = FILELIST_PAGE_exit();
        if(HI_SUCCESS != s32Ret)
        {
            MLOGE("Error:%#x\n", s32Ret);
        }
    }
    return HIGV_PROC_GOON;
}
#ifdef WENBIN
static HI_S32 calcRealGourpIndex(HI_U32 u32GridIndex)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32GroupCount = 0;
    HI_U32 u32StartRow = 0;
    HI_U32 u32CurRow = 0;
    HI_U32 i = 0;
    HI_S32 count = u32GridIndex;
    HI_U32 realGroupIndex = u32GridIndex;
    HI_PDT_FILEMNG_GROUP_INFO_S stGroupInfo;

    if(u32GridIndex < 0)
    {
        MLOGE("err\n");
        return 0;
    }
    if(SHOW_ALL_FILE == g_filelist_flag)
    {
        return realGroupIndex;
    }

    s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);
    for ( i = 0; i < u32GroupCount; i++)
    {
        u32CurRow = u32StartRow + i + 1;   //FileMng's index begins from 1, not 0
        s32Ret = HI_PDT_FileMng_GetGroupInfo(&stGroupInfo, u32CurRow);
        if (s32Ret != HI_SUCCESS)
        {
            MLOGE("HI_PDT_FileMng_GetGroupInfo failed\n");
            return HI_FAILURE;
        }
        if( 0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "normmp4") ||
            0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "slowmp4") ||
            0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "lpsemp4") ||
            0 == strcmp(stGroupInfo.stGroupInfo.aszGrpType, "loopmp4"))
        {
            if(SHOW_VIDEO == g_filelist_flag)
            {
                count--;
                if(count < 0)
                    break;
            }
            else if(SHOW_PHOTO == g_filelist_flag)
            {
                realGroupIndex++;
            }
        }
        else
        {
            if(SHOW_VIDEO == g_filelist_flag)
            {
                realGroupIndex++;
            }
            else if(SHOW_PHOTO == g_filelist_flag)
            {
                count--;
                if(count < 0)
                    break;
            }                
        }
        
    }


    return realGroupIndex;
}
#endif
HI_S32 FILELIST_PAGE_grid_files_oncellcolselect_1(HI_HANDLE hWidget, HI_U32 u32GridIndex, HI_U32 u32ChildIndex)
{
    MLOGE("~~\n");
    return HIGV_PROC_GOON;
}

/**
 * @brief  callback on user click a grid's child view
 */
HI_S32 FILELIST_PAGE_grid_files_oncellcolselect(HI_HANDLE hWidget, HI_U32 u32GridIndex, HI_U32 u32ChildIndex)
{
    HI_S32 s32Ret = 0;
    g_oldu32CurGroupIndex = u32GridIndex + 1;
#ifdef WENBIN
    u32GridIndex = calcRealGourpIndex(u32GridIndex);
    //MLOGD("GridIndex = %u, ChildIndex = %u\n", u32GridIndex, u32ChildIndex);
#endif    
    g_u32CurGroupIndex = u32GridIndex + 1;  //in filemng modult, index starts from 1.

    HI_U32 u32GroupCount = 0;
    s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);
    if (s32Ret != HI_SUCCESS)
    {
        u32GroupCount = 0;
        MLOGE("HI_PDT_FileMng_GetGroupCount failed\n");
    }

    if(0 == u32GroupCount || g_u32CurGroupIndex > u32GroupCount)
    {
        return HIGV_PROC_GOON;
    }

    if (s_bSelectState)
    {
        s32Ret = LIST_FindGroupIndex(g_u32CurGroupIndex);

        if (HI_SUCCESS == s32Ret)
        {
            LIST_DeleteGroupIndex(g_u32CurGroupIndex);  //unselect it
        }
        else
        {
            LIST_AddGroupIndex(g_u32CurGroupIndex);  //select it
        }
        HI_GV_ScrollGrid_SetSelCell(FILELIST_PAGE_grid_files, u32GridIndex);
        HI_GV_ADM_Sync(datamodel_file_list);  //update UI
        return HIGV_PROC_GOON;
    }


    //go to playback page
    s32Ret = HI_GV_PARSER_LoadViewById(PLAYBACK_PAGE);
    s32Ret |= HI_GV_Widget_Show(PLAYBACK_PAGE);
    s32Ret |= HI_GV_Widget_Hide(FILELIST_PAGE);
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("HIGV Error: %#x\n", s32Ret);
    }
    return HIGV_PROC_GOON;
}
static HI_BOOL firstTime = HI_TRUE;
 void onShowSetWorkModeThread()
{
    pthread_detach(pthread_self());
    MLOGE("onShowSetWorkModeThread\n");
    HI_S32 s32Ret = 0;
    HI_PDT_WORKMODE_S stWorkMode = {};
    s32Ret = HI_PDT_WorkMode_GetWorkMode(&stWorkMode);
    if (s32Ret)
    {
        MLOGE("HI_PDT_WorkMode_GetWorkMode, s32Ret:%#x\n", s32Ret);
    }
    if(PDT_WORKMODE_PLAYBACK != stWorkMode.enWorkMode)
    {
        stWorkMode.enWorkMode = PDT_WORKMODE_PLAYBACK;
        s32Ret = HI_PDT_WorkMode_SetWorkMode(&stWorkMode);
        if (s32Ret)
        {
            MLOGE("HI_PDT_WorkMode_SetWorkMode, s32Ret:%#x\n", s32Ret);
        }
    }
    if(firstTime == HI_TRUE)
    {
        MLOGE(">>>>>>>>>>>>-----HI_GV_Widget_Paint\n");
        HI_GV_Widget_Paint(FILELIST_PAGE, 0);
        firstTime = HI_FALSE;
    }
}
HI_S32 FILELIST_PAGE_onshow(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32CurGridIndex = 0;
    HI_U32 u32GroupCount = 0;
	HI_U32 pHLHeight, pVLWidth;
	HI_COLOR pHLColor, pVLColor;
	extern USER_PARA_S* g_pstUserPara;

    s_photoFileNum = 0;
    s_videoFileNum = 0;
    s_totalFileNum = 0;

	if(1 == g_pstUserPara->commPara.u8FirstEntryAlbum)
	{
		HI_GV_Widget_Show(filelist_user_guider_groupbox);
		g_pstUserPara->commPara.u8FirstEntryAlbum = 0;
		extern HI_S32 fy_User_SaveParam(HI_U8 enSaveFlag);
	    s32Ret |= fy_User_SaveParam(HI_TRUE);
	    if(HI_SUCCESS != s32Ret)
		{
			MLOGE("err: %d\n", s32Ret);
	    } 
	}
	else
	{
		HI_GV_Widget_Hide(filelist_user_guider_groupbox);
	}

	switch(g_filelist_flag)
	{
		case SHOW_VIDEO:
		{
			HI_GV_Widget_SetSkin(FILELIST_PAGE_label_1,HIGV_SKIN_NORMAL,skin_filelist_blue);
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_2,HIGV_SKIN_NORMAL,background_white);
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_1,HIGV_SKIN_NORMAL,background_skin_blue);    
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_2,HIGV_SKIN_NORMAL,background_skin);    
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_1,HIGV_SKIN_ACITVE,skin_filelist_blue);
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_2,HIGV_SKIN_ACITVE,background_white);
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_1,HIGV_SKIN_ACITVE,background_skin_blue);    
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_2,HIGV_SKIN_ACITVE,background_skin); 
			break;
		}
		case SHOW_PHOTO:
		{
			HI_GV_Widget_SetSkin(FILELIST_PAGE_label_2,HIGV_SKIN_NORMAL,skin_filelist_blue);
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_2,HIGV_SKIN_ACITVE,skin_filelist_blue);    
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_1,HIGV_SKIN_NORMAL,background_white);
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_2,HIGV_SKIN_NORMAL,background_skin_blue);    
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_1,HIGV_SKIN_NORMAL,background_skin);    
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_1,HIGV_SKIN_ACITVE,background_white);
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_2,HIGV_SKIN_ACITVE,background_skin_blue);    
		    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_1,HIGV_SKIN_ACITVE,background_skin);
			break;
		}
	}
  
    HI_GV_Widget_Hide(groupbox_left_button_delete);
    HI_GV_Widget_Hide(groupbox_left_button_cancel);
	HI_GV_ScrollGrid_GetGridding(FILELIST_PAGE_grid_files, &pHLHeight, &pHLColor, &pVLWidth, &pVLColor);
	MLOGD(">>> pHLHeight:%d, pHLColor:%d, pVLWidth:%d, pVLColor:%d\n", pHLHeight, pHLColor, pVLWidth, pVLColor);
	HI_GV_ScrollGrid_SetGridding(FILELIST_PAGE_grid_files, pHLHeight, Black_color, pVLWidth, Black_color);

    HI_PDT_WORKSTATE_S stWorkState = {.enCmd = PDT_CMD_BUTT};
    stWorkState.enEvent = PDT_EVT_FILELIST_ENTRY;
    s32Ret = HI_PDT_EventProc_SetState(stWorkState);
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    
    s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);
    if (s32Ret != HI_SUCCESS)
    {
        u32GroupCount = 0;
        MLOGE("HI_PDT_FileMng_GetGroupCount failed\n");
    }
    
    if((u32GroupCount <= 4)||(g_u32CurGroupIndex == 0)) //if(u32GroupCount <= 4)    
    {
        HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
    }
    
    if(0 == g_u32CurGroupIndex || g_u32CurGroupIndex > 0xFFFFFFF0)
    {
        u32CurGridIndex = 0;
    }
    else
    {
        if(SHOW_ALL_FILE == g_filelist_flag)
        {
            u32CurGridIndex = g_u32CurGroupIndex - 1;
        }
        else
        {
            u32CurGridIndex = g_oldu32CurGroupIndex -1;            
        }

    }
    HI_U32 u32OrginGridIndex = -1;
    HI_GV_ScrollGrid_GetSelCell(FILELIST_PAGE_grid_files,&u32OrginGridIndex);
    if(u32OrginGridIndex != u32CurGridIndex || (SHOW_ALL_FILE != g_filelist_flag))
    {
        HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
        HI_GV_ScrollGrid_SetSelCell(FILELIST_PAGE_grid_files, u32CurGridIndex);
        MLOGD("u32OrginGridIndex =%d,SetSelCell: %u\n",u32OrginGridIndex, u32CurGridIndex);
    }
    
    if(u32CurGridIndex >= 1)
    {
        HI_GV_Widget_Active(FILELIST_PAGE_grid_files);
    }

    HI_GV_ADM_Sync(datamodel_file_list);
	#ifdef WENBIN
    //新开线程处理workMode
    pthread_t pid;;
    pthread_create(&pid, NULL, (void *)onShowSetWorkModeThread, NULL);
	#endif

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

HI_S32 FILELIST_PAGE_exit()
{
    s_photoFileNum = 0;
    s_videoFileNum = 0;
    s_totalFileNum = 0;
    //g_filelist_flag = SHOW_ALL_FILE;

    HI_S32 s32Ret = HI_SUCCESS;

    if (s_bSelectState)
    {
        s_bSelectState = HI_FALSE;
        LIST_Clear();
        HI_GV_ADM_Sync(datamodel_file_list);
        return HI_SUCCESS;
    }

    s_bSelectState = HI_FALSE;
	#ifdef WENBIN
    showHudLoading();
	pthread_t pid;;
    pthread_create(&pid, NULL, (void *)setWorkModeThread, (void *)&g_stSavedWorkMode);
	#endif
	
    s32Ret  = HI_GV_Widget_Show(HOME_PAGE);
    if (s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
        return s32Ret;
    }
    s32Ret = HI_GV_Widget_Active(HOME_PAGE);//s32Ret |= HI_GV_Widget_Active(HOME_PAGE);
    if (s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
        return s32Ret;
    }
    s32Ret = HI_GV_Widget_Hide(FILELIST_PAGE);//s32Ret |= HI_GV_Widget_Hide(FILELIST_PAGE);
    if (s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
        return s32Ret;
    }
    
    g_u32CurGroupIndex = 0;
    
    HI_PDT_WORKSTATE_S stWorkState = {.enCmd = PDT_CMD_BUTT};
    stWorkState.enEvent = PDT_EVT_FILELIST_EXIT;
    s32Ret = HI_PDT_EventProc_SetState(stWorkState);
    if (HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }


    return s32Ret;
}

HI_S32 FILELIST_PAGE_button_back_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

//    s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE_button_back);
//    s32Ret |= HI_GV_Msg_SendAsync(FILELIST_PAGE_button_back, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);

    s32Ret = FILELIST_PAGE_exit();
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_GOON;
}

#if 0
HI_S32 FILELIST_PAGE_button_back_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    printf("FILELIST_PAGE_button_back_onkeydown\n");

    if (HIGV_KEY_F2 == wParam)
    {
        s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE_button_delete);
    }
    else if (HIGV_KEY_F3 == wParam || HIGV_KEY_F1 == wParam)
    {
        s32Ret = FILELIST_PAGE_exit();
    }

    if (s32Ret != HI_SUCCESS)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_GOON;
}
#endif
static HI_HANDLE s_ItemHandle = 0;

static void Item_Update_handle(HIGV_LISTENER_S* listener, void* data)
{
    HI_RECT rect = {0};
    HI_GV_Widget_GetRect(FILELIST_PAGE,&rect);
    HI_GV_Widget_Move(FILELIST_PAGE,HI_GV_TweenAnimGetTweenValue(s_ItemHandle, 0), rect.y);
}

static void Item_Start_handle(HIGV_LISTENER_S* listener, void* data)
{
   // MLOGE(">>\n");
}

static void Item_End_handle(HIGV_LISTENER_S* listener, void* data)
{
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

void FilelistAnimmoveYItem(HIGV_CORD fromValue,HIGV_CORD toValue)
{
    s_ItemHandle = HI_GV_TweenAnimCreate();
    HI_GV_TweenAnimSetDuration(s_ItemHandle,300);

    HI_GV_TweenAnimAddTween(s_ItemHandle,HIGV_TWEEN_TRANSITION_LINEAR,HIGV_TWEEN_EASE_IN,fromValue,toValue);

    HI_GV_TweenAnimSetExecListener(s_ItemHandle, Item_Update_handle);
        
    HI_GV_TweenAnimSetStartedListener(s_ItemHandle, Item_Start_handle);


    HI_GV_TweenAnimSetFinishedListener(s_ItemHandle, Item_End_handle);


    HI_GV_TweenAnimStart(s_ItemHandle);    
}


HI_S32 FILELIST_PAGE_button_delete_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    //s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE_button_delete);
    //s32Ret |= HI_GV_Msg_SendAsync(FILELIST_PAGE_button_delete, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
    
    HI_U32 u32GroupCount = 0;

    if (s_bSelectState)
    {
        s_bSelectState = HI_FALSE;
        LIST_DeleteNodeAndGroup();
        
        s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);
        if (s32Ret != HI_SUCCESS)
        {
            u32GroupCount = 0;
            MLOGE("HI_PDT_FileMng_GetGroupCount failed\n");
        }
        
        if(u32GroupCount <= 4)
        {
            HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
        }
    }
    else
    {
        s_bSelectState = HI_TRUE;
    }

    HI_GV_ADM_Sync(datamodel_file_list);
    
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_GOON;
}

#if 0
HI_S32 FILELIST_PAGE_button_delete_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32GroupCount = 0;

    if (HIGV_KEY_F1 == wParam)
    {
        if (s_bSelectState)
        {
            s_bSelectState = HI_FALSE;
            LIST_DeleteNodeAndGroup();
            
            s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);
            if (s32Ret != HI_SUCCESS)
            {
                u32GroupCount = 0;
                MLOGE("HI_PDT_FileMng_GetGroupCount failed\n");
            }
            
            if(u32GroupCount <= 4)
            {
                HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
            }
        }
        else
        {
            s_bSelectState = HI_TRUE;
        }

        HI_GV_ADM_Sync(datamodel_file_list);
    }
    else if (HIGV_KEY_F2 == wParam)
    {
        s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE_grid_files);
        s32Ret |= HI_GV_Msg_SendAsync(FILELIST_PAGE_grid_files, HIGV_MSG_KEYDOWN, HIGV_KEY_F2, 0);
    }
    else if (HIGV_KEY_F3 == wParam)
    {
        s32Ret = FILELIST_PAGE_exit();
    }

    if (s32Ret != HI_SUCCESS)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_GOON;
}
#endif

#if 0
HI_S32 FILELIST_PAGE_grid_files_onkeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32GroupCount = 0;

    s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);
    if (s32Ret != HI_SUCCESS)
    {
        u32GroupCount = 0;
        MLOGE("HI_PDT_FileMng_GetGroupCount failed\n");
    }

    if (HIGV_KEY_F1 == wParam)
    {
        if(0 == g_u32CurGroupIndex || g_u32CurGroupIndex > u32GroupCount)
        {
            MLOGD("g_u32CurGroupIndex not in range, change to 1\n");
            g_u32CurGroupIndex = 1;
        }
        FILELIST_PAGE_grid_files_oncellcolselect(FILELIST_PAGE_grid_files, g_u32CurGroupIndex-1, 0);
    }
    else if (HIGV_KEY_F2 == wParam)
    {
        if(0 == u32GroupCount || g_u32CurGroupIndex >= u32GroupCount)
        {
            g_u32CurGroupIndex = 0;
            HI_GV_ADM_Sync(datamodel_file_list);
            s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE_button_back);
            return HIGV_PROC_GOON;
        }
        else
        {
            g_u32CurGroupIndex += 1;
            HI_GV_ADM_Sync(datamodel_file_list);

            //this will make higv call xxx_onfocusmove(),  just change focus grid
             //HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
            HI_GV_ScrollGrid_SetSelCell(FILELIST_PAGE_grid_files, g_u32CurGroupIndex-1);
        }
    }
    else if (HIGV_KEY_F3 == wParam)
    {
        s32Ret = FILELIST_PAGE_exit();
    }

    if (s32Ret != HI_SUCCESS)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_GOON;
}
#endif

HI_S32 FILELIST_PAGE_onKeydown(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{

    if (HIGV_KEY_F2 == wParam) //mode key
    {
        FILELIST_PAGE_exit();
    }
    else if (HIGV_KEY_F1 == wParam) //photo key
    {
    	HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
        FILELIST_PAGE_exit();
    }
    else
    {
        MLOGE("KEY CODE %#x not processed\n", wParam);
    }

    return HIGV_PROC_GOON;
}

HI_S32 FILELIST_PAGE_ontouchaction(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 point_x, point_y;
    HIGV_TOUCH_EVENT_S touchEvent;
    HIGV_TOUCH_E type;

    memset(&touchEvent, 0x0, sizeof(touchEvent));
    memcpy(&touchEvent, (HIGV_TOUCH_EVENT_S*)lParam, sizeof(touchEvent));

    point_x = touchEvent.last.x;
    point_y = touchEvent.last.y;

    type = touchEvent.last.type;

    switch (type)
    {
        case HIGV_TOUCH_START:
        {
			MLOGD(">>>>>>>>>> HIGV_TOUCH_START\n");
			g_startX = point_x;
			if(HI_GV_Widget_IsShow(filelist_user_guider_groupbox))
			{
				HI_GV_Widget_Hide(filelist_user_guider_groupbox);
			}
            break;
        }

        case HIGV_TOUCH_END:
        {
			MLOGD(">>>>>>>>>> HIGV_TOUCH_END\n");
            break;
        }

        default:
        	break;
    }
	
    return HIGV_PROC_GOON;
}

HI_S32 FILELIST_PAGE_ongesturescroll(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;
	HI_BOOL dMoveFlag = HI_FALSE;

    MLOGD("\n");

    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetState failed\n");
        return HIGV_PROC_STOP;
    }


    if (PDT_WORKMODE_STARTED == HI_PDT_WorkMode_GetState())
    {
        MLOGD("recording or snapping, ignore gesture fling\n");
        return HIGV_PROC_STOP;
    }

    memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);

    startX = gestureEvent.gesture.fling.start.x;
    startY = gestureEvent.gesture.fling.start.y;
    endX = gestureEvent.gesture.fling.end.x;
    endY = gestureEvent.gesture.fling.end.y;
    MLOGD("dx:%d, dy:%d\n", endX - startX, endY - startY);
	dMoveFlag = (g_startX - endX) > 80 ? HI_TRUE : HI_FALSE;
    if (abs(startX - endX) > abs(startY - endY))// left or right
    {
        if ((startX > endX)&&(dMoveFlag == HI_TRUE))// to left
        {
        	if(g_pageChangeEnd == HI_TRUE)
        	{
        		g_pageChangeEnd = HI_FALSE;
	            s32Ret = FILELIST_PAGE_exit();
	            if(HI_SUCCESS != s32Ret)
	            {
	                MLOGE("Error:%#x\n", s32Ret);
	            }        
	            HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
        	}
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

        }
    }

    return HIGV_PROC_GOON;
}

HI_S32 FILELIST_PAGE_ongesturefling(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{

    HIGV_GESTURE_EVENT_S gestureEvent = {};
    HI_S32 startX, startY, endX, endY;
    HI_S32 s32Ret = HI_SUCCESS;

    MLOGD("\n");

    if (HI_SUCCESS != s32Ret )
    {
        MLOGE("GetState failed\n");
        return HIGV_PROC_STOP;
    }


    if (PDT_WORKMODE_STARTED == HI_PDT_WorkMode_GetState())
    {
        MLOGD("recording or snapping, ignore gesture fling\n");
        return HIGV_PROC_STOP;
    }

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
        	if(g_pageChangeEnd == HI_TRUE)
        	{
        		g_pageChangeEnd = HI_FALSE;
	            s32Ret = FILELIST_PAGE_exit();
	            if(HI_SUCCESS != s32Ret)
	            {
	                MLOGE("Error:%#x\n", s32Ret);
	            }        
	            HI_GV_Msg_SendAsync(HOME_PAGE, HIGV_MSG_EVENT, UI_EVENT_JUMP_HOME, 0);
			}
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

        }
    }

    return HIGV_PROC_GOON;
}


HI_S32 FILELIST_PAGE_ongesturelongtap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    //s32Ret |= HI_GV_Widget_Active(FILELIST_PAGE_button_delete);
    //s32Ret |= HI_GV_Msg_SendAsync(FILELIST_PAGE_button_delete, HIGV_MSG_KEYDOWN, HIGV_KEY_F1, 0);
    
    HI_U32 u32GroupCount = 0;


    if (s_bSelectState)
    {
        s_bSelectState = HI_FALSE;
        s_bDelete = HI_FALSE;
		
        s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);
        if (s32Ret != HI_SUCCESS)
        {
            u32GroupCount = 0;
            MLOGE("HI_PDT_FileMng_GetGroupCount failed\n");
        }
        
        if(u32GroupCount <= 4)
        {
            HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
        }
        HI_GV_Widget_Hide(groupbox_left_button_delete);
        HI_GV_Widget_Hide(groupbox_left_button_cancel);          
    }
    else
    {
        s_bSelectState = HI_TRUE;
        HI_GV_Widget_Show(groupbox_left_button_delete);
        HI_GV_Widget_Show(groupbox_left_button_cancel);        
    }
    HI_GV_Widget_Paint(FILELIST_PAGE, 0);

    HI_GV_ADM_Sync(datamodel_file_list);
    
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_GOON;
}


HI_S32 FILELIST_PAGE_button_1_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("#######\n");
    
    g_filelist_flag = SHOW_PHOTO;
    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_2,HIGV_SKIN_NORMAL,skin_filelist_blue);
    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_2,HIGV_SKIN_ACITVE,skin_filelist_blue);    
    
    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_1,HIGV_SKIN_NORMAL,background_white);
    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_2,HIGV_SKIN_NORMAL,background_skin_blue);    
    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_1,HIGV_SKIN_NORMAL,background_skin);    
    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_1,HIGV_SKIN_ACITVE,background_white);
    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_2,HIGV_SKIN_ACITVE,background_skin_blue);    
    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_1,HIGV_SKIN_ACITVE,background_skin);  
    HI_GV_Widget_Paint(FILELIST_PAGE, 0);
    
    HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
    HI_GV_ScrollGrid_SetSelCell(FILELIST_PAGE_grid_files, 0);
    HI_GV_ADM_Sync(datamodel_file_list);


	return HIGV_PROC_GOON;  
}

HI_S32 FILELIST_PAGE_button_2_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	MLOGE("#######\n");

	g_filelist_flag = SHOW_VIDEO;
    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_1,HIGV_SKIN_NORMAL,skin_filelist_blue);
    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_2,HIGV_SKIN_NORMAL,background_white);
    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_1,HIGV_SKIN_NORMAL,background_skin_blue);    
    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_2,HIGV_SKIN_NORMAL,background_skin);    
    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_1,HIGV_SKIN_ACITVE,skin_filelist_blue);
    HI_GV_Widget_SetSkin(FILELIST_PAGE_label_2,HIGV_SKIN_ACITVE,background_white);
    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_1,HIGV_SKIN_ACITVE,background_skin_blue);    
    HI_GV_Widget_SetSkin(FILELIST_PAGE_button_2,HIGV_SKIN_ACITVE,background_skin);      
    HI_GV_Widget_Paint(FILELIST_PAGE, 0);

    HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
    HI_GV_ScrollGrid_SetSelCell(FILELIST_PAGE_grid_files, 0);
    HI_GV_ADM_Sync(datamodel_file_list);


    return HIGV_PROC_GOON;    
}

HI_S32 FILELIST_PAGE_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("============\n");
	HIGV_GESTURE_EVENT_S gestureEvent = {};
	HI_S32 startX, startY;
	HIGV_INPUTEVENT_S stInputEvent;
	HI_S32 s32Ret = HI_SUCCESS;
	HI_U32 u32GroupCount = 0;
	
	memset(&stInputEvent, 0, sizeof(stInputEvent));
	memcpy(&gestureEvent, (HIGV_GESTURE_EVENT_S*)lParam, wParam);
	startX = gestureEvent.gesture.fling.start.x;
	startY = gestureEvent.gesture.fling.start.y;


	if(HI_TRUE == s_bSelectState)
    {
        if((startX > 440)&&(startX <= 480)&&(startY > 216)&&(startY <= 320))
		{
			HI_GV_Msg_SendAsync(groupbox_left_button_cancel, HIGV_MSG_GESTURE_TAP, 0, 0);
            return HIGV_PROC_STOP;
		}  
		else if((startX > 440)&&(startX <= 480)&&(startY >= 0)&&(startY < 106))
		{
			HI_GV_Msg_SendAsync(groupbox_left_button_delete, HIGV_MSG_GESTURE_TAP, 0, 0);
            return HIGV_PROC_STOP;            
		}

    }
	if((startX > 40)&&(startX < 210)&&(startY >= 0)&&(startY <= 70))
	{
		HI_GV_Msg_SendAsync(FILELIST_PAGE_button_2, HIGV_MSG_GESTURE_TAP, 0, 0);
        return HIGV_PROC_STOP;        
	}
	else if((startX > 230)&&(startX < 400)&&(startY >= 0)&&(startY <= 70))
	{
		HI_GV_Msg_SendAsync(FILELIST_PAGE_button_1, HIGV_MSG_GESTURE_TAP, 0, 0);
        return HIGV_PROC_STOP;
	}

	return HIGV_PROC_GOON;
}

HI_S32 groupbox_left_button_delete_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
    MLOGE("#######\n");
    HI_S32 s32Ret = HI_SUCCESS;
    
    HI_U32 u32GroupCount = 0;

    if (s_bSelectState)
    {
		s_bDelete = HI_TRUE;
        LIST_DeleteNodeAndGroup();
        
        s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);
        if (s32Ret != HI_SUCCESS)
        {
            u32GroupCount = 0;
            MLOGE("HI_PDT_FileMng_GetGroupCount failed\n");
        }
        
        if(u32GroupCount <= 4)
        {
            HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
        }
    }
    else
    {
        s_bSelectState = HI_TRUE;
    }

    HI_GV_ADM_Sync(datamodel_file_list);
    
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }

    return HIGV_PROC_GOON; 
}

HI_S32 groupbox_left_button_cancel_ongesturetap(HI_HANDLE hWidget, HI_U32 wParam, HI_U32 lParam)
{
	HI_S32 s32Ret = HI_SUCCESS;
	HI_U32 u32GroupCount = 0;

	if (s_bSelectState)
    {
        s_bSelectState = HI_FALSE;
		s_bDelete = HI_FALSE;
        
        s32Ret = HI_PDT_FileMng_GetGroupCount( &u32GroupCount);
        if (s32Ret != HI_SUCCESS)
        {
            u32GroupCount = 0;
            MLOGE("HI_PDT_FileMng_GetGroupCount failed\n");
        }
        
        if(u32GroupCount <= 4)
        {
            HI_GV_ScrollGrid_MoveToOrigin(FILELIST_PAGE_grid_files);
        }
        HI_GV_Widget_Hide(groupbox_left_button_delete);
        HI_GV_Widget_Hide(groupbox_left_button_cancel);          
    }
    else
    {
        s_bSelectState = HI_TRUE;
        HI_GV_Widget_Show(groupbox_left_button_delete);
        HI_GV_Widget_Show(groupbox_left_button_cancel);        
    }
    HI_GV_Widget_Paint(FILELIST_PAGE, 0);
    HI_GV_ADM_Sync(datamodel_file_list);
    
    if(HI_SUCCESS != s32Ret)
    {
        MLOGE("Error:%#x\n", s32Ret);
    }
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

