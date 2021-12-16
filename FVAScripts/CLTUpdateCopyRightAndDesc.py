hotel_image.image_description = "The Dolphin Hotel in Orlando, viewed at sunset from the Swan Hotel"
hotel_image.copyright = "Copyright 2021 (Your name here)"

print(f"Description: {hotel_image.image_description}")
print(f"Copyright: {hotel_image.copyright}")
with open('./images/hotel updated.jpg', 'wb') as updated_hotel_file:
    updated_hotel_file.write(hotel_image.get_file())
