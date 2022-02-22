# https://github.com/justinshenk/fer

pip install FER
from fer import FER
import matplotlib.pyplot as plt
%matplotlib inline

test_image_one = plt.imread("/emo/test_img1.jpg")

emo_detector = FER(mtcnn=True)

captured_emotions = emo_detector.detect_emotions(test_image_one)

print(captured_emotions)
plt.imshow(test_image_one)

dominant_emotion, emotion_score = emo_detector.top_emotion(test_image_one)
print(dominant_emotion, emotion_score)
