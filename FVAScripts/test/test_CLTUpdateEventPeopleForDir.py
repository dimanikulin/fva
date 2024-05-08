import unittest
from unittest.mock import patch
import os
import sys
import csv

class CLTUpdateEventPeopleForDirTests(unittest.TestCase):

    @patch('os.rename')
    @patch('open')
    @patch('csv.reader')
    @patch('os.listdir')
    @patch('os.remove')
    def test_update_event_people_for_dir(self, mock_remove, mock_listdir, mock_reader, mock_open, mock_rename):
        # Arrange
        sys.argv = ['', 'fvaFile.csv', 'input_dir', 'new_people_ids']
        mock_rename.return_value = None
        mock_open.return_value.__enter__.return_value = mock_open
        mock_open.write.return_value = None
        mock_open.close.return_value = None
        mock_open.__exit__.return_value = None
        mock_open.return_value.__exit__.return_value = None
        mock_listdir.return_value = ['file1.jpg', 'file2.jpg']
        mock_reader.return_value = [['record1'], ['record2']]

        # Act
        import CLTUpdateEventPeopleForDir

        # Assert
        mock_rename.assert_called_with('fvaFile.csv', 'fvaFile.csv_np')
        mock_open.assert_called_with('fvaFile.csv', 'a', newline='\n', encoding='utf-8')
        mock_reader.assert_called_with(mock_open, delimiter=',', quotechar='|')
        mock_listdir.assert_called_with('input_dir')
        mock_open.write.assert_called_with('record1\n')
        mock_open.write.assert_called_with('record2\n')
        mock_remove.assert_called_with('fvaFile.csv_np')

if __name__ == '__main__':
    unittest.main()