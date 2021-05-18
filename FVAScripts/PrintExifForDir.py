from exif import Image
import os
import sys

image_members = []

for filename in os.listdir(sys.argv[1]):
    fullPath = sys.argv[1] + "/" + filename
    exifInfo = Image(fullPath)
    if exifInfo.has_exif:
        print(f"Image {filename}")
        #print(f"Make: {exifInfo.make}")
        #print(f"Model: {exifInfo.model}")
        #print(f"Lens make: {exifInfo.get('lens_make', 'Unknown')}")
        #print(f"Lens model: {exifInfo.get('lens_model', 'Unknown')}")
        #print(f"Lens specification: {exifInfo.get('lens_specification', 'Unknown')}")
        #print(f"OS version: {exifInfo.get('software', 'Unknown')}")
        print(f"{exifInfo.datetime_original}.{exifInfo.subsec_time_original} {exifInfo.get('offset_time', '')}\n")
        #print(f"Latitude: {exifInfo.gps_latitude} {exifInfo.gps_latitude_ref}\n")
        #print(f"Longitude: {exifInfo.gps_longitude} {exifInfo.gps_longitude_ref}\n")
        image_members.append(fullPath)
        for index, image_member_list in enumerate(image_members):
            print(f"Image {index} contains {len(image_member_list)} members:")
            print(f"{image_member_list}\n")
    else:
        print(f"Image {filename} does not contain any EXIF information.\n")