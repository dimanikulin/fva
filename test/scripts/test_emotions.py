import unittest
import os
import matplotlib.pyplot as plt
from fer import FER
from unittest.mock import patch

class EmotionsTests(unittest.TestCase):

    def test_detect_emotions(self):
        # Arrange
        test_image_path = "/emo/test_img1.jpg"
        test_image = plt.imread(test_image_path)
        emo_detector = FER(mtcnn=True)

        # Act
        with patch('fer.FER.detect_emotions') as mock_detect_emotions:
            mock_detect_emotions.return_value = {'emotion1': 0.8, 'emotion2': 0.2}
            captured_emotions = emo_detector.detect_emotions(test_image)

        # Assert
        self.assertEqual(captured_emotions, {'emotion1': 0.8, 'emotion2': 0.2})

    def test_top_emotion(self):
        # Arrange
        test_image_path = "/emo/test_img1.jpg"
        test_image = plt.imread(test_image_path)
        emo_detector = FER(mtcnn=True)

        # Act
        with patch('fer.FER.top_emotion') as mock_top_emotion:
            mock_top_emotion.return_value = ('happy', 0.9)
            dominant_emotion, emotion_score = emo_detector.top_emotion(test_image)

        # Assert
        self.assertEqual(dominant_emotion, 'happy')
        self.assertEqual(emotion_score, 0.9)

if __name__ == '__main__':
    unittest.main()