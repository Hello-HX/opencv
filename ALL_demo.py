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

#角度判断
def get_angle(v1, v2):
    angle = np.dot(v1, v2) / (np.sqrt(np.sum(v1 * v1)) * np.sqrt(np.sum(v2 * v2)))
    angle = np.arccos(angle) / 3.14 * 180

    return angle
##########################################

#手势判读
def get_str_guester(up_fingers, list_lms):
    if len(up_fingers) == 1 and up_fingers[0] == 8:

        v1 = list_lms[6] - list_lms[7]
        v2 = list_lms[8] - list_lms[7]

        angle = get_angle(v1, v2)

        if angle < 160:
            str_guester = "9"
        else:
            str_guester = "1"

    elif len(up_fingers) == 1 and up_fingers[0] == 4:
        str_guester = "Good"

    elif len(up_fingers) == 1 and up_fingers[0] == 20:
        str_guester = "Bad"

    elif len(up_fingers) == 1 and up_fingers[0] == 12:
        str_guester = "FXXX"

    elif len(up_fingers) == 2 and up_fingers[0] == 8 and up_fingers[1] == 12:
        str_guester = "2"

    elif len(up_fingers) == 2 and up_fingers[0] == 4 and up_fingers[1] == 20:
        str_guester = "6"

    elif len(up_fingers) == 2 and up_fingers[0] == 4 and up_fingers[1] == 8:
        str_guester = "8"

    elif len(up_fingers) == 3 and up_fingers[0] == 8 and up_fingers[1] == 12 and up_fingers[2] == 16:
        str_guester = "3"

    elif len(up_fingers) == 3 and up_fingers[0] == 4 and up_fingers[1] == 8 and up_fingers[2] == 12:

        dis_8_12 = list_lms[8, :] - list_lms[12, :]
        dis_8_12 = np.sqrt(np.dot(dis_8_12, dis_8_12))

        dis_4_12 = list_lms[4, :] - list_lms[12, :]
        dis_4_12 = np.sqrt(np.dot(dis_4_12, dis_4_12))

        if dis_4_12 / (dis_8_12 + 1) < 3:
            str_guester = "7"

        elif dis_4_12 / (dis_8_12 + 1) > 5:
            str_guester = "Gun"
        else:
            str_guester = "7"

    elif len(up_fingers) == 3 and up_fingers[0] == 4 and up_fingers[1] == 8 and up_fingers[2] == 20:
        str_guester = "ROCK"

    elif len(up_fingers) == 4 and up_fingers[0] == 8 and up_fingers[1] == 12 and up_fingers[2] == 16 and up_fingers[
        3] == 20:
        str_guester = "4"

    elif len(up_fingers) == 5:
        str_guester = "5"

    elif len(up_fingers) == 0:
        str_guester = "10"

    else:
        str_guester = " "

    return str_guester
################################################

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
wCam, hCam = 1280, 640#屏幕大小
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
    #cv2.namedWindow("img", cv2.WINDOW_NORMAL)
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
                list_lms = []
                #开始遍历点
                for i,lm in enumerate(handLms.landmark):
                    #比例换算与存坐标点
                    xPos = int(lm.x*imgWidth)
                    yPos = int(lm.y*imgHeight)
                    xList.append(xPos)
                    yList.append(yPos)
                    list_lms.append([int(xPos), int(yPos)])

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

              #手势判断模块
                #构造凸包
                list_lms = np.array(list_lms, dtype=np.int32)
                hull_index = [0, 1, 2, 3, 6, 10, 14, 19, 18, 17, 10]
                hull = cv2.convexHull(list_lms[hull_index, :])

                # 绘制凸包
                cv2.polylines(img, [hull], True, (0, 255, 0), 2)

                # 查找外部的点数
                n_fig = -1
                ll = [4, 8, 12, 16, 20]
                up_fingers = []
                for i in ll:
                    pt = (int(list_lms[i][0]), int(list_lms[i][1]))
                    dist = cv2.pointPolygonTest(hull, pt, True)
                    if dist < 0:
                        up_fingers.append(i)

                #回应手势
                str_guester = get_str_guester(up_fingers, list_lms)
                cv2.putText(img, ' %s' % (str_guester), (500, 90), cv2.FONT_HERSHEY_SIMPLEX, 3, (255, 255, 0), 4,
                            cv2.LINE_AA)
                ################################################################

                #拇指食指关键点用于操控音量
                x1, y1 = xList[4],yList[4]
                x2, y2 = xList[8],yList[8]
                cx, cy = (x1 + x2) // 2, (y1 + y2) // 2
                cv2.circle(img, (x1, y1), 15, (113, 227, 250), cv2.FILLED)
                cv2.circle(img, (x2, y2), 15, (113, 227, 250), cv2.FILLED)
                cv2.line(img, (x1, y1), (x2, y2), (173, 202, 25), 3)
                cv2.circle(img, (cx, cy), 15, (108, 96, 244), cv2.FILLED)
                length = math.hypot(x2 - x1, y2 - y1)

                """""
                #一维线性换算出音量值
                # Hand range 50 - 300
                # Volume Range -96 - 0
                vol = np.interp(length, [50, 300], [minVol, maxVol])
                volBar = np.interp(length, [50, 300], [400, 150])  #音量水准
                volPer = np.interp(length, [50, 300], [0, 100])    #音量百分率
                print(int(length), vol)
                if str_guester=='5':
                 volume.SetMasterVolumeLevel(vol, None)
                 """
                #小于触发条件
                if length < 50:
                    cv2.circle(img, (cx, cy), 15, (116, 186, 209), cv2.FILLED)
                """""
                #音量框图
                cv2.rectangle(img, (50, 150), (85, 400), (173, 202, 25), 3)
                if str_guester == '5':
                 cv2.rectangle(img, (50, int(volBar)), (85, 400), (173, 202, 25), cv2.FILLED)
                 cv2.putText(img, f'{int(volPer)} %', (40, 450), cv2.FONT_HERSHEY_COMPLEX,
                            1, (173, 202, 25), 3)
                """
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

cap.release()

