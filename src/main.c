#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <stdbool.h>
#include <SDL2/SDL_scancode.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "TUM_Ball.h"
#include "TUM_Draw.h"
#include "TUM_Event.h"
#include "TUM_Sound.h"
#include "TUM_Utils.h"
#include "TUM_Font.h"

#include "AsyncIO.h"
#define STACK_SIZE 4
// STATE MACHINE DEFS COPIED FROM THE MASTER DEMO 
#define STATE_QUEUE_LENGTH 1

#define STATE_COUNT 2

#define STATE_ONE 0
#define STATE_TWO 1
#define STATE_THREE 2

#define NEXT_TASK 0
#define INITIAL_STATE 4
#define PAUSE_STATE 6 
#define DEAD_STATE 7
#define ACTIVE_STATE 5
#define PREV_TASK 1
#define INT_GODMODE_STATE 20
typedef struct spaceShipStruct_t
{
    bool attackState; // IF false then passive -> we can shoot
    int spaceShipMissileX;
    int spaceShipMissileY;
    int mothershipXPosition;
    int mothershipYPosition; 
    int difficulty;
    bool godmode;
    bool AI;
    int health; 
    SemaphoreHandle_t lock;
}spaceShipStruct_t;
spaceShipStruct_t spaceShipStruct;

//UDP STUFF 
static QueueHandle_t DifficultyQueue = NULL;
static QueueHandle_t AttackStateQueue = NULL;

#define UDP_BUFFER_SIZE 1024
#define UDP_RECEIVE_PORT 1234
#define UDP_TRANSMIT_PORT 1235

TaskHandle_t UDPInputCheckingTaskHandle = NULL;
TaskHandle_t UDPControlTask = NULL;
static QueueHandle_t NextKeyQueue = NULL;
static QueueHandle_t DeltaPosQueue = NULL;

static QueueHandle_t NextStateQueue = NULL;



static SemaphoreHandle_t HandleUDP = NULL;

aIO_handle_t udp_soc_receive = NULL, udp_soc_transmit = NULL;
typedef enum { NONE = 0, INC = 1, DEC = -1 } opponent_cmd_t;
void UDPHandler(size_t read_size, char *buffer, void *args)
{
    opponent_cmd_t next_key = NONE;
    BaseType_t xHigherPriorityTaskWoken1 = pdFALSE;
    BaseType_t xHigherPriorityTaskWoken2 = pdFALSE;
    BaseType_t xHigherPriorityTaskWoken3 = pdFALSE;

    if (xSemaphoreTakeFromISR(HandleUDP, &xHigherPriorityTaskWoken1) ==
        pdTRUE) {

        char send_command = 0;
        if (strncmp(buffer, "INC", (read_size < 3) ? read_size : 3) ==
            0) {
            next_key = INC;
            send_command = 1;
        }
        else if (strncmp(buffer, "DEC",
                         (read_size < 3) ? read_size : 3) == 0) {
            next_key = DEC;
            send_command = 1;
        }
        else if (strncmp(buffer, "NONE",
                         (read_size < 4) ? read_size : 4) == 0) {
            next_key = NONE;
            send_command = 1;
        }

        if (NextKeyQueue && send_command) {
            xQueueSendFromISR(NextKeyQueue, (void *)&next_key,
                              &xHigherPriorityTaskWoken2);
        }
        xSemaphoreGiveFromISR(HandleUDP, &xHigherPriorityTaskWoken3);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken1 |
                           xHigherPriorityTaskWoken2 |
                           xHigherPriorityTaskWoken3);
    }
    else {
        fprintf(stderr, "[ERROR] Overlapping UDPHandler call\n");
    }
}

signed int deltadebug;
void vUDPControlTask(void *pvParameters)
{
    static char buf[50];
    bool state;
    bool laststate = true;
    char last_difficulty = -1;
    char difficulty = 1;
    signed int delta;
    bool attackstate;
    bool lastattackstate = false;
    //udp_soc_receive = aIOOpenUDPSocket(addr, port, UDP_BUFFER_SIZE, UDPHandler, NULL);

    //printf("UDP socket opened on port %d\n", port);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(15));
        if(xSemaphoreTake(spaceShipStruct.lock,portMAX_DELAY)==pdTRUE){
            attackstate = spaceShipStruct.attackState;
            xSemaphoreGive(spaceShipStruct.lock);
        }
        while (xQueueReceive(NextStateQueue, &state, 0) == pdTRUE) {
        }
        while (xQueueReceive(DeltaPosQueue, &deltadebug,0 )==pdTRUE){
        }
        while (xQueueReceive(DifficultyQueue, &difficulty, 0) == pdTRUE) {
        }
        while (xQueueReceive(AttackStateQueue, &attackstate,0) == pdTRUE ){
        }


        if(xSemaphoreTake(spaceShipStruct.lock,portMAX_DELAY)==pdTRUE){
            if(spaceShipStruct.AI){
                    if (attackstate != lastattackstate){
                        if (attackstate){
                            sprintf(buf, "ATTACKING");    
                        }else{
                            sprintf(buf, "PASSIVE");    
                        }
                        aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf,
                                        strlen(buf));
                        lastattackstate=attackstate;
                    }
                    if(state!=laststate){
                        if(state){  
                        sprintf(buf, "RESUME");            
                        }else{
                        sprintf(buf, "PAUSE");
                        }
                        aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf,
                                        strlen(buf));
                        laststate=state;
                    }
                    if(state){
                        if(deltadebug<0){
                            sprintf(buf,"-%d",-deltadebug);
                            aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf,strlen(buf));
                        }
                        if(deltadebug>0){
                            sprintf(buf,"+%d",deltadebug);
                            aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf,strlen(buf));
                        }
                        if(deltadebug==0){
                            sprintf(buf,"%d",deltadebug);
                            aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf,strlen(buf));
                        }
                    }
                    if (last_difficulty != difficulty) {
                        sprintf(buf, "D%d", difficulty + 1);
                        aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf,
                                    strlen(buf));
                        last_difficulty = difficulty;
                    }
            }
            xSemaphoreGive(spaceShipStruct.lock);
        }



    }
}

#define STARTING_STATE INITIAL_STATE

#define STATE_DEBOUNCE_DELAY 300
SemaphoreHandle_t horizontalAlienMotion=NULL;
TaskHandle_t alienDescentTaskHandle=NULL;
TaskHandle_t AlienMissiletrackingTaskHandle=NULL;
TaskHandle_t intGodModeStateHandle=NULL;
TaskHandle_t godModeMouseDebounceTaskHandle=NULL;
SemaphoreHandle_t godModeMouseToggle=NULL;
typedef struct score_t{int score;
int killscore;
int highscore;
int level;
SemaphoreHandle_t lock;}score_t;
score_t score;
typedef struct alienMissiles_t{
    int missilePosX;
    int missilePosY;
    bool missileActive;
    SemaphoreHandle_t lock;
}alienMissiles_t;
alienMissiles_t alienMissilesStruct[8];
TaskHandle_t AlienShootingTaskHandle=NULL;
TaskHandle_t aliensMovingOneTaskHandle=NULL;
TaskHandle_t aliensMovingTwoTaskHandle=NULL;
TaskHandle_t deathStateTaskHandle = NULL;
TaskHandle_t initialStateHandle = NULL;
SemaphoreHandle_t aliensCreate=NULL;
SemaphoreHandle_t shelterCreate = NULL;
TaskHandle_t alienCreationTaskHandle=NULL;
TaskHandle_t collisionDetectionTaskHandle=NULL;
int debugVar = 0;
/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
typedef struct alien1_t{
    int posX;
    double posY;
    bool alive;
    bool active;
    SemaphoreHandle_t lock;
}alien1_t;
alien1_t  aliens_1[5][8];
typedef struct shelterblock_t{
        int posX;
        int posY;
        bool active;
        bool alive;
        bool godmode;
        SemaphoreHandle_t lock;
}shelterblock_t;
shelterblock_t shelterBlocks[20];

