import unittest
import os
import csv
from io import StringIO
from unittest.mock import patch
from CLTUpdatePlaceForDir import update_place_for_dir

class CLTUpdatePlaceForDirTests(unittest.TestCase):

    def setUp(self):
        # Create a temporary test directory
        self.test_dir = 'test_dir'
        os.makedirs(self.test_dir)

        # Create a temporary test file
        self.test_file = os.path.join(self.test_dir, 'test_file.csv')
        with open(self.test_file, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow(['1', 'file1.txt', 'place1'])
            writer.writerow(['2', 'file2.txt', 'place2'])
            writer.writerow(['3', 'file3.txt', 'place3'])

    def tearDown(self):
        # Remove the temporary test directory and file
        os.remove(self.test_file)
        os.rmdir(self.test_dir)

    def test_update_place_for_dir(self):
        # Arrange
        sys_argv = ['CLTUpdatePlaceForDir.py', self.test_file, self.test_dir, 'new_place']
        expected_output = '1,file1.txt,new_place\n2,file2.txt,new_place\n3,file3.txt,new_place\n'

        # Act
        with patch('sys.argv', sys_argv):
            with patch('sys.stdout', new=StringIO()) as fake_out:
                update_place_for_dir()

        # Assert
        self.assertEqual(fake_out.getvalue(), expected_output)

    def test_update_place_for_dir_no_changes(self):
        # Arrange
        sys_argv = ['CLTUpdatePlaceForDir.py', self.test_file, self.test_dir, 'new_place']
        expected_output = '1,file1.txt,place1\n2,file2.txt,place2\n3,file3.txt,place3\n'

        # Act
        with patch('sys.argv', sys_argv):
            with patch('sys.stdout', new=StringIO()) as fake_out:
                update_place_for_dir()

        # Assert
        self.assertEqual(fake_out.getvalue(), expected_output)

if __name__ == '__main__':
    unittest.main()