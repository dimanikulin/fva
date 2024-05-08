import unittest
import subprocess
import os
import stat
from unittest.mock import patch
from io import StringIO
import sys
import csv
import win32com.client

class TestFixNoGeoIssues(unittest.TestCase):

    def test_fix_no_geo_issues(self):
        # Mock the csv file data
        csv_data = [
            ['FVA_ERROR_NO_GEO', 'file1.jpg', 'place1'],
            ['FVA_ERROR_NO_GEO', 'file2.jpg', 'place2'],
            ['FVA_ERROR_OTHER', 'file3.jpg', 'place3']
        ]
        with patch('builtins.open', return_value=StringIO('\n'.join([','.join(row) for row in csv_data]))) as mock_csvfile:
            # Mock the fvaPlaces.csv file data
            fva_places_data = [
                ['place1', 'lat1', 'lon1', 'ref1', 'alt1', 'altref1'],
                ['place2', 'lat2', 'lon2', 'ref2', 'alt2', 'altref2'],
                ['place3', 'lat3', 'lon3', 'ref3', 'alt3', 'altref3']
            ]
            with patch('builtins.open', return_value=StringIO('\n'.join([','.join(row) for row in fva_places_data]))) as mock_fva_places_csvfile:
                # Mock the subprocess.call function
                with patch('subprocess.call') as mock_subprocess_call:
                    # Mock the os.chmod function
                    with patch('os.chmod') as mock_os_chmod:
                        # Mock the print function
                        with patch('builtins.print') as mock_print:
                            # Mock the win32com.client.Dispatch function
                            with patch('win32com.client.Dispatch') as mock_dispatch:
                                # Mock the shell.SendKeys function
                                with patch.object(win32com.client.Dispatch, 'SendKeys') as mock_sendkeys:
                                    # Set the command line arguments
                                    sys.argv = ['fixNoGeoIssues.py', 'place1']
                                    # Run the script
                                    exec(open('fixNoGeoIssues.py').read())
                                    
                                    # Assert that the csv files were opened correctly
                                    mock_csvfile.assert_called_once_with('issues3.csv', newline='', encoding='utf-8')
                                    mock_fva_places_csvfile.assert_called_once_with('../../#data#/fvaPlaces.csv', newline='', encoding='utf-8')
                                    
                                    # Assert that the subprocess.call function was called with the correct arguments
                                    mock_subprocess_call.assert_called_once_with(['../exiftool(-k).exe', '-GPSLatitude=lat1', '-GPSLongitude=lon1', '-GPSLatitudeRef=ref1', '-GPSLongitudeRef=altref1', '-GPSAltitude=0 -GPSAltitudeRef=0', 'file1.jpg'])
                                    
                                    # Assert that the os.chmod function was called with the correct arguments
                                    mock_os_chmod.assert_called_once_with('file1.jpg', stat.S_IWRITE)
                                    
                                    # Assert that the print function was called with the correct arguments
                                    mock_print.assert_called_once_with('file1.jpg, lat1, lon1')
                                    
                                    # Assert that the win32com.client.Dispatch function was called with the correct argument
                                    mock_dispatch.assert_called_once_with("WScript.Shell")
                                    
                                    # Assert that the shell.SendKeys function was called with the correct argument
                                    mock_sendkeys.assert_called_once_with("{ENTER}", 0)