typedef struct Exercise3VariableIncrementationStruct {
    int LeftNumber;
    int RightNumber;
    int Variable;
    SemaphoreHandle_t lock;
} Exercise3VariableIncrementationStruct_t;
Exercise3VariableIncrementationStruct_t Exercise3VariableIncrementationStruct;
static SemaphoreHandle_t RightButtonSignal=NULL;
static SemaphoreHandle_t SpaceShipMissileSem=NULL;
static TaskHandle_t LevelIncreasingTaskHandle=NULL;
const unsigned char next_state_signal = NEXT_TASK;
const unsigned char int_godmode_state_signal = INT_GODMODE_STATE;
const unsigned char active_state_signal = ACTIVE_STATE;
const unsigned char pause_state_signal = PAUSE_STATE;
const unsigned char dead_state_signal = DEAD_STATE;
const unsigned char prev_state_signal = PREV_TASK;
static TaskHandle_t StateMachine = NULL;
static TaskHandle_t shelterCreatingTaskHandle = NULL;
static QueueHandle_t StateQueue = NULL;
//////////////////////////
#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)
#define KEYCODE(CHAR) SDL_SCANCODE_##CHAR
// Global Variables used in Exercise 2 
coord_t triangle_points[3]; //Struct holding the coordinates of the triangle
int circle_x,circle_y,square_x,square_y; 
float AngleIncrementer =0; // Used in the rotating motion of the circle and square
int buttonPresses_A=0;
int buttonPresses_B=0;
int buttonPresses_C=0;
int buttonPresses_D=0;
const int debounceDelay = pdMS_TO_TICKS(25); 
int xOffset = 0;
int yOffset = 0;
//-----------------------------------------
static TaskHandle_t DrawingTask_Handle = NULL;
static TaskHandle_t PositionIncrementationTask_Handle = NULL;
static TaskHandle_t BufferSwap = NULL;
static TaskHandle_t PauseTaskHandle = NULL;
StaticTask_t LeftCircleBuffer;
StackType_t LeftCircleStack[ STACK_SIZE ];

static SemaphoreHandle_t DrawSignal = NULL;
static SemaphoreHandle_t ScreenLock = NULL;


typedef struct buttons_buffer {
    unsigned char buttons[SDL_NUM_SCANCODES];
    unsigned char lastButtonState[SDL_NUM_SCANCODES];
    TickType_t  lastChange[SDL_NUM_SCANCODES];
    SemaphoreHandle_t lock;
} buttons_buffer_t;

static buttons_buffer_t buttons = { 0 };

// STATE MACHINE CODE 
static int vCheckStateInput(void)
{
    if (xSemaphoreTake(buttons.lock, 0) == pdTRUE) {

        
        if (buttons.buttons[KEYCODE(P)]) {
            buttons.buttons[KEYCODE(P)] = 0;
            if (StateQueue) {
                xSemaphoreGive(buttons.lock);
                xQueueSend(StateQueue, &pause_state_signal, 0);
                return 0;
            }
            return -1;
        }
        if (buttons.buttons[KEYCODE(S)]) {
            buttons.buttons[KEYCODE(S)] = 0;
            if (StateQueue) {
                xSemaphoreGive(buttons.lock);
                xQueueSend(StateQueue, &active_state_signal, 0);
                return 0;
            }
            return -1;
        }

        if (buttons.buttons[KEYCODE(C)]) {
            buttons.buttons[KEYCODE(C)] = 0;
            if (StateQueue) {
                xSemaphoreGive(buttons.lock);
                xQueueSend(StateQueue, &int_godmode_state_signal, 0);
                return 0;
            }
            return -1;
        }            
        if (buttons.buttons[KEYCODE(X)]) {
            buttons.buttons[KEYCODE(X)] = 0;
            if (StateQueue) {
                xSemaphoreGive(buttons.lock);
                xQueueSend(StateQueue, &dead_state_signal, 0);
                return 0;
            }
            return -1;
        }

        xSemaphoreGive(buttons.lock);
    }

    return 0;
}

void changeState(volatile unsigned char *state, unsigned char forwards)
{
    switch (forwards) {
        case ACTIVE_STATE:
                *state= ACTIVE_STATE;
            break;    
        case PAUSE_STATE:
                *state = PAUSE_STATE;
            break;
        case DEAD_STATE:
                *state = DEAD_STATE;
            break;
        case INT_GODMODE_STATE:
                *state = INT_GODMODE_STATE;
            break;
        default:
            break;
    }
}

/*
 * Example basic state machine with sequential states
 */
void basicSequentialStateMachine(void *pvParameters)
{
    unsigned char current_state = STARTING_STATE; // Default state
    unsigned char state_changed =
        1; // Only re-evaluate state if it has changed
    unsigned char input = 0;

    const int state_change_period = STATE_DEBOUNCE_DELAY;

    TickType_t last_change = xTaskGetTickCount();

    while (1) {
        if (state_changed) {
            goto initial_state;
        }

        // Handle state machine input
        if (StateQueue)
            if (xQueueReceive(StateQueue, &input, portMAX_DELAY) ==
                pdTRUE)
                if (xTaskGetTickCount() - last_change >
                    state_change_period) {
                    changeState(&current_state, input);
                    state_changed = 1;
                    last_change = xTaskGetTickCount();
                }

initial_state:
        // Handle current state
        if (state_changed) {
            switch (current_state) {
                case INITIAL_STATE:
                    if(PositionIncrementationTask_Handle){
                        vTaskSuspend(PositionIncrementationTask_Handle);
                    } 
                    if(DrawingTask_Handle){
                        vTaskSuspend(DrawingTask_Handle);
                    } 
                    if (deathStateTaskHandle){
                        vTaskSuspend(deathStateTaskHandle);
                    }
                    if (PauseTaskHandle) {
                        vTaskSuspend(PauseTaskHandle);
                    }
                    if (AlienMissiletrackingTaskHandle){
                        vTaskSuspend(AlienMissiletrackingTaskHandle);
                    }
                    if (alienDescentTaskHandle){
                        vTaskSuspend(alienDescentTaskHandle);
                    }
                    if(initialStateHandle){
                        vTaskResume(initialStateHandle);
                    }
                    if(intGodModeStateHandle){
                        vTaskSuspend(intGodModeStateHandle);
                    }
                    if (PositionIncrementationTask_Handle){
                        vTaskSuspend(PositionIncrementationTask_Handle);
                    }
                    break;
                case ACTIVE_STATE:
                    if (PauseTaskHandle) {
                        vTaskSuspend(PauseTaskHandle);
                    }
                    if (PositionIncrementationTask_Handle) {
                        vTaskResume(PositionIncrementationTask_Handle);
                    }
                    if (PositionIncrementationTask_Handle){
                        vTaskResume(PositionIncrementationTask_Handle);
                    }
                    if (DrawingTask_Handle) {
                        vTaskResume(DrawingTask_Handle);
                    }
                    if (AlienMissiletrackingTaskHandle){
                        vTaskResume(AlienMissiletrackingTaskHandle);
                    }
                    if (deathStateTaskHandle){
                        vTaskSuspend(deathStateTaskHandle);
                    }
                    if(initialStateHandle){
                        vTaskSuspend(initialStateHandle);
                    }
                    if (alienDescentTaskHandle){
                        vTaskResume(alienDescentTaskHandle);
                    }
                    if(intGodModeStateHandle){
                        vTaskSuspend(intGodModeStateHandle);
                    }
                    break;
                case PAUSE_STATE:
                    if (DrawingTask_Handle) {
                        vTaskSuspend(DrawingTask_Handle);
                    }
                    if (PositionIncrementationTask_Handle) {
                        vTaskSuspend(PositionIncrementationTask_Handle);
                    }
                    if (PauseTaskHandle) {
                        vTaskResume(PauseTaskHandle);
                    }
                    if (AlienMissiletrackingTaskHandle){
                        vTaskSuspend(AlienMissiletrackingTaskHandle);
                    }
                    if (deathStateTaskHandle){
                        vTaskSuspend(deathStateTaskHandle);
                    }
                    if(initialStateHandle){
                        vTaskSuspend(initialStateHandle);
                    }
                    if (alienDescentTaskHandle){
                        vTaskSuspend(alienDescentTaskHandle);
                    }
                    if(intGodModeStateHandle){
                        vTaskSuspend(intGodModeStateHandle);
                    }
                    if (PositionIncrementationTask_Handle){
                        vTaskSuspend(PositionIncrementationTask_Handle);
                    }
                    break;
                case DEAD_STATE:
                    if(PositionIncrementationTask_Handle){
                        vTaskSuspend(PositionIncrementationTask_Handle);
                    } 
                    if(DrawingTask_Handle){
                        vTaskSuspend(DrawingTask_Handle);
                    } 
                    if (deathStateTaskHandle){
                        vTaskResume(deathStateTaskHandle);
                    }
                    if (PauseTaskHandle) {
                        vTaskSuspend(PauseTaskHandle);
                    }
                    if (AlienMissiletrackingTaskHandle){
                        vTaskSuspend(AlienMissiletrackingTaskHandle);
                    }
                    if(initialStateHandle){
                        vTaskSuspend(initialStateHandle);
                    }
                    if (alienDescentTaskHandle){
                        vTaskSuspend(alienDescentTaskHandle);
                    }
                    if(intGodModeStateHandle){
                        vTaskSuspend(intGodModeStateHandle);
                    }
                    if (PositionIncrementationTask_Handle){
                        vTaskSuspend(PositionIncrementationTask_Handle);
                    }
                    break;
                case INT_GODMODE_STATE:
                    if(PositionIncrementationTask_Handle){
                        vTaskSuspend(PositionIncrementationTask_Handle);
                    } 
                    if(DrawingTask_Handle){
                        vTaskSuspend(DrawingTask_Handle);} 
                    if (deathStateTaskHandle){
                        vTaskSuspend(deathStateTaskHandle);
                    }
                    if (PauseTaskHandle) {
                        vTaskSuspend(PauseTaskHandle);
                    }
                    if (AlienMissiletrackingTaskHandle){
                        vTaskSuspend(AlienMissiletrackingTaskHandle);
                    }
                    if (alienDescentTaskHandle){
                        vTaskSuspend(alienDescentTaskHandle);
                    }
                    if(initialStateHandle){
                        vTaskSuspend(initialStateHandle);
                    }
                    if(intGodModeStateHandle){
                        vTaskResume(intGodModeStateHandle);
                    }
                    if (PositionIncrementationTask_Handle){
                        vTaskSuspend(PositionIncrementationTask_Handle);
                    }
                    break;
            }
            state_changed = 0;
        }
    }
}
//____________________________________________________________________________________
#define FPS_AVERAGE_COUNT 50
#define FPS_FONT "IBMPlexSans-Bold.ttf"

