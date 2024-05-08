import unittest
from unittest.mock import patch
import sys
from io import StringIO

from CLTMerge2csv import merge_files

class CLTMerge2csvTests(unittest.TestCase):
    @patch('builtins.open')
    def test_merge_files(self, mock_open):
        # Arrange
        main_file_path = "main_file.csv"
        merge_file_path = "merge_file.csv"
        main_file = StringIO()
        merge_file = StringIO()
        main_file.write("Main File Content")
        merge_file.write("Merge File Content")
        main_file.seek(0)
        merge_file.seek(0)
        mock_open.side_effect = [main_file, merge_file]

        # Act
        merge_files(main_file_path, merge_file_path)

        # Assert
        main_file.seek(0)
        merged_content = main_file.read()
        self.assertIn("Main File Content", merged_content)
        self.assertIn("Merge File Content", merged_content)

    @patch('builtins.open')
    def test_merge_files_empty_merge_file(self, mock_open):
        # Arrange
        main_file_path = "main_file.csv"
        merge_file_path = "merge_file.csv"
        main_file = StringIO()
        merge_file = StringIO()
        main_file.write("Main File Content")
        main_file.seek(0)
        merge_file.seek(0)
        mock_open.side_effect = [main_file, merge_file]

        # Act
        merge_files(main_file_path, merge_file_path)

        # Assert
        main_file.seek(0)
        merged_content = main_file.read()
        self.assertEqual(merged_content, "Main File Content")

if __name__ == '__main__':
    unittest.main()