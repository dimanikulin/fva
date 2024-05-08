import unittest
from unittest.mock import patch
from io import StringIO
import sys
import os

from exif import Image
from PrintExifForDir import print_exif_info_for_dir

class PrintExifForDirTests(unittest.TestCase):
    @patch('builtins.print')
    @patch('os.listdir')
    @patch('exif.Image')
    def test_print_exif_info_for_dir_with_exif(self, mock_Image, mock_listdir, mock_print):
        # Arrange
        mock_listdir.return_value = ['image1.jpg', 'image2.jpg']
        mock_Image.return_value.has_exif = True
        mock_Image.return_value.datetime_original = '2022-01-01'
        mock_Image.return_value.subsec_time_original = '123456'
        mock_Image.return_value.get.return_value = 'Offset Time'
        expected_output = [
            "Image image1.jpg",
            "2022-01-01.123456 Offset Time\n",
            "Image image2.jpg",
            "2022-01-01.123456 Offset Time\n"
        ]

        # Act
        print_exif_info_for_dir('/path/to/directory')

        # Assert
        calls = [mock.call(output) for output in expected_output]
        mock_print.assert_has_calls(calls)

    @patch('builtins.print')
    @patch('os.listdir')
    @patch('exif.Image')
    def test_print_exif_info_for_dir_without_exif(self, mock_Image, mock_listdir, mock_print):
        # Arrange
        mock_listdir.return_value = ['image1.jpg', 'image2.jpg']
        mock_Image.return_value.has_exif = False
        expected_output = [
            "Image image1.jpg does not contain any EXIF information.\n",
            "Image image2.jpg does not contain any EXIF information.\n"
        ]

        # Act
        print_exif_info_for_dir('/path/to/directory')

        # Assert
        calls = [mock.call(output) for output in expected_output]
        mock_print.assert_has_calls(calls)

if __name__ == '__main__':
    unittest.main()