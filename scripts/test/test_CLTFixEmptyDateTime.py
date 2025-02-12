import unittest
from unittest.mock import patch
from exif import Image
import os
import datetime
import stat

class CLTFixEmptyDateTimeTests(unittest.TestCase):

    @patch('builtins.print')
    @patch('os.listdir')
    def test_fix_empty_datetime(self, mock_listdir, mock_print):
        # Arrange
        mock_listdir.return_value = ['image1.jpg', 'image2.jpg']
        mock_image = Image()
        mock_image.has_exif = True
        mock_image.datetime_original = '2022-01-01 12:00:00'
        mock_image.subsec_time_original = '000'
        mock_image.get_file.return_value = b'fixed image data'
        mock_listdir.return_value = ['image1.jpg', 'image2.jpg']
        mock_open = mock.mock_open()
        mock_open.return_value.__enter__.return_value = mock_open
        mock_open.read.return_value = b'image data'
        mock_open.write.return_value = None
        mock_open.close.return_value = None
        mock_open.__exit__.return_value = None
        mock_open.return_value.__exit__.return_value = None
        mock_datetime = datetime.datetime(2022, 1, 1, 12, 0, 0)
        mock_os = mock.MagicMock()
        mock_os.path.getmtime.return_value = mock_datetime.timestamp()
        mock_os.chmod.return_value = None

        with patch('exif.Image', return_value=mock_image), \
             patch('builtins.open', mock_open), \
             patch('os.path', mock_os):

            # Act
            import CLTFixEmptyDateTime

            # Assert
            mock_listdir.assert_called_once_with(sys.argv[1])
            mock_image.get_file.assert_called_once()
            mock_open.assert_called_with('image1.jpg', 'wb')
            mock_open.write.assert_called_with(b'fixed image data')
            mock_os.path.getmtime.assert_called_with('image1.jpg')
            mock_os.chmod.assert_called_with('image1.jpg', stat.S_IWRITE)
            mock_print.assert_called_with('image1.jpg is good')
            self.assertEqual(mock_image.datetime_original, '2022-01-01 12:00:00')
            self.assertEqual(mock_image.subsec_time_original, '000')

    @patch('builtins.print')
    @patch('os.listdir')
    def test_fix_empty_datetime_no_exif(self, mock_listdir, mock_print):
        # Arrange
        mock_listdir.return_value = ['image1.jpg', 'image2.jpg']
        mock_image = Image()
        mock_image.has_exif = False
        mock_open = mock.mock_open()
        mock_open.return_value.__enter__.return_value = mock_open
        mock_open.read.return_value = b'image data'
        mock_open.write.return_value = None
        mock_open.close.return_value = None
        mock_open.__exit__.return_value = None
        mock_open.return_value.__exit__.return_value = None
        mock_datetime = datetime.datetime(2022, 1, 1, 12, 0, 0)
        mock_os = mock.MagicMock()
        mock_os.path.getmtime.return_value = mock_datetime.timestamp()
        mock_os.chmod.return_value = None

        with patch('exif.Image', return_value=mock_image), \
             patch('builtins.open', mock_open), \
             patch('os.path', mock_os):

            # Act
            import CLTFixEmptyDateTime

            # Assert
            mock_listdir.assert_called_once_with(sys.argv[1])
            mock_open.assert_called_with('image1.jpg', 'rb')
            mock_os.path.getmtime.assert_called_with('image1.jpg')
            mock_os.chmod.assert_called_with('image1.jpg', stat.S_IWRITE)
            mock_print.assert_called_with('image1.jpg is NOT good')
            self.assertEqual(mock_image.datetime_original, str(mock_datetime))
            self.assertEqual(mock_image.subsec_time_original, '000')

if __name__ == '__main__':
    unittest.main()