void vDrawFPS(void)
{
    static unsigned int periods[FPS_AVERAGE_COUNT] = { 0 };
    static unsigned int periods_total = 0;
    static unsigned int index = 0;
    static unsigned int average_count = 0;
    static TickType_t xLastWakeTime = 0, prevWakeTime = 0;
    static char str[10] = { 0 };
    static int text_width;
    int fps = 0;
    font_handle_t cur_font = tumFontGetCurFontHandle();

    if (average_count < FPS_AVERAGE_COUNT) {
        average_count++;
    }
    else {
        periods_total -= periods[index];
    }

    xLastWakeTime = xTaskGetTickCount();

    if (prevWakeTime != xLastWakeTime) {
        periods[index] =
            configTICK_RATE_HZ / (xLastWakeTime - prevWakeTime);
        prevWakeTime = xLastWakeTime;
    }
    else {
        periods[index] = 0;
    }

    periods_total += periods[index];

    if (index == (FPS_AVERAGE_COUNT - 1)) {
        index = 0;
    }
    else {
        index++;
    }

    fps = periods_total / average_count;

    tumFontSelectFontFromName(FPS_FONT);

    sprintf(str, "FPS: %2d", fps);

    if (!tumGetTextSize((char *)str, &text_width, NULL))
        checkDraw(tumDrawText(str, SCREEN_WIDTH - text_width - 10,
                              SCREEN_HEIGHT - DEFAULT_FONT_SIZE * 1.5,
                              Skyblue),
                  __FUNCTION__);

    tumFontSelectFontFromHandle(cur_font);
    tumFontPutFontHandle(cur_font);
}
void xGetButtonInput(void)
{
    if (xSemaphoreTake(buttons.lock, 0) == pdTRUE) {
        xQueueReceive(buttonInputQueue, &buttons.buttons, 0);
        xSemaphoreGive(buttons.lock);
    }
}
void checkDraw(unsigned char status, const char *msg)
{
    if (status) {
        if (msg)
            fprints(stderr, "[ERROR] %s, %s\n", msg,
                    tumGetErrorMessage());
        else {
            fprints(stderr, "[ERROR] %s\n", tumGetErrorMessage());
        }
    }   
}
/*  
 *   The Task that refreshes the screen at a constant frame rate
 */
void vSwapBuffers(void *pvParameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    const TickType_t frameratePeriod = 20;

    tumDrawBindThread(); // Setup Rendering handle with correct GL context

    while (1) {
        if (xSemaphoreTake(ScreenLock, portMAX_DELAY) == pdTRUE) {
            tumDrawUpdateScreen();
            tumEventFetchEvents(FETCH_EVENT_BLOCK);
            xSemaphoreGive(ScreenLock);
            xSemaphoreGive(DrawSignal);
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(frameratePeriod));
        }
    }
}


/* function that detects a change in the state of a button 

    takes in : The SDL index of the button to be checked 
    
    gives out: true if the button is changed from LOW to HIGH and false else 
*/    
bool ButtonStateChangeCheck(int buttonSDLIndex)
{
    //step 1 initialise a ret and a button state variable 
    unsigned char buttonState = 0;
    bool ret = false;
    
    if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
    {
        buttonState = buttons.buttons[buttonSDLIndex]; //save the state from the buffer onto the variable we just created
    }

    //Step3 Compare the button state to the previous one
    if (buttonState != buttons.lastButtonState[buttonSDLIndex]) {
        //If theyre diff : check the debounce 
        if (xTaskGetTickCount() - buttons.lastChange[buttonSDLIndex] > debounceDelay) {
            if (buttonState ==1 ) {
                //this means LOW to HIGH return TRUE 
                ret = true;
            }
            // Save the time at which the change occured
            buttons.lastChange[buttonSDLIndex] = xTaskGetTickCount();
        }
    }
    // Update the buffer with the new state 
    buttons.lastButtonState[buttonSDLIndex] = buttonState;
    xSemaphoreGive(buttons.lock);

    return ret;
}

