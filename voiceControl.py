#引入一些库
import cv2
import time
import numpy as np
import math
import mediapipe as mp
from ctypes import cast, POINTER
from comtypes import CLSCTX_ALL
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
############################################################################

#导入手部模型
cap = cv2.VideoCapture(0)
mpHands = mp.solutions.hands
hands = mpHands.Hands(
               static_image_mode=False,
               max_num_hands=2,
               model_complexity=1,
               min_detection_confidence=0.5,
               min_tracking_confidence=0.5)
mpDraw = mp.solutions.drawing_utils
handLmsStyle = mpDraw.DrawingSpec(color=(101,67,254),thickness=5)
handConStyle = mpDraw.DrawingSpec(color=(247,245,255),thickness=3)#使用模型#使用模型
####################################################################################

#视频参数
pTime=0
cTime=0
#640,480
wCam, hCam = 1200, 640#屏幕大小
cap.set(3, wCam)
cap.set(4, hCam)
###################################################################################

#音频模块
devices = AudioUtilities.GetSpeakers()
interface = devices.Activate(
    IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
volume = cast(interface, POINTER(IAudioEndpointVolume))
# volume.GetMute()
# volume.GetMasterVolumeLevel()
volRange = volume.GetVolumeRange()
minVol = volRange[0]
maxVol = volRange[1]
vol = 0
volBar = 400
volPer = 0
#print(volRange)
################################################################################

#执行模块
while True:
    #读取图片
    success,img = cap.read()
    img= cv2.flip(img, 1)
    #########################

    if success:
        #记录图像参数
        imgRGB = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
        result = hands.process(imgRGB)
        #print(result.multi_hand_landmarks)
        #图像的宽度与高度
        imgHeight = img.shape[0]
        imgWidth = img.shape[1]
        imgZidth = img.shape[2]
        #########################################

        #操控骨骼点
        if result.multi_hand_landmarks:
            #遍历骨骼点
            for handLms in result.multi_hand_landmarks:
                mpDraw.draw_landmarks(img,handLms,mpHands.HAND_CONNECTIONS,handLmsStyle,handConStyle)
                xList = []#用于存点
                yList = []
                bbox = []
                #开始遍历点
                for i,lm in enumerate(handLms.landmark):
                    #比例换算与存坐标点
                    xPos = int(lm.x*imgWidth)
                    yPos = int(lm.y*imgHeight)
                    xList.append(xPos)
                    yList.append(yPos)

                    #绘制拇指与中指的关键点
                    if i==4:
                        cv2.circle(img,(xPos,yPos),10,(113,227,250),cv2.FILLED)
                    if i==8:
                        cv2.circle(img,(xPos,yPos),10,(113,227,250),cv2.FILLED)

                    #保存边界点用于画矩形
                    xmin, xmax = min(xList), max(xList)
                    ymin, ymax = min(yList), max(yList)
                    bbox = xmin, ymin, xmax, ymax

                    #标记点
                    #cv2.putText(img,str(i),(xPos-25,yPos+5),cv2.FONT_HERSHEY_SIMPLEX,0.5,(60,44,23),2)
                    print(i,xPos,yPos)

                cv2.rectangle(img, (bbox[0] - 20, bbox[1] - 20), (bbox[2] + 20, bbox[3] + 20), (101, 67, 254), 2)
              ##################################################################

                #拇指食指关键点用于操控音量
                x1, y1 = xList[4],yList[4]
                x2, y2 = xList[8],yList[8]
                cx, cy = (x1 + x2) // 2, (y1 + y2) // 2
                cv2.circle(img, (x1, y1), 15, (113, 227, 250), cv2.FILLED)
                cv2.circle(img, (x2, y2), 15, (113, 227, 250), cv2.FILLED)
                cv2.line(img, (x1, y1), (x2, y2), (173, 202, 25), 3)
                cv2.circle(img, (cx, cy), 15, (108, 96, 244), cv2.FILLED)
                length = math.hypot(x2 - x1, y2 - y1)

                #一维线性换算出音量值
                # Hand range 50 - 300
                # Volume Range -96 - 0
                vol = np.interp(length, [50, 300], [minVol, maxVol])
                volBar = np.interp(length, [50, 300], [400, 150])  #音量水准
                volPer = np.interp(length, [50, 300], [0, 100])    #音量百分率
                print(int(length), vol)
                volume.SetMasterVolumeLevel(vol, None)

                #小于触发条件
                if length < 50:
                    cv2.circle(img, (cx, cy), 15, (116, 186, 209), cv2.FILLED)

                #音量框图
                cv2.rectangle(img, (50, 150), (85, 400), (173, 202, 25), 3)
                cv2.rectangle(img, (50, int(volBar)), (85, 400), (173, 202, 25), cv2.FILLED)
                cv2.putText(img, f'{int(volPer)} %', (40, 450), cv2.FONT_HERSHEY_COMPLEX,
                            1, (173, 202, 25), 3)
            ######################################################################################


        #帧数计算
        cTime = time.time()
        fps=1/(cTime-pTime)
        pTime=cTime
        cv2.putText(img,f"FPS:{int(fps)}",(30,50),cv2.FONT_HERSHEY_SIMPLEX,1,(173,202,25),2)

        cv2.imshow('img',img)
        #############################################################################################

    #退出机制
    if cv2.waitKey(1) == ord('q'):
        break