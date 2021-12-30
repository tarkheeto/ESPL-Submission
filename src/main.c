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

#define NEXT_TASK 0
#define PREV_TASK 1



#define STARTING_STATE STATE_ONE

#define STATE_DEBOUNCE_DELAY 300
SemaphoreHandle_t horizontalAlienMotion=NULL;
typedef struct score_t{int score;
int level;
SemaphoreHandle_t lock;}score_t;
score_t score;
TaskHandle_t aliensMovingOneTaskHandle=NULL;
TaskHandle_t aliensMovingTwoTaskHandle=NULL;
SemaphoreHandle_t aliensCreate=NULL;
SemaphoreHandle_t shelterCreate = NULL;
TaskHandle_t alienCreationTaskHandle=NULL;
TaskHandle_t collisionDetectionTaskHandle=NULL;
int debugVar = 0;
static TaskHandle_t LeftNumber = NULL;
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
    int posY;
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
        SemaphoreHandle_t lock;
}shelterblock_t;
shelterblock_t shelterBlocks[20];
typedef struct spaceShipStruct_t
{
    bool attackState; // IF false then passive -> we can shoot
    int spaceShipMissileX;
    int spaceShipMissileY;
    int mothershipXPosition;
    int mothershipYPosition;  
    SemaphoreHandle_t lock;
}spaceShipStruct_t;
spaceShipStruct_t spaceShipStruct;
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
static TaskHandle_t LeftCircleHandle = NULL;
static TaskHandle_t RightCircleHandle = NULL;
static TaskHandle_t DrawingTask_Handle = NULL;
static TaskHandle_t PositionIncrementationTask_Handle = NULL;
static TaskHandle_t BufferSwap = NULL;
static TaskHandle_t Exercise3 = NULL;
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
        if (buttons.buttons[KEYCODE(E)]) {
            buttons.buttons[KEYCODE(E)] = 0;
            if (StateQueue) {
                xSemaphoreGive(buttons.lock);
                xQueueSend(StateQueue, &next_state_signal, 0);
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
        case NEXT_TASK:
            if (*state == STATE_COUNT - 1) {
                *state = 0;
            }
            else {
                (*state)++;
            }
            break;
        case PREV_TASK:
            if (*state == 0) {
                *state = STATE_COUNT - 1;
            }
            else {
                (*state)--;
            }
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
                case STATE_ONE:
                    if (Exercise3) {
                        vTaskSuspend(Exercise3);
                    }
                    if (PositionIncrementationTask_Handle) {
                        vTaskResume(PositionIncrementationTask_Handle);
                    }
                    if (DrawingTask_Handle) {
                        vTaskResume(DrawingTask_Handle);
                    }
                    break;
                case STATE_TWO:
                    if (DrawingTask_Handle) {
                        vTaskSuspend(DrawingTask_Handle);
                    }
                    if (PositionIncrementationTask_Handle) {
                        vTaskSuspend(PositionIncrementationTask_Handle);
                    }
                    if (Exercise3) {
                        vTaskResume(Exercise3);
                    }
                    break;
                default:
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

        static char strdt[10] = { 0 };
               
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
                    sprintf(strdt,"Score: %d",score.score);
                    tumDrawText(strdt,10,
                              DEFAULT_FONT_SIZE * 1,
                              Yellow);
                    xSemaphoreGive(score.lock);
                }


                //Drawing the shelter blocks
                for (int counter =0; counter <20;counter++){
                    if (xSemaphoreTake(shelterBlocks[counter].lock,0)==pdTRUE){
                        if(shelterBlocks[counter].alive==true){
                            tumDrawFilledBox(shelterBlocks[counter].posX,shelterBlocks[counter].posY,25,2,White);
                        }
                        xSemaphoreGive(shelterBlocks[counter].lock);
                    }
                }   

                //Drawing Missiles fired by our mother ship and the ship itself
                if (xSemaphoreTake(spaceShipStruct.lock,0)==pdTRUE){	
                    tumDrawLoadedImage(ball_spritesheet_image,spaceShipStruct.mothershipXPosition,
                                        spaceShipStruct.mothershipYPosition);                  
                        if (spaceShipStruct.attackState){
                            tumDrawFilledBox(spaceShipStruct.spaceShipMissileX+10,spaceShipStruct.spaceShipMissileY,3,5,Red);
                        }
                    xSemaphoreGive(spaceShipStruct.lock);
                }

                //Drawing the first type of aliens 
                for (int c1=0;c1<5;c1++){
                    for(int c2=0;c2<8;c2++){
                        if(xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                            if(aliens_1[c1][c2].alive) {
                                switch(c1){ 
                                case 0:    
                                tumDrawSetLoadedImageScale(alien1_image,0.06);    
                                tumDrawLoadedImage(alien1_image,aliens_1[c1][c2].posX,aliens_1[c1][c2].posY);
                                break;

                                case 1:
                                tumDrawSetLoadedImageScale(alien2_image,0.06);    
                                tumDrawLoadedImage(alien2_image,aliens_1[c1][c2].posX,aliens_1[c1][c2].posY);
                                break;

                                case 2:
                                tumDrawSetLoadedImageScale(alien3_image,0.08);    
                                tumDrawLoadedImage(alien3_image,aliens_1[c1][c2].posX,aliens_1[c1][c2].posY);    
                                break;

                                case 3:
                                tumDrawSetLoadedImageScale(alien4_image,0.07);    
                                tumDrawLoadedImage(alien4_image,aliens_1[c1][c2].posX,aliens_1[c1][c2].posY);    
                                break;

                                case 4:
                                tumDrawSetLoadedImageScale(alien5_image,0.09);    
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
if (xSemaphoreTake(spaceShipStruct.lock,0)==pdTRUE){
spaceShipStruct.attackState=false;    
spaceShipStruct.mothershipXPosition=310;
spaceShipStruct.mothershipYPosition=400;   
xSemaphoreGive(spaceShipStruct.lock);
    }

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
                        if(spaceShipStruct.mothershipXPosition<=560){ 
                         spaceShipStruct.mothershipXPosition+=6;
                    }}
                    if (buttons.buttons[KEYCODE(LEFT)]) { 
                        if(spaceShipStruct.mothershipXPosition>=10){ 
                         spaceShipStruct.mothershipXPosition-=6;
                    }}
                    xSemaphoreGive(buttons.lock);
                }
                
                if (ButtonStateChangeCheck(KEYCODE(SPACE))==true){
                        vSpaceshipShoot();}  
                if (ButtonStateChangeCheck(KEYCODE(O))==true){
                    
                        xSemaphoreGive(shelterCreate);}      
                if (spaceShipStruct.attackState){
                    spaceShipStruct.spaceShipMissileY-=10;
                    if (spaceShipStruct.spaceShipMissileY<=0){spaceShipStruct.attackState=false;}
                }
                xSemaphoreGive(spaceShipStruct.lock);
                }


                vTaskDelay((TickType_t)20);
            }
            vCheckStateInput();

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
                    if (abs(spaceShipStruct.spaceShipMissileY- shelterBlocks[counter].posY)<4 && abs(spaceShipStruct.spaceShipMissileX -shelterBlocks[counter].posX)<13 && 
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
                            aliens_1[c1][c2].alive) {
                                aliens_1[c1][c2].alive=false;
                                spaceShipStruct.attackState=false;
                                if (xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){
                                    score.score++;
                                    xSemaphoreGive(score.lock);
                                }
                            }
                        xSemaphoreGive(aliens_1[c1][c2].lock);
                    }
                }
            }

        xSemaphoreGive(spaceShipStruct.lock);
        } 
        
        vTaskDelay((TickType_t)20);

    }
}