void Drawing_Task(void *pvParameters)
{
        // Mothership image handle
        image_handle_t ball_spritesheet_image =
        tumDrawLoadImage("../resources/images/mothership.png");

        // Main logo at the top middle section 
        image_handle_t mainlogo_image = 
        tumDrawLoadImage("../resources/images/mainlogo.png");

        //Alien Type 1 
        image_handle_t alien1_image =
        tumDrawLoadImage("../resources/images/firstalientype.png");

        //Alien Type 2
        image_handle_t alien2_image =
        tumDrawLoadImage("../resources/images/secondalientype.png");
    
        //Alien Type 3
        image_handle_t alien3_image =
        tumDrawLoadImage("../resources/images/thirdalientype.png");

        //Alien Type 4
        image_handle_t alien4_image =
        tumDrawLoadImage("../resources/images/fourthalientype.png");

        //Alien Type 5
        image_handle_t alien5_image =
        tumDrawLoadImage("../resources/images/fifthalientype.png");

        //Mothership Health
        image_handle_t heart_image=
        tumDrawLoadImage("../resources/images/heart.png");

        static char strdt[40] = { 0 };
               
    while (1) {

            if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
                pdTRUE) {

                xSemaphoreTake(ScreenLock, portMAX_DELAY);
                tumDrawClear(Black); // Clear screen
                tumDrawSetLoadedImageScale(ball_spritesheet_image,0.05);
                tumDrawSetLoadedImageScale(mainlogo_image,0.1);
                tumDrawLoadedImage(mainlogo_image,275,20);

                //Score drawing
                if (xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){
                    sprintf(strdt,"Score: %d",score.killscore);
                    tumDrawText(strdt,150,
                              450,
                              Yellow);
                    xSemaphoreGive(score.lock);
                }
                if (xSemaphoreTake(spaceShipStruct.lock,portMAX_DELAY)==pdTRUE){
                    sprintf(strdt,"AI = D%d",spaceShipStruct.difficulty+1);
                    tumDrawText(strdt,250,450,Yellow);

                    if(spaceShipStruct.AI){
                       sprintf(strdt,"AI MODE ON"); 
                    }else{
                        sprintf(strdt,"AI MODE OFF");
                    }
                    tumDrawText(strdt,350,450,Yellow);
                    xSemaphoreGive(spaceShipStruct.lock);
                }
                // Drawing the line that separates health, score and FPS from the ship
                tumDrawFilledBox(0,435,640,5,White);

                //Drawing the shelter blocks
                for (int counter =0; counter <20;counter++){
                    if (xSemaphoreTake(shelterBlocks[counter].lock,0)==pdTRUE){
                        if(shelterBlocks[counter].alive==true){
                            tumDrawFilledBox(shelterBlocks[counter].posX,shelterBlocks[counter].posY,25,2,White);
                        }
                        xSemaphoreGive(shelterBlocks[counter].lock);
                    }
                }   

                //Drawing Missiles fired by our mother ship and the ship itself and hearts
                if (xSemaphoreTake(spaceShipStruct.lock,0)==pdTRUE){	
                    tumDrawLoadedImage(ball_spritesheet_image,spaceShipStruct.mothershipXPosition,
                                        spaceShipStruct.mothershipYPosition);                  
                        if (spaceShipStruct.attackState){
                            tumDrawFilledBox(spaceShipStruct.spaceShipMissileX+10,spaceShipStruct.spaceShipMissileY,3,5,Red);
                        }
                    
                    //Drawing the hearts
                    tumDrawSetLoadedImageScale(heart_image,0.03);
                    switch(spaceShipStruct.health){
                        case 3:
                            tumDrawLoadedImage(heart_image,10,450);
                            tumDrawLoadedImage(heart_image,30,450);
                            tumDrawLoadedImage(heart_image,50,450);
                            break;
                        case 2:
                            tumDrawLoadedImage(heart_image,10,450);
                            tumDrawLoadedImage(heart_image,30,450);

                            break;
                        case 1: 
                            tumDrawLoadedImage(heart_image,10,450);
                            break;
                        case 0:
                            break;
                    }


                    xSemaphoreGive(spaceShipStruct.lock);
                }

                // Drawing Missiles fired by the random aliens

                for (int c1=0;c1<8;c1++){
                    if (xSemaphoreTake(alienMissilesStruct[c1].lock,portMAX_DELAY)==pdTRUE){
                        if(alienMissilesStruct[c1].missileActive){
                            tumDrawFilledBox(alienMissilesStruct[c1].missilePosX,alienMissilesStruct[c1].missilePosY,3,5,White);
                        }
                        xSemaphoreGive(alienMissilesStruct[c1].lock);
                    }

                }
                

                //Drawing aliens
                for (int c1=0;c1<5;c1++){
                    for(int c2=0;c2<8;c2++){
                        if(xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                            if(aliens_1[c1][c2].alive) {
                                switch(c1){ 
                                case 0:    
                                tumDrawSetLoadedImageScale(alien1_image,0.03);    
                                tumDrawLoadedImage(alien1_image,aliens_1[c1][c2].posX,aliens_1[c1][c2].posY);
                                break;

                                case 1:
                                tumDrawSetLoadedImageScale(alien2_image,0.03);    
                                tumDrawLoadedImage(alien2_image,aliens_1[c1][c2].posX,aliens_1[c1][c2].posY);
                                break;

                                case 2:
                                tumDrawSetLoadedImageScale(alien3_image,0.047);    
                                tumDrawLoadedImage(alien3_image,aliens_1[c1][c2].posX,aliens_1[c1][c2].posY);    
                                break;

                                case 3:
                                tumDrawSetLoadedImageScale(alien4_image,0.04);    
                                tumDrawLoadedImage(alien4_image,aliens_1[c1][c2].posX,aliens_1[c1][c2].posY);    
                                break;

                                case 4:
                                tumDrawSetLoadedImageScale(alien5_image,0.055);    
                                tumDrawLoadedImage(alien5_image,aliens_1[c1][c2].posX,aliens_1[c1][c2].posY);    
                                break;
                                }
                            }
                            xSemaphoreGive(aliens_1[c1][c2].lock);
                        }
                    }
                }

                

                vDrawFPS();
                xSemaphoreGive(ScreenLock);
            }

        vTaskDelay(20);

      vCheckStateInput();
    }

}


void vSpaceshipShoot(){
  
        
    if (spaceShipStruct.attackState == false){
        spaceShipStruct.attackState = true;
        spaceShipStruct.spaceShipMissileX=spaceShipStruct.mothershipXPosition;
        spaceShipStruct.spaceShipMissileY=spaceShipStruct.mothershipYPosition+3;
    }
  
}


void PositionIncrementation_Task(void *pvParameters){
//Spaceship initialisation
char difficulty = 1; // 0: easy 1: normal 2: hard
if (xSemaphoreTake(spaceShipStruct.lock,0)==pdTRUE){
    spaceShipStruct.attackState=false;
    spaceShipStruct.AI=false;
    spaceShipStruct.health=3;    
    spaceShipStruct.mothershipXPosition=310;
    spaceShipStruct.mothershipYPosition=400;   

    xSemaphoreGive(spaceShipStruct.lock);
    }
bool state = true;
in_port_t port = UDP_RECEIVE_PORT;
    while(1){
                tumEventFetchEvents(FETCH_EVENT_BLOCK |
                                    FETCH_EVENT_NO_GL_CHECK);
                xGetButtonInput(); // Update global input
                if (xSemaphoreTake(buttons.lock, 0) == pdTRUE) {
                        if (buttons.buttons[KEYCODE(ESCAPE)]) { // Equiv to SDL_SCANCODE_Q
                            exit(EXIT_SUCCESS);
                        }
                    xSemaphoreGive(buttons.lock);
                }
                if (xSemaphoreTake(spaceShipStruct.lock,0)==pdTRUE){
                    if (xSemaphoreTake(buttons.lock, 0) == pdTRUE) {

                        if (buttons.buttons[KEYCODE(RIGHT)]) {
                            if(spaceShipStruct.mothershipXPosition<=600){ 
                            spaceShipStruct.mothershipXPosition+=7;
                        }}
                        if (buttons.buttons[KEYCODE(LEFT)]) { 
                            if(spaceShipStruct.mothershipXPosition>=10){ 
                            spaceShipStruct.mothershipXPosition-=7;
                        }}
                        xSemaphoreGive(buttons.lock);
                    }
                
                    if (ButtonStateChangeCheck(KEYCODE(SPACE))==true){
                            vSpaceshipShoot();}  

                    if (ButtonStateChangeCheck(KEYCODE(D))==true){
                        difficulty = (difficulty + 1) % 3;
                        spaceShipStruct.difficulty=difficulty;
                        xQueueSend(DifficultyQueue, (void *) &difficulty, portMAX_DELAY);}  

                    if (ButtonStateChangeCheck(KEYCODE(A))==true){
                        if(spaceShipStruct.AI){	 
                            if (udp_soc_receive==NULL){printf("BIG ERROR\n");}                          
                            aIOCloseConn(udp_soc_receive);
                            printf("UDP socket closed on port %d\n", port);
                            spaceShipStruct.AI=false;                            
                        }
                        else{
                            spaceShipStruct.AI=true;
                            udp_soc_receive = aIOOpenUDPSocket(NULL, port, UDP_BUFFER_SIZE, UDPHandler, NULL);
                            printf("UDP socket opened on port %d\n", port);
                        }
                    }
                    if (ButtonStateChangeCheck(KEYCODE(H))==true){
                        
                    }
                        
                    if (spaceShipStruct.attackState){
                        xQueueSend(AttackStateQueue,&spaceShipStruct.attackState,0);
                        spaceShipStruct.spaceShipMissileY-=10;                    
                        if (spaceShipStruct.spaceShipMissileY<=0){spaceShipStruct.attackState=false;}
                    }else{xQueueSend(AttackStateQueue,&spaceShipStruct.attackState,0);}
                    if(spaceShipStruct.health<=0){
                        spaceShipStruct.health=3;
                        xQueueSend(StateQueue, &dead_state_signal, 0);
                    }
                    xSemaphoreGive(spaceShipStruct.lock);
                }
                xQueueSend(NextStateQueue, &state, 0);
                vTaskDelay(20);
            }
            vCheckStateInput();

}



