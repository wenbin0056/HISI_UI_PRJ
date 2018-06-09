
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


// typedef enum
// {
//     HIGV_TWEEN_TRANSITION_LINEAR,   Linear transition 
//     HIGV_TWEEN_TRANSITION_SINE,    /**< Sine transition */
//     HIGV_TWEEN_TRANSITION_QUINT,   /**< Quint transition */
//     HIGV_TWEEN_TRANSITION_QUART,   /**< Quart transition */
//     HIGV_TWEEN_TRANSITION_EXPO,    /**< Expo transition */
//     HIGV_TWEEN_TRANSITION_ELASTIC, /**< Elastic transition */
//     HIGV_TWEEN_TRANSITION_CUBIC,   /**< Cubic transition */
//     HIGV_TWEEN_TRANSITION_BOUNCE,  /**< Bounce transition */
//     HIGV_TWEEN_TRANSITION_BACK,    /**< Back transition */
//     HIGV_TWEEN_TRANSITION_BUTT
// }HIGV_TWEEN_TRANSITION_E;

/**向X方向偏移*/
HI_S32 addMoveXAnimation(HI_HANDLE hWidgt, HIGV_CORD beginX, HIGV_CORD endX,HI_U32 duration,HIGV_TWEEN_TRANSITION_E animType);

/**向Y方向偏移*/
HI_S32 addMoveYAnimation(HI_HANDLE hWidgt, HIGV_CORD beginY, HIGV_CORD endY,HI_U32 duration,HIGV_TWEEN_TRANSITION_E animType);

/**向XY方向同时偏移*/
HI_S32 addMoveXYAnimation(HI_HANDLE hWidgt, HIGV_CORD beginXY, HIGV_CORD endXY,HI_U32 duration,HIGV_TWEEN_TRANSITION_E animType);