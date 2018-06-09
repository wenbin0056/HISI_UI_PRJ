#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hi_gv_animation.h"
#include "hi_gv_widget.h"
#include "higv_cextfile.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*  __cplusplus  */

#define MoveXFlag (1)
#define MoveYFlag (2)
#define MoveXYFlag (3)

//最大的动画数量
#define MaxAnimations (5)
//最大的Wideget
#define MaxWidget (5)
#define InvaildUIID (-1)
#define InvaildAnimationHandle (-1)

//X轴移动的动画句柄数字
static HI_HANDLE MoveAnimationXHandle[MaxAnimations] = {-1,-1,-1,-1,-1};

//当前X轴移动的动画句柄
static HI_HANDLE currentXHandleIndex = -1;

//Y轴移动的动画句柄数组
static HI_HANDLE MoveAnimationYHandle[MaxAnimations] = {-1,-1,-1,-1,-1};

//当前Y轴移动的动画句柄
static HI_HANDLE currentYHandleIndex = -1;

//XY轴同时移动的动画句柄数组
static HI_HANDLE MoveAnimationXYHandle[MaxAnimations] = {-1,-1,-1,-1,-1};

//当前XY轴同时移动的动画句柄
static HI_HANDLE currentXYHandleIndex = -1;

//记录X轴移动的widgets
static HI_HANDLE MoveXWidgts[MaxWidget] = {-1,-1,-1,-1,-1};

//记录X轴移动的widgets
static HI_HANDLE MoveYWidgts[MaxWidget] = {-1,-1,-1,-1,-1};

//记录X轴移动的widgets
static HI_HANDLE MoveXYWidgts[MaxWidget] = {-1,-1,-1,-1,-1};

static HI_S32 addMoveAnimation(HI_S32 moveFlag,HI_HANDLE hWidgt, HIGV_CORD beginX, HIGV_CORD endX,HI_U32 duration,HIGV_TWEEN_TRANSITION_E animType);
static void animMoveX_update_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveX_start_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveX_end_handle(HIGV_LISTENER_S* listener, void* data);

static void animMoveY_update_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_start_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveY_end_handle(HIGV_LISTENER_S* listener, void* data);

static void animMoveXY_update_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveXY_start_handle(HIGV_LISTENER_S* listener, void* data);
static void animMoveXY_end_handle(HIGV_LISTENER_S* listener, void* data);

static HI_BOOL isActing = HI_FALSE;

//添加X轴移动的动画
HI_S32 addMoveXAnimation(HI_HANDLE hWidgt, HIGV_CORD beginX, HIGV_CORD endX,HI_U32 duration,HIGV_TWEEN_TRANSITION_E animType)
{
    if(isActing)return HIGV_PROC_GOON;
    int i;
    //找到数组中为-1的widget
    for(i = 0;i<5;i++)
    {
        HI_HANDLE MovehWidgt = MoveXWidgts[i];
        if(MovehWidgt ==-1)break;
    }
    MoveXWidgts[i] = hWidgt;
    currentXHandleIndex = i;
    addMoveAnimation(MoveXFlag,hWidgt,beginX,endX,duration,animType);
    return HIGV_PROC_GOON;
}


//添加Y轴移动的动画
HI_S32 addMoveYAnimation(HI_HANDLE hWidgt, HIGV_CORD beginY, HIGV_CORD endY,HI_U32 duration,HIGV_TWEEN_TRANSITION_E animType)
{
    if(isActing)return HIGV_PROC_GOON;
    int i;
    for(i = 0;i<5;i++)
    {
        HI_HANDLE MovehWidgt = MoveYWidgts[i];
        if(MovehWidgt ==-1)break;
    }
    MoveYWidgts[i] = hWidgt;
    currentYHandleIndex = i;
    addMoveAnimation(MoveYFlag,hWidgt,beginY,endY,duration,animType);
    return HIGV_PROC_GOON;
}