void vUDPInputCheckingTask(){
int lastpos; 
signed int delta;
    while(1){

                static opponent_cmd_t current_key = NONE;

                if (NextKeyQueue) {
                    xQueueReceive(NextKeyQueue, &current_key, 0);
                }
                if (current_key){ 
                    if(xSemaphoreTake(spaceShipStruct.lock,portMAX_DELAY)==pdTRUE){
                        if(spaceShipStruct.AI){
                            lastpos=spaceShipStruct.mothershipXPosition;
                            if (current_key == INC) {
                                //printf("INC\n"); for debugging purposes
                                if(spaceShipStruct.mothershipXPosition<=500){ 
                                    spaceShipStruct.mothershipXPosition+=7;
                                    delta= lastpos - spaceShipStruct.mothershipXPosition;
                                    xQueueSend(DeltaPosQueue,&delta,0);
                                }else if(spaceShipStruct.mothershipXPosition>500 && spaceShipStruct.mothershipXPosition<=590){ 
                                    spaceShipStruct.mothershipXPosition+=7;
                                    delta= lastpos - spaceShipStruct.mothershipXPosition;
                                    xQueueSend(DeltaPosQueue,&delta,0);
                                }else if(spaceShipStruct.mothershipXPosition>=590){
                                    delta =0;
                                    xQueueSend(DeltaPosQueue,&delta,0);
                                } 
                            }
                            else if (current_key == DEC) {
                               // printf("DEC\n"); for debugging purposes
                                if(spaceShipStruct.mothershipXPosition>=10){ 
                                    spaceShipStruct.mothershipXPosition-=7;
                                    delta= lastpos - spaceShipStruct.mothershipXPosition;
                                    xQueueSend(DeltaPosQueue,&delta,0);
                                }else {delta= 0;
                                    xQueueSend(DeltaPosQueue,&delta,0);}                
                            }  
                        }
                    xSemaphoreGive(spaceShipStruct.lock);
                    }
                }




        vTaskDelay(20);
    }

}


void shelterCreatingTask(){
    for (int counter =0; counter <20;counter++){
        if (xSemaphoreTake(shelterBlocks[counter].lock,0)==pdTRUE){
        if (counter<5){     
        shelterBlocks[counter].alive=true;    
        shelterBlocks[counter].posX=50 + counter*25;
        shelterBlocks[counter].posY=350;
        }
        if (counter>=5 && counter< 10){
        shelterBlocks[counter].alive=true;    
        shelterBlocks[counter].posX= counter*25;
        shelterBlocks[counter].posY=300;
        }
        if (counter>=10 && counter< 15){
        shelterBlocks[counter].alive=true;    
        shelterBlocks[counter].posX= counter*25;
        shelterBlocks[counter].posY=325;
        }
        if (counter>=15){
        shelterBlocks[counter].alive=true;    
        shelterBlocks[counter].posX=20+counter*25;
        shelterBlocks[counter].posY=350;
        }
        xSemaphoreGive(shelterBlocks[counter].lock);
        }
    }


    while(1){
        if (xSemaphoreTake(shelterCreate,portMAX_DELAY)==pdTRUE){
            for (int counter =0; counter <20;counter++){
                if (xSemaphoreTake(shelterBlocks[counter].lock,0)==pdTRUE){
                shelterBlocks[counter].alive=true;    
                xSemaphoreGive(shelterBlocks[counter].lock);
                }   
            }
        }
                
                
            
        vTaskDelay((TickType_t)20);
    }
}
void collisionDetectionTask(){
    while(1){
        if(xSemaphoreTake(spaceShipStruct.lock,portMAX_DELAY)==pdTRUE){ //to get the access to the missile's position

            //Collision detection between the mothership's missile and the shelter blocks
            for (int counter =0; counter <20;counter++){
                if(xSemaphoreTake(shelterBlocks[counter].lock,portMAX_DELAY)==pdTRUE){// to get access to the shelter's location and state
                    if (abs(spaceShipStruct.spaceShipMissileY- shelterBlocks[counter].posY)<8 && abs(spaceShipStruct.spaceShipMissileX -shelterBlocks[counter].posX)<15 && 
                    shelterBlocks[counter].alive) {
                        shelterBlocks[counter].alive=false;
                        spaceShipStruct.attackState=false;
                    }
                xSemaphoreGive(shelterBlocks[counter].lock);    
                }
            }

            //Collision detection between the mothership's missile and aliens 
            for(int c1=0;c1<5;c1++){
                for(int c2=0;c2<8;c2++){
                    if (xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                        if (abs(spaceShipStruct.spaceShipMissileY- aliens_1[c1][c2].posY)<4 && abs(spaceShipStruct.spaceShipMissileX -aliens_1[c1][c2].posX)<13 && 
                            aliens_1[c1][c2].alive && spaceShipStruct.attackState==true) {
                                aliens_1[c1][c2].alive=false;
                                spaceShipStruct.attackState=false;
                                aliens_1[c1][c2].active=false;
                                if (c1 != 0){ 
                                aliens_1[c1-1][c2].active=true;
                                }
                                
                                if (xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){
                                    score.score++;
                                    switch(c1){ 
                                    case 0:
                                        score.killscore+=50;
                                        break;
                                    case 1: 
                                        score.killscore+=40;
                                        break;
                                    case 2:
                                        score.killscore+=30;
                                        break;
                                    case 3:
                                        score.killscore+=20;
                                        break; 
                                    case 4:
                                        score.killscore+=10;
                                        break;    
                                    }
                                    xSemaphoreGive(score.lock);
                                }
                            }
                        xSemaphoreGive(aliens_1[c1][c2].lock);
                    }
                }
            }

        xSemaphoreGive(spaceShipStruct.lock);

        } 
        
        // Collision detection between the alien Missiles and shelter
            for (int counter =0; counter <20;counter++){
                if(xSemaphoreTake(shelterBlocks[counter].lock,portMAX_DELAY)==pdTRUE){// to get access to the shelter's location and state
                    for(int x=0; x<8; x++){ 
                        xSemaphoreTake(alienMissilesStruct[x].lock,portMAX_DELAY);
                        if (abs(alienMissilesStruct[x].missilePosY- shelterBlocks[counter].posY)<8 && abs(alienMissilesStruct[x].missilePosX -shelterBlocks[counter].posX)<20 && 
                        shelterBlocks[counter].alive && alienMissilesStruct[x].missileActive) {
                            shelterBlocks[counter].alive=false;
                            alienMissilesStruct[x].missileActive=false;
                        }
                        xSemaphoreGive(alienMissilesStruct[x].lock);
                    }    
                    xSemaphoreGive(shelterBlocks[counter].lock);    
                }
            }        

                // Collision detection between the aliens and shelter
            for (int counter =0; counter <20;counter++){
                if(xSemaphoreTake(shelterBlocks[counter].lock,portMAX_DELAY)==pdTRUE){// to get access to the shelter's location and state

                    for(int c1= 0;c1<5;c1++){
                        for (int c2=0;c2<8;c2++){
                            if(xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                              if (abs(aliens_1[c1][c2].posY- shelterBlocks[counter].posY)<8 && abs(aliens_1[c1][c2].posX -shelterBlocks[counter].posX)<20 && 
                                    shelterBlocks[counter].alive && aliens_1[c1][c2].alive){
                                    shelterBlocks[counter].alive=false;
                                 } 
                    
                              xSemaphoreGive(aliens_1[c1][c2].lock);
                            }
                        }
                    }
                    xSemaphoreGive(shelterBlocks[counter].lock);    
                }
            }        


        // Collision detection between the alien Missiles and the mothership 
        if(xSemaphoreTake(spaceShipStruct.lock,portMAX_DELAY)==pdTRUE){
            for(int counter=0; counter<8;counter++){
                if(xSemaphoreTake(alienMissilesStruct[counter].lock,portMAX_DELAY)==pdTRUE){
                       if (abs(alienMissilesStruct[counter].missilePosY- 400)<15 && abs(alienMissilesStruct[counter].missilePosX -spaceShipStruct.mothershipXPosition)<16 && alienMissilesStruct[counter].missileActive) {                           
                           if(spaceShipStruct.godmode==false){ 
                                spaceShipStruct.health--;
                                }
                            alienMissilesStruct[counter].missileActive=false;               

                        } 
                   xSemaphoreGive(alienMissilesStruct[counter].lock); 
                }
            }
            xSemaphoreGive(spaceShipStruct.lock);    
        }

        //collision detection between aliens and shelter blocks    

        // ALIEN bottom descent detection
        for(int c1 =0 ; c1 < 5 ; c1++){
            for(int c2 = 0; c2 <8 ;c2++){
                if (xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                     if(aliens_1[c1][c2].posY>=400){
                      xQueueSend(StateQueue, &dead_state_signal, 0);}
                xSemaphoreGive(aliens_1[c1][c2].lock);
                }
            }
        }    

        
        vTaskDelay((TickType_t)1);

    }
}