void alienCreationTask(){
    // Score initialisation
    if (xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){
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
                                aliens_1[c1][c2].posX= c2*60;
                                aliens_1[c1][c2].posY=80;
                                break;
                                case 1:
                                aliens_1[c1][c2].posX= 180+c2*60;
                                aliens_1[c1][c2].posY=120;
                                break;
                                case 2:
                                aliens_1[c1][c2].posX= c2*60;
                                aliens_1[c1][c2].posY=160;
                                break;
                                case 3:
                                aliens_1[c1][c2].posX= 180+c2*60;
                                aliens_1[c1][c2].posY=200;
                                break;
                                case 4:
                                aliens_1[c1][c2].posX= c2*60;
                                aliens_1[c1][c2].posY=240;
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
                                aliens_1[c1][c2].posX= c2*60;
                                aliens_1[c1][c2].posY=80;
                                break;
                                case 1:
                                aliens_1[c1][c2].posX= 180+c2*60;
                                aliens_1[c1][c2].posY=120;
                                break;
                                case 2:
                                aliens_1[c1][c2].posX= c2*60;
                                aliens_1[c1][c2].posY=160;
                                break;
                                case 3:
                                aliens_1[c1][c2].posX= 180+c2*60;
                                aliens_1[c1][c2].posY=200;
                                break;
                                case 4:
                                aliens_1[c1][c2].posX= c2*60;
                                aliens_1[c1][c2].posY=240;
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

void LevelIncreasingTask(){
    while(1){
        //Check if we reached a multiple of 50 so that we could re draw the aliens and later on change their speeds accordingly
        if (xSemaphoreTake(score.lock,portMAX_DELAY)==pdTRUE){
            if (score.score%40==0 && score.score != 0){
                score.level++;
                xSemaphoreGive(aliensCreate);
                xSemaphoreGive(shelterCreate);
            }
            xSemaphoreGive(score.lock);
        }
        vTaskDelay((TickType_t)20);
    }
}
typedef struct aliensHorizontalMotionStruct_t{
    int posXCounter;
    bool motiontoggle;
    SemaphoreHandle_t lock;
}aliensHorizontalMotionStruct_t;
aliensHorizontalMotionStruct_t aliensHorizontalMotionStruct;

void AliensMovingOneTask(){
    int lvl;
    if (xSemaphoreTake(aliensHorizontalMotionStruct.lock,portMAX_DELAY)==pdTRUE){
        aliensHorizontalMotionStruct.posXCounter=0;
        aliensHorizontalMotionStruct.motiontoggle=false;
        xSemaphoreGive(aliensHorizontalMotionStruct.lock);
    }
    while(1){
       if(xSemaphoreTake(aliensHorizontalMotionStruct.lock,portMAX_DELAY)==pdTRUE){ 
           if(aliensHorizontalMotionStruct.motiontoggle==false){ 
                for (int c1 =0;c1<5;c1++){
                    for(int c2 = 0; c2<8; c2++){
                        if (xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                            switch (c1) {
                                case 0: aliens_1[c1][c2].posX+=1 ;
                                        break;
                                case 1:aliens_1[c1][c2].posX-=1 ;
                                       break;
                                case 2:aliens_1[c1][c2].posX+=1 ;
                                       break;
                                case 3:aliens_1[c1][c2].posX-=1 ;
                                       break;
                                case 4:aliens_1[c1][c2].posX+=1 ;
                                       break;
                            }
                            //aliens_1[c1][c2].posX+=1 ;
                            xSemaphoreGive(aliens_1[c1][c2].lock);
                        }
                        
                    }
                }
                aliensHorizontalMotionStruct.posXCounter +=1 ;
                if (aliensHorizontalMotionStruct.posXCounter >=160){
                    aliensHorizontalMotionStruct.posXCounter=0;
                    aliensHorizontalMotionStruct.motiontoggle=true;
                }
            }
            xSemaphoreGive(aliensHorizontalMotionStruct.lock);
        }
        vTaskDelay((TickType_t)20);
    }       
}

void AliensMovingTwoTask(){
int lvl;
    while(1){
       if(xSemaphoreTake(aliensHorizontalMotionStruct.lock,portMAX_DELAY)==pdTRUE){ 
           if(aliensHorizontalMotionStruct.motiontoggle==true){ 
                for (int c1 =0;c1<5;c1++){
                    for(int c2 = 0; c2<8; c2++){
                        if (xSemaphoreTake(aliens_1[c1][c2].lock,portMAX_DELAY)==pdTRUE){
                            switch (c1) {
                                case 0: aliens_1[c1][c2].posX-=1 ;
                                        break;
                                case 1:aliens_1[c1][c2].posX+=1 ;
                                       break;
                                case 2:aliens_1[c1][c2].posX-=1 ;
                                       break;
                                case 3:aliens_1[c1][c2].posX+=1 ;
                                       break;
                                case 4:aliens_1[c1][c2].posX-=1 ;
                                       break;
                            }
                            
                            xSemaphoreGive(aliens_1[c1][c2].lock);
                        }
                    }
                }
                aliensHorizontalMotionStruct.posXCounter +=1;
                if (aliensHorizontalMotionStruct.posXCounter >=160){
                    aliensHorizontalMotionStruct.posXCounter=0;
                    aliensHorizontalMotionStruct.motiontoggle=false;
                }
            }
            xSemaphoreGive(aliensHorizontalMotionStruct.lock);
        }
        vTaskDelay((TickType_t)20);
    }       
}

#define LBI 0x01
int debugvarr222 =0;
void vLeftNumber(void *pvParameters){
uint32_t NotificationBuffer;
    while(1){
        NotificationBuffer = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (NotificationBuffer | LBI){
            /*if(xSemaphoreTake(Exercise3VariableIncrementationStruct.lock, portMAX_DELAY) == pdTRUE){
                Exercise3VariableIncrementationStruct.LeftNumber++;
                xSemaphoreGive(Exercise3VariableIncrementationStruct.lock);*/ 
            }
        }
        
    }
//}
void vRightNumber(void *pvParameters){

    while(1){
        if (xSemaphoreTake(RightButtonSignal,portMAX_DELAY) == pdTRUE){
            if(xSemaphoreTake(Exercise3VariableIncrementationStruct.lock, portMAX_DELAY) == pdTRUE){
                Exercise3VariableIncrementationStruct.RightNumber++;
                xSemaphoreGive(Exercise3VariableIncrementationStruct.lock);
            }
        }
    }
}
#define toggleRightCircle    0x0001 //Bit 0 toggles the right circle
#define toggleLeftCircle   0x0002   //Bit 1 roggles the left cicle
int vLeftCircleDebug = 0;
void vLeftCircle(void *pvParameters){
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(500);
    //xLastWakeTime = xTaskGetTickCount();
    while(1){
        xLastWakeTime = xTaskGetTickCount();
        xTaskNotify(Exercise3, toggleLeftCircle, eSetBits);   //Tell the drawing task to toggle the right circle
        vLeftCircleDebug++;
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
void vRightCircle(void *pvParameters)
{
TickType_t xLastWakeTime;
const TickType_t xFrequency = pdMS_TO_TICKS(250);
    while(1){

         xLastWakeTime = xTaskGetTickCount();
        xTaskNotify(Exercise3, toggleRightCircle, eSetBits);   //Tell the drawing task to toggle the right circle
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        //printf("toggled right circle!\n");
    }
}

int debugVarrunningcheck=0;
void vExercise3(void *pvParameters){
    uint32_t NotificationBuffer;
    bool RightCircleFlag = false;
    bool LeftCircleFlag = false;
 
    while (1)
    {
    tumEventFetchEvents(FETCH_EVENT_BLOCK |
                                FETCH_EVENT_NO_GL_CHECK);
    xGetButtonInput(); // Update global input
    NotificationBuffer = ulTaskNotifyTake(pdTRUE, 0);    //Recieve whatever is in the queue and clear it    
    if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
                pdTRUE) {            
                xSemaphoreTake(ScreenLock, portMAX_DELAY);
                tumDrawSetGlobalXOffset(0);
                tumDrawSetGlobalYOffset(0);
                tumDrawClear(White); // Clear screen
                    if(NotificationBuffer & toggleRightCircle){         //Toggle the Corrosponding Circle if it's time
                        RightCircleFlag = !RightCircleFlag;
                    }
                
                    if(NotificationBuffer & toggleLeftCircle){   
                        LeftCircleFlag = !LeftCircleFlag;
                    }
                    if (LeftCircleFlag){
                        tumDrawCircle(200,200,50,Red);
                    }
                    if (RightCircleFlag){
                        tumDrawCircle(400,200,50,Blue);
                    }
                    //if (ButtonStateChangeCheck(KEYCODE(V))==true){xTaskNotify(LeftNumber,LEFT_BUTTON_INCREMENT,eSetBits);}
                vDrawFPS();                    
                xSemaphoreGive(ScreenLock);
                    /*if (xSemaphoreTake(buttons.lock, 0) == pdTRUE) {
                        if (buttons.buttons[KEYCODE(
                                                Q)]) { // Equiv to SDL_SCANCODE_Q
                            exit(EXIT_SUCCESS);
                        }
                    xSemaphoreGive(buttons.lock);

                    }*/
            }
                   
    NotificationBuffer = ulTaskNotifyTake(pdTRUE, 0);    //Recieve whatever is in the queue and clear it

    if(NotificationBuffer & toggleRightCircle){         //Toggle the Corrosponding Circle if it's time
        RightCircleFlag = !RightCircleFlag;
    }

    if(NotificationBuffer & toggleLeftCircle){   
        LeftCircleFlag = !LeftCircleFlag;
        
    }
    xSemaphoreGive(DrawSignal);
    debugVarrunningcheck++;

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
    score.lock = xSemaphoreCreateMutex();
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
                    mainGENERIC_PRIORITY, &collisionDetectionTaskHandle);
    if (xTaskCreate(PositionIncrementation_Task, "Position Incrementation Task", mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &PositionIncrementationTask_Handle) != pdPASS) {
    goto err_demotask;
    }
    if (xTaskCreate(vExercise3, "Exercise 3 main task", mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &Exercise3) != pdPASS) {
    goto err_demotask;
    }
    LeftCircleHandle= xTaskCreateStatic(vLeftCircle, "Left Circle", STACK_SIZE, NULL,
                    mainGENERIC_PRIORITY+1, LeftCircleStack,&LeftCircleBuffer); 

    if (xTaskCreate(vRightCircle, "Right Circle Task", mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY, &RightCircleHandle) != pdPASS) {
    goto err_demotask;
    }
    if (xTaskCreate(vLeftNumber, "LeftNumb Ex3", mainGENERIC_STACK_SIZE * 2, NULL,
                mainGENERIC_PRIORITY+1, &LeftNumber) != pdPASS) {
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
// GITHUB TOKEN: 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