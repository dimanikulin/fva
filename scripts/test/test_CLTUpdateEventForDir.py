import unittest
import os
import csv
from CLTUpdateEventForDir import update_event_for_dir

class TestCLTUpdateEventForDir(unittest.TestCase):
    def setUp(self):
        # Create a test directory with some files
        self.test_dir = 'test_dir'
        os.mkdir(self.test_dir)
        self.files = ['file1.txt', 'file2.txt', 'file3.txt']
        for file in self.files:
            open(os.path.join(self.test_dir, file), 'w').close()

        # Create a test CSV file with some records
        self.test_csv = 'test_csv.csv'
        with open(self.test_csv, 'w', newline='', encoding='utf-8') as csvfile:
            writer = csv.writer(csvfile, delimiter=',', quotechar='|')
            writer.writerow(['ID', 'Filename', 'EventID'])
            writer.writerow(['1', 'file1.txt', '100'])
            writer.writerow(['2', 'file2.txt', '200'])
            writer.writerow(['3', 'file3.txt', '300'])

    def tearDown(self):
        # Remove the test directory and files
        for file in self.files:
            os.remove(os.path.join(self.test_dir, file))
        os.rmdir(self.test_dir)

        # Remove the test CSV file
        os.remove(self.test_csv)

    def test_update_event_for_dir(self):
        # Arrange
        input_file = self.test_csv
        input_dir = self.test_dir
        new_event_id = '400'

        # Act
        update_event_for_dir(input_file, input_dir, new_event_id)

        # Assert
        expected_output = [
            ['ID', 'Filename', 'EventID'],
            ['1', 'file1.txt', '100'],
            ['2', 'file2.txt', '400'],
            ['3', 'file3.txt', '300']
        ]
        with open(input_file, newline='', encoding='utf-8') as csvfile:
            reader = csv.reader(csvfile, delimiter=',', quotechar='|')
            actual_output = [row for row in reader]

        self.assertEqual(actual_output, expected_output)

if __name__ == '__main__':
    unittest.main()