void alienCreationTask(){
    // Score initialisation
    if (xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){
        score.killscore=0;
        score.score=0;
        score.level=0;
        xSemaphoreGive(score.lock);
    }

    //alien variable initialisation
    for(int c1 =0 ; c1 < 5 ; c1++){
        for(int c2 = 0; c2 <8 ;c2++){
            if (xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                aliens_1[c1][c2].active=false;
                aliens_1[c1][c2].alive=true;
                switch (c1){
                                case 0:
                                aliens_1[c1][c2].posX=90+ c2*60;
                                aliens_1[c1][c2].posY=80;
                                break;
                                case 1:
                                aliens_1[c1][c2].posX= 90+ c2*60;
                                aliens_1[c1][c2].posY=105;
                                break;
                                case 2:
                                aliens_1[c1][c2].posX= 90+ c2*60;
                                aliens_1[c1][c2].posY=130;
                                break;
                                case 3:
                                aliens_1[c1][c2].posX= 90+ c2*60;
                                aliens_1[c1][c2].posY=155;
                                break;
                                case 4:
                                aliens_1[c1][c2].posX=90+ c2*60;
                                aliens_1[c1][c2].posY=180;
                                aliens_1[c1][c2].active=true;
                                break;
                }
                
                xSemaphoreGive(aliens_1[c1][c2].lock);
            }

        }
    }
    while(1){
        if(xSemaphoreTake(aliensCreate,0)==pdTRUE){
                for(int c1 =0 ; c1 < 5 ; c1++){
                    for(int c2 = 0; c2 <8 ;c2++){
                        if (xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                            aliens_1[c1][c2].active=false;
                            aliens_1[c1][c2].alive=true;
                            switch (c1){
                                case 0:
                                aliens_1[c1][c2].posX=90+ c2*60;
                                aliens_1[c1][c2].posY=80;
                                break;
                                case 1:
                                aliens_1[c1][c2].posX= 90+ c2*60;
                                aliens_1[c1][c2].posY=105;
                                break;
                                case 2:
                                aliens_1[c1][c2].posX= 90+ c2*60;
                                aliens_1[c1][c2].posY=130;
                                break;
                                case 3:
                                aliens_1[c1][c2].posX= 90+ c2*60;
                                aliens_1[c1][c2].posY=155;
                                break;
                                case 4:
                                aliens_1[c1][c2].posX=90+ c2*60;
                                aliens_1[c1][c2].posY=180;
                                aliens_1[c1][c2].active=true;
                                break;   
                            }

                        xSemaphoreGive(aliens_1[c1][c2].lock);
                        }

                    }
                }
        }
        vTaskDelay((TickType_t)20);
    }
}
void vAlienShoot(){

}
void AlienShootingTask(){
    int randomNumber;
    while(1){
            if (xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){
                randomNumber= rand() % (7 + 1 - 0) + 0;
                xSemaphoreGive(score.lock);
            }
            for(int c1=0;c1<5;c1++){
                if (xSemaphoreTake(aliens_1[c1][randomNumber].lock,portMAX_DELAY)==pdTRUE){

                
                    if (aliens_1[c1][randomNumber].alive && aliens_1[c1][randomNumber].active){
                        if(xSemaphoreTake(alienMissilesStruct[randomNumber].lock,portMAX_DELAY)==pdTRUE){
                            if (alienMissilesStruct[randomNumber].missileActive==false){
                            alienMissilesStruct[randomNumber].missileActive=true;
                            alienMissilesStruct[randomNumber].missilePosX=aliens_1[c1][randomNumber].posX;
                            alienMissilesStruct[randomNumber].missilePosY=aliens_1[c1][randomNumber].posY;
                            }
                            xSemaphoreGive(alienMissilesStruct[randomNumber].lock);
                        }
                    }
                xSemaphoreGive(aliens_1[c1][randomNumber].lock);    
                }
            } 
            vTaskDelay(pdMS_TO_TICKS(3000));
    }     
}

void AlienMissiletrackingTask(){

    while(1){
        for(int c1=0;c1<8;c1++){
            if(xSemaphoreTake(alienMissilesStruct[c1].lock,portMAX_DELAY)==pdTRUE){
                if(alienMissilesStruct[c1].missileActive){
                    alienMissilesStruct[c1].missilePosY++;
                    if (alienMissilesStruct[c1].missilePosY>500){
                        alienMissilesStruct[c1].missileActive=false;
                    }
                }
                xSemaphoreGive(alienMissilesStruct[c1].lock); 
            }
        }
        vTaskDelay((TickType_t)20);
    }
}

typedef struct aliensHorizontalMotionStruct_t{
    int posXCounter;
    bool motiontoggle;
    bool excessMotionToggle;
    SemaphoreHandle_t lock;
}aliensHorizontalMotionStruct_t;
aliensHorizontalMotionStruct_t aliensHorizontalMotionStruct;

void AliensMovingOneTask(){
    int excessMotion;
    if (xSemaphoreTake(aliensHorizontalMotionStruct.lock,portMAX_DELAY)==pdTRUE){
        aliensHorizontalMotionStruct.posXCounter=0;
        aliensHorizontalMotionStruct.motiontoggle=false;
        aliensHorizontalMotionStruct.excessMotionToggle=false;
        xSemaphoreGive(aliensHorizontalMotionStruct.lock);
    }
    while(1){
       if(xSemaphoreTake(aliensHorizontalMotionStruct.lock,portMAX_DELAY)==pdTRUE){ 
           if(aliensHorizontalMotionStruct.motiontoggle==false){ 
                for (int c1 =0;c1<5;c1++){
                    for(int c2 = 0; c2<8; c2++){
                        if (xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                            switch (c1) {
                                case 0: aliens_1[c1][c2].posX+=1;
                                        break;
                                case 1:aliens_1[c1][c2].posX-=1;
                                       break;
                                case 2:aliens_1[c1][c2].posX+=1;
                                       break;
                                case 3:aliens_1[c1][c2].posX-=1;
                                       break;
                                case 4:aliens_1[c1][c2].posX+=1;
                                       break;
                            }
                            //aliens_1[c1][c2].posX+=1 ;
                            xSemaphoreGive(aliens_1[c1][c2].lock);
                        }
                        
                    }
                }
                if (aliensHorizontalMotionStruct.excessMotionToggle==false){
                    excessMotion=20;                    
                }else {excessMotion=40;}

                aliensHorizontalMotionStruct.posXCounter +=1 ;
                if (aliensHorizontalMotionStruct.posXCounter >=excessMotion){
                    aliensHorizontalMotionStruct.posXCounter=0;
                    aliensHorizontalMotionStruct.motiontoggle=true;
                    aliensHorizontalMotionStruct.excessMotionToggle=true;
                }
            }
            xSemaphoreGive(aliensHorizontalMotionStruct.lock);
        }
        vTaskDelay((TickType_t)40);
    }       
}

void AliensMovingTwoTask(){
    while(1){
       if(xSemaphoreTake(aliensHorizontalMotionStruct.lock,portMAX_DELAY)==pdTRUE){ 
           if(aliensHorizontalMotionStruct.motiontoggle==true){ 
                for (int c1 =0;c1<5;c1++){
                    for(int c2 = 0; c2<8; c2++){
                        if (xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                            switch (c1) {
                                case 0: aliens_1[c1][c2].posX-=1;
                                        break;
                                case 1:aliens_1[c1][c2].posX+=1;
                                       break;
                                case 2:aliens_1[c1][c2].posX-=1;
                                       break;
                                case 3:aliens_1[c1][c2].posX+=1;
                                       break;
                                case 4:aliens_1[c1][c2].posX-=1;
                                       break;
                            }
                            
                            xSemaphoreGive(aliens_1[c1][c2].lock);
                        }
                    }
                }
                aliensHorizontalMotionStruct.posXCounter +=1;
                if (aliensHorizontalMotionStruct.posXCounter >=40){
                    aliensHorizontalMotionStruct.posXCounter=0;
                    aliensHorizontalMotionStruct.motiontoggle=false;
                }
            }
            xSemaphoreGive(aliensHorizontalMotionStruct.lock);
        }
        vTaskDelay((TickType_t)40);
    }       
}
void vAlienDescentTask(){
    while(1){
        if(xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){ 
            for (int c1 =0;c1<5;c1++){
                    for(int c2=0;c2<8;c2++){
                         if(xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                            aliens_1[c1][c2].posY+= 0.05+(0.01* score.level)+(0.001* score.score);
   
                            xSemaphoreGive(aliens_1[c1][c2].lock);
                         }
                         
                    }

                }
        xSemaphoreGive(score.lock);
        }
        vTaskDelay(40);
    }
}
void LevelIncreasingTask(){
    while(1){
        //Check if we reached a multiple of 50 so that we could re draw the aliens and later on change their speeds accordingly
        if (xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){
            if (score.score%40==0 && score.score != 0){
                score.level++;
                xSemaphoreTake(aliensHorizontalMotionStruct.lock,portMAX_DELAY);
                aliensHorizontalMotionStruct.excessMotionToggle=false;
                xSemaphoreGive(aliensHorizontalMotionStruct.lock);
                xSemaphoreGive(aliensCreate);
                xSemaphoreGive(shelterCreate);
                score.score=0;
            }
            xSemaphoreGive(score.lock);
        }
        vTaskDelay((TickType_t)20);
    }
}