//添加XY轴移动的动画
HI_S32 addMoveXYAnimation(HI_HANDLE hWidgt, HIGV_CORD beginXY, HIGV_CORD endXY,HI_U32 duration,HIGV_TWEEN_TRANSITION_E animType)
{
    if(isActing)return HIGV_PROC_GOON;
    int i;
    for(i = 0;i<5;i++)
    {
        HI_HANDLE MovehWidgt = MoveXYWidgts[i];
        if(MovehWidgt ==-1)break;
    }
    MoveXYWidgts[i] = hWidgt;
    currentXYHandleIndex = i;
    addMoveAnimation(MoveXYFlag,hWidgt,beginXY,endXY,duration,animType);
    return HIGV_PROC_GOON;
}

static HI_S32 addMoveAnimation(HI_S32 moveFlag,HI_HANDLE hWidgt, HIGV_CORD fromValue, HIGV_CORD toValue,HI_U32 duration,HIGV_TWEEN_TRANSITION_E animType)
{
    printf("add Animation\n");
    HI_S32 s32Ret = HI_SUCCESS;
    if(HI_SUCCESS != s32Ret)
    {
        printf("could not get view frame viewID=%d\n",hWidgt);
        return HI_FAILURE;
    }
    if(moveFlag == MoveXFlag)
    {
        MoveAnimationXHandle[currentXHandleIndex] = HI_GV_TweenAnimCreate();
        HI_GV_TweenAnimSetDuration(MoveAnimationXHandle[currentXHandleIndex], duration);

        //设置动画属性值得范围
        HI_GV_TweenAnimAddTween(MoveAnimationXHandle[currentXHandleIndex], animType, HIGV_TWEEN_EASE_OUT,fromValue,toValue);

        //添加动画回调函数
        HI_GV_TweenAnimSetExecListener(MoveAnimationXHandle[currentXHandleIndex], animMoveX_update_handle);
        
        //添加动画回调函数
        HI_GV_TweenAnimSetStartedListener(MoveAnimationXHandle[currentXHandleIndex], animMoveX_start_handle);

        //添加动画回调函数
        HI_GV_TweenAnimSetFinishedListener(MoveAnimationXHandle[currentXHandleIndex], animMoveX_end_handle);
        
        //开始动画
        HI_GV_TweenAnimStart(MoveAnimationXHandle[currentXHandleIndex]);
    }
    else if(moveFlag == MoveYFlag)
    {
        MoveAnimationYHandle[currentYHandleIndex] = HI_GV_TweenAnimCreate();
        HI_GV_TweenAnimSetDuration(MoveAnimationYHandle[currentYHandleIndex], duration);

        //设置动画属性值得范围
        HI_GV_TweenAnimAddTween(MoveAnimationYHandle[currentYHandleIndex], animType, HIGV_TWEEN_EASE_OUT,fromValue,toValue);

        //添加动画回调函数
        HI_GV_TweenAnimSetExecListener(MoveAnimationYHandle[currentYHandleIndex], animMoveY_update_handle);
        
        //添加动画回调函数
        HI_GV_TweenAnimSetStartedListener(MoveAnimationYHandle[currentYHandleIndex], animMoveY_start_handle);

        //添加动画回调函数
        HI_GV_TweenAnimSetFinishedListener(MoveAnimationYHandle[currentYHandleIndex], animMoveY_end_handle);
        
        //开始动画
        HI_GV_TweenAnimStart(MoveAnimationYHandle[currentYHandleIndex]);
    }
    else
    {
        MoveAnimationXYHandle[currentXYHandleIndex] = HI_GV_TweenAnimCreate();
        HI_GV_TweenAnimSetDuration(MoveAnimationXYHandle[currentXYHandleIndex], duration);

        //设置动画属性值得范围
        HI_GV_TweenAnimAddTween(MoveAnimationXYHandle[currentXYHandleIndex], animType, HIGV_TWEEN_EASE_OUT,fromValue,toValue);

        //添加动画回调函数
        HI_GV_TweenAnimSetExecListener(MoveAnimationXYHandle[currentXYHandleIndex], animMoveXY_update_handle);
        
        //添加动画回调函数
        HI_GV_TweenAnimSetStartedListener(MoveAnimationXYHandle[currentXYHandleIndex], animMoveXY_start_handle);

        //添加动画回调函数
        HI_GV_TweenAnimSetFinishedListener(MoveAnimationXYHandle[currentXYHandleIndex], animMoveXY_end_handle);
        
        //开始动画
        HI_GV_TweenAnimStart(MoveAnimationXYHandle[currentXYHandleIndex]);
    }
    
    return HIGV_PROC_GOON;
}

