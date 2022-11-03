import cv2
import mediapipe as mp
import math


class HandDetector:
    """
    利用mediapipe寻找手， 得到手部关键点坐标. 能够检测出多少只手指是伸张的
    以及两个手指指尖的距离 ，对检测到的手计算它的锚框.
    """
    #对象属性
    def __init__(self, mode=False, maxHands=2, detectionCon=0.5, minTrackCon=0.5):
        """
        :param mode: 在静态模式会对没一张图片进行检测：比较慢
        :param maxHands: 检测到手的最大个数
        :param detectionCon: 最小检测阈值
        :param minTrackCon: 最小追踪阈值
        """
        self.mode = mode
        self.maxHands = maxHands
        self.detectionCon = detectionCon
        self.minTrackCon = minTrackCon

        self.mpHands = mp.solutions.hands
        self.hands = self.mpHands.Hands(static_image_mode=self.mode, max_num_hands=self.maxHands,
                                        min_detection_confidence=self.detectionCon,
                                        min_tracking_confidence=self.minTrackCon)
        self.mpDraw = mp.solutions.drawing_utils
        self.tipIds = [4, 8, 12, 16, 20]  # 从大拇指开始，依次为每个手指指尖
        self.fingers = []
        self.lmList = []
        self.handLmsStyle = self.mpDraw.DrawingSpec(color=(101, 67, 254), thickness=6)
        self.handConStyle = self.mpDraw.DrawingSpec(color=(247, 245, 255), thickness=5)

    #手部检测与轮廓绘制
    def findHands(self, img, draw=True, flipType=True):
        """
        Finds hands in a BGR image.
        :param img: Image to find the hands in.
        :param draw: Flag to draw the output on the image.
        :return: Image with or without drawings
        """

        #手部检测 Image to find the hands in
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        self.results = self.hands.process(imgRGB)

        allHands = []   #用来存手势的类型，用于返回
        h, w, c = img.shape
        # print("multi_hand_landmarks")
        # print(self.results.multi_hand_landmarks)
        #print("self.results.multi_handedness")
        #print(self.results.multi_handedness)
        # self.results.multi_hand_landmarks为21个landmark,21个关键点信息
        # landmark
        # {
        #     x: 0.08329735696315765
        #     y: 0.861643373966217
        #     z: 8.069470709415327e-07
        # }

        # self.results.multi_handedness"
        # [classification {
        #     index: 0   代表手的索引号，第几个手掌
        #     score: 0.5836966037750244
        #     label: "Left"
        # }
        #  ]


        #绘制与坐标FLag to draw the output on the image
        if self.results.multi_hand_landmarks:
            for handType, handLms in zip(self.results.multi_handedness, self.results.multi_hand_landmarks):
                myHand = {}
                ## lmList
                mylmList = [] #存各个节点数据
                xList = []  #存各个节点的X
                yList = []  #存各个节点的Y
                for id, lm in enumerate(handLms.landmark):
                    #坐标换算
                    px, py, pz = int(lm.x * w), int(lm.y * h), int(lm.z * w)
                    mylmList.append([px, py, pz])
                    xList.append(px)
                    yList.append(py)

                ## bbox 计算面积并给出矩形框
                xmin, xmax = min(xList), max(xList)  # 取最大数值
                ymin, ymax = min(yList), max(yList)
                boxW, boxH = xmax - xmin, ymax - ymin
                bbox = xmin, ymin, boxW, boxH
                cx, cy = bbox[0] + (bbox[2] // 2), \
                         bbox[1] + (bbox[3] // 2)

                #我的手
                myHand["lmList"] = mylmList
                myHand["bbox"] = bbox
                myHand["center"] = (cx, cy)

                #手指类型，需要思考
                if flipType:
                    if handType.classification[0].label == "Right":
                        myHand["type"] = "Left"
                    else:
                        myHand["type"] = "Right"
                else:
                    myHand["type"] = handType.classification[0].label
                allHands.append(myHand)

                ## draw画手
                if draw:
                    self.mpDraw.draw_landmarks(img, handLms,
                                               self.mpHands.HAND_CONNECTIONS)
                    cv2.rectangle(img, (bbox[0] - 20, bbox[1] - 20),
                                  (bbox[0] + bbox[2] + 20, bbox[1] + bbox[3] + 20),
                                  (101, 67, 254), 2)
                    cv2.putText(img, myHand["type"], (bbox[0] - 30, bbox[1] - 30), cv2.FONT_HERSHEY_PLAIN,
                                4, (106, 68, 20), 3)
        if draw:
            return allHands, img
        else:
            return allHands

    #手指的状态
    def fingersUp(self, myHand):
        """
        Finds how many fingers are open and returns in a list.
        Considers left and right hands separately
        :return: List of which fingers are up
        """
        myHandType = myHand["type"]
        myLmList = myHand["lmList"]
        if self.results.multi_hand_landmarks:
            fingers = []
            # Thumb拇指，思考原理
            if myHandType == "Right":

                if myLmList[self.tipIds[0]][0] < myLmList[self.tipIds[0] - 1][0]:
                    fingers.append(1)
                else:
                    fingers.append(0)
            else:
                if myLmList[self.tipIds[0]][0] > myLmList[self.tipIds[0] - 1][0]:
                    fingers.append(1)
                else:
                    fingers.append(0)

            # 4 Fingers
            for id in range(1, 5):
                # 其他手指指尖的y坐标小于次指尖的点的坐标，则为竖直
                if myLmList[self.tipIds[id]][1] < myLmList[self.tipIds[id] - 2][1]:
                    fingers.append(1)
                else:
                    fingers.append(0)
        return fingers

    def findDistance(self, p1, p2, img=None):
        """
        计算指尖距离
        :param p1: Point1
        :param p2: Point2
        :param img: 要绘制的图
        :param draw: 标志变量
        :return: 返回指尖距离，和绘制好的图
        """

        x1, y1 = p1
        x2, y2 = p2
        cx, cy = (x1 + x2) // 2, (y1 + y2) // 2
        length = math.hypot(x2 - x1, y2 - y1)
        info = (x1, y1, x2, y2, cx, cy)
        if img is not None:
            cv2.circle(img, (x1, y1), 15, (113, 227, 250), cv2.FILLED)   # 食指尖画圈
            cv2.circle(img, (x2, y2), 15, (113, 227, 250), cv2.FILLED)   # 中指尖画圈
            cv2.line(img, (x1, y1), (x2, y2), (173, 202, 25), 3)
            cv2.circle(img, (cx, cy), 15, (108, 96, 244), cv2.FILLED)   # 两指中间画紫圈
            return length, info, img
        else:
            return length, info


def main():
    cap = cv2.VideoCapture(0)
    detector = HandDetector(detectionCon=0.8, maxHands=2)
    while True:
        # Get image frame
        success, img = cap.read()
        # Find the hand and its landmarks
        hands, img = detector.findHands(img)  # with draw
        # hands = detector.findHands(img, draw=False)  # without draw

        if hands:
            # Hand 1
            hand1 = hands[0]
            lmList1 = hand1["lmList"]  # List of 21 Landmark points
            bbox1 = hand1["bbox"]  # Bounding box info x,y,w,h
            centerPoint1 = hand1['center']  # center of the hand cx,cy
            handType1 = hand1["type"]  # Handtype Left or Right

            fingers1 = detector.fingersUp(hand1)

            if len(hands) == 2:
                # Hand 2
                hand2 = hands[1]
                lmList2 = hand2["lmList"]  # List of 21 Landmark points
                bbox2 = hand2["bbox"]  # Bounding box info x,y,w,h
                centerPoint2 = hand2['center']  # center of the hand cx,cy
                handType2 = hand2["type"]  # Hand Type "Left" or "Right"

                fingers2 = detector.fingersUp(hand2)

                # Find Distance between two Landmarks. Could be same hand or different hands
                length, info, img = detector.findDistance(lmList1[8][0:2], lmList2[8][0:2], img)  # with draw
                # length, info = detector.findDistance(lmList1[8], lmList2[8])  # with draw
        # Display
        cv2.imshow("Image", img)
        cv2.waitKey(1)


if __name__ == "__main__":
    main()

        