void vPauseState(void *pvParameters){
    image_handle_t pause_image =
    tumDrawLoadImage("../resources/images/pause.png");        
    tumDrawSetLoadedImageScale(pause_image,0.3);    
    static char str[20] = { 0 };
    bool state = false;
    while(1)
    {
    tumEventFetchEvents(FETCH_EVENT_BLOCK |
                                FETCH_EVENT_NO_GL_CHECK);
    xGetButtonInput(); // Update global input
    if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
                pdTRUE) {            
                xSemaphoreTake(ScreenLock, portMAX_DELAY);
                tumDrawSetGlobalXOffset(0);
                tumDrawSetGlobalYOffset(0);
                tumDrawClear(Black); // Clear screen
                tumDrawLoadedImage(pause_image,30,90);
                
                    sprintf(str,"Pause menu  S TO START");
                    tumDrawText(str,400,360,Red);


                    if(xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){
                        sprintf(str,"Current score: %d", score.killscore);
                        tumDrawText(str,400,400,Red);
                        sprintf(str,"Current high score: %d",score.highscore);
                        tumDrawText(str,400,440,Red);
                        xSemaphoreGive(score.lock);
                    }
        
                xSemaphoreGive(ScreenLock);
            }

    xQueueSend(NextStateQueue, &state, 0);               
    xSemaphoreGive(DrawSignal);
    vCheckStateInput();
    vTaskDelay(20);
    }
}
void vInitialState(){
        image_handle_t gamestart_image =
        tumDrawLoadImage("../resources/images/gamestart.png");        
        tumDrawSetLoadedImageScale(gamestart_image,0.5);
        static char strdtt[20] = { 0 };
    while(1){
        tumEventFetchEvents(FETCH_EVENT_BLOCK |
                                    FETCH_EVENT_NO_GL_CHECK);
        xGetButtonInput(); // Update global input
                    if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
                pdTRUE) {            
                            xSemaphoreTake(ScreenLock, portMAX_DELAY);
                            tumDrawSetGlobalXOffset(0);
                            tumDrawSetGlobalYOffset(0);
                            tumDrawClear(Black); // Clear screen
                            tumDrawLoadedImage(gamestart_image,90,20);
                            //showing the FPS in this state just felt weird 
                            //vDrawFPS();                    

                            //SCORE DRAWING

                                sprintf(strdtt,"PRESS S TO START");
                                tumDrawText(strdtt,240,
                                        360,
                                        Yellow);
                                sprintf(strdtt,"PRESS P TO PAUSE");
                                tumDrawText(strdtt,240,
                                        380,
                                        Yellow);
                                sprintf(strdtt,"PRESS A TO TOGGLE AI");
                                tumDrawText(strdtt,240,
                                        400,
                                        Yellow);
                                sprintf(strdtt,"PRESS D TO ADJUST AI DIFFICULTY");
                                tumDrawText(strdtt,240,
                                        420,
                                        Yellow);
                                sprintf(strdtt,"PRESS C TO OPEN CHEAT MODE");
                                tumDrawText(strdtt,240,
                                        440,
                                        Yellow);
                                


                            xSemaphoreGive(ScreenLock);
                            xSemaphoreGive(DrawSignal);
                }
        vCheckStateInput();
        vTaskDelay(20);  
    }
}

void vIntGodModeStateTask(){
    image_handle_t arrow_up_image =
    tumDrawLoadImage("../resources/images/arrow_up.png");        
    image_handle_t arrow_down_image =
    tumDrawLoadImage("../resources/images/arrow_down.png");  
    tumDrawSetLoadedImageScale(arrow_up_image,0.05);
    tumDrawSetLoadedImageScale(arrow_down_image,0.05);
    image_handle_t gamestart_image =
    tumDrawLoadImage("../resources/images/gamestart.png");        
    tumDrawSetLoadedImageScale(gamestart_image,0.5);    
    static char strdttt[40] = { 0 };
    if(xSemaphoreTake(spaceShipStruct.lock,portMAX_DELAY)==pdTRUE){
        spaceShipStruct.godmode=false;
        xSemaphoreGive(spaceShipStruct.lock);
    }
    while(1){
        tumEventFetchEvents(FETCH_EVENT_BLOCK |
                                    FETCH_EVENT_NO_GL_CHECK);
        xGetButtonInput(); // Update global input
                    if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
                pdTRUE) {            
                xSemaphoreTake(ScreenLock, portMAX_DELAY);
                tumDrawSetGlobalXOffset(0);
                tumDrawSetGlobalYOffset(0);
                tumDrawClear(Black); // Clear screen
                
                // USED FOR DEVELOPMENT PURPOSES
                /*sprintf(strdttt, "Axis 1: %5d | Axis 2: %5d", tumEventGetMouseX(),
                tumEventGetMouseY());
                tumDrawText(strdttt,10,10,Yellow);*/
                if(xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){ 
                    sprintf(strdttt, "INTENDED SCORE: %d",score.killscore);
                    tumDrawText(strdttt,130,365,Yellow);
                    sprintf(strdttt, "INTENDED LEVEL: %d",score.level);
                    tumDrawText(strdttt,450,365,Yellow);
                    xSemaphoreGive(score.lock);
                }
                tumDrawLoadedImage(gamestart_image,90,20);
                tumDrawLoadedImage(arrow_up_image,90,350);
                tumDrawLoadedImage(arrow_down_image,90,380);
                tumDrawLoadedImage(arrow_up_image,400,350);
                tumDrawLoadedImage(arrow_down_image,400,380);

                if(xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){    
                    if(tumEventGetMouseLeft() && (abs(tumEventGetMouseX()-100)<=15) && (abs(tumEventGetMouseY()-360)<=15) ){
                        score.killscore+=100;
                    }
                    if(tumEventGetMouseLeft() && (abs(tumEventGetMouseX()-100)<=15) && (abs(tumEventGetMouseY()-390)<=15) ){
                        score.killscore-=100;
                    }
                    if(tumEventGetMouseLeft() && (abs(tumEventGetMouseX()-410)<=15) && (abs(tumEventGetMouseY()-360)<=15) ){
                        score.level++;
                    }
                    if(tumEventGetMouseLeft() && (abs(tumEventGetMouseX()-410)<=15) && (abs(tumEventGetMouseY()-390)<=15) ){
                        score.level--;
                    }
                    xSemaphoreGive(score.lock);
                }
                sprintf(strdttt,"GOD MODE");
                tumDrawText(strdttt,350,430,Yellow);
                sprintf(strdttt,"PRESS S TO RETURN/START THE GAME ");
                tumDrawText(strdttt,50,455,Yellow);
                sprintf(strdttt,"PRESS G TO ACTIVATE GOD MODE ");
                tumDrawText(strdttt,350,455,Yellow);
                tumDrawCircle(450,440,13,Grey);
                if (xSemaphoreTake(spaceShipStruct.lock,portMAX_DELAY)==pdTRUE){
                    switch (spaceShipStruct.godmode){
                        case true:
                            tumDrawCircle(450,440,10,Green);
                            break;
                        case false:
                            tumDrawCircle(450,440,10,Red);
                            break;
                    }

                    if ((ButtonStateChangeCheck(KEYCODE(G))==true)){
                        spaceShipStruct.godmode=!spaceShipStruct.godmode;
                    }
                    
                    xSemaphoreGive(spaceShipStruct.lock);
                }
                xSemaphoreGive(ScreenLock);
                xSemaphoreGive(DrawSignal);
                }
        vCheckStateInput();
        vTaskDelay(20);
    }
}


