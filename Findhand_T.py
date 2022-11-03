import cv2
import mediapipe as mp
import time

cap = cv2.VideoCapture(0)
mpHands = mp.solutions.hands
hands = mpHands.Hands(
               static_image_mode=False,
               max_num_hands=2,
               model_complexity=1,
               min_detection_confidence=0.5,
               min_tracking_confidence=0.5)
mpDraw = mp.solutions.drawing_utils
handLmsStyle = mpDraw.DrawingSpec(color=(0,0,255),thickness=3)
handConStyle = mpDraw.DrawingSpec(color=(0,255,0),thickness=2)#使用模型#使用模型
pTime=0
cTime=0

while True:
    ret,img = cap.read()
    if ret:
        imgRGB = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
        result = hands.process(imgRGB)
        #print(result.multi_hand_landmarks)
        imgHeight = img.shape[0]
        imgWidth = img.shape[1]

        if result.multi_hand_landmarks:
            for handLms in result.multi_hand_landmarks:
                mpDraw.draw_landmarks(img,handLms,mpHands.HAND_CONNECTIONS,handLmsStyle,handConStyle)
                for i,lm in enumerate(handLms.landmark):
                    xPos = int(lm.x*imgWidth)
                    yPos = int(lm.y*imgHeight)
                    if i==4:
                        cv2.circle(img,(xPos,yPos),10,(0,256,256),cv2.FILLED)
                    cv2.putText(img,str(i),(xPos-25,yPos+5),cv2.FONT_HERSHEY_SIMPLEX,0.5,(255,0,0),2)
                    print(i,xPos,yPos)
        cTime = time.time()
        fps=1/(cTime-pTime)
        pTime=cTime
        cv2.putText(img,f"FPS:{int(fps)}",(30,50),cv2.FONT_HERSHEY_SIMPLEX,1,(255,0,0),2)

        cv2.imshow('img',img)

    if cv2.waitKey(1) == ord('q'):
        break