static void animMoveX_update_handle(HIGV_LISTENER_S* listener, void* data)
{
    int i;
    for(i = 0;i<MaxWidget;i++)
    {
        HI_HANDLE MovehWidgt = MoveXWidgts[i];
        if(MovehWidgt != InvaildUIID)
        {
            HI_RECT rect = {0};
            HI_GV_Widget_GetRect(MovehWidgt,&rect);
            HI_GV_Widget_Move(MovehWidgt,HI_GV_TweenAnimGetTweenValue(MoveAnimationXHandle[i], 0),rect.y);
        }
    }
    //刷新window
    HI_GV_Widget_Update(HOME_PAGE, HI_NULL);
}

static void animMoveX_start_handle(HIGV_LISTENER_S* listener, void* data)
{
    isActing = HI_TRUE;
    printf(">>===animX_start_handle\n");
}

static void animMoveX_end_handle(HIGV_LISTENER_S* listener, void* data)
{
    printf(">>===animX_end_handle\n");
    int i;
    for(i = 0;i <MaxWidget;i++)
    {
        MoveXWidgts[i] = InvaildUIID;
    }
    isActing = HI_FALSE;
}

static void animMoveY_update_handle(HIGV_LISTENER_S* listener, void* data)
{
    int i;
    for(i = 0; i<MaxWidget ;i++)
    {
        HI_HANDLE MovehWidgt = MoveYWidgts[i];
        if(MovehWidgt != InvaildUIID)
        {
            HI_RECT rect = {0};
            HI_GV_Widget_GetRect(MovehWidgt,&rect);
            HI_GV_Widget_Move(MovehWidgt,rect.x,HI_GV_TweenAnimGetTweenValue(MoveAnimationYHandle[i], 0));
        }
    }
    //刷新window
    HI_GV_Widget_Update(HOME_PAGE, HI_NULL);
}

static void animMoveY_start_handle(HIGV_LISTENER_S* listener, void* data)
{
    printf(">>===animY_start_handle\n");
    isActing = HI_TRUE;
    
}

static void animMoveY_end_handle(HIGV_LISTENER_S* listener, void* data)
{
    printf(">>===animY_end_handle-----%d\n",*(int*)data);
    int i;
    for(i = 0;i <MaxWidget;i++)
    {
        MoveYWidgts[i] = InvaildUIID;
    }
    isActing = HI_FALSE;
}

static void animMoveXY_update_handle(HIGV_LISTENER_S* listener, void* data)
{
    int i;
    for(i = 0;i<MaxWidget;i++)
    {
        HI_HANDLE MovehWidgt = MoveXYWidgts[i];
        if(MovehWidgt != InvaildUIID)
            HI_GV_Widget_Move(MovehWidgt,HI_GV_TweenAnimGetTweenValue(MoveAnimationXYHandle[i], 0),HI_GV_TweenAnimGetTweenValue(MoveAnimationXYHandle[i], 0));
    }

    //刷新window
    HI_GV_Widget_Update(HOME_PAGE, HI_NULL);
}

static void animMoveXY_start_handle(HIGV_LISTENER_S* listener, void* data)
{
    printf(">>===animY_start_handle\n");
    isActing = HI_TRUE;

}

static void animMoveXY_end_handle(HIGV_LISTENER_S* listener, void* data)
{
    printf(">>===animY_end_handle\n");
    int i;
    for(i = 0;i <MaxWidget;i++)
    {
        MoveXYWidgts[i] = InvaildUIID;
    }
    isActing = HI_FALSE;
}