void vDeathState(){
        image_handle_t gameover_image =
        tumDrawLoadImage("../resources/images/gameover.png");        
        tumDrawSetLoadedImageScale(gameover_image,0.5);
        static char strdtt[20] = { 0 };
    while(1){
            tumEventFetchEvents(FETCH_EVENT_BLOCK |
                                FETCH_EVENT_NO_GL_CHECK);
            xGetButtonInput(); // Update global input
            if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
                pdTRUE) {            
                xSemaphoreTake(ScreenLock, portMAX_DELAY);
                tumDrawSetGlobalXOffset(0);
                tumDrawSetGlobalYOffset(0);
                tumDrawClear(Black); // Clear screen
                tumDrawLoadedImage(gameover_image,170,100);
                //showing the FPS in this state just felt weird 
                //vDrawFPS();                    
                xSemaphoreTake(aliensHorizontalMotionStruct.lock,portMAX_DELAY);
                aliensHorizontalMotionStruct.excessMotionToggle=false;
                xSemaphoreGive(aliensHorizontalMotionStruct.lock);
                xSemaphoreGive(aliensCreate);
                xSemaphoreGive(shelterCreate);
                //SCORE DRAWING
                if (xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){
                    if (score.killscore>score.highscore){
                        score.highscore=score.killscore;
                    }
                    //reseting the killscore for if the user wants to play again
                    score.killscore=0;
                    sprintf(strdtt,"HIGHSCORE: %d",score.highscore);
                    tumDrawText(strdtt,260,
                              300,
                              Yellow);
                    xSemaphoreGive(score.lock);
                }
                sprintf(strdtt,"PRESS S TO REPLAY");
                    tumDrawText(strdtt,260,
                              320,
                              Yellow);

                xSemaphoreGive(ScreenLock);
                xSemaphoreGive(DrawSignal);
                }
        vCheckStateInput();        
        vTaskDelay(20);
    }
}
int main(int argc, char *argv[])
{
    char *bin_folder_path = tumUtilGetBinFolderPath(argv[0]);

    printf("Initializing: ");

    if (tumDrawInit(bin_folder_path)) {
        PRINT_ERROR("Failed to initialize drawing");
        goto err_init_drawing;
    }
    if (tumEventInit()) {
        PRINT_ERROR("Failed to initialize events");
        goto err_init_events;
    }
    if (tumSoundInit(bin_folder_path)) {
        PRINT_ERROR("Failed to initialize audio");
        goto err_init_audio;
    }
    HandleUDP = xSemaphoreCreateMutex();
    score.lock = xSemaphoreCreateMutex();
    for(int cx =0;cx<8;cx++){
        alienMissilesStruct[cx].lock=xSemaphoreCreateMutex();
    }
    aliensHorizontalMotionStruct.lock = xSemaphoreCreateMutex();
    buttons.lock = xSemaphoreCreateMutex(); // Locking mechanism
    for (int counter =0; counter <20;counter++){
    shelterBlocks[counter].lock=xSemaphoreCreateMutex();
    }
    for(int c1 =0 ; c1 < 5 ; c1++){
        for(int c2 = 0; c2 <8 ;c2++){
                aliens_1[c1][c2].lock = xSemaphoreCreateMutex();
        }
    }
    if (!buttons.lock) {
        PRINT_ERROR("Failed to create buttons lock");
        goto err_buttons_lock;
    }
    shelterCreate =xSemaphoreCreateBinary();
    aliensCreate = xSemaphoreCreateBinary();
    spaceShipStruct.lock = xSemaphoreCreateMutex();
    NextKeyQueue = xQueueCreate(1, sizeof(opponent_cmd_t));
    AttackStateQueue =xQueueCreate(1,sizeof(bool));
    DeltaPosQueue = xQueueCreate(5, sizeof(signed int));
    if (!NextKeyQueue) {
        exit(EXIT_FAILURE);
    }
    NextStateQueue = xQueueCreate(1, sizeof(bool));
    if (!NextStateQueue) {
        exit(EXIT_FAILURE);
    }
    DifficultyQueue = xQueueCreate(1, sizeof(unsigned char));
    if (!DifficultyQueue) {
        exit(EXIT_FAILURE);
    }

    StateQueue = xQueueCreate(STATE_QUEUE_LENGTH, sizeof(unsigned char));
    if (!StateQueue) {
        PRINT_ERROR("Could not open state queue");
        goto err_demotask;
    }

    if (xTaskCreate(basicSequentialStateMachine, "StateMachine",
                    mainGENERIC_STACK_SIZE * 2, NULL,
                    configMAX_PRIORITIES - 1, &StateMachine) != pdPASS) {
        goto err_demotask;
    }
    if (xTaskCreate(Drawing_Task, "Drawing_Task", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainGENERIC_PRIORITY, &DrawingTask_Handle) != pdPASS) {
        goto err_demotask;
    }
    xTaskCreate(collisionDetectionTask,"collision Detection Task", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainGENERIC_PRIORITY+3, &collisionDetectionTaskHandle);
    if (xTaskCreate(PositionIncrementation_Task, "Position Incrementation Task", mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &PositionIncrementationTask_Handle) != pdPASS) {
    goto err_demotask;
    }
    if (xTaskCreate(vPauseState, "Pause state task", mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &PauseTaskHandle) != pdPASS) {
    goto err_demotask;
    }

    if (xTaskCreate(vSwapBuffers, "BufferSwapTask",
                    mainGENERIC_STACK_SIZE * 2, NULL, configMAX_PRIORITIES-1,
                    BufferSwap) != pdPASS) {
    }
    xTaskCreate(shelterCreatingTask,"task that creates a shelter",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &shelterCreatingTaskHandle);
    xTaskCreate(LevelIncreasingTask,"task that increases the level",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &LevelIncreasingTaskHandle);
    xTaskCreate(alienCreationTask,"task that creates aliens",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &alienCreationTaskHandle);   
    xTaskCreate(AliensMovingOneTask,"task that moves aliens 1",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &aliensMovingOneTaskHandle);  
    xTaskCreate(AliensMovingTwoTask,"task that moves aliens 1",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &aliensMovingTwoTaskHandle); 
    xTaskCreate(AlienShootingTask,"task that moves aliens 1",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &AlienShootingTaskHandle); 
    xTaskCreate(AlienMissiletrackingTask,"task that tracks alien missiles",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &AlienMissiletrackingTaskHandle);
    xTaskCreate(vDeathState,"death state task",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &deathStateTaskHandle);
    xTaskCreate(vInitialState,"initial start screen state task",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &initialStateHandle);
    xTaskCreate(vAlienDescentTask,"task that manages the descent of the aliens",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &alienDescentTaskHandle);   
    xTaskCreate(vIntGodModeStateTask,"task that manages the user settings of god mode",mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &intGodModeStateHandle);     
    xTaskCreate(vUDPControlTask, "UDPControlTask", mainGENERIC_STACK_SIZE, NULL, mainGENERIC_PRIORITY,&UDPControlTask);            
    xTaskCreate(vUDPInputCheckingTask, "UDP Input checking Task", mainGENERIC_STACK_SIZE, NULL, mainGENERIC_PRIORITY,&UDPInputCheckingTaskHandle);            
    DrawSignal = xSemaphoreCreateBinary(); // Screen buffer locking
    if (!DrawSignal) {
        PRINT_ERROR("Failed to create draw signal");
    }
    ScreenLock = xSemaphoreCreateMutex();
    if (!ScreenLock) {
        PRINT_ERROR("Failed to create screen lock");
    }
    RightButtonSignal = xSemaphoreCreateBinary();
    SpaceShipMissileSem = xSemaphoreCreateMutex();
    vTaskStartScheduler();

    return EXIT_SUCCESS;

err_demotask:
    vSemaphoreDelete(buttons.lock);
err_buttons_lock:
    tumSoundExit();
err_init_audio:
    tumEventExit();
err_init_events:
    tumDrawExit();
err_init_drawing:
    return EXIT_FAILURE;
}

// cppcheck-suppress unusedFunction
__attribute__((unused)) void vMainQueueSendPassed(void)
{
    /* This is just an example implementation of the "queue send" trace hook. */
}

// cppcheck-suppress unusedFunction
__attribute__((unused)) void vApplicationIdleHook(void)
{
#ifdef __GCC_POSIX__
    struct timespec xTimeToSleep, xTimeSlept;
    /* Makes the process more agreeable when using the Posix simulator. */
    xTimeToSleep.tv_sec = 1;
    xTimeToSleep.tv_nsec = 0;
    nanosleep(&xTimeToSleep, &xTimeSlept);
#